 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCreateUpdateFile.cpp
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

#include "cCreateUpdateFile.h"
#include <qfiledialog.h>

#include "../ExportImport/flatexport/exportf.h"

using namespace Qrap;

#define STRLEN 512


cCreateUpdateFile::cCreateUpdateFile():
	QDialog()
{
	setupUi(this);
	this->setModal(true);
}

cCreateUpdateFile::~cCreateUpdateFile()
{}

void cCreateUpdateFile::setUserName(string _username,string _password,string _machineid)
{
	username = _username;
	password = _password;
	machineid = _machineid;
}

void cCreateUpdateFile::on_selectFileNamePushButton_clicked()
{
	// get the filename using a dialog
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
			"/home/qrap/export.txd",
			tr("Images (*.txt *.txd)"));

	// set the text of the line edit
	if (!fileName.isNull()) {
		printf("%s\n",fileName.ascii());
		updateFileLineEdit->setText(fileName);
		filename = string(fileName.ascii());
	}
}

void cCreateUpdateFile::on_createUpdateFilePushButton_clicked()
{
	bool fullupdate;

	// retrieve the value of the text box
	fullupdate = fullUpdateFileCheckBox->isChecked();

	// create the export object
	ExportF ef;

	// connect to the database
	ef.connectDB("localhost","qrap",username.c_str(), password.c_str());

	// export the file
	if (fullupdate)  {
		ef.exportFile(filename.c_str(),NULL);
	} else {
		ef.exportFile(filename.c_str(),machineid.c_str());
		printf("machine ID %s\n",machineid.c_str());
	}
}


