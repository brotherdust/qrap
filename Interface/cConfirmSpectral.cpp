 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralAnalysis.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This is the interface part that initiates spectral analysis
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

#include "cConfirmSpectral.h"

//*********************************************************************************************
cConfirmSpectral::cConfirmSpectral() : QDialog()
{
	setupUi(this);
	this->setModal(true);
	LoadDefaults();
}

//*********************************************************************************************
cConfirmSpectral::cConfirmSpectral(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	setupUi(this);
	this->setModal(true);
	LoadDefaults();
}

//***********************************************************************************************
cConfirmSpectral::~cConfirmSpectral()
{
}

//*************************************************************************************************
bool cConfirmSpectral::SetPoints(QList<QgsPoint> Points)
{
	char *text= new char[33];
	if (Points.size() > 1)
	{
		double North = Points[0].y();
		double South = Points[0].y();
		double East = Points[0].x();
		double West = Points[0].x();
		string query;
		pqxx::result SitesIn;
		query = "SELECT siteid,sitename,ST_AsText(location) AS location,status,";
		query += " sector,radioinstallation_view.id AS radinst,"; 
		query += " techkey, technologytype,groundheight,status,project,flagx,flagz, ";
		query += " maxrange as radius";
		query += " FROM radioinstallation_view cross join technology cross join site_view ";
		query += " WHERE techkey=technology.id AND location ";
        	query +="@ ST_GeomFromText('POLYGON((";
	        for (int i = 0 ; i < Points.size();i++)
 	       	{
 		       	North = max(North,Points[i].y());
 		       	South = min(South,Points[i].y());
 		       	East = max(East,Points[i].x());
 		       	West = min(West,Points[i].x());
			gcvt(Points[i].x(),10,text);
  		      	query += text;
  		      	query += " ";
			gcvt(Points[i].y(),10,text);
        		query += text;
        		query += ",";
     		}
       		NorthWestCorner.Set(North,West,DEG);
        	SouthEastCorner.Set(South,East,DEG);
		gcvt(Points[0].x(),10,text);
  		query += text;
  		query += " ";
		gcvt(Points[0].y(),10,text);
        	query += text;
        	query += "))',4326) AND siteid = site_view.id";	
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database Select on sites table failed"<< endl;
			string err = "Database Select on sites table failed"; 
			QRAP_WARN(err.c_str());
			return false;
		}
		gDb.GetLastResult(SitesIn);
		if (SitesIn.size() >0)
		{
			tableInstallations->setRowCount(SitesIn.size());
			for (int i = 0; i < SitesIn.size();i++)
			{
				QCheckBox *Enabled = new QCheckBox(SitesIn[i]["sitename"].c_str());
				Enabled->setCheckState(Qt::Checked);
				tableInstallations->setCellWidget(i,0,Enabled);
				tableInstallations->setItem(i,1,new QTableWidgetItem(SitesIn[i]["sector"].c_str()));
				tableInstallations->setItem(i,2,new QTableWidgetItem(SitesIn[i]["radinst"].c_str()));
				tableInstallations->setItem(i,3,new QTableWidgetItem(SitesIn[i]["technologytype"].c_str()));
				tableInstallations->setItem(i,4,new QTableWidgetItem(SitesIn[i]["project"].c_str()));
				tableInstallations->setItem(i,5,new QTableWidgetItem(SitesIn[i]["flagx"].c_str()));
				tableInstallations->setItem(i,6,new QTableWidgetItem(SitesIn[i]["flagz"].c_str()));
				tableInstallations->setItem(i,7,new QTableWidgetItem(SitesIn[i]["status"].c_str()));
				tableInstallations->setItem(i,8,new QTableWidgetItem(SitesIn[i]["radius"].c_str()));
				string instname=SitesIn[i]["radinst"].c_str();
				instname+=":";
				instname+=SitesIn[i]["sitename"].c_str();
				comboRx->insertItem(instname.c_str());
			}
		}
		else
		{
			string err = "No Sites in selected area! ";
			QRAP_WARN(err.c_str());
			cout << err << endl;			
			return false;
		}
		return true;
	}
	delete [] text;
	return false;
}

//*************************************************************************************************************
void cConfirmSpectral::LoadDefaults()
{
	doubleFreqRes->setValue(1.0);
}

//*************************************************************************************************************
void cConfirmSpectral::on_pushCancel_clicked()
{
	close();
}

//************************************************************************************************************
void cConfirmSpectral::on_pushOk_clicked()
{
	double North,West,South,East;
	NorthWestCorner.Get(North,West);
	SouthEastCorner.Get(South,East);
	for (int i = 0; i < tableInstallations->rowCount();i++)
	{
		void *Wid = tableInstallations->cellWidget(i,0);
		QCheckBox *get =(QCheckBox*) Wid;
		if (get->isChecked())
		{
			mRadInst.push_back((unsigned)tableInstallations->item(i,2)->text().toDouble());
		}
	}
	mRx = atoi(comboRx->currentText().latin1());	
	mFrequencySpacing = doubleFreqRes->value()/1000;
	mkFactor = doublekFactor->value();
	accept();
}
