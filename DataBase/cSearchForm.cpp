 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSearchForm.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
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

#include "cSearchForm.h"

using namespace std;
using namespace Qrap;

//************************************************************************************************
// The contructor
cSearchForm::cSearchForm (QWidget* parent, const QString& table, QWidget* callingObject) : QDialog(parent)
{
	resize(554, 465);
	setWindowTitle(QApplication::translate("cSearchForm", "QRAP Search", 0, QApplication::UnicodeUTF8));
	setWindowIcon(QIcon(QString::fromUtf8(":/images/logo_icon.png")));
	
	tableName = table;
	
	// Setup the search form
	SetupUi();
	
	// Set the relevant radio buttons
	string setting;
	setting = gDb.GetSetting("wildcards");
	
	if(setting=="%contains%")
		containsRadio->setChecked(true);
	else
		if(setting=="beginning%")
			beginningRadio->setChecked(true);
	else
		if(setting=="%end")
			endRadio->setChecked(true);
	else
		containsRadio->setChecked(true);
	
	setting = gDb.GetSetting("searchtype");
	
	if(setting=="OR")
		orRadio->setChecked(true);
	else
		if(setting=="AND")
			andRadio->setChecked(true);
	else
		orRadio->setChecked(true);
	
	// Setup the scroll area
	scroller->setWidget(form);
	
	connect(this,SIGNAL(GetResults(std::string)),callingObject,SLOT(ExecuteSearch(std::string)));
}


//**************************************************************************************************
void cSearchForm::EnableWidgets ()
{
	QList<QListWidgetItem*> selection = fieldsList->selectedItems();
	
	int size = fieldsList->count();
	
	for(int i=0 ; i<size ; i++)
	{
		QListWidgetItem* item = fieldsList->item(i);
		QString fieldName = item->data(Qt::UserRole).toString();
		
		QMap<QString,QWidget*>::const_iterator it = form->formWidgets.find(fieldName);
		
		if(selection.contains(item))
		{	
			if(it!=form->formWidgets.end())
				it.value()->setEnabled(true);
		}
		else
		{
			if(it!=form->formWidgets.end())
				it.value()->setEnabled(false);
		}
	} // for
}


//**************************************************************************************************
// Build the search query
void cSearchForm::Search ()
{
	int wildcard;
	int searchtype;
	char text[33];
	int Int;
	double Double;
	
	// Save the wild card and search type settings
	if(containsRadio->isChecked())
	{
		gDb.SetSetting("wildcards","%contains%");
		wildcard = 0;
	}
	else
	if(beginningRadio->isChecked())
	{
		gDb.SetSetting("wildcards","beginning%");
		wildcard = 1;
	}
	else
	if(endRadio->isChecked())
	{
		gDb.SetSetting("wildcards","%end");
		wildcard = 2;
	}
	
	if(orRadio->isChecked())
	{
		gDb.SetSetting("searchtype","OR");
		searchtype = 0;
	}
	else
	if(andRadio->isChecked())
	{
		gDb.SetSetting("searchtype","AND");
		searchtype = 1;
	}
	
	QMapIterator<QString,QWidget*> it(form->formWidgets);
	string where = "(";
	int j=0;
	
	while(it.hasNext())
	{
		it.next();
		string fieldName = it.key().toStdString();
		
		if(it.value()->isEnabled())
		{
			string value;
			
			cDatabase::FieldDataType type = gDb.GetFieldDataType(tableName.toStdString(),fieldName);
			
			// Add the joining statement
			if(j!=0)
			{
				if(searchtype==0)
					where += " OR ";
				else
					where += " AND ";
			} // if hasNext()
			
			where += "(text(" + tableName.toStdString() + "." + fieldName + ") LIKE " + "'";
			
			switch(type)
			{
				case cDatabase::dtString:
					value = dynamic_cast<QLineEdit*>(it.value())->text().toStdString();
					break;
				case cDatabase::dtInteger:
					Int = dynamic_cast<QSpinBox*>(it.value())->value();
					gcvt(Int,8,text);
					value = text;
					break;
				case cDatabase::dtReal:
					Double = dynamic_cast<QDoubleSpinBox*>(it.value())->value();
					gcvt(Double,10,text);
					value = text;
					break;
				case cDatabase::dtDate:
					value = dynamic_cast<QDateEdit*>(it.value())->date().toString("yyyy-MM-dd").toStdString();
					break;
				case cDatabase::dtDateTime:
					value = dynamic_cast<QDateEdit*>(it.value())->date().toString("yyyy-MM-dd hh:mm:ss").toStdString();
					break;
				case cDatabase::dtBoolean: break;
				case cDatabase::dtPostGisPoint: break;
				case cDatabase::dtPostGisLineString: break;
				case cDatabase::dtPostGisPolygon: break;
			} // switch
			
			
			switch(wildcard)
			{
				case 0:
					where += "%" + value + "%') ";
					break;
				case 1:
					where += value + "%') ";
					break;
				case 2:
					where += "%" + value + "') ";
					break;
				default:
					where += "%" + value + "%') ";
					break;
			} // switch
			
			j++;
		} // if isEndabled()
	} // while
	
	where += ")";
	
	cout << endl << where << endl;
	
	emit GetResults(where);
	
	accept();
}


//***********************************************************************************************
// Setup the user interface
void cSearchForm::SetupUi()
{
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setGeometry(QRect(210, 430, 341, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
	
	fieldsList = new QListWidget(this);
	fieldsList->setObjectName(QString::fromUtf8("fieldsList"));
	fieldsList->setGeometry(QRect(10, 10, 201, 401));
	fieldsList->setSelectionMode(QAbstractItemView::MultiSelection);
	
	connect(fieldsList,SIGNAL(itemSelectionChanged()),this,SLOT(EnableWidgets()));
	
	bottomLine = new QFrame(this);
	bottomLine->setObjectName(QString::fromUtf8("bottomLine"));
	bottomLine->setGeometry(QRect(10, 410, 541, 16));
	bottomLine->setFrameShadow(QFrame::Plain);
	bottomLine->setFrameShape(QFrame::HLine);
	
	line = new QFrame(this);
	line->setObjectName(QString::fromUtf8("line"));
	line->setGeometry(QRect(220, 20, 331, 16));
	line->setFrameShadow(QFrame::Plain);
	line->setFrameShape(QFrame::HLine);
	
	scroller = new QScrollArea(this);
	scroller->setObjectName(QString::fromUtf8("scroller"));
	scroller->setGeometry(QRect(220,230,330,181));
	
	
	title = new QLabel(this);
	title->setObjectName(QString::fromUtf8("title"));
	title->setGeometry(QRect(220, 10, 330, 22));
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setWeight(75);
	title->setFont(font);
	
	// Setup the radio buttons
	wildCardsGroup = new QGroupBox(this);
	wildCardsGroup->setObjectName(QString::fromUtf8("wildCardsGroup"));
	wildCardsGroup->setGeometry(QRect(220, 35, 330, 91));
	
	containsRadio = new QRadioButton(wildCardsGroup);
	containsRadio->setObjectName(QString::fromUtf8("containsRadio"));
	containsRadio->setGeometry(QRect(10, 20, 231, 23));
	
	beginningRadio = new QRadioButton(wildCardsGroup);
	beginningRadio->setObjectName(QString::fromUtf8("beginningRadio"));
	beginningRadio->setGeometry(QRect(10, 40, 291, 23));
	
	endRadio = new QRadioButton(wildCardsGroup);
	endRadio->setObjectName(QString::fromUtf8("endRadio"));
	endRadio->setGeometry(QRect(10, 60, 251, 23));
	
	// Setup the search group
	searchTypeGroup = new QGroupBox(this);
	searchTypeGroup->setObjectName(QString::fromUtf8("searchTypeGroup"));
	searchTypeGroup->setGeometry(QRect(220, 135, 330, 71));
	
	orRadio = new QRadioButton(searchTypeGroup);
	orRadio->setObjectName(QString::fromUtf8("orRadio"));
	orRadio->setGeometry(QRect(10, 20, 99, 23));
	
	andRadio = new QRadioButton(searchTypeGroup);
	andRadio->setObjectName(QString::fromUtf8("andRadio"));
	andRadio->setGeometry(QRect(10, 40, 99, 23));
	
	fieldsTitle = new QLabel(this);
	fieldsTitle->setObjectName(QString::fromUtf8("fieldsTitle"));
	fieldsTitle->setGeometry(QRect(220, 210, 101, 18));
	QFont fieldFont;
	fieldFont.setBold(true);
	fieldFont.setWeight(75);
	fieldsTitle->setFont(fieldFont);
	
	RetranslateUi();
	
	form = new cSearchFormWidgets((*fieldsList),tableName,this);
	
	RetranslateUi();
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(Search()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
} // setupUi



//**************************************************************************************
void cSearchForm::RetranslateUi()
{
	DbStructure dbs;
	gDb.GetDbStructure(dbs);
	
	QString temp = QString::fromStdString(dbs[tableName.toStdString()].mLabel);
	title->setText(QApplication::translate("cSearchForm", "Search: " + temp, 0, QApplication::UnicodeUTF8));
	wildCardsGroup->setTitle(QApplication::translate("cSearchForm", "Wild Cards", 0, QApplication::UnicodeUTF8));
	containsRadio->setToolTip(QApplication::translate("cSearchForm", "The search string appears anywhere in the statement", 0, QApplication::UnicodeUTF8));
	containsRadio->setText(QApplication::translate("cSearchForm", "Anywhere in the statement", 0, QApplication::UnicodeUTF8));
	beginningRadio->setToolTip(QApplication::translate("cSearchForm", "The search string appears at the beginning of the satement", 0, QApplication::UnicodeUTF8));
	beginningRadio->setText(QApplication::translate("cSearchForm", "Apears at the beginning of the statement", 0, QApplication::UnicodeUTF8));
	endRadio->setToolTip(QApplication::translate("cSearchForm", "The search string appears at the end of the statement", 0, QApplication::UnicodeUTF8));
	endRadio->setText(QApplication::translate("cSearchForm", "Apears at the end of the statement", 0, QApplication::UnicodeUTF8));
	searchTypeGroup->setTitle(QApplication::translate("cSearchForm", "Search Type", 0, QApplication::UnicodeUTF8));
	orRadio->setToolTip(QApplication::translate("cSearchForm", "Join all the search fields with and OR", 0, QApplication::UnicodeUTF8));
	orRadio->setText(QApplication::translate("cSearchForm", "OR", 0, QApplication::UnicodeUTF8));
	andRadio->setToolTip(QApplication::translate("cSearchForm", "Join all the search fields with and AND", 0, QApplication::UnicodeUTF8));
	andRadio->setText(QApplication::translate("cSearchForm", "AND", 0, QApplication::UnicodeUTF8));
	fieldsTitle->setText(QApplication::translate("cSearchForm", "Search Fields", 0, QApplication::UnicodeUTF8));
} // retranslateUi
