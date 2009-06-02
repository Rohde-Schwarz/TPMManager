/***************************************************************************
 *   Copyright (C) 2006-2009 Sirrix AG                                     *
 *   Authors:                                                              *
 *	 Anoosheh Zaerin <a.zaerin@sirrix.com>                                 *
 *   René Korthaus <r.korthaus@sirrix.com>                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MICROTPM_HH_
#define MICROTPM_HH_

#include <stdexcept>
#include <string>
#include <vector>

#include <trousers/tss.h>
#include <trousers/trousers.h>
#include <tss/tspi.h>
#include <tss/tpm_error.h>

#include <microtss/PublicKey.h>

#define TSS_ERROR_LAYER(x)      (x & 0x3000)
#define TSS_ERROR_CODE(x)       (x & 0xFFF)

/**
* @file TPM.h
*
* @brief TPM Encapsulation Class Header File
*
**/

namespace microtss {
/// ...
typedef std::vector<BYTE> ByteString ;

enum TpmState { 
	DisabledDeactivatedNoOwner = 0, ///   000
	DisabledDeactivatedOwner   = 1, ///   001
	DisabledActivatedNoOwner   = 2, ///   010
	DisabledActivatedOwner     = 3, ///   011
	EnabledDeactivatedNoOwner  = 4, ///   100
	EnabledDeactivatedOwner    = 5, ///   101
	EnabledActivatedNoOwner    = 6, ///   110
	EnabledActivatedOwner      = 7  ///   111
};

const unsigned int OWNER_MASK     = 1;
const unsigned int ACTIVATED_MASK = 2;
const unsigned int ENABLED_MASK   = 4;

/**
 * @class TPMError
 * @brief Base exception class used for TPM command errors
 */
class TPMError : public std::runtime_error {
	public:
		TPMError( const std::string &str );
};

/**
 * @class AuthenticationFailure
 * @brief Exception class used whenever a TPM authentication error occurs
 */
class AuthenticationFailure : public TPMError {
	public:
		AuthenticationFailure( const std::string &str );
};

/**
 * @class NoOwnerError
 * @brief Exception class used to signal that no TPM Owner is installed
 */
class NoOwnerError : public TPMError {
	public:
		NoOwnerError( const std::string &str );
};

/**
 * @class HasOwnerError
 * @brief Exception class used to signal that a TPM Owner is already installed
 */
class HasOwnerError : public TPMError {
	public:
		HasOwnerError( const std::string &str);
};

/**
 * @class NoSRKError
 * @brief Exception class used to signal that no Storage Root Key (SRK) is present inside the TPM
 */
class NoSRKError : public TPMError {
	public:
		NoSRKError( const std::string &str );
};

/**
 * @class IsDisabledError
 * @brief Exception class used to signal that the TPM is currently disabled
 */
class IsDisabledError : public TPMError {
	public:
		IsDisabledError( const std::string &str );
};

/**
 * @class IsDeactivatedError
 * @brief Exception class used to signal that the TPM is currently deactivated
 */
class IsDeactivatedError : public TPMError {
	public:
		IsDeactivatedError( const std::string &str );
};

/**
 * @class UnknownError
 * @brief Exception class used to signal that an unknown error occured
 */
class UnknownError : public TPMError {
	public:
		UnknownError( const std::string &str );
};

/**
* @class TPM
*
* @brief Encapsulates a TPM object
*
* This class encapsulates a TPM object. It is used to get and set the TPM status and send commands to the TPM.
*
* @note Implementation for TPM version 1.2, partially 1.1b (e.g. temp. deactivate)
*
* @todo Does it make sense to move all myContextHanle-related methods to TSS?
*/
class TPM
{

	public:
		/**
		* @brief Constructor
		* Creates a new TPM object and reads its capabilities
		*/
		TPM( const TSS_HCONTEXT &contextHandle );
		
		/**
		* @brief Destructor
		* Free context memory and close the context
		*/ 
		~TPM();
		
		/**
		* @brief Get TPM version
		* @return TPM version (according to TCG spec)
		*/
		const std::string getVersion() const;
		
			
		/**
		* @brief Get TPM revision
		* @return TPM revision number
		*/
		const std::string getRevision() const;
		
		/**
		* @brief Get the name of TPM manufacturer
		* @return name of TPM manufacturer
		*/
		const std::string getVendorName() const;
		
		/**
		* @brief Get TCG Software Stack (TSS) version
		* @return the version of TSS
		*/
		const std::string getTSSVersion() const;
		
		/**
		* @brief Get the TPM status
		* @return the TPM Disabled/Deactivated/NoSRK/HasOwner status
		* @note Not functional anymore, use the getter methods "is-PROPERTY-()" instead
		*/ 
		TpmState getState() const;

		/// @return true if TPM is enabled.
		inline bool isEnabled() const;
		/// @return true if TPM is disabled.
		inline bool isDisabled() const;
		/// @return true if TPM is activated.
		inline bool isActivated() const;
		/// @return true if TPM is temporarily deactivated.
		inline bool isDeactivated() const;
		/// @return true if TPM has Owner
		inline bool hasOwner() const;
		/// @return true if TPM has Endorsement Key
		inline bool hasEndorsementKey() const;
		/// @return true if Endorsement Key usage is restricted to TPM owner only
		inline bool isEndorsementKeyResticted();
		/// @return true if Endorsement Key was created by user and not TPM manufacturer
		inline bool isUserCreatedEndorsement();
		/// @return true if TPM supports maintenance feature
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
		
		/**
		* @brief Take ownership of the TPM
		* @param ownerPwd			The TPM Owner password
		* @param srkPwd				The SRK password
		* @param wellKnownSecret true if the SRK password should be set to WELL_KNOWN_SECRET
		*
		* @throws HasOwnerError if TPM already has an owner
		* @throws IsDisabledError if TPM is currently disabled
		* @throws UnknownError if an unknown error occured
		*/
		void takeOwnership( const std::string &ownerPwd, const std::string &srkPwd, bool wellKnownSecret );
		/**
		* @brief Change owner password
		* @param oldOwnerPwd	current TPM Owner password
		* @param newOwnerPwd	new TPM Owner password
		*
		* @throws NoSRKError If TPM has no owner
		* @throws IsDisabledError If TPM is disabled
		* @throws IsDeactivatedError If TPM is deactivated
		* @throws UnknownError If an unknown error occured
		*/
		void changeOwnerPassword( const std::string &oldOwnerPwd, const std::string &newOwnerPwd );
		
		/** 
		* @brief Clear TPM Ownership
		* @param  password Owner password
		*
		* @throws NoSRKError If TPM has no Owner
		* @throws IsDisabledError If TPM is disabled
		* @throws IsDeactivatedError If TPM is deactivated
		* @throws UnknownError If an unknown error occured		
		*		
		*/
		void clearOwnership( const std::string &password );
		
		/**
		* @brief Enable/Disable the TPM
		* @param set	true if TPM should be disabled, false if TPM should be enabled
		* @param password	TPM Owner password
		*
		*/
		inline void setDisabled( bool set, const std::string &password );

		/**
		* @brief Run full TPM self test
		*/
		bool selfTestFull() const;
		
		/** 
		* @brief Return full selftest result
		* @return result of selftest as hexadecimal
		*/
		const std::string selfTestResult() const;
		
		/**
		* @brief Set TPM to temporarily deactivated
		* @param password	TPM Owner password
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void setTempDeactivated( const std::string &password );
		
		/**
		* @brief Get the public part of TPM Endorsement Key
		* @param password	TPM Owner password
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		microtss::PublicKey getEndorsementPublicKey( const std::string &password );
		
		/**
		* @brief Permanently disable the ability to read the endorsement public key without TPM owner authorization
		* @param password	TPM Owner password
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void restrictEK( std::string password );
		
		/**
		* @brief Disable the functionality of creating a maintenance archive until a new owner is set
		* @param password	TPM Owner password
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void killMaintenance( std::string password );
	
	protected:
		/**
		* @brief Set the secret for TSS_POLICY_USAGE
		* @param password	password in plaintext
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void setSecret( const std::string &password );

	private:
		/**
		* @brief Read Capabilities and detect the version of the TPM
		*
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void readVersion();
		/**
		* @brief Read the correct Version of the TPM 1.2
		*
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
    	void readVersionVal();
    	
    	
		/**
		* @brief Read capabilities and detect the VendorID as String
		*
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void readVendorName();
		
		/**
		* @brief Read TPM Capability
		* @param capArea Capability
		* @param data    Pointer to the data to be returned.
		* @return 		  length of data to be returned
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*
		* @todo make a template method!
		*/
		inline UINT32 readCapabilities( TSS_FLAG capArea, BYTE *& data );
		
		/**
		* @brief Read the capability of TPM defined with CapabilityArea and SubCapability and set the value of them at Data
		* @param capArea	Capability
		* @param subCap		Subcapability (detailed)
		* @param data    Pointer to the data to be returned.
		* @return 		  length of data to be returned
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		UINT32 readCapabilities( TSS_FLAG capArea, UINT32 subCap, BYTE *& data );
		// template<type T> void readCapability( TSS_FLAG capArea, T &val );
		
		
		/**
		* @return Status of TPM for given Flag
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		bool getStatusFlag( TSS_FLAG statusFlag );
		
		/**
		* @brief Set a TPM Status flag
		* @param statusFlag		status flag to set
		* @param state			state of flag
		*
		* @throws AuthenticationFailure	If authentication fails
		* @throws IsDeactivatedError	If TPM is deactivated
		* @throws IsDisabledError		If TPM is disabled
		* @throws UnknownError			If an unknown error occured
		*/
		void setStatusFlag( TSS_FLAG statusFlag, bool state );
		
		/**
		* @brief Detect TPM State and set myState to IsDisabled/IsDeactivated/NoSRK/HasOwner
		*
		* @throws UnknownError			If an unknown error occured
		*/
		void readState();
		
		
		/**
		* @brief Read number of PCRs
		*/
		void readNumberOfPCR();
		
		/**
		* @brief Read Values of PCRs
		*/
		void readPCRValues();
		
		/**
		* @brief Read the number of 2048-bit RSA keys that can be loaded
		*/
		void readKeyLoadCount();
		
		/**
		* @brief Read the number of available monotonic counters
		*/
		void readCountersCount();
		
		/**
		* @brief Check if Endorsement Key exists and if access is restricted
		**/
		void checkEndorsementKey();
		
		/**
		* @brief Check if Endorsement key is by user created
		*/		
		void userCreatedEndorsement();
		
		/**
		*  Check if create maintenance archive is allowed
		**/
		void readMaintenanceState();

		/**
		* @brief Parses result and throws appropriated exception
		* @note	Is called whenever a Tspi command is issued to check the result of the operation
		*/
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
		TSS_HTPM    myTpmHandle;				/// Handle to TPM Object
		bool		myHasOwner;				/// true if TPM Owner is set
		bool		myIsDisabled;			/// true if TPM is disabled
		bool 		myIsDeactivated;		/// true if TPM is deactivated
		bool		myHasMaintenance; 		/// true if TPM supports Maintenance
		bool		myHasEndorsementKey; 	/// true if TPM has Endorsement Key
		bool		myUserCreatedEndorsement; /// true if Endorsement key is by user created
		bool		myEKRestricted;			/// true if Endorsement key is restricted
};

void TPM::setDisabled( bool set, const std::string &password )
{
	if ( set == true ) {
		if ( isDisabled() )
			return;
	} else if( set == false) {
		if ( isEnabled() )
			return;
	}

	setSecret( password );
	setStatusFlag( TSS_TPMSTATUS_OWNERSETDISABLE, set ); 
	
	readState();
}

UINT32 TPM::readCapabilities( TSS_FLAG capArea, BYTE *& data )
{
	return readCapabilities(capArea, 0, data);
}

bool TPM::isEnabled() const
{
	return !myIsDisabled;
}

bool TPM::isDisabled() const
{
	return myIsDisabled;
}

bool TPM::isActivated() const
{
	return !isDeactivated();
}

bool TPM::isDeactivated() const
{
	return myIsDeactivated;
}

bool TPM::hasOwner() const
{
	return myHasOwner;
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
