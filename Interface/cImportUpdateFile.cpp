 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cImportUpdateFile.cpp
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


#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <qfiledialog.h>

using namespace std;

#include "cImportUpdateFile.h"
#include "../ExportImport/flatimport/importf.h"

using namespace Qrap;


cImportUpdateFile::cImportUpdateFile()
{
	setupUi(this);
	setModal(true);
}


cImportUpdateFile::~cImportUpdateFile()
{

}

void cImportUpdateFile::setUserName(string _username,string _password)
{
	username = _username;
	password = _password;
}


void cImportUpdateFile::on_fileSelectionPushButton_clicked()
{
	printf("cImportUpdateFile::on_fileSelectionPushButton_clicked\n");

	// get the filename
	QString fileName = QFileDialog::getOpenFileName("/home",
			    "Import Files (*.txt *.txd *.*)",
			    this,
			    "open file dialog",
			    "Choose a file" );

	// check that a string was selected
	if (!fileName.isNull()) {
		// save the filename
		fileNameLineEdit->setText(fileName);
		filename = string(fileName.ascii());
		printf("%s\n",filename.c_str());
	}
}


void cImportUpdateFile::on_importFilePushButton_clicked()
{
	printf("cImportUpdateFile::on_importFilePushButton_clicked()\n");

	// create the import object
	ImportF imf;

	// connect to the database
	imf.connectDB("localhost","qrap",username.c_str(), password.c_str());

	// import the file
	imf.importFile(filename.c_str());	
}
