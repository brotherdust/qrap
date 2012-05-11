 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : MainWindow.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *                : (Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 custom widget that inherits QMainWindow and is 
 *                  responsible creating the the main window GUI.
 *
 ***************************************************************************
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


// Include global headers

#include <QtGui>
#include <iostream>

// Include local headers
#include "RapTab.h"
#include "MainWindow.h"
#include "PreferencesDialog.h"
#include "../Interface/LoadRasterFiles/cLoadRasterFiles.h"
#include "../Interface/cLoadMeasurements.h"
#include "cCsvExportDialog.h"
#include "cCsvImportDialog.h"
#include "cSearchForm.h"
#include "cAntennaFilesImporter.h"
#include "cFilter.h"
#include "QMessageBox"

#ifdef QRAP_SERVER_EDITION
#include "AdminWindow.h"
#endif

using namespace std;
using namespace Qrap;

MainWindow* Qrap::gMainWindow;

//****************************************************************************
MainWindow::MainWindow (QWidget* parent, Qt::WFlags fl)
{
	
	
	// Setup the QRAP error handling system
	QRAP_SET_ALERT_CALLBACK((AlertCallback)(Qrap::MainWindowAlert));
	
	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
	ReadSettings();
	
	// Create the clipboard
	clipboard = QApplication::clipboard();
	
	// Set the window title
	setWindowTitle("QRAP Database Interface 0.1    (GNU GPL v.3)   see  www.QRap.org.za ");
	setWindowIcon(QIcon(":images/logo_icon.png"));
	
	// Check if there are any system preferences
	if(gDb.GetSetting("location")=="")
	{
//		gDb.SetSetting("location","DD.dddd");
		gDb.SetSetting("sensitivity","dBm");
		gDb.SetSetting("power","dBm");
		gDb.SetSetting("eirp","dBm");
		gDb.SetSetting("impedence","50");
		gDb.SetSetting("predictioneirp","explicit");
		gDb.SetSetting("location","DD:MM:SS");
		gDb.SetSetting("mode","normal");
		gDb.SetSetting("DisplayUnits","dBm");
		gDb.SetSetting("TechType","0:NULL");
		gDb.SetSetting("RqSN","8.00");
		gDb.SetSetting("FadeMargin","3.00");
		gDb.SetSetting("RxMin","-110.00");
		gDb.SetSetting("RqCiCo","9.00");
		gDb.SetSetting("RqCiad","-9.00");
		gDb.SetSetting("RqEbNo","8.00");
		gDb.SetSetting("kFactorServer","1.00");
		gDb.SetSetting("kFactorInt","2.5");
		gDb.SetSetting("UseClutter","false");
		gDb.SetSetting("BTLDir","/home/");
		gDb.SetSetting("OutputDir","/home");
		gDb.SetSetting("SiteSelectSense","150.00");
		gDb.SetSetting("PlotResolution","90.00");
		gDb.SetSetting("DownLink","true");
	} // if GetSetting(location)
	
//	QWidget* gridLayout_1 = new QWidget(this);
//	gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
	//gridLayout_1->setGeometry(QRect(0,70,790,180));
//	setCentralWidget(gridLayout_1);
//	QGridLayout* gridLayout1 = new QGridLayout(gridLayout_1);
//	gridLayout1->setSpacing(6);
//	gridLayout1->setMargin(0);
//	gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));

	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    	sizePolicy.setHorizontalStretch(200);
    	sizePolicy.setVerticalStretch(200);

    	QSplitter*  splitter;
	splitter = new QSplitter(this);
    	splitter->setObjectName(QString::fromUtf8("splitter"));
    	splitter->setOrientation(Qt::Horizontal);
	splitter->setOpaqueResize(true);
	splitter->setSizePolicy(sizePolicy);
    	sizePolicy.setHeightForWidth(splitter->sizePolicy().hasHeightForWidth());
	setCentralWidget(splitter);

	rapTab = new RapTab(splitter);
	rapTab->setGeometry(QRect(10,10,1550,450));
	rapTab->setMinimumSize(QSize(100, 100));
	rapTab->setSizePolicy(sizePolicy);
    	splitter->addWidget(rapTab);	

	Filter = new cFilter(splitter);
	Filter->setGeometry(QRect(10,10,350,450));
	Filter->setMinimumSize(QSize(100, 100));
	Filter->setSizePolicy(sizePolicy);
	splitter->addWidget(Filter);

	setMaximumHeight(700);
	showMaximized ();
	

//	QSpacerItem *spacerItem1 = new QSpacerItem(1, 90, QSizePolicy::Minimum, QSizePolicy::Minimum);
//    gridLayout1->addItem(spacerItem1, 0, 1, 1, 1);
	
	
//	QSpacerItem *spacerItem = new QSpacerItem(1, 400, QSizePolicy::Minimum, QSizePolicy::Maximum);
//    gridLayout1->addItem(spacerItem, 1, 1, 1, 1);
		
	// Create the main widget
//	CreateDockingTab(Body);
	
	
}

//**************************************************************************
void Qrap::MainWindowAlert (AlertLevel level, const std::string& msg)
{
	// Call the alert function handler
	gMainWindow->Alert(level, msg);
}

//*****************************************************************************
// Creates the docking tab and all the table views
void MainWindow::CreateDockingTab (QWidget* Parent)
{

	rapTab = new RapTab(Parent);
//	setCentralWidget(rapTab);
	rapTab->show();
}


//********************************************************************************
// Displays the error message and a QMessageBox
void MainWindow::Alert (AlertLevel level,
	    		const std::string& msg)
{
	AlertCode alert = gDb.GetLastAlert();
	QString str = QString::fromStdString(msg);
	
	if(alert!=acOk)
	{
		QString lastAlert = QString::fromStdString(TranslateAlertCode(alert));
		str += "\n" + lastAlert + "\n\n Check the log file for more detail.";
	} // if acOK
	else
	{
		str += "";
	} // else acOK
	
	
	switch (level)
	{
		case alWarning:
			QMessageBox::warning(this,"QRAP Warning", str);
			cout << "[QRAP-WARNING] " << str.toStdString() << endl;
			break;
		case alError:
			QMessageBox::critical(this,"QRAP Error", str);
			cout << "[ QRAP-ERROR ] " << str.toStdString() << endl;
			break;
		case alFatal:
			QMessageBox::warning(this,"QRAP Fatal Error", str);
			cout << "[ QRAP-FATAL ] " << str.toStdString() << endl;
			break;
		default:
			QMessageBox::information(this,"QRAP Information", str,QMessageBox::Information, 						QMessageBox::NoButton);
			cout << "[ QRAP-INFO  ] "<< str.toStdString() << endl;
			break;
	}
}

//*********************************************************************************
// Captures the close event
void MainWindow::CloseEvent (QCloseEvent *event)
{
	WriteSettings();
	event->accept();
}

//*********************************************************************************************
// Establishes a new connection with the main Qrap database
void MainWindow::NewConnection ()
{

}

//*******************************************************************************************
// Renders a simple about message describing what Qrap is
void MainWindow::About ()
{
	QMessageBox::about(this, tr("About QRAP"),
		tr("<b>QRap</b><br><br>"
		  "Version 0.1"
		  " Licensed under the GNU General Public License version 3"
		  " www.QRap.org.za"
		  "<p>The Qrap application allows the user to interface"
		  "with the RAP database through a conrtolled procedure.</p>"));
}

//******************************************************************************************
// Create all the menu and toolbar actions
void MainWindow::CreateActions ()
{
	connectAct = new QAction(QIcon(":/images/connect.png"), tr("&Synchronise"), this);
	connectAct->setShortcut(tr("Ctrl+S"));
	connectAct->setStatusTip(tr("Synchronise to the main QRAP database"));
	connect(connectAct, SIGNAL(triggered()), this, SLOT(NewConnection()));
	
/*	loginAct = new QAction(tr("L&ogin"),this);
	loginAct->setShortcut(tr("Ctrl+O"));
	loginAct->setStatusTip(tr("Authenticate the Qrap user"));
	connect(loginAct,SIGNAL(triggered()),this,SLOT(QrapDatabaseLogin()));*/

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
	cutAct->setShortcut(tr("Ctrl+X"));
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));

	copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));

	pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(About()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	
	insertRowAct = new QAction(QIcon(":images/add.png"),tr("&Insert"),this);
	insertRowAct->setText("Insert");
	insertRowAct->setShortcut(tr("Ctrl+N"));
	insertRowAct->setStatusTip(tr("Insert a new row"));
	
	deleteRowAct = new QAction(QIcon(":images/remove.png"),tr("&Delete"),this);
	deleteRowAct->setShortcut(tr("Ctrl+D"));
	deleteRowAct->setText("Delete");
	deleteRowAct->setStatusTip(tr("Delete selected row"));
	
	reloadTableAct = new QAction(QIcon(":images/reload.png"),tr("&Reload"),this);
	reloadTableAct->setShortcut(tr("Ctrl+R"));
	reloadTableAct->setText("Reload");
	reloadTableAct->setStatusTip(tr("Reload table data"));
	
	showAllAct = new QAction(QIcon(":images/down.png"),tr("&Show All"),this);
	showAllAct->setShortcut(tr("Ctrl+A"));
	showAllAct->setText("Show All");
	showAllAct->setStatusTip(tr("Show all the table data"));
	
	preferencesAct = new QAction(QIcon(":images/configure.png"),tr("Prefere&nces"),this);
	preferencesAct->setStatusTip(tr("Set the system preferences."));
	connect(preferencesAct,SIGNAL(triggered()),this,SLOT(LoadPreferences()));
	
	antennaImportAct = new QAction(tr("Antenna Files"),this);
	antennaImportAct->setStatusTip(tr("Import antenna files."));
	connect(antennaImportAct,SIGNAL(triggered()),this,SLOT(ImportAntennas()));
	
	rasterImportAct = new QAction(tr("Raster Files"),this);
	rasterImportAct->setStatusTip(tr("Import raster files."));
	connect(rasterImportAct,SIGNAL(triggered()),this,SLOT(ImportRasters()));

	measImportAct = new QAction(tr("Measurements"),this);
	measImportAct->setStatusTip(tr("Import Measurements in various formats to the database"));
	connect(measImportAct,SIGNAL(triggered()),this,SLOT(ImportMeasurements()));
	
	searchAct = new QAction(QIcon(":images/find.png"),tr("Search"),this);
	searchAct->setStatusTip(tr("Search for values in the current table"));
	searchAct->setShortcut(tr("Ctrl+F"));
	
	csvExportAct = new QAction(tr("Export to &CSV"), this);
	csvExportAct->setStatusTip(tr("Export the current table, multiple tables or an SQL command's results to a CSV file."));
	connect(csvExportAct, SIGNAL(triggered()), this, SLOT(ExportCsv()));
	
	csvImportAct = new QAction(tr("Import from &CSV"), this);
	csvImportAct->setStatusTip(tr("Import from data from CSV files into the database."));
	connect(csvImportAct, SIGNAL(triggered()), this, SLOT(ImportCsv()));

#ifdef QRAP_SERVER_EDITION
	adminAct = new QAction(tr("A&dmin"),this);
	adminAct->setStatusTip(tr("Administrate the Users and Groups priviledges"));
	connect(adminAct,SIGNAL(triggered()),this,SLOT(LoadAdmin()));
#endif
	
}

//**********************************************************************
// Creates the menus for the Qrap system
void MainWindow::CreateMenus ()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
// 	fileMenu->addAction(loginAct);
	fileMenu->addAction(connectAct);
	importMenu = fileMenu->addMenu(tr("&Import"));
	importMenu->addAction(antennaImportAct);
	importMenu->addAction(rasterImportAct);
	importMenu->addAction(csvImportAct);
	importMenu->addAction(measImportAct);
	
	exportMenu = fileMenu->addMenu(tr("&Export"));
	exportMenu->addAction(csvExportAct);
	
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(reloadTableAct);
	editMenu->addAction(showAllAct);
	editMenu->addAction(insertRowAct);
	editMenu->addAction(deleteRowAct);
	editMenu->addSeparator();
	
	editMenu->addAction(cutAct);
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);
	editMenu->addSeparator();
	editMenu->addAction(preferencesAct);
	
#ifdef QRAP_SERVER_EDITION
	editMenu->addAction(adminAct);
#endif

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

//************************************************************
// Creates the toolbar for the Qrap program
void MainWindow::CreateToolBars ()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(connectAct);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAct);
	editToolBar->addAction(copyAct);
	editToolBar->addAction(pasteAct);
	
	dbToolBar = addToolBar(tr("Database"));
	dbToolBar->addAction(searchAct);
	dbToolBar->addAction(reloadTableAct);
	dbToolBar->addAction(showAllAct);
	dbToolBar->addAction(insertRowAct);
	dbToolBar->addAction(deleteRowAct);
}

//*********************************************************************
// Creates the status bar for the Qrap program
void MainWindow::CreateStatusBar ()
{
	statusBar()->showMessage(tr("Ready"));
}

//***********************************************************************
// Reads the previous windows size settings from a file
void MainWindow::ReadSettings ()
{
	QSettings settings("Qrap", "Qrap Database Interface");
	QPoint pos = settings.value("pos", QPoint(100,100)).toPoint();
	QSize size = settings.value("size", QSize(950, 1900)).toSize();
	resize(size);
	move(pos);
}

//***************************************************************************
// Writes the current window size settings to a file
void MainWindow::WriteSettings ()
{
	QSettings settings("Qrap", "Qrap Database Interface");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

//****************************************************************************
// Load the preferences dialog
void MainWindow::LoadPreferences ()
{
	PreferencesDialog* preferences = new PreferencesDialog();
	preferences->show();
}

//******************************************************************************
// Import the antennas
void MainWindow::ImportAntennas ()
{
	cAntennaFilesImporter* antennas = new cAntennaFilesImporter(0);
	antennas->show();
}


//******************************************************************************
// Import measurements files
void MainWindow::ImportMeasurements()
{
	cLoadMeasurements* loadMeasurments = new cLoadMeasurements(0);
	loadMeasurments->show();
}


//******************************************************************************
// Import the rasters
void MainWindow::ImportRasters ()
{
	cLoadRasterFiles* raster = new cLoadRasterFiles(0);
	raster->show();
}

#ifdef QRAP_SERVER_EDITION
//*********************************************************************
// Loads the user and group administration dialog
void MainWindow::LoadAdmin ()
{
	AdminWindow* admin = new AdminWindow(0);
	admin->show();
}
#endif

//**********************************************************************************
void MainWindow::ExportCsv ()
{
	cCsvExportDialog* exportDlg = new cCsvExportDialog(0, currentTable);
	exportDlg->show();
}

//************************************************************************************
void MainWindow::ImportCsv ()
{
	cCsvImportDialog* importDlg = new cCsvImportDialog(0, currentTable);
	importDlg->show();
}

//*********************************************************************************
// Creates a dialog that asks the user for a username and password and then authenticates the Qrap user.
// bool MainWindow::QrapDatabaseLogin()
// {
// 	LoginDialog* login = new LoginDialog(this);
// 	login->show();
// }

//*****************************************************
void MainWindow::CreateFilter(QWidget* Parent)
{
	cFilter* Filter = new cFilter(Parent);
	Filter->show();
}

//*****************************************************
void MainWindow::FilterChanged()
{
	cout << "FILTER CHANGED IN MAIN WINDOW>>>"<<endl;
	reloadTableAct->activate(QAction::Trigger);
}

//*****************************************************
void MainWindow::Area()
{
	emit SelectArea();
	QString temp = "Please select an area on the canvas.\nPress the right mouse button to end.";
	QMessageBox::information(this, "QRap", temp);
	close();
}

