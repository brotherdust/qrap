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

#include "cImportExportMenu.h"

#include <string>
#include <qfiledialog.h>

#include "cBackup.h"
#include "cRestore.h"
#include "cCreateUpdateFile.h"
#include "cImportUpdateFile.h"
#include "cUpdateToServer.h"

using namespace Qrap;

cImportExportMenu::cImportExportMenu() {
	setupUi(this);
	setModal(true);

	printf("CALLED!!! cImportExportMenu::cImportExportMenu cImportExportMenu::cImportExportMenu\n");
}

cImportExportMenu::~cImportExportMenu()
{}

void cImportExportMenu::setDB(string _dbusername,string _dbpassword)
{
	dbusername = _dbusername;
	dbpassword = _dbpassword;
}

void cImportExportMenu::setMachine(string _machinename)
{
	machinename = _machinename;
}

void cImportExportMenu::on_createExportFilePushButton_clicked()
{
	// create a new update file dialog
	cCreateUpdateFile *cufdialog = new cCreateUpdateFile();
	cufdialog->setUserName(dbusername,dbpassword,machinename);

	// display the dialog
	cufdialog->show();
}

void cImportExportMenu::on_importFilePushButton_clicked()
{
	// create a import file dialog
	cImportUpdateFile *iufdialog = new cImportUpdateFile();
	iufdialog->setUserName(dbusername,dbpassword);

	// display the dialog
	iufdialog->show();

}

void cImportExportMenu::on_createBackupFilePushButton_clicked()
{
	printf(" cImportExportMenu::on_createBackupFilePushButton_clicked()\n");

	// create a new backup dialog
	cBackup *backupDialog = new cBackup();
	backupDialog->setUserName(dbusername,dbpassword);

	// show the dialog
	backupDialog->show();
}

void cImportExportMenu::on_restoreBackupPushButton_clicked()
{
	// create the restore dialog
	cRestore *restoreDialog = new cRestore();
	restoreDialog->setUserName(dbusername,dbpassword);

	// show the dialog
	restoreDialog->show();
}

void cImportExportMenu::on_updateToServerPushButton_clicked()
{
	// create the update to server dialog
	cUpdateToServer *updateToServer = new cUpdateToServer();

	// set the user name and password
	updateToServer->setUserName(dbusername,dbpassword);

	// set the machine name
	updateToServer->setMachineName(machinename);

	// show the dialog
	updateToServer->show();
}

void cImportExportMenu::on_helpPushButton_clicked()
{
	// launch the help in a standalone viewer
	system("evince /usr/share/doc/qrap/importexport.pdf&");	
}

void cImportExportMenu::on_runServerPushButton_clicked()
{
	system("nohup qrapserver &");
}

	
