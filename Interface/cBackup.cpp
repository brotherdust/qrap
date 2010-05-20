 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Backup.cpp
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

#include "cBackup.h"

using namespace Qrap;

#define STRLEN 512

cBackup::cBackup(): QDialog()
{
	setupUi(this);
	this->setModal(true);
}

cBackup::~cBackup()
{}

void cBackup::setUserName(string _username,string _password)
{
	username = _username;
	password = _password;
}

void cBackup::on_backupPushButton_clicked()
{
	Backup bk;
	QString fileName;
	char fname[STRLEN];

	// get the output file name and convert to a null terminated c string
	fileName = backupLineEdit->text();
	strcpy(fname,fileName.ascii());

	// print the filename
	printf("[%s]\n",fname);

	// connect to the db
	bk.connectDB("localhost","qrap",username.c_str(),password.c_str());

	// backup the tables to a file (using an internal list)
	bk.backup(fname);
}


void cBackup::on_backupChooseFilePushButton_clicked()
{
	printf("cBackup::on_backupChooseFilePushButton_clicked\n");

	// get the filename from the dialog
	QString fileName = QFileDialog::getSaveFileName(this,
		"save file",
		"/home/qrap",
		"Backup Files (*.tar *.*)");

	// set the textbox
	// check that a string was selected
	if (!fileName.isNull()) {
		// save the filename
		backupLineEdit->setText(fileName);
		printf("[%s]\n",fileName.ascii());
	}
}

