 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cImportUpdateFile.h
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

#ifndef CIMPORTUPDATEFILE_H_
#define CIMPORTUPDATEFILE_H_

#include <stdio.h>
#include <stdlib.h>

#include <qgspoint.h>
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QObject>

#include "ui_ImportUpdateFile.h"


using namespace std;

namespace Qrap
{
	class cImportUpdateFile : public QDialog,Ui::ImportUpdateFileDialog
	{Q_OBJECT

	public:
		cImportUpdateFile();
		~cImportUpdateFile();

		// set the username and password that will be used to log into the database
		void setUserName(string _username,string _password);

	public slots:
		void on_fileSelectionPushButton_clicked();
		void on_importFilePushButton_clicked();

	private:
		string username,password;
		string filename;
	};
};



#endif // CIMPORTUPDATEFILE_H_

