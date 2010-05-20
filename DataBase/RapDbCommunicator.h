 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : RapDbCommunicator.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : A custom class that will enable different widgets to 
 *                  be able to communicate effectively with the database 
 *                  system i.e. insert, delete and select. The output
 *                  is connected to the calling widget's table widgets.
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

#ifndef Qrap_RapDbCommunicator_h
#define Qrap_RapDbCommunicator_h

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
#include <QVariant>
#include <QFileDialog>
#include <QTextStream>
#include <math.h>

//include local headers
#include "ComboDelegate.h"
#include "MainWindow.h"

namespace Qrap
{
	/**
	 * The RapDbCommunicator class is a custom class that will enable a calling class to communicate with
	 * the database engine. The output is rendered in the calling class's table widget that is passed
	 * to the functions.
	 */
	class RapDbCommunicator
	{
		public:
			/**
			 * The constructor is responsible for setting up the layout structure and the 
			 * creating the widgets.
			 */
			RapDbCommunicator (QWidget* parentWidget =0 ,
					   QProgressBar* progressBar = NULL);
			
			/**
			 * This function is responsible for converting the location format from degrees:minutes:seconds to degrees only.
			 */
			QString ConvertPointLocation (QString value  );
			
			/** 
			 * This function is responsible for deleting the selected set of rows from the database engine.
			 */
			void DeleteRows (QTableWidget* table,
					 QString tableName);
			
			/**
			 * Private SLOT - This slot is called whenever an item in a table is changed, it will update the 
			 * database with the new changes.
			 */
			void UpdateDatabase (QTableWidget* table,
					     QString tableName,
					     int row,
					     int col);
			
			/**
			 * This function loads the headers for a particular table.
			 * 
			 * @param table A pointer to the table widget that the data must be written to.
			 * @param tableName A QString containing the name of the table.
			 */
			void LoadTableHeaders (QTableWidget* table,
					       QString tableName,
					       QString keyField,
					       bool view=false);
			
			/**
			 * This function sets the value of a cell in the table view to the appropriate data type.
			 * 
			 * @param tableName The name of the table that this data belongs to.
			 * @param fieldName The name of the field that this data belongs to.
			 * @param val The raw value of the cell as returned from the database.
			 */
			void GetCellDataType(std::string tableName,
					     std::string fieldName,
					     QVariant& val,
					     DegreeFormat locationFormat=dfDeg);
			
			/**
			 * This function is a generic function that populates the tables when a given site is
			 * selected. It is generic in the sense that it takes a number of arguments to distiguish
			 * the table that it is inserting elements into.
			 * 
			 * @param table A pointer to the table widget that the data must be written to.
			 * @param tableName A QString containing the name of the table.
			 * @param currentId An integer containing the currently selected site's id, if this parameter is -1 then all the sites will be selected.
			 * @param site A boolean that determines whether the access to a row is dependant on the status of the relevant site.
			 */
			void PopulateTable (QTableWidget* table,
					    const QString& tableName,
					    const QString& keyField,
					    int currentId,
					    bool site = false,
					    bool view = false,
					    std::string search = "");
			
			/**
			 * This function returns a SQL string that will convert the given double value from a format to dBm.
			 * 
			 * @param unit The unit type, either utPower,utSensitivity or utEirp
			 * @param fielName The database fieldName where this new value will be stored.
			 * @param value The actual value that needs to be converted.
			 *
			 * @return A SQL string that contains the unit conversion formula so that postgres can do the actual calculation.
			 */
			static std::string ConvertTodBm (UnitType unit,
							 double value);
			
			/**
			 * This function returns a SQL string that will convert the given column name from dBm to a specified format.
			 * @param unit The unit type, either utPower,utSensitivity or utEirp
			 * @param fieldName The database fieldName that contains the value;
			 */
			static std::string ConvertFromdBm (UnitType unit,
						           const std::string& fieldName);
			
		private:
			QWidget*		parent;		///< A pointer to a Qt4 widget that keeps track to which widget this object belongs.
			QProgressBar*		progress;	///< A pointer to a Qt4 progress bar widget that indicates the progress of loading data.
			DbStructure		dbs;		///< Contains a local copy of the database structure.
	};
}

#endif
