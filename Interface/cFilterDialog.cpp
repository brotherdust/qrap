 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cFilterDialog.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe (magdaleen.ballot@up.ac.za) 
 *    Description : This is the interface class to create Custom Filter Areas
 *
 **************************************************************************
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

#include "cFilterDialog.h"
using namespace Qrap;
//********************************************************
cFilterDialog::cFilterDialog() : QDialog()
{
	Setup();
}

/***********************************************************/
cFilterDialog::cFilterDialog(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	Setup();
}


//*******************************************************
cFilterDialog::~cFilterDialog()
{
}

//*******************************************************
void cFilterDialog::Setup()
{
	setupUi(this);
	this->setModal(true);
}

//*******************************************************
void cFilterDialog::SetPoints(QList<QgsPoint> Points)
{
	mPoints.clear();
	mPoints = Points;
}

//*******************************************************
void cFilterDialog::accept()
{
	char * text = new char[33];
	cout << lineEditName->text().toStdString() << endl;
	if (mPoints.size() > 1)
	{
		string query;
		query = "INSERT INTO customareafilter (areaname,the_geom) VALUES ('";
		query += lineEditName->text().toStdString();
		query += "', ST_GeomFromText('POLYGON((";
        	for (int i = 0 ; i < mPoints.size();i++)
        	{
			gcvt(mPoints[i].x(),10,text);
        		query += text;
        		query += " ";
			gcvt(mPoints[i].y(),10,text);
        		query += text;
        		query += ",";
        	}
		gcvt(mPoints[0].x(),10,text);
        	query += text;
        	query += " ";
		gcvt(mPoints[0].y(),10,text);
        	query += text;
        	query += "))',4326));";	
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database custom area filter insert failed in cFilterDialog::accept()"<< endl;
			return;
		}
	}
	delete [] text;
	gDb.SetSetting("FilterAreaType","customareafilter");
	gDb.SetSetting("FilterAreaField","areaname");
	gDb.SetSetting("FilterArea",lineEditName->text().toStdString());
	gMainWindow = new MainWindow();
	cFilter* Filter = dynamic_cast<cFilter*>(gMainWindow->Filter);
	Filter->LoadDefaults();
	Filter->ChangeAlterButton();
	gMainWindow->show();
	close();

}
