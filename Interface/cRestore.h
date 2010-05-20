 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRestore.h
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

#ifndef CRESTORE_H_
#define CRESTORE_H_

#include <stdio.h>
#include <stdlib.h>

#include <qgspoint.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QObject>

#include "ui_Restore.h"

using namespace std;

namespace Qrap
{
	class cRestore : public QDialog,Ui::RestoreDialog
	{Q_OBJECT

	public:
		cRestore();
		~cRestore();

		// set the username and password that will be used to log into the database
		void setUserName(string _username,string _password);

	public slots:
		void on_restoreChooseFilePushButton_clicked();
		void on_restorePushButton_clicked();
/*		void on_backupPushButton_clicked();
		void on_restorePushButton_clicked();
		void on_backupChooseFilePushButton_clicked();
		void on_restoreChooseFilePushButton_clicked();*/


	private:
		string username,password;
	};
};

#endif // CRESTORE_H_
