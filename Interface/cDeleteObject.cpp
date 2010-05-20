 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cDeleteObject.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : (Magdaleen Ballot) (magdaleen.ballot@up.ac.za) 
 *    Description : This is the interface class to delete GUI objects
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

#include "cDeleteObject.h"
//******************************************************************
cDeleteObject::cDeleteObject(): QDialog()
{
	setupUi(this);
	this->setModal(true);
}

//******************************************************************
cDeleteObject::cDeleteObject(QWidget* parent, Qt::WFlags fl): QDialog(parent, fl)
{
	cout << "in deleteObject constructer" << endl;
  	setupUi(this);
	cout << "Na " << endl;
  	this->setModal(true);
 }  

//******************************************************************
cDeleteObject::~cDeleteObject()
{
}


//******************************************************************
bool cDeleteObject::SetParameters(QString Lat,QString Lon,bool site,double scale)
{
	QString Label = "Are you sure you want to delete: "; 
	string query;
	pqxx::result r;
	mSite = site;
	if (site)
	{
		string SiteSense;
		pqxx::result SiteCloseby;
		cGeoP Here,There;
		There.SetGeoType(DEG,-1);
		double North,South,East,West;
		Here.Set(Lat.toDouble(),Lon.toDouble(),DEG,-1);
		SiteSense = gDb.GetSetting("SiteSelectSense");
		if (SiteSense == "")
			SiteSense = "150";
		There.FromHere(Here,(scale/atof(SiteSense.c_str())),315);
		There.SetGeoType(DEG);
		There.Get(North,West);
		There.FromHere(Here,(scale/atof(SiteSense.c_str())),135);
		There.SetGeoType(DEG);
		There.Get(South,East);
		query = "SELECT id,sitename,AsText(location) AS location,groundheight,status FROM site WHERE location ";
		query +="&& SetSRID('BOX3D(";
		char text[12];
		gcvt(West,10,text);
		query += text;
		query +=" ";
		gcvt(North,10,text);
		query += text;
		query +=", ";		
		gcvt(East,10,text);
		query += text;
		query +=" ";	
		gcvt(South,10,text);
		query += text;
		query += ")'::box3d,4326);";
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database Select on sites table failed"<< endl;
			close();
		}
		gDb.GetLastResult(SiteCloseby);
		if (SiteCloseby.size() >0)
		{
//			char B[255];
//			string Location;
//			Location = SiteCloseby[0]["location"].c_str();
//			Location = Location.substr(6,Location.length()-7);
//			strcpy(B,Location.c_str());
			Label += SiteCloseby[0]["sitename"].c_str();
			id = (int)atof(SiteCloseby[0]["id"].c_str());
			
		}
		else
		{
			return false;
		}
	}
	else
	{
		string query = "SELECT id,linkname ";
		query += "FROM links WHERE line && GeomFromText('POINT(";
		query +=  Lon.latin1();
		query += " ";
		query += Lat.latin1();
		query += ")',4326)";
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database link select failed"<< endl;
		}
		else
		{
			pqxx::result Li;
			gDb.GetLastResult(Li);
			if (Li.size())
			{
				Label += Li[0]["linkname"].c_str();
				id = (int)atof(Li[0]["id"].c_str());
			}
		}
	}
	label->setText(Label);
	return true;
}

//******************************************************************
void cDeleteObject::on_btnOk_clicked()
{
	string query;
	char *temp = new char[33];
	gcvt(id,8,temp);
	if (mSite)
	{
		query = "DELETE FROM site where id = ";
	}
	else
	{
		query = "SELECT radioinstallation1 as tx, radioinstallation2 as rx from links where id =";
		query += temp;
		if (gDb.PerformRawSql(query))
		{
			pqxx::result RI;
			gDb.GetLastResult(RI);
			query = "Delete from radioinstallation where id in (";
			query += RI[0]["tx"].c_str();
			query += ",";
			query += RI[0]["rx"].c_str();
			query += ");";
			gDb.PerformRawSql(query);
		}
		query = "DELETE FROM links where id = ";
	}
	query += temp;
	gDb.PerformRawSql(query);
	delete [] temp;
	accept();
}

//******************************************************************
void cDeleteObject::on_btnCancel_clicked()
{
	close();
}

