 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAntennaFilesImporter.h
 *    Copyright   : University of Pretoria
 *    Author      : Roeland van Nieuwkerk (roeland.frans@gmail.com)
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class creates a Qt4 dialog that will allow the user 
 *                  to import specific antenna files.
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


#ifndef Qrap_cAntennaFilesImporter_h
#define Qrap_cAntennaFilesImporter_h

// include the global headers
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QProgressBar>
#include <QListWidgetItem>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGroupBox>

// include local headers
#include "Config.h"


namespace Qrap
{
	/**
	 * This class inherits from QDialog and is responsible for allowing the user to select antenna
	 * files and import them into the antennadevice, antennapattern and antenna tables.
	 */
	class cAntennaFilesImporter : public QDialog
	{
		Q_OBJECT
		
		public:
			cAntennaFilesImporter (QWidget* parent);
			
		private slots:
			/**
			 * Private SLOT - This slot will open the file dialog box and then populate the filesList widget
			 * with the selected file names.
			 */
			void SelectFiles ();
			
			/**
			 * This function will import all the antennas from select antenna files.
			 */
			void ImportAntennas ();
			
		private:
			QDialogButtonBox*	buttonBox;	///< This is a Qt4 dialog button box that will contain the Cancel and Ok buttons.
			QStringList		fileNames;	///< A list of all the file names that have been selected.
			QListWidget*		filesList;	///< A list widget that displays the list of selected files.
			QPushButton*		browseButton;	///< A push button that will load the file dialog and get the selected filenames.
			QComboBox*		technologyCombo;	///< A combobox that will allow the user to select the relevant technology type for these antennas.
			QGroupBox*		technologyGroup;	///< A Group box that contains the technology combobox.
			QLabel*			fileLabel;	///< A label that contains the title of the files list.
			QLabel*			technologyLabel;	///< A label that describes the technology combo box.
			QProgressBar*		progress;	///< A pointer to a Qt4 progress bar that displays the progress of the importing.
	};
}
#endif
