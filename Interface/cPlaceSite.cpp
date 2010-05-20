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

#include "cPlaceSite.h"
//***********************************************************
cPlaceSite::cPlaceSite(): QDialog()
{
	setupUi(this);
	this->setModal(true);
}

//***********************************************************
cPlaceSite::cPlaceSite(QgisInterface * QgisInt, QWidget* parent, 
			Qt::WFlags fl): QDialog(parent, fl)
{
	int j=0;
	setupUi(this);
	this->setModal(true);
	string table="site";
	string field="status";
	cDatabase::FieldUiType uiType;
	StringIntArray vals;
	StringIntArrayIterator iterator;
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);

	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		comboBoxStatus->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	if (j==0) comboBoxStatus->insertItem(0,"0:NULL");
}
  
//***********************************************************
cPlaceSite::~cPlaceSite()
{
}

//***********************************************************
bool cPlaceSite::SetParameters(QString Lat,QString Lon,bool Edit,double scale)
{
	mScale = scale;
	string query;
	pqxx::result r;
	mEdit = Edit;
	DegreeFormat locationFormat;
	string setting = gDb.GetSetting("location");
			
	if (setting=="DD:MM:SS")
		locationFormat = dfDegMinSec;
	else if (setting=="DD:MM.mm")
		locationFormat = dfDegMin;
	else 
		locationFormat = dfDeg;
	double lat = Lat.toDouble();
	double lon = Lon.toDouble();
	cout << "lat: " << lat << "   lon: " << lon << endl;
	string Location = QString ("POINT(%1 %2)").arg(Lon).arg(Lat).toStdString();

	cout  << Location << endl;
	if (mEdit)
	{
		btnPlaceSite->setText("Update");
		setWindowTitle(QApplication::translate("Site", "Edit the Site", 0, QApplication::UnicodeUTF8));
		string SiteSense;
		pqxx::result SiteCloseby;
		cGeoP Here;
		Here.Set(Lat.toDouble(),Lon.toDouble(),DEG,-1);
		SiteSense = gDb.GetSetting("SiteSelectSense");
		if (SiteSense == "")
			SiteSense = "150";
		double searchD = scale/atof(SiteSense.c_str());
		
		SiteCloseby = FindClosebySites(Here, searchD);
		if (SiteCloseby.size() >0)
		{
			mId = (int)atoi(SiteCloseby[0]["id"].c_str());
			string Lati,Long;
			string Location;
			Location = SiteCloseby[0]["ligging"].c_str();
			ExtractLatLongFromPoint(Location,locationFormat,Lati,Long);
			txtLongitude->setText(Long.c_str());
			txtLatitude->setText(Lati.c_str());
			txtSite->setText(SiteCloseby[0]["sitename"].c_str());
			txtGroundHeight->setText(SiteCloseby[0]["groundheight"].c_str());
			comboBoxStatus->setCurrentIndex(comboBoxStatus->findText(SiteCloseby[0]["status"].c_str()));
		}
		else
		{
			return false;
		}
	}	
	else
	{
		string La,Lo;
		ExtractLatLongFromPoint(Location,locationFormat,La,Lo);
		txtLongitude->setText(Lo.c_str());
		txtLatitude->setText(La.c_str());
	}

	if ((!mEdit)||(txtGroundHeight->text()=="0"))
	{
		int Height = GetGroundHeight(lat, lon);
		QString GroundHeight = QString("%1").arg(Height);
		txtGroundHeight->setText(GroundHeight);			
	}
	
	return true;
}

//***********************************************************
bool cPlaceSite::ChangeLocation(QString Lat,QString Lon)
{
	cout << "In cPlaceSite::ChangeLocation" << endl;
	DegreeFormat locationFormat;
	string setting = gDb.GetSetting("location");
			
	if (setting=="DD:MM:SS")
		locationFormat = dfDegMinSec;
	else if (setting=="DD:MM.mm")
		locationFormat = dfDegMin;
	else 
		locationFormat = dfDeg;

	double lat = Lat.toDouble();
	double lon = Lon.toDouble();
	cout << "lat: " << lat << "   lon: " << lon << endl;
	string Location = QString ("POINT(%1 %2)").arg(Lon).arg(Lat).toStdString();
	string La,Lo;
	cout  << Location << endl;
	ExtractLatLongFromPoint(Location,locationFormat,La,Lo);
	txtLongitude->setText(Lo.c_str());
	txtLatitude->setText(La.c_str());
	if ((!mEdit)||(txtGroundHeight->text()=="0"))
	{
		int Height = GetGroundHeight(lat, lon);

		QString GroundHeight = QString("%1").arg(Height);
		txtGroundHeight->setText(GroundHeight);			
	}
	return true;
	setFocus ();
	grabMouse ();
	show();
}

//***********************************************************
void cPlaceSite::on_btnPlaceSite_clicked()
{
	if (!mEdit)
	{
		if (InsertSite())
			accept();
	}
	else
	{
		if (UpdateSite())
			accept();
	}
	close();
	
}

//***********************************************************
void cPlaceSite::on_btnDefaultInst_clicked()
{
	cout << "In insert default installation" << endl;
	if (InsertDefRad())
		accept();
	close();
}

//***********************************************************
void cPlaceSite::on_btnCancel_clicked()
{
	close();
}

//***********************************************************
void cPlaceSite::on_btnMove_clicked()
{
	cout << " In cPlaceSite::on_btnMove_clicked()" << endl;
	emit MoveSiteClicked();
}

//***********************************************************
void cPlaceSite::on_btnEditInstallation_clicked()
{
	cout << "cPlaceSite::on_btnEditInstallation_clicked()   mId=" << mId<< endl; 
	if (!mEdit)
	{
		mId = InsertSite();
		if (mId>-1)
		{
			cout << "cPlaceSite::on_btnEditInstallation_clicked() voor Main Window initialise" << endl;
			gMainWindow = new MainWindow();
			cout << "cPlaceSite::on_btnEditInstallation_clicked() voor EditSite" << endl;
			gMainWindow->rapTab->mSites->EditSite(mId);
			cout << "cPlaceSite::on_btnEditInstallation_clicked() voor gMainWindow->show()" << endl;
			gMainWindow->show();
			accept();
		}
	}
	else
	{
		cout << "cPlaceSite::on_btnEditInstallation_clicked() voor Main Window initialise" << endl;
		gMainWindow = new MainWindow();
		cout << "cPlaceSite::on_btnEditInstallation_clicked() voor EditSite" << endl;
		gMainWindow->rapTab->mSites->EditSite(mId);
		cout << "cPlaceSite::on_btnEditInstallation_clicked() voor gMainWindow->show()" << endl;
		gMainWindow->show();
		accept();
	}
	close();
}

//***********************************************************
int cPlaceSite::InsertSite()
{
	DegreeFormat locationFormat;
	string setting = gDb.GetSetting("location");
	if (setting=="DD:MM:SS")
		locationFormat = dfDegMinSec;
	else if (setting=="DD:MM.mm")
		locationFormat = dfDegMin;
	else if (setting=="DD.dddd")
		locationFormat = dfDeg;
	else	locationFormat = dfDeg;

	if (txtSite->text() == "")
	{
		QMessageBox::information(this, "QRap", "Please enter a site name!");
		cout << "Please enter a site name!" << endl;
		return false;
	}
	
	string q = "SELECT (MAX(id)+1) AS id FROM site";
	gDb.PerformRawSql(q);
	pqxx::result r;
	gDb.GetLastResult(r);
	QString MaxId = r[0]["id"].c_str();
	int SiteId = atoi(r[0]["id"].c_str());
	//cout << "*********Size: " << MaxId.toDouble()<< endl;
	if (MaxId.toDouble() > 0)
	{
		MaxId = r[0]["id"].c_str();		
		int SiteId = atoi(r[0]["id"].c_str());
		cout << "HERE*********MAX ID!!!!: " << MaxId.toStdString()<< endl;
	}
	else
	{
		MaxId = "1";
		int SiteId = 1;
	}
	//cout << "*********MAX ID!!!!: " << r[0]["id"].c_str() << endl;
	string query = "INSERT INTO site (id,lastmodified,sitename,status,groundheight,machineid,location) VALUES(";
	query+=MaxId.toStdString();
	query += ",now(),'";
	query += txtSite->text().latin1();
	query += "', '";
	query += comboBoxStatus->currentText().latin1();
	query += "', ";
	query += txtGroundHeight->text().latin1();
	query += ", ";
	char *temp;
	temp = new char[10];
	gcvt(gDb.globalMachineID,8,temp);
	query+= temp; 
	query+= ", "; 
	query+= FindLatLon(txtLatitude->text(),txtLongitude->text()).toStdString();
	query+= ")";

	printf("insert query [%s]\n",query.c_str());

	delete [] temp;
	if (!gDb.PerformRawSql(query))
	{
		QMessageBox::information(this, "QRap", "Error inserting the site!");
		cout << "Error inserting the site!: " << query<< endl;
		return -1;
	}
	return SiteId;
}

//***********************************************************
bool cPlaceSite::InsertDefRad()
{
	char * siteID;
	siteID = new char[33];
	char * machineID;
	machineID = new char[33];
	QString Tech = QString("%1").arg(gDb.GetSetting("TechType").c_str());
	Tech = Tech.mid(0,Tech.find(":"));
	string Teg = Tech.toStdString();
	if (Tech.toDouble()==0)
	{
		QMessageBox::information(this, "QRap", "No Default technology type selected!");
//		QRAP_ERROR("No Default technology type selected!");
		cout << "No Default technology type selected!"<< endl;
		close();
	}
	
		/*string query = "SELECT * ";
		query += "FROM radioinstallation CROSS JOIN SITE WHERE siteid = site.id AND radioinstallation.techkey = ";
		query += Tech.latin1();
		query += " AND site.status = 'Default'";*/
		string query = "SELECT * ";
		query += "FROM radioinstallation ";
		query += "CROSS JOIN technology ";
		query += "CROSS JOIN site ";
		query += "WHERE technology.defaultsite = site.id ";
		query += "AND radioinstallation.siteid = site.id ";
		query += "AND radioinstallation.id<>0 ";
		query += "AND technology.id = ";

		query += Teg.c_str();
		//query +=" AND site.status='Default'";
		cout << "In cPlaceSite::InsertDefRad() Tech=" << Teg.c_str() << endl; 	
		cout << query << endl;	
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database Select for Default installations table failed: "<< query << endl;
		QMessageBox::information(this, "QRap", "Database Select for Default installations table failed.");
		close();
		return false;
	}
	else
	{
		pqxx::result radinst;
		gDb.GetLastResult(radinst);
		if (radinst.size() == 0)
		{
			QMessageBox::information(this, "QRap", "It seems like there is no Default Site added to the Technology, or no Default Technology defined. See Technology entry under Supporting Tables of the Database and or Preference Settings");			
			query = "SELECT * ";
			query += "FROM radioinstallation WHERE id =0";
			gDb.PerformRawSql(query);
			gDb.GetLastResult(radinst);	
		}

		if (!mEdit)
			mId = InsertSite();

		cout << "In cPlaceSite::InsertDefRad() Inst Size =" << radinst.size() << endl; 
		cout << "In PlaceSite::InsertDefRad(): mId=" << mId << endl;
		if (mId>-1)
		{
			cout << "In cPlaceSite::InsertDefRad() Inst Size =" << radinst.size() << endl; 
			cout << "In PlaceSite::InsertDefRad(): mId=" << mId << endl;			
			for (int i = 0; i < radinst.size(); i++)
			{
				string temp;
				gcvt(mId,20,siteID);
//				string ID = siteID;
//				ID +=radinst[i]["sector"].c_str();

				gcvt(gDb.globalMachineID,8,machineID);
				query = "INSERT INTO radioinstallation ";
				query += "(lastmodified, machineid, siteid, sector,techkey,eirp,txpower,";
				query += "txlosses,txantennaheight,txantpatternkey,txbearing,txmechtilt,";
				query += "rxlosses,rxantennaheight,rxantpatternkey,rxbearing,rxmechtilt,";
				query += "rxsensitivity,project,flagx,flagz) VALUES (now(),";
				query += machineID;
				query += ", "; //+ID +", ";
				query += siteID;
				query += ", ";
				temp = radinst[i]["sector"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["techkey"].c_str();
					if (temp != "")	query += temp;
					else		query += "null";
					query += ", ";
				temp = radinst[i]["eirp"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["txpower"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["txlosses"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["txantennaheight"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["txantpatternkey"].c_str();
					if (temp != "")	query += temp;
					else		query += "null";
					query += ", ";
				temp = radinst[i]["txbearing"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["txmechtilt"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["rxlosses"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["rxantennaheight"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["rxantpatternkey"].c_str();
					if (temp != "")	query += temp;
					else		query += "null";
					query += ", ";
				temp = radinst[i]["rxbearing"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["rxmechtilt"].c_str();
					if (temp != "")	query += temp;
					else		query += "0";
					query += ", ";
				temp = radinst[i]["rxsensitivity"].c_str();
					if (temp != "")	query += temp;
					else		query += "null";
					query += ", ";
				temp = radinst[i]["project"].c_str();
				if (temp!="") query += temp;
				else query += "null";
				query += ", ";
				temp = radinst[i]["flagx"].c_str();
				if (temp!="") query += temp;
				else query += "null";
				query += ", ";
				temp = radinst[i]["flagz"].c_str();
				if (temp!="") query += temp;
				else query += "null";
				query += ")";
				if (!gDb.PerformRawSql(query))
				{
					cout << "Insert Defaults"<< endl;
					close();
				}
				else cout << "Inserted Sector " << i+1 << endl;
			
			} // end for loop go through all default radioinstallations / sectors
			delete [] siteID;
			delete [] machineID;
			return true;
		} // if Site successfully added	

		else
		{
			delete [] siteID;
			delete [] machineID;
			return false;
		}
	}
	delete [] siteID;
	delete [] machineID;
	return false;
}


//******************************************************************
bool cPlaceSite::UpdateSite()
{
	cout << "In update Site" << endl;
	if (txtSite->text() == "")
	{
		QMessageBox::information(this, "QRap", "Please enter a site name!");
		return false;
	}
	if (txtGroundHeight->text()=="") txtGroundHeight->setText("0");
	if (comboBoxStatus->currentText() == "")
	{
		QMessageBox::information(this, "QRap", "Please select a site status!");
		return false;
	}
	if (!mId)
	{
		QMessageBox::information(this, "QRap", QString("There was an error finding the site ID!\nThe current site ID is: %1").arg(mId));
		return false;
	}
	pqxx::result r;
	gDb.GetLastResult(r);
	
	string query = "UPDATE site SET ";
	query+= "lastmodified =";
	query += "now(),";
	query+= "sitename ='";
	query += txtSite->text().latin1();
	query += "', ";
	query+= "status ='";
	query += comboBoxStatus->currentText().latin1();
	query += "', groundheight = ";
	query += txtGroundHeight->text().latin1();
	query += ", location = ";
	query+= FindLatLon(txtLatitude->text(),txtLongitude->text()).toStdString();
	query+=" WHERE id = ";
	query += QString("%1").arg(mId).toStdString();
	if (!gDb.PerformRawSql(query))
	{
		QMessageBox::information(this, "QRap", "Error updating site data");
		return false;
	}
	else return true;
}


