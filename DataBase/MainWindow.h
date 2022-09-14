 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : MainWindow.h
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


#ifndef Qrap_MainWindow_h
#define Qrap_MainWindow_h

// include Qt headers
#include <QMainWindow>
#include <QClipboard>
#include <QtWidgets/QVBoxLayout>

// include local headers
#include "Config.h"
#include "cFilter.h"

class QAction;
class QMenu;

namespace Qrap
{
	class RapTab;
	
	/**
	 * The main window class, it inherits the QMainWindow class from the Qt4 library.
	 * 
	 * This class contains all the menu, toolbar widgets aswell as the main tab widget that contains all the database tables.
	 * This class is also responsible for defining all the menu and toolbar actions and handles the copy and paste features.
	 * If the server version of the QRAP database client is compiled, this main class also creates the 
	 * actions to load the User/Groups administration dialog.
	 */


	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
	
		/**
		 * Constructor - creates the actions, menus, toolbars and the main tab widget.
		 * 
		 * The Qt4 actions are firsted created and once these actions have been created
		 * the menus and toolbars are created using these actions. The main windows central widget
		 * is set to be the tab widget that contains all the database tables in tabs.
		 * 
		 * \see createActions()
		 * \see createMenus()
		 * \see createToolBars()
		 * \see createStatusBar()
		 * \see readSettings()
		 */
		MainWindow (QWidget* parent = 0, Qt::WFlags fl = 0);
		
		/**
		 * Once the user has logged in the table vies and forms can be created, this function creates the Docking widget and
		 * all the table views.
		 */
		void CreateDockingTab (QWidget* Parent);
		
		/**
		 * This function handles all the error displaying to a QMessageBox.
		 * 
		 * @param level The level of the alert that was generated.
		 * @param msg The message that needs to be displayed.
		 */
		void Alert (AlertLevel level,
			    const std::string& msg);
		
		QAction*	insertRowAct;		///< A pointer to a Qt4 action object that connects to an insert row slot.
		QAction*	deleteRowAct;		///< A pointer to a Qt4 action object that connects to a delete row slot.
		QAction*	reloadTableAct;		///< A pointer to a Qt4 action object that connects to a table reload slot.
		QAction*	showAllAct;		///< A pointer to a Qt4 action object that connects to a slot that loads the entire table's contents.
		QClipboard*	clipboard;		///< A pointer to a Qt4 clipboard that communicates with the window system clipboard.
		QAction*	copyAct;		///< A pointer to a Qt4 action object that copies data from edit widgets into a clipboard, this is part of the edit menu.
		QAction*	pasteAct;		///< A pointer to a Qt4 action object that pastes data from the clipboard into a edit widget, this is part of the edit menu.
		QAction*	cutAct;			///< A pointer to a Qt4 action object that cuts data from edit widgets into a clipboard, this is part of the edit menu.
		QAction*	searchAct;		///< A pointer to a Qt4 action object that calls the search form and will perform a search in the database.
		QString     currentTable;           ///< Keeps track of the currently active table.
		RapTab*		rapTab;			///< A pointer to a custom RapTab object that contains all the database tables to navigate site information.
		cFilter* 	Filter;

	public slots:
		void FilterChanged();
		void Area();
		
	signals:
		void SelectArea();
		
	protected:
		/**
		 * Handles the close event for the main window.
		 * 
		 * @param event A pointer to a QCloseEvent object that contains the data of a close event.
		 */
		void CloseEvent (QCloseEvent* event);
		
	private slots:
		/**
		 * Private SLOT - This slot atempts to establish a connection with the main QRAP server.
		 */
		void NewConnection ();
		
		/**
		 * Private SLOT - This function creates a Qrap Login dialog that will atempt to authenticate the user.
		 */
// 		bool QrapDatabaseLogin ();
		
		/**
		 * Private SLOT - This slot loads a basic dialog giving brief information about the QRAP software.
		 */
		void About ();
		
		/**
		 * Private SLOT - This function will load the preferences dialog that will enable the user to set a number of system settings.
		 */
		void LoadPreferences ();
		
		/**
		 * Private SLOT - This function will load the antenna importing function from the cRapDbCommunicator class.
		 */
		void ImportAntennas ();
		
		/**
		 * Private SLOT - This function will load the raster file importing widget.
		 */
		void ImportRasters ();

		/**
		 * Private SLOT - This function will load the measurement importing widget.
		 */
		void ImportMeasurements ();
		
		/**
		 * Private SLOT - This function will open the CSV export dialog.
		 */
		void ExportCsv ();
		
		/**
		 * Private SLOT - This function will open the CSV import dialog.
		 */
		void ImportCsv ();
		
	#ifdef QRAP_SERVER_EDITION
		/**
		 * Private SLOT - If the server version is compiled this slot is defined and connected to the adminAct action.
		 */
		void LoadAdmin ();
	#endif
	
	private:
		/**
		 * Creates the actions for the main window and connects the actions' triggerd() signals to the relevant slots.
		 * It also loads icons for various actions.
		 */
		void CreateActions ();
	
		/**
		 * Creates the menus from the defined actions.
		 */
		void CreateMenus ();
		
		/**
		 * Creates the toolbars from the defined actions.
		 */
		void CreateToolBars ();
		
		/**
		 * Create the statusbar.
		 */
		void CreateStatusBar ();
	
		/**
		 * This reads the windows size settings from when the program was last run. If the program has never
		 * been run it assumes the default window size settings.
		 */
		void ReadSettings ();

		/**
		 * Creates the filtering widget.
		 */
		void CreateFilter(QWidget* Parent);
	
		/**
		 * Upon the close event the current window size settings are saved in a file, this then gets loaded the
		 * next time the program is executed.
		 */
		void WriteSettings ();
	
		QMenu*		fileMenu;		///< A pointer to a Qt4 menu object which contains all the file menu related actions.
		QMenu*		editMenu;		///< A pointer to a Qt4 menu object which contains all the edit menu related actions.
		QMenu*		importMenu;		///< A pointer to a Qt4 menu object which contains all the file importing actions.
		QMenu*		exportMenu;		///< A pointer to a Qt4 menu object which contains all the file exporting actions.
		QMenu*		helpMenu;		///< A pointer to a Qt4 menu object which contains all the help menu related actions.
		QToolBar*	fileToolBar;		///< A pointer to a Qt4 toolbar object which contains some of the file menu actions.
		QToolBar*	editToolBar;		///< A pointer to a Qt4 toolbar object which contains some of the edit menu actions.
		QToolBar*	dbToolBar;		///< A pointer to a Qt4 toolbar object which contains some of the database editing actions.
		QAction*	connectAct;		///< A pointer to a Qt4 action object that connects to the newConnection() slot and is part of the file menu.
		QAction*	loginAct;		///< A pointer to a Qt4 action object that calls the Qrap login dialog that will authenticate a user.
		QAction*	exitAct;		///< A pointer to a Qt4 action object that emits a close event, it is part of the file menu.
		QAction*	aboutAct;		///< A pointer to a Qt4 action object that loads the about dialog, this is part of the help menu.
		QAction*	aboutQtAct;		///< A pointer to a Qt4 action object that loads the aboutQt dialog, this is part of the help menu.
		QAction*	preferencesAct;		///< A pointer to a Qt4 action object that loads the preferences dialog, this will allow the user to set a number of system preferences.
		QAction*	antennaImportAct;	///< A pointer to a Qt4 action object that loads the antenna importing function in the cRapDbCommunicator class.
		QAction*	rasterImportAct;	///< A pointer to a Qt4 action object that loads the raster file importing widget.
		QAction*	measImportAct;		///< A pointer to a Qt4 action object that loads the measurement file importing widget.
		QAction*	csvExportAct;           ///< A pointer to a Qt4 action object that opens the CSV export dialog.
		QAction*	csvImportAct;           ///< A pointer to a Qt4 action object that opens the CSV import dialog.
		
	#ifdef QRAP_SERVER_EDITION
		QAction*	adminAct;		///< A pointer to a Qt4 action object that loads the User/Group administration dialog, this is part of the edit menu.
	#endif
		
	};
	
	void MainWindowAlert (AlertLevel level, const std::string& msg);
	
	/**
	 * Global MainWindow pointer.
	 */
	extern MainWindow* gMainWindow;
}
#endif
