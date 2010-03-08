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
* @mainpage TPM Manager
*
* The TPM Manager is an open source software for managing Trusted Platform Modules (TPM)
* developed by Sirrix AG in cooperation with Ruhr University Bochum. It features an easy to use graphical
* user interface and can be used on every platform shipped with a TPM that is supported
* by the Linux Kernel.
* <br>
*
* @author Anoosheh Zaerin <a href="mailto:a.zaerin@sirrix.com">&lt;a.zaerin@sirrix.com&gt;</a>
* @author Ren&eacute; Korthaus <a href="mailto:r.korthaus@sirrix.com">&lt;r.korthaus@sirrix.com&gt;</a>
*
* You can find more documentation for download on the project page. See <a href="https://projects.sirrix.com/trac/tpmmanager">https://projects.sirrix.com/trac/tpmmanager</a>.
*/

/**
* @file Main.cxx
*
* @brief TPM Manager Main Application File
*
**/

#include <QApplication>
#include "TPMManager.hxx"
#include <string.h>
/// Main application loop
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	TPM_Manager win;
	/* fullscreen mode */
	if( argc > 1 && argv++ && ( strcmp(*argv, "-fs") == 0 || strcmp(*argv, "-fullscreen") == 0 ) )
		win.showMaximized();
	else	
		win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
