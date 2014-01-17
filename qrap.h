 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : qrap.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Dirk van der Merwe 
 *                : Magdaleen Ballot
 *		  : magdaleen.ballot@up.ac.za
 *    Description : This is the main QRap plugin for QGIS. QRap is Radio
 *                  Planning Tool.
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
 /*  $Id: QRap.h 6931 2007-05-06 02:40:33Z wonder $ */
/***************************************************************************
 *   QGIS Programming conventions:
 *   
 *   mVariableName - a class level member variable
 *   sVariableName - a static class level member variable
 *   variableName() - accessor for a class member (no 'get' in front of name)
 *   setVariableName() - mutator for a class member (prefix with 'set')
 *
 *   Additional useful conventions:
 *
 *   theVariableName - a method parameter (prefix with 'the')
 *   myVariableName - a locally declared variable within a method ('my' prefix)
 * 
 *   DO: Use mixed case variable names - myVariableName
 *   DON'T: separate variable names using underscores: my_variable_name (NO!)
 *
 * **************************************************************************/
#ifndef QRap_H
#define QRap_H

using namespace std;

//QT4 includes
#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QBitmap>
#include <QTimer>


//QGIS includes
#include <qgisplugin.h>
#include <qgisinterface.h>
#include <qgisgui.h>
#include <qgsproject.h>
#include <qgspoint.h>
#include <qgsmapcanvas.h>
#include <qgsmaptool.h>
#include <qgsmaptopixel.h> 
#include <qgsmaptoolemitpoint.h> 
#include <qgscolorrampshader.h>

#include <QCursor>
#include <QString>
#include <QObject>
#include <QMouseEvent>
#include <qgisinterface.h>
#include <qgsrubberband.h>
#include <qgsmaplayer.h>
#include <qgsrasterlayer.h>
#undef CPL_SUPRESS_CPLUSPLUS

// Radio Planning Tool Includes
#include "DataBase/Config.h"
#include "DataBase/MainWindow.h"
#include "DataBase/LoginDialog.h"
#include "Prediction/cSpectralPlot.h"
#include "Prediction/cPlotTask.h"
#include "Prediction/cLink.h"
#include <cMouseEvents.h>
#include "Interface/cPlaceSite.h"
#include "Interface/cDeleteObject.h"
#include "Interface/cConfirmLink.h"
#include "Interface/cConfirmPrediction.h"
#include "Interface/cConfirmSpectral.h"
#include "Interface/cConfirmMultiLink.h"
#include "Interface/cLinkAnalysis.h"
#include "Interface/cSpectralAnalysis.h"
#include "Interface/cColourManager.h"
#include "Interface/cFilterDialog.h"
#include "DataBase/PreferencesDialog.h"
#include "Interface/cMeasurementAnalysis.h"
#include "Interface/cBackup.h"
#include "Interface/Optimisation.h"
#include "Interface/cImportExportMenu.h"
#include "Interface/cUpdateToServer.h"
#include "Interface/cCreateUpdateFile.h"
#include "Interface/cImportUpdateFile.h"

#undef CPL_SUPRESS_CPLUSPLUS

//forward declarations
class QAction;
class QToolBar;

class QgisInterface;
class QgisApp;
class QgsRubberBand;


//class Plugin;
/*
* \brief QRap plugin for QGIS
* Radio System Planning Tool
*/

enum MouseActionType
{
	CLEAN,
	PLACESITE,
	SELECTSITE,
	DELETESITE,
	MOVESITE,
	LINK1,
	LINK2,
	SELECTLINK,
	DELETELINK,
	AREA,
	MULTILINK,
	OPTIMISATION,
	SPECTRAL,
	FILTERAREA
};

class QRap: public QObject, public QgisPlugin 
{
Q_OBJECT public:
      
  	//////////////////////////////////////////////////////////////////////
  	//
  	//                MANDATORY PLUGIN METHODS FOLLOW
  	//
  	//////////////////////////////////////////////////////////////////////

  	/** 
  	* Constructor for a plugin. The QgisInterface pointer is passed by 
  	* QGIS when it attempts to instantiate the plugin.
  	* @param theInterface Pointer to the QgisInterface object. 
   	*/
  	QRap(QgisInterface * mQGisIface);
  	//! Destructor
  	virtual ~QRap();
  
 	
public slots:
  	//! init the gui
  	virtual void initGui();
  	//! Show the dialog box
  	void run();
    	//! unload the plugin
  	void unload();
  	//! show the help document
  	void help();
  
  	void Help();
  	void PlaceSite();
  	void Prediction();
  	void CreateLinkAnalysis();
  	void SelectSite();
  	void SelectLink();
  	void SelectArea();
	void MoveSiteClicked();
	void Optimise();
  	void Preferences();
	void Measurements();
  	void SpectralAnalysis();
  	void ColourManager();
  	void DeleteLink();
  	void DeleteSite();
	void MultiLink();

  	void ImportExport();

  	void Test();
  	void MakeUpdateFile();
    
  
public slots:
	void ReceivedRightPoint(QgsPoint &);
	void ReceivedLeftPoint(QgsPoint &);
	void ReceiveMouseMove(QgsPoint &);


private:

  	////////////////////////////////////////////////////////////////////
  	//
  	// MANDATORY PLUGIN PROPERTY DECLARATIONS  .....
  	//
  	////////////////////////////////////////////////////////////////////
  
  	//! Pointer to our toolbar
  	QToolBar *mToolBarPointer;
  	//! Pointer to the QGIS interface object
  	QgisInterface *mQGisIface;
  	//!pointer to the qaction for this plugin
  	QAction * mQActionPointer;
    	//! Pionter to QGIS main application object
    	QWidget *qgisMainWindow;

  	////////////////////////////////////////////////////////////////////
  	//
  	// ADD YOUR OWN PROPERTY DECLARATIONS AFTER THIS POINT.....
  	//
  	////////////////////////////////////////////////////////////////////
  
 	// Actions
  	QAction *mSiteAction;
  	QAction *mRadioAction;
  	QAction *mLinkAction;
  	QAction *mSelectSiteAction;
  	QAction *mDeleteSiteAction;
  	QAction *mSelectLinkAction;
  	QAction *mDeleteLinkAction;
	QAction *mMultiLinkAction;
  	QAction *mPreferencesAction;
	QAction *mOptimisationAction;
  	QAction *mSpectralAction;
  	QAction *mImportExportAction;
  	QAction *mHelpAction;
	QAction *mMeasAnalysisAction;
//  	QAction *mColourManagerAction;

  
  	void PlaceSiteDialog(double lat, double lon,bool IsOld);
	void MoveSiteDialog(double lat, double lon);
  	void UpdateSiteLayer();
  	void InitRubberBand(bool IsArea);
  	void DesRubberBand();
  	bool openDatabaseConnection();
  	void PerformLink();
  	bool PerformPrediction();
  	void FilterArea();
  	void PerformSpectral();
	void PerformMultiLink();
	void PerformOptimisation();
  	void GetLink(double lat, double lon);
  	MouseEvents *Mouse;
  	QString Action;
  	MouseActionType mMouseType;
  	QList<QgsPoint> mPoints;
  	QgsRubberBand *mRubberBand;  
	cPlaceSite *mPlacedSite;
	bool mLoaded;
	// the username and password for the database login.
  	string machinename;
};
#endif //QRap_H
