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
* @file PasswordDialog.cxx
*
* @brief Password Dialog Class Implementation File
*
**/

#include "PasswordDialog.hxx"
#include <iostream>

using namespace std;

PasswordDialog::PasswordDialog( QWidget * parent, Qt::WindowFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	// set up description
	setDescription("Please enter password.");
	// set up password field
	myPassword->clear();	
	myPassword->setEchoMode(QLineEdit::Password);
	// initially disable OK button
	okButton->setEnabled( false );
	QMetaObject::connectSlotsByName( this );
}

void PasswordDialog::setDescription(const QString & desc)
{
	myDescription->setText(desc);
}

void PasswordDialog::setPrompt(const QString & prompt)
{
	setWindowTitle(prompt);
}

QString PasswordDialog::password()
{
	return myPassword->text();
}

void PasswordDialog::on_myPassword_textEdited(const QString & text)
{
	if( text.length() > 0 ) {
		okButton->setEnabled( true );
	} else {
		okButton->setEnabled( false );
	}
}

PasswordDialog::~PasswordDialog()
{
}
