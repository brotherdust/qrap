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

#ifndef CCREATEUPDATEFILE_H_
#define CCREATEUPDATEFILE_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <qgspoint.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QObject>

#include "ui_CreateUpdateFile.h"

using namespace std;


namespace Qrap
{
	class cCreateUpdateFile : public QDialog,Ui::CreateUpdateFileDialog
	{Q_OBJECT

	public:
		cCreateUpdateFile();
		~cCreateUpdateFile();

		// set the username and password that will be used to log into the database
		void setUserName(string _username,string _password,string _machineid);

	public slots:
		void on_selectFileNamePushButton_clicked();
		void on_createUpdateFilePushButton_clicked();

	private:
		string username,password;
		string filename;
		string machineid;
	};
};


#endif //


