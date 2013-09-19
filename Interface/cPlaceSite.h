 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cPlaceSite.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : This is the interface class ease site placement and 
 *                : adding it to the database
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


#ifndef CPLACESITE_H_
#define CPLACESITE_H_

#include <qgisinterface.h>
#include <qgisgui.h>
//#include <qgsapplication.h>
#include "../DataBase/Config.h"
#include "../DataBase/MainWindow.h"
#include "../Prediction/cRaster.h"
#include "../Prediction/cGeoP.h"
#include "ui_Site.h"
#include <stdio.h>
#include <QMessageBox>
#include "../DataBase/RapTab.h"
#include "../DataBase/cSearchForm.h"
#include "../DataBase/cRapSites.h"
#include "../DataBase/cSiteEditTools.h"

using namespace std;
using namespace Qrap;

class cPlaceSite: public QDialog, private Ui::Site
{Q_OBJECT
public:
	cPlaceSite();
	cPlaceSite(QgisInterface *QgisInt, QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cPlaceSite();
	
	bool SetParameters(QString Lat,QString Lon,bool Edit,double scale);
	bool ChangeLocation(QString Lat,QString Lon);
	
public slots:
    	void on_btnPlaceSite_clicked();
    	void on_btnCancel_clicked();
	void on_btnMove_clicked();
    	void on_btnEditInstallation_clicked();
	void on_btnDefaultInst_clicked();
	
	signals:
	void MoveSiteClicked();

private:
	string machinename;
	int InsertSite(); // returns id of inserted cell ans -1 on failure
	bool InsertDefRad();
	bool UpdateSite();
//	QString FindLatLon(QString lat,QString lon);
	bool mEdit;
	double mScale;
	int mId;
	QgisInterface * mQGisIface;
	
};

#endif /*CPLACESITE_H_*/
