 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapFormBuilder.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *		  : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 custom widget that will display a mTable view 
 *                  in the one tab and a edit form on the other tab.
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

// Include Global Headers
#include <QMessageBox>
#include <QTabWidget>

// Include Local Headers
#include "Config.h"
#include "cRapFormBuilder.h"
#include "RapDbCommunicator.h"
#include "cAntennaFilesImporter.h"

using namespace std;
using namespace Qrap;

//*************************************************************************
// Constructor that creates all the GuiEngine classes for each database mTable
cRapFormBuilder::cRapFormBuilder (QWidget* parent, QString tableName, QTableWidget* tableWidget, bool insert) : QWidget(parent)
{
	cout << "Entering cRapFormBuilder::cRapFormBuilder" << endl;
	mFindNewHeight = false;
	mTableView = tableWidget;
	mTable = tableName;
	mCurrentRecordID=0;
	
	// Create the form grid layout
	mFormLayout = new QGridLayout(this);

//	QString fieldName="Date";
//	mFormWidgets.insert(fieldName,static_cast<QWidget*>(new QDateEdit(this)));

	mButtonRow = mLayoutRow;
	if ((mButtonRow<0)||(mButtonRow>1000))
		mButtonRow=0;
	mButtonRow =0;
	cout << "cRapFormBuilder::cRapFormBuilder; mButtonRow = " << mButtonRow << endl; 
	mUpdateButton = new QPushButton("&Update",this);
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);
	mUpdate=false;
	mFormLayout->addWidget(mUpdateButton,mButtonRow,0,1,1,Qt::AlignLeft);
	connect(mUpdateButton,SIGNAL(clicked()),this,SLOT(Update()));

	mNextButton = new QPushButton(QIcon(":images/next.png"),"",this);
	mNextButton->setToolTip("Next Record");
	mNextButton->setVisible(false);
	mNextButton->setEnabled(false);
	mFormLayout->addWidget(mNextButton,mButtonRow,2,1,1,Qt::AlignLeft);
	connect(mNextButton,SIGNAL(clicked()),this,SLOT(NextRecord()));
	
	mPreviousButton = new QPushButton(QIcon(":images/previous.png"),"",this);
	mPreviousButton->setToolTip("Previous Record");
	mPreviousButton->setVisible(false);
	mPreviousButton->setEnabled(false);
	mFormLayout->addWidget(mPreviousButton,mButtonRow,1,1,1,Qt::AlignRight);
	connect(mPreviousButton,SIGNAL(clicked()),this,SLOT(PreviousRecord()));
	
	mCommitButton = new QPushButton("Comm&it",this);
	mCommitButton->setToolTip("Commit the new entry to the database");
	mFormLayout->addWidget(mCommitButton,mButtonRow,0,1,1,Qt::AlignRight);
	connect(mCommitButton,SIGNAL(clicked()),this,SLOT(Insert()));
	mCommitButton->setVisible(false); 
	mCommitButton->setEnabled(false);

	mCommitAddNextButton = new QPushButton("Add N&ext",this);
	mCommitAddNextButton->setToolTip("Commit the new entry to the database and add another entry.");
	mCommitAddNextButton->setVisible(false);
	mCommitAddNextButton->setEnabled(false);
	mFormLayout->addWidget(mCommitAddNextButton,mButtonRow,1,1,1,Qt::AlignLeft);
	connect(mCommitAddNextButton,SIGNAL(clicked()),this,SLOT(InsertNext()));

	mCloseByButton = new QPushButton("Search nearby &Sites",this);
	mCloseByButton->setToolTip("Search for sites nearby to ensure that one is not re-defining a site.");
	mCloseByButton->setVisible(false);
	mCloseByButton->setEnabled(false);
	mFormLayout->addWidget(mCloseByButton,7,1,1,1,Qt::AlignLeft);
	connect(mCloseByButton,SIGNAL(clicked()),this,SLOT(CloseBySite()));

        mCalculateRxLossesButton = new QPushButton("Calculate &Rx Losses",this);
	mCalculateRxLossesButton->setToolTip("Calculate the rx losses.");
	mCalculateRxLossesButton->setVisible(false);
	mCalculateRxLossesButton->setEnabled(false);
	mFormLayout->addWidget(mCalculateRxLossesButton,15,3,1,1,Qt::AlignRight);
	connect(mCalculateRxLossesButton,SIGNAL(clicked()),this,SLOT(CalculateRXLosses()));
        
        mCalculateTxLossesButton = new QPushButton("Calculate &Tx Losses",this);
	mCalculateTxLossesButton->setToolTip("Calculate the tx losses.");
	mCalculateTxLossesButton->setVisible(false);
	mCalculateTxLossesButton->setEnabled(false);
	mFormLayout->addWidget(mCalculateTxLossesButton,9,3,1,1,Qt::AlignRight);
	connect(mCalculateTxLossesButton,SIGNAL(clicked()),this,SLOT(CalculateTXLosses()));

	mGroundHeightButton = new QPushButton("&Lookup",this);
	mGroundHeightButton->setToolTip("Search the Digital Elevation Model raster files for the ground height of the site.");
	mGroundHeightButton->setVisible(false);
	mGroundHeightButton->setEnabled(false);
	mUpdate=false;
	mFormLayout->addWidget(mGroundHeightButton,9,2,1,1,Qt::AlignLeft);
	connect(mGroundHeightButton,SIGNAL(clicked()),this,SLOT(FindGroundHeight()));

	mDefaultRadiosButton = new QPushButton("Add &Default Radio Installations",this);
	mDefaultRadiosButton->setToolTip("Add the default radioinstallation(s) for the current default technology to the site.");
	mDefaultRadiosButton->setVisible(false);
	mDefaultRadiosButton->setEnabled(false);
	mUpdate=false;
	mFormLayout->addWidget(mDefaultRadiosButton,10,1,1,1,Qt::AlignLeft);
	connect(mDefaultRadiosButton,SIGNAL(clicked()),this,SLOT(InsertDefaultRadios()));

	mButtonPosX0 = mUpdateButton->pos().x();
	mButtonPosX1 = mPreviousButton->pos().x();
	mButtonPosX2 = mNextButton->pos().x();
	mButtonPosY = mUpdateButton->pos().y();
	if (mButtonRow)
		mLayoutDelta = mButtonPosY/mButtonRow;
	else mLayoutDelta = mButtonPosY;

	if (mTable=="site")
	{
		mCloseByButton->setVisible(true);
		mCloseByButton->setEnabled(true);
		mGroundHeightButton->setVisible(true);
		mGroundHeightButton->setEnabled(true);
		mDefaultRadiosButton->setVisible(true);
		mDefaultRadiosButton->setEnabled(true);
	}

        if (mTable == "radioinstallation")
        {
               mCalculateRxLossesButton->setVisible(true);
               mCalculateRxLossesButton->setEnabled(true);
               mCalculateTxLossesButton->setVisible(true);
               mCalculateTxLossesButton->setEnabled(true);
        }

	// Get the database structure
	gDb.GetDbStructure(mDbs);
	
	cout << "cRapFormBuilder::cRapFormBuilder Voor rowCount:"<< mTableView->rowCount() << endl;
	if(mTableView->rowCount()<=0)
		insert=true;
	mLayoutRow++;	

	if(insert)
	{
		// Get the default values for the mTable
		cout << "cRapFormBuilder::cRapFormBuilder insert is TRUE" << endl;
		if(!gDb.GetDefaults(mTable.toStdString(),mDefaults))
		{
			cout << "Could not load the mDefaults." << endl;
			return;
		} // if
		
		cout << "cRapFormBuilder::cRapFormBuilder Voor GenericInsert" << endl;
		if(mTable=="antennadevice")
			CreateAntennaDeviceForm();
		else
			CreateGenericInsertForm();
	} // if insert
	else
	{
		cout << "cRapFormBuilder::cRapFormBuilder insert is FALSE" << endl;
		cout << "cRapFormBuilder::cRapFormBuilder Voor GenericUpdate" << endl;
		CreateGenericUpdateForm();
		cout << "cRapFormBuilder::cRapFormBuilder Na GenericUpdate" << endl;
	} // else insert
	
}

//**********************************************************************
cRapFormBuilder::cRapFormBuilder (StringMap ref, QWidget* parent,
				QString tableName,
				QTableWidget* tableWidget,
			        bool insert)
{
	mReferences = ref;
	mTableView = tableWidget;
	mTable = tableName;
	mCurrentRecordID=0;
	mFindNewHeight = false;
	
	// Create the form grid layout
	mFormLayout = new QGridLayout(this);

//	QString fieldName="Date";
//	mFormWidgets.insert(fieldName,static_cast<QWidget*>(new QDateEdit(this)));
	mButtonRow = mLayoutRow;
	if ((mButtonRow<0)||(mButtonRow>1000))
		mButtonRow=0;	
	mButtonRow =0;
	cout << "cRapFormBuilder::cRapFormBuilder for table; mButtonRow = " << mButtonRow << endl;
	// make sure that there is an mUpdate button
	mUpdateButton = new QPushButton("&Update",this);
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);
	mUpdate=false;
	mFormLayout->addWidget(mUpdateButton,mButtonRow,0,1,1,Qt::AlignLeft);
	connect(mUpdateButton,SIGNAL(clicked()),this,SLOT(Update()));

	mNextButton = new QPushButton(QIcon(":images/next.png"),"",this);
	mNextButton->setToolTip("Next Record");
	mNextButton->setVisible(false);
	mNextButton->setEnabled(false);
	mFormLayout->addWidget(mNextButton,mButtonRow,2,1,1,Qt::AlignLeft);
	connect(mNextButton,SIGNAL(clicked()),this,SLOT(NextRecord()));

	mPreviousButton = new QPushButton(QIcon(":images/previous.png"),"",this);
	mPreviousButton->setToolTip("Previous Record");
	mPreviousButton->setVisible(false);
	mPreviousButton->setEnabled(false);
	mFormLayout->addWidget(mPreviousButton,mButtonRow,1,1,1,Qt::AlignRight);
	connect(mPreviousButton,SIGNAL(clicked()),this,SLOT(PreviousRecord()));

	mCommitButton = new QPushButton("Comm&it",this);
	mCommitButton->setToolTip("Commit the new entry to the database");
	mFormLayout->addWidget(mCommitButton,mButtonRow,0,1,1,Qt::AlignRight);
	connect(mCommitButton,SIGNAL(clicked()),this,SLOT(Insert()));
	mCommitButton->setVisible(false); 
	mCommitButton->setEnabled(false);

	mCommitAddNextButton = new QPushButton("Add N&ext",this);
	mCommitAddNextButton->setToolTip("Commit the new entry to the database and add another entry.");
	mCommitAddNextButton->setVisible(false);
	mCommitAddNextButton->setEnabled(false);
	mFormLayout->addWidget(mCommitAddNextButton,mButtonRow,1,1,1,Qt::AlignLeft);
	connect(mCommitAddNextButton,SIGNAL(clicked()),this,SLOT(InsertNext()));

	mCloseByButton = new QPushButton("Search nearby &Sites",this);
	mCloseByButton->setToolTip("Search for sites nearby to ensure that one is not re-defining a site.");
	mCloseByButton->setVisible(false);
	mCloseByButton->setEnabled(false);
	mFormLayout->addWidget(mCloseByButton,7,1,1,1,Qt::AlignLeft);
	connect(mCloseByButton,SIGNAL(clicked()),this,SLOT(CloseBySite()));

	mGroundHeightButton = new QPushButton("&Lookup",this);
	mGroundHeightButton->setToolTip("Search the Digital Elevation Model raster files for the ground height of the site.");
	mGroundHeightButton->setVisible(false);
	mGroundHeightButton->setEnabled(false);
	mFormLayout->addWidget(mGroundHeightButton,9,2,1,1,Qt::AlignLeft);
	connect(mGroundHeightButton,SIGNAL(clicked()),this,SLOT(FindGroundHeight()));

	mDefaultRadiosButton = new QPushButton("Add &Default Radio Installations",this);
	mDefaultRadiosButton->setToolTip("Add the default radioinstallation(s) for the current default technology to the site.");
	mDefaultRadiosButton->setVisible(false);
	mDefaultRadiosButton->setEnabled(false);
	mUpdate=false;
	mFormLayout->addWidget(mDefaultRadiosButton,10,1,1,1,Qt::AlignLeft);
	connect(mDefaultRadiosButton,SIGNAL(clicked()),this,SLOT(InsertDefaultRadios()));

	mButtonPosX0 = mUpdateButton->pos().x();
	mButtonPosX1 = mPreviousButton->pos().x();
	mButtonPosX2 = mNextButton->pos().x();
	mButtonPosY = mUpdateButton->pos().y();
	if (mButtonRow)
		mLayoutDelta = mButtonPosY/mButtonRow;
	else mLayoutDelta = mButtonPosY;

	if (mTable=="site")
	{
		mCloseByButton->setVisible(true);
		mCloseByButton->setEnabled(true);
		mGroundHeightButton->setVisible(true);
		mGroundHeightButton->setEnabled(true);
		mDefaultRadiosButton->setVisible(true);
		mDefaultRadiosButton->setEnabled(true);
	}

	// Get the database structure
	gDb.GetDbStructure(mDbs);
	
	cout << "cRapFormBuilder::cRapFormBuilder Voor rowCount:"<< mTableView->rowCount() << endl;
	if(mTableView->rowCount()<=0)
		insert=true;
	mLayoutRow++;	
	if(insert)
	{

		cout << "cRapFormBuilder::cRapFormBuilder overloaded insert is TRUE" << endl;	
		// Get the default values for the mTable
		if(!gDb.GetDefaults(mTable.toStdString(),mDefaults))
		{
			cout << "Could not load the mDefaults." << endl;
			return;
		} // if
		
		if(mTable=="antennadevice")
			CreateAntennaDeviceForm();
		else
			CreateGenericInsertForm();
	} // if insert
	else
	{
		cout << "cRapFormBuilder::cRapFormBuilder overloaded insert is FALSE" << endl;
		CreateGenericUpdateForm();
	} // else insert
}


//***********************************************************************
void cRapFormBuilder::TableItemSelectionChanged ()
{
	cout << "Running empty cRapFormBuilder::TableItemSelectionChanged ()" << endl;
}

//****************************************************************************
// Perform the insert query
int cRapFormBuilder::InsertData (const QString& tableName, QMap<QString,QWidget*> &widgets, 
					bool antennaDevice, int antdevicekey)
{
	QMapIterator<QString,QWidget*> it(widgets);
	QMap<QString,QVariant> tableViewValues;
	StringMap values;
	string where="";
	char *text = new char[33];
	int Int;
	cout<<"cRapFormBuilder::InsertData ... Data insert begin"<<endl;	
	while(it.hasNext())
	{
		it.next();
		QString fieldName = it.key();
//		cout << "fieldName = " << fieldName.toStdString().c_str() << endl;
		// Check if the datawidget does not belong to to a point type
		if (fieldName.contains(":"))
		{
			QString format;
			QString latitude;
			QString longitude;
			QString latInput;
			QString longInput;
			QStringList parts = fieldName.split(":");
			
			// if the degrees minutes seconds radio is checked then do the relevant conversion
			if( static_cast<QRadioButton*>(it.value())->isChecked() )
			{
				format = "DDMMSS";
				mDegreeFormat = dfDegMinSec;
				it.next();
				it.next();
				it.next();
				
				// Get the latitude value
				latInput = static_cast<QLineEdit*>(it.value())->text();

				mLat = GetDecimalDegrees(latInput.toStdString(),dfDegMinSec,true);
				if (mLat==0.0)
					return -1;
				else
				{
					char   *ret;
					ret = new char[33];
					sprintf(ret, "%.6lf", mLat);
					string ReturnValue = string(ret);
					latitude = QString::fromStdString(ReturnValue);
					delete [] ret;
				}
				
				// Get the longitude value
				it.next();
				longInput = static_cast<QLineEdit*>(it.value())->text();
				mLon = GetDecimalDegrees(longInput.toStdString(),dfDegMinSec,false);
				if (mLon==0.0)
					return -1;
				else
				{
					char   *ret;
					ret = new char[33];
					sprintf(ret, "%.6lf", mLon);
					string ReturnValue = string(ret);
					longitude = QString::fromStdString(ReturnValue);
					delete [] ret;		
				}
				
				// Get the value to send to the mTable view
				tableViewValues.insert(parts[0],QVariant(latInput + " | " + longInput));
			} // if DDMMSS
			else
			{
				it.next();
				// if the degrees minutes decimals radio button is checked then do the relevant conversion
				if(static_cast<QRadioButton*>(it.value())->isChecked())
				{
					format = "DDMMmm";
					mDegreeFormat = dfDegMin;
					it.next();
					it.next();
					
					latInput = static_cast<QLineEdit*>(it.value())->text();

					mLat = GetDecimalDegrees(latInput.toStdString(),dfDegMin,true);
					if (mLat==0.0)
						return -1;
					else
					{
						char   *ret;
						ret = new char[33];
						sprintf(ret, "%.6lf", mLat);
						string ReturnValue = string(ret);
						latitude = QString::fromStdString(ReturnValue);
						delete [] ret;
					}
					
					// Get the longitude value
					it.next();
					longInput = static_cast<QLineEdit*>(it.value())->text();
					mLon = GetDecimalDegrees(longInput.toStdString(),dfDegMin,false);
					if (mLon==0.0)
						return -1;
					else
					{
						char   *ret;
						ret = new char[33];
						sprintf(ret, "%.6lf", mLon);
						string ReturnValue = string(ret);
						longitude = QString::fromStdString(ReturnValue);
						delete [] ret;		
					}
					
					// Get the value to send to the mTable view
					tableViewValues.insert(parts[0],QVariant(latInput + " | " + longInput));
				} // if DDMMmm
				else
				{
					it.next();
					// if the Degrees decimals radio is checked then do the relevant conversion
					if(static_cast<QRadioButton*>(it.value())->isChecked())
					{
						format = "DDdddd";
						mDegreeFormat = dfDeg;
						it.next();
						latitude = static_cast<QLineEdit*>(it.value())->text();
						it.next();
						longitude = static_cast<QLineEdit*>(it.value())->text();
						mLon = GetDecimalDegrees(longInput.toStdString(),dfDeg,false);
						mLat = GetDecimalDegrees(latInput.toStdString(),dfDeg,true);

						// Check if the format of the degrees decimals is correct
						if(!latitude.contains(QRegExp("^[-+ {0,2}[0-9]{1,4}[.]{0,1}[0-9]{0,8}[ ]{0,1}$")) )
						{
							QRAP_ERROR_CODE("Incorrect latitude format.", acInvalidInputFormat);
							return -1;
						} // if latitude contains
						
						if(!longitude.contains(QRegExp("^[-+ ]{0,3}[0-9]{1,4}[.]{0,1}[0-9]{0,8}[ ]{0,1}$")) )
						{
							QRAP_ERROR_CODE("Incorrect longitude format.", acInvalidInputFormat);
							return -1;
						} // if longitude contains
						
						// Get the value to send to the mTable view
						tableViewValues.insert(parts[0],QVariant(latitude + " | " + longitude));
					} // if DDdddd
					else
					{
						format = "";
						mLat = 0.0;
						mLon = 0.0;
						QRAP_ERROR("No location format specified. Choose either,\n\nDD:MM:SS X\nDD:MM.mm X\n\261DD.dddd");
						return -1;
					} // if DDdddd
				} // else DDMMmm
			} // else DDMMSS
			values[parts[0].toStdString()] = "POINT("  +  longitude.toStdString() + " " + latitude.toStdString() + ")";
			if (mTable=="site") 
				DeleteBTL(mCurrentRecordID);
		} // if contains
		else
		{
			if(fieldName=="azimuth"||fieldName=="elevation"||fieldName=="envelopes")
			{
				QTableWidget* valuesTable = static_cast<QTableWidget*>(widgets[fieldName]);
				string angles = "'{";
				string patternValues = "'{";
				
				for( int row=0; row<valuesTable->rowCount(); row++)
				{
					angles += valuesTable->item(row,0)->data(Qt::DisplayRole).toString().toStdString();
					patternValues += valuesTable->item(row,1)->data(Qt::DisplayRole).toString().toStdString();
					
					if(row!=valuesTable->rowCount()-1)
					{
						angles += ",";
						patternValues += ",";
					} // if row
				} // for row
				
				angles += "}'";
				patternValues += "}'";
				
				QString anglesColumn = valuesTable->horizontalHeaderItem(0)->data(Qt::UserRole).toString();
				QString valuesColumn = valuesTable->horizontalHeaderItem(1)->data(Qt::UserRole).toString();
				
				values[anglesColumn.toStdString()] = angles;
				values[valuesColumn.toStdString()] = patternValues;
				
				tableViewValues.insert(anglesColumn,QVariant(0));
				tableViewValues.insert(valuesColumn,QVariant(0));
			} // if azimuth||elevation
			else
			{
				cDatabase::FieldDataType type;
				cDatabase::FieldUiType uiType;
				UnitType unit;
				double doubleVal;
				
				// Get the field data type
				if(mDbs[tableName.toStdString()].mFields.count(fieldName.toStdString())>0)
				{
					uiType = gDb.GetFieldUiType(tableName.toStdString(),fieldName.toStdString());
					type = gDb.GetFieldDataType(tableName.toStdString(),fieldName.toStdString());
				}
				else
				{
					uiType = cDatabase::utNormal;
					type = cDatabase::dtString;
				}
				
				cout << "cRapFormBuilder::InsertData	type = "<< type << endl;
				if(uiType == cDatabase::utNormal)
				{
					switch(type)
					{
						case cDatabase::dtPostGisPoint:
//							cout << "Point" << endl;
							break;
						case cDatabase::dtPostGisLineString:
//							cout << "Line" << endl;
							break;
						case cDatabase::dtPostGisPolygon:
//							cout << "Polygon" << endl;
							break;
						case cDatabase::dtInteger:
							Int = static_cast<QSpinBox*>(it.value())->value();
							gcvt(Int, 8, text);
							values[fieldName.toStdString()] = text;
							if(fieldName=="id")
								mCurrentRecordID=(int)(static_cast<QSpinBox*>(it.value())->value());
							// Get the value to send to the mTableView
							if(it.value()->isEnabled())
								tableViewValues.insert(fieldName,QVariant(static_cast<QSpinBox*>(it.value())->value()));

							break;
						case cDatabase::dtReal:
							// Get the unit type for the current field
							if(mDbs[tableName.toStdString()].mFields.count(fieldName.toStdString())>0)
								unit = mDbs[tableName.toStdString()].mFields[fieldName.toStdString()].mUnitType;
							else
								unit = utNone;
							
							doubleVal = static_cast<QDoubleSpinBox*>(it.value())->value();
							
							// Get the SQL queries that will handle the correct unit conversions.
							if(unit!=utNone)
							{
								switch (unit)
								{
									case utSensitivity:
										values[fieldName.toStdString()] = RapDbCommunicator::ConvertTodBm(unit,doubleVal);
										break;
									case utPower:
										values[fieldName.toStdString()] = RapDbCommunicator::ConvertTodBm(unit,doubleVal);
										break;
									case utEirp:
										values[fieldName.toStdString()] = RapDbCommunicator::ConvertTodBm(unit,doubleVal);
										break;
									default:
										gcvt(doubleVal, 8, text);
										values[fieldName.toStdString()] = text;
								}//end switch
							}//end if unit
							else
							{
								gcvt(doubleVal, 8, text);
								values[fieldName.toStdString()] = text;
							}
							
							// Get the data to send to the mTable view
							tableViewValues.insert(fieldName,QVariant(doubleVal));
							break;
						case cDatabase::dtDate:
							values[fieldName.toStdString()] = "'" + static_cast<QDateEdit*>(it.value())->date().toString("yyyy-MM-dd").toStdString() + "'";
							tableViewValues.insert(fieldName,QVariant(static_cast<QDateEdit*>(it.value())->date()));
							break;
						case cDatabase::dtDateTime:
							values[fieldName.toStdString()] = "'" + static_cast<QDateTimeEdit*>(it.value())->dateTime().toString("yyyy-MM-dd hh:mm:ss.z").toStdString() + "'";
							
							// Get the data to send to the mTable view
							tableViewValues.insert(fieldName,QVariant(static_cast<QDateEdit*>(it.value())->date()));
							break;
						default:
							if(it.value()->isEnabled())
							{
								values[fieldName.toStdString()] = "'" + static_cast<QLineEdit*>(it.value())->text().toStdString() + "'";
								
								// Get the data to send to the mTable view
								tableViewValues.insert(fieldName,QVariant(static_cast<QLineEdit*>(it.value())->text()));
							}
							break;
					} // switch
				} // if uiType
				else
				{
					QStringList parts;
					parts = static_cast<QComboBox*>(it.value())->currentText().split(":");
					
					if(parts.size()>1)
					{
						if(uiType==cDatabase::utDropdownFixed)
							values[fieldName.toStdString()] = "'" + parts[1].toStdString() + "'";
						else
						{
							if(parts[1]=="NULL")
								values[fieldName.toStdString()] = "NULL";
							else
								values[fieldName.toStdString()] =  parts[0].toStdString();
						}
						
						// Get the data to send to the mTable view
						tableViewValues.insert(fieldName,QVariant(static_cast<QComboBox*>(it.value())->currentText()));
					}
					else
						values[fieldName.toStdString()] = "NULL";
				} // else uiType
			} // else azimuth || elevation
		} // else key
	} // while
	
	int newId;
	
	// Add the antdevicekey foreign key to the fields in the case of an antennapattern entry in conjunction to the antennadevice insert
	if(antennaDevice==true && antdevicekey!=-1)
	{
		gcvt(antdevicekey, 8, text);
		values["antdevicekey"] = text;
	}
	
	// Insert the data into the database
	if(gDb.GetSetting("mode")=="normal")
	{
		if(!mUpdate)
		{
			if( (newId=gDb.Insert(values,tableName.toStdString()))==-1 )
				return -1;
			else mCurrentRecordID=newId;
		}
		else
		{
			gcvt(mCurrentRecordID, 8, text);
			where = "id=";
			where += text;
			if(!gDb.Update(values,tableName.toStdString(), where))
			{
				return -1;
				mUpdate=false;
			}
		}
	} // if mode
	else
	{
//		cout << "Set the mDefaults" << endl;
		if(!gDb.SetDefaults(tableName.toStdString(),values))
			return -1;
		
		newId = 0;
		mCurrentRecordID=newId;
	} // else mode
	
	cout <<"cRapFormBuilder::InsertData voor mTableView stuff" << endl;
	// Synchronise the changes made in the form with the mTable view
	if(antennaDevice==false)
	{
		// Make sure that no mUpdate signals are triggered
		disconnect(mTableView,SIGNAL(cellChanged(int,int)),0,0);
		mTableView->clearSelection();
		if (mTableView->rowCount()<=0) 
			mUpdate=false;
		if(!mUpdate)
		{
			cout <<"cRapFormBuilder::InsertData NOT mUpdate" << endl;
			mTableView->setSortingEnabled(false);
			mTableView->setRowCount(mTableView->rowCount()+1);
			if (mTableView->currentRow()<0)
				mTableView->selectRow(0);
			if (mTableView->currentRow()>=mTableView->rowCount())
				mTableView->selectRow(mTableView->rowCount()-1);
			int size = mTableView->columnCount();
			
			for(int i=0; i<size ; i++)
			{
				QTableWidgetItem* item = new QTableWidgetItem;
				
				if(i==0)
					item->setData(Qt::DisplayRole,QVariant(newId));
				else
				{
					QString fieldName = mTableView->horizontalHeaderItem(i)->data(Qt::UserRole).toString();
					if(tableViewValues.contains(fieldName))
					{
						item->setData(Qt::DisplayRole,tableViewValues[fieldName]);
					} // if tableViewValues.contains
				} // if i==0
				
				mTableView->setItem(mTableView->rowCount()-1,i,item);
			} // for
			
			mTableView->selectRow(mTableView->rowCount()-1);
			mTableView->setSortingEnabled(true);
		} // if mUpdate
		else
		{
//			cout <<"cRapFormBuilder::InsertData mUpdate" << endl;
//			cout <<"cRapFormBuilder::InsertData size = " << mTableView->columnCount()<< endl;
//			cout <<"cRapFormBuilder::InsertData mTableView->currentRow() = " << mTableView->currentRow()<< endl;
//			cout <<"cRapFormBuilder::InsertData mTableView->rowCount() = " << mTableView->rowCount()<< endl;
			int size = mTableView->columnCount();
			if (mTableView->currentRow()<0)
				mTableView->selectRow(0);
			if (mTableView->currentRow()>=mTableView->rowCount())
				mTableView->selectRow(mTableView->rowCount()-1);
//			cout <<"cRapFormBuilder::InsertData mTableView->currentRow() = " << mTableView->currentRow()<< endl;
			for(int i=1; i<size ; i++)
			{
//				cout <<"cRapFormBuilder::InsertData 	i = " << i << endl;
				QString fieldName = mTableView->horizontalHeaderItem(i)->data(Qt::UserRole).toString();
//				cout <<"cRapFormBuilder::InsertData 	" << i << "	" << fieldName.toStdString().c_str()<< endl;
				if(tableViewValues.contains(fieldName))
				{
					mTableView->item(mTableView->currentRow(),i)->setData(Qt::DisplayRole,tableViewValues[fieldName]);
				} // if tableViewValues.contains
			} // for
		} // else mUpdate
	} // if antennaDevice
	delete [] text;
	mFindNewHeight = false;
/*	cout << "cRapFormBuilder::InsertData 3; mButtonRow = " << mButtonRow << endl; 
	mUpdateButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitAddNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX2);
	mPreviousButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mButtonPosY = mUpdateButton->pos().y();
	if (mButtonRow)
		mLayoutDelta = mButtonPosY/mButtonRow;
	else mLayoutDelta = mButtonPosY;
*/	cout<<"Data insert completed"<<endl;
	return newId;
}


//***********************************************************************
// Populate the forms widgets with new data from the mTable view
void cRapFormBuilder::PopulateForm ()
{
	cout << "Entering cRapFormBuilder::PopulateForm ()" << endl;
	int size = mTableView->columnCount();
	QString fieldName;
	cDatabase::FieldUiType uiType;
	cDatabase::FieldDataType type;
	QVariant val;
	string query="";
	char *text = new char[33];
	bool rowReadOnly = false;
	int id=0;
	cout << "cRapFormBuilder::PopulateForm (): mTableView->currentRow() = " << mTableView->currentRow()<< endl;
	if ((mTableView->currentRow()<0)||(mTableView->currentRow() > mTableView->rowCount()-1)) 
		mTableView->setCurrentCell(0,0);
	cout << "cRapFormBuilder::PopulateForm (): 2 mTableView->currentRow() = " << mTableView->currentRow()<< endl;
	for(int i=0; i<size ; i++)
	{
		QTableWidgetItem* item = mTableView->item(mTableView->currentRow(),i);
		fieldName = mTableView->horizontalHeaderItem(i)->data(Qt::UserRole).toString();
//		cout << "cRapFormBuilder::PopulateForm (): " << fieldName.toStdString() << endl;
		val = item->data(Qt::DisplayRole);
		bool readOnly = false;
		
		if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
		{
			type = gDb.GetFieldDataType(mTable.toStdString(),fieldName.toStdString());
			uiType = gDb.GetFieldUiType(mTable.toStdString(),fieldName.toStdString());
			
			if(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mReadOnly 
				|| item->flags()==Qt::ItemIsSelectable)
				readOnly = true;
			else
				rowReadOnly = false;
		}
		else
		{
			uiType = cDatabase::utNormal;
			type = cDatabase::dtString;
			readOnly = true;
		}
		
		QStringList location;
		
		// Get the id of the row
		if(fieldName=="id")
		{
			id = item->data(Qt::DisplayRole).toInt();
			mCurrentRecordID=id;
		}
		
		if(fieldName.contains("azimuthangles")||fieldName.contains("elevationangles")
			||fieldName.contains("azimuthpattern")||fieldName.contains("elevationpattern")
			||fieldName.contains("offsets")||fieldName.contains("values"))
		{
			// Create one QTablewidget for the azimuth angles and values
			if(fieldName=="azimuthangles")
			{
				disconnect(mAzimuthTable,SIGNAL(cellChanged(int,int)),0,0);
				mAzimuthTable->setRowCount(0);
				
				// Perform a select query to get the values for the angles and patterns
				query = "select azimuthangles,azimuthpattern from antennapattern where id = ";
				gcvt(id,8,text);
				query += text;
				query += ";";
				if(gDb.PerformRawSql(query))
				{
					pqxx::result r;
					
					gDb.GetLastResult(r);
					
					if(r.size()>0)
					{
						QString temp =  r[0]["azimuthangles"].c_str();
						temp.remove("{");
						temp.remove("}");
						
						QStringList angles = temp.split(",");
						
						temp = r[0]["azimuthpattern"].c_str();
						temp.remove("{");
						temp.remove("}");
						QStringList patternValues = temp.split(",");
						
						if(angles.size()>0)
						{
							for(int i=0;i<angles.size();i++)
							{
								mAzimuthTable->setRowCount(i+1);
								QTableWidgetItem* item = new QTableWidgetItem(angles[i]);
								mAzimuthTable->setItem(i,0,item);
							} // for
						} // if angles.size
						
						if(patternValues.size()>0)
						{
							for(int i=0; i<patternValues.size(); i++)
							{
								if(i>mAzimuthTable->rowCount()-1)
									mAzimuthTable->setRowCount(i+1);
								
								QTableWidgetItem* item = new QTableWidgetItem(patternValues[i]);
								mAzimuthTable->setItem(i,1,item);
							} // for
						} // if patternValues.size
						
					} // if r
				} // if PerformRawSql
				else
				{
					mAzimuthTable->setRowCount(1);
					QTableWidgetItem* first = new QTableWidgetItem("0");
					QTableWidgetItem* second = new QTableWidgetItem("0");
					mAzimuthTable->setItem(0,0,first);
					mAzimuthTable->setItem(0,1,second);
				} // else PerformRawSql
				
				connect(mAzimuthTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateAzimuthRow(int,int)));
			} // if azimuthangles
			else
			{
				if(fieldName=="elevationangles")
				{
					disconnect(mElevationTable,SIGNAL(cellChanged(int,int)),0,0);
					mElevationTable->setRowCount(0);
					
					// Perform a select query to get the values for the angles and patterns
					query = "select elevationangles,elevationpattern from antennapattern where id = ";
					gcvt(id,8,text);
					query += id;
					query +=";";
					if(gDb.PerformRawSql(query))
					{
						pqxx::result r;
						
						gDb.GetLastResult(r);
						
						if(r.size()>0)
						{
							QString temp =  r[0]["elevationangles"].c_str();
							temp.remove("{");
							temp.remove("}");
							
							QStringList angles = temp.split(",");
							
							temp = r[0]["elevationpattern"].c_str();
							temp.remove("{");
							temp.remove("}");
							QStringList patternValues = temp.split(",");
							
							if(angles.size()>0)
							{
								for(int i=0;i<angles.size();i++)
								{
									mElevationTable->setRowCount(i+1);
									QTableWidgetItem* item = new QTableWidgetItem(angles[i]);
									mElevationTable->setItem(i,0,item);
								} // for
							} // if angles.size
							
							if(patternValues.size()>0)
							{
								for(int i=0; i<patternValues.size(); i++)
								{
									if(i>mElevationTable->rowCount()-1)
										mElevationTable->setRowCount(i+1);
									
									QTableWidgetItem* item = new QTableWidgetItem(patternValues[i]);
									mElevationTable->setItem(i,1,item);
								} // for
							} // if patternValues.size
							
						} // if r
					} // if PerformRawSql
					else
					{
						mElevationTable->setRowCount(1);
						QTableWidgetItem* first = new QTableWidgetItem("0");
						QTableWidgetItem* second = new QTableWidgetItem("0");
						mElevationTable->setItem(0,0,first);
						mElevationTable->setItem(0,1,second);
					} // else PerformRawSql
					
					connect(mElevationTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateElevationRow(int,int)));
				} // if elevationangles
				else
				{
					if(fieldName=="offsets")
					{
						disconnect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),0,0);
						mEnvelopeTable->setRowCount(0);
					
						// Perform a select query to get the values for the angles and patterns
						query = "select offsets,values from envelopes where id = ";
						gcvt(id,8,text);
						query+=text;
						query+=";";
						if(gDb.PerformRawSql(query))
						{
							pqxx::result r;
						
							gDb.GetLastResult(r);
						
							if(r.size()>0)
							{
								QString temp =  r[0]["offsets"].c_str();
								temp.remove("{");
								temp.remove("}");
							
								QStringList offsets = temp.split(",");
							
								temp = r[0]["values"].c_str();
								temp.remove("{");
								temp.remove("}");
								QStringList values = temp.split(",");
							
								if(offsets.size()>0)
								{
									for(int i=0;i<offsets.size();i++)
									{
										mEnvelopeTable->setRowCount(i+1);
										QTableWidgetItem* item = new QTableWidgetItem(offsets[i]);
										mEnvelopeTable->setItem(i,0,item);
									} // for
								} // if angles.size
							
								if(values.size()>0)
								{
									for(int i=0; i<values.size(); i++)
									{
										if(i>mEnvelopeTable->rowCount()-1)
											mEnvelopeTable->setRowCount(i+1);
									
										QTableWidgetItem* item = new QTableWidgetItem(values[i]);
										mEnvelopeTable->setItem(i,1,item);
									} // for
								} // if patternValues.size
							
							} // if r
						} // if PerformRawSql
						else
						{
							mEnvelopeTable->setRowCount(1);
							QTableWidgetItem* first = new QTableWidgetItem("0");
							QTableWidgetItem* second = new QTableWidgetItem("0");
							mEnvelopeTable->setItem(0,0,first);
							mEnvelopeTable->setItem(0,1,second);
						} // else PerformRawSql
					
						connect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateEnvelopeRow(int,int)));
					} // if offsets
				} // else elevationangles
			} // else azimuthangles
		} // if fieldName
		else
		{
			if(uiType == cDatabase::utNormal)
			{
				switch (type)
				{
					case cDatabase::dtPostGisPoint:
						location = val.toString().split("|");
						
						if(location.count()>1)
						{
							mLat = GetDecimalDegrees(location[0].toStdString(),mDegreeFormat,mDegreeFormat!=dfDeg, true);
							static_cast<QLineEdit*>(mFormWidgets[fieldName+":latitude"])->setText(location[0]);
							mLon = GetDecimalDegrees(location[1].toStdString(),mDegreeFormat,mDegreeFormat!=dfDeg, false);
							static_cast<QLineEdit*>(mFormWidgets[fieldName+":longitude"])->setText(location[1]);
						} // if location
						else
						{
							mLat=0.0;
							mLon=0.0;
							static_cast<QLineEdit*>(mFormWidgets[fieldName+":latitude"])->setText("");
							static_cast<QLineEdit*>(mFormWidgets[fieldName+":longitude"])->setText("");
						} // else location
						break;
					case cDatabase::dtString:
						static_cast<QLineEdit*>(mFormWidgets[fieldName])->setText(val.toString());
						break;
					case cDatabase::dtInteger:
						static_cast<QSpinBox*>(mFormWidgets[fieldName])->setValue(val.toInt());
						break;
					case cDatabase::dtReal:
						static_cast<QDoubleSpinBox*>(mFormWidgets[fieldName])->setValue(val.toDouble());
						break;
					case cDatabase::dtDateTime:
						static_cast<QDateTimeEdit*>(mFormWidgets[fieldName])->setDateTime(val.toDateTime());
						break;
					case cDatabase::dtDate:
						static_cast<QDateEdit*>(mFormWidgets[fieldName])->setDate(val.toDate());
						break;
					case cDatabase::dtBoolean:
						static_cast<QCheckBox*>(mFormWidgets[fieldName])->setCheckState(val.toBool()?Qt::Checked:Qt::Unchecked);
						break;
					default:
						// Do nothing as default
						break;
				}
				
				
			} // if uiType
			else
			{
				// Set the default value for the different dialogs
				QComboBox* combo = static_cast<QComboBox*>(mFormWidgets[fieldName]);
				combo->setCurrentIndex(combo->findText(val.toString(),Qt::MatchCaseSensitive));
			} // else uiType
			
			
			// enable or disable the widget
			if(type==cDatabase::dtPostGisPoint)
				if(readOnly)
				{
					mFormWidgets[fieldName+":latitude"]->setEnabled(false);
					mFormWidgets[fieldName+":longitude"]->setEnabled(false);
				} 
				else // else readOnly
				{
					mFormWidgets[fieldName+":latitude"]->setEnabled(true);
					mFormWidgets[fieldName+":longitude"]->setEnabled(true);
				} // end else readOnly
			else // else GeoType
				if(readOnly)
					mFormWidgets[fieldName]->setEnabled(false);
			else
				mFormWidgets[fieldName]->setEnabled(true);
		} // end else fieldName
		
	}  // for
	
	// If the entire row is readOnly then make sure that the location dialogs are made readonly
	if(!rowReadOnly)
	{
		QMapIterator<QString,QWidget*> it(mFormWidgets);
		
		while(it.hasNext())
		{
			it.next();
			if(it.key().contains(":latitude"))
				static_cast<QLineEdit*>(it.value())->setEnabled(true);
			else
				if(it.key().contains(":longitude"))
				{
					static_cast<QLineEdit*>(it.value())->setEnabled(true);
					break;
				} // if contains
		} // while
	}
	cout << "cRapFormBuilder::PopulateForm (): 3 mTableView->currentRow() = " << mTableView->currentRow()<< endl;
/*	mUpdateButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitAddNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX2);
	mPreviousButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
*/	delete [] text;
	cout << "Leaving cRapFormBuilder::PopulateForm ()" << endl;
}


//***********************************************************************
void cRapFormBuilder::Insert ()
{
	int TempID = InsertData(mTable, mFormWidgets);
	if (TempID!=-1)
	{
		mCurrentRecordID = TempID;
		cout << "In cRapFormBuilder::Insert ()  mCurrentRecordID = " << mCurrentRecordID << endl;
		mCommitButton->setVisible(false);
		mCommitButton->setEnabled(false);
		mCommitAddNextButton->setVisible(false);
		mCommitAddNextButton->setEnabled(false);
		emit InsertComplete(TempID);
	
//		Clear();
	
		// Create the form grid layout
//		if (mFormLayout != NULL)
//			delete mFormLayout;
//		mFormLayout = new QGridLayout(this);
			
		// make sure that there is an mUpdate button
///		mUpdateButton->move(mButtonRow*mLayoutDelta,0);
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(false);
		mUpdate=false;
		mInserting =false;

//		mNextButton->move(mButtonRow*mLayoutDelta,2*mFormLayout->horizontalSpacing());
		mNextButton->setVisible(true);
		mNextButton->setEnabled(true);
//		mPreviousButton->move(mButtonRow*mLayoutDelta,mFormLayout->horizontalSpacing());
		mPreviousButton->setVisible(true);
		mPreviousButton->setEnabled(true);
//		CreateGenericUpdateForm();
	}
	if (mTable=="site")
	{
		mCloseByButton->setVisible(true);
		mCloseByButton->setEnabled(true);
		mGroundHeightButton->setVisible(true);
		mGroundHeightButton->setEnabled(true);
		mDefaultRadiosButton->setVisible(true);
		mDefaultRadiosButton->setEnabled(true);
	}
        if (mTable == "radioinstallation")
        {
               mCalculateRxLossesButton->setVisible(true);
               mCalculateRxLossesButton->setEnabled(true);
               mCalculateTxLossesButton->setVisible(true);
               mCalculateTxLossesButton->setEnabled(true);
        }
}


//********************************************************************
// Insert the data and reload the default data into the form
void cRapFormBuilder::InsertNext ()
{
	if(InsertData(mTable, mFormWidgets)!=-1)
	{
		Clear();
		
		mCommitButton->close();
		mCommitAddNextButton->close();
		mUpdate =false;
		// Create the new insert form
		CreateGenericInsertForm();

	} // if InsertData
}


//***************************************************************
// Insert the antenna data
void cRapFormBuilder::InsertAntenna ()
{
	mUpdate =false;
	if( (mCurrentRecordID=InsertData(mTable,mFormWidgets))!=-1 )
		if (InsertData("antennapattern",mPatternWidgets,true,mCurrentRecordID)!=-1)
			emit InsertComplete(0);
}


//********************************************************************
// Insert the antenna data and then clear the form and recreate it
void cRapFormBuilder::InsertNextAntenna ()
{
	mUpdate =false;	
}


//*********************************************************************
// Clear the form
void cRapFormBuilder::Clear ()
{
	mUpdate =false;
	QMapIterator<QString,QLabel*> labelIt(mFormLabels);
	QMapIterator<QString,QWidget*> widgetIt(mFormWidgets);
	
	// Destroy all the form labels
	while(labelIt.hasNext())
	{
		labelIt.next();
		labelIt.value()->close();
	}
	
	// Destroy all the form widgets
	while(widgetIt.hasNext())
	{
		widgetIt.next();
		widgetIt.value()->close();
	}
	
	mFormWidgets.clear();
	mFormLabels.clear();
}


//*****************************************************************
// Update the data
void cRapFormBuilder::Update ()
{
	mUpdate=true;
	mInserting=false;
	if(InsertData(mTable, mFormWidgets,true)!=-1)
	{
		mUpdateButton->setEnabled(false);
	}
}


//*********************************************************************
// Update the navigation buttons
void cRapFormBuilder::ImportAntennaFromFile ()
{
	mUpdate =false;
	cAntennaFilesImporter* antennas = new cAntennaFilesImporter(0);
	
	antennas->show();
}


//********************************************************************
// Change to the next record
void cRapFormBuilder::NextRecord ()
{
	int currentRow = mTableView->currentRow();
	cout << "cRapFormBuilder::NextRecord (); currentRow = " << currentRow << endl;
	if ((currentRow<0)||(currentRow > mTableView->rowCount()-1))
		currentRow=0;
	else if (currentRow==mTableView->rowCount()-1)
		currentRow=0;
	else currentRow++;

	cout << "cRapFormBuilder::NextRecord (); currentRow = " << currentRow << endl;	
	mTableView->setCurrentCell(currentRow,0);
	mTableView->selectRow(currentRow);
	
	// Populate the form
	PopulateForm();
	
	// make sure that the mUpdateButton is disabled

//	mUpdateButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);
	mUpdate=false;
}


//******************************************************************
// Change to the previous record
void cRapFormBuilder::PreviousRecord ()
{
	int currentRow = mTableView->currentRow();
	cout << "cRapFormBuilder::NextRecord (); currentRow = " << currentRow << endl;
	if ((currentRow<0)||(currentRow > mTableView->rowCount()-1))
		currentRow=0;
	else if (currentRow==0)
		currentRow = mTableView->rowCount()-1;
	else
		currentRow--;
	cout << "cRapFormBuilder::NextRecord (); currentRow = " << currentRow << endl;	
	mTableView->setCurrentCell(currentRow,0);
	mTableView->selectRow(currentRow);
	
	// Populate the form
	PopulateForm();
	
	// Make sure that mUpdate button is disabled
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);
	mUpdate=false;
}

//************************************************************
// Flag that content has changed
void cRapFormBuilder::ContentChanged (const QString& text)
{
	if (!mInserting)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
	if (mTable=="site")
	{
		mCloseByButton->setVisible(true);
		mCloseByButton->setEnabled(true);
		mGroundHeightButton->setVisible(true);
		mGroundHeightButton->setEnabled(true);
		mDefaultRadiosButton->setVisible(true);
		mDefaultRadiosButton->setEnabled(true);
	}
        if (mTable == "radioinstallation")
        {
               mCalculateRxLossesButton->setVisible(true);
               mCalculateRxLossesButton->setEnabled(true);
               mCalculateTxLossesButton->setVisible(true);
               mCalculateTxLossesButton->setEnabled(true);
        }
}

//**************************************************************
void cRapFormBuilder::ContentChanged (int val)
{
	if (!mInserting)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}

}

//***************************************************************
void cRapFormBuilder::ContentChanged (double val)
{
	if (!mInserting)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
}

//***************************************************************
void cRapFormBuilder::ContentChanged (QDateTime& dateTime)
{
	if (!mInserting)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
}

//**************************************************************
void cRapFormBuilder::ContentChanged (QDate& date)
{
	if (!mInserting)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
}

//***************************************************************
void cRapFormBuilder::FormatToDDdddd()
{
	static_cast<QRadioButton*>(mFormWidgets["location:DDdddd"])->setChecked(true);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMmm"])->setChecked(false);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMSS"])->setChecked(false);
	mDegreeFormat = dfDeg;
	QString Ligging; 
	Ligging = MakeStringFromDecDegrees ( mLat, mDegreeFormat, true);
	static_cast<QLineEdit*>(mFormWidgets["location:latitude"])->setText(Ligging);
	Ligging = MakeStringFromDecDegrees ( mLon, mDegreeFormat, false);
	static_cast<QLineEdit*>(mFormWidgets["location:longitude"])->setText(Ligging);
}

//***************************************************************
void cRapFormBuilder::FormatToDDMMmm()
{
	static_cast<QRadioButton*>(mFormWidgets["location:DDdddd"])->setChecked(false);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMmm"])->setChecked(true);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMSS"])->setChecked(false);
	mDegreeFormat = dfDegMin;
	QString Ligging; 
	Ligging = MakeStringFromDecDegrees ( mLat, mDegreeFormat, true);
	static_cast<QLineEdit*>(mFormWidgets["location:latitude"])->setText(Ligging);
	Ligging = MakeStringFromDecDegrees ( mLon, mDegreeFormat, false);
	static_cast<QLineEdit*>(mFormWidgets["location:longitude"])->setText(Ligging);
}

//***************************************************************
void cRapFormBuilder::FormatToDDMMSS()
{
	static_cast<QRadioButton*>(mFormWidgets["location:DDdddd"])->setChecked(false);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMmm"])->setChecked(false);
	static_cast<QRadioButton*>(mFormWidgets["location:DDMMSS"])->setChecked(true);
	mDegreeFormat = dfDegMinSec;
	QString Ligging; 
	Ligging = MakeStringFromDecDegrees ( mLat, mDegreeFormat, true);
	static_cast<QLineEdit*>(mFormWidgets["location:latitude"])->setText(Ligging);
	Ligging = MakeStringFromDecDegrees ( mLon, mDegreeFormat, false);
	static_cast<QLineEdit*>(mFormWidgets["location:longitude"])->setText(Ligging);
}

//***********************************************************************************
void cRapFormBuilder::FindCurrentLocation()
{
	QString Input;
	Input = static_cast<QLineEdit*>(mFormWidgets["location:latitude"])->text();
	mLat = GetDecimalDegrees(Input.toStdString(), mDegreeFormat,mDegreeFormat!=dfDeg , true);	
	Input = static_cast<QLineEdit*>(mFormWidgets["location:longitude"])->text();	
	mLon = GetDecimalDegrees(Input.toStdString(), mDegreeFormat,mDegreeFormat!=dfDeg , false);
}

//***************************************************************
void cRapFormBuilder::FindGroundHeight()
{
	FindCurrentLocation();
	QString fieldName="groundheight";
	int Height = GetGroundHeight(mLat, mLon);	
	static_cast<QSpinBox*>(mFormWidgets[fieldName])->setValue(Height);	
	mGroundHeightButton->setVisible(false);
	mGroundHeightButton->setEnabled(false);
}

//***************************************************************
void cRapFormBuilder::InsertDefaultRadios()
{
	if (!mInserting)
	{
		cout << "In cRapFormBuilder::InsertDefaultRadios() NOT Inserting " << endl;
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);		
		mUpdateButton->click();
	}
	else
	{
		cout << "In cRapFormBuilder::InsertDefaultRadios() Inserting " << endl;
		mCommitButton->click();
	}
	cout << "In cRapFormBuilder::InsertDefaultRadios()  mCurrentRecordID = " << mCurrentRecordID << endl;
	InsertDefaultRadioInsts(mCurrentRecordID);
	mDefaultRadiosButton->setVisible(false);
	mDefaultRadiosButton->setEnabled(false);
}

//***************************************************************
void cRapFormBuilder::CalculateTXLosses()
{
	int currentRow = mTableView->currentRow();
        double cableloss = 0;
        double combloss = 0;
        int txLength = 0;
        
        QTableWidgetItem* item = mTableView->item(currentRow,0);
        QString id = item->data(Qt::DisplayRole).toString();
        
        string query = "";
        pqxx::result CurrentRecord;
    
        query = "SELECT combiner_splitter_key FROM combiner_splitter WHERE rikey = ";
        query += id.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(CurrentRecord);
        
        if(CurrentRecord.size() > 0)
        {
            string query2 = "";
            pqxx::result CurrentRecord2;

            query2 = "SELECT losses FROM combiner_splitter_type WHERE id = ";
            query2 += CurrentRecord[0]["combiner_splitter_key"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(CurrentRecord2);
            
            combloss = atof(CurrentRecord2[0]["losses"].c_str());
        }
        else
        {
            QString err = "There seems to be no combiner linked to the current radio installation. ";
            QRAP_WARN(err.toStdString().c_str());
            cout << "Query Empty: " << query.c_str() << endl;
            //QMessageBox::information(this, "QRap", err);
            return;
        }
        
        string query3 = "";
        pqxx::result CurrentRecord3;
    
        query3 = "SELECT cablekey,txlength FROM cable WHERE rikey = ";
        query3 += id.toStdString();
        query3 += ";";
        gDb.PerformRawSql(query3);
        gDb.GetLastResult(CurrentRecord3);
        
        if(CurrentRecord3.size() > 0)
        {
            string query4 = "";
            pqxx::result CurrentRecord4;

            query4 = "SELECT signal_loss_per_metre FROM cable_type WHERE id = ";
            query4 += CurrentRecord3[0]["cablekey"].c_str();
            query4 += ";";
            gDb.PerformRawSql(query4);
            gDb.GetLastResult(CurrentRecord4);
            
            cableloss = atof(CurrentRecord4[0]["signal_loss_per_metre"].c_str());
            txLength = atoi(CurrentRecord3[0]["txlength"].c_str());
        }
        else
        {
            QString err = "There seems to be no cable linked to the current radio installation. ";
            QRAP_WARN(err.toStdString().c_str());
            cout << "Query Empty: " << query.c_str() << endl;
            //QMessageBox::information(this, "QRap", err);
            return;
        }
        
        double txloss = (txLength * cableloss) + combloss;
        
        static_cast<QDoubleSpinBox*>(mFormWidgets["txlosses"])->setValue(txloss);
}

//***************************************************************
void cRapFormBuilder::CalculateRXLosses()
{
	int currentRow = mTableView->currentRow();
        double cableloss = 0;
        double combloss = 0;
        int rxLength = 0;
        
        QTableWidgetItem* item = mTableView->item(currentRow,0);
        QString id = item->data(Qt::DisplayRole).toString();
        
        string query = "";
        pqxx::result CurrentRecord;
    
        query = "SELECT combiner_splitter_key FROM combiner_splitter WHERE rikey = ";
        query += id.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(CurrentRecord);
        
        if(CurrentRecord.size() > 0)
        {
            string query2 = "";
            pqxx::result CurrentRecord2;

            query2 = "SELECT losses FROM combiner_splitter_type WHERE id = ";
            query2 += CurrentRecord[0]["combiner_splitter_key"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(CurrentRecord2);
            
            combloss = atof(CurrentRecord2[0]["losses"].c_str());
        }
        else
        {
            QString err = "There seems to be no combiner linked to the current radio installation. ";
            QRAP_WARN(err.toStdString().c_str());
            cout << "Query Empty: " << query.c_str() << endl;
            //QMessageBox::information(this, "QRap", err);
            return;
        }
        
        string query3 = "";
        pqxx::result CurrentRecord3;
    
        query3 = "SELECT cablekey,rxlength FROM cable WHERE rikey = ";
        query3 += id.toStdString();
        query3 += ";";
        gDb.PerformRawSql(query3);
        gDb.GetLastResult(CurrentRecord3);
        
        if(CurrentRecord3.size() > 0)
        {
            string query4 = "";
            pqxx::result CurrentRecord4;

            query4 = "SELECT signal_loss_per_metre FROM cable_type WHERE id = ";
            query4 += CurrentRecord3[0]["cablekey"].c_str();
            query4 += ";";
            gDb.PerformRawSql(query4);
            gDb.GetLastResult(CurrentRecord4);
            
            cableloss = atof(CurrentRecord4[0]["signal_loss_per_metre"].c_str());
            rxLength = atoi(CurrentRecord3[0]["rxlength"].c_str());
        }
        else
        {
            QString err = "There seems to be no cable linked to the current radio installation. ";
            QRAP_WARN(err.toStdString().c_str());
            cout << "Query Empty: " << query.c_str() << endl;
            //QMessageBox::information(this, "QRap", err);
            return;
        }
        
        double rxloss = (rxLength * cableloss) + combloss;
        
        static_cast<QDoubleSpinBox*>(mFormWidgets["rxlosses"])->setValue(rxloss);
}

//***************************************************************
void cRapFormBuilder::CloseBySite()
{
	FindCurrentLocation();
	cGeoP Here(mLat,mLon,DEG);
	string SiteSense = gDb.GetSetting("SiteSelectSense");
	if (SiteSense == "")
		SiteSense = "150";
	double searchD = atof(SiteSense.c_str());
	pqxx::result SiteCloseby = FindClosebySites(Here, searchD);
	int currentRow = mTableView->currentRow();
	int Row;
//	bool found=false;	
	if (SiteCloseby.size() >0)
	{
		mCurrentRecordID = (int)atoi(SiteCloseby[0]["id"].c_str());
		int currentRow = mTableView->currentRow();
		
		QString fieldName;
		cDatabase::FieldUiType uiType;
//		cDatabase::FieldDataType type;
		int id=0;
		
		mTableView->setCurrentCell(0,0);
		
		// Get the id of the row ... assuming the first collunm is the id
		while ((id!=mCurrentRecordID)&&(Row<(mTableView->rowCount()-1)))
		{
			QTableWidgetItem* item = mTableView->item(Row,0);
			id = item->data(Qt::DisplayRole).toInt();
			Row++;
		}
		if (id==mCurrentRecordID)
		{
			currentRow=Row-1;
			mTableView->setCurrentCell(currentRow,0);
			mTableView->selectRow(currentRow);
			PopulateForm();
		}	
		else
		{
			static_cast<QSpinBox*>(mFormWidgets["id"])->setValue(mCurrentRecordID);
			double lat,lon;
			string Result;
			Result = SiteCloseby[0]["ligging"].c_str();
			ExtractLatLongFromPoint(Result,lat,lon);
			QString Ligging; 
			Ligging = MakeStringFromDecDegrees ( lat, mDegreeFormat, true);
			static_cast<QLineEdit*>(mFormWidgets["location:latitude"])->setText(Ligging.toStdString().c_str());
			Ligging = MakeStringFromDecDegrees ( lon, mDegreeFormat, false);
			static_cast<QLineEdit*>(mFormWidgets["location:longitude"])->setText(Ligging.toStdString().c_str());
			static_cast<QLineEdit*>(mFormWidgets["sitename"])->setText(SiteCloseby[0]["sitename"].c_str());
			static_cast<QSpinBox*>(mFormWidgets["groundheight"])->setValue((int)atoi(SiteCloseby[0]["groundheight"].c_str()));
			cout << (SiteCloseby[0]["status"]).c_str() << endl; 
			static_cast<QComboBox*>(mFormWidgets["status"])->
			setCurrentIndex(static_cast<QComboBox*>(mFormWidgets["status"])->findText(SiteCloseby[0]["status"].c_str()));
		}
	}
	else
	{
		QRAP_INFO("No close by sites found");
	}
}

//***************************************************************
// Enable all the form widgets and disable the mBrowseButton
void cRapFormBuilder::AntennaDeviceFromManualInput (bool checked)
{
	if(checked)
	{
		QMapIterator<QString,QWidget*> it(mFormWidgets);
		
		// Enable all the antenna device widgets
		while(it.hasNext())
		{
			it.next();
			it.value()->setEnabled(true);
		} // while
		
		it=mPatternWidgets;
		
		// Enable all the antenna pattern widgets
		while(it.hasNext())
		{
			it.next();
			it.value()->setEnabled(true);
		} // whil

		mBrowseButton->setEnabled(false);
		mCommitButton->setVisible(true);
		mCommitButton->setEnabled(true);
	} // if checked
}

//****************************************************************
// Disable all the form widgets and enable the mBrowseButton
void cRapFormBuilder::AntennaDeviceFromFile (bool checked)
{
	if(checked)
	{
		QMapIterator<QString,QWidget*> it(mFormWidgets);
		
		// Disable all the antennadevice widgets
		while(it.hasNext())
		{
			it.next();
			it.value()->setEnabled(false);
		} // while
		
		it=mPatternWidgets;
		// Disable all the antennapattern widgets
		while(it.hasNext())
		{
			it.next();
			it.value()->setEnabled(false);
		} // while
		
		mBrowseButton->setEnabled(true);
		mCommitButton->setEnabled(false);
	} // if checked
}

//*******************************************************************
// Create a new row in the azimuth mTable
void cRapFormBuilder::CreateAzimuthRow (int row, int col)
{
	if( (row==mAzimuthTable->rowCount()-1)&&(col==mAzimuthTable->columnCount()-1) )
	{
		mAzimuthTable->setRowCount(mAzimuthTable->rowCount()+1);
		
		disconnect(mAzimuthTable,SIGNAL(cellChanged(int,int)),0,0);
		mAzimuthTable->setItem(row+1,col-1,new QTableWidgetItem("0"));
		mAzimuthTable->setItem(row+1,col,new QTableWidgetItem("0"));
		connect(mAzimuthTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateAzimuthRow(int,int)));
		mAzimuthTable->setCurrentCell(row+1,col-1);
	}
	
	if(mUpdateButton!=NULL)
	{
//		mUpdateButton->move(mButtonRow*mFormLayout->verticalSpacing(),0);
		mUpdateButton->setVisible(true);		
		mUpdateButton->setEnabled(true);
	}
}

//**********************************************************************
// Create a new row in the elevation mTable
void cRapFormBuilder::CreateElevationRow (int row, int col)
{
	if( (row==mElevationTable->rowCount()-1)&&(col==mElevationTable->columnCount()-1) )
	{
		mElevationTable->setRowCount(mElevationTable->rowCount()+1);
		
		disconnect(mElevationTable,SIGNAL(cellChanged(int,int)),0,0);
		mElevationTable->setItem(row+1,col-1,new QTableWidgetItem("0"));
		mElevationTable->setItem(row+1,col,new QTableWidgetItem("0"));
		connect(mElevationTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateElevationRow(int,int)));
		mElevationTable->setCurrentCell(row+1,col-1);
	}
	
	if(mUpdateButton!=NULL)
	{
//		mUpdateButton->move(mButtonRow*mFormLayout->verticalSpacing(),0);
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
}

//**********************************************************************
// Create a new row in the spectral envelope mTable
void cRapFormBuilder::CreateEnvelopeRow (int row, int col)
{
	if( (row==mEnvelopeTable->rowCount()-1)&&(col==mEnvelopeTable->columnCount()-1) )
	{
		mEnvelopeTable->setRowCount(mEnvelopeTable->rowCount()+1);
		
		disconnect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),0,0);
		mEnvelopeTable->setItem(row+1,col-1,new QTableWidgetItem("0"));
		mEnvelopeTable->setItem(row+1,col,new QTableWidgetItem("0"));
		connect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateEnvelopeRow(int,int)));
		mEnvelopeTable->setCurrentCell(row+1,col-1);
	}
	
	if(mUpdateButton!=NULL)
	{
		mUpdateButton->setVisible(true);
		mUpdateButton->setEnabled(true);
	}
}

//*******************************************************************
// Create the form widget
void cRapFormBuilder::CreateFormWidget (QString fieldName,
					QString fieldLabel,
					QMap<QString,QWidget*> &widgets,
					cDatabase::FieldDataType type,
					QVariant val,
					int &layoutRow,
					bool readOnly)
{
	UnitType unit;
	QStringList location;
	string setting;
	bool direction = true;
	cout << " Entering cRapFormBuilder::CreateFormWidget" << endl;

	if (mInserting) cout << "cRapFormBuilder::CreateFormWidget mInserting is true." << endl;
	else cout << "cRapFormBuilder::CreateFormWidget mInserting is false." << endl;

	switch(type)
	{
		case cDatabase::dtPostGisPoint:
				cout << " dtPostGisPoint in cRapFormBuilder::CreateFormWidget" << endl;
			
			// Add the radio buttons to the form
			widgets.insert(fieldName+":DDMMSS",static_cast<QWidget*>(new QRadioButton("DD:MM:SS X",this)));
			widgets.insert(fieldName+":DDMMmm",static_cast<QWidget*>(new QRadioButton("DD:MM.mm X",this)));
			widgets.insert(fieldName+":DDdddd",static_cast<QWidget*>(new QRadioButton("\261 DD.dddd",this)));
			
			// Create the form labels
			mFormLabels.insert(fieldName+":latitude",new QLabel("Latitude"));
			mFormLabels.insert(fieldName+":longitude",new QLabel("Longitude"));
			
			// Set all the tool tips for the radio buttons
			static_cast<QRadioButton*>(widgets[fieldName+":DDMMSS"])->setToolTip("Enter the location in Degrees(DD):Minutes(MM):Seconds(SS) and direction X (N,S,E,W)");
			static_cast<QRadioButton*>(widgets[fieldName+":DDMMmm"])->setToolTip("Enter the location in Degrees(DD):Minutes(MM).Decimals(mm) and direction X (N,S,E,W)");
			static_cast<QRadioButton*>(widgets[fieldName+":DDdddd"])->setToolTip("Enter the location in Degrees(DD).Decimals(dddd) and direction to be + or -");
			
			mFormLayout->addWidget(widgets[fieldName+":DDMMSS"],layoutRow,2,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName+":DDMMmm"],layoutRow+1,2,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName+":DDdddd"],layoutRow+2,2,1,1,Qt::AlignLeft);
			

			// Get the format of the location
			setting = gDb.GetSetting("location");
			if(setting=="DD:MM:SS")
			{
				static_cast<QRadioButton*>(widgets[fieldName+":DDMMSS"])->setChecked(true);
				mDegreeFormat = dfDegMinSec;
			}
			else if(setting=="DD:MM.mm")
			{
				static_cast<QRadioButton*>(widgets[fieldName+":DDMMmm"])->setChecked(true);
				mDegreeFormat = dfDegMin;
			}
			else
			{
				static_cast<QRadioButton*>(widgets[fieldName+":DDdddd"])->setChecked(true);
				mDegreeFormat = dfDeg;
				direction = false;
			}
			
			connect(static_cast<QRadioButton*>(widgets[fieldName+":DDMMSS"]),SIGNAL(clicked()),this,SLOT(FormatToDDMMSS()));
			connect(static_cast<QRadioButton*>(widgets[fieldName+":DDMMmm"]),SIGNAL(clicked()),this,SLOT(FormatToDDMMmm()));
			connect(static_cast<QRadioButton*>(widgets[fieldName+":DDdddd"]),SIGNAL(clicked()),this,SLOT(FormatToDDdddd()));

			static_cast<QRadioButton*>(widgets[fieldName+":DDMMSS"])->setEnabled(true);
			static_cast<QRadioButton*>(widgets[fieldName+":DDMMmm"])->setEnabled(true);
			static_cast<QRadioButton*>(widgets[fieldName+":DDdddd"])->setEnabled(true);

			// Add the logitude and latitude input boxes to the form
			widgets.insert(fieldName+":latitude",static_cast<QWidget*>(new QLineEdit(this)));
			widgets.insert(fieldName+":longitude",static_cast<QWidget*>(new QLineEdit(this)));
			
			// Set the the value for the latitude and longitude
			location = val.toString().split("|");
			
			if(location.count()>1)
			{
				static_cast<QLineEdit*>(widgets[fieldName+":latitude"])->setText(location[0]);
				mLat = GetDecimalDegrees(location[0].toStdString(),mDegreeFormat,direction, true);
				static_cast<QLineEdit*>(widgets[fieldName+":longitude"])->setText(location[1]);
				mLon = GetDecimalDegrees(location[1].toStdString(),mDegreeFormat,direction, false);
			} // if location
			
			// Set the tool tip for the input boxes
			static_cast<QLineEdit*>(widgets[fieldName+":latitude"])->setToolTip("Latitude coordinate");
			static_cast<QLineEdit*>(widgets[fieldName+":longitude"])->setToolTip("Longitude coordinate");
			
			if(!mInserting)
			{
				// connect the input boxes to the ContentChanged slot
				connect(static_cast<QLineEdit*>(widgets[fieldName+":latitude"]),SIGNAL(textChanged(const QString)),this,SLOT(ContentChanged(const QString)));
				connect(static_cast<QLineEdit*>(widgets[fieldName+":longitude"]),SIGNAL(textChanged(const QString)),this,SLOT(ContentChanged(const QString)));
			} // if mInserting
//			else
//			{
			mCloseByButton->setVisible(true);
			mCloseByButton->setEnabled(true);
//			} 
				
			mFormLayout->addWidget(mFormLabels[fieldName+":latitude"],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName+":latitude"],layoutRow,1,1,1,Qt::AlignLeft);
			
			mFormLayout->addWidget(mFormLabels[fieldName+":longitude"],layoutRow+1,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName+":longitude"],layoutRow+1,1,1,1,Qt::AlignLeft);
			
			layoutRow+=3;
			break;
			
			
		case cDatabase::dtPostGisLineString:
			// Do nothing
			break;
			
		case cDatabase::dtPostGisPolygon:
			// Do nothing
			break;
			
		case cDatabase::dtInteger:
			// Create the widget
			widgets.insert(fieldName,static_cast<QWidget*>(new QSpinBox(this)));
			static_cast<QSpinBox*>(widgets[fieldName])->setMinimum(-10000000);
			static_cast<QSpinBox*>(widgets[fieldName])->setMaximum(10000000);
			static_cast<QSpinBox*>(widgets[fieldName])->setValue(val.toInt());
			
			// Create the form label
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			// Add the form to the layout
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QSpinBox*>(widgets[fieldName]),SIGNAL(valueChanged(int)),this,SLOT(ContentChanged(int)));
			break;
			
			
		case cDatabase::dtReal:
			widgets.insert(fieldName,static_cast<QWidget*>(new QDoubleSpinBox(this)));
			static_cast<QDoubleSpinBox*>(widgets[fieldName])->setMinimum(-10000000);
			static_cast<QDoubleSpinBox*>(widgets[fieldName])->setMaximum(10000000);
			
//			\\.TODO: change number of decimal places here ... need to check Qt manuals.
			
			static_cast<QDoubleSpinBox*>(widgets[fieldName])->setValue(val.toDouble());
			
			// Create the form label
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Get the unit type for the current field
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				unit = mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mUnitType;
			else 
				unit = utNone;
			
			switch (unit)
			{
				case utSensitivity:
					mFormLabels.insert(fieldName+":sensitivity",new QLabel(trUtf8(gDb.GetSetting("sensitivity").c_str()),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":sensitivity"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utPower:
					mFormLabels.insert(fieldName+":power",new QLabel(trUtf8(gDb.GetSetting("power").c_str()),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":power"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utEirp:
					mFormLabels.insert(fieldName+":eirp", new QLabel(trUtf8(gDb.GetSetting("eirp").c_str()),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":eirp"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utHeight:
					mFormLabels.insert(fieldName+":height", new QLabel(trUtf8("meter"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":height"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utDistance:
					mFormLabels.insert(fieldName+":distance", new QLabel(trUtf8("km"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":distance"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utLossGain:
					mFormLabels.insert(fieldName+":dbell", new QLabel(trUtf8("dB"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":dbell"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utMegaHertz:
					mFormLabels.insert(fieldName+":megahertz", new QLabel(trUtf8("MHz"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":megahertz"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utkiloHertz:
					mFormLabels.insert(fieldName+":kilohertz", new QLabel(trUtf8("kHz"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":kilohertz"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utAngle:
					mFormLabels.insert(fieldName+":degrees", new QLabel(trUtf8("degrees"),this));
					mFormLayout->addWidget(mFormLabels[fieldName+":degrees"],layoutRow,2,1,1,Qt::AlignLeft);
					break;
				case utNone:
					break;
			}
			
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QDoubleSpinBox*>(widgets[fieldName]),SIGNAL(valueChanged(double)),this,SLOT(ContentChanged(double)));
			break;
			
			
		case cDatabase::dtBoolean:
			widgets.insert(fieldName,static_cast<QWidget*>(new QCheckBox(this)));
			static_cast<QCheckBox*>(widgets[fieldName])->setCheckState(val.toBool()?Qt::Checked:Qt::Unchecked);
			
			// Create the form label
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QCheckBox*>(widgets[fieldName]),SIGNAL(stateChanged(int)),this,SLOT(ContentChanged(int)));
			break;
			
			
		case cDatabase::dtDate:
			widgets.insert(fieldName,static_cast<QWidget*>(new QDateEdit(this)));
			static_cast<QDateEdit*>(widgets[fieldName])->setDate(val.toDate());
			
			// Creat the form label
			
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QDateEdit*>(widgets[fieldName]),SIGNAL(dateChanged(QDate)),this,SLOT(ContentChanged(QDate)));
			break;
			
			
		case cDatabase::dtDateTime:
			widgets.insert(fieldName,static_cast<QWidget*>(new QDateTimeEdit(this)));
			static_cast<QDateTimeEdit*>(widgets[fieldName])->setDateTime(val.toDateTime());
			
			// Create the form label
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QDateTimeEdit*>(widgets[fieldName]),SIGNAL(dateTimeChanged(QDateTime)),this,SLOT(ContentChanged(QDateTime)));
			break;
			
		default:
			// Create the widget
			widgets.insert(fieldName,static_cast<QWidget*>(new QLineEdit(this)));
			static_cast<QLineEdit*>(widgets[fieldName])->setText(val.toString());
			
			// Create the form label
			mFormLabels.insert(fieldName,new QLabel(fieldLabel,this));
			
			// Add the widget to the layout
			mFormLayout->addWidget(mFormLabels[fieldName],layoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(widgets[fieldName],layoutRow,1,1,1,Qt::AlignLeft);
			
			// Set the widget's tooltip
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				widgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			
			if(readOnly)
				widgets[fieldName]->setEnabled(false);
			
			if(!mInserting)
				connect(static_cast<QLineEdit*>(widgets[fieldName]),SIGNAL(textChanged(const QString)),this,SLOT(ContentChanged(const QString)));
			break;
	} // switch
}

//*********************************************************************
// Create the insert form
void cRapFormBuilder::CreateGenericInsertForm ()
{
	cout << "REFERENCES SIZE IN cRapFormBuilder::CreateGenericInsertForm "<< mReferences.size() << endl;
	bool rowReadOnly=false;
	mUpdate=false;
	mInserting =true;
	char *text;
	text = new char[33];
/*	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setUnderline(false);
	font.setWeight(75);
	font.setStrikeOut(false);
	font.setKerning(true);
*/	
	mCommitButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitButton->setVisible(true);
	mCommitButton->setEnabled(true);
	mCommitAddNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mCommitAddNextButton->setVisible(true);
	mCommitAddNextButton->setEnabled(true);
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);

	mFormLabels.insert("title",new QLabel("Insert Form"),this);
//	mFormLabels["title"]->setFont(font);
	mFormLayout->addWidget(mFormLabels["title"],2,0,1,1,Qt::AlignLeft);

	int size = mTableView->columnCount();
	mLayoutRow = 2;
	if (size<0) 
		mTableView->setCurrentCell(0,0);

	for( int i=0 ; i<size ; i++ )
	{
		QTableWidgetItem* header = mTableView->horizontalHeaderItem(i);
		
		// Add the widget to the form
		cDatabase::FieldDataType type;
		UnitType unit;
		cDatabase::FieldUiType uiType;
		QVariant val;
		QString fieldName = header->data(Qt::UserRole).toString();
		
		if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
		{
			uiType = gDb.GetFieldUiType(mTable.toStdString(),fieldName.toStdString());
			type = gDb.GetFieldDataType(mTable.toStdString(),fieldName.toStdString());
			if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
				unit = mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mUnitType;
			else unit = utNone;
		}
		else
		{
			uiType = cDatabase::utNormal;
			type = cDatabase::dtString;
			unit = utNone;
		}

		if(mDefaults.size()>0) 
		{
			if(unit!=utNone)
			{
				string setting;
				double wyswaarde;
				switch (unit)
				{
					case utSensitivity:
						setting=gDb.GetSetting("sensitivity");
						break;
					case utPower:
						setting=gDb.GetSetting("power");
						break;
					case utEirp:
						setting=gDb.GetSetting("eirp");
						break;
					default: 
						setting="nonconvert"; // will result in no change
						break;
				}
				if (setting=="dB\316\274V")
				{
					string impedance = gDb.GetSetting("impedence");
					if (impedance=="50")
						wyswaarde =  atof(mDefaults[fieldName.toStdString()].c_str()) +107;
					else wyswaarde = atof(mDefaults[fieldName.toStdString()].c_str()) +  108.75;
				}
				else if (setting=="dBW") wyswaarde = atof(mDefaults[fieldName.toStdString()].c_str()) - 30.0;
				else if (setting=="W") wyswaarde = pow(10,(atof(mDefaults[fieldName.toStdString()].c_str())-30.0)/10.0);
				else wyswaarde = atof(mDefaults[fieldName.toStdString()].c_str());
				gcvt(wyswaarde,10,text);
				val.setValue(QString::fromStdString(text));			
			}
			else			
				val.setValue(QString::fromStdString(mDefaults[fieldName.toStdString()]));
		}
		else
			val.setValue(QString(""));
			
		if(fieldName.contains("azimuthangles")||fieldName.contains("elevationangles")||fieldName.contains("azimuthpattern")||fieldName.contains("elevationpattern")||fieldName.contains("offsets")||fieldName.contains("values"))
		{
			// Create one QTablewidget for the azimuth angles and values
			if(fieldName=="azimuthangles")
			{
				mAzimuthTable = new QTableWidget(1,2,this);
				QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
				QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
				
				anglesHeader->setData(Qt::UserRole,QVariant("azimuthangles"));
				patternHeader->setData(Qt::UserRole,QVariant("azimuthpattern"));
				
				mAzimuthTable->setHorizontalHeaderItem(0,anglesHeader);
				mAzimuthTable->setHorizontalHeaderItem(1,patternHeader);
				
				QTableWidgetItem* first = new QTableWidgetItem("0");
				QTableWidgetItem* second = new QTableWidgetItem("0");
				mAzimuthTable->setItem(0,0,first);
				mAzimuthTable->setItem(0,1,second);
				mAzimuthTable->verticalHeader()->setVisible(false);
				
				mFormWidgets.insert("azimuth",static_cast<QWidget*>(mAzimuthTable));
				mFormLabels.insert("azimuth",new QLabel("Azimuth Pattern"));
				
				mFormLayout->addWidget(mFormLabels["azimuth"],mLayoutRow,0,1,1,Qt::AlignTop);
				mFormLayout->addWidget(mFormWidgets["azimuth"],mLayoutRow++,1,1,1,Qt::AlignLeft);
				
				connect(mAzimuthTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateAzimuthRow(int,int)));
			}
			else
			{
				if(fieldName=="elevationangles")
				{
					mElevationTable = new QTableWidget(1,2,this);
					QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
					QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
								
					anglesHeader->setData(Qt::UserRole,QVariant("elevationangles"));
					patternHeader->setData(Qt::UserRole,QVariant("elevationpattern"));
								
					mElevationTable->setHorizontalHeaderItem(0,anglesHeader);
					mElevationTable->setHorizontalHeaderItem(1,patternHeader);
								
					QTableWidgetItem* first = new QTableWidgetItem("0");
					QTableWidgetItem* second = new QTableWidgetItem("0");
					mElevationTable->setItem(0,0,first);
					mElevationTable->setItem(0,1,second);
					mElevationTable->verticalHeader()->setVisible(false);
								
					mFormWidgets.insert("elevation",static_cast<QWidget*>(mElevationTable));
					mFormLabels.insert("elevation",new QLabel("Elevation Pattern"));
								
					mFormLayout->addWidget(mFormLabels["elevation"],mLayoutRow,0,1,1,Qt::AlignTop);
					mFormLayout->addWidget(mFormWidgets["elevation"],mLayoutRow++,1,1,1,Qt::AlignLeft);
									
					connect(mElevationTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateElevationRow(int,int)));
				} // if elevationangles
				else
				{
					if(fieldName=="offsets")
					{
						mEnvelopeTable = new QTableWidget(1,2,this);
						QTableWidgetItem* anglesHeader = new QTableWidgetItem("Frequency Offsets");
						QTableWidgetItem* patternHeader = new QTableWidgetItem("Envelope Values");
								
						anglesHeader->setData(Qt::UserRole,QVariant("offsets"));
						patternHeader->setData(Qt::UserRole,QVariant("values"));
								
						mEnvelopeTable->setHorizontalHeaderItem(0,anglesHeader);
						mEnvelopeTable->setHorizontalHeaderItem(1,patternHeader);
						mEnvelopeTable->resizeColumnsToContents();
								
						QTableWidgetItem* first = new QTableWidgetItem("0");
						QTableWidgetItem* second = new QTableWidgetItem("0");
						mEnvelopeTable->setItem(0,0,first);
						mEnvelopeTable->setItem(0,1,second);
						mEnvelopeTable->verticalHeader()->setVisible(false);
									
						mFormWidgets.insert("envelopes",static_cast<QWidget*>(mEnvelopeTable));
						mFormLabels.insert("envelopes",new QLabel("Spectral Envelope"));
								
						mFormLayout->addWidget(mFormLabels["envelopes"],mLayoutRow,0,1,1,Qt::AlignTop);
						mFormLayout->addWidget(mFormWidgets["envelopes"],mLayoutRow++,1,1,1,Qt::AlignLeft);
									
						connect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateEnvelopeRow(int,int)));
					}
				}
			} // else azimuth azimuthangels
		} // if
		else
		{
			// Check the type of the field
			if(uiType == cDatabase::utNormal)
			{
				// Do not create a form widget for extra fields from other tables and the id field
				if(fieldName!="id" && !header->data(Qt::UserRole).toString().contains(".") )
					CreateFormWidget(fieldName,header->data(Qt::DisplayRole).toString(),mFormWidgets,type,val,mLayoutRow,false);
			} // if
			else
			{
				mFormWidgets.insert(fieldName,static_cast<QWidget*>(new QComboBox(this)));
				
				// Populate the combobox with selection values
				StringIntArray vals;
				StringIntArrayIterator iterator;
				
				gDb.GetFieldUiParams(mTable.toStdString(),fieldName.toStdString(),vals);
				
				
				QComboBox* combo = static_cast<QComboBox*>(mFormWidgets[fieldName]);
	// 			if(type==cDatabase::dtBoolean)
	// 			{
	// 				if(mDefaults[fieldName.toStdString()] == "t")
	// 					combo->setCurrentIndex(combo->findText("1:TRUE",Qt::MatchCaseSensitive));
	// 				else
	// 					combo->setCurrentIndex(combo->findText("0:FALSE",Qt::MatchCaseSensitive));
	// 			} // if type
				
				int j=0;
				if(uiType==cDatabase::utDropdownTable)
				{
					j=1;
					combo->insertItem(0,"0:NULL");
				}

				int key = 0;
				// Populate the combobox with the default data
				for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
				{
					QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
					combo->insertItem(j,temp,qVariantFromValue(iterator->first));
					if (mReferences.size() > 0)
					{
						
						StringMapIterator it = mReferences.begin();
						while (it != mReferences.end())
						{
							if ((QString::number(iterator->first)==QString::fromStdString((*it).second))&&(QString::fromStdString((*it).first)==fieldName))
							{
								key = j;
							}
							it++;
						}
					}
					j++;
				} // for
			
				combo->setCurrentIndex(key);
								
				// Add the form label
				mFormLabels.insert(fieldName,new QLabel(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mLabel),this));
				
				// Add the widget to the layout
				mFormLayout->addWidget(mFormLabels[fieldName],mLayoutRow,0,1,1,Qt::AlignLeft);
				mFormLayout->addWidget(mFormWidgets[fieldName],mLayoutRow,1,1,1,Qt::AlignLeft);
				
				// Set the widget's tooltip
				mFormWidgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
			} // else
					
			if(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mReadOnly && fieldName!="id")
			{
				mFormWidgets[fieldName]->setEnabled(false);
			} // if
		}
		
		mLayoutRow++;
		mButtonRow = mLayoutRow;
	} // for
	
//	mCommitButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitButton->setVisible(true);
	mCommitButton->setEnabled(true);
	mUpdateButton->setVisible(false);
	mUpdateButton->setEnabled(false);
//	mCommitAddNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mCommitAddNextButton->setVisible(true);
	mCommitAddNextButton->setEnabled(true);
	mUpdate =false;
	mButtonPosY = mUpdateButton->pos().y();
	if (mButtonRow)
		mLayoutDelta = mButtonPosY/mButtonRow;
	else mLayoutDelta = mButtonPosY;
	delete [] text;
	connect(this,SIGNAL(InsertComplete(int)),parentWidget(),SLOT(setCurrentIndex(int)));
	setLayout(mFormLayout);
}


//**********************************************************************
// Create the mUpdate form
void cRapFormBuilder::CreateGenericUpdateForm ()
{
	char *text= new char[33];
	string query="";
	mUpdate=true;
	mInserting = false;
	if (mTableView->rowCount()<=0)
	{
		CreateGenericInsertForm ();
		return;	
	}
	cout << " Entering cRapFormBuilder::CreateGenericUpdateForm ()" << endl;
	cout << "REFERENCES SIZE IN cRapFormBuilder::CreateGenericUpdateForm ()  "<< mReferences.size() << endl;
	bool rowReadOnly=false;
	// Create the Push Buttons
	mNextButton->setVisible(true);
	mNextButton->setEnabled(true);
	mPreviousButton->setVisible(true);
	mPreviousButton->setEnabled(true);
	mUpdateButton->setVisible(true);
	
	// Add the buttons to the form layout
	cout << "cRapFormBuilder::CreateGenericUpdateForm ()    mTableView->currentRow() = " <<  mTableView->currentRow() << endl;
	int size = mTableView->columnCount();
	int mLayoutRow = 2;
	int id=0;
	if (mTableView->currentColumn()<0)
	{
		mTableView->setCurrentCell(0,0);
	}
	cout << "cRapFormBuilder::CreateGenericUpdateForm ()  Voor lus; size="<< size << endl;
	for( int i=0 ; i<size ; i++ )
	{
		bool readOnly=false;
		if (mTableView->currentRow()<0)
			mTableView->selectRow(0);
		else if ((mTableView->currentRow()>=mTableView->rowCount())&&(mTableView->rowCount()>0))
			mTableView->selectRow(mTableView->rowCount()-1);	
		int row = mTableView->currentRow();
		if (row<0) row=0;
		cout << "cRapFormBuilder::CreateGenericUpdateForm() RowCount = "<< mTableView->rowCount() << endl;
		cout << "cRapFormBuilder::CreateGenericUpdateForm() currentRow = "<< mTableView->currentRow() << endl;
		// Get the mTable header item and the data item
		QTableWidgetItem* header = mTableView->horizontalHeaderItem(i);
		QTableWidgetItem* item = mTableView->item(row,i);

		// Add the widget to the form
		cDatabase::FieldDataType type;
		cDatabase::FieldUiType uiType;
		
		QString fieldName = header->data(Qt::UserRole).toString();
		if(mDbs[mTable.toStdString()].mFields.count(fieldName.toStdString())>0)
		{
//			cout << "cRapFormBuilder::CreateGenericUpdateForm () Field Count> 0" << endl;
			uiType = gDb.GetFieldUiType(mTable.toStdString(),fieldName.toStdString());
			type = gDb.GetFieldDataType(mTable.toStdString(),fieldName.toStdString());
//			cout << "i = "<< i <<"		uiType = " << uiType << "		type = " << type << endl;
			if(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mReadOnly || item->flags()==Qt::ItemIsSelectable)
				readOnly = true;
			else
				rowReadOnly = false;
		} // if mDbs
		else
		{
//			cout << "cRapFormBuilder::CreateGenericUpdateForm () Field Count==0" << endl;
			uiType = cDatabase::utNormal;
			type = cDatabase::dtString;
			readOnly = true;
		} // else mDbs
		
	
		if(fieldName.contains("azimuthangles")||fieldName.contains("elevationangles")
			||fieldName.contains("azimuthpattern")||fieldName.contains("elevationpattern")
			||fieldName.contains("offsets")||fieldName.contains("values"))
		{
			// Create one QTablewidget for the azimuth angles and values
			if(fieldName=="azimuthangles")
			{
				mAzimuthTable = new QTableWidget(1,2,this);
				QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
				QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
				
				anglesHeader->setData(Qt::UserRole,QVariant("azimuthangles"));
				patternHeader->setData(Qt::UserRole,QVariant("azimuthpattern"));
				
				mAzimuthTable->setHorizontalHeaderItem(0,anglesHeader);
				mAzimuthTable->setHorizontalHeaderItem(1,patternHeader);
				mAzimuthTable->verticalHeader()->setVisible(false);
				
				// Perform a select query to get the values for the angles and patterns
				query = "select azimuthangles,azimuthpattern from antennapattern where id = ";
				gcvt(id,8,text);
				query += text;
				query += ";";	
				if(gDb.PerformRawSql(query))
				{
					pqxx::result r;
					
					gDb.GetLastResult(r);
					
					if(r.size()>0)
					{
						QString temp =  r[0]["azimuthangles"].c_str();
						temp.remove("{");
						temp.remove("}");
						
						QStringList angles = temp.split(",");
						
						temp = r[0]["azimuthpattern"].c_str();
						temp.remove("{");
						temp.remove("}");
						QStringList patternValues = temp.split(",");
						
						if(angles.size()>0)
						{
							for(int i=0;i<angles.size();i++)
							{
								mAzimuthTable->setRowCount(i+1);
								QTableWidgetItem* item = new QTableWidgetItem(angles[i]);
								mAzimuthTable->setItem(i,0,item);
							} // for
						} // if angles.size
						
						if(patternValues.size()>0)
						{
							for(int i=0; i<patternValues.size(); i++)
							{
								if(i>mAzimuthTable->rowCount()-1)
									mAzimuthTable->setRowCount(i+1);
								
								QTableWidgetItem* item = new QTableWidgetItem(patternValues[i]);
								mAzimuthTable->setItem(i,1,item);
							} // for
						} // if patternValues.size
						
					} // if r
				} // if PerformRawSql
				else
				{
					QTableWidgetItem* first = new QTableWidgetItem("0");
					QTableWidgetItem* second = new QTableWidgetItem("0");
					mAzimuthTable->setItem(0,0,first);
					mAzimuthTable->setItem(0,1,second);
				} // else PerformRawSql
				
				mFormWidgets.insert("azimuth",static_cast<QWidget*>(mAzimuthTable));
				mFormLabels.insert("azimuth",new QLabel("Azimuth Pattern"));
				
				mFormLayout->addWidget(mFormLabels["azimuth"],mLayoutRow,0,1,1,Qt::AlignTop);
				mFormLayout->addWidget(mFormWidgets["azimuth"],mLayoutRow++,1,1,1,Qt::AlignLeft);
				
				connect(mAzimuthTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateAzimuthRow(int,int)));
			}
			else
			{
				if(fieldName=="elevationangles")
				{
					mElevationTable = new QTableWidget(1,2,this);
					QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
					QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
								
					anglesHeader->setData(Qt::UserRole,QVariant("elevationangles"));
					patternHeader->setData(Qt::UserRole,QVariant("elevationpattern"));
								
					mElevationTable->setHorizontalHeaderItem(0,anglesHeader);
					mElevationTable->setHorizontalHeaderItem(1,patternHeader);
					mElevationTable->verticalHeader()->setVisible(false);
					
					// Perform a select query to get the values for the angles and patterns
					query = "select elevationangles,elevationpattern from antennapattern where id = ";
					gcvt(id,8,text);
					query +=text;
					query +=";";
					if(gDb.PerformRawSql(query))
					{
						pqxx::result r;
					
						gDb.GetLastResult(r);
					
						if(r.size()>0)
						{
							QString temp =  r[0]["elevationangles"].c_str();
							temp.remove("{");
							temp.remove("}");
						
							QStringList angles = temp.split(",");
						
							temp = r[0]["elevationpattern"].c_str();
							temp.remove("{");
							temp.remove("}");
							QStringList patternValues = temp.split(",");
						
							if(angles.size()>0)
							{
								for(int i=0;i<angles.size();i++)
								{
									mElevationTable->setRowCount(i+1);
									QTableWidgetItem* item = new QTableWidgetItem(angles[i]);
									mElevationTable->setItem(i,0,item);
								} // for
							} // if angles.size
						
							if(patternValues.size()>0)
							{
								for(int i=0; i<patternValues.size(); i++)
								{
									if(i>mElevationTable->rowCount()-1)
										mElevationTable->setRowCount(i+1);
								
									QTableWidgetItem* item = new QTableWidgetItem(patternValues[i]);
									mElevationTable->setItem(i,1,item);
								} // for
							} // if patternValues.size
						
						} // if r
					} // if PerformRawSql
					else
					{
						QTableWidgetItem* first = new QTableWidgetItem("0");
						QTableWidgetItem* second = new QTableWidgetItem("0");
						mAzimuthTable->setItem(0,0,first);
						mAzimuthTable->setItem(0,1,second);
					} // else PerformRawSql
								
					mFormWidgets.insert("elevation",static_cast<QWidget*>(mElevationTable));
					mFormLabels.insert("elevation",new QLabel("Elevation Pattern"));
								
					mFormLayout->addWidget(mFormLabels["elevation"],mLayoutRow,0,1,1,Qt::AlignTop);
					mFormLayout->addWidget(mFormWidgets["elevation"],mLayoutRow++,1,1,1,Qt::AlignLeft);
									
					connect(mElevationTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateElevationRow(int,int)));
				} // if elevationangles
				else
				{
					if(fieldName=="offsets")
					{
						mEnvelopeTable = new QTableWidget(1,2,this);
						QTableWidgetItem* anglesHeader = new QTableWidgetItem("Frequency Offsets");
						QTableWidgetItem* patternHeader = new QTableWidgetItem("Envelope Value");
								
						anglesHeader->setData(Qt::UserRole,QVariant("offsets"));
						patternHeader->setData(Qt::UserRole,QVariant("values"));
								
						mEnvelopeTable->setHorizontalHeaderItem(0,anglesHeader);
						mEnvelopeTable->setHorizontalHeaderItem(1,patternHeader);
						mEnvelopeTable->verticalHeader()->setVisible(false);
						mEnvelopeTable->resizeColumnsToContents();
						
						// Perform a select query to get the values for the angles and patterns
						query = "select offsets,values from envelopes where id=";
						gcvt(id,8,text);
						query += text;
						query +=";";
						if(gDb.PerformRawSql(query))
						{
							pqxx::result r;
					
							gDb.GetLastResult(r);
					
							if(r.size()>0)
							{
								QString temp =  r[0]["offsets"].c_str();
								temp.remove("{");
								temp.remove("}");
						
								QStringList offsets = temp.split(",");
						
								temp = r[0]["values"].c_str();
								temp.remove("{");
								temp.remove("}");
								QStringList values = temp.split(",");
								if(offsets.size()>0)
								{
									for(int i=0;i<offsets.size();i++)
									{
										mEnvelopeTable->setRowCount(i+1);
										QTableWidgetItem* item = new QTableWidgetItem(offsets[i]);
										mEnvelopeTable->setItem(i,0,item);
									} // for
								} // if angles.size
								if(values.size()>0)
								{
									for(int i=0; i<values.size(); i++)
									{
										if(i>mEnvelopeTable->rowCount()-1)
											mEnvelopeTable->setRowCount(i+1);
								
										QTableWidgetItem* item = new QTableWidgetItem(values[i]);
										mEnvelopeTable->setItem(i,1,item);
									} // for
								} // if patternValues.size
						
							} // if r
						} // if PerformRawSql
						else
						{
							QTableWidgetItem* first = new QTableWidgetItem("0");
							QTableWidgetItem* second = new QTableWidgetItem("0");
							mEnvelopeTable->setItem(0,0,first);
							mEnvelopeTable->setItem(0,1,second);
						} // else PerformRawSql
								
						mFormWidgets.insert("envelopes",static_cast<QWidget*>(mEnvelopeTable));
						mFormLabels.insert("envelopes",new QLabel("Spectral Envelope"));
								
						mFormLayout->addWidget(mFormLabels["envelopes"],mLayoutRow,0,1,1,Qt::AlignTop);
						mFormLayout->addWidget(mFormWidgets["envelopes"],mLayoutRow++,1,1,1,Qt::AlignLeft);
									
						connect(mEnvelopeTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateEnvelopeRow(int,int)));
					} // if offsets
				} // else elevation elevationangels
			} // else azimuth azimuthangels
		} // if
		else
		{
//			cout << "cRapFormBuilder::CreateGenericUpdateForm () Not Antenna fields" << endl;
			// Check the type of the field
			if(uiType == cDatabase::utNormal)
			{
//				cout << " mLayoutRow = " << mLayoutRow << endl;
//				cout << header->data(Qt::DisplayRole).toString().toStdString().c_str() << endl;
				QString opskrif = header->data(Qt::DisplayRole).toString();
//				cout << fieldName.toStdString().c_str() << endl;
//				cout << item->data(Qt::DisplayRole).toString().toStdString().c_str() << endl;
//				cout << "cRapFormBuilder::CreateGenericUpdateForm Voor CreateFormWidget" << endl;
				CreateFormWidget(fieldName,opskrif,mFormWidgets,type,item->data(Qt::DisplayRole),mLayoutRow,readOnly);
//				cout << "cRapFormBuilder::CreateGenericUpdateForm Na CreateFormWidget" << endl;
				if(fieldName=="id")
				{
					id=item->data(Qt::DisplayRole).toInt();
					mCurrentRecordID=id;
				}
			} // if
			else
			{
//				cout << "cRapFormBuilder::CreateGenericUpdateForm Voor mFormWidgets.insert" << endl;
				mFormWidgets.insert(fieldName,static_cast<QWidget*>(new QComboBox(this)));
				
				// Populate the combobox with selection values
				StringIntArray vals;
				StringIntArrayIterator iterator;
				
//				cout << "cRapFormBuilder::CreateGenericUpdateForm Voor getUIParams" << endl;
				gDb.GetFieldUiParams(mTable.toStdString(),fieldName.toStdString(),vals);
				
				QComboBox* combo = static_cast<QComboBox*>(mFormWidgets[fieldName]);
				int j;
//				cout << "cRapFormBuilder::CreateGenericUpdateForm () 4444" << endl;				
				if(uiType == cDatabase::utDropdownTable)
				{
					j=1;
					combo->insertItem(0,"0:NULL");
				} // if uiType
				else
					j=0;
				
				// Poulate the combobox with the default data
				for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
				{
					QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
					combo->insertItem(j,temp,qVariantFromValue(iterator->first));
					
					j++;
				} // for
	
				
				combo->setCurrentIndex(combo->findText(item->data(Qt::DisplayRole).toString(),Qt::MatchCaseSensitive));
				
				// Create the form label
				mFormLabels.insert(fieldName,new QLabel(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mLabel),this));
				
				// Add the widget to the layout
				mFormLayout->addWidget(mFormLabels[fieldName],mLayoutRow,0,1,1,Qt::AlignLeft);
				mFormLayout->addWidget(mFormWidgets[fieldName],mLayoutRow,1,1,1,Qt::AlignLeft);
				
				// Set the widget's tooltip
				mFormWidgets[fieldName]->setToolTip(QString::fromStdString(mDbs[mTable.toStdString()].mFields[fieldName.toStdString()].mDescription));
				
				if(readOnly)
					combo->setEnabled(false);
				
				connect(combo,SIGNAL(currentIndexChanged(int)),this,SLOT(ContentChanged(int)));
			} // else
			if (fieldName=="groundheight")
			{
				mGroundHeightButton->setVisible(true);
				mGroundHeightButton->setEnabled(true);		
			}

//			cout << "cRapFormBuilder::CreateGenericUpdateForm () 5555" << endl;
			mLayoutRow++;
			mButtonRow = mLayoutRow;
		}
		
	} // for
	
//		cout << "cRapFormBuilder::CreateGenericUpdateForm () 6666" << endl;
	// If the entire row is readOnly then make sure that the location dialogs are made readonly
	if(rowReadOnly)
	{
		QMapIterator<QString,QWidget*> it(mFormWidgets);
		
		while(it.hasNext())
		{
			it.next();
			if(it.key().contains(":latitude"))
				static_cast<QLineEdit*>(it.value())->setEnabled(false);
			else
				if(it.key().contains(":longitude"))
				{
					static_cast<QLineEdit*>(it.value())->setEnabled(false);
					break;
				} // if contains
		} // while
	} // if readOnly
	
	// Create the mUpdate button
	mUpdateButton->setEnabled(false);
	mUpdate=false;
//	mUpdateButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
//	mNextButton->move(mButtonRow*mLayoutDelta,mButtonPosX2);
//	mPreviousButton->move(mButtonRow*mLayoutDelta,mButtonPosX1);
	mUpdateButton->setVisible(true);
	mNextButton->setVisible(true);
	mNextButton->setEnabled(true);
	mPreviousButton->setVisible(true);
	mPreviousButton->setEnabled(true);
	mButtonPosY = mUpdateButton->pos().y();
	if (mButtonRow)
		mLayoutDelta = mButtonPosY/mButtonRow;	
	else mLayoutDelta = mButtonPosY;
	setLayout(mFormLayout);

	delete [] text;
//	cout << "cRapFormBuilder::CreateGenericUpdateForm ()  Voor connect;"<<  endl;
	
//	connect(mPreviousButton,SIGNAL(clicked()),this,SLOT(PreviousRecord()));
//	connect(mNextButton,SIGNAL(clicked()),this,SLOT(NextRecord()));
//	connect(mUpdateButton,SIGNAL(clicked()),this,SLOT(Update()));
}


//*************************************************************************
// Create the antenna device form
void cRapFormBuilder::CreateAntennaDeviceForm ()
{
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setUnderline(false);
	font.setWeight(75);
	font.setStrikeOut(false);
	font.setKerning(true);
	QRadioButton*	fileRadio;		///< A pointer to a Qt4 radio button widget that determines whether an antenna will be imported from file.
	QRadioButton*	patternRadio;		///< A pointer to a Qt4 radio button widget that determines whether an antenna will be inputted manually.
	
	QMap<QString,QWidget*> *widgets;
	int mLayoutRow = 2;
	
	// iterate through the antennadevice form and the antenna pattern form
	for( int antenna=0; antenna<2 ;antenna++)
	{
		QString tableName;
		
		// antennadevice is the first set of form widgets and antennapattern is the second set of form widgets
		if(antenna == 0)
		{
			tableName = "antennadevice";
			
			mFormLabels.insert("title",new QLabel("Insert Form"),this);
			mFormLabels["title"]->setFont(font);
			mFormLayout->addWidget(mFormLabels["title"],mLayoutRow++,0,1,1,Qt::AlignLeft);
			
			fileRadio = new QRadioButton("Import antenna from file",this);
			patternRadio = new QRadioButton("Create new antenna",this);
			mBrowseButton = new QPushButton("Browse...");
			
			connect(fileRadio,SIGNAL(toggled(bool)),this,SLOT(AntennaDeviceFromFile(bool)));
			connect(patternRadio,SIGNAL(toggled(bool)),this,SLOT(AntennaDeviceFromManualInput(bool)));
			
			mFormLayout->addWidget(fileRadio,mLayoutRow,0,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(mBrowseButton,mLayoutRow++,1,1,1,Qt::AlignLeft);
			mFormLayout->addWidget(patternRadio,mLayoutRow++,0,1,1,Qt::AlignLeft);
			
			// add the widgets to the mFormWidgets map
			widgets = &mFormWidgets;
			
		} // if antenna==0
		else
		{
			tableName = "antennapattern";
			
			// Create the form widgets for the antenna pattern mTable
			mFormLabels.insert("pattern",new QLabel("Antenna Pattern"),this);
			mFormLabels["pattern"]->setFont(font);
			mFormLayout->addWidget(mFormLabels["pattern"],mLayoutRow++,0,1,1,Qt::AlignLeft);
			
			// add the widgets to the special mPatternWidgets map
			widgets = &mPatternWidgets;
		} // else antenna==0
		
		int size = mDbs[tableName.toStdString()].mFields.size();
		DbFieldMap::const_iterator patternIt;
		
		// Create the form
		for (int i=0 ; i<size ; i++)
		{
			// Get the relevant field in the creation order
			patternIt = gDb.GetFieldWithOrder(tableName.toStdString(),i);
			// This is just a conveniance variable to minimise the use of QString::fromStdString() function calls
			QString fieldName(patternIt->first.c_str());
			cDatabase::FieldUiType uiType;
			cDatabase::FieldDataType type;
			QVariant val;
			
			uiType = gDb.GetFieldUiType(tableName.toStdString(),patternIt->first);
			type = gDb.GetFieldDataType(tableName.toStdString(),patternIt->first);
			
			if(patternIt->second.mVisible)
			{
				if(uiType==cDatabase::utNormal)
				{
					// Do not create a form widget for the id field
					if(patternIt->first!="id" )
					{
						// Create custom tablewidgets for the azimuth and elevation values
						if(patternIt->first=="azimuthangles"||patternIt->first=="azimuthpattern"||patternIt->first=="elevationangles"||patternIt->first=="elevationpattern")
						{
							// Create one QTablewidget for the azimuth angles and values
							if(patternIt->first=="azimuthangles")
							{
								mAzimuthTable = new QTableWidget(1,2,this);
								QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
								QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
								
								anglesHeader->setData(Qt::UserRole,QVariant("azimuthangles"));
								patternHeader->setData(Qt::UserRole,QVariant("azimuthpattern"));
								
								mAzimuthTable->setHorizontalHeaderItem(0,anglesHeader);
								mAzimuthTable->setHorizontalHeaderItem(1,patternHeader);
								mAzimuthTable->verticalHeader()->setVisible(false);
								
								QTableWidgetItem* first = new QTableWidgetItem("0");
								QTableWidgetItem* second = new QTableWidgetItem("0");
								mAzimuthTable->setItem(0,0,first);
								mAzimuthTable->setItem(0,1,second);
								
								(*widgets).insert("azimuth",static_cast<QWidget*>(mAzimuthTable));
								mFormLabels.insert("azimuth",new QLabel("Azimuth Pattern"));
								
								mFormLayout->addWidget(mFormLabels["azimuth"],mLayoutRow,0,1,1,Qt::AlignTop);
								mFormLayout->addWidget((*widgets)["azimuth"],mLayoutRow++,1,1,1,Qt::AlignLeft);
								
								connect(mAzimuthTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateAzimuthRow(int,int)));
							}
							else
							{
								if(patternIt->first=="elevationangles")
								{
									mElevationTable = new QTableWidget(1,2,this);
									QTableWidgetItem* anglesHeader = new QTableWidgetItem("Angles");
									QTableWidgetItem* patternHeader = new QTableWidgetItem("Pattern Values");
								
									anglesHeader->setData(Qt::UserRole,QVariant("elevationangles"));
									patternHeader->setData(Qt::UserRole,QVariant("elevationpattern"));
								
									mElevationTable->setHorizontalHeaderItem(0,anglesHeader);
									mElevationTable->setHorizontalHeaderItem(1,patternHeader);
									mElevationTable->verticalHeader()->setVisible(false);
								
									QTableWidgetItem* first = new QTableWidgetItem("0");
									QTableWidgetItem* second = new QTableWidgetItem("0");
									mElevationTable->setItem(0,0,first);
									mElevationTable->setItem(0,1,second);
								
									(*widgets).insert("elevation",static_cast<QWidget*>(mElevationTable));
									mFormLabels.insert("elevation",new QLabel("Elevation Pattern"));
								
									mFormLayout->addWidget(mFormLabels["elevation"],mLayoutRow,0,1,1,Qt::AlignTop);
									mFormLayout->addWidget((*widgets)["elevation"],mLayoutRow++,1,1,1,Qt::AlignLeft);
									
									connect(mElevationTable,SIGNAL(cellChanged(int,int)),this,SLOT(CreateElevationRow(int,int)));
								} // if elevationangles
							} // else azimuth azimuthangels
						} // if azimuth , elevation
						else
						{
							CreateFormWidget(fieldName,QString::fromStdString(patternIt->second.mLabel),(*widgets),type,val,mLayoutRow, false);
							mLayoutRow++;
							mButtonRow = mLayoutRow;
						} // else azimuth, elevation
						
					} // if patternIt->first!="id"
				} // if uiType
				else
				{
					
					// if the dropdown mReferences an antenna device key it must be ignored
					if(fieldName=="antdevicekey")
						continue;
					
					// Create the combobox
					(*widgets).insert(fieldName,static_cast<QWidget*>(new QComboBox(this)));
					
					// Populate the combobox with selection values
					StringIntArray vals;
					StringIntArrayIterator iterator;
					
					gDb.GetFieldUiParams(tableName.toStdString(),patternIt->first,vals);
					
					
					QComboBox* combo = static_cast<QComboBox*>((*widgets)[fieldName]);
					
					int j;
					if(uiType==cDatabase::utDropdownTable)
					{
						j=1;
						combo->insertItem(0,"0:NULL");
					}
					else
						j=0;
					
					// Populate the combobox with the default data
					for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
					{
						QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
						combo->insertItem(j,temp,qVariantFromValue(iterator->first));
						
						j++;
					} // for
					
					// Add the form label
					mFormLabels.insert(fieldName,new QLabel(QString::fromStdString(patternIt->second.mLabel)),this);
					
					// Add the widget to the layout
					mFormLayout->addWidget(mFormLabels[fieldName],mLayoutRow,0,1,1,Qt::AlignLeft);
					mFormLayout->addWidget((*widgets)[fieldName],mLayoutRow,1,1,1,Qt::AlignLeft);
					
					// Set the widget's tooltip
					(*widgets)[fieldName]->setToolTip(QString::fromStdString(patternIt->second.mDescription));
					
					mLayoutRow++;
				} // else uiType
				mButtonRow = mLayoutRow;
			} // if mVisible
			
		} // for i
	} // for antenna

//	mCommitButton->move(mButtonRow*mLayoutDelta,mButtonPosX0);
	mCommitButton->setVisible(true);
	mCommitButton->setEnabled(true);	

	fileRadio->setChecked(true);
	
	connect(mCommitButton,SIGNAL(clicked()),this,SLOT(InsertAntenna()));
	connect(this,SIGNAL(InsertComplete(int)),parentWidget(),SLOT(setCurrentIndex(int)));
	connect(mBrowseButton,SIGNAL(clicked()),this,SLOT(ImportAntennaFromFile()));
	
	setLayout(mFormLayout);
}

