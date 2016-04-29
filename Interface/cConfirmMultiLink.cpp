 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmMultiLink.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This is the interface class to initiate a search for
 *                : possible links among a set of sites (sans radio insts)
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

#include "cConfirmMultiLink.h"

//*********************************************************************************************
cConfirmMultiLink::cConfirmMultiLink() : QDialog()
{
	setupUi(this);
	this->setModal(true);
	LoadDefaults();
}

//*********************************************************************************************
cConfirmMultiLink::cConfirmMultiLink(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	setupUi(this);
	this->setModal(true);
	LoadDefaults();
	mFL = fl;
	mParent = parent;
}

//***********************************************************************************************
cConfirmMultiLink::~cConfirmMultiLink()
{
}

//*************************************************************************************************************
void cConfirmMultiLink::LoadDefaults()
{
	int j=0;
	string query;
	QString err="";
	pqxx::result r;

	pushOk->setText("Ok");
	pushCancel->setText("Cancel");
	pushOk->setEnabled(true);
	pushCancel->setEnabled(true);

	checkBox->setCheckState(Qt::Checked);
	
	QString Dir = (QString)(gDb.GetSetting("OutputDir").c_str());
	lineEdit->setText(Dir);

	if (checkBox->isChecked())
	{
		lineEdit->setVisible(true);
		lineEdit->setEnabled(true);
		lblPath->setVisible(true);
	}
	else
	{
		lineEdit->setVisible(false);
		lineEdit->setEnabled(false);
		lblPath->setVisible(false);
	}

	connect(checkBox, SIGNAL(stateChanged(int)),this,  SLOT(checkBoxChanged(int)));

	query = "SELECT id,technologytype from technology;";
	if (!gDb.PerformRawSql(query))
	{
		err= "Database Select for technology in cConfirmMultiLink::cConfirmMultiLink() failed";
		QRAP_WARN(err.toStdString().c_str());
		QMessageBox::information(this, "QRap", err);
	}
	else
	{
		gDb.GetLastResult(r);
		if (r.size() >0)
		{
			for (int i = 0; i < r.size();i++)
			{
				cbTechnology->insertItem(j,r[i]["technologytype"].c_str());
				j++;
			}
		}
		else
		{
			if (j==0) cbTechnology->insertItem(0,"0:NULL");
			err= "You need to define at least one technology with a default site-installation";
			QRAP_WARN(err.toStdString().c_str());
			QMessageBox::information(this, "QRap", err);
		}
	}
	query = "SELECT id,projectname from projects;";
	if (!gDb.PerformRawSql(query))
	{
		err= "Database Select for projects in cConfirmMultiLink::cConfirmMultiLink() failed";
		cout << err.toStdString().c_str() << " Query: " << query;
		QRAP_WARN(err.toStdString().c_str());
	}
	else
	{
		j=0;
		gDb.GetLastResult(r);
		if (r.size() >0)
		{
			for (int i = 0; i < r.size();i++)
			{
				cbProject->insertItem(j,r[i]["projectname"].c_str());
				j++;
			}
		}
		else
		{
			if (j==0) cbTechnology->insertItem(0,"0:NULL");
			err= "Project list is empty";
			QRAP_WARN(err.toStdString().c_str());
		}
	}
	cbMinClear->insertItem(0,"100");
	cbMinClear->insertItem(1,"60");
	cbMinClear->insertItem(2,"0");
	dsbMaxPath->setValue(150.0);
	dsbMaxDist->setValue(50.0);
}

//*************************************************************************************************
void cConfirmMultiLink::checkBoxChanged()
{
	cout << "In cConfirmMultiLink::checkBoxChanged()" << endl; 
	if (checkBox->isChecked())
	{
		lineEdit->setVisible(true);
		lineEdit->setEnabled(true);
		lblPath->setVisible(true);
	}
	else
	{
		lineEdit->setVisible(false);
		lineEdit->setEnabled(false);
		lblPath->setVisible(false);
	}
}

//*************************************************************************************************
bool cConfirmMultiLink::SetPoints(QList<QgsPoint> Points)
{
	string err="";
	char *text= new char[33];
	if (Points.size() > 1)
	{
		double North = Points[0].y();
		double South = Points[0].y();
		double East = Points[0].x();
		double West = Points[0].x();
		string query;
		pqxx::result SitesIn;
		query = "SELECT id,sitename, ST_AsText(location) AS location,status";
		query += " FROM site_view_only ";
		query += " WHERE  location ";
        	query +="@ ST_GeomFromText('POLYGON((";
	        for (int i = 0 ; i < Points.size();i++)
 	       	{
 		       	North = max(North,Points[i].y());
 		       	South = min(South,Points[i].y());
 		       	East = max(East,Points[i].x());
 		       	West = min(West,Points[i].x());
//			sprintf (text, "%f", Points[i].x());
//			cout << text << endl;
			gcvt(Points[i].x(),10,text);
  		      	query += text;
  		      	query += " ";
//			sprintf (text, "%f", Points[i].y());
			gcvt(Points[i].y(),10,text);
        		query += text;
        		query += ",";
     		}
		gcvt(Points[0].x(),10,text);
  		query += text;
  		query += " ";
		gcvt(Points[0].y(),10,text);
        	query += text;
        	query += "))',4326);";	
		if (!gDb.PerformRawSql(query))
		{
			cout << "Database Select on sites table failed in cConfirmMultiLink::SetPoints"<< endl;
			string err = "Database Select on sites table failed"; 
			QRAP_WARN(err.c_str());
			delete [] text;
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
				tableInstallations->setItem(i,1,new QTableWidgetItem(SitesIn[i]["id"].c_str()));
				tableInstallations->setItem(i,2,new QTableWidgetItem(SitesIn[i]["status"].c_str()));
			}
		}
		else
		{
			string err = "No Sites in selected area! ";
			QRAP_WARN(err.c_str());
			delete [] text;
			return false;
		}
		delete [] text;
		return true;
	}
	delete [] text;
	return false;
}



//***********************************************************************************************
void cConfirmMultiLink::on_pushCancel_clicked()
{
	close();
}

//***********************************************************************************************
void cConfirmMultiLink::on_pushOk_clicked()
{
	bool Success = true;
	pqxx::result r;
	QList<string> SiteNames;
	bool SavePDF=false;
	QString path = lineEdit->text();
	path += "/";
	QString file = path;
	QString Transmitter="Transmitter";
	QString Receiver="Receiver";

	if (checkBox->isChecked())
		SavePDF=true;

	// Get Values from interface
	for (int i = 0; i < tableInstallations->rowCount(); i++)
	{
		void *Wid = tableInstallations->cellWidget(i,0);
		QCheckBox *get =(QCheckBox*) Wid;
		if (get->isChecked())
		{
			mSiteList.push_back((unsigned)tableInstallations->item(i,1)->text().toDouble());
		}
	}
	mMinClear = atof(cbMinClear->currentText().latin1())/100.0;
	mMaxPath = dsbMaxPath->value();
	mMaxDist = dsbMaxDist->value();
	mkFactor = dsbKfactor->value();
	mTechnology=cbTechnology->currentText().latin1();

	string query = "SELECT technology.id as techkey, eirp, ";
	query += " radioinstallation.txpower,radioinstallation.txlosses,";
	query += " radioinstallation.rxlosses,radioinstallation.rxsensitivity,btlfreq, ";
	query += "txantpatternkey,txbearing,txmechtilt,txantennaheight, ";
	query += "rxantpatternkey,rxbearing,rxmechtilt,rxantennaheight ";
	query += "FROM radioinstallation cross join site cross join technology ";
	query += "WHERE radioinstallation.siteid=site.id and defaultsite=site.id ";
	query += "and technologytype ='"; 
	query += mTechnology; 
	query += "';";
	mInst.sSiteID=0;
	if(!gDb.PerformRawSql(query))
	{
		Success = false;
		QString err = "There seem to be an error with the query to get the default";
		err+= "radio installation  for the technology.";
		cout << "Query Failed: " << query.c_str() << endl;
		QRAP_ERROR(err.toStdString().c_str());
		QMessageBox::information(this, "QRap", err);
	} // if
	else
	{
		gDb.GetLastResult(r);
		
		int size = r.size();
		if (size>0)
		{
			mTechKey = atoi(r[0]["techkey"].c_str());
			mInst.sEIRP = atof(r[0]["eirp"].c_str());
			mInst.sTxPower = atof(r[0]["txpower"].c_str());
			mInst.sTxSysLoss = atof(r[0]["txlosses"].c_str());
			mInst.sRxSysLoss = atof(r[0]["rxlosses"].c_str());
			mInst.sRxSens = atof(r[0]["rxsensitivity"].c_str());
			mInst.sTxPatternKey = atoi(r[0]["txantpatternkey"].c_str());
			mInst.sTxAzimuth = atof(r[0]["txbearing"].c_str());
			mInst.sTxMechTilt = atof(r[0]["txmechtilt"].c_str());
			mInst.sRxPatternKey = atoi(r[0]["rxantpatternkey"].c_str());
			mInst.sRxAzimuth = atof(r[0]["rxbearing"].c_str());
			mInst.sRxMechTilt = atof(r[0]["rxmechtilt"].c_str());
			mInst.sTxHeight = atof(r[0]["txantennaheight"].c_str());
			mInst.sRxHeight = atof(r[0]["rxantennaheight"].c_str());
			mInst.sFrequency = atof(r[0]["btlfreq"].c_str());
		}
		else
		{
			QString err = "There seem to be no default site instalation defined for the technology. ";
			QRAP_WARN(err.toStdString().c_str());
			cout << "Query Empty: " << query.c_str() << endl;
			QMessageBox::information(this, "QRap", err);
			Success=false;
			return;

		} // else
	} // else

	query = "SELECT id from projects where projectname = '";
	query += cbProject->currentText().latin1();
	query +="'";
	if(!gDb.PerformRawSql(query))
	{
		string err = "There seem to be an error with the query to get the project";
		err+=query; 
		mProject=0;
		QRAP_WARN(err.c_str());
	} // if
	else
	{
		gDb.GetLastResult(r);
		
		int size = r.size();
		if (size>0)
			mProject = atoi(r[0]["id"].c_str());
		else mProject=0;
	}


	eOutputUnits Units = dBm;
	double PlotResolution = atof(gDb.GetSetting("PlotResolution").c_str());
	double tempPlotRes = PlotResolution;
	string Down = gDb.GetSetting("Downlink");
	bool DownLink;
	if (Down=="false")
		DownLink  = false;
	else DownLink  = true;
	string Clutter = gDb.GetSetting("UseClutter");
	bool UseClutter=false;
	if (Clutter=="true")
		UseClutter = true;


	if (Success)
	{
		pushOk->setEnabled(false);
		pushCancel->setEnabled(false);
		pushOk->setText("wait");
		pushCancel->setText("wait");
		short int ClutterSource = atoi(gDb.GetSetting("ClutterSource").c_str());
		short int DEMsource = atoi(gDb.GetSetting("DEMsource").c_str());

		// Go through all possible links and evaluate them, if good save.
		int size = mSiteList.size();
		int i,j;
		double Distance, PathLoss, MinClearance;
		bool Work=true;
		int TxRadID, RxRadID;
		char * temp = new char[33];
		cLink Link;
		string LinkName="Link";
		QString Linkname = "Link";
		int LinkID=0;
		double tempPlotRes;
		tempPlotRes=PlotResolution;
		Link.SetLink(Units, DownLink, mInst.sFrequency, mkFactor, tempPlotRes,
				DEMsource, ClutterSource, UseClutter, 0, 0, true);

		for (i=0; i<size; i++)
		{
			if (Link.SetTxSite(mSiteList[i],mInst))
			{
				for (j=i+1; j<size; j++)
				{
					if (Link.SetRxSite(mSiteList[j],mInst))
					{
						cout <<i << " x " << j << "  of " << size << " : ";
						if (Link.DoLink(true,mMaxDist*1000))
						{
			    			MinClearance = Link.GetMinClearance();
							PathLoss = Link.GetPathLoss();	
							Work = (MinClearance>=mMinClear)
								&&(PathLoss<=mMaxPath);
							if (Work)
							{
								TxRadID=InsertRadInst(mSiteList[i], j, 
											Link.GetTxBearing(), 
											Link.GetTxTilt());
								RxRadID=InsertRadInst(mSiteList[j], i, 
											Link.GetRxBearing(), 
											Link.GetRxTilt());
								Link.SetTxInst(TxRadID);
								Link.SetRxInst(RxRadID);
								LinkName="Link";
								LinkID=0;
								Link.DoLink(false);
								Link.SaveLink(LinkName,LinkID);
								if (SavePDF)
								{
									cLinkAnalysis * LinkDisplay = new cLinkAnalysis(mParent,mFL);
									Linkname = LinkName.c_str();
									Transmitter = "Site:";
									gcvt(mSiteList[i],8,temp);
									Transmitter += temp;
									Transmitter += "_Radio:";
									gcvt(TxRadID,8,temp);
									Transmitter += temp;
									gcvt(mSiteList[j],8,temp);
									Receiver = "Site:";
									Receiver += temp;
									Receiver += "_Radio:";
									gcvt(RxRadID,8,temp);
									Receiver += temp;
									tempPlotRes = PlotResolution;
									LinkDisplay->DoAndSetUpDisplay(Units, DownLink, 
													mInst.sFrequency, 
													mkFactor, tempPlotRes,
													DEMsource, ClutterSource, 
													UseClutter, TxRadID, RxRadID,
													Linkname, Transmitter, 
													Receiver, LinkID);
									LinkDisplay->SetPath(path);
									LinkDisplay->SetAutoName(true);
									LinkDisplay->MakePDF();
									LinkDisplay->OKClose();
									LinkDisplay->exec();
									delete LinkDisplay;
								}
								cout << mSiteList[i] << " to " << mSiteList[j] << " saved." << endl;
							}
							else
							{
								cout << mSiteList[i] << " to " << mSiteList[j] << " does not work." << endl;
							}
							Link.ReSetPlotRes(PlotResolution);
						}
						else
						{
							cout << mSiteList[i] << " to " << mSiteList[j] << " skipped."<< endl; 
						}
					}
					else
					{
						cout << "Problem with site: " <<mSiteList[j] <<  endl;
					}
				}
			}
			else
			{
				cout << "Problem with site: " <<mSiteList[i] <<  endl;
			}
		}
		accept();
		close();
		delete [] temp;
	}
}

//***********************************************************************************
int cConfirmMultiLink::InsertRadInst(int SiteID, int Sector, double Bearing, double Tilt)			
{
	string query="";
	char * text;
	text = new char[33];

	query = "INSERT INTO radioinstallation ";
	query += "(lastmodified, machineid, siteid, sector,techkey,eirp,txpower,";
	query += "txlosses,txantennaheight,txantpatternkey,txbearing,txmechtilt,";
	query += "rxlosses,rxantennaheight,rxantpatternkey,rxbearing,rxmechtilt,";
	query += "project) VALUES (now(),";
	gcvt(gDb.globalMachineID,8,text);
	query += text;
	query += ", ";
	gcvt(SiteID,20,text);
	query += text;
	query += ", ";
	gcvt(Sector,8,text);
	query += text;
	query += ", ";
	gcvt(mTechKey,8,text);
	query += text;
	query += ", ";
	gcvt(mInst.sEIRP,8,text);
	query += text;
	query += ", ";	
	gcvt(mInst.sTxPower,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sTxSysLoss,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sTxHeight,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sTxPatternKey,8,text);					
	query += text;
	query += ", ";
	gcvt(Bearing,8,text);					
	query += text;
	query += ", ";
	gcvt(Tilt,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sRxSysLoss,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sRxHeight,8,text);					
	query += text;
	query += ", ";
	gcvt(mInst.sRxPatternKey,8,text);					
	query += text;
	query += ", ";
	gcvt(Bearing,8,text);					
	query += text;
	query += ", ";
	gcvt(Tilt,8,text);					
	query += text;
	query += ", ";
	gcvt(mProject,8,text);					
	if (0==mProject)
		strcpy(text, "null");
	query += text;
	query += ")";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Insert Radioinstallation in cConfirmMultiLink::InsertRadInst failed"<< endl;
		delete [] text;
		return 0;				
	}

	int LastID=0;
	pqxx::result r;
	query = "select currval('radioinstallation_id_seq');";	
	if(!gDb.PerformRawSql(query))
	{
		string err = "There seem to be an error getting the last ID of the radioinstallation table";
		err+=query; 
		QRAP_WARN(err.c_str());
	} // if
	else
	{
		gDb.GetLastResult(r);
		
		int size = r.size();
		if (size>0)
			LastID= atoi(r[0][0].c_str());
	}
	delete [] text;
	return LastID;
}				

