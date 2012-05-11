 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : RapTab.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that will enable different widgets 
 *                  to be docked in a tabbed format
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

#include <QMessageBox>
#include "Config.h"
#include "RapTab.h"

using namespace std;
using namespace Qrap;

// Constructor that creates all the GuiEngine classes for each database table
RapTab::RapTab (QWidget* parent) : QTabWidget(parent)
{
	cout << "Entering RapTab::RapTab " << endl;
	mSites = new cRapSites(this);
	mCells = new cRapCells(this);
	mInventory = new cRapInventory(this);
	mSupportingTables = new cRapSupportingTables(this);
	mLinks = new cRapLinks(this);
	mServedKeyLocations = new cRapServedKeyLocations(this);
	mRasterFiles = new cRapRasterFiles(this);
	mMeasurements = new cRapMeasurements(this);
	
	// Add the mSites to the tab widget
	addTab(mSites,"Sites");
	addTab(mCells,"Cells");
	addTab(mInventory,"Inventory");
	addTab(mSupportingTables,"Supporting Tables");
	addTab(mLinks,"Links");
	addTab(mServedKeyLocations,"Served Key Locations");
	addTab(mRasterFiles,"Raster Files");
	addTab(mMeasurements,"Measurements");
	
	//Customize the tab widget
	setTabPosition(QTabWidget::North);
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    	sizePolicy.setHorizontalStretch(200);
    	sizePolicy.setVerticalStretch(200);	
	setSizePolicy(sizePolicy);
	// Add to the tab widget
	setGeometry(QRect(0,0,900,400));
	
	cout << "RapTab::RapTab connecting TabIndexChanged" << endl;
		connect(this,SIGNAL(currentChanged(int)),this,SLOT(TabIndexChanged(int)));
	
	// Make sure that the initial signals and slots are connected
	cout << "RapTab::RapTab  TabIndexChanged(currentIndex()) ... currentIndex = " << currentIndex() << endl;
	TabIndexChanged(currentIndex());
}



// Manage the insertRowAct, deleteRowAct and reloadTableAct actions
void RapTab::TabIndexChanged (int index)
{
	cout << "Entering RapTab::TabIndexChanged. index = " << index << endl;
	QAction* insertAct = static_cast<MainWindow*>(window())->insertRowAct;
	QAction* deleteAct = static_cast<MainWindow*>(window())->deleteRowAct;
	QAction* reloadAct = static_cast<MainWindow*>(window())->reloadTableAct;
	QAction* showAllAct = static_cast<MainWindow*>(window())->showAllAct;
	QAction* copyAct = static_cast<MainWindow*>(window())->copyAct;
	QAction* pasteAct = static_cast<MainWindow*>(window())->pasteAct;
	QAction* cutAct = static_cast<MainWindow*>(window())->cutAct;
	QAction* searchAct = static_cast<MainWindow*>(window())->searchAct;
	
	// Make sure that the action signals are disconnected
	disconnect(insertAct,SIGNAL(triggered()),0,0);
	disconnect(deleteAct,SIGNAL(triggered()),0,0);
	disconnect(reloadAct,SIGNAL(triggered()),0,0);
	disconnect(showAllAct,SIGNAL(triggered()),0,0);
	disconnect(copyAct,SIGNAL(triggered()),0,0);
	disconnect(pasteAct,SIGNAL(triggered()),0,0);
	disconnect(cutAct,SIGNAL(triggered()),0,0);
	disconnect(searchAct,SIGNAL(triggered()),0,0);
	
	switch(index)
	{
		case 0:
			// Make the necessary connections for the mSites object
			connect(insertAct,SIGNAL(triggered()),mSites,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mSites,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mSites,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mSites,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mSites,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mSites,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mSites,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mSites,SLOT(Search()));
			
			if( (mSites->getCurrentSiteStatus() == "Active") && (mSites->getCurrentTableName() != "site"))
			{
				insertAct->setEnabled(false);
				deleteAct->setEnabled(false);
			}
			else
			{
				insertAct->setEnabled(true);
				deleteAct->setEnabled(true);
			}
			break;
			
		case 1:
			// Make the necessary connections for the mCells object
			connect(insertAct,SIGNAL(triggered()),mCells,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mCells,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mCells,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mCells,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mCells,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mCells,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mCells,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mCells,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
			
		case 2:
			// Make the necessary connections for the mInventory object
			connect(insertAct,SIGNAL(triggered()),mInventory,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mInventory,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mInventory,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mInventory,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mInventory,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mInventory,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mInventory,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mInventory,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
			
		case 3:
			// Make the necessary connections for the mSupportingTables object
			connect(insertAct,SIGNAL(triggered()),mSupportingTables,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mSupportingTables,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mSupportingTables,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mSupportingTables,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mSupportingTables,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mSupportingTables,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mSupportingTables,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mSupportingTables,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
			
		case 4:
			// Make the necessary connections for the mLinks object
			connect(insertAct,SIGNAL(triggered()),mLinks,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mLinks,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mLinks,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mLinks,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mLinks,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mLinks,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mLinks,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mLinks,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
			
		case 5:
			// Make the necessary connections for the mServedKeyLocations object
			connect(insertAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mServedKeyLocations,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
			
		case 6:
			// Make the necessary connections for the mRasterFiles object
			connect(insertAct,SIGNAL(triggered()),mRasterFiles,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mRasterFiles,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mRasterFiles,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mRasterFiles,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mRasterFiles,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mRasterFiles,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mRasterFiles,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mRasterFiles,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;

		case 7:
			// Make the necessary connections for the mMeasurements object
			connect(insertAct,SIGNAL(triggered()),mMeasurements,SLOT(InsertRow()));
			connect(deleteAct,SIGNAL(triggered()),mMeasurements,SLOT(DeleteRows()));
			connect(reloadAct,SIGNAL(triggered()),mMeasurements,SLOT(ReloadTable()));
			connect(showAllAct,SIGNAL(triggered()),mMeasurements,SLOT(ShowAllContents()));
			connect(copyAct,SIGNAL(triggered()),mMeasurements,SLOT(Copy()));
			connect(pasteAct,SIGNAL(triggered()),mMeasurements,SLOT(Paste()));
			connect(cutAct,SIGNAL(triggered()),mMeasurements,SLOT(Cut()));
			connect(searchAct,SIGNAL(triggered()),mMeasurements,SLOT(Search()));
			insertAct->setEnabled(true);
			deleteAct->setEnabled(true);
			break;
	} // switch
}
