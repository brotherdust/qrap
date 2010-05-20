 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : RapTab.h
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

#ifndef Qrap_RapTab_h
#define Qrap_RapTab_h

// Include the necessary Qt header
#include <QWidget>
#include <QTabWidget>

// Include the local header
#include "MainWindow.h"
#include "cRapSites.h"
#include "cRapCells.h"
#include "cRapInventory.h"
#include "cRapSupportingTables.h"
#include "cRapLinks.h"
#include "cRapServedKeyLocations.h"
#include "cRapRasterFiles.h"

namespace Qrap
{
	/**
	* The RapTab class, this class inherits the QTabWidget class and is responsible for containing
	* all the tabs that contain the table views for navigating the Qrap system.
	*/
	
	class RapTab : public QTabWidget
	{
		Q_OBJECT
		
	public:
		/**
		* This constructor is responsible for querying the database structure in the database engine object
		* and builds the GuiEngine class that will contain all the data for each table to which the Qrap user 
		* has access.
		*/	
		RapTab (QWidget* parent=0);
		
		cRapSites*		mSites;			///< A pointer to a cRapSites object that allows the user to navigate through the Qrap system.
		
	private slots:
		/**
		 * Private SLOT - This slot is responsible for managing the index changes of the tab widget.
		 */
		void TabIndexChanged (int index);
		
	private:
		
		cRapCells*		mCells;			///< A pointer to a cRapCells that allows the user to manage relevant cells data.
		cRapInventory*		mInventory;		///< A pointer to a cRapInventory object that allows the user to manage the inventory data.
		cRapSupportingTables*	mSupportingTables;	///< A pointer to a cSupportingTables object that allows the user to manage the supporting tables.
		cRapLinks*		mLinks;			///< A pointer to a cRapLinks object that allows the user to manage the links data.
		cRapServedKeyLocations*	mServedKeyLocations;	///< A pointer to a cRapServedKeyLocations object that allows the user to manage the served key locations.
		cRapRasterFiles*	mRasterFiles;		///< A pointer to a cRapRasterFiles object that allos the user to manage the raster file data.
	};
}
#endif
