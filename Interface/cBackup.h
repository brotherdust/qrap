 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Backup.h
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

#ifndef CBACKUP_H_
#define CBACKUP_H_

#include <stdio.h>
#include <stdlib.h>

#include <qgspoint.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QObject>
#include <string>
#include <qfiledialog.h>
#include "../ExportImport/backup/backup.h"
#include "../ExportImport/backup/restore.h"
#include "ui_Backup.h"

using namespace std;

namespace Qrap
{
	class cBackup : public QDialog,Ui::BackupDialog
	{Q_OBJECT

	public:
		cBackup();
		~cBackup();

		// set the username and password that will be used to log into the database
		void setUserName(string _username,string _password);

	public slots:
		void on_backupPushButton_clicked();
		void on_backupChooseFilePushButton_clicked();

	private:
		string username,password;
	};
};

#endif // CBACKUP_H_

