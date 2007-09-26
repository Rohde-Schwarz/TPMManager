/***************************************************************************
 *   Copyright (C) 2006 by Anoosheh Zaerin   *
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
 
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <tpmmanagerwidget.h>

static const char description[] =
    I18N_NOOP("A KDE KPart Application");

static const char version[] = VERSION;

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

int main(int argc, char **argv)
{
    KAboutData about("tpm_manager", I18N_NOOP("TPM_Manager"), version, description,
                     KAboutData::License_GPL, "(C) 2006 Anoosheh Zaerin", 0, 0, "anoosheh.zaerin@rub.de");
    about.addAuthor( "Anoosheh Zaerin", 0, "anoosheh.zaerin@rub.de" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    TPM_ManagerWidget *mainWin = 0;

    if (app.isRestored())
    {
        // RESTORE(TPM_Manager);
    }
    else
    {
        // no session.. just start up normally
        KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

        /// @todo do something with the command line args here

        mainWin = new TPM_ManagerWidget();
        app.setMainWidget( mainWin );
        mainWin->show();

        args->clear();
    }

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

