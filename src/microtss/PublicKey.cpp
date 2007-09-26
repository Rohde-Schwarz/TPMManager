/***************************************************************************
 *   Copyright (C) 2006 by Anoosheh Zaerin   *
 *   anoosheh.zaerin@rub.de   *
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

#include "PublicKey.hh"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace microtss;

PublicKey::PublicKey( const TSS_HKEY	&endorsementKeyHandle ) :
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
		case TPM_KEY_SIGNING:
			usage = "Signing";
			break;	
		case TPM_KEY_STORAGE:
			usage = "Storage";
			break;			
		case TPM_KEY_IDENTITY:
			usage = "Identity";
			break;			
		case TPM_KEY_AUTHCHANGE:
			usage = "AuthChange";
			break;			
		case TPM_KEY_BIND:
			usage = "Bind";
			break;			
		case TPM_KEY_LEGACY:
			usage = "Legacy";
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
	case TCPA_ALG_RSA:
		algo = "RSA";
		break;
	case TCPA_ALG_DES:
		algo = "DES";
		break;
	case TCPA_ALG_3DES:
		algo = "3DES";
		break;
	case TCPA_ALG_SHA:
		algo = "SHA";
		break;
	case TCPA_ALG_HMAC:
		algo = "HMAC";
		break;
	case TCPA_ALG_AES:
		algo = "AES";
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
	case TCPA_ES_NONE:
		encscheme = "None";
		break;
	case TCPA_ES_RSAESPKCSv15:
		encscheme = "PKCS 15";
		break;
	case TCPA_ES_RSAESOAEP_SHA1_MGF1:
		encscheme = "RSA OAEP SHA1 MGF1";
		break;
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
	case TCPA_SS_NONE:
		sigscheme = "None";
		break;
	case TCPA_SS_RSASSAPKCS1v15_SHA1:
		sigscheme = "RSA PKCS15 SHA1";
		break;
	case TCPA_SS_RSASSAPKCS1v15_DER:
		sigscheme = "RSA PKCS15 DER";
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

	if ( algorithm == "RSA") 
	{
		result = Tspi_GetAttribUint32( keyHandle, 
											TSS_TSPATTRIB_RSAKEY_INFO,
											TSS_TSPATTRIB_KEYINFO_RSA_KEYSIZE,
											&attributeSize );
		if ( result != TSS_SUCCESS )
		{
			cerr << "Get Attribute Data. Error occured while trying to access to Endorsement Key Size info." << endl;
			return "Error";
		}
		ostringstream ostr;
		ostr << attributeSize;
		return ostr.str();
	}
	else return "Unknown key Size";
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
