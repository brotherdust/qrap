 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cImportExportMenu.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Christo du Plessis (magdaleen.ballot@up.ac.za) 
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

#ifndef CIMPORTEXPORTMENU_H_
#define CIMPORTEXPORTMENU_H_

#include <stdio.h>
#include <stdlib.h>

#include <qgspoint.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QDialog>

#include "ui_ImportExportMenu.h"

using namespace std;

namespace Qrap 
{
	class cImportExportMenu : public QDialog,Ui::ImportExportMenuDialog
	{Q_OBJECT
	public:
		cImportExportMenu();
		~cImportExportMenu();

		// set the database username and password
		void setDB(string _dbusername,string _dbpassword);
		void setMachine(string _machinename);

	public slots:
		void on_createExportFilePushButton_clicked();
		void on_importFilePushButton_clicked();
		void on_createBackupFilePushButton_clicked();
		void on_restoreBackupPushButton_clicked();
		void on_updateToServerPushButton_clicked();
		void on_runServerPushButton_clicked();
		void on_helpPushButton_clicked();

	private:
		string dbusername,dbpassword;
		string machinename;
	};
};


#endif // CIMPORTEXPORTMENU_H_
