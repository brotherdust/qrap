 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSearchFormWidgetsWidgets.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : This class creates a Qt4 widget that will create the 
 *                  form widgets used in the search form dialog.
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

#ifndef Qrap_cSearchFormWidgets_h
#define Qrap_cSearchFormWidgets_h

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
#include <QString>
#include <QMap>
#include <QGridLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QGroupBox>
#include <QRadioButton>

// include local headers
#include "Config.h"


namespace Qrap
{
	/**
	 * This class inherits from QDialog and is responsible for allowing the user to enter certain
	 * search strings on multiple fields in the current table.
	 */
	class cSearchFormWidgets : public QWidget
	{
		Q_OBJECT
		
		public:
			/**
			 * This is the constructor.
			 */
			cSearchFormWidgets (QListWidget& fieldsList, const QString& table="", QWidget* parent=0);
			
			QMap<QString,QWidget*>formWidgets;	///< This is an associative QMap that contians all the widgets for each field in the table. The key for every field is the table name.
			QMap<QString,QLabel*>formLabels;	///< This is an associative QMap that contains all the labels for the widgets in the formWidgets form.
			
		private:
			DbStructure	dbs;			///< This will contain the entire database structure.
	};
}
#endif
