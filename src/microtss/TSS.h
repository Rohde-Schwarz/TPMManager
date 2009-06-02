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

#ifndef MICROTSS_HH_
#define MICROTSS_HH_

#include <stdexcept>
#include <vector>
#include <string>

#include <trousers/tss.h>
#include <tss/tspi.h>
#include <tss/tpm_error.h>

/**
* @file TSS.h
*
* @brief TCG Software Stack Class Header File
*
**/ 

namespace microtss {

/**
* @class TSSSystemNotFound
*
* @brief Implements an error class in case TSS is not found
*
*/
class TSSSystemNotFound : public std::runtime_error {
	public:
		/**
		* @brief Issues a runtime error
		*
		*/
		TSSSystemNotFound();
};

/**
* @class TPMDriverNotFound
*
* @brief Implements an error class in case TPM driver is not found
*
*/
class TPMDriverNotFound : public std::runtime_error {
	public:
		/**
		* @brief Issues a runtime error
		*
		*/
		TPMDriverNotFound();
};

/**
* @class TSS
*
* @brief Encapsulates TCG software stack (TSS) object
*
* This class encapsulates a TSS object. It is used to connect to a TSS for communication with the TPM and get detailed information on the TSS itself. 
*
*/ 
class TSS {
	public:
		/**
		* @brief Default constructor
		*
		* Creates a new TSS context, connects to the TSS and reads TSS version and vendor.
		*/
		TSS();
		
		/**
		* @brief Default destructor
		*
		*/
		~TSS();
		
		/**
		* @brief Get name of TSS vendor
		* @return Name of the TSS manufacturer
		*
		*/
		const std::string getVendorName() const;
		
		/**
		* @brief Get version of TSS
		* @return Version number of TSS
		*
		*/
		const std::string getVersion() const;
		
		/**
		* @brief Get revision of the TSS
		* @return Revision number of TSS
		*
		*/
		const std::string getRevision() const;
		
		/**
		* @brief Get a TSS context handle
		* @return Default context handle object
		*
		*/
		inline TSS_HCONTEXT getContextHandle() const;

	private:
		/**
		* @brief Create a TSS context object and connects to it
		*
		*/
		void createTssContext();
		
		/**
		* @brief Read capabilities and detect the version of the TSS
		*
		*/
		void readVersion();
		
		/**
		* @brief Read capabilities and detect the TSS VendorID as string
		*
		*/
		void readVendorName();
		
		/**
		* @brief Read TPM capability
		* @param capArea Capability
		* @param data    Pointer to the data to be returned.
		* 
		* @todo make a template method!
		*/
		inline UINT32 readCapabilities( TSS_FLAG capArea, BYTE *& data );
		
		/**
		* @brief Read TPM capability (detailed) 
		* @param capArea capability
		* @param subCap detailed (sub) capability
		* @param data    Pointer to the data to be returned.
		* 
		*/
		UINT32 readCapabilities( TSS_FLAG capArea, UINT32 subCap, BYTE *& data );
		
		std::string		myVersion;
		std::string 	myRevision;
		std::string 	myVendor;
		
		/// Handle to TSS Context
		TSS_HCONTEXT	myContextHandle;	
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
