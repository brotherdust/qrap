 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmLink.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : This is the interface class to initiate a link analysis
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

#include "cConfirmLink.h"
#include <QMessageBox>

//*****************************************************************
cConfirmLink::cConfirmLink(): QDialog()
{
	setupUi(this);
	this->setModal(true);
}

//***************************************************************/
cConfirmLink::cConfirmLink(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	setupUi(this);
	this->setModal(true);
}

//***************************************************************/
cConfirmLink::~cConfirmLink()
{
}


//***************************************************************/
bool cConfirmLink::SetPoints(QList<QgsPoint> Points,double scale)
{
	mLinkID=0;
	pqxx::result TechSet;
	QString LinkName="";
	char* text;
	text = new char[33];
	QString Tech = QString("%1").arg(gDb.GetSetting("TechType").c_str());
	Tech = Tech.mid(0,Tech.find(":"));
	int TTKey = (int)Tech.toDouble();
	cout << "TTKey:" << TTKey <<endl;

	bool Select = false;
	string SiteSense;
	string query;
	pqxx::result SiteCloseby;
	cGeoP Here,There;
	There.SetGeoType(DEG,-1);
	double North,South,East,West;
	SiteSense = gDb.GetSetting("SiteSelectSense");
	if (SiteSense == "")
		SiteSense = "150";
	Here.Set(Points[0].y(),Points[0].x(),DEG,-1);
	There.FromHere(Here,(scale/atof(SiteSense.c_str())),315);
	There.SetGeoType(DEG);
	There.Get(North,West);
	There.FromHere(Here,(scale/atof(SiteSense.c_str())),135);
	There.SetGeoType(DEG);
	There.Get(South,East);
	query = "SELECT distinct radioinstallation_view.id AS rid,site.sitename,technologytype, ";
	query += "radioinstallation_view.techkey AS ttk ";
	query += "FROM radioinstallation_view LEFT OUTER JOIN technology ON (radioinstallation_view.techkey=technology.id)  ";
	query += "CROSS JOIN site WHERE (siteid = site.id) AND ";
	query +="location && SetSRID('BOX3D(";
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
		QMessageBox::information(this, "QRap", "Database Select on sites table failed!");
		cout << "Database Select on sites table failed"<< endl;
		close();
	}
	else
	{
		Select = false;
		gDb.GetLastResult(SiteCloseby);
		if (SiteCloseby.size())
		{
			TxIDTable->setRowCount(SiteCloseby.size());
			for (int i = 0; i < SiteCloseby.size();i++)
			{
				QRadioButton *Enabled = new QRadioButton(SiteCloseby[i]["sitename"].c_str());
				LinkName = SiteCloseby[i]["sitename"].c_str();
				bool Se = (i==SiteCloseby.size()-1)&&(!Select);
				if (Se&&(TTKey==atof(SiteCloseby[i]["ttk"].c_str())))
				{
					Enabled->setChecked(true);
					Select = true;
				}
				else
					Enabled->setChecked(false);
				TxIDTable->setCellWidget(i,0,Enabled);
				TxIDTable->setItem(i,1,new QTableWidgetItem(SiteCloseby[i]["technologytype"].c_str()));
				TxIDTable->setItem(i,2,new QTableWidgetItem(SiteCloseby[i]["rid"].c_str()));
			}
		}
		else
		{
			QMessageBox::information(this, "QRap", "There were no radio installations near the selected link!");
			return false;
		}
	}
	Here.Set(Points[1].y(),Points[1].x(),DEG,-1);
	There.FromHere(Here,(scale/atof(SiteSense.c_str())),315);
	There.SetGeoType(DEG);
	There.Get(North,West);
	There.FromHere(Here,(scale/atof(SiteSense.c_str())),135);
	There.SetGeoType(DEG);
	There.Get(South,East);
	query = "SELECT distinct radioinstallation_view.id AS rid,site.sitename, technologytype, ";
	query += "radioinstallation_view.techkey AS ttk ";
	query += "FROM radioinstallation_view LEFT OUTER JOIN technology ON (radioinstallation_view.techkey=technology.id)  ";
	query += "CROSS JOIN site WHERE (siteid = site.id) AND ";
	query +="location && SetSRID('BOX3D(";
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
	else
	{
		gDb.GetLastResult(SiteCloseby);
		if (SiteCloseby.size())
		{
			Select = false;
			RxIDTable->setRowCount(SiteCloseby.size());
			for (int i = 0; i < SiteCloseby.size();i++)
			{
				QRadioButton *Enabled = new QRadioButton(SiteCloseby[i]["sitename"].c_str());
				if (0==i)
				{
					LinkName += "_to_";
					LinkName += SiteCloseby[i]["sitename"].c_str();
				}
				if (((i==SiteCloseby.size()-1)&&(!Select))&&(TTKey==atof(SiteCloseby[i]["ttk"].c_str())))
				{
					Enabled->setChecked(true);
					Select = true;
				}
				else
					Enabled->setChecked(false);
				RxIDTable->setCellWidget(i,0,Enabled);
				RxIDTable->setItem(i,1,new QTableWidgetItem(SiteCloseby[i]["technologytype"].c_str()));
				RxIDTable->setItem(i,2,new QTableWidgetItem(SiteCloseby[i]["rid"].c_str()));
			}
		}
		else
		{
			return false;
		}
	}
		
	string setting;
	
	setting = gDb.GetSetting("DisplayUnits");
	if(setting!="")
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText(QString::fromStdString(setting),Qt::MatchCaseSensitive));
	else
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText("dBm",Qt::MatchCaseSensitive));

	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		kFactorIntSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorIntSpinBox->setValue(1.33);

	setting = gDb.GetSetting("UseClutter");
	if(setting=="true")
		useClutterCheckBox->setChecked(true);
	else
		useClutterCheckBox->setChecked(false);
	
	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
		plotResolutionSpinBox->setValue(atof(setting.c_str()));
	else
		plotResolutionSpinBox->setValue(90.00);
	
	setting = gDb.GetSetting("DownLink");
	if(setting=="true")
		downLinkRadio->setChecked(true);
	else
		upLinkRadio->setChecked(true);

	gcvt(TTKey,10,text);
	query = "Select btlfreq, fademargin, ebno, rxmin from technology where id=";
	query += text;
	query += ";";
	bool EmptyResult = false; 
	if (!gDb.PerformRawSql(query))
	{
		cout << "Could not get technology. "<< endl;
		EmptyResult = true;
	}
	else
	{
		cout << "In cConfirmLink::SetPoints:  Got query results. " << endl; 
		gDb.GetLastResult(TechSet);
		if (TechSet.size()>0)
		{
			cout << "In cConfirmLink::SetPoints:  Results not empty " << endl; 
			frequencySpinBox->setValue(atof(TechSet[0]["btlfreq"].c_str()));
		}
		else EmptyResult=true;
	}
		
	if (EmptyResult)
	{
		frequencySpinBox->setValue(2400);
	}
	nameEdit->insert(LinkName);
	delete [] text;
	cout << "In cConfirmLink::SetPoints: Exiting " << endl; 
	return true;
}


/***************************************************************/
void cConfirmLink::on_btnCancel_clicked()
{
	close();
}



//*****************************************************************************************
void cConfirmLink::on_btnOk_clicked()
{
	cout << "Entering cConfirmLink::on_btnOk_clicked()" << endl;
	TxID = -9999;
	RxID = -9999;
	cout << "TxIDTable->rowCount(): " << TxIDTable->rowCount() << endl;
	for (int i = 0; i < TxIDTable->rowCount();i++)
	{
		void *Wid = TxIDTable->cellWidget(i,0);
		QRadioButton *get =(QRadioButton*) Wid;
		if (get->isChecked())
		{
			TxName = get->text();
			TxID =(int)TxIDTable->item(i,2)->text().toDouble();
			cout << "cConfirmLink::on_btnOk_clicked(),  get->isChecked() TxID: " << TxID <<endl;
		}
		if ((i==TxIDTable->rowCount()-1)&&(TxID==-9999))
		{
			TxName = get->text();
			TxID =(int)TxIDTable->item(i,2)->text().toDouble();
			cout << "cConfirmLink::on_btnOk_clicked(),  (i==TxIDTable->rowCount()-1) TxID: " << TxID <<endl;
		}
	}
	
	cout << "RxIDTable->rowCount(): " << RxIDTable->rowCount() << endl;
	for (int i = 0; i < RxIDTable->rowCount();i++)
	{
		void *Wid = RxIDTable->cellWidget(i,0);
		QRadioButton *get =(QRadioButton*) Wid;
		if (get->isChecked())
		{
			RxName = get->text();
			RxID =(int)RxIDTable->item(i,2)->text().toDouble();
			cout << "cConfirmLink::on_btnOk_clicked(),  get->isChecked() RxID: " << RxID <<endl;
		}
		if ((i==RxIDTable->rowCount()-1)&&(RxID==-9999))
		{
			RxName = get->text();
			RxID =(int)RxIDTable->item(i,2)->text().toDouble();
			cout << "cConfirmLink::on_btnOk_clicked(),  (i==RxIDTable->rowCount()-1) RxID: " << RxID <<endl;
		}
	}
	
	//string query = "SELECT ST_AsText(location) AS location,";
	switch (displayUnitsCombo->currentIndex())
	{
		case 0:			Units = dB;		break;
		case 1:			Units = dBW;		break;
		case 2:			Units = dBm;		break;
		case 3:			Units = dBuV;		break;
		case 4:			Units = dBuVm;		break;
		case 5:			Units = dBWm2Hz;	break;
		case 6:			Units = dBWm2;		break;
		default:		Units = dBm;		break;			
	}

	Downlink=downLinkRadio->isChecked();
	
	kFactor=kFactorIntSpinBox->value();
	PlotResolution=plotResolutionSpinBox->value();
	Frequency = frequencySpinBox->value();
	mLinkName = nameEdit->text();
	
	string query = "SELECT id from filesetsused WHERE type = 'DEM';";
	pqxx::result DEM;
	if (!gDb.PerformRawSql(query))
	{
		string err = "Database Select on FileSetsUsed for DEM failed ";
		QRAP_WARN(err.c_str());
		cout << "Database Select on FileSetsUsed table for DEM failed"<< endl;
	}
	gDb.GetLastResult(DEM);
	if (DEM.size() >0)
	{
		DEMsource = (int)atof(DEM[0]["id"].c_str());
	}

	UseClutter = useClutterCheckBox->isChecked();
	
	if (UseClutter)
	{
		query = "SELECT id from filesetsused WHERE type = 'Clutter';";
		pqxx::result Clutter;
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database Select on FileSetsUsed for Clutter failed"<< endl;
		}
		gDb.GetLastResult(Clutter);
		if (Clutter.size() >0)
		{
			ClutterSource = (int)atof(Clutter[0]["id"].c_str());
		}
		else
		{
			ClutterSource = 1;
			UseClutter = false;
		}
	}
	cout << "Exiting cConfirmLink::on_btnOk_clicked()" << endl;
	accept();
}

//*******************************************************************************
bool cConfirmLink::SetOldLink(int ID, int RxID,int TxID, string Name, double frequency, double kfactor)
{
	mLinkID = ID;
	char Tx[33];
    	char Rx[33];
    	gcvt(TxID,8,Tx);
	gcvt(RxID,8,Rx);
	string query;

	pqxx::result SiteCloseby;
	query = "SELECT distinct radioinstallation_view.id AS rid,site.sitename,technologytype, ";
	query += "radioinstallation_view.techkey AS ttk ";
	query += "FROM radioinstallation_view LEFT OUTER JOIN technology ON (radioinstallation_view.techkey=technology.id)  ";
	query += "CROSS JOIN site WHERE (siteid = site.id) AND ";
    	query += "radioinstallation_view.id = ";
    	query += Tx;
    	query += ";";
	
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database Select on Radio/Installation for selected existing link Tx failed"<< endl;
		close();
	}
	else
	{
		gDb.GetLastResult(SiteCloseby);
		if (SiteCloseby.size())
		{
			TxIDTable->setRowCount(1);
			QRadioButton *Enabled = new QRadioButton(SiteCloseby[0]["sitename"].c_str());
			Enabled->setChecked(true);
			TxIDTable->setCellWidget(0,0,Enabled);
			TxIDTable->setItem(0,1,new QTableWidgetItem(SiteCloseby[0]["technologytype"].c_str()));
			TxIDTable->setItem(0,2,new QTableWidgetItem(SiteCloseby[0]["rid"].c_str()));
		}
		else return false;
	}

	query = "SELECT distinct radioinstallation_view.id AS rid,site.sitename,technologytype, ";
	query += "radioinstallation_view.techkey AS ttk ";
	query += "FROM radioinstallation_view LEFT OUTER JOIN technology ON (radioinstallation_view.techkey=technology.id)  ";
	query += "CROSS JOIN site WHERE (siteid = site.id) AND ";
	query += "radioinstallation_view.id = ";
	query += Rx;
	query += ";";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database Select on Radio/Installation for selected existing link Rx failed"<< endl;
		close();
	}
	else
	{
		gDb.GetLastResult(SiteCloseby);
		if (SiteCloseby.size())
		{
			RxIDTable->setRowCount(1);
			QRadioButton *Enabled = new QRadioButton(SiteCloseby[0]["sitename"].c_str());
			Enabled->setChecked(true);
			RxIDTable->setCellWidget(0,0,Enabled);
			RxIDTable->setItem(0,1,new QTableWidgetItem(SiteCloseby[0]["technologytype"].c_str()));
			RxIDTable->setItem(0,2,new QTableWidgetItem(SiteCloseby[0]["rid"].c_str()));
		}
		else return false;
	}
	
	nameEdit->setText(QApplication::translate("ConfirmLink", Name.c_str(), 0, QApplication::UnicodeUTF8));	
	
	string setting;
	
	setting = gDb.GetSetting("DisplayUnits");
	if(setting!="")
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText(QString::fromStdString(setting),Qt::MatchCaseSensitive));
	else
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText("dBm",Qt::MatchCaseSensitive));

	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		kFactorIntSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorIntSpinBox->setValue(1.33);

	setting = gDb.GetSetting("UseClutter");
	if(setting=="true")
		useClutterCheckBox->setChecked(true);
	else
		useClutterCheckBox->setChecked(false);
	
	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
		plotResolutionSpinBox->setValue(atof(setting.c_str()));
	else
		plotResolutionSpinBox->setValue(90.00);
	
	setting = gDb.GetSetting("DownLink");
	if(setting=="true")
		downLinkRadio->setChecked(true);
	else
		upLinkRadio->setChecked(true);


	pqxx::result TechSet;
	QString Tech = QString("%1").arg(gDb.GetSetting("TechType").c_str());
	Tech = Tech.mid(0,Tech.find(":"));
	int TTKey = (int)Tech.toDouble();
	char TechKey[33];
	gcvt(TTKey,8,TechKey);

	query = "Select btlfreq, fademargin, ebno, rxmin from technology where id=";
	query += TechKey;
	query += ";";
	bool EmptyResult = false; 
	if (!gDb.PerformRawSql(query))
	{
		cout << "Could not get technology. "<< endl;
		EmptyResult = true;
	}
	else
	{
		gDb.GetLastResult(TechSet);
		if (TechSet.size())
		{
			frequencySpinBox->setValue(atof(TechSet[0]["btlfreq"].c_str()));
		}
		else EmptyResult=true;
	}
		
	if (EmptyResult)
	{
		frequencySpinBox->setValue(2400);
	}
		
	frequencySpinBox->setValue(frequency);
	kFactorIntSpinBox->setValue(kfactor);
	return true;
}
