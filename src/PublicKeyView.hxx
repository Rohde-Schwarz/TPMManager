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

#ifndef PUBLICKEYVIEW_H
#define PUBLICKEYVIEW_H
//
#include <QDialog>
#include "ui_PublicKeyView.h"
#include <microtss/PublicKey.hxx>

/**
* @file PublicKeyView.hxx
*
* @brief Public Key Dialog Class Header File
*
**/

/**
* @class PublicKeyView
*
* @brief Implements a user dialog to view details of a public key (e.g. Endorsement Key)
*
* This class implements a user dialog to view details of a public key. The base class is automatically generated as "ui_publickeyview".
*
*/

class PublicKeyView : public QDialog, public Ui::PublicKeyView
{
Q_OBJECT
public:
	/**
	* @brief Default constructor
	*
	* @param parent parent QWidget object
	* @param f window flags
	*
	* @return New PublicKeyView instance
	*
	*/
	PublicKeyView( QWidget * parent = 0, Qt::WindowFlags f = 0);
	
	/**
	* @brief Default destructor
	*
	*/
	~PublicKeyView();	
	
	/**
	* @brief Sets up the public key view dialog
	*
	* @param pk the public key object to view details of
	*/
	void setPublicKey( microtss::PublicKey &pk );
private slots:
};
#endif





