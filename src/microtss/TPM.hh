#ifndef MICROTPM_HH_
#define MICROTPM_HH_

#include <stdexcept>
#include <string>
#include <vector>

#include <trousers/tss.h>
#include <trousers/trousers.h>
#include <tss/tspi.h>
#include <tss/tcpa_error.h>

#include <microtss/PublicKey.hh>

#define TSS_ERROR_LAYER(x)      (x & 0x3000)
#define TSS_ERROR_CODE(x)       (x & 0xFFF)

namespace microtss {
/// ...
typedef std::vector<BYTE> ByteString ;

enum TpmState { 
	DisabledDeactivatedNoOwner = 0,
	DisabledDeactivatedOwner   = 1,
	DisabledActivatedNoOwner   = 2,
	DisabledActivatedOwner     = 3,
	EnabledDeactivatedNoOwner  = 4,
	EnabledDeactivatedOwner    = 5,
	EnabledActivatedNoOwner    = 6,
	EnabledActivatedOwner      = 7
};

const unsigned int OWNER_MASK     = 1;
const unsigned int ACTIVATED_MASK = 2;
const unsigned int ENABLED_MASK   = 4;

/**
 * 
 */
class TPMError : public std::runtime_error {
	public:
		TPMError( const std::string &str );
};

/**
 * 
 */
class AuthenticationFailure : public TPMError {
	public:
		AuthenticationFailure( const std::string &str );
};

/**
 * 
 */
class NoSRKError : public TPMError {
	public:
		NoSRKError( const std::string &str );
};

/**
 * 
 */
class IsDisabledError : public TPMError {
	public:
		IsDisabledError( const std::string &str );
};

/**
 * 
 */
class IsDeactivatedError : public TPMError {
	public:
		IsDeactivatedError( const std::string &str );
};

/**
 * @brief Abstraction of the TPM.
 *
 * Hides implementation details of specific TPM versions/revisions.
 *
 * @note Implementation for TPM version 1.1b
 *
 * @todo Makes it sense to move all myContextHanle-related methods to TSS?
 */
class TPM
{

	public:

		/// Creates a new TPM object and reads its capabilities. 
		TPM( const TSS_HCONTEXT &contextHandle );
		/// Free context memory and close the context 
		~TPM();
		/// @return TPM version (according to TCG spec).
		const std::string getVersion() const;	
		/// @return TPM revision.
		const std::string getRevision() const;
		/// @return the name of the TPM manufacturer.
		const std::string getVendorName() const;
		/// @return the version of TSS
		const std::string getTSSVersion() const;
		/// @return the TPM Disabled/Deactivated/NoSRK/HasOwner status
		const TpmState getState() const;

		/// @return true if TPM is enabled.
		inline bool isEnabled() const;
		/// @return true if TPM is disabled.
		inline bool isDisabled() const;
		/// @return true if TPM is activated.
		inline bool isActivated() const;
		/// @return true if TPM is temporarily deactivated.
		inline bool isDeactivated() const;
		/// @return true when TPM has Owner
		inline bool hasOwner() const;
		/// @return true when TPM has Endorsement Key
		inline bool hasEndorsementKey() const;
		inline bool isEndorsementKeyResticted();
		inline bool isUserCreatedEndorsement();
		/// checks if TPM supports Maintenance
		inline bool hasMaintenance() const;

		/*/// @return Returns true if TPM is persistence deactivated.
		const bool getPersistenceDeactivated() const;
		/// @return Returns true if TPM is volatile deactivated.
		const bool getVolatileDeactivated() const;*/
		/// @return the number of PCRs supported by TPM
		UINT32 getNumberOfPCR() const;
		/// @return the number of 2048-bit RSA keys that can be loaded. This MAY vary with time and circumstances.
		UINT32 getKeyLoadCount() const;
		/// @return the number of available monotonic counters
		UINT32 getCountersCount() const;
		/// @return a vector of PCR values 
		const std::vector<ByteString> getPCRValues() const;
		/// Taking ownership of the TPM.
		/// @param ownerPwd			The Owner password
		/// @param srkPwd				The SRK password
		/// @param wellKnownSecret true if the SRK password is to set to WELL_KNOWN_SECRET
		void takeOwnership( const std::string &ownerPwd, const std::string &srkPwd, bool wellKnownSecret );
		/// Change owner password.
		void changeOwnerPassword( const std::string &oldOwnerPwd, const std::string &newOwnerPwd );
		
		/** 
       * Clear the TPM ownership.
       * @param  password Owner password.
       * @throws NoSRKError if TPM has no Owner.
       * @throws IsDisabledError if TPM is disabled.
       * @throws IsDeactivatedError if TPM is deactivated.
		 */
		void clearOwnership( const std::string &password );
		/// Enable the TPM.
		void setEnabled( const std::string &password );
		/// Disable the TPM.
		void setDisabled( const std::string &password );
		
		/** Run full TPM self test. 
       *    @return resullt of selftest as hexadecimal
       */
		const std::string selfTestFull() const;
		/// set TPM tepmorarily deactivated
		void setTempDeactivated();
		/// Get the Endorsement Public Infos
		microtss::PublicKey getEndorsementPublicKey( const std::string &password );
		/// Will Permanently disable the ability to read the endorsement public key without 
		/// required TPM owner authorization
		void restrictEK( std::string password );
		/// Disable  the functionality of creating a maintenance archive until a new owner is set.
		void killMaintenance( std::string password );
		/// Checks if the files /dev/tpm or /dev/tpm0 exist.
		bool driverAvailable();

	protected:
		/// Get the owner Password and set Secret
		void setSecret( const std::string &password );

	private:
		/// Read Capabilities and detect the Version of the TPM.
		void readVersion();
		
		/// Read capabilities and detect the VendorID as String.
		void readVendorName();
		
		/**
		 * @param capArea Capability
		 * @param len     length of returned data
		 * @param data    Pointer to the data to be returned.
		 * 
		 * @todo make a template method!
		 */
		inline UINT32 readCapabilities( TSS_FLAG capArea, BYTE *& data );
		/// Read the capability of TPM defined with CapabilityArea and SubCapability and set the value of them at Data
		UINT32 readCapabilities( TSS_FLAG capArea, UINT32 subCap, BYTE *& data );
		// template<type T> void readCapability( TSS_FLAG capArea, T &val );  
		/// Get the Status of TPM for given Flag
		bool getStatusFlag( TSS_FLAG statusFlag );
		/// Set the TPM Status flag
		void setStatusFlag( TSS_FLAG statusFlag, bool state );
		/// Detect TPM State and set myState to IsDisabled/IsDeactivated/NoSRK/HasOwner
		void readState();
		/// Read number of PCRs
		void readNumberOfPCR();
		/// Read Values of PCRs
		void readPCRValues();
		/// Read the number of 2048-bit RSA keys that can be loaded.
		void readKeyLoadCount();
		/// Read the number of available monotonic counters.
		void readCountersCount();
		/// Check if Endorsement Key extist and if that if restricted.
		void checkEndorsementKey();
		/// Check if Endorsement key is by user created
		void userCreatedEndorsement();
		/// Check if create maintenance archive is allowed
		void readMaintenanceState();

		/// Parses result and throws appropriated exception.
      void tssResultHandler( TSS_RESULT result, const std::string &str ) const;
		
		std::string myVersion;
		std::string myRevision;
		std::string myVendor;
		TpmState		myState;
		UINT32		myNumberOfPCR;			/// Number of PCR registers
		UINT32		myKeyLoadCount;		/// Number of 2048-bit RSA keys that can be loaded
		UINT32		myCountersCount;		/// Number of available monotonic counters
		std::vector<ByteString>  myPCRValues;

		TSS_HCONTEXT myContextHandle;			/// Handle to TSS Context
		TSS_HTPM     myTpmHandle;				/// Handle to TPM Object
		bool			 myHasMaintenance; 		/// true if TPM supports Maintenance
		bool			 myHasEndorsementKey; 	/// true if TPM has Endorsement Key
		bool			 myUserCreatedEndorsement; /// true if Endorsement key is by user created
		bool			 myEKRestricted;			/// true if Endorsement key is restricted
};

UINT32 TPM::readCapabilities( TSS_FLAG capArea, BYTE *& data ) 
{
	return readCapabilities(capArea, 0, data);
}

bool TPM::isEnabled() const
{
	return ( myState & ENABLED_MASK ) != 0;
}

bool TPM::isDisabled() const
{
	return !isEnabled();
}

bool TPM::isActivated() const
{
	return ( myState & ACTIVATED_MASK ) != 0;
}

bool TPM::isDeactivated() const
{
	return !isActivated();
}

bool TPM::hasOwner() const
{
	return (myState & OWNER_MASK) != 0;
}

bool TPM::hasMaintenance() const
{
	return myHasMaintenance;	
}

bool TPM::hasEndorsementKey() const
{
	return myHasEndorsementKey;
}

bool TPM::isEndorsementKeyResticted()
{
	return myEKRestricted;
}

bool TPM::isUserCreatedEndorsement()
{
	return myUserCreatedEndorsement;
}
} // namespace microtss
#endif /*TPM_HH_*/
