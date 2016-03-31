/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cUpdateToServer.h
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

#ifndef CUPDATETOSERVER_H_
#define CUPDATETOSERVER_H_

#include <stdio.h>
#include <stdlib.h>

#include <QTreeWidgetItem>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QObject>
#include <QDialog>

#include "ui_UpdateToServer.h"

using std::string;

namespace Qrap
{

	class cUpdateToServer: public QDialog,Ui::UpdateToServerDialog
	{Q_OBJECT
	public:
		cUpdateToServer();
		~cUpdateToServer();
#if 1
		// Set the Username and password that will be used to connect to the database
		void setUserName(string _username,string _password);

		// Sets the machine name to be used by this class.
		void setMachineName(string _machinename);

	public slots:
		void on_backupSelectFileName_clicked();
		void on_updateAllPushButton_clicked();
		void on_sendServerUpdateButton_clicked();
//		void on_getServerUpdatesPushButton_clicked();

	protected:
		/// Retrieves the settings that will be used for the update.
		void getUpdateSettings();

		// Will add all the updates from the server to the new local database
		void mergeServerUpdates(const char *fname);

		// Delete the sites that was deleted on the server, using a merge delete
		void mergeDelete(const char *fname);


		string hostname, portnum;
		string backupfname;
		string username,password;
		string machinename;
		char date[1024];
		char *dateP;
#endif
	};

}


#endif // CUPDATETOSERVER_H_
