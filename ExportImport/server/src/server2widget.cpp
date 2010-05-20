 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : server2widget.cpp
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

#include <qlabel.h>
#include <qpushbutton.h>
#include <klocale.h>
#include <qlineedit.h>

#include "fserver.h"

#include "server2widget.h"

#include <string>

using std::string;

server2Widget::server2Widget(QWidget* parent, const char* name, WFlags fl)
        : server2WidgetBase(parent,name,fl)
{
	/* allocate the server */
	fserver = new FServer();
	
	/* set the started state */
	started = false;
	
	/* set the buttons enabled state */
	startServerButton->setEnabled(!started);
	stopServerPushButton->setEnabled(started);
}

server2Widget::~server2Widget()
{
	/** delete the server */
	delete fserver;
}

/*$SPECIALIZATION$*/
void server2Widget::button_clicked()
{

}

void server2Widget::stopServerSlot()
{
	if (started) {
		// stop the server
		
		
		// set the buttons enabled state
		started = !started;
		startServerButton->setEnabled(!started);
		stopServerPushButton->setEnabled(started);
	}
}


void server2Widget::startServerSlot()
{
	unsigned int portnum;
	string username,password,hostname,dbname,portstr;
	string workingdir;


	if (!started) {
		// get the username and password
		username = string(serverUserNameLineEdit->text().ascii());
		password = string(serverPasswordLineEdit->text().ascii());
		hostname = string(serverHostnameLineEdit->text().ascii());
		dbname = string(serverDBNameLineEdit->text().ascii());
		portstr = string(portNumberLineEdit->text().ascii());
		workingdir = string(workingDirectoryLineEdit->text().ascii());

		// print the settings to stdout
		printf("server settings: \tworkingdir %s\n\tportnum %s\n\t hostname %s\n\t username %s \n",workingdir.c_str(),portstr.c_str(),hostname.c_str(),username.c_str());

		// get the port number 
		portnum = atoi(portstr.c_str());
		fserver->setPortNumber(portnum);
		
		// get the root directory 
		fserver->setRootDirectory(workingdir.c_str());

		// set the db info
		fserver->setDBInfo(username.c_str(),password.c_str(),dbname.c_str(),hostname.c_str());
		
		// start the server
		fserver->runServer();
	
		// set the buttons enabled state
		started = !started;
		startServerButton->setEnabled(!started);
		stopServerPushButton->setEnabled(started);
	}
}




#include "server2widget.moc"

