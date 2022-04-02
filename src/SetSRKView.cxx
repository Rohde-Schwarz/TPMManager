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
* @file SetSRKView.cpp
*
* @brief SRK Password Choose Dialog Class Implementation File
*
**/

#include "SetSRKView.hxx"
#include <iostream>
#include <QString>
#include <QMessageBox>

using namespace std;

SetSRKView::SetSRKView( QWidget * parent, Qt::WindowFlags f) 
	: QDialog(parent, f),
	myManuallySRKRadioButton(false),
	myDefaultSRKRadioButton(false)
{
	setupUi(this);
}

void SetSRKView::slotSetSrk()
{
    if ( radioButtonSrkManually->isChecked() ) {
        myManuallySRKRadioButton = true;
        myDefaultSRKRadioButton = false;
    }
    if ( radioButtonSrkDefault->isChecked() ) {
        myDefaultSRKRadioButton = true;
        myManuallySRKRadioButton = false;
    }

    if ( !myDefaultSRKRadioButton && !myManuallySRKRadioButton )
        QMessageBox::critical( this, "Error: No radio button chosen " ," Choose an option to set SRK. " );
}

SetSRKView::~SetSRKView()
{
}

