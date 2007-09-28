/***************************************************************************
 *   Copyright (C) 2007 by Anoosheh Zaerin   *
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

#include <config.h>

#include <kmainwindow.h>
#include <klocale.h>

#include "tpmmanager.h"
#include "tpmmanagerwidget.h"

tpmmanager::tpmmanager()
    : KMainWindow( 0, "tpmmanager" )
{
   QWidget *widget = new TPM_ManagerWidget( this );
   setCentralWidget( widget );
   // CONNECT( widget, buttonOK, SIGNAL( clicked() ), this, SLOT( close() ) );
	resize( sizeForCentralWidgetSize( widget->size() ) );
}

tpmmanager::~tpmmanager()
{
}

#include "tpmmanager.moc"
