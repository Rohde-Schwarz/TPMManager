// #include "tpm_exist.hh"

#include "TSS.hh"
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace microtss;


TSSSystemNotFound::TSSSystemNotFound() :
	runtime_error("Error at connect to TSS system!")
{}

TPMDriverNotFound::TPMDriverNotFound() :
	runtime_error("TPM driver not found!")
{}

TSS::TSS()
{
   /* 
	* @brief            returns a handle to a new context object. The context handle is used in 
	*                   various functions to assign resources to it.
	* @param  hContext  Receives the handle to the created context object.
	* @return			TSS_SUCCESS or TSS_E_INTERNAL_ERROR
	*/
	if ( Tspi_Context_Create( &myContextHandle ) != TSS_SUCCESS ) {
		cerr << "TSS.cpp: Error at create context object " << endl;
	}
	
	/*
	 * @brief			     	establishes a connection to a local or remote TSS system
	 * @param myContextHandle   Handle of the context object
	 * @param wszDestinaion 	Pointer to a null terminated UNICODE string specifying the remote 
	 * 						 	system which is to be connected.
	 * 						 	If NULL, the context object is bound to the local system.
	 * @return				 	TSS_SUCCESS, TSS_E_INVALID_HANDLE, 
	 * 						 	TSS_E_NO_CONNECTION, TSS_E_INTERNAL_ERROR
	 */
	if (Tspi_Context_Connect( myContextHandle, NULL ) != TSS_SUCCESS) 
		throw TSSSystemNotFound();

	readVersion();
	readVendorName();
};

UINT32 TSS::readCapabilities( TCPA_CAPABILITY_AREA capArea, UINT32 subCap, BYTE *& data )
{
	UINT32 len = 0;
	
		/*
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
	if ( Tspi_Context_GetCapability( myContextHandle, 
											capArea, 
											sizeof(UINT32), 
											(BYTE*) &subCap,
											&len, 
											&data ) != TSS_SUCCESS )
		// throw exception
   		cerr << "TSS.cpp: Error at get capabilities!" << endl;

   return len;
}

void TSS::readVersion()
{
	TSS_VERSION   *version; 
	ostringstream sVersion;

	readCapabilities( TSS_TCSCAP_VERSION, (BYTE*&) version );
	
   sVersion << (unsigned int) version->bMajor << ".";
	
	if (version->bMinor == 0x01)
		sVersion << "1b" << ends;
	else
		sVersion << (unsigned int) version->bMinor << ends;	
	
	myVersion = sVersion.str();
	
	ostringstream sRevision;
	
	sRevision << (unsigned int) version->bRevMajor << "." << (unsigned int) version->bRevMinor << ends;

	myRevision = sRevision.str();
	
	Tspi_Context_FreeMemory( myContextHandle, (BYTE*)version );
	
}

void TSS::readVendorName()
{
	myVendor = "Trousers";
/*
	char *name;

	UINT32 len = readCapabilities( TSS_TCSCAP_MANUFACTURER, (BYTE*&) name);
		
	cout << "len = " << len;
	
	myVendor.assign(name, len);
*/	
}

const string TSS::getVersion() const
{
	return myVersion;
}

const string TSS::getRevision() const
{
	return myRevision;
}

const string TSS::getVendorName() const
{
	return myVendor;
}
