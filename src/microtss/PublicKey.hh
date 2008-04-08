#ifndef MICROPUBLICKEY_HH
#define MICROPUBLICKEY_HH

#include <string>
#include <iostream>
#include <trousers/tss.h>

/**
*/
namespace microtss {
class PublicKey{
    friend std::ostream& operator<<( std::ostream &, const PublicKey & );

public:
    PublicKey( const TSS_HKEY	&endorsementKeyHandle );

    ~PublicKey();

	 const std::string getVersion();
	 const std::string getType();
	 const std::string getAlgorithm();
	 const std::string getEncryptionScheme();
	 const std::string getSignatureScheme();
	 const std::string getKeySize( std::string algorithm );

	 
protected:
	 TSS_HKEY keyHandle;
};

std::ostream& operator<<( std::ostream &ostr, const PublicKey &pk );
} // namespace microtss
#endif
