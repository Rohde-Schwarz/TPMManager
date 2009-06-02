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

#ifndef NEWPASSWORDDIALOG_H
#define NEWPASSWORDDIALOG_H

#include <QDialog>
#include "ui_NewPasswordDialog.h"

/**
* @file NewPasswordDialog.h
*
* @brief New Password Dialog Class Header File
*
**/

/**
* @class NewPasswordDialog
*
* @brief Implements a user dialog to choose a new password (e.g. owner password)
*
* This class implements a user dialog to choose a new password. The base class is automatically generated as "ui_NewPasswordDialog".
*
*/ 

class NewPasswordDialog : public QDialog, public Ui::NewPasswordDialog
{
Q_OBJECT
public:
	/**
	* @brief Default constructor
	*
	* @param parent parent QWidget object
	* @param f window flags
	*
	* @return New NewPasswordDialog instance
	*
	*/
	NewPasswordDialog( QWidget * parent = 0, Qt::WFlags f = 0 );
	
	/**
	* @brief Default destructor
	*
	*/
	~NewPasswordDialog();	
	
	/**
	* @brief Sets the descriptive text of the password to choose in the dialog
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
	* @brief Slot called whenever text is edited in the new password field
	*
	* @param text the current text entered into the text field 
	*/
	void on_myNewPassword_textEdited(const QString & text);
	
	/**
	* @brief Slot called whenever text is edited in the confirm new password field
	*
	* @param text the current text entered into the text field 
	*/
	void on_myConfirmPassword_textEdited(const QString & text);
};
#endif
