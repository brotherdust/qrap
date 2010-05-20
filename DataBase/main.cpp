 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : main.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : Qt4 GUI for the Qrap project

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

#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QBitmap>
#include <QTimer>

#include "Config.h"
#include "MainWindow.h"
#include "LoginDialog.h"

using namespace Qrap;
using namespace std;

int main(int argc, char *argv[])
{
	Q_INIT_RESOURCE(application);
	
	QApplication   app(argc, argv);
	QPixmap        pixmap("images/rap_logo.png");
	QSplashScreen* splash = new QSplashScreen(pixmap,Qt::WindowStaysOnTopHint);
	bool           error = false;
	
	
	splash->setMask(pixmap.mask());
	splash->show();
	app.processEvents();
	
	cout << "Loading settings..." << endl;
	
	if (!gSettings.LoadFromFile("settings.xml"))
	{
		cout << "Error while loading settings from settings.xml" << endl;
		return 1;
	}
	
	cout << "Loaded settings." << endl;
	
	LoginDialog* loginDialog;
	
	loginDialog = new LoginDialog(0);
	QTimer::singleShot(1500,loginDialog,SLOT(show()));
	
	QTimer::singleShot(1500,splash,SLOT(close()));
//	gMainWindow = new MainWindow();
//	gMainWindow->show();
	return app.exec();
}
