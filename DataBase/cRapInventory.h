 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapInventory.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : This class displays all the tables related to physical 
 *                  equipment in inventory and allows the user to edit the tables.
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


#ifndef Qrap_cRapInventory_h
#define Qrap_cRapInventory_h

//include Qt headers
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QStringList>
#include <QMap>
#include <QMessageBox>
#include <QAction>
#include <QProgressBar>
#include <QClipboard>
#include <QHeaderView>
#include <QMessageBox>

//include local headers
#include "ComboDelegate.h"
#include "MainWindow.h"
#include "RapDbCommunicator.h"
#include "cRapTableTab.h"

namespace Qrap
{
	/**
	 * The cRapInventory class inherits the basic Qt Widget class and this class is responsible
	 * for providing the user a logical navigation experience through the Qrap system. It allows
	 * the user to navigate the system on a hierarchical level based on the selected site.
	 */
	class cRapInventory : public QWidget
	{
		Q_OBJECT
		
		public slots:
			/**
			 * Public SLOT - This slot is responsible for inserting a new row into the table and the database engine.
			 */
			void InsertRow ();
			
			/** 
			 * Public SLOT - This slot is responsible for deleting the selected set of rows from the database engine.
			 */
			void DeleteRows ();
			
			/**
			 * Public SLOT - This slot is responisible for reloading the contents of the current table.
			 */
			void ReloadTable ();
			
			/**
			 * Public SLOT - This slot will load all the contents for a particular table.
			 */
			void ShowAllContents ();
			
			/**
			 * Public SLOT - This slot is responsible for copying the current cell data into the clipboard.
			 */
			void Copy ();
			
			/**
			 * Public SLOT - This slot is responsible for pasting the current string into the current cell.
			 */
			void Paste ();
			
			/**
			 * Public SLOT - This slot is responsible for cutting the data from a current cell.
			 */
			void Cut ();
			
			/**
			 * Public SLOT - This slot is responsible for creating the search dialog and such that this user can perform
			 * a search on the currently selected table.
			 */
			void Search ();
			
			/**
			 * Public SLOT - This slot will execute the search query and populate the table view.
			 */
			void ExecuteSearch (std::string search);
			
		public:
			/**
			 * The constructor is responsible for setting up the layout structure and the 
			 * creating the widgets.
			 */
			cRapInventory (QWidget* parent=0);
			
			/**
			 * The destructor.
			 */
			~cRapInventory ();
			
			/**
			 * This function returns the name of the currently selected table
			 */
			QString getCurrentTableName ()
			{
				return mTables.key(mCurrentTable);
			}
			
		private slots:
			/**
			 * Private SLOT - This slot is responsible for handeling a selection change in the 
			 * tables list and making the respective table visible.
			 */
			void TableSelectionChanged ();
			
			/**
			 * Private SLOT - This slot is called whenever an item in a table is changed, it will update the 
			 * database with the new changes.
			 */
			void UpdateDatabase (int row, int col);
			
		private:
			/**
			 * This function will populate the currently selected table with the necessary additional columns.
			 */
			void PopulateTable ();
			
			/**
			 * This function will populate the table list with the relevant table names.
			 */
			void PopulateTableList ();

			QMap<QString,cRapTableTab*>	mTables;		///< A string map that contains all the RapTableTab widgets.
			QListWidget*			mTableList;		///< A Qt4 pointer to a list widget that will display all the available tables that contain all the needed information related to different sites.
			QGridLayout*			mMainLayout;		///< A Qt4 pointer to a gird layout widget that contains all the widgets widgets.
			cRapTableTab*			mCurrentTable;		///< A pointer to a RapTableTab class that stores the currently selected table.
			bool				mInsertingRow;		///< Keeps track whether a row is being inserted or not.
			QProgressBar*			mProgress;		///< A pointer to a Qt4 progress bar widget that indicates the progress of loading data.
			RapDbCommunicator*		mDbCommunicator;	///< A pointer to a RapDbCommunicator class that handles interaction with the database engine.
			bool				tableViewSelected;	///< A boolean that checks if the table view of any form is selected.
	};
}

#endif
