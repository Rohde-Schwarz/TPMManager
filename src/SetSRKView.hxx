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

#ifndef SETSRKVIEW_H
#define SETSRKVIEW_H
//
#include <QDialog>
#include "ui_SetSRKDialog.h"
#include <QRadioButton>

/**
* @file SetSRKView.hxx
*
* @brief SRK Password Choose Dialog Class Header File
*
**/

/**
* @class SetSRKView
*
* @brief Implements a user dialog to choose a Storage Root Key (SRK) secret.
*
* This class implements a user dialog to choose a SRK secret. The base class is automatically generated as "ui_setsrkdialog.h".
*
*/

class SetSRKView : public QDialog, public Ui::SetSRKDialog
{
Q_OBJECT
public:
	/**
	* @brief Default constructor
	*
	* @param parent parent QWidget object
	* @param f window flags
	*
	* @return New SetSRKView instance
	*
	*/
	SetSRKView( QWidget * parent = 0, Qt::WindowFlags f = 0 );
	
	/**
	* @brief Default destructor
	*
	*/
	~SetSRKView();
	
	/**
	* @brief Get user's choice on SRK secret
	*
	* @return whether user chose to set SRK secret manually or not
	*/
	inline bool setManually();

protected slots:
	/**
	* @brief Slot called whenever user accepts dialog (OK)
	*
	*/
	void slotSetSrk();	
	
private:
	bool myManuallySRKRadioButton;	
	bool myDefaultSRKRadioButton;
};

bool SetSRKView::setManually(){
	if ( myManuallySRKRadioButton )
		return true;
	else 
		return false;
}

#endif





