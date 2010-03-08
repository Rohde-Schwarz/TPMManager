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
* @file PublicKeyView.cxx
*
* @brief Public Key Dialog Class Implementation File
*
**/

#include "PublicKeyView.hxx"
#include <QLabel>
#include <QString>
#include <string>

using namespace std;
using namespace microtss;

PublicKeyView::PublicKeyView( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
}

void PublicKeyView::setPublicKey( PublicKey &pk )
{
	myVersion->setText( QString::fromStdString( pk.getVersion() ) );
	myType->setText( QString::fromStdString( pk.getType() ) );

	string algo = pk.getAlgorithm();

	myAlgorithm->setText( QString::fromStdString( algo ) );
	myKeySize->setText( QString::fromStdString( pk.getKeySize( algo ) ) );
	myEncScheme->setText( QString::fromStdString( pk.getEncryptionScheme() ) );
	mySigScheme->setText( QString::fromStdString( pk.getSignatureScheme() ) );
}
PublicKeyView::~PublicKeyView()
{
}
//

