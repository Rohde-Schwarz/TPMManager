#ifndef MICROTSS_HH_
#define MICROTSS_HH_

#include <stdexcept>
#include <vector>
#include <string>

#include <trousers/tss.h>
#include <tss/tspi.h>
#include <tss/tcpa_error.h>

namespace microtss {
/**
 * 
 */
class TSSSystemNotFound : public std::runtime_error {
	public:
		TSSSystemNotFound();
};

/**
 * 
 */
class TPMDriverNotFound : public std::runtime_error {
	public:
		TPMDriverNotFound();
};

/**
 * @brief Abstraction of the TSS interface.
 *
 * Encapsulates access to the underlying Trusted Software Stack (TSS) and TPM.
 */
class TSS {
	public:
		/// 
		TSS();
		/// @return Returns the name of the TSS manufacturer.
		const std::string getVendorName() const;
		/// @return Returns the version of TSS
		const std::string getVersion() const;
		/// @return TSS revision.
		const std::string getRevision() const;
		
		inline TSS_HCONTEXT getContextHandle() const;

	private:
		/// Creates a TSS context object and connecting to it.
		void createTssContext();
		
		/// Read Capabilities and detect the Version of the TSS.
		void readVersion();
		
		/// Read capabilities and detect the TSS VendorID as String.
		void readVendorName();
		
		/**
		 * @param capArea Capability
		 * @param len     length of returned data
		 * @param data    Pointer to the data to be returned.
		 * 
		 * @todo make a template method!
		 */
		inline UINT32 readCapabilities( TSS_FLAG capArea, BYTE *& data );
		UINT32 readCapabilities( TSS_FLAG capArea, UINT32 subCap, BYTE *& data );
		
		std::string		myVersion;
		std::string 	myRevision;
		std::string 	myVendor;

		TSS_HCONTEXT	myContextHandle;	//Handle to TSS Context
};

UINT32 TSS::readCapabilities( TSS_FLAG capArea, BYTE *& data ) 
{
	return readCapabilities(capArea, 0, data);
}

TSS_HCONTEXT TSS::getContextHandle() const 
{
	return myContextHandle;
}
} // namespace microtss
#endif /*TSS_HH_*/
