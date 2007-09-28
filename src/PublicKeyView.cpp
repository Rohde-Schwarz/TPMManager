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
#include "PublicKeyView.hh"
#include <qlabel.h>

#include <string>

using namespace std;
using namespace microtss;

PublicKeyView::PublicKeyView( PublicKey &pk )
{
	myVersion->setText( pk.getVersion() );
	myType->setText( pk.getType() );

	string algo = pk.getAlgorithm();

	myAlgorithm->setText( algo );
	myKeySize->setText( pk.getKeySize( algo ) );
	myEncScheme->setText( pk.getEncryptionScheme() );
	mySigScheme->setText( pk.getSignatureScheme() );
}
