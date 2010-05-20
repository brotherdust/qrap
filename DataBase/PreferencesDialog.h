 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : PreferencesDialog.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *                : (Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 dialog widget that presents an interface that the 
 *                  user can use to change preferences to the user interface.
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
 
#ifndef Qrap_PreferencesDialog_h
#define Qrap_PreferencesDialog_h

// Include global headers
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QDialog>
#include <QIcon>
#include <QHeaderView>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

// Include local headers
#include "Config.h"
#include "../Interface/cColourManager.h"

namespace Qrap
{
	/**
	 * This class builds the entire preferences dialog and it inherits from QDialog.
	 */
	class PreferencesDialog : public QDialog
	{
		Q_OBJECT
		
		public:
			/**
			 * Constructor
			 */
			PreferencesDialog (QWidget* parent=0);
			
		private slots:
			/**
			 * Private SLOT - This slot will take all the settings and write all the settings to the database settings table.
			 */
			void WriteSettings ();
			
			/**
			 * Private SLOT - This slot will display the relevant form depending on which preference index was selected in the tree view.
			 */
			void SelectionChanged (QTreeWidgetItem* item, int col);
			
		private:
			/**
			 * This function reads all the settings from the database and sets up the widgets accordingly.
			 */
			void ReadSettings ();
			
			/**
			 * This function sets up the all the widgets in the user interface.
			 */
			void SetupUi ();
			
			/**
			 * This functions handles all the language translation for the user interface.
			 */
			void RetranslateUi ();
			
			QDialogButtonBox *buttonBox;
			QWidget *gridLayout;
			QGridLayout *gridLayout1;
			QTreeWidget *preferencesTree;
			QStackedWidget *preferencesStack;
			QWidget *unitsPreferences;
			QGroupBox *sensitivityGroup;
			QRadioButton *receiverPower;
			QRadioButton *receiverVoltage;
			QRadioButton *receiverdBW;
			QGroupBox *powerGroup;
			QRadioButton *powerWattRadio;
			QRadioButton *powerdBmRadio;
			QRadioButton *powerdBWRadio;
			QGroupBox *eirpGroup;
			QRadioButton *eirpWattRadio;
			QRadioButton *eirpdBmRadio;
			QRadioButton *eirpdBWRadio;
			QLabel *unitsLabel;
			QFrame *unitsHeaderLine;
			QGroupBox *positionFormatGroup;
			QRadioButton *DDMMSSRadio;
			QRadioButton *DDMMmmRadio;
			QRadioButton *DDddddRadio;
			QGroupBox *impedenceGroup;
			QRadioButton *fiftyOhm;
			QRadioButton *seventyFiveOhm;
			QGroupBox *predictionEIRP;
			QRadioButton *explicitEIRP;
			QRadioButton *calculatedEIRP;
			QWidget *rapPreferences;
			QLabel *defaultsLabel;
			QFrame *defaultHeaderLine;
			QGroupBox *displayModeGroup;
			QRadioButton *editNormalRadio;
			QRadioButton *editDefaultsRadio;
			QWidget *plotPreferences;
			QLabel *plotPreferencesLabel;
			QFrame *plotHeaderLine;
			QGroupBox *displayUnitsGroup;
			QComboBox *displayUnitsCombo;
			QLabel *displayUnitsLabel;
			QGroupBox *plotDefaultsGroup;
			QWidget *gridLayout_2;
			QGridLayout *gridLayout2;
			QLabel *RqSNLabel;
			QDoubleSpinBox *RqSNSpinBox;
			QLabel *fadeMarginLabel;
			QDoubleSpinBox *fadeMarginSpin;
			QLabel *rxMinLabel;
			QDoubleSpinBox *rxMinSpinBox;
			QLabel *RqClCoLabel;
			QDoubleSpinBox *rqCociSpinBox;
			QLabel *RqCIadLabel;
			QDoubleSpinBox *rqCiadSpinBox;
			QLabel *rqEbNoLabel;
			QDoubleSpinBox *rqEbNoSpinBox;
			QLabel *kFactorServerLabel;
			QDoubleSpinBox *kFactorServerSpinBox;
			QLabel *kFactorIntLabel;
			QDoubleSpinBox *kFactorIntSpinBox;
			QLabel *useClutterLabel;
			QCheckBox *useClutterCheckBox;
			QLabel *siteSelectSenseLabel;
			QDoubleSpinBox *siteSelectSenseSpinBox;
			QLabel *downLinkLabel;
			QLabel *upLinkLabel;
			QRadioButton *downLinkRadio;
			QRadioButton *upLinkRadio;
			QLabel *plotResolutionLabel;
			QDoubleSpinBox *plotResolutionSpinBox;
			QGroupBox *directoriesGroup;
			QWidget *gridLayout_3;
			QGridLayout *gridLayout3;
			QLabel *btlDirectoryLabel;
			QLineEdit *btlDirectoryEdit;
			QLabel *outputDirectoryLabel;
			QLineEdit *outputDirectoryEdit;
			QGroupBox *technologyGroup;
			QLabel *technologyLabel;
			QComboBox *technologyCombo;
			QFrame *bottomLine;
			QTreeWidgetItem* colours;
			cColourManager *colour;
	};
}

#endif
