 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapTablesTab.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that will display a table view in the  
 *                  one tab and a edit form on the other tab.
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

#ifndef Qrap_cRapTableTab_h
#define Qrap_cRapTableTab_h

// Include the necessary Qt header
#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QScrollArea>
#include <QSizePolicy>

// Include the local header
#include "MainWindow.h"
#include "cRapFormBuilder.h"

namespace Qrap
{
	/**
	* The cRapTableTab class, this class inherits the QTabWidget class and is responsible for containing
	* a tab that displays a table view containing all the data for the specified database table and the other tab displays an edit form.
	*/
	class cRapTableTab : public QTabWidget
	{
		Q_OBJECT
	
	public:
		/**
		* This constructor is responsible for creating the two tabs and loading the data into the table view.
		* 
		* @param parent A pointer to this widget's parent widget.
		* @param table The name of the database table that this cRapTableTab represents.
		*/	
		cRapTableTab (QWidget* parent=0, QString table = "NULL");
		
		/**
		 * This function returns a pointe to the table view.
		 * 
		 * @return A pointer to the table widget.
		 */
		QTableWidget* GetTable ()
		{
			return mTableView;
		}
		
		/**
		 * This function destroys an existing form widget and creates an insert form widget.
		 */
		void CreateInsertForm ();
		void CreateUpdateForm ();
		
		/**
		 * This sets the refereces to cerain fields
		 * 
		 * @param reference A set of references
		 */
		 void SetReferences(StringMap reference) {mRef =reference;}

		QTableWidget*	mTableView;		///< A pionter to a Qt4 table widget that contains the data.
		QTableWidget*	mAntennaPatternView;	///< A pointer to a Qt4 table widget that contains all the data for the antenna pattern data this will only be applicable to the antenna device table.
		QString		mTableName;		///< Stores the name of the database table that this cRapTableTab is related to.
		bool		*mTableViewSelected;	///< A pointer to a boolean that keeps track of the current selected view.
		
	public slots:
		/**
		 * Private SLOT - This slot is responsible for managing the index changes of the tab widget.
		 */
		void TabIndexChanged (int index);
		
		
	private:
		cRapFormBuilder*	mForm;			///< A pointer to a cRapFormBuilder class that will generate the form when the user selects the form tab.
		QScrollArea*		mScroller;		///< A pointer to a Qt4 scroll area that will enable this widget to be viewed fully by scroll bars.
		bool			mInserting;		///< Lets the object know that an insert form is being use instead of a normal form.
		StringMap 		mRef;
	};
}
#endif
