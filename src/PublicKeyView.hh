#ifndef PUBLICKEYVIEW_HH
#define PUBLICKEYVIEW_HH

#include <publickeyview.h>
#include <microtss/PublicKey.hh>

/**
*/
class PublicKeyView : public PublicKeyViewWidget {
public:
    PublicKeyView( microtss::PublicKey &pk );

};

#endif
