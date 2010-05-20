 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRestore.cpp
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

#include "cRestore.h"

#include <string>
#include <qfiledialog.h>

#include "../ExportImport/backup/backup.h"
#include "../ExportImport/backup/restore.h"

using namespace Qrap;

#define STRLEN 512

cRestore::cRestore(): QDialog()
{
	setupUi(this);
	this->setModal(true);
}

cRestore::~cRestore()
{}

void cRestore::setUserName(string _username,string _password)
{
	username = _username;
	password = _password;
}

void cRestore::on_restorePushButton_clicked()
{
	Restore rs;
	char fname[STRLEN];
	QString fileName;

	// get the filename
	fileName = restoreLineEdit->text();
	strcpy(fname,fileName.ascii());
	
	// print the filename
	printf("[%s]\n",fname);

	// connect to the database
	rs.connectDB("localhost","qrap",username.c_str(),password.c_str());

	// restore the files
	rs.restoreFile(fname);	
}


void cRestore::on_restoreChooseFilePushButton_clicked()
{
	// get the filename
	QString fileName = QFileDialog::getOpenFileName("/home/qrap",
			    "Backup Files (*.tar  *.*)",
			    this,
			    "open file dialog",
			    "Choose a file" );

	// check that a string was selected
	if (!fileName.isNull()) {
		// save the filename
		restoreLineEdit->setText(fileName);
		printf("[%s]\n",fileName.ascii());
	}
}



