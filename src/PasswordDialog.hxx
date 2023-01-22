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

#ifndef PASSWORDDIALOG_H
#define PASSWORDDIALOG_H

#include <QDialog>
#include "ui_PasswordDialog.h"

/**
* @file PasswordDialog.hxx
*
* @brief Password Dialog Class Header File
*
**/

/**
* @class PasswordDialog
*
* @brief Implements a user dialog to enter a password (e.g. owner password)
*
* This class implements a user dialog to enter a password. The password is used to authenticate the user to the TPM (e.g. through owner password). The base class is automatically generated as "ui_PasswordDialog".
*
*/ 

class PasswordDialog : public QDialog, public Ui::PasswordDialog
{
Q_OBJECT
public:
	/**
	* @brief Default constructor
	*
	* @param parent parent QWidget object
	* @param f window flags
	*
	* @return New PasswordDialog instance
	*
	*/
	PasswordDialog( QWidget * parent = 0, Qt::WindowFlags f = QFlag( 0 ) );
	
	/**
	* @brief Default destructor
	*
	*/
	~PasswordDialog();	
	
	/**
	* @brief Sets the descriptive text of the password to enter in the dialog
	*
	* @param desc descriptive text
	*
	*/
	void setDescription(const QString & desc);
	
	/**
	* @brief Sets the password dialog title
	*
	* @param prompt password dialog title
	*
	*/
	void setPrompt(const QString & prompt);
	
	/**
	* @brief Gets the password the user entered
	*
	* @return The new password
	*
	*/
	QString password();
	
private slots:
	/**
	* @brief Slot called whenever text is edited in the password text field
	*
	* @param text the current text entered into the text field 
	*/
	void on_myPassword_textEdited(const QString & text);
};
#endif
