 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cUpdateToServer.cpp
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

#include <string>
#include <vector>
#include <pqxx/pqxx>

//using namespace pqxx;

#include "cUpdateToServer.h"
#include "../DataBase/cBase.h"
#include "../DataBase/cDatabase.h"
#include "../ExportImport/flatexport/exportf.h"
#include "../ExportImport/flatimport/importf.h"
#include "../ExportImport/libfcs/fclient.h"
#include "../ExportImport/mergedelete/mergedelete.h"
#include "../ExportImport/libfcs/idresolve.h"

#include <string>
#include <QFileDialog>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>

const char *tmpfname  = "tmp.txd";
const char *failedconnectstr = "Failed to connect to the server. Check connection settings and try again";
const char *appname = "QRap";
const char *infostr = "info";
const char *updatefname = "update.txd";

using namespace Qrap;


cUpdateToServer::cUpdateToServer()
{
	setupUi(this);
	this->setModal(true);
}

cUpdateToServer::~cUpdateToServer()
{}

void cUpdateToServer::on_backupSelectFileName_clicked()
{
	printf("cUpdateToServer::on_backupSelectFileName_clicked\n");

	// get the backup file
	QString fileName = QFileDialog::getSaveFileName(this,
		"save file",
		"/home/christo/Documents/qrap/",
		"Backup Files (*.tar *.*)");

	// set the textbox
	if (!fileName.isNull()) {
		// set the filename
		backupFileNameLineEdit->setText(fileName);
	}
}

void cUpdateToServer::on_updateAllPushButton_clicked()
{
	FClient fclient;
	int portn;
	int id;
	char buf[1024];
	int ret;
	int hid;

	printf("cUpdateToServer::on_updateAllPushButton_clicked\n");
	hid = gethostid();

	// get the update settings
	getUpdateSettings();	

	// set the port number of the server to connect to
	portn = atoi(portnum.c_str());
	fclient.setServerPort(portn);

	// set the server hostname
	fclient.setServerName(hostname.c_str());

	// perform the connection
	id = fclient.requestID("");

	// set the global id
	gDb.globalMachineID = id;


	IdResolve idr;

	// connect to the local database
	ret = idr.connectDB("localhost","qrap",username.c_str(),password.c_str());
	if (ret<0) {
		QMessageBox::information(this,appname,"Failed to connect to local database");
	}

	// add the ID (check if it is already added)
	idr.resolveId(hid,"localid");
}

void cUpdateToServer::on_sendServerUpdateButton_clicked()
{
	int portn;
	int ret;
	char buffer[1024];
	char buf[1024];
	char fname[1024];
	char mname[1024];
	


	printf("cUpdateToServer::on_updateAllPushButton_clicked\n");

	// get the machine name
	sprintf(mname,"%d",gDb.globalMachineID);

	// retrieve the connection settings
	getUpdateSettings();

	// create the export object
	ExportF ef;

	// connect to the database
	ef.connectDB("localhost","qrap",username.c_str(),password.c_str());

	// export the file
	getcwd(buffer,1024);
	sprintf(fname,"%s/%s",buffer,tmpfname);
	ef.exportFile(fname,mname,dateP);
	printf("fname [%s]\n",fname);
	printf("QQQ machine name [%s]\n",mname);

	// setup the server
	FClient fclient;

	// set the port number of the server to connect to
	portn = atoi(portnum.c_str());
	fclient.setServerPort(portn);

	// set the server hostname
	fclient.setServerName(hostname.c_str());
	
	// send the file
	ret = fclient.sendFile(tmpfname);

	// show a message box if there was a failure
	if (!ret) {
		QMessageBox::information(this,appname,failedconnectstr,"Ok");
	}

	// get the server updates
	ret = fclient.getFile(updatefname);

	// show an error message if there was a failure
	if (!ret) {
		sprintf(buf,"Failed to fetch file [%s] from the server",updatefname);
		QMessageBox::information(this,appname,buf);
	}

	// print the cwd
	getcwd(buf,1024);
	printf("cwd [%s]\n",buf);
	printf("filename [%s]\n",updatefname);

	// merge the server side updates
	mergeServerUpdates(updatefname);

	// delete the sites that was deleted on the server
	mergeDelete(updatefname);
}

void cUpdateToServer::mergeDelete(const char *fname)
{
	MergeDelete *md;
	int ret;
	char buf[2048];
	char cwd[2048];

	printf("\n\ncUpdateToServer::mergeDelete\n");

	// create the merge delete structure 
	md = new MergeDelete();

	// connect to the local database
	ret = md->connectDB("localhost", "qrap", username.c_str(), password.c_str());

	// if an error occured, report it to the user
	if (!ret) {
		sprintf(buf,"cUpdateToServer::mergeDelete: Failed to connect to database. Username %s password %s database qrap",username.c_str(),password.c_str());
		QMessageBox::information(this,appname,buf);
		return ;
	}

	// get the full filename (assuming the file is in the cwd)
	getcwd(cwd,1024);	
	sprintf(buf,"%s/%s",cwd,fname);

	// merge delete the unnecessary entries
	printf("calling mergedelete with [%s]\n",buf);
	md->mergeDelete(buf);

	// free the merge delete structure
	delete md;
}

void cUpdateToServer::mergeServerUpdates(const char *fname)
{
	ImportF *imf;
	int ret;
	char buf[1024];
	char cwd[1024];

	printf("\n\ncUpdateToServer::mergeServerUpdates\n");
	
	// create a new import structure
	imf = new ImportF();

	// make the database connection 
	ret = imf->connectDB("localhost", "qrap", username.c_str(), password.c_str());

	// print an error message if the connection failed
	if (!ret) {
		sprintf(buf,"Failed to connect to database. Username %s password %s database qrap",username.c_str(),password.c_str());
		QMessageBox::information(this,appname,buf);
	}

	// get the full filename (assuming the file is in the cwd)
	getcwd(cwd,1024);	
	sprintf(buf,"%s/%s",cwd,fname);
	printf("full fname [%s]\n",buf);

	// import the file
	ret = imf->importFile(buf);
	if (!ret) {
		sprintf(buf,"cUpdateToServer::mergeServerUpdates: Failed to import file [%s]",fname);
		QMessageBox::information(this,appname,buf);
	}
	
	// delete the import structure
	delete imf;	
}
#if 0
void cUpdateToServer::on_getServerUpdatesPushButton_clicked()
{
	printf("cUpdateToServer::on_updateAllPushButton_clicked\n");
}
#endif


void cUpdateToServer::getUpdateSettings()
{
	// get the hostname and port number
	hostname = string(serverHostNameLineEdit->text().ascii());
	portnum = string(serverPortNumberLineEdit->text().ascii());
	backupfname = string(backupFileNameLineEdit->text().ascii());

	// check if the checkbox is checked
	if (filterUpdatesByDateCheckBox->isChecked()) {
		strcpy(date,dateLineEdit->text().ascii());
		dateP = date;
	} else {
		dateP = NULL;
		strcpy(date,"");
	}

	// print the connection settings
	printf("hostname %s\nportnum %s\nbackupfname %s\n",hostname.c_str(),portnum.c_str(),backupfname.c_str());
}

void cUpdateToServer::setUserName(string _username,string _password)
{
	username = _username;
	password = _password;
}

void cUpdateToServer::setMachineName(string _machinename)
{
	machinename = _machinename;	
}


