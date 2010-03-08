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
* @file NewPasswordDialog.cxx
*
* @brief New Password Dialog Class Implementation File
*
**/

#include "NewPasswordDialog.hxx"

NewPasswordDialog::NewPasswordDialog( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	setDescription("Please enter new password.");
	// set up password field
	myNewPassword->clear();	
	myNewPassword->setEchoMode(QLineEdit::Password);
	myConfirmPassword->clear();	
	myConfirmPassword->setEchoMode(QLineEdit::Password);
	// initially disable OK button
	okButton->setEnabled( false );
	QMetaObject::connectSlotsByName( this );
}
//

void NewPasswordDialog::setDescription(const QString & desc)
{
	myDescription->setText(desc);
}

void NewPasswordDialog::setPrompt(const QString & prompt)
{
	setWindowTitle(prompt);
}

QString NewPasswordDialog::password()
{
	return myNewPassword->text();
}

void NewPasswordDialog::on_myNewPassword_textEdited(const QString & text)
{
	if( text.length() > 0 && ( text.compare( myConfirmPassword->text() ) == 0 ) ) {
		okButton->setEnabled( true );
	} else {
		okButton->setEnabled( false );
	}
}

void NewPasswordDialog::on_myConfirmPassword_textEdited(const QString & text)
{
	if( text.length() > 0 && ( text.compare( myNewPassword->text() ) == 0 ) ) {
		okButton->setEnabled( true );
	} else {
		okButton->setEnabled( false );
	}
}

NewPasswordDialog::~NewPasswordDialog()
{
}
