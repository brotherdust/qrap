 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : main.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Christo (magdaleen.ballot@up.ac.za) 
 *    Description : Not really ready to be used
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#include "server2.h"
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char description[] =
    I18N_NOOP("A KDE KPart Application");

static const char version[] = "0.1";

static KCmdLineOptions options[] =
{
//    { "+[URL]", I18N_NOOP( "Document to open" ), 0 },
    KCmdLineLastOption
};

class exe {
	int i;
};

int main(int argc, char **argv)
{
    KAboutData about("server2", I18N_NOOP("server2"), version, description,
                     KAboutData::License_Custom, "(C) 2008 qrap,,,", 0, 0, "qrap@QRAPserver");
    about.addAuthor( "qrap,,,", 0, "qrap@QRAPserver" );
    KCmdLineArgs::init(argc, argv, &about);
    KCmdLineArgs::addCmdLineOptions( options );
    KApplication app;
    server2 *mainWin = 0;
	

		if (app.isRestored())
		{
			RESTORE(server2);
		}
		else
		{
			// no session.. just start up normally
			KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	
			/// @todo do something with the command line args here
	
			mainWin = new server2();
			app.setMainWidget( mainWin );
			mainWin->show();
	
			args->clear();
		}

    // mainWin has WDestructiveClose flag by default, so it will delete itself.
    return app.exec();
}

