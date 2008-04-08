#include "TPM.hh"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <tss/tss_error_basics.h>

using namespace std;
using namespace microtss;

TPM::TPM( const TSS_HCONTEXT &contextHandle ) :
  myState( DisabledDeactivatedNoOwner ),
  myCountersCount( 0 ),
  myContextHandle( contextHandle ),
  myHasMaintenance( false ),
  myHasEndorsementKey( true ),
  myUserCreatedEndorsement( false ),
  myEKRestricted( false ),
  myNumberOfPCR( 0 )
{
	/**
	 * @brief					retrieves the TPM object of a context. Only one instance of this object exists 
	 * 							for a given context and implicitly represents a TPM owner.
	 * @param myContextHandle	Handle of the context object
	 * @param myTpmHandle		Receives the handle of the TPM object bound to the context.
	 * @return					TSS_SUCCESS, TSS_E_INVALID_HANDLE, 
	 * 							TSS_E_BAD_PARAMETER, TSS_E_INTERNAL_ERROR
	 */

	tssResultHandler( Tspi_Context_GetTpmObject( myContextHandle, &myTpmHandle ),
		 					"Error at get TPM object" );
 
	readVersion();
  readVersionVal();
	readVendorName();
	readNumberOfPCR();
	readKeyLoadCount();
	readCountersCount();
  readState();
  readPCRValues();
	checkEndorsementKey();
	userCreatedEndorsement();
	//readMaintenanceState();
};

TPM::~TPM()
{
	Tspi_Context_FreeMemory( myContextHandle, 0 );
	Tspi_Context_Close( myContextHandle );
} 

UINT32 TPM::readCapabilities( TSS_FLAG capArea, UINT32 subCap, BYTE *& data )
{
	UINT32 len = 0;
	
		/**
		 * @brief					TSS Tspi function, provide the capabilities of the TPM.
		 * @param hTPM				Handle of the TPM object
		 * @param capArea	   		Flag indicating the attribute to query.
		 * 							TSS_TPMCAP_VERSION: Returns the TSS_VERSION structure that 
		 * 							identifies the version of the TPM.
		 * @param ulSubCapLength		The length (in bytes) of the rgbSubCap parameter.
		 * @param rgbSubCap		    Data indicating the attribute to query.
		 * @param pulRespDataLength Receives the length (in bytes) of the prgbRespData parameter.
		 * @param prgbRespData		Receives pointer to the actual data of the specified attribute.
		 * @return					TSS_SUCCESS, TSS_E_INVALID_HANDLE, 
		 * 							TSS_E_BAD_PARAMETER, TSS_E_INTERNAL_ERROR
		 */
	tssResultHandler( Tspi_TPM_GetCapability( myTpmHandle,
										capArea,
										sizeof(UINT32),
										(BYTE*) &subCap,
										&len,
										&data ), "Error at get capabilities!" );


   return len;
}

bool TPM::getStatusFlag( TSS_FLAG statusFlag )
{
	TSS_BOOL    state;
	
	/**
	 * @brief 					This method queries the TPM status
	 * @param hTPM 			Handle of the TPM object.
	 * @param statusFlag		Flag indicating the status to retrieve.
	 * @param pfTpmState		The value referenced by pfTpmState contains the queried status value.
	 */
	tssResultHandler( Tspi_TPM_GetStatus(myTpmHandle,
										statusFlag,
										&state), "Error at read Statusflag! "  );
	
	return state;
}

void TPM::setStatusFlag( TSS_FLAG statusFlag, bool state )
{
	/**
	 * @brief 					This method queries the TPM status
	 * @param hTPM 			Handle of the TPM object.
	 * @param statusFlag		Flag indicating the status to retrieve.
	 * @param pfTpmState		The value referenced by pfTpmState contains the queried status value.
	 */
	tssResultHandler( Tspi_TPM_SetStatus(myTpmHandle,
										statusFlag,
										(TSS_BOOL)state), "setStatusFlag");

}

void TPM::readVersion()
{
	TSS_VERSION *version;
	ostringstream sVersion;

	readCapabilities( TSS_TPMCAP_VERSION, (BYTE*&) version );
	
	sVersion << (unsigned int) version->bMajor << ".";
	
	if (version->bMinor == 0x01)
		sVersion << "1b" << ends;
	else
		sVersion << (unsigned int) version->bMinor << ends;
	
	myVersion = sVersion.str();

	ostringstream sRevision( myRevision );
	
	sRevision << (unsigned int) version->bRevMajor << "." << (unsigned int) version->bRevMinor << ends;
	myRevision = sRevision.str();

	Tspi_Context_FreeMemory( myContextHandle, (BYTE*)version );
}

void TPM::readVersionVal()
{
	TPM_CAP_VERSION_INFO *versionInfo;
	ostringstream sVersion;

	readCapabilities( TSS_TPMCAP_VERSION_VAL, (BYTE*&) versionInfo );
	sVersion << (unsigned int) versionInfo->version.major << "." << (unsigned int) versionInfo->version.minor;

  myVersion = sVersion.str();

  ostringstream sRevision( myRevision );
  sRevision << (unsigned int) versionInfo->version.revMajor << "." << (unsigned int) versionInfo->version.revMinor << ends;
  myRevision = sRevision.str();

  Tspi_Context_FreeMemory( myContextHandle, (BYTE*)versionInfo );
}

void TPM::readVendorName()
{
	BYTE *name;

	UINT32 len = readCapabilities( TSS_TPMCAP_PROPERTY, TSS_TPMCAP_PROP_MANUFACTURER, name );
		
	myVendor.assign( (char*) name, len);	
	Tspi_Context_FreeMemory( myContextHandle, name );
}

void TPM::readState()
{
   UINT32 pcrValLen;
   BYTE* pcrVal;
   try {
         tssResultHandler( Tspi_TPM_PcrRead(myTpmHandle, 0, &pcrValLen, &pcrVal),
                "Read PCR value!" );
         myState = (TpmState)( myState | ENABLED_MASK ); 
   } catch ( IsDisabledError & e ) {
      /// is disabled
      myState = (TpmState) ( myState & ~ENABLED_MASK ); 
   }  catch ( IsDeactivatedError & e) {
      /// is deactivated
      myState = (TpmState) ( myState & ~ACTIVATED_MASK ); 
   } catch ( exception & e ){
   }

   myState = (TpmState)( myState | ACTIVATED_MASK ); 

   UINT32 len;
   TSS_BOOL *owner;
   len = readCapabilities(TSS_TPMCAP_PROPERTY, TSS_TPMCAP_PROP_OWNER, (BYTE*&) owner );
   if ( len == 0 ) {
      std::cerr << "Error at read Capabilities. " << endl; 
   }
   else if ( *owner == true ){
         /// TPM has Owner
         myState = (TpmState) ( myState | OWNER_MASK );  
         }
        else{
         /// TPM has no Owner
         myState = (TpmState) ( myState & ~OWNER_MASK );  
         }

   Tspi_Context_FreeMemory( myContextHandle, (BYTE*&) owner );
}

void TPM::readMaintenanceState()
{
	try {
	myHasMaintenance = getStatusFlag( TSS_TPMSTATUS_ALLOWMAINTENANCE );
	} catch ( AuthenticationFailure &e )
	{ 
		myHasMaintenance = false; 
	} catch ( IsDisabledError &e )
	{} catch ( NoSRKError &e )
	{} catch ( exception &e )
	{}
}

void TPM::readNumberOfPCR()
{
	UINT32 len;
	BYTE *count;

	len = readCapabilities( TSS_TPMCAP_PROPERTY, TSS_TPMCAP_PROP_PCR, count);

	
	if (len == 0)
		std::cerr << "Error at read Capabilities" << std::endl; 

	myNumberOfPCR = (UINT32)*count;
}

void TPM::readKeyLoadCount()
{
	UINT32 len;
	BYTE *count;

	len = readCapabilities( TSS_TPMCAP_PROPERTY, TSS_TPMCAP_PROP_SLOTS, count );
	
	if (len == 0)
		std::cerr << "Error at read Capabilities" << std::endl; 

	myKeyLoadCount = (UINT32)*count;
}

void TPM::readCountersCount()
{
//	UINT32 len;
//	BYTE *count=0;

	//len = readCapabilities( TSS_TPMCAP_PROPERTY, TSS_TPMCAP_PROP_COUNTERS, count );
	
	//if (len == 0)
		//std::cerr << "Error at read Capabilities" << std::endl; 

	// myCountersCount = (UINT32)*count;
}

const string TPM::getVersion() const
{
	return myVersion;
}

const string TPM::getRevision() const
{
	return myRevision;
}

const string TPM::getVendorName() const
{
	return myVendor;
}

const TpmState TPM::getState() const
{
	return myState;
}

/*const bool TPM::getPersistenceDeactivated() const
{
	return myPDeactivated;
}

const bool TPM::getVolatileDeactivated() const
{
	return myVDeactivated;
}*/

UINT32 TPM::getNumberOfPCR() const
{
	return myNumberOfPCR;
}

UINT32 TPM::getKeyLoadCount() const
{
	return myKeyLoadCount;
}

UINT32 TPM::getCountersCount() const
{
	return myCountersCount;
}

const vector<ByteString> TPM::getPCRValues() const
{
	return myPCRValues;	
}

void TPM::readPCRValues()
{
  if ( myState != EnabledActivatedNoOwner && myState != EnabledActivatedOwner )
		return;
  UINT32 pcrValLen;
	BYTE*	  pcrVal;
	ByteString temp;

	myPCRValues.resize(myNumberOfPCR);

	for(size_t i = 0; i<myNumberOfPCR; ++i)
	{
		tssResultHandler( Tspi_TPM_PcrRead(myTpmHandle, i, &pcrValLen, &pcrVal),
		 						"Read PCR value!" );
		temp.resize(pcrValLen);
		for (size_t j=0; j<pcrValLen; ++j) {
			temp[j] = pcrVal[j];
		}
		myPCRValues[i] = temp;
		Tspi_Context_FreeMemory( myContextHandle, pcrVal );
	}
}

void TPM::checkEndorsementKey()
{
	TSS_HKEY		 myEndorsementPublicKeyHandle; 

	TSS_RESULT result = Tspi_TPM_GetPubEndorsementKey( myTpmHandle, FALSE, NULL, &myEndorsementPublicKeyHandle );
	// If Endorsement key could be read, then set myHasEndorsement key  true.
	if ( result == TSS_SUCCESS )
		myHasEndorsementKey = true;
	else if ( result == TCPA_E_DISABLED_CMD )
		{
			myHasEndorsementKey = true;
			myEKRestricted = true;
		}
}

void TPM::userCreatedEndorsement()
{
	/*tssResultHandler( getStatusFlag( TSS_TPMSTATUS_CEKP_USED ),
							"check if is user created Endorsement key" );
	myUserCreatedEndorsement = true;*/
}

void TPM::takeOwnership( const std::string &ownerPwd, const std::string &srkPwd, bool wellknownsecret )
{
	BYTE wellKnownSecretValue[] = TSS_WELL_KNOWN_SECRET;

	if ( hasOwner() )
		return; // throw...

	if ( isDisabled() )
		return; // throw...

	// Set owner password
	TSS_HPOLICY tpmPolicy;

	tssResultHandler( Tspi_GetPolicyObject( myTpmHandle, TSS_POLICY_USAGE, &tpmPolicy ),
							"Getting the TPM policy handle failed!" );

	tssResultHandler( Tspi_Policy_SetSecret( tpmPolicy, TSS_SECRET_MODE_PLAIN, ownerPwd.length(), 
										(BYTE *)ownerPwd.c_str() ),
							"Setting owner policy failed!" );
	
	// Create SRK handle and set SRK password.
	TSS_HOBJECT srkHandle;
	TSS_HPOLICY srkPolicy;

	tssResultHandler( Tspi_Context_CreateObject( myContextHandle, TSS_OBJECT_TYPE_RSAKEY, TSS_KEY_TSP_SRK, &srkHandle ), 
							"Creating SRK object failed!" );

	tssResultHandler( Tspi_GetPolicyObject( srkHandle, TSS_POLICY_USAGE, &srkPolicy ),
						"Getting SRK policy handle failed" );

	if ( wellknownsecret )
		tssResultHandler( Tspi_Policy_SetSecret( srkPolicy, TSS_SECRET_MODE_SHA1, 20, wellKnownSecretValue ), "Setting SRK policy failed!" );
	else
		tssResultHandler( Tspi_Policy_SetSecret( srkPolicy, TSS_SECRET_MODE_PLAIN, srkPwd.length(), (BYTE *)srkPwd.c_str() ), "Setting SRK policy failed!" );

	tssResultHandler( Tspi_TPM_TakeOwnership( myTpmHandle, srkHandle, 0 ), 
							"Taking ownership failed!" );

	myState = (TpmState) (myState | OWNER_MASK);
}

void TPM::changeOwnerPassword( const string &oldOwnerPwd, const string &newOwnerPwd )
{
	if ( !hasOwner() )
		throw( NoSRKError( "Change ownership failed!" ) );
	else if ( isDisabled()  )
		throw( IsDisabledError( "Change ownership failed!" ));
	else if ( isDeactivated() )
		throw( IsDeactivatedError( "Change ownership failed!" ));

	setSecret( oldOwnerPwd );
  cout << "set old password passed" << endl;
	TSS_HPOLICY newPolicy;

	tssResultHandler( Tspi_Context_CreateObject( myContextHandle, TSS_OBJECT_TYPE_POLICY, TSS_POLICY_USAGE, &newPolicy ),
				         "changeOwnerPassword: Context_CreateObject" );

	tssResultHandler( Tspi_Policy_SetSecret( newPolicy, TSS_SECRET_MODE_PLAIN, newOwnerPwd.length(), 
										 (BYTE *)newOwnerPwd.c_str() ),
							"Setting owner policy failed!" );

	tssResultHandler( Tspi_ChangeAuth( myTpmHandle, 0, newPolicy ),
	                  "Change owner password failed!" );
}

void TPM::clearOwnership( const string &password )
{
	if ( !hasOwner() )
		throw( NoSRKError( "Clear ownership failed!" ) );
	else if ( isDisabled()  )
		throw( IsDisabledError( "Clear ownership failed!" ));
	else if ( isDeactivated() )
		throw( IsDeactivatedError( "Clear ownership failed!" ));

	setSecret( password );
	
	tssResultHandler( Tspi_TPM_ClearOwner( myTpmHandle, false ), "Clear ownership failed!" );
	
   myState = DisabledDeactivatedNoOwner;
}

/// @todo implementierung zu einer methode zusammenfassen
///       und zwei inline-Methoden machen.
void TPM::setEnabled( const string &password )
{
	if ( isEnabled() )
		return;

	setSecret( password );

	setStatusFlag( TSS_TPMSTATUS_OWNERSETDISABLE, false ); // try ... catch
	
	readState();
}

void TPM::setDisabled( const string &password )
{
	if ( isDisabled() )
		return;

	setSecret( password );

	setStatusFlag( TSS_TPMSTATUS_OWNERSETDISABLE, true ); 

	myState = (TpmState) ( myState & ~ENABLED_MASK );
}

void TPM::setSecret( const string &password )
{
	TSS_HPOLICY policy;
	if ( Tspi_GetPolicyObject(myTpmHandle, TSS_POLICY_USAGE, &policy) != TSS_SUCCESS )
		std::cerr << "Get Policy failed!" << std::endl;

	if ( Tspi_Policy_SetSecret( policy, TSS_SECRET_MODE_PLAIN, password.length(), (BYTE*)password.c_str() ) != TSS_SUCCESS )
		std::cerr << "Set secret failed!" << std::endl;
}

const string TPM::selfTestFull() const
{
	UINT32 testResultLen;
	BYTE   *testResult;
	ostringstream result;

	if ( Tspi_TPM_SelfTestFull( myTpmHandle ) != TSS_SUCCESS )
		std::cerr << "TPM selftest failed!" << std::endl;
	else
	{
		Tspi_TPM_GetTestResult( myTpmHandle, &testResultLen,  &testResult );
		
		result << "0x";
		for (size_t i=0; i < testResultLen; ++i )
		  result << hex << (int) testResult[i];
		result << ends;
	}

	return result.str();
}

void TPM::setTempDeactivated()
{
	if (Tspi_TPM_SetStatus( myTpmHandle, TSS_TPMSTATUS_SETTEMPDEACTIVATED, true ) != TSS_SUCCESS )
	{
		///throw exception
		cerr << "Set TPM status to temporarily Deactivated failed!" << endl;
	}
	myState = (TpmState) (myState & ~ACTIVATED_MASK);
}

microtss::PublicKey TPM::getEndorsementPublicKey( const std::string &password )
{
	TSS_HKEY		 myEndorsementPublicKeyHandle; 
	ostringstream  attributes;
	bool ownerauth = FALSE;
	setSecret( password );
	
	if ( password != "" ) ownerauth = TRUE;
	TSS_RESULT result = Tspi_TPM_GetPubEndorsementKey( myTpmHandle, ownerauth, NULL, &myEndorsementPublicKeyHandle );
	if ( result != TSS_SUCCESS && result != TCPA_E_DISABLED_CMD )
		tssResultHandler( result, "Get public Endorsement Key failed!" );
	
	return microtss::PublicKey( myEndorsementPublicKeyHandle );
}

void TPM::restrictEK( string password )
{
	if (isEndorsementKeyResticted())
		return;

	setSecret( password );
	setStatusFlag( TSS_TPMSTATUS_DISABLEPUBEKREAD, true );
	myEKRestricted = true;
}

void TPM::killMaintenance( string password )
{
	setSecret( password );
	TSS_RESULT result = Tspi_TPM_KillMaintenanceFeature( myTpmHandle );
	if ( TSS_SUCCESS != result )
		tssResultHandler( result, "Kill Maintenance Archive failed!" );
	
}

void TPM::tssResultHandler( TSS_RESULT result, const string &str ) const
{
	if ( result == TSS_SUCCESS )
		return;

//	cout << "TSS Error Layer: " << TSS_ERROR_LAYER( result ) << endl;

	switch( Trspi_Error_Code( result ) )
	{
		case TCPA_E_AUTHFAIL:
			throw AuthenticationFailure( str );
         break;
      case TCPA_E_NOSRK:
			throw NoSRKError( str );
			break;
		case TCPA_E_DEACTIVATED:
			throw IsDeactivatedError( str );
			break; 
		case TCPA_E_DISABLED:
			throw IsDisabledError( str );
			break;
      default:
			cerr << "TPM::throwException: Unknown Exception(Error Code: " << Trspi_Error_Code( result ) << "- " << Trspi_Error_String( result ) << ") : " + str << endl;
         break;
	}
}

TPMError::TPMError( const std::string &str ) :
  runtime_error( "TPM Error: " + str ) 
{

}

AuthenticationFailure::AuthenticationFailure( const std::string &str ) :
  TPMError( "Wrong owner password. " + str )
{}

NoSRKError::NoSRKError( const std::string &str ) :
  TPMError( "No SRK. " + str ) 
{

}

IsDisabledError::IsDisabledError( const std::string &str ) :
  TPMError( "TPM is disabled. " + str ) 
{

}

IsDeactivatedError::IsDeactivatedError( const std::string &str ) :
  TPMError( "TPM is deactivated. " + str ) 
{

}
