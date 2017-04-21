 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2016/04/01
 *    License     : GNU GPLv3
 *    File        : qrap.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This is the main QRap plugin for QGIS. QRap is Radio
 *                  Planning Tool. (www.QRap.org.za)
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

#include "qrap.h"
//
#include <qgisinterface.h>
#include <qgisgui.h>

// Qt4 Related Includes
#include <QAction>
#include <QToolBar>
#include <QMessageBox>

//////////////////////////////////////////////////////////////////////
//
// THE FOLLOWING METHODS ARE MANDATORY FOR ALL PLUGINS
//
//////////////////////////////////////////////////////////////////////

//**********************************************************************
/**
 * Constructor for the plugin. The plugin is passed a pointer 
 * an interface object that provides access to exposed functions in QGIS.
 * @param theQGisInterface - Pointer to the QGIS interface object
 */

static const QString sPluginIcon = ":/qrap/Coverage.png";
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;
static const QString sCategory = QObject::tr( "Database" );
static const QString sName = QObject::tr("Q-Rap");
static const QString sDescription = QObject::tr("Radio Systems Planning Tool");
static const QString sPluginVersion = QObject::tr("Version 0.1");


//*************************************************************************
 QRap::QRap(QgisInterface * theQgisInterface):
    QgisPlugin( sName, sDescription, sCategory, sPluginVersion, sPluginType ),
//		qgisMainWindow(theQgisInterface->mainWindow() ),
		mQGisIface (theQgisInterface)
{

}

//************************************************************************
QRap::~QRap()
{
}

//
//*****************************************************************************
/*
 * Initialize the GUI interface for the plugin - this is only called once when the plugin is 
 * added to the plugin registry in the QGIS application.
 */
void QRap::initGui()
{
	mToolBarPointer = 0;
	printf("QRap::initGui\n");
	mPoints.clear();
	mMouseType = CLEAN;
	// Create the action for tool

	cout << "Voor new Actions" << endl;
	mQActionPointer = new QAction(QIcon(":/qrap/Data.png"),tr("Q-Rap Database Interface"), this);
	mSiteAction = new QAction(QIcon(":/qrap/Site.png"),tr("Q-Rap: Place a Site"), this);
    	mSelectSiteAction = new QAction(QIcon(":/qrap/SiteSelect.png"),tr("Q-Rap: Select a Site"), this);
    	mDeleteSiteAction = new QAction(QIcon(":/qrap/SiteDelete.png"),tr("Q-Rap: Delete a Site"), this);
	mLinkAction = new QAction(QIcon(":/qrap/Link.png"),tr("Q-Rap: Link Analysis"), this);
	mSelectLinkAction = new QAction(QIcon(":/qrap/LinkSelect.png"),tr("Q-Rap: Select a Link"), this);
	mDeleteLinkAction = new QAction(QIcon(":/qrap/LinkDelete.png"),tr("Q-Rap: Delete a Link"), this);
	mMultiLinkAction = new QAction(QIcon(":/qrap/MultiLink.png"),tr("Q-Rap: Establish all Links possible in set of Sites"), this);
	mRadioAction = new QAction(QIcon(":/qrap/Coverage.png"),tr("Q-Rap: Perform a Prediction"), this);
	mMeasAnalysisAction = new QAction(QIcon(":/qrap/Measurements.png"),tr("Q-Rap: Compare measurements with predictions"), this);
    	mSpectralAction = new QAction(QIcon(":/qrap/Spectral.png"),tr("Q-Rap: Perform Spectral Interference Analysis"), this);
    	mPreferencesAction = new QAction(QIcon(":/qrap/Preferences.png"),tr("Q-Rap Preferences"), this);
	mOptimisationAction = new QAction(QIcon(":/qrap/Optimisation.png"),tr("Q-Rap: Optimise link structure in selected area"), this);
//    	mImportExportAction = new QAction(QIcon(":/qrap/ImportExport.png"),tr("Import Export"),this);
//    	mHelpAction = new QAction(QIcon(":/qrap/Help.png"),tr("Q-Rap Help"), this);

     	cout << "Na new Actions" << endl;
  
	// Connect the action to the run
  	connect(mQActionPointer, SIGNAL(activated()), this, SLOT(run()));
  	connect(mSiteAction, SIGNAL(activated()), this, SLOT(PlaceSite()));
  	connect(mSelectSiteAction, SIGNAL(activated()), this, SLOT(SelectSite()));
  	connect(mDeleteSiteAction, SIGNAL(activated()), this, SLOT(DeleteSite()));
  	connect(mLinkAction, SIGNAL(activated()), this, SLOT(CreateLinkAnalysis()));
  	connect(mDeleteLinkAction, SIGNAL(activated()), this, SLOT(DeleteLink()));
	connect(mMultiLinkAction, SIGNAL(activated()), this, SLOT(MultiLink()));
  	connect(mSelectLinkAction, SIGNAL(activated()), this, SLOT(SelectLink()));
  	connect(mRadioAction, SIGNAL(activated()), this, SLOT(Prediction()));
	connect(mMeasAnalysisAction, SIGNAL(activated()), this, SLOT(Measurements()));
  	connect(mSpectralAction, SIGNAL(activated()), this, SLOT(SpectralAnalysis()));
  	connect(mPreferencesAction, SIGNAL(activated()), this, SLOT(Preferences()));
	connect(mOptimisationAction, SIGNAL(activated()), this, SLOT(Optimise()));
//  	connect(mImportExportAction,SIGNAL(activated()), this, SLOT(ImportExport()));
//  	connect(mHelpAction,SIGNAL(activated()), this, SLOT(Help()));
  	cout << "Na Connect" << endl;

  	// Add the toolbar to the main window
  	mToolBarPointer = mQGisIface->addToolBar(tr("Q-Rap")); 
	mQGisIface->addToolBar(mToolBarPointer);
  	mToolBarPointer->setIconSize(QSize(24,24));
  	mToolBarPointer->setObjectName("Q-Rap");
  	// Add the icon to the toolbar

  	mToolBarPointer->addAction(mSiteAction);
  	mToolBarPointer->addAction(mSelectSiteAction);
  	mToolBarPointer->addAction(mDeleteSiteAction);
  	mToolBarPointer->addAction(mLinkAction);
  	mToolBarPointer->addAction(mSelectLinkAction);
  	mToolBarPointer->addAction(mDeleteLinkAction);
  	mToolBarPointer->addAction(mRadioAction);
	mToolBarPointer->addAction(mMultiLinkAction);
	mToolBarPointer->addAction(mMeasAnalysisAction);
  	mToolBarPointer->addAction(mSpectralAction);
        mToolBarPointer->addAction(mOptimisationAction);
  	mToolBarPointer->addAction(mPreferencesAction);
  	mToolBarPointer->addAction(mQActionPointer);
//  	mToolBarPointer->addAction(mImportExportAction);
//  	mToolBarPointer->addAction(mHelpAction); 

	mLoaded = true; 
 
  	cout << "VOOR DataBase Connect" << endl;
  	openDatabaseConnection();
  	cout << "Na DataBase Connect" << endl;

//  	mQGisIface->addPluginToMenu( tr( "Q-Rap Database" ), mQActionPointer );

  	Mouse = new MouseEvents(mQGisIface->mapCanvas());
   mMapToolEmitPoint = new QgsMapToolEmitPoint(mQGisIface->mapCanvas());
  	cout << "Na Mouse" << endl;
  	connect(Mouse, SIGNAL(RightPoint(QgsPoint&)), this, SLOT(ReceivedRightPoint(QgsPoint&)));
  	connect(Mouse, SIGNAL(LeftPoint(QgsPoint&)), this, SLOT(ReceivedLeftPoint(QgsPoint&)));
//	connect(Mouse, SIGNAL(canvasClicked(QgsPoint&, Qt::MouseButton)), this, SLOT(ReceivedLeftPoint(QgsPoint&)));
}

//****************************************************************************************
//method defined in interface
void QRap::help()
{
  	
}

//****************************************************************************************
// Unload the plugin by cleaning up the GUI
void QRap::unload()
{
  	if (mLoaded)
  	{
	  	// remove the GUI  
	
  		mToolBarPointer->removeAction(mSiteAction);
		mToolBarPointer->removeAction(mSelectSiteAction);
		mToolBarPointer->removeAction(mDeleteSiteAction);
  		mToolBarPointer->removeAction(mLinkAction);
		mToolBarPointer->removeAction(mSelectLinkAction);
		mToolBarPointer->removeAction(mDeleteLinkAction);
	  	mToolBarPointer->removeAction(mRadioAction);
		mToolBarPointer->removeAction(mMultiLinkAction);
		mToolBarPointer->removeAction(mOptimisationAction);
	  	mToolBarPointer->removeAction(mSpectralAction);
		mToolBarPointer->removeAction(mPreferencesAction);
		mToolBarPointer->removeAction(mMeasAnalysisAction);
		mToolBarPointer->removeAction(mQActionPointer);

	  	delete mQActionPointer;
	  	delete mSelectSiteAction;
		delete mDeleteSiteAction;
		delete mSiteAction;
	  	delete mRadioAction;
	  	delete mLinkAction;
		delete mSelectLinkAction;
		delete mDeleteLinkAction;
	  	delete mSpectralAction;
		delete mMultiLinkAction;
	  	delete mPreferencesAction;
		delete mMeasAnalysisAction;
		
		delete mToolBarPointer;

	//	disconnect(Mouse);
	//	delete Mouse;
//		mQGisIface->removeToolBar(mToolBarPointer);
		mLoaded =false;

	}
	cout << "QRap::unload(): leaving" << endl;
}


//////////////////////////////////////////////////////////////////////////
//
//
//  THE FOLLOWING CODE IS AUTOGENERATED BY THE PLUGIN BUILDER SCRIPT
//    YOU WOULD NORMALLY NOT NEED TO MODIFY THIS, AND YOUR PLUGIN
//      MAY NOT WORK PROPERLY IF YOU MODIFY THIS INCORRECTLY
//
//
//////////////////////////////////////////////////////////////////////////

//************************************************************************
/** 
 * Required extern functions needed  for every plugin 
 * These functions can be called prior to creating an instance
 * of the plugin class
 */
// Class factory to return a new instance of the plugin class
QGISEXTERN QgisPlugin * classFactory(QgisInterface * theQgisInterfacePointer)
{
  	return new QRap(theQgisInterfacePointer);
}

//******************************************************************************
// Return the name of the plugin - note that we do not user class members as
// the class may not yet be insantiated when this method is called.
QGISEXTERN QString name()
{
	return sName;
}

//*******************************************************************************
// Return the description
QGISEXTERN QString description()
{
	return sDescription;
}

//********************************************************************************
// Return the type (either UI or MapLayer plugin)
QGISEXTERN int type()
{
	return sPluginType;
}

//********************************************************************************
// Return the version number for the plugin
QGISEXTERN QString version()
{
	return sPluginVersion;
}

//********************************************************************************
// Return the Icon for the plugin
QGISEXTERN QString icon()
{
  return sPluginIcon;
}

//********************************************************************************
// Return the category
QGISEXTERN QString category()
{
  return sCategory;
}

//*******************************************************************************
// Delete ourself
QGISEXTERN void unload( QgisPlugin * thePluginPointer )
{
  	delete thePluginPointer;
}

//*****************************************************************************
bool QRap::openDatabaseConnection()
{
	cout << "In QRap::openDatabaseConnection() " << endl;
	
	/** \TODO: Get a place for settings.xml and structure.xml or find a way to set the location
	 */

	if (!gSettings.LoadFromFile("/usr/lib/qgis/plugins/settings.xml"))
	{
		cout << "Error while loading settings from settings.xml" << endl;
		exit(1);
	}
	
	cout << "Na Load Settings file in QRap::openDatabaseConnection() " << endl;
	LoginDialog* loginDialog;
	
	loginDialog = new LoginDialog(0);
	
	//********************************* Checking the default settings**/
	if (loginDialog->exec()==0)
	{
		// get the values from the login dialog
//		dbusername = loginDialog->getUserName();
//		dbpassword = loginDialog->getPassword();
//		gDb.Connect (dbusername, dbpassword,);
		//Why was this happening twice ... probably part of Christo's junk ... 
		
		//***********************************************************************************	
		/*Christo du Plessis se junk 	
  		cout << "Voor new IdResolve in QRap::openDatabaseConnection() " << endl;
		// resolve the host id using id resolve
		IdResolve idr;
		int ret,id;
		cout << "Voor connectDB " << endl;
		// connect to the local database
		ret = idr.connectDB("localhost","Q-Rap", dbusername.c_str(),dbpassword.c_str());
		if (!ret) {
			QMessageBox::information(NULL,"Q-Rap","Failed to connect to the local database");
		}

		// retrieve the hostname
		cout << "Voor getHostId " << endl;
		id = idr.getHostId();
		if (id<0) {
			QMessageBox::information(NULL,"APP","No Machine Id registered with server. Register a machine id before inserting sites");
		}
		
		// set the machine id
		gDb.globalMachineID = id;

		// set the machine name (a string version of the same ID)
		char buf[1024];
		sprintf(buf,"%d",id);
		machinename = string(buf);

		printf("machinename: [%s]\n",machinename.c_str());

		//einde van Christo se junk
		*/

		if (gDb.Connected())
		{
			if (gDb.GetSetting("Units") == "") 		gDb.SetSetting("Units","dBm");
			if (gDb.GetSetting("RqSN") == "") 		gDb.SetSetting("RqSN","8");
			if (gDb.GetSetting("RxMin") == "") 		gDb.SetSetting("RxMin","-110");
			if (gDb.GetSetting("FadeMargin") == "") 	gDb.SetSetting("FadeMargin","3");
			if (gDb.GetSetting("RqClco") == "")		gDb.SetSetting("RqClco","9");
			if (gDb.GetSetting("RqClad") == "")		gDb.SetSetting("RqClad","-9");
			if (gDb.GetSetting("RqEbNo") == "")		gDb.SetSetting("RqEbNo","8");
			if (gDb.GetSetting("kFactorServ") == "")	gDb.SetSetting("kFactorServ","1");
			if (gDb.GetSetting("kFactorInt") == "")		gDb.SetSetting("kFactorInt","2.5");
			if (gDb.GetSetting("DEMsource") == "")		gDb.SetSetting("DEMsource","1");
			if (gDb.GetSetting("DownLink") == "")		gDb.SetSetting("DownLink","1");
			if (gDb.GetSetting("ClutterSource") == "")	gDb.SetSetting("ClutterSource","1");
			if (gDb.GetSetting("UseClutter") == "")		gDb.SetSetting("UseClutter","-1");
			if (gDb.GetSetting("PlotResolution") == "")	gDb.SetSetting("PlotResolution","90");
			if (gDb.GetSetting("BTLDir") == "")		gDb.SetSetting("BTLDir","Data/BTL/");
			if (gDb.GetSetting("OutputDir") == "")		gDb.SetSetting("OutputDir","Data/Output/");
			if (gDb.GetSetting("SiteSelectSense") == "")	gDb.SetSetting("SiteSelectSense","150");
			cout << "QRap::openDatabaseConnection(): Done setting defaults " << endl;
		}
		else
		{
			QMessageBox::information(mQGisIface->mainWindow(), "Q-Rap", "Failing to connect to QRap-database. Q-Rap is a Radio Planning Tool that uses a PostgreSQL database. For more information on how to create the database see the manual.pdf $3.2. Also see http://www.QRap.org.za for more info. Removing Q-Rap plugin");	
			cout << "QRap::openDatabaseConnection(): else gDb.Connected() " << endl;
			unload();
		}

	}
	cout << "Verlaat openDatabaseConnection " << endl;
} 

//*************************************************************************************
//* This actually calls the database interface
void QRap::run()
{
 	gMainWindow = new MainWindow(qgisMainWindow);
	connect(gMainWindow, SIGNAL(SelectArea()),this,  SLOT(SelectArea()));
	gMainWindow->show();
	mQGisIface->mapCanvas()->refresh();
}

//**************************************************************************************
void QRap::PlaceSite()
{
	cout << " in QRap::PlaceSite() " << endl;
	mMouseType = PLACESITE;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
}

//************************************************************************************
void QRap::Prediction()
{
	cout << " in QRap::Prediction() " << endl;
	mMouseType = AREA;
	InitRubberBand(true);
}

//************************************************************************************
void QRap::CreateLinkAnalysis()
{
	cout << " in QRap::CreateLinkAnalysis() " << endl;
	mMouseType = LINK1;
	InitRubberBand(false);
}

//*************************************************************************************
void QRap::SpectralAnalysis()
{
	cout << " in QRap::SpectralAnalysis() " << endl;
	mMouseType = SPECTRAL;
	InitRubberBand(true);
}

//*************************************************************************************
void QRap::MultiLink()
{
	cout << " in QRap::MultiLink() " << endl;
	mMouseType = MULTILINK;
	InitRubberBand(true);
}

//*************************************************************************************
void QRap::ReceivedRightPoint(QgsPoint &Point)
{
	cout << " in QRap::ReceivedRIGHTPoint(QgsPoint &Point) " << endl;
	if (mMouseType != CLEAN)
	{
		if (mMouseType == PLACESITE)
		{
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == SELECTLINK)
		{
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == DELETELINK)
		{
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == LINK1 || mMouseType == LINK2)
		{
			DesRubberBand();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == AREA)
		{
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			PerformPrediction();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mMouseType = CLEAN;
		}
		if (mMouseType == FILTERAREA)
		{
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			FilterArea();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mMouseType = CLEAN;
		}
		if (mMouseType == SPECTRAL)
		{
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			PerformSpectral();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mMouseType = CLEAN;
		}
		if (mMouseType == MULTILINK)
		{
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			PerformMultiLink();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mMouseType = CLEAN;
		}
		if (mMouseType == OPTIMISATION)
		{
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			PerformOptimisation();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mMouseType = CLEAN;
		}
	}
}


//******************************************************************************************
void QRap::ReceivedLeftPoint(QgsPoint &Point)
{
	cout << " in QRap::ReceivedLeftPoint(QgsPoint &Point) " << endl;
	if (mMouseType != CLEAN)
	{
		if (mMouseType == MOVESITE)
		{
			cout << " movesite " << endl;
			QMessageBox::information(mQGisIface->mainWindow(), "Q-Rap", "Wait: getting height data at point");
			MoveSiteDialog((Point.y()),(Point.x()));
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			mQGisIface->mapCanvas()->refresh();
		}
		else if (mMouseType == PLACESITE)
		{
			cout << " placesite " << endl;
			QMessageBox::information(mQGisIface->mainWindow(), "Q-Rap", "Wait: getting height data at point");
			PlaceSiteDialog((Point.y()),(Point.x()),false);
			if (mMouseType!=MOVESITE)
			{
				mMouseType = CLEAN;
				mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
				mQGisIface->mapCanvas()->refresh();
			}
		}
		else if (mMouseType == SELECTSITE)
		{
			cout << " selectsite " << endl;
			PlaceSiteDialog((Point.y()),(Point.x()),true);
			if (mMouseType!=MOVESITE)
			{
				mMouseType = CLEAN;
				mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
			}
		}
		if (mMouseType == LINK2)
		{
			cout << " Link2 " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			DesRubberBand();
			PerformLink();
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == LINK1)
		{
			cout << " Link1 " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
			mMouseType = LINK2;
		}
		if (mMouseType == AREA)
		{
			cout << " Area " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
		}
		if (mMouseType == FILTERAREA)
		{
			cout << " Filter Area " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
		}
		if (mMouseType == SELECTLINK)
		{
			cout << " Select Link " << endl;
			GetLink((Point.y()),(Point.x()));
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == SPECTRAL)
		{
			cout << " spectral " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
		}
		if (mMouseType == MULTILINK)
		{
			cout << " multilink " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
		}
		if (mMouseType == OPTIMISATION)
		{
			cout << " optimisation " << endl;
			mPoints.append(Point);
			mRubberBand->addPoint(Point);
		}
		if (mMouseType == DELETESITE)
		{
			cout << "Deleting site we hope ..." << endl;
			cDeleteObject *Delete = new cDeleteObject(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
			cout << "After delete object constructer" << endl;
			QString Lat = QString("%1").arg(Point.y());
			QString Lon = QString("%1").arg(Point.x());
			if (Delete->SetParameters(Lat,Lon,true,mQGisIface->mapCanvas()->scale()))
			{
				if (Delete->exec()==1)
				{
					mQGisIface->mapCanvas()->refresh(); 
				}
			}
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
		if (mMouseType == DELETELINK)
		{
			cout << " delete link " << endl;
			cDeleteObject *Delete;
			Delete = new cDeleteObject(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
			QString Lat = QString("%1").arg(Point.y());
			QString Lon = QString("%1").arg(Point.x());
			if (Delete->SetParameters(Lat,Lon,false,mQGisIface->mapCanvas()->scale()))
			{
				if (Delete->exec()==1)
				{
					mQGisIface->mapCanvas()->refresh(); 
				}
			}
			mMouseType = CLEAN;
			mQGisIface->mapCanvas()->setCursor(Qt::OpenHandCursor);
		}
	}
	else  cout << " clean " << endl;
}


//*****************************************************************************************
void QRap::PlaceSiteDialog(double lat, double lon,bool IsOld)
{
	cout << " In QRap::PlaceSiteDialog" << endl;
	QString Lat = QString("%1").arg(lat);
	QString Lon = QString("%1").arg(lon);
	mPlacedSite = new cPlaceSite(mQGisIface, mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	connect(mPlacedSite, SIGNAL(MoveSiteClicked()),this,  SLOT(MoveSiteClicked()));

	// place the site
	if (mPlacedSite->SetParameters(Lat,Lon,IsOld,mQGisIface->mapCanvas()->scale()))
	{
		if (mPlacedSite->exec()==1)
		{
			mQGisIface->mapCanvas()->refresh(); 
		}
	}
}

//*****************************************************************************************
void QRap::MoveSiteDialog(double lat, double lon)
{
	QString Lat = QString("%1").arg(lat);
	QString Lon = QString("%1").arg(lon);
	cout << " In QRap::MoveSiteDialog(double lat, double lon)" << endl;
	// place the site
	mPlacedSite->show();
	mPlacedSite->ChangeLocation(Lat,Lon);
	if (mPlacedSite->exec()==1)
	{
		mQGisIface->mapCanvas()->refresh(); 
	}
}


//***************************************************************************************
void QRap::ReceiveMouseMove(QgsPoint &Point)
{
//	cout << "QRap::ReceiveMouseMove" << endl;
	if (mMouseType != CLEAN || mMouseType!=PLACESITE)
	{
		 mRubberBand->movePoint(Point);
	}
}

//***************************************************************************************
void QRap::UpdateSiteLayer()
{
	cout << "QRap::UpdateSiteLayer()" << endl;
	int LayerCount = mQGisIface->mapCanvas()->layerCount();
	QString temp = QString("%1").arg(LayerCount);
	QMessageBox::information(mQGisIface->mainWindow(), "Q-Rap", temp);
}

//****************************************************************************************
void QRap::InitRubberBand(bool IsArea)
{
	cout << "QRap::InitRubberBand(bool IsArea) " << endl;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	QGis::GeometryType bArea;
	if (IsArea)
		bArea = QGis::Polygon;
	else 	bArea = QGis::Line;

	mPoints.clear();
	mRubberBand = new 
	QgsRubberBand(mQGisIface->mapCanvas(), bArea);
	connect(Mouse, SIGNAL(MouseMove(QgsPoint&)), this, SLOT(ReceiveMouseMove(QgsPoint&)));
  	mQGisIface->mapCanvas()->setCursor(Qt::CrossCursor);
  
  	int myRed = 180;//settings.value("/qgis/default_measure_color_red", 180).toInt();
 	int myGreen = 180;//settings.value("/qgis/default_measure_color_green", 180).toInt();
  	int myBlue = 180;//settings.value("/qgis/default_measure_color_blue", 180).toInt();
  	mRubberBand->setColor(QColor(myRed, myGreen, myBlue));
} 

//*****************************************************************************************
void QRap::DesRubberBand()
{
	cout << "mMouseType = CLEAN in QRap::DesRubberBand()" << endl;
	mMouseType = CLEAN;
	disconnect(Mouse, SIGNAL(MouseMove(QgsPoint&)), this, SLOT(ReceiveMouseMove(QgsPoint&)));
	delete mRubberBand;
	mQGisIface->mapCanvas()->setCursor(Qt::ArrowCursor);
}




//***************************************************************
void QRap::PerformLink()
{
	cConfirmLink *ConfirmLink = new cConfirmLink(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	if (ConfirmLink->SetPoints(mPoints,mQGisIface->mapCanvas()->scale()))
	{
		cout << " In QRap::PerformLink(): SetPoints successful." << endl;
		if (ConfirmLink->exec()==1)
		{
			cout << " In QRap::PerformLink(): ConfirmLink->exec()=1" << endl;
			cLinkAnalysis *Plot = new cLinkAnalysis(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
			Plot->DoAndSetUpDisplay(ConfirmLink->Units, ConfirmLink->Downlink, ConfirmLink->Frequency,
						ConfirmLink->kFactor, ConfirmLink->PlotResolution,
						ConfirmLink->DEMsource,	ConfirmLink->ClutterSource, ConfirmLink->UseClutter,
						ConfirmLink->TxID, ConfirmLink->RxID, ConfirmLink->mLinkName,
						ConfirmLink->TxName, ConfirmLink->RxName,0);
			if (Plot->exec())
			{
				mQGisIface->mapCanvas()->refresh();
			}
			
		}
	}
	cout << " In QRap::PerformLink(): Exiting." << endl;
}

/***************************************************************/
bool QRap::PerformPrediction()
{
	cConfirmPrediction *ConfirmPrediction = 
			new cConfirmPrediction(mQGisIface, mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	if (ConfirmPrediction->SetPoints(mPoints))
	{
		if (ConfirmPrediction->exec()==1)
		{
			mQGisIface->mapCanvas()->refresh();
		}//if accepted
	}//if has sites
	return true;
}

/***********************************************************/
void QRap::GetLink(double lat, double lon)
{
	cout << "mMouseType = CLEAN in QRap::GetLink(double lat, double lon)" << endl;
	mMouseType = CLEAN;
	char Lat[33];
	char Lon[33];
	gcvt(lat,8,Lat);
	gcvt(lon,8,Lon);
	string query = "SELECT id, txinst,rxinst, linkname, frequency, kfactor ";
	query += "FROM links WHERE line && ST_GeomFromText('POINT(";
	query +=  Lon;
	query += " ";
	query += Lat;
	query += ")',4326)";
	if (!gDb.PerformRawSql(query))
	{
		cout << "In QRap::GetLink(...)  Database link select failed"<< endl;
	}
	else
	{
		pqxx::result Li;
		gDb.GetLastResult(Li);
		if (Li.size()>0)
		{
			cout << "In QRap::GetLink(...) Database link select suceeded" << endl;
			int ID =(int)atof(Li[0]["id"].c_str()) ;
			int TxID =(int)atof(Li[0]["txinst"].c_str()) ;
			int RxID = (int)atof(Li[0]["rxinst"].c_str());
			double frequency = (double)atof(Li[0]["frequency"].c_str());
			double kfactor = (double)atof(Li[0]["kfactor"].c_str());
			string name = Li[0]["linkname"].c_str();
			cout << name << endl;
			cConfirmLink *ConfirmLink = new cConfirmLink(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
			cout << "In QRap::GetLink(...) After constructor " << endl;
			if (ConfirmLink->SetOldLink(ID,TxID,RxID,name,frequency, kfactor))
			{
				cout << "In QRap::GetLink(...) SetOldLink success " << endl;
				if (ConfirmLink->exec()==1)
				{
					cLinkAnalysis *Plot = new cLinkAnalysis(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
					Plot->DoAndSetUpDisplay(ConfirmLink->Units, ConfirmLink->Downlink, ConfirmLink->Frequency,
							ConfirmLink->kFactor, ConfirmLink->PlotResolution,
							ConfirmLink->DEMsource,	ConfirmLink->ClutterSource, ConfirmLink->UseClutter,
							ConfirmLink->TxID, ConfirmLink->RxID, ConfirmLink->mLinkName,
							ConfirmLink->TxName, ConfirmLink->RxName, ID);
					if (Plot->exec())
					{
						mQGisIface->mapCanvas()->refresh();
					}
				}
			}
		}	
	}
}

/************************************************************/
/*void QRap::ColourManager()
{
	cColourManager *CM = new cColourManager(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	CM->show();
}*/

//**********************************************************************
void QRap::PerformSpectral()
{
	cout << "voor new Confirmspectral in QRap::PerformSpectral()" << endl;
	cConfirmSpectral ConfirmSpectral(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	cout << "voor ConfirmSpectral->SetPoints(mPoints) in QRap::PerformSpectral()" << endl;
	if (ConfirmSpectral.SetPoints(mPoints))
	{
		cout << "voor ConfirmSpectral->exec() in QRap::PerformSpectral()" << endl;
		if(ConfirmSpectral.exec()==1)
		{
			cSpectralAnalysis Plot(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
			Plot.DoAndSetUpDisplay(ConfirmSpectral.mRx,ConfirmSpectral.mRadInst,
						ConfirmSpectral.mFrequencySpacing, ConfirmSpectral.mkFactor);
			if (Plot.exec())
			{
				mQGisIface->mapCanvas()->refresh();
			}
		}
	}
}

//**********************************************************************
void QRap::PerformMultiLink()
{
	cConfirmMultiLink ConfirmMultiLink(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	if (ConfirmMultiLink.SetPoints(mPoints))
	{
		if(ConfirmMultiLink.exec()==1)
		{
			mQGisIface->mapCanvas()->refresh();
		}
	}
}

//**********************************************************************
void QRap::PerformOptimisation()
{
	Optimisation Optim(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	if ((Optim.SetPoints(mPoints))&&(Optim.LoadAntennas())&&(Optim.LoadCables())&&(Optim.LoadConnectors()))
	{
		if(Optim.exec()==1)
		{
			mQGisIface->mapCanvas()->refresh();
		}
	}
}

//**************************************************************
void QRap::DeleteLink()
{
	mMouseType = DELETELINK;
	cout << " Delete Link " << endl;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	mQGisIface->mapCanvas()->setCursor(Qt::ArrowCursor);
}

//****************************************************************
void QRap::DeleteSite()
{
  	mMouseType = DELETESITE;
	cout << " Delete Site " << endl;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	mQGisIface->mapCanvas()->setCursor(Qt::ArrowCursor);	
}

//********************************************************
 void QRap::SelectArea()
{
	cout << " Select Area " << endl;
  	mMouseType = FILTERAREA;
	InitRubberBand(true);
}

//*****************************************************************************************
void QRap::SelectSite()
{
	mMouseType = SELECTSITE;
	cout << " Set Select SITE " << endl;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	mQGisIface->mapCanvas()->setCursor(Qt::ArrowCursor);	
}

//***********************************************************************************
void QRap::SelectLink()
{
	mMouseType = SELECTLINK;
	cout << " Set Select Link " << endl;
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	mQGisIface->mapCanvas()->setCursor(Qt::ArrowCursor);
}

//************************************************************************************
void QRap::Preferences()
{
	PreferencesDialog *Preferences = new PreferencesDialog();
	Preferences->show();
}

//************************************************************************************
void QRap::Optimise()
{
	mMouseType = OPTIMISATION;
	cout << " Optimise Mesh network " << endl;
	InitRubberBand(true);
}

//************************************************************************************
void QRap::Measurements()
{
	cMeasurementAnalysis *Measurements = new cMeasurementAnalysis();
	Measurements->show();
}


//****************************************************************************************
void QRap::ImportExport()
{
	printf("QRap::ImportExport\n");

	// create a new menu
//	cImportExportMenu *ifMenuDialog = new cImportExportMenu();

	// set the db username and password
//	ifMenuDialog->setDB(dbusername,dbpassword);
//	ifMenuDialog->setMachine(machinename);
	
	// show the menu
//	ifMenuDialog->show();
}

//******************************************************************
void QRap::Help()
{
	// launch the help file in evince
//	system("evince /usr/share/doc/qrap/manual.pdf&");
}

//********************************************************
 void QRap::MoveSiteClicked()
{
	mPlacedSite->hide();
	mQGisIface->mapCanvas()->setMapTool(Mouse);
	mQGisIface->mapCanvas()->setCursor(Qt::CrossCursor);
	mMouseType = MOVESITE;  
}
  

//********************************************************
void QRap::FilterArea()
{
	cFilterDialog* Filter = new cFilterDialog(mQGisIface->mainWindow(), QgisGui::ModalDialogFlags);
	Filter->SetPoints(mPoints);
	Filter->show();
}






