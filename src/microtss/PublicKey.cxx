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

/**
* @file PublicKey.cxx
*
* @brief Public Key Class Implementation File
*
**/


#include "PublicKey.hxx"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace microtss;

PublicKey::PublicKey( const TSS_HKEY &endorsementKeyHandle ) :
  keyHandle( endorsementKeyHandle )
{
}

PublicKey::~PublicKey()
{
}

const string PublicKey::getVersion()
{
	UINT32		 attributeSize;
	BYTE 			 *pAttribute;

	TSS_RESULT result = Tspi_GetAttribData( keyHandle, 
											TSS_TSPATTRIB_KEY_INFO,
			  								TSS_TSPATTRIB_KEYINFO_VERSION, 
											&attributeSize,
			  								&pAttribute );
	if ( result != TSS_SUCCESS )
		{
			cerr <<  "Get Attribute Data. Error occured while trying to access to Endorsement Key Version info." << endl;
			return "Error";
		}

	ostringstream  attributes;
	for ( UINT32 i=0; i<attributeSize; ++i )
		attributes << hex << setfill( '0' ) << setw(2) << (unsigned int) pAttribute[i];
	return attributes.str();
}

const string PublicKey::getType()
{
	TSS_RESULT result;
	UINT32		 attributeType;

	result = Tspi_GetAttribUint32( keyHandle, 
										TSS_TSPATTRIB_KEY_INFO,
			   						TSS_TSPATTRIB_KEYINFO_USAGE,
										&attributeType );
	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Usage info." << endl;
		return "Error";
	}

	string usage;
	switch ( attributeType ) {
		case TSS_KEYUSAGE_BIND:
			usage = "Binding";
			break;	
		case TSS_KEYUSAGE_IDENTITY:
			usage = "Identity";
			break;			
		case TSS_KEYUSAGE_LEGACY:
			usage = "Legacy";
			break;			
		case TSS_KEYUSAGE_SIGN:
			usage = "Signing";
			break;			
		case TSS_KEYUSAGE_MIGRATE:
			usage = "Migration";
			break;			
		case TSS_KEYUSAGE_STORAGE:
			usage = "Storage";
			break;
		case TSS_KEYUSAGE_AUTHCHANGE:
			usage = "AuthChange";
			break;
		default:
			usage = "Endorsement Key";
		}
	return usage;
}

const string PublicKey::getAlgorithm()
{
	TSS_RESULT result;
	UINT32		 attributeAlgorithm;

	result = Tspi_GetAttribUint32( keyHandle, 
										TSS_TSPATTRIB_KEY_INFO,
			    						TSS_TSPATTRIB_KEYINFO_ALGORITHM,
										&attributeAlgorithm );
	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Algorithm info." << endl;
		return "Error";
	}
	string algo;
	switch (attributeAlgorithm) {
	case TSS_ALG_RSA:
		algo = "RSA";
		break;
	case TSS_ALG_DES:
		algo = "DES";
		break;
	case TSS_ALG_3DES:
		algo = "3DES";
		break;
	case TSS_ALG_SHA:
		algo = "SHA";
		break;
	/*
	case TSS_ALG_SHA256:
		algo = "SHA-256";
		break;
	*/
	case TSS_ALG_HMAC:
		algo = "HMAC";
		break;
	case TSS_ALG_AES:
		algo = "AES-128 (Legacy)";
		break;
	/*
	case TSS_ALG_AES128:
		algo = "AES-128";
		break;
	*/
	case TSS_ALG_AES192:
		algo = "AES-192";
		break;
	case TSS_ALG_AES256:
		algo = "AES-256";
		break;
	case TSS_ALG_MGF1:
		algo = "XOR-MGF1";
		break;
	case TSS_ALG_XOR:
		algo = "XOR (rolling nonces)";
		break;
	default:
		algo = "Unknown";
	}
	return algo;
}

const string PublicKey::getEncryptionScheme()
{
	TSS_RESULT result;
	UINT32		 attributeScheme;

	result = Tspi_GetAttribUint32( keyHandle, 
										TSS_TSPATTRIB_KEY_INFO,
			    						TSS_TSPATTRIB_KEYINFO_ENCSCHEME,
										&attributeScheme );
	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Encryption scheme info." << endl;
		return "Error";
	}	
	
	string encscheme;
	switch ( attributeScheme ) {
	case TSS_ES_NONE:
		encscheme = "None";
		break;
	case TSS_ES_RSAESPKCSV15:
		encscheme = "PKCS 15";
		break;
	case TSS_ES_RSAESOAEP_SHA1_MGF1:
		encscheme = "RSA OAEP SHA1 MGF1";
		break;
	/*
	case TSS_ES_RSAESOIAP_SHA1_MGF1:
		encscheme = "RSA OIAP SHA1 MGF1";
		break;
	case TSS_ES_RSAESOSAP_SHA1_MGF1:
		encscheme = "RSA OSAP SHA1 MGF1";
		break;
	*/
	default:
		encscheme = "Unknown";
	}
	
	return encscheme;
}

const string PublicKey::getSignatureScheme()
{
	TSS_RESULT result;
	UINT32	  attributeScheme;

	result = Tspi_GetAttribUint32( keyHandle, 
										TSS_TSPATTRIB_KEY_INFO,
			    						TSS_TSPATTRIB_KEYINFO_SIGSCHEME,
										&attributeScheme );
	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Signature scheme info." << endl;
		return "Error";
	}
	string sigscheme;
	switch ( attributeScheme ) {
	case TSS_SS_NONE:
		sigscheme = "None";
		break;
	case TSS_SS_RSASSAPKCS1V15_SHA1:
		sigscheme = "RSA PKCSv1.5 SHA1";
		break;
	case TSS_SS_RSASSAPKCS1V15_DER:
		sigscheme = "RSA PKCSv1.5 DER";
		break;
	default:
		sigscheme = "Unknown";
	}
	return sigscheme;
}

const string PublicKey::getKeySize( string algorithm )
{	
	TSS_RESULT result;
	UINT32	  attributeSize;
	string size;

	result = Tspi_GetAttribUint32( keyHandle, 
										TSS_TSPATTRIB_KEY_INFO,
										TSS_TSPATTRIB_KEYINFO_SIZE,
										&attributeSize );
	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Size info." << endl;
		return "Error";
	}
		
	switch( attributeSize ) {
		case TSS_KEY_SIZEVAL_512BIT:
			size = "512 bit";
			break;
		case TSS_KEY_SIZEVAL_1024BIT:
			size = "1024 bit";
			break;
		case TSS_KEY_SIZEVAL_2048BIT:
			size = "2048 bit";
			break;
		case TSS_KEY_SIZEVAL_4096BIT:
			size = "4096 bit";
			break;
		case TSS_KEY_SIZEVAL_8192BIT:
			size = "8192 bit";
			break;
		case TSS_KEY_SIZEVAL_16384BIT:
			size = "16384 bit";
			break;
		default:
			size = "Unknown";
	}
	
	return size;
}
ostream&  microtss::operator << ( ostream &ostr, const microtss::PublicKey &pk )
{
	TSS_RESULT result;
	UINT32		 attributeSize;
	BYTE 			 *pAttribute;
	result = Tspi_GetAttribData( pk.keyHandle,
											TSS_TSPATTRIB_KEY_BLOB,
											TSS_TSPATTRIB_KEYBLOB_BLOB,
											&attributeSize,
											&pAttribute );

	if ( result != TSS_SUCCESS )
	{
		cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Public Key." << endl;
		return ostr << "Error";
	}

   ostr.write( (char*)pAttribute, attributeSize );

	return ostr;
}
