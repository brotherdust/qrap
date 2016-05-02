 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmPrediction.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    print part used and adapted from qgsquickprint.cpp by Tim Sutton (c) 2008
 *    Description : This is the interface part that initiates area predictions
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

#include "cConfirmPrediction.h"
using namespace Qrap;

/*******************************************************************/
cConfirmPrediction::cConfirmPrediction(): QDialog()
{
	setupUi(this);
	this->setModal(true);
	pushButtonPrint->setEnabled(false);
	btnDone->setEnabled(false);
	LoadDefaults();
}
/***********************************************************/
cConfirmPrediction::cConfirmPrediction(QgisInterface * QgisInt,
					QWidget* parent, Qt::WFlags fl)
			: QDialog(parent, fl)
{
	setupUi(this);
	this->setModal(true);
	pushButtonPrint->setEnabled(false);
	btnDone->setEnabled(false);
	LoadDefaults();
	mQGisIface = QgisInt;
	mMapCanvas = mQGisIface->mapCanvas();
	mMapRenderer = mMapCanvas->mapSettings();
}

/***********************************************************/
cConfirmPrediction::~cConfirmPrediction()
{
}

/***********************************************************/
bool cConfirmPrediction::SetPoints(QList<QgsPoint> Points)
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

		query = "SELECT distinct siteid, site_view_only.sitename as sitename ,ST_AsText(location) AS location,status,";
		query += " sector,radioinstallation_view.id AS radinst,"; 
		query += " groundheight, status, project, flagx, flagz, technologytype, techkey ";
		query += "FROM radioinstallation_view LEFT OUTER JOIN technology  ";
		query += " ON (radioinstallation_view.techkey=technology.id) cross join site_view_only ";
		query += " WHERE (radioinstallation_view.siteid = site_view_only.id) and location ";
        	query +="@ ST_GeomFromText('POLYGON((";
	        for (int i = 0 ; i < Points.size();i++)
	        {
	        	North = max(North,Points[i].y());
	        	South = min(South,Points[i].y());
	        	East = max(East,Points[i].x());
	        	West = min(West,Points[i].x());
			gcvt(Points[i].x(),12,text);
	        	query += text;
	        	query += " ";
			gcvt(Points[i].y(),12,text);
	        	query += text;
	        	query += ",";
	        }
	        NorthWestCorner.Set(North,West,DEG);
	        SouthEastCorner.Set(South,East,DEG);
		cout << "North West corner: " << endl;
		NorthWestCorner.Display();
		cout << "South East corner: " << endl;
		SouthEastCorner.Display();
		gcvt(Points[0].x(),12,text);
	        query += text;
	        query += " ";
		gcvt(Points[0].y(),12,text);
	        query += text;
	        query += "))',4326) order by radioinstallation_view.id;";	
		if (!gDb.PerformRawSql(query))
		{
			QMessageBox::information(this, "QRap", "Problem with database query to get sites in selected area!");
			delete [] text;
			return false;
		}
		gDb.GetLastResult(SitesIn);
		if (SitesIn.size() >0)
		{
			tableWidget->setRowCount(SitesIn.size());
			for (int i = 0; i < SitesIn.size();i++)
			{
				QCheckBox *Enabled = new QCheckBox(SitesIn[i]["sitename"].c_str());
				Enabled->setCheckState(Qt::Checked);
				tableWidget->setCellWidget(i,0,Enabled);
				tableWidget->setItem(i,1,new QTableWidgetItem(SitesIn[i]["sector"].c_str()));
				tableWidget->setItem(i,3,new QTableWidgetItem(SitesIn[i]["technologytype"].c_str()));
				tableWidget->setItem(i,4,new QTableWidgetItem(SitesIn[i]["radinst"].c_str()));
				tableWidget->setItem(i,5,new QTableWidgetItem(SitesIn[i]["project"].c_str()));
				tableWidget->setItem(i,6,new QTableWidgetItem(SitesIn[i]["flagx"].c_str()));
				tableWidget->setItem(i,7,new QTableWidgetItem(SitesIn[i]["flagz"].c_str()));
				tableWidget->setItem(i,8,new QTableWidgetItem(SitesIn[i]["status"].c_str()));
				if (SitesIn[i]["techkey"].c_str()!="")
				{
					QString Query = QString("SELECT maxrange from technology WHERE id = %1").arg(SitesIn[i]["techkey"].c_str());
					if (!gDb.PerformRawSql(Query.toStdString()))
					{
						string err = "Query on database to get the maximum range for the technology failed ";
						QMessageBox::information(this, "QRap", "Query on database to get the maximum range for technology failed");
//						QRAP_WARN(err.c_str());
						cout << err << endl;	
						//return false;
						tableWidget->setItem(i,2,new QTableWidgetItem("10"));//Radius
					}
					else
					{
						pqxx::result R;
						gDb.GetLastResult(R);
						if (R.size() >0)
						{
							tableWidget->setItem(i,2,new QTableWidgetItem(R[0]["maxrange"].c_str()));//Radius
						}
						else
						{
							tableWidget->setItem(i,2,new QTableWidgetItem("5"));//Radius
						} 
					}
				}
				else
				{
					tableWidget->setItem(i,2,new QTableWidgetItem("10"));//Radius
				}
			}
		}
		else
		{
			// No Sites are found, thus only make DEM prediction availible to the user.
			string err = "No Sites in selected area!\nOnly a DEM Prediction is available. ";
			QMessageBox::information(this, "QRap", "No Sites in selected area!\nOnly a DEM Prediction is available.");
//			QRAP_WARN(err.c_str());
			cout << err << endl;
			plotTypeCombo->setCurrentIndex(11);	
			plotTypeCombo->setEnabled(false);	
			radioRadius->setEnabled(false);	
			delete [] text;
			return true;
		}
		delete [] text;
		return true;
	}
	delete [] text;
	return false;
}


/***********************************************************/
void cConfirmPrediction::LoadDefaults()
{
	//\TODO: NoiseLevel,MinAngleRes
	//\TODO: Actually the defaults should be chosen from the choosen technology type. 
	//\TODO: The best will be to set the noise level as a level per Hz

	string setting;
	setting = gDb.GetSetting("DisplayUnits");
	if(setting!="")
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText(QString::fromStdString(setting),Qt::MatchCaseSensitive));
	else
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText("dBm",Qt::MatchCaseSensitive));

	setting = gDb.GetSetting("RqSN");
	if(setting!="")
		RqSNSpinBox->setValue(atof(setting.c_str()));
	else
		RqSNSpinBox->setValue(8.00);
	
	setting = gDb.GetSetting("FadeMargin");
	if(setting!="")
		fadeMarginSpin->setValue(atof(setting.c_str()));
	else
		fadeMarginSpin->setValue(3.00);
	
	setting = gDb.GetSetting("RxMin");
	if(setting!="")
		rxMinSpinBox->setValue(atof(setting.c_str()));
	else
		rxMinSpinBox->setValue(-110.00);
	
	setting = gDb.GetSetting("RqCiCo");
	if(setting!="")
		rqCociSpinBox->setValue(atof(setting.c_str()));
	else
		rqCociSpinBox->setValue(9.00);
	
	setting = gDb.GetSetting("RqCiad");
	if(setting!="")
		rqCiadSpinBox->setValue(atof(setting.c_str()));
	else
		rqCiadSpinBox->setValue(-9.00);
		
	setting = gDb.GetSetting("NoiseLevel");
	if(setting!="")
		noiseLevelSpinBox->setValue(atof(setting.c_str()));
	else
		noiseLevelSpinBox->setValue(-123.00);
		
	setting = gDb.GetSetting("MinAngleRes");
	if(setting!="")
		minAngleResSpinBox->setValue(atof(setting.c_str()));
	else
		minAngleResSpinBox->setValue(1.00);
	
	setting = gDb.GetSetting("RqEbNo");
	if(setting!="")
		rqEbNoSpinBox->setValue(atof(setting.c_str()));
	else
		rqEbNoSpinBox->setValue(8.00);
	
	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		kFactorServerSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorServerSpinBox->setValue(1.00);
	
	setting = gDb.GetSetting("kFactorInt");
	if(setting!="")
		kFactorIntSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorIntSpinBox->setValue(2.5);
	
	setting = gDb.GetSetting("UseClutter");
	if(setting=="true")
		useClutterCheckBox->setChecked(true);
	else
		useClutterCheckBox->setChecked(false);
	
	setting = gDb.GetSetting("BTLDir");
	if(setting!="")
		mBTLDir = (QString::fromStdString(setting));
	else
		mBTLDir = "Data/BTL/";
	
	setting = gDb.GetSetting("OutputDir");
	if(setting!="")
		outputDirectoryEdit->setText(QString::fromStdString(setting));
	else
		outputDirectoryEdit->setText("Data/Output/");
	
	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
		plotResolutionSpinBox->setValue(atof(setting.c_str()));
	else
		plotResolutionSpinBox->setValue(20.00);
	
	setting = gDb.GetSetting("DownLink");
	if(setting=="true")
		downLinkRadio->setChecked(true);
	else
		upLinkRadio->setChecked(true);
		
	string query = "SELECT id,name FROM mobile";
	pqxx::result Mobiles;
	if (!gDb.PerformRawSql(query))
	{
		string err = "Database Select on mobile table failed ";
		QMessageBox::information(this, "QRap", "Database Select on mobile table failed");
//		QRAP_WARN(err.c_str());
		cout << err << endl;		
	}
	gDb.GetLastResult(Mobiles);
	if (Mobiles.size() >0)
	{
		QString Mob = "";
		for (int i =0;i<Mobiles.size();i++)
		{
			Mob = Mobiles[i]["id"].c_str();
			Mob += ":";
			Mob += Mobiles[i]["name"].c_str();
			mobileCombo->addItem(Mob);
		}
	}
	else
	{
		string err = "There seem to be no Mobile installations defined ";
		QMessageBox::information(this, "QRap", "There seem to be no Mobile installations defined.");
//		QRAP_WARN(err.c_str());
		cout << err << endl;	
	}
	
	//Nuwe File Handling....
	/*int i=0;
	QString FileName = QString ("%1/%2%3").arg(outputDirectoryEdit->text()).arg(outputFileNameEdit->text()).arg(i);
	QFile F;
	
	while (F.exists(FileName))
	{
		i++;
		FileName = QString ("%1/%2%3").arg(outputDirectoryEdit->text()).arg(outputFileNameEdit->text()).arg(i);
	}
	outputFileNameEdit->setText(QString("%1%2").arg(outputFileNameEdit->text()).arg(i));
	//Tot Hier
	* */
	//Nuwe Nuwe FileHandling...
	outputFileNameEdit->setText("CombinedCoverage.img");
}


/***********************************************************/
void cConfirmPrediction::on_btnCancel_clicked()
{
	close();
}

/***********************************************************/
void cConfirmPrediction::on_btnDone_clicked()
{
	close();
}

/***********************************************************/
void cConfirmPrediction::SetOwnColours()
{
	int r,g,b,a;
	{
	QgsColorRampShader::ColorRampItem myNewColorRampItem;
	r = (int)0;
	g = (int)0;
	b = (int)0;
	switch(mPlotType) // set up of the prediction's colour type
	{
		case Cov:			
			myNewColorRampItem.value = RequiredMinimumReceiverLevel;
			r = (int)255;
			break;
		case NumServers:	
			myNewColorRampItem.value = 1;
			g = (int)255;
			break;
		case DEM:			
			myNewColorRampItem.value = -10;		
			b = (int)255;
			break;
		case IntRatioCo:	
			myNewColorRampItem.value = -RequiredCoChannelCarrierToInterference;
			r = (int)255;
			break;
		case IntRatioAd:	
			myNewColorRampItem.value = -RequiredAdjacentCarrierToInterference;	
			r = (int)255;
			break;
		case NumInt:		
			myNewColorRampItem.value = 0;	
			g = (int)255;
			break;
		case SN:			
			myNewColorRampItem.value = -RequiredSignalToNoise;	
			r = (int)255;
			break;		
		default:			
			myNewColorRampItem.value = -120;	
			r = (int)255;
			break;
	}
	a = (int)255;
	QColor Col =  QColor(r,g,b,a);
	myNewColorRampItem.color = Col;
       			
	mColorRampItems.push_back(myNewColorRampItem);
	}
	{
	QgsColorRampShader::ColorRampItem myNewColorRampItem;
	r = (int)0;
	g = (int)0;
	b = (int)0;
	switch(mPlotType) // set up of the prediction's colour type
	{
		case Cov:			
			myNewColorRampItem.value = 0.5*RequiredMinimumReceiverLevel;
			b = (int)255;
			break;
		case NumServers:	
			myNewColorRampItem.value = 0.5*NumberOfFixedInstallations;	
			b = (int)255;
			break;
		case DEM:			
			myNewColorRampItem.value = 500;	
			g = (int)255;
			break;
		case IntRatioCo:	
			myNewColorRampItem.value = RequiredCoChannelCarrierToInterference;
			g = (int)255;
			break;
		case IntRatioAd:	
			myNewColorRampItem.value = 2.0*RequiredAdjacentCarrierToInterference;
			g = (int)255;
			break;
		case NumInt:		
			myNewColorRampItem.value = 0.5*NumberOfFixedInstallations;
			b = (int)255;
			break;
		case SN:			
			myNewColorRampItem.value = RequiredSignalToNoise;
			b = (int)255;
			break;		
		default:			
			myNewColorRampItem.value = 2000;	
			g = (int)255;
			break;
	}
	a = (int)255;
	QColor Col =  QColor(r,g,b,a);
	myNewColorRampItem.color = Col;
	    			
	mColorRampItems.push_back(myNewColorRampItem);
	}
	{
	r = (int)0;
	g = (int)0;
	b = (int)0;
	QgsColorRampShader::ColorRampItem myNewColorRampItem;
	switch(mPlotType) // set up of the prediction's colour type
	{
		case Cov:			
			myNewColorRampItem.value = 0; 
			g = (int)255;
			break;
		case NumServers:	
			myNewColorRampItem.value = NumberOfFixedInstallations;
			r = (int)255;
			break;
		case DEM:			
			myNewColorRampItem.value = 2225;
			r = (int)255;
			break;
		case IntRatioCo:	
			myNewColorRampItem.value = 2.0*RequiredCoChannelCarrierToInterference;
			g = (int)255;
			break;
		case IntRatioAd:	
			myNewColorRampItem.value = 2.0*RequiredAdjacentCarrierToInterference;
			g = (int)255;
			break;
		case NumInt:		
			myNewColorRampItem.value = NumberOfFixedInstallations;
			r = (int)255;
			break;
		case SN:			
			myNewColorRampItem.value = RequiredSignalToNoise;
			g = (int)255;
			break;		
		default:			
			myNewColorRampItem.value = 3500;
			r = (int)255;
			break;
	}
	a = (int)255;
	QColor Col =  QColor(r,g,b,a);
	myNewColorRampItem.color = Col;
	mColorRampItems.push_back(myNewColorRampItem);
	}
	if (mPlotType==DEM)
	{
		QgsColorRampShader::ColorRampItem myNewColorRampItem;
		myNewColorRampItem.value = 3500;
		a = (int)255;
		r = (int)0;
		g = (int)255;
		b = (int)255;
	     			
		QColor Col =  QColor(r,g,b,a);
		myNewColorRampItem.color = Col;
		mColorRampItems.push_back(myNewColorRampItem);
	}
}

/***********************************************************/
void cConfirmPrediction::on_btnDo_clicked()
{
	
	rxMinSpinBox->setEnabled(false);
	plotResolutionSpinBox->setEnabled(false);
	kFactorIntSpinBox->setEnabled(false);
	kFactorServerSpinBox->setEnabled(false);
	minAngleResSpinBox->setEnabled(false);
	rqCociSpinBox->setEnabled(false);
	rqCiadSpinBox->setEnabled(false);
	rqEbNoSpinBox->setEnabled(false);
	fadeMarginSpin->setEnabled(false);
	noiseLevelSpinBox->setEnabled(false);
	RqSNSpinBox->setEnabled(false);
	mobileCombo->setEnabled(false);
	outputDirectoryEdit->setEnabled(false);
	outputFileNameEdit->setEnabled(false);
	outputTypeCombo->setEnabled(false);
	plotTypeCombo->setEnabled(false);
	displayUnitsCombo->setEnabled(false);
	useClutterCheckBox->setEnabled(false);
	UpDownLink->setEnabled(false);
	downLinkRadio->setEnabled(false);
	upLinkRadio->setEnabled(false);
	pushButtonPrint->setEnabled(false);
	btnDone->setEnabled(false);
	btnCancel->setEnabled(false);
	btnDo->setEnabled(false);
	btnCancel->setVisible(false);
	btnDo->setVisible(false);
	radioArea->setEnabled(false);
	radioRadius->setEnabled(false);

	mRadInst.clear();
	mRanges.clear();
	double North,West,South,East;
	NorthWestCorner.SetGeoType(DEG);
	SouthEastCorner.SetGeoType(DEG);
	NorthWestCorner.Get(North,West);
	SouthEastCorner.Get(South,East);
	cout << "North: " << North << "	South: " << South << "	East: " << East << "	West: " << West << endl;
	if (plotTypeCombo->currentIndex()!=11)
	{
		for (int i = 0; i < tableWidget->rowCount();i++)
		{
			void *Wid = tableWidget->cellWidget(i,0);
			QCheckBox *get =(QCheckBox*) Wid;
			if (get->isChecked())
			{
				bool ok;
				double Rad =tableWidget->item(i,2)->text().toDouble(&ok);
				mRadInst.push_back((unsigned)tableWidget->item(i,4)->text().toDouble());
				if ((plotResolutionSpinBox->value()/1000 <=Rad) && ok)
				{
					mRanges.push_back(tableWidget->item(i,2)->text().toDouble());
				}
				else
				{
					mRanges.push_back(plotResolutionSpinBox->value()/1000);
				}
				if (radioRadius->isChecked())
				{
					double Lat,Lon;
					string PointString;
					unsigned spacePos; 
					double Radius = tableWidget->item(i,2)->text().toDouble()*1000;
					string query = "SELECT ST_AsText(location) AS location"; 
					query += " FROM radioinstallation cross join site WHERE ";
					query += " siteid = site.id AND ";
					query += "radioinstallation.id  = ";
					query += tableWidget->item(i,4)->text().latin1();
					if (!gDb.PerformRawSql(query))
					{
						string err = "Database Select on sites table to get plot size failed ";
						QMessageBox::information(this, "QRap", "Database Select on sites table to get plot size failed");
//						QRAP_WARN(err.c_str());
						cout << err << endl;
					}
					pqxx::result SiteLoc;
					gDb.GetLastResult(SiteLoc);
					if (SiteLoc.size() >0)
					{	
						PointString = SiteLoc[0]["location"].c_str();
						spacePos = PointString.find_first_of(' ');
						Lon = atof((PointString.substr(6,spacePos).c_str())); 
						Lat = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
						cGeoP Temp;
						Temp.Set(Lat,Lon,DEG);
						North = max(North,Lat);
						South = min(South,Lat);
						East = max(East,Lon);
						West = min(West,Lon);
						cGeoP NewPoint;
						NewPoint.FromHere(Temp,Radius,0);
						Temp.Display();
						NewPoint.Display();
					
						//Check North
						NewPoint.Get(Lat,Lon);
						cout << "Dist: " << Temp.Distance(NewPoint) << endl;;
						cout << "RADIUS: ******** " << Radius <<endl;
						cout << "North: " << North << "	Lat: " << Lat<< endl;
						North = max(North,Lat);
						South = min(South,Lat);
						East = max(East,Lon);
						West = min(West,Lon);
						//Check East
						NewPoint.FromHere(Temp,Radius,90);
						NewPoint.Get(Lat,Lon);
						North = max(North,Lat);
						South = min(South,Lat);
						East = max(East,Lon);
						West = min(West,Lon);
						cout << "Dist: " << Temp.Distance(NewPoint) << endl;
						cout << "East: " << East << "	Lon: " << Lon << endl;
						//Check South
						NewPoint.FromHere(Temp,Radius,180);
						NewPoint.Get(Lat,Lon);
						North = max(North,Lat);
						South = min(South,Lat);
						East = max(East,Lon);
						West = min(West,Lon);
						cout << "Dist: " << Temp.Distance(NewPoint) << endl;
						cout << "South: " << South << "	Lat: " << Lat<< endl;
						//Check West
						NewPoint.FromHere(Temp,Radius,270);
						NewPoint.Get(Lat,Lon);
						North = max(North,Lat);
						South = min(South,Lat);
						East = max(East,Lon);
						West = min(West,Lon);
						cout << "Dist: " << Temp.Distance(NewPoint) << endl;
						cout << "West: " << West << "	Lon: " << Lon << endl;
						NorthWestCorner.Set(North,West);
						SouthEastCorner.Set(South,East);
						cout << "North: " << North << endl;
						cout << "South: " << South << endl;
						cout << "East: " << East << endl;
						cout << "West: " << West << endl;
					}
					else
					{
						string err = "Query result for site to get radius is empty.";
						QMessageBox::information(this, "QRap", "Query result for site to get radius is empty.");
//						QRAP_WARN(err.c_str());
						cout << err << endl;	
					}
				}
			}
		}
	}

	switch (plotTypeCombo->currentIndex())
	{
		case 0:		mPlotType = Cov;			break;
		case 1:		mPlotType = PrimServer;		break;
		case 2:		mPlotType = SecondServer;	break;
		case 3:		mPlotType = NumServers;		break;
		case 4:		mPlotType = IntRatioCo;		break;
		case 5:		mPlotType = IntRatioAd;		break;
		case 6:		mPlotType = IntAreas;		break;
		case 7:		mPlotType = NumInt;		break;
		case 8:		mPlotType = PrimIntCo;		break;
		case 9:		mPlotType = PrimIntAd;		break;
		case 10:	mPlotType = SN;			break;
		case 11:	mPlotType = DEM;			break;
//		case 12:	mPlotType =  EbNo;		break;
//		case 13:	mPlotType = ServiceLimits;	break;
		default:	mPlotType = Cov;			break;			
	}
	switch (displayUnitsCombo->currentIndex())
	{
		case 0:		DisplayUnits = dB;		break;
		case 1:		DisplayUnits = dBW;		break;
		case 2:		DisplayUnits = dBm;		break;
		case 3:		DisplayUnits = dBuV;		break;
		case 4:		DisplayUnits = dBuVm;		break;
		case 5:		DisplayUnits = dBWm2Hz;		break;
		case 6:		DisplayUnits = dBWm2;		break;
		default:	DisplayUnits = dBm;		break;			
	}
	if (downLinkRadio->isChecked())
		DownLink = true;
	else
		DownLink = false;
	RequiredSignalToNoise = RqSNSpinBox->value();
	RequiredMinimumReceiverLevel= rxMinSpinBox->value();
	FadeMargin = fadeMarginSpin->value();
	RequiredCoChannelCarrierToInterference=rqCociSpinBox->value();
	RequiredAdjacentCarrierToInterference=rqCiadSpinBox->value();
	RequiredEnergyPerBitToNoiseRatio=rqEbNoSpinBox->value();
	NoiseLevel=noiseLevelSpinBox->value();
	kFactorForServers=kFactorServerSpinBox->value();
	kFactorForInterferers=kFactorIntSpinBox->value();
	
	string query = "SELECT id,orderarray from filesetsused WHERE type = 'DEM'";
	pqxx::result DEMset,Resolution;
	if (!gDb.PerformRawSql(query))
	{
		string err = "Database select on FileSetsUsed for DEM failed";
		QMessageBox::information(this, "QRap", "Database select on FileSetsUsed for DEM failed");
//		QRAP_WARN(err.c_str());
		cout << err << endl;
	}
	QString DEMRes,DEMFileKey;
	gDb.GetLastResult(DEMset);
	if (DEMset.size() >0)
	{
		cout << "HERE WE ARE SELECTING THE RESOLUTION:"<<endl;
		//Why?
		DEMsourceList = (int)atof(DEMset[0]["id"].c_str());
		cout << "Source List: " << DEMsourceList <<endl;
		string QueryResult=DEMset[0]["orderarray"].c_str();
		int NumBytes = QueryResult.length();
		int i=0;
		IntArray FileSetOrder;
		cout << "before while" << endl;
		if (NumBytes)
		{
	   		while ((i<NumBytes)&&(QueryResult[i]!='}'))
   			{
   				int temp = 0;
   				while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
			    	{
				 	temp =  temp*10 + (QueryResult[i]-'0');
   					i++;
   				}
			    	if (QueryResult[i]!='}') 
					i++;
				cout << temp << endl;
			    	FileSetOrder.push_back(temp);
			}// end while i<NumBytes
			DEMFileKey = QString("%1").arg(FileSetOrder[0]);
			cout << DEMFileKey.toStdString() << endl;
			query = "SELECT resolution FROM filesets WHERE id=";
			query+=DEMFileKey.toStdString();
			if (!gDb.PerformRawSql(query))
				cout << "Error in query: " << query << endl;
			gDb.GetLastResult(Resolution);
//			if (Resolution.size() >0)
//				DEMRes = Resolution[0]["resolution"].c_str();
			//else DEMRes = "90";
		}
		else
		{
			string err = "The File set Order for the DEM seems to be empty. ";
			err += "Go the QRap Database Interface, File, Import, Rasters to set it up";
			QMessageBox::information(this, "QRap",err.c_str());
//			QRAP_WARN(err.c_str());
			cout << err << endl;	
		}
	}
	else
	{
		string err = "The File set Order for the DEM seems to be empty. ";
		err += "Go the QRap Databace Interface, File, Import, Rasters to set it up";
		QMessageBox::information(this, "QRap", err.c_str());
//		QRAP_WARN(err.c_str());
		cout << err << endl;	
	}
	
	pqxx::result Clutter;
	if (useClutterCheckBox->isChecked())
	{
		query = "SELECT id from filesetsused WHERE type = 'Clutter'";
		if (!gDb.PerformRawSql(query))
		{
			string err = "Database Select on FileSetsUsed for Clutter failed";
			QMessageBox::information(this, "QRap", "Database Select on FileSetsUsed for Clutter failed");
//			QRAP_WARN(err.c_str());
			cout << err << endl;
		}
		gDb.GetLastResult(Clutter);
		if (Clutter.size() >0)
		{
			ClutterSourceList = (int)atof(Clutter[0]["id"].c_str());
			UseClutterDataInPathLossCalculations = true;
		}
		else
		{
			ClutterSourceList = 1;
			UseClutterDataInPathLossCalculations = false;
		}
	}
	else
	{
		ClutterSourceList = 1;
		UseClutterDataInPathLossCalculations = false;
	}
	
	PlotResolution=plotResolutionSpinBox->value();
	if (PlotResolution <= DEMRes.toDouble())
	{
		PlotResolution = DEMRes.toDouble();
	}
	cout << "Plot Res: " << PlotResolution << endl;
	MinimumAngularResolution=minAngleResSpinBox->value();
	
	///////////// MOBILE KEY!!!!!!!
	cout << "cConfirmPrediction::on_btnDo_clicked(). Mobile key: ";
	QString Mob = mobileCombo->currentText();
	Mob=Mob.mid(0,Mob.indexOf(":"));
	MobileInstallationKey=(unsigned int)Mob.toDouble();
	cout << MobileInstallationKey << endl;
	
	NumberOfFixedInstallations=mRadInst.size();
	FixedInstallationKeys = new unsigned[NumberOfFixedInstallations];
	CoverangeRanges= new double[NumberOfFixedInstallations]; // In Kilometer
	
	for (int i = 0; i < NumberOfFixedInstallations;i++)
	{
		FixedInstallationKeys[i] = mRadInst[i];
		CoverangeRanges[i] = mRanges[i];
	}
	DirectoryToStoreResult = outputDirectoryEdit->text().latin1();
	OutputFileForResult = outputFileNameEdit->text().latin1();
	
	bool FileWritten=false;
	cPlotTask Prediction;
	Prediction.SetPlotTask(	mPlotType, DisplayUnits, DownLink,
				RequiredSignalToNoise, RequiredMinimumReceiverLevel,
				FadeMargin, RequiredCoChannelCarrierToInterference,
				RequiredAdjacentCarrierToInterference, RequiredEnergyPerBitToNoiseRatio,
				NoiseLevel, kFactorForServers, kFactorForInterferers,
				DEMsourceList, ClutterSourceList, UseClutterDataInPathLossCalculations,
				NorthWestCorner, SouthEastCorner, PlotResolution,
				MinimumAngularResolution, MobileInstallationKey,
				NumberOfFixedInstallations, FixedInstallationKeys,
				CoverangeRanges, // In Kilometer
				DirectoryToStoreResult, OutputFileForResult);
			
	if (mPlotType == DEM)
	{	
		cout << "Entering DEM in cConfirmPrediction::on_btnDo_clicked()" << endl;
		cRaster Output;
		cRasterFileHandler DEM(DEMsourceList); 
		string Dir = DirectoryToStoreResult.c_str();
		string OutputFile = OutputFileForResult.c_str();
		cGeoP NW = NorthWestCorner;
		cGeoP SE = SouthEastCorner;
		double nNorth,nSouth,nEast,nWest;
		NW.Get(nNorth,nWest);
		SE.Get(nSouth,nEast);
		cGeoP tempP(nNorth,(nWest+nEast)/2.0);
		int central = tempP.DefaultCentMer(WGS84GC);
		double DistRes = PlotResolution;

		unsigned Rows=2, Cols=2;
		Float2DArray Data;
		Data = new_Float2DArray(Rows, Cols);
		DEM.GetForDEM(NW, SE, DistRes, Rows, Cols, Data,DEG);
		NW.SetGeoType(DEG);
		SE.SetGeoType(DEG);
		DistRes*=cDegResT;
		FileWritten = Output.WriteFile(Data, NW, SE, Rows, Cols, DistRes, DistRes,
						Dir,OutputFile,HFA,DEG,DEG,central);
		cout << "Wrote Output in cConfirmPrediction::on_btnDo_clicked()" << endl;
		delete_Float2DArray(Data);
		cout << "Deleted Output Array in cConfirmPrediction::on_btnDo_clicked()" << endl;
	}//end if DEM
 	else if ((mPlotType==Cov)||(mPlotType==PrimServer)||(mPlotType==SecondServer)
 				||(mPlotType==NumServers)||(mPlotType==SN))
 	{
		Prediction.CombineCov();
		FileWritten = Prediction.WriteOutput(DEG);
 	}
 	else
 	{
		Prediction.InterferencePlot();
		FileWritten = Prediction.WriteOutput(DEG);
 	}
 	
 	if (!FileWritten)
 	{
 		string err = "Could not write the output. ";
 		err+= "Do you have write permission to the directory: '";
 		err+=DirectoryToStoreResult.c_str();
 		err += "/'? You can change it under Plot Preferences. ";
 		QString fout = "Could not write the output. ";
 		fout += "Do you have write permission to the directory: '";
 		fout += DirectoryToStoreResult.c_str();
 		fout += "/'? You can change it under Plot Preferences. ";
 		QMessageBox::information(mQGisIface->mainWindow(), "QRap", fout);
 		QRAP_ERROR(err.c_str());
 		return;	
 	}

	string Plot;
	bool discrete;
	bool getFromDB;
	switch(mPlotType) // set up of the prediction's colour type
	{
		case Cov:		Plot = "Coverage";			getFromDB = true;	discrete = false;	break;
		case PrimServer:	Plot = "Primary Server";		getFromDB = false;	discrete = false;	break;
		case SecondServer:	Plot = "Secondary Server";		getFromDB = false;	discrete = false;	break;
		case NumServers:	Plot = "Number of Servers";		getFromDB = true;	discrete = false;	break;
		case DEM:		Plot = "Digital Elevation Model";	getFromDB = true;	discrete = false;	break;
		case IntRatioCo:	Plot = "Carrier to Co-channel Interference Ratio"; 	getFromDB = true;	discrete = false;	break;
		case IntRatioAd:	Plot = "Carrier to Adjacent-channel Interf Ratio";	getFromDB = true;	discrete = false;	break;
		case IntAreas:		Plot = "Interfered Areas"; 		getFromDB = false;	discrete = false;	break;
		case NumInt:		Plot = "Number of Interferers"; 	getFromDB = true;	discrete = false;	break;
		case PrimIntCo:		Plot = "Primary Co-channel Interferers";	getFromDB = false;	discrete = false;	break;
		case PrimIntAd:		Plot = "Primary Adjacent-channel Interferers"; 	getFromDB = false;	discrete = false;	break;
		case SN:		Plot = "Signal to Noise Ratio"; 	getFromDB = true;	discrete = false;	break;	
//		case EbNo:		Plot = "Energy per Bit to Noise Power"; getFromDB = true;	discrete = false;	break;
//		case ServiceLimits;	Plot = "Service Limiters"; 		getFromDB = true;	discrete = false;	break;	
		default:		Plot = "";				getFromDB = true;	discrete = false;	break;
	}
	QString File = DirectoryToStoreResult.c_str();
	File +="/"; //\TODO: Windows....
	File+=OutputFileForResult.c_str();
	QgsRasterLayer *mRasterLayerO = mQGisIface->addRasterLayer(File);
	QgsRasterTransparency *Deurskynend= new QgsRasterTransparency();
	QgsRasterTransparency::TransparentSingleValuePixel myTransparentPixel; // setting null value transparancy
	QList<QgsRasterTransparency::TransparentSingleValuePixel> myTransparentSingleValuePixelList;// setting null value transparancy
	myTransparentPixel.percentTransparent = 100.00;// setting null value transparancy
	myTransparentPixel.min = -10000;// setting null value transparancy
	myTransparentPixel.max = -9998;// setting null value transparancy
	myTransparentSingleValuePixelList.append(myTransparentPixel);// setting null value transparancy
	Deurskynend->setTransparentSingleValuePixelList( myTransparentSingleValuePixelList );

  	QgsRasterShader* rasterShader = new QgsRasterShader();
  	QgsColorRampShader* colorRampShader = new QgsColorRampShader();
  	colorRampShader->setColorRampType( QgsColorRampShader::INTERPOLATED );
    	if(colorRampShader)
    	{
      		//iterate through mColormapTreeWidget and set colormap info of layer
		pqxx::result res;
      		string query = "SELECT * FROM colourmanagement WHERE plottype ='";
		query +=Plot;
		query +="';";
		cout <<query << endl;
		if (!gDb.PerformRawSql(query))
		{
			cout <<"Error selecting colours" << endl;
			SetOwnColours();
		}
		else
		{
			gDb.GetLastResult(res);
			int r,g,b,a;	
			if (getFromDB)
			{
				if (res.size() == 0) // If no colours has been set up
				{
					cout <<"Colours in database empty" << endl;
					SetOwnColours();
					cout <<" Done with own colours" << endl;
				}
				else
				{
	 				for(int i =0; i < res.size(); i++)
      					{
        					QgsColorRampShader::ColorRampItem myNewColorRampItem;
        					myNewColorRampItem.value = atof(res[i]["val"].c_str());
        					r = (int)atof(res[i]["r"].c_str());
      						g = (int)atof(res[i]["g"].c_str());
      						b = (int)atof(res[i]["b"].c_str());
      						a = (int)atof(res[i]["a"].c_str());
      						cout << res[i]["val"].c_str() << "\t" << r << "\t"<< g << "\t"<< b << endl;
      						QColor Col =  QColor(r,g,b,a);
        					myNewColorRampItem.color = Col;
        					myNewColorRampItem.label = res[i]["label"].c_str();
        					mColorRampItems.push_back(myNewColorRampItem);
      					}//end for
				}
 			}//getFromDB
 			else
 			{ // this clause takes care of primary/secondary servers
 				int j = 0;
		 		srand(NumberOfFixedInstallations);
		 		for(int i =0; i < NumberOfFixedInstallations; i++)
		      		{
		 			int Kies=0;
		 			int BeginV=0;
		      			QgsColorRampShader::ColorRampItem myNewColorRampItem;
		      			myNewColorRampItem.value = FixedInstallationKeys[i];
		      			if (j < res.size())
		      			{
//			      			if (j == res.size())
// 		    				j = 0;	
   		   				r = (int)atof(res[j]["r"].c_str());
   		   				g = (int)atof(res[j]["g"].c_str());
   		   				b = (int)atof(res[j]["b"].c_str());
   		   				a = (int)atof(res[j]["a"].c_str());
   		   				j++;
						myNewColorRampItem.label = res[i]["label"].c_str();
   		   			}
   		   			else  // generate random colours if no colours are found
   		   			{
   		   				if (i<6)
   		   				{
   			     				Kies = (i%3);
   			     				BeginV = 2*((i/3)%2); // has value of 0 or 2
   		   					r = 127.5*BeginV;
   		   					g = 127.5*BeginV;
   		   					b = 127.5*BeginV;
   		   					if (Kies==0) 		r += 127.5*2.0*(1-BeginV);  
   		   					else if (Kies==1) 	b += 127.5*2.0*(1-BeginV);
   		   					else if (Kies==2) 	g += 127.5*2.0*(1-BeginV);
   		   				}
   		   				else
   		   				{
   		   					r = (int) (rand() %7)*255.0/7.0;
   		   					g = (int) (rand() %4)*255.0/4.0;
   		   					b = (int) (rand()% 5)*255.0/5.0;
   		   				}
   		   				a = (int) 255;
   		   				cout << "r: " << r << "  g: " << g << "  b: " << b << "  a: " << a << endl;
   		   				myNewColorRampItem.label = myNewColorRampItem.value;
   		   			}
   	   				
   	   				QColor Col =  QColor(r,g,b,a);
   		     			myNewColorRampItem.color = Col;
   		     			mColorRampItems.push_back(myNewColorRampItem);
				}// for number of inst
   	   		}//end found db info for primary/secondary servers
		}// end Database success
    		colorRampShader->setColorRampItemList(mColorRampItems);
  	
      		if (discrete)
      			colorRampShader->setColorRampType(QgsColorRampShader::DISCRETE);
      		else
      			colorRampShader->setColorRampType(QgsColorRampShader::INTERPOLATED);
 		rasterShader->setRasterShaderFunction( colorRampShader );
  		QgsSingleBandPseudoColorRenderer* r = 
				new QgsSingleBandPseudoColorRenderer( mRasterLayerO->dataProvider(), 1, rasterShader );
		r->setRasterTransparency(Deurskynend);
  		mRasterLayerO->setRenderer( r );

    	} //end if there is a RasterShader

    	else
    	{
        	printf("QgsRasterLayerProperties::apply color ramp was NOT set because RasterShaderFunction was NULL");
    	}
	mQGisIface->mapCanvas()->refresh(); 
	pushButtonPrint->setEnabled(true);
	btnDone->setEnabled(true);
}

//*************************************************************************************
void cConfirmPrediction::on_plotTypeCombo_currentIndexChanged(QString File)
{
	 File = File.remove(" ");
	 outputFileNameEdit->setText(QString("%1.img").arg(File));
	 if ((File == "DigitalElevationModel")||(plotTypeCombo->currentIndex()==11))
	 {
	 	displayUnitsCombo->setEnabled(false);
	 	RqSNSpinBox->setEnabled(false);
	 	rxMinSpinBox->setEnabled(false);
	 	fadeMarginSpin->setEnabled(false);
	 	rqCociSpinBox->setEnabled(false);
	 	rqCiadSpinBox->setEnabled(false);
	 	rqEbNoSpinBox->setEnabled(false);
	 	downLinkRadio->setEnabled(false);
	 	upLinkRadio->setEnabled(false);
	 	noiseLevelSpinBox->setEnabled(false);
	 	kFactorServerSpinBox->setEnabled(false);
	 	kFactorIntSpinBox->setEnabled(false);
	 	minAngleResSpinBox->setEnabled(false);
	 	mobileCombo->setEnabled(false);
	 	radioArea->setChecked(true);
	 }
	 else
	 {
	 	displayUnitsCombo->setEnabled(true);
	 	RqSNSpinBox->setEnabled(true);
	 	rxMinSpinBox->setEnabled(true);
	 	fadeMarginSpin->setEnabled(true);
	 	rqCociSpinBox->setEnabled(true);
	 	rqCiadSpinBox->setEnabled(true);
	 	rqEbNoSpinBox->setEnabled(true);
	 	downLinkRadio->setEnabled(true);
	 	upLinkRadio->setEnabled(true);
	 	noiseLevelSpinBox->setEnabled(true);
	 	kFactorServerSpinBox->setEnabled(true);
	 	kFactorIntSpinBox->setEnabled(true);
	 	minAngleResSpinBox->setEnabled(true);
	 	mobileCombo->setEnabled(true);
	 	radioRadius->setChecked(true);
	 }
}

//*************************************************************************************
// This is very much based on qgsquickprint.cpp by Tim Sutton 2008 - thank you
//
void cConfirmPrediction::on_pushButtonPrint_clicked()
{
	string err="The print option is no longer available. Use the QGIS D.print composer instead";
	QRAP_ERROR(err.c_str());
/*	QPrinter* myPrinter = new QPrinter (QPrinter::PrinterResolution );
	myPrinter->setDocName(plotTypeCombo->currentText());

	QString TitleText = plotTypeCombo->currentText();
	myPrinter->setPageSize(QPrinter::A4); 
  	myPrinter->setOrientation(QPrinter::Landscape);  

	int lineLength = myPrinter->pageRect().width();
	int row = 0;

	QPrintDialog printDialog( myPrinter, parentWidget() );
	if ( !printDialog.exec() == QDialog::Accepted )
      		return;
  	myPrinter->setOrientation( QPrinter::Landscape );  
	QPainter* myPainter = new QPainter( myPrinter );
   		
	myPainter->setPen( QPen(Qt::black) ); 
	//set the top left origin for the print layout
	int myOriginX = myPrinter->pageRect().left();
	int myOriginY = myPrinter->pageRect().top();
	int myDrawableWidth = myPrinter->pageRect().width() - myOriginX;
	int myDrawableHeight = myPrinter->pageRect().height() - myOriginY;
	//define the spacing between layout elements
	int myHorizontalSpacing = myDrawableWidth / 100; // 1%
	int myVerticalSpacing = myDrawableHeight / 100; // 1%

 	//define the proportions for the page layout
  	int myMapWidthPercent = 85;
  	int myMapHeightPercent = 75;
  	int myLegendWidthPercent = 15;
  	int myLegendHeightPercent = 75;
	int myPlotInfoPercent = 25;
  	//
	cout << " Na sizing Definition" << endl;
	int myOriginalDpi = mMapRenderer->outputDpi();
	cout << " Na Getting original DPi" << endl;
	//sensible default to prevent divide by zero
  	if ( 0 == myOriginalDpi ) myOriginalDpi = 96;
	cout << " Na if DPi" << endl;
  	QSize myOriginalSize = mMapRenderer->outputSize();
	cout << " Na Getting Originals" << endl;

  	//define the font sizes and family
  	int myMapTitleFontSize = 20;
  	int myMapDateFontSize = 16;
  	int myLegendFontSize = 10;
#ifdef Q_OS_LINUX//Tim does not like this...
  	myLegendFontSize -= 2;
#endif

#ifdef WIN32 //Tim does not like this either...
  	myMapTitleFontSize /= 2;
  	myMapDateFontSize /= 2;
  	myMapNameFontSize /= 2;
  	myLegendFontSize /= 2;
#endif
  	QString myFontFamily = "Arial";

	cout << "Voor titles" << endl;
  	// Background colour for pixmaps
  	QColor myLegendBackgroundColour = Qt::white;
  	//QColor myMapBackgroundColour = "#98dbf9"; // nice blue colour

  	// Draw the PageBorder
  	//
	cout << " Voor draw first rectagle" << endl;
 // 	myPainter->drawRect(myOriginX, myOriginY, myDrawableWidth, myDrawableHeight );
  	//
  	// Draw the PageTitle
  	//
  	QFont myTitleFont( myFontFamily, myMapTitleFontSize );
  	myPainter->setFont( myTitleFont );
  	QFontMetrics myTitleMetrics( myTitleFont, myPrinter );
  	int myPageTitleHeight = myTitleMetrics.height();
  	int myPageTitleWidth = myTitleMetrics.width( TitleText );
  	QRect myPageTitleRect( myOriginX+myHorizontalSpacing, myOriginY-(myPageTitleHeight/2),
        	                 myPageTitleWidth, myPageTitleHeight );
  	// make sure the title goes onto a white background
  	myPainter->setPen( Qt::white );
  	myPainter->drawRect( myPageTitleRect );
  	myPainter->setPen( Qt::black );
	TitleText = plotTypeCombo->currentText();
  	myPainter->drawText( myPageTitleRect, Qt::AlignCenter, TitleText );

	cout << "Voor datums" << endl;
 	 //
 	 // Draw the MapDate
  	//
  	QFont myDateFont( myFontFamily, myMapDateFontSize );
  	QString myDateText( QDate::currentDate().toString( Qt::LocalDate ) );
  	myPainter->setFont( myDateFont );
  	QFontMetrics myDateMetrics( myDateFont, myPrinter );
  	int myDateHeight = myDateMetrics.height();
  	int myDateWidth = myDateMetrics.width(myDateText);
  	QRect myDateRect( myOriginX+myDrawableWidth-myDateWidth-myHorizontalSpacing,
        	            myOriginY-( myPageTitleHeight / 2 ), myDateWidth, myPageTitleHeight );
  	// make sure the title goes onto a white background
  	myPainter->setPen( Qt::white );
  	myPainter->drawRect( myDateRect );
  	myPainter->setPen( Qt::black );
  	myPainter->drawText( myDateRect, Qt::AlignCenter, myDateText );

  	// Draw the map onto a pixmap
  	// @TODO: we need to save teh extent of the screen map and
  	// then set them again for the print map so that the map scales
  	// properly in the print
  	int myMapDimensionX = ( myDrawableWidth / 100 ) * myMapWidthPercent;
  	int myMapDimensionY = ( myDrawableHeight / 100 ) * myMapHeightPercent;

  	QImage myMapImage( QSize( myMapDimensionX, myMapDimensionY ), QImage::Format_ARGB32 );
  	myMapImage.setDotsPerMeterX(( double )( myPrinter->logicalDpiX() ) / 25.4 * 1000.0 );
  	myMapImage.setDotsPerMeterY(( double )( myPrinter->logicalDpiY() ) / 25.4 * 1000.0 );
  	myMapImage.fill( 0 );
  	QPainter myMapPainter;
  	myMapPainter.begin( &myMapImage );
  	// Now resize for print
  	mMapRenderer->setOutputSize( QSize(myMapDimensionX, myMapDimensionY), 
					(myPrinter->logicalDpiX() + myPrinter->logicalDpiY())/2);
  	mMapRenderer->render( &myMapPainter );

  	myMapPainter.end();
  	//draw the map pixmap onto our pdf print device
 	myOriginX = myPrinter->pageRect().left() + myHorizontalSpacing;
  	myOriginY += myVerticalSpacing * 2 + myPageTitleHeight/2;

  	myPainter->drawImage( myOriginX, myOriginY, myMapImage );

	//
  	// Draw the (vector) legend
  	//
  	QFont myLegendFont( myFontFamily, myLegendFontSize );
  	//myPrintPainter.setFont(myLegendFont);
  	int myLegendDimensionX = ( myDrawableWidth / 100 ) * myLegendWidthPercent;
  	int myLegendDimensionY = ( myDrawableHeight / 100 ) * myLegendHeightPercent;

  	// Create a viewport to make coordinate conversions easier
  	// The viewport has the same dimensions as the page(otherwise items
  	// drawn into it will appear squashed), but a different origin.
  	QRect myOriginalViewport = myPainter->viewport(); //for restoring later
  	myOriginX += myMapDimensionX + myHorizontalSpacing;
  	myPainter->setViewport( myOriginX, myOriginY, myOriginalViewport.width(),
        	                      myOriginalViewport.height() );
	//draw a rectangale around the legend frame
  	//@TODO make this user settable
  	if ( 0 == 1 ) //put some real logic here
  	{
    		myPainter->drawRect( 0, 0, myLegendDimensionX, myLegendDimensionY );
  	}
  	//get font metric and other vars needed
  	QFontMetrics myLegendFontMetrics( myLegendFont, myPrinter );
  	int myLegendFontHeight = myLegendFontMetrics.height();
	int myLegendFontWidth = myLegendFontMetrics.width("o");
  	int myLegendXPos = 0;
  	int myLegendYPos = 0;
  	int myLegendSpacer = myLegendFontHeight / 2; //for vertical and horizontal spacing
  	int myLegendVerticalSpacer = myLegendFontHeight / 3; //for vertical between rows
  	int myIconWidth = myLegendFontHeight;
  	myPainter->setFont( myLegendFont );
  	QStringList myLayerSet = mMapRenderer->layerSet();
  	QStringListIterator myLayerIterator( myLayerSet );
	cout << "Voor legends" << endl;
  	//second clause below is to prevent legend spilling out the bottom
  	while ( myLayerIterator.hasNext()&& myLegendYPos < myLegendDimensionY )
  	{
    		QString myLayerId = myLayerIterator.next();
    		QgsMapLayer * mypLayer = QgsMapLayerRegistry::instance()->mapLayer( myLayerId );
    		if ( mypLayer )
    		{
      			QgsVectorLayer *mypVectorLayer  = dynamic_cast<QgsVectorLayer *>( mypLayer );
      			if ( mypVectorLayer )
      			{
        			QString myLayerName = mypVectorLayer->name();
        			QIcon myIcon;
	        		QPixmap myPixmap( QSize( myIconWidth, myIconWidth ) );   //square
        			//based on code from qgslegendlayer.cpp - see that file for more info
        			QgsFeatureRendererV2* mypRenderer = mypVectorLayer->rendererV2();
        			QList<QgsSymbolV2*> mySymbolList = mypRenderer->symbols();
        			//
        			// Single symbol
        			//
        			double widthScale = ( myPrinter->logicalDpiX() + myPrinter->logicalDpiY() ) / 2.0 / 25.4;
	
        			if ( 1 == mySymbolList.size() )
        			{
        	  			QgsSymbolV2 * mypSymbol = mySymbolList.at( 0 );
        	  			//myPainter->setPen( mypSymbol->pen() );
        	  			//myPainter->setBrush( mypSymbol->brush() );
        	  			myLegendXPos = 0 ;
        	  			if ( mypSymbol->type() == QGis::Point )
        	  			{
        	    				QImage myImage;
        	    				myImage = mypSymbol->getPointSymbolAsImage( widthScale );
        	    				myPainter->drawImage( myLegendXPos, myLegendYPos, myImage );
        	  			}
        	  			else if ( mypSymbol->type() == QGis::Line )
        	  			{
        	    				myPainter->drawLine( myLegendXPos, myLegendYPos,
        	                            				myLegendXPos + myIconWidth,
        	                            				myLegendYPos + myIconWidth );
        	  			}
        	  			else //polygon
        	  			{
        	    				myPainter->drawRect( myLegendXPos, myLegendYPos, myIconWidth, myIconWidth );
        	  			}
        	  			myLegendXPos += myIconWidth + myLegendSpacer;
        	  			myPainter->setPen( Qt::black );
        	  			QStringList myWrappedLayerNameList = wordWrap( myLayerName, myLegendFontMetrics,
        	                                      						myLegendDimensionX - myIconWidth );
        	  			//
        	  			// Loop through wrapped legend label lines
        	  			//
        	  			QStringListIterator myLineWrapIterator( myWrappedLayerNameList );
        	  			while ( myLineWrapIterator.hasNext() )
        	  			{
        	    				QString myLine = myLineWrapIterator.next();
        	    				QRect myLegendItemRect( myLegendXPos,myLegendYPos,
        	                           					myLegendDimensionX - myIconWidth, myLegendFontHeight );
        	    				myPainter->drawText( myLegendItemRect, Qt::AlignLeft, myLine );
        	    				myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
        	  			}
        			}//symbol list
        			else  //class breaks
	        		{
        	  			// draw in the layer name first, after we loop for the class breaks
        	 			 QStringList myWrappedLayerNameList = wordWrap( myLayerName, myLegendFontMetrics,
        	                                      					 myLegendDimensionX - myIconWidth );
        	  			// Check the wrapped layer name wont overrun the space we have
        	  			// for the legend ...
        	  			int myLabelHeight = myLegendFontHeight * myWrappedLayerNameList.count();
        	  			if ( myLegendYPos + myLabelHeight > myLegendDimensionY )
        	  			{
        	    				continue;
        	  			}
				        //
				        // Loop through wrapped legend label lines
        	  			//
        	  			QStringListIterator myLineWrapIterator( myWrappedLayerNameList );
        	  			while ( myLineWrapIterator.hasNext() )
        	  			{
        	    				QString myLine = myLineWrapIterator.next();
        	    				myLegendXPos = myIconWidth;
        	    				QRect myLegendItemRect( myLegendXPos,myLegendYPos,
        	                            				myLegendFontMetrics.width( myLine ),myLegendFontHeight );
        	    				myPainter->setPen( Qt::black );
        	    				myPainter->drawText( myLegendItemRect, Qt::AlignLeft, myLine );
        	   				myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
        	 			}
        	  			//
        	  			// Loop through the class breaks
        	  			//
        	 			QListIterator<QgsSymbolV2 *> myIterator( mySymbolList );
        	  			while ( myIterator.hasNext() && myLegendYPos < myLegendDimensionY )
        	  			{
        	    				QgsSymbolV2 * mypSymbol = myIterator.next();
        	    				myPainter->setPen( mypSymbol->pen() );
        	    				myPainter->setBrush( mypSymbol->brush() );
        	    				myLegendXPos = myLegendSpacer * 3; //extra indent for class breaks
        	    				if ( mypSymbol->type() == QGis::Point )
        	    				{
        	      					QImage myImage;
        	      					myImage = mypSymbol->getPointSymbolAsImage( widthScale );
        	      					myPainter->drawImage( myLegendXPos, myLegendYPos, myImage );
        	    				}
        	    				else if ( mypSymbol->type() == QGis::Line )
        	    				{
        	      					myPainter->drawLine( myLegendXPos, myLegendYPos, myLegendXPos + myIconWidth,
        	                               					myLegendYPos + myIconWidth );
        	    				}
        	    				else //polygon
        	    				{	
        	      					myPainter->drawRect(myLegendXPos, myLegendYPos, myIconWidth, myIconWidth );
        	    				}
        	    				//
        	    				// Now work out the class break label
        	    				//
        	    				QString myLabel;
        	    				QString myLower = mypSymbol->lowerValue();
        	    				if ( !myLower.isEmpty() )
        	    				{
        	      					myLabel = myLower;
        	    				}
        	    				QString myUpper = mypSymbol->upperValue();
        	    				if ( !myUpper.isEmpty() )
        	    				{
        	      					myLabel += " - ";
        	      					myLabel += myUpper;
        	    				}
        	    				QString myText = mypSymbol->label();
        	    				if ( !myText.isEmpty() )
        	    				{
        	      					myLabel += " ";
        	      					myLabel += myText;
        	    				}
        	    				myLabel = myLabel.trimmed();
        	    				myLegendXPos += myIconWidth + myLegendSpacer;
        	    				myPainter->setPen( Qt::black );
	
        	    				QStringList myWrappedLayerNameList 
								= wordWrap( myLabel, myLegendFontMetrics,
        	                                         		myLegendDimensionX - myLegendXPos );
        	    				//
        	    				// Loop through wrapped legend label lines
        	    				//
        	    				QStringListIterator myLineWrapIterator( myWrappedLayerNameList );
        	    				while ( myLineWrapIterator.hasNext() )
        	    				{
        	      					QString myLine = myLineWrapIterator.next();
        	      					// check if the text will overflow the space we have
        	      					QRect myLegendItemRect( myLegendXPos, myLegendYPos,
        	                              					myLegendDimensionX - myIconWidth,
        	                              					myLegendFontHeight );
        	      					myPainter->drawText( myLegendItemRect, Qt::AlignLeft, myLine );
        	     					myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
        	    				} //wordwrap loop
          				} //symbol loop
        			} //class breaks
      			} //if vectorlayer
    		} //if maplayer
  	} //layer iterator
	myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
	
	// Drawing the Map Legend... for the Qrap rasters
	if (myLegendYPos >= myLegendDimensionY)
	{
		myPrinter->newPage();
		myLegendYPos = 0;
	}

//	bool Server =((plotTypeCombo->currentText() == "Primary Server") ||
//			(plotTypeCombo->currentText() == "Secondary Server")	||
//			(plotTypeCombo->currentText() == "Primary Co-Channel Interferers") ||
//			(plotTypeCombo->currentText() == "Primary Adjacent-Channel Interferers"));


	bool Server = 	((mPlotType == PrimServer) || (mPlotType == SecondServer) || 
			(mPlotType == PrimIntCo) || (mPlotType == PrimIntAd));

	string query = "SELECT * FROM colourmanagement WHERE plottype ='";
	query +=plotTypeCombo->currentText().toStdString();
	query +="';";
	cout <<query << endl;
	if (!gDb.PerformRawSql(query))
	{
		string err = "Database Select on colourmanagement failed";
		QMessageBox::information(this, "QRap", "Database Select on colourmanagement failed");
//		QRAP_WARN(err.c_str());
		cout << err << endl;	
	}
	else
	{
		pqxx::result res;
		gDb.GetLastResult(res);
		int r,g,b,a;	
		if (!Server)
		{
			cout << " In cConfirmPrediction::on_pushButtonPrint_clicked(): Is NOT server " << endl;
			for (int i = 0; i < res.size() ; i++)
			{
				cout << " In cConfirmPrediction::on_pushButtonPrint_clicked(): for kleure: i = "<< i << endl;
				if (myLegendYPos > myLegendDimensionY)
				{
					myPrinter->newPage();
					myLegendYPos = 0;
				}
				int r,g,b,a;
				r = (int)atof(res[i]["r"].c_str());
				g = (int)atof(res[i]["g"].c_str());
				b = (int)atof(res[i]["b"].c_str());
				a = (int)atof(res[i]["a"].c_str());
				QColor Colour = QColor(r,g,b,a);
				myPainter->fillRect(myLegendXPos - myIconWidth -myLegendSpacer ,
							myLegendYPos-(myLegendFontHeight+myLegendVerticalSpacer/2),
							myLegendXPos, myLegendFontHeight,QBrush(Colour));
				myPainter->drawText( myLegendXPos + myIconWidth + myLegendSpacer, 
							myLegendYPos, QString("%1").arg(res[i]["val"].c_str()));
				myPainter->drawText( myLegendXPos + myLegendSpacer + 10*myLegendFontWidth,
							myLegendYPos, QString("%1").arg(res[i]["label"].c_str()));
				myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
			}//for i to res.size
		}//server
		else
		{
			if (res.size() > 0) 
			{
				if (myLegendYPos >= myLegendDimensionY)
				{
					myPrinter->newPage();
					myLegendYPos = 0;
				}

				int j = 0;
				for (int i = 0; i < tableWidget->rowCount();i++)
				{
					void *Wid = tableWidget->cellWidget(i,0);
					QCheckBox *get =(QCheckBox*) Wid;
					if (get->isChecked() &&	(j < res.size()))
					{
						if (myLegendYPos > myLegendDimensionY)
						{
							myPrinter->newPage();
							myLegendYPos = 0;
							myPainter->drawText( myLegendXPos, myLegendYPos, QString("Colour"));
							myPainter->drawText( myLegendXPos + myIconWidth + myLegendSpacer, myLegendYPos, 
										QString("Radio Installation"));
							myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
						}
						int r,g,b,a;
						r = (int)atof(res[j]["r"].c_str());
						g = (int)atof(res[j]["g"].c_str());
						b = (int)atof(res[j]["b"].c_str());
						a = (int)atof(res[j]["a"].c_str());
						QColor Colour = QColor(r,g,b,a);
						QString It =  tableWidget->item(i,2)->text();
						myPainter->fillRect(myLegendXPos - myIconWidth -myLegendSpacer ,myLegendYPos-myLegendFontHeight/2,
							myLegendXPos, myLegendFontHeight,QBrush(Colour));
						myPainter->drawText( myLegendXPos + myIconWidth + myLegendSpacer, 
									myLegendYPos, QString("%1: %2").arg(get->text()).arg(It));
						myLegendYPos += myLegendVerticalSpacer + myLegendFontHeight;
						j++;
						if (j==res.size()) j=0;
					}// if get checked
				} //for i to rowCount
			}//if res.size
		} // else not server
	}// else query worked

	//
  	// Draw the scale bar
  	//
  	int myYpos = myLegendDimensionY - myLegendVerticalSpacer - myLegendFontHeight;
	int myLogoXDim = myLegendDimensionX - 8*myLegendFontWidth ;
  	myPainter->setViewport( myPrinter->pageRect().right() - myLogoXDim - myLegendXPos, myYpos, 
				myPrinter->pageRect().right(),  myLegendDimensionY );
  	renderPrintScaleBar( myPainter, mMapRenderer,  myLogoXDim);
  	myPainter->setViewport( myOriginalViewport );

	// Print plot info
//	bool CoInterf = (plotTypeCombo->currentText()=="Co-channel Interference")||
//			(plotTypeCombo->currentText()=="Primary Co-channel interferer");
//	bool AdInterf = (plotTypeCombo->currentText()=="Adjacent-channel Interference")||
//			(plotTypeCombo->currentText()=="Primary adjacent-channel interferer");
//	bool Interf = (plotTypeCombo->currentText()=="Interfered areas")||CoInterf||
//			(plotTypeCombo->currentText()=="Number of Interferers")||AdInterf||
//			(plotTypeCombo->currentText()=="Energy per Bit to Noise Power");
//	bool Noise = (plotTypeCombo->currentText()=="Energy per Bit to Noise Power")||
//			(plotTypeCombo->currentText()=="Signal to Noise Ratio")||
//			(plotTypeCombo->currentText()=="Service Limiters");
	

	bool CoInterf = (mPlotType==IntRatioCo)|| (mPlotType==PrimIntCo);
	bool AdInterf = (mPlotType==IntRatioAd)||(mPlotType==PrimIntAd);
	bool Interf = (mPlotType==IntAreas)||CoInterf||(mPlotType==NumInt)||AdInterf||(mPlotType==EbNo);
	bool Noise = (mPlotType==EbNo)||(mPlotType==SN)||(mPlotType==ServiceLimits);

	myYpos = max(myPrinter->pageRect().bottom()- myPlotInfoPercent*myDrawableHeight/100, myLegendYPos);
	myPainter->drawLine(myPrinter->pageRect().left(),myYpos, myPrinter->pageRect().right()-myPrinter->pageRect().left(), myYpos);
   	myYpos += myLegendVerticalSpacer + myLegendFontHeight;
	int myOriginXLeft = myPrinter->pageRect().left() + myHorizontalSpacing*2;
	int myOriginXMid = myPrinter->pageRect().left() + myDrawableWidth/3 + myHorizontalSpacing*2;
	int myOriginXRight = myPrinter->pageRect().left() + 2*myDrawableWidth/3 + myHorizontalSpacing*2;
	myPainter->drawText( myOriginXMid, myYpos, QString("Plot Resolution (m): %1").arg(plotResolutionSpinBox->value()));
	
	if (mPlotType != DEM)
	{
		myPainter->drawText( myOriginXRight, myYpos, QString("Plot Units: %1").arg(displayUnitsCombo->currentText()));
		if (downLinkRadio->isChecked())
			myPainter->drawText( myOriginXLeft, myYpos, QString("Direction:  Downlink"));
		else	myPainter->drawText( myOriginXLeft, myYpos, QString("Direction:  Uplink"));
		myYpos+= myLegendVerticalSpacer + myLegendFontHeight;

		myPainter->drawText( myOriginXMid, myYpos, QString("Minimum Angle Resolution: %1").arg(minAngleResSpinBox->value()));
		myPainter->drawText( myOriginXLeft, myYpos, QString("Mobile Radio: %1").arg(mobileCombo->currentText()));
		myPainter->drawText( myOriginXRight, myYpos, QString("Minimum Required Receiver Signal: %1").arg(rxMinSpinBox->value()));
		myYpos+= myLegendVerticalSpacer + myLegendFontHeight;

		myPainter->drawText( myOriginXRight, myYpos, QString("k-factor for Servers: %1").arg(kFactorServerSpinBox->value()));
		myPainter->drawText( myOriginXMid, myYpos, QString("Fade Margin: %1").arg(fadeMarginSpin->value()));
		if (Interf)
		{
			myYpos+= myLegendVerticalSpacer + myLegendFontHeight;
			myPainter->drawText( myOriginXRight, myYpos, 
					QString("k-factor for Interferers: %1").arg(kFactorIntSpinBox->value()));
			if (!AdInterf)
				myPainter->drawText( myOriginXLeft, myYpos, 
					QString("Required Co Channel to interferer: %1").arg(rqCociSpinBox->value()));
			if (!CoInterf)
				myPainter->drawText( myOriginXMid, myYpos, 
					QString("Required Adjacent Channel to interferer: %1").arg(rqCiadSpinBox->value()));			
			myYpos+= myLegendVerticalSpacer + myLegendFontHeight;
		}
		if (Noise)
		{
			myPainter->drawText( myOriginXMid, myYpos, QString("Required Signal to Noise Ratio: %1").arg(RqSNSpinBox->value()));
			myPainter->drawText( myOriginXRight, myYpos, QString("Noise Level: %1").arg(rxMinSpinBox->value()));
			myPainter->drawText( myOriginXLeft, myYpos, QString("Energy per Bit to Noise Power: %1").arg(rqEbNoSpinBox->value()));
			myYpos+= myLegendVerticalSpacer + myLegendFontHeight;
		}
		
		// Provide Site info
		myYpos+= (myLegendVerticalSpacer + myLegendFontHeight);
		int ListSiteY = myYpos;
		myPainter->drawLine(myPrinter->pageRect().left(),myYpos+myLegendVerticalSpacer, 
					myPrinter->pageRect().right()-myPrinter->pageRect().left(), myYpos+myLegendVerticalSpacer);
		int NumCols = (int)(myDrawableWidth/(75*myLegendFontWidth));
		int iReqColWidth = myDrawableWidth/NumCols;
		bool nextCol = (iReqColWidth*2)<=myDrawableWidth;
		int iSite = myOriginXLeft;
		int iSector = iSite+ 25*myLegendFontWidth;
		int iRadInst = iSector + 10*myLegendFontWidth;
		int iStatus = iRadInst + 13*myLegendFontWidth;  
		int iRadius = iStatus + 16*myLegendFontWidth;
		myPainter->drawText( iSite, myYpos, QString("Site"));
		myPainter->drawText( iSector, myYpos, QString("Sector"));
		myPainter->drawText( iRadInst, myYpos, QString("RadInstID"));
		myPainter->drawText( iStatus, myYpos, QString("Status"));
		myPainter->drawText( iRadius, myYpos, QString("Radius"));

		myYpos += myLegendVerticalSpacer + myLegendFontHeight;
//		myPainter->drawLine(myPrinter->pageRect().left(),myYpos, myPrinter->pageRect().right()-myPrinter->pageRect().left(), myYpos);
		myYpos += myLegendVerticalSpacer;

		void *Wid; 
		bool ok;
		double Rad;
		QCheckBox *get;
		QString sRange, sSite, sSector, sRadInst, sStatus;
		for (int i = 0; i < tableWidget->rowCount();i++)
		{
			Wid = tableWidget->cellWidget(i,0);
			get =(QCheckBox*) Wid;
			if (get->isChecked())
			{
				if (myYpos >= myPrinter->pageRect().height())
				{
					if (nextCol)
					{
						nextCol = false;
						myYpos = ListSiteY;
						iSite += iReqColWidth;
						iSector += iReqColWidth;;
						iRadInst += iReqColWidth;;
						iStatus += iReqColWidth;;  
						iRadius += iReqColWidth;;
					}
					else
					{			
						myPrinter->newPage();
						myYpos = myLegendVerticalSpacer +myPrinter->pageRect().top();
						iSite = myOriginXLeft;
						iSector = iSite+ 25*myLegendFontWidth;
						iRadInst = iSector + 10*myLegendFontWidth;
						iStatus = iRadInst + 13*myLegendFontWidth;  
						iRadius = iStatus + 16*myLegendFontWidth;
						myPainter->drawLine(myPrinter->pageRect().left(), myYpos+myLegendVerticalSpacer, 
									myPrinter->pageRect().right()-myPrinter->pageRect().left(), 
									myYpos+myLegendVerticalSpacer);
					}
					myPainter->drawText( iSite, myYpos, QString("Site"));
					myPainter->drawText( iSector, myYpos, QString("Sector"));
					myPainter->drawText( iRadInst, myYpos, QString("RadInstID"));
					myPainter->drawText( iStatus, myYpos, QString("Status"));
					myPainter->drawText( iRadius, myYpos, QString("Radius"));
					myYpos += myLegendVerticalSpacer + myLegendFontHeight;
				} // if end of page
				
				if (tableWidget->item(i,7)!=NULL)
					Rad =tableWidget->item(i,7)->text().toDouble(&ok);
				else	Rad =0;
					
				if (tableWidget->item(i,5) != NULL)
					sRadInst = tableWidget->item(i,5)->text();
				else	sRadInst = "";
				
				sSite = get->text();

				if (tableWidget->item(i,1) != NULL)
					sSector = tableWidget->item(i,1)->text();
				else	sSector = "";

				if (tableWidget->item(i,8) != NULL)
					sStatus = tableWidget->item(i,6)->text();
				else	sStatus = "";
				
				if (tableWidget->item(i,2) != NULL)
					sRange = tableWidget->item(i,2)->text();
				else	sRange = "";
					
				myPainter->drawText( iSite, myYpos, sSite);
				myPainter->drawText( iSector, myYpos, sSector);
				myPainter->drawText( iRadInst, myYpos, sRadInst);
				myPainter->drawText( iStatus, myYpos, sStatus);
				myPainter->drawText( iRadius, myYpos, sRange);
				myYpos += myLegendVerticalSpacer + myLegendFontHeight;

			} // is checked
		} // for i
	} // end if not DEM	
 	
	//
  	// Restore the map render to its former glory
  	//
  	mMapRenderer->setOutputSize( myOriginalSize, myOriginalDpi );
 	myPainter->end();
	// Cleaning up
	delete myPrinter;
   	delete myPainter;
*/

}

//*********************************************************************************
// Copied from qgsquickprint.cpp, Tim Sutton 2008
void cConfirmPrediction::renderPrintScaleBar( QPainter * thepPainter,
    QgsMapRenderer * thepMapRenderer,
    int theMaximumWidth )

{
  //hard coding some options for now
  bool mySnappingFlag = true;
  QColor mColour = Qt::black;
  // Hard coded sizes
  int myTextOffsetX = 0;
  int myTextOffsetY = 5;
  int myXMargin = 20;
  int myYMargin = 20;
  int myPreferredSize = theMaximumWidth - ( myXMargin * 2 );
  double myActualSize = 0;
  int myBufferSize = 1; //softcode this later
  QColor myBackColor = Qt::white; //used for text
  QColor myForeColor = Qt::black; //used for text

  //Get canvas dimensions
  //int myCanvasHeight = thepMapCanvas->height();

  //Get map units per pixel. This can be negative at times (to do with
  //projections) and that just confuses the rest of the code in this
  //function, so force to a positive number.
  double myMapUnitsPerPixelDouble = fabs( thepMapRenderer->mapUnitsPerPixel() );
  //
  // Exit if the canvas width is 0 or layercount is 0 or QGIS will freeze
  int myLayerCount = thepMapRenderer->layerSet().count();
  if ( !myLayerCount || !myMapUnitsPerPixelDouble ) return;

  //Calculate size of scale bar for preferred number of map units
  double myScaleBarWidth = myPreferredSize;
  myActualSize = myScaleBarWidth * myMapUnitsPerPixelDouble;


  // Work out the exponent for the number - e.g, 1234 will give 3,
  // and .001234 will give -3
  double myPowerOf10 = floor( log10( myActualSize ) );

  // snap to integer < 10 times power of 10
  if ( mySnappingFlag )
  {
    double scaler = pow( 10.0, myPowerOf10 );
    myActualSize = round( myActualSize / scaler ) * scaler;
    myScaleBarWidth = myActualSize / myMapUnitsPerPixelDouble;
  }

  //Get type of map units and set scale bar unit label text
  QGis::UnitType myMapUnits = thepMapRenderer->mapUnits();
  QString myScaleBarUnitLabel;
  switch ( myMapUnits )
  {
    case QGis::Meters:
      if ( myActualSize > 1000.0 )
      {
        myScaleBarUnitLabel = tr( " km" );
        myActualSize = myActualSize / 1000;
      }
      else if ( myActualSize < 0.01 )
      {
        myScaleBarUnitLabel = tr( " mm" );
        myActualSize = myActualSize * 1000;
      }
      else if ( myActualSize < 0.1 )
      {
        myScaleBarUnitLabel = tr( " cm" );
        myActualSize = myActualSize * 100;
      }
      else
        myScaleBarUnitLabel = tr( " m" );
      break;
    case QGis::Feet:
      if ( myActualSize > 5280.0 ) //5280 feet to the mile
      {
        myScaleBarUnitLabel = tr( " miles" );
        myActualSize = myActualSize / 5280;
      }
      else if ( myActualSize == 5280.0 ) //5280 feet to the mile
      {
        myScaleBarUnitLabel = tr( " mile" );
        myActualSize = myActualSize / 5280;
      }
      else if ( myActualSize < 1 )
      {
        myScaleBarUnitLabel = tr( " inches" );
        myActualSize = myActualSize * 12;
      }
      else if ( myActualSize == 1.0 )
      {
        myScaleBarUnitLabel = tr( " foot" );
      }
      else
      {
        myScaleBarUnitLabel = tr( " feet" );
      }
      break;
    case QGis::Degrees:
      if ( myActualSize == 1.0 )
        myScaleBarUnitLabel = tr( " degree" );
      else
        myScaleBarUnitLabel = tr( " degrees" );
      break;
    case QGis::UnknownUnit:
      myScaleBarUnitLabel = tr( " unknown" );
    default:
      QgsDebugMsg( "Error: not picked up map units - actual value = "
                   + QString::number( myMapUnits ) );
  };

  //Set font and calculate width of unit label
  int myFontSize = 10; //we use this later for buffering
  QFont myFont( "helvetica", myFontSize );
  thepPainter->setFont( myFont );
  QFontMetrics myFontMetrics( myFont );
  double myFontWidth = myFontMetrics.width( myScaleBarUnitLabel );
  double myFontHeight = myFontMetrics.height();

  //Set the maximum label
  QString myScaleBarMaxLabel = QString::number( myActualSize );

  //Calculate total width of scale bar and label
  //we divide by 2 because the max scale label
  //will be centered over the endpoint of the scale bar
  double myTotalScaleBarWidth = myScaleBarWidth + ( myFontWidth / 2 );

  //determine the origin of scale bar (bottom right)
  //for x origin set things up so the scalebar is centered
  int myOriginX = ( theMaximumWidth - myTotalScaleBarWidth ) / 2;
  int myOriginY = myYMargin;

  //Set pen to draw with
  QPen myForegroundPen( mColour, 2 );
  QPen myBackgroundPen( Qt::white, 3 );

  //Cast myScaleBarWidth to int for drawing
  int myScaleBarWidthInt = ( int ) myScaleBarWidth;

  //now draw the bar itself in user selected color
  thepPainter->setPen( myForegroundPen );
  //make a glossygradient for the background
  QGradientStops myStops;
  myStops << QGradientStop( 0.0, QColor( "#616161" ) );
  myStops << QGradientStop( 0.5, QColor( "#505050" ) );
  myStops << QGradientStop( 0.6, QColor( "#434343" ) );
  myStops << QGradientStop( 1.0, QColor( "#656565" ) );
  //draw again with the brush in the revers direction to complete teh glossiness
  QLinearGradient myReverseGlossyBrush(
    QPointF( myOriginX, myOriginY +  myFontHeight*3 ),
    QPointF( myOriginX, myOriginY ) );
  thepPainter->setBrush( myReverseGlossyBrush );
  thepPainter->drawRect(
    myOriginX,
    myOriginY,
    myOriginX + myScaleBarWidthInt,
    myOriginY + myFontHeight
  );

  //
  //Do drawing of scale bar text
  //


  //Draw the minimum label buffer
  thepPainter->setPen( myBackColor );
  myFontWidth = myFontMetrics.width( "0" );

  for ( int i = 0 - myBufferSize; i <= myBufferSize; i++ )
  {
    for ( int j = 0 - myBufferSize; j <= myBufferSize; j++ )
    {
      thepPainter->drawText( int( i + ( myOriginX - ( myFontWidth / 2 ) ) ),
                             int( j + ( myOriginY - ( myFontHeight / 4 ) ) ) - myTextOffsetY,
                             "0" );
    }
  }

  //Draw minimum label
  thepPainter->setPen( myForeColor );

  thepPainter->drawText(
    int( myOriginX - ( myFontWidth / 2 ) ),
    int( myOriginY - ( myFontHeight / 4 ) ) - myTextOffsetY,
    "0"
  );

  //
  //Draw maximum label
  //
  thepPainter->setPen( myBackColor );
  myFontWidth = myFontMetrics.width( myScaleBarMaxLabel );
  myFontHeight = myFontMetrics.height();
  //first the buffer
  for ( int i = 0 - myBufferSize; i <= myBufferSize; i++ )
  {
    for ( int j = 0 - myBufferSize; j <= myBufferSize; j++ )
    {
      thepPainter->drawText( int( i + ( myOriginX + myScaleBarWidthInt - ( myFontWidth / 2 ) ) ),
                             int( j + ( myOriginY - ( myFontHeight / 4 ) ) ) - myTextOffsetY,
                             myScaleBarMaxLabel );
    }
  }
  //then the text itself
  thepPainter->setPen( myForeColor );
  thepPainter->drawText(
    int( myOriginX + myScaleBarWidthInt - ( myFontWidth / 2 ) ),
    int( myOriginY - ( myFontHeight / 4 ) ) - myTextOffsetY,
    myScaleBarMaxLabel
  );

  //
  //Draw unit label
  //
  thepPainter->setPen( myBackColor );
  myFontWidth = myFontMetrics.width( myScaleBarUnitLabel );
  //first the buffer
  for ( int i = 0 - myBufferSize; i <= myBufferSize; i++ )
  {
    for ( int j = 0 - myBufferSize; j <= myBufferSize; j++ )
    {
      thepPainter->drawText( i + ( myOriginX + myScaleBarWidthInt + myTextOffsetX ),
                             j + myOriginY + myFontHeight + ( myFontHeight*2.5 ) + myTextOffsetY,
                             myScaleBarUnitLabel );
    }
  }
  //then the text itself
  thepPainter->setPen( myForeColor );
  thepPainter->drawText(
    myOriginX + myScaleBarWidthInt + myTextOffsetX,
    myOriginY + myFontHeight + ( myFontHeight*2.5 ) +  myTextOffsetY,
    myScaleBarUnitLabel
  );
}


//************************************************************************************************
// Copied from qgsquickprint.cpp, Tim Sutton 2008
QStringList cConfirmPrediction::wordWrap( QString theString,
                                     QFontMetrics theMetrics,
                                     int theWidth )
{
  //iterate the string
  QStringList myList;
  QString myCumulativeLine = "";
  QString myStringToPreviousSpace = "";
  int myPreviousSpacePos = 0;
  for ( int i = 0; i < theString.count(); ++i )
  {
    QChar myChar = theString.at( i );
    if ( myChar == QChar( ' ' ) )
    {
      myStringToPreviousSpace = myCumulativeLine;
      myPreviousSpacePos = i;
    }
    myCumulativeLine += myChar;
    if ( theMetrics.width( myCumulativeLine ) >= theWidth )
    {
      //time to wrap
      //@todo deal with long strings that have no spaces
      //forcing a break at current pos...
      myList << myStringToPreviousSpace.trimmed();
      i = myPreviousSpacePos;
      myStringToPreviousSpace = "";
      myCumulativeLine = "";
    }
  }//end of i loop
  //add whatever is left in the string to the list
  if ( !myCumulativeLine.trimmed().isEmpty() )
  {
    myList << myCumulativeLine.trimmed();
  }

  //qDebug("Wrapped legend entry: %s\n%s", theString, myList.join("\n").toLocal8Bit().constData() );
  return myList;
}


