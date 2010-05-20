 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSearchFormWidgetsWidgets.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : This class creates a Qt4 widget that will create the form 
 *                  widgets used in the search form dialog.
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

#include "cSearchFormWidgets.h"

using namespace std;
using namespace Qrap;

//********************************************************************************************************
// The contructor
cSearchFormWidgets::cSearchFormWidgets (QListWidget& fieldsList, const QString& table, QWidget* parent) : QWidget(parent)
{
	
	QGridLayout* formLayout = new QGridLayout();
	
	// Get the database structure
	gDb.GetDbStructure(dbs);
	
	int size = dbs[table.toStdString()].mFields.size();
	int layoutRow = 1;
	DbFieldMap::const_iterator it;
	
	cout << "Got to the constructor: "<< table.toStdString() << endl;
	
	for (int i=0; i<size ; i++)
	{
		it = gDb.GetFieldWithOrder(table.toStdString(),i);
		
		if(it->second.mVisible)
		{
			cDatabase::FieldUiType uiType = gDb.GetFieldUiType(table.toStdString(),it->first);
			cDatabase::FieldDataType type = gDb.GetFieldDataType(table.toStdString(),it->first);
			QString fieldName = QString::fromStdString(it->first);
			
			if(uiType==cDatabase::utNormal)
			{
				cout << "Normal data type: "<< it->first << endl;
				QListWidgetItem* item;
				QString label = QString::fromStdString(dbs[table.toStdString()].mFields[it->first].mLabel);
				switch(type)
				{
					case cDatabase::dtString:
						formWidgets.insert(fieldName,dynamic_cast<QWidget*>(new QLineEdit(this)));
						formLabels.insert(fieldName,new QLabel(label));
						formLayout->addWidget(formLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
						formLayout->addWidget(formWidgets[fieldName],layoutRow++,1,1,1,Qt::AlignLeft);
						formWidgets[fieldName]->setEnabled(false);
						
						item = new QListWidgetItem(label);
						item->setData(Qt::UserRole,QVariant::QVariant(it->first.c_str()));
						fieldsList.addItem(item);
						break;
					case cDatabase::dtInteger:
						formWidgets.insert(fieldName,dynamic_cast<QWidget*>(new QSpinBox(this)));
						dynamic_cast<QSpinBox*>(formWidgets[fieldName])->setMinimum(-10000000);
						dynamic_cast<QSpinBox*>(formWidgets[fieldName])->setMaximum(10000000);
						
						formLabels.insert(fieldName,new QLabel(label));
						formLayout->addWidget(formLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
						formLayout->addWidget(formWidgets[fieldName],layoutRow++,1,1,1,Qt::AlignLeft);
						formWidgets[fieldName]->setEnabled(false);
						
						item = new QListWidgetItem(label);
						item->setData(Qt::UserRole,QVariant::QVariant(it->first.c_str()));
						fieldsList.addItem(item);
						break;
					case cDatabase::dtReal:
						formWidgets.insert(fieldName,dynamic_cast<QWidget*>(new QDoubleSpinBox(this)));
						dynamic_cast<QDoubleSpinBox*>(formWidgets[fieldName])->setMinimum(-10000000.00);
						dynamic_cast<QDoubleSpinBox*>(formWidgets[fieldName])->setMaximum(10000000.00);
						
						formLabels.insert(fieldName,new QLabel(label));
						formLayout->addWidget(formLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
						formLayout->addWidget(formWidgets[fieldName],layoutRow++,1,1,1,Qt::AlignLeft);
						formWidgets[fieldName]->setEnabled(false);
						
						item = new QListWidgetItem(label);
						item->setData(Qt::UserRole,QVariant::QVariant(it->first.c_str()));
						fieldsList.addItem(item);
						break;
					case cDatabase::dtDate:
						formWidgets.insert(fieldName,dynamic_cast<QWidget*>(new QDateEdit(this)));
						
						formLabels.insert(fieldName,new QLabel(label));
						formLayout->addWidget(formLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
						formLayout->addWidget(formWidgets[fieldName],layoutRow++,1,1,1,Qt::AlignLeft);
						formWidgets[fieldName]->setEnabled(false);
						
						item = new QListWidgetItem(label);
						item->setData(Qt::UserRole,QVariant::QVariant(it->first.c_str()));
						fieldsList.addItem(item);
						break;
					case cDatabase::dtDateTime:
						formWidgets.insert(fieldName,dynamic_cast<QWidget*>(new QDateTimeEdit(this)));
						
						formLabels.insert(fieldName,new QLabel(label));
						formLayout->addWidget(formLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
						formLayout->addWidget(formWidgets[fieldName],layoutRow++,1,1,1,Qt::AlignLeft);
						formWidgets[fieldName]->setEnabled(false);
						
						item = new QListWidgetItem(label);
						item->setData(Qt::UserRole,QVariant::QVariant(it->first.c_str()));
						fieldsList.addItem(item);
						break;
					default:
						break;
				} // switch
			} // if uiType
			else
			{
			} // else uiType
		} // if visible
	} // for
	
	setLayout(formLayout);
}
