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

#ifndef MICROPUBLICKEY_HH
#define MICROPUBLICKEY_HH

#include <string>
#include <iostream>
#include <trousers/tss.h>

/**
* @file PublicKey.hxx
*
* @brief Public Key Class Header File
*
**/

namespace microtss {

/**
* @class PublicKey
*
* @brief Encapsulates a Public Key object
*
* This class implements a Public Key object. It is used with the TPM's Endorsement Key.
*
*/ 

class PublicKey{
    friend std::ostream& operator<<( std::ostream &, const PublicKey & );

public:
	/**
	* @brief Default constructor
	*
	* @param endorsementKeyHandle Handle of the Endorsement Key
	*
	*/
    PublicKey( const TSS_HKEY	&endorsementKeyHandle );

	/**
	* @brief Default destructor
	*
	*/
    ~PublicKey();

	/**
	* @brief Get the version number of the public key
	*
	* @return version number of public key
	*
	*/
	const std::string getVersion();
	
	/**
	* @brief Get the type of public key
	*
	* @return type of public key
	*
	*/
	const std::string getType();
	
	/**
	* @brief Get the algorithm of public key
	*
	* @return algorithm of public key
	*
	*/
	const std::string getAlgorithm();
	
	/**
	* @brief Get the encryption scheme of public key
	*
	* @return encryption scheme of public key (if any)
	*
	*/
	const std::string getEncryptionScheme();
	
	/**
	* @brief Get the signature scheme of public key
	*
	* @return signature scheme of public key (if any)
	*
	*/
	const std::string getSignatureScheme();
	
	/**
	* @brief Get the key size of public key
	*
	* @return key size of public key (in bits)
	*
	*/
	const std::string getKeySize( std::string algorithm );

	 
protected:
	 TSS_HKEY keyHandle;
};

std::ostream& operator<<( std::ostream &ostr, const PublicKey &pk );
} // namespace microtss
#endif
