 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSearchForm.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk ((roeland.frans@gmail.com)
 *    Description : Allow searching the database
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

#ifndef Qrap_cSearchForm_h
#define Qrap_cSearchForm_h

// include the global headers
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QLabel>
#include <QListWidget>
#include <QScrollArea>
#include <QTableWidget>

// include local headers
#include "Config.h"
#include "cSearchFormWidgets.h"
#include "RapDbCommunicator.h"


namespace Qrap
{
	/**
	 * This class inherits from QDialog and is responsible for allowing the user to enter certain
	 * search strings on multiple fields in the current table.
	 */
	class cSearchForm : public QDialog
	{
		Q_OBJECT
		
		signals:
			void GetResults (std::string search);
		
		public:
			/**
			 * The constructor that creates the widget.
			 * 
			 * @param parent A pointer to the parent widget.
			 */
			cSearchForm (QWidget* parent,
				     const QString& table, QWidget* callingObject);
			
		private slots:
			/**
			 * Private SLOT - This slot will enable or disable any widget in the form that is selected or not selected in the fields list.
			 */
			void EnableWidgets ();
			
			/**
			 * Private SLOT - This slot will build the search query and save the current wild card and search type settings.
			 */
			void Search ();
			
		private:
			/**
			 * Setup the user interface.
			 */
			void SetupUi ();
			
			/**
			 * Set all the text descriptions for the static widgets on the form.
			 */
			void RetranslateUi ();
			
			QDialogButtonBox	*buttonBox;		///< Contains the buttons for the dialog box.
			QListWidget		*fieldsList;		///< This is a Qt4 pointer to a list widget that contains a list of all the visible fields in the table.
			QFrame			*bottomLine;		///< This is a Qt4 pointer to a frame widget that just provides a dividing line at the bottom of the form.
			QFrame			*line;			///< This is a Qt4 pointer to a frame widget that just provides a dividing line at the top of the form.
			QLabel			*title;			///< This is a Qt4 pointer to a label widget that displays the title of the form in bold.
			QString			tableName;		///< This contains the name of the table to which this search form belongs.
			QScrollArea		*scroller;		///< This will contain the cSearchFormWidgets widget and provide scrolling.
			cSearchFormWidgets	*form;			///< This is the class that contains all the form widgets.
			QRadioButton*		containsRadio;		///< This radio button will cause the following SQL statement to be produced: fieldName LIKE '%text%'
			QRadioButton*		beginningRadio;		///< This radio button will cause the following SQL statement to be produced: fieldName LIKE 'text%'
			QRadioButton*		endRadio;		///< This radio button will cause the following SQL statement to be produced: fieldName LIKE '%text'
			QRadioButton*		orRadio;		///< This radio button will make sure that each individual LIKE statements are joined with an OR statement.
			QRadioButton*		andRadio;		///< This radio button will make sure that each individual LIKE statements are joined with an AND statement.
			QGroupBox*		wildCardsGroup;		///< This group box will contain the radio buttons for the type of wildcards used.
			QGroupBox*		searchTypeGroup;	///< This group box will contain the radio buttons that determine if the different search statements are join with and OR or an AND statement.
			QLabel*			fieldsTitle;		///< This contains the title 'Search Fields'.
	};
}
#endif
