 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : RapDbCommunicator.cpp
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
 
#include "Config.h"
#include "RapDbCommunicator.h"
#include "LogDialog.h"

using namespace Qrap;
using namespace std;

//*************************************************************
// Constructor
RapDbCommunicator::RapDbCommunicator (QWidget* parentWidget,
				      QProgressBar* progressBar)
{
	parent = parentWidget;
	progress = progressBar;
	
	// Build the custom query and load the header of the table
	gDb.GetDbStructure(dbs);
}


//***************************************************************
// Delete the selected rows
void RapDbCommunicator::DeleteRows (QTableWidget* table,
				    QString tableName)
{
	// Get the list of selection ranges 
	QList<QTableWidgetSelectionRange> selection = table->selectedRanges();
	QListIterator<QTableWidgetSelectionRange> it(selection);	
	bool error = false;
	
	// Iterate through the Selection ranges
	while(it.hasNext())
	{
		QTableWidgetSelectionRange range = it.next();
		int start,end;
		
		if(range.topRow()>range.bottomRow())
		{
			start = range.bottomRow();
			end = range.topRow();
		} // if
		else
		{
			start = range.topRow();
			end = range.bottomRow();
		} // else
		
		table->setCurrentCell(start,0);
		
		// Get the contents of the rows and columns for the range
		for( int i=start ; i<=end ; i++)
		{
			
			string fieldName = table->horizontalHeaderItem(0)->data(Qt::UserRole).toString().toStdString();
			string val = table->item(i,0)->data(Qt::DisplayRole).toString().toStdString();
			string whereClause = fieldName + "=" + val;
			
			cout << "whereClause" << whereClause << endl;
			
			// Delete the row from the database
			if(!gDb.Delete(tableName.toStdString(),whereClause))
			{
				// A QRAP_ERROR,QRAP_INFO,QRAP_WARN or QRAP_FATAL is called by the cDatabase class and handled by Qrap::cMainWindow::Alert()
				error = true;
				break;
			} // if
		} // for
		
		if(!error)
		{
			
			if(!table->model()->removeRows(start,range.rowCount()))
			{
				cout << "Could not delete rows" << endl;
			} // if
		}
		else
		{
			break;
		} // else
	} // while
}


//******************************************************************
// Update the database
void RapDbCommunicator::UpdateDatabase(QTableWidget* table,
				       QString tableName,
				       int row,
				       int col)
{
	// \TODO: Do proper type convertion in UpdateDatabase() to support Postgres 8.3.

	if (row<0) row = 0;
	else if (row>table->rowCount()-1) row = table->rowCount()-1;	
	table->selectRow(row);
	cout << "RapDbCommunicator::UpdateDatabase 	row x col = " << row << " x " << col << endl;
	StringMap tableData;
	string fieldName = table->horizontalHeaderItem(col)->data(Qt::UserRole).toString().toStdString();
	cout << "RapDbCommunicator::UpdateDatabase 	fieldName = " << fieldName.c_str() << endl;
	cDatabase::FieldUiType uiType = gDb.GetFieldUiType(tableName.toStdString(),fieldName);
	cout << "RapDbCommunicator::UpdateDatabase 	uiType = " << uiType << endl;
	QTableWidgetItem* updateItem = table->item(row,col);
	cout << "RapDbCommunicator::UpdateDatabase 	*table " << table << endl;

	// Check if the dropdown delegate gets it's contents from a table list, if so the number must be written to the database
	if(uiType==cDatabase::utDropdownTable)
	{
		QStringList parts;
		parts = updateItem->data(Qt::DisplayRole).toString().split(":");
		
		// Insert the data
		if(parts.size()>1)
		{
			if(parts[1]=="NULL")
				tableData[fieldName] = "NULL";
			else
				tableData[fieldName] = parts[0].toStdString();
		}
		else
			tableData[fieldName] = "0";
	}
	else
	{
		// Check if the dropdown delegate gets it's contents from a fixed list, if so the text must be written to the database not the number
		if(uiType==cDatabase::utDropdownFixed)
		{
			QStringList parts;
			parts = updateItem->data(Qt::DisplayRole).toString().split(":");
			
			// Insert the data
			tableData[fieldName] = "'" + parts[1].toStdString() + "'";
		}
		else
		{
			// Insert the data
			tableData[fieldName] = "'" +  updateItem->data(Qt::DisplayRole).toString().toStdString() + "'";
		} // else
	} // else
	
	string id;
	
	cout << "RapDbCommunicator::UpdateDatabase 	table->columnCount() = " << table->columnCount() << endl;
	// Perform the update query
	for(int i=0; i < table->columnCount() ; i++)
	{
		if(table->horizontalHeaderItem(i)->data(Qt::UserRole).toString() == "id")
		{
			id = table->item(table->currentRow(),i)->data(Qt::DisplayRole).toString().toStdString();
			break;
		} // if
	} // for
	
	string whereClause = "id = " + id;
	
	cout << "RapDbCommunicator::UpdateDatabase 	whereClause = " << whereClause.c_str() << endl;
	if(gDb.GetSetting("mode")=="normal")
	{
		if(!gDb.Update(tableData,tableName.toStdString(),whereClause))
		{
			// A QRAP_ERROR,QRAP_INFO,QRAP_WARN or QRAP_FATAL is called by the cDatabase class and handled by Qrap::cMainWindow::Alert()
			
			// Clear the items data on error
			updateItem->setData(Qt::DisplayRole,QVariant::QVariant(""));
		} // if
	} // if mode
	else
	{
		if(!gDb.SetDefaults(tableName.toStdString(),tableData))
		{
			updateItem->setData(Qt::DisplayRole,QVariant::QVariant(""));
		} // if
	} // if mode
}


//************************************************************************
// Load the table headers
void RapDbCommunicator::LoadTableHeaders(QTableWidget* table,
					 QString tableName,
				         QString keyField,
					 bool view)
{	
	int size = dbs[tableName.toStdString()].mFields.size();
	int j = 0;
	DbFieldMap::const_iterator it;
	
	// Load the table headers
	for( int i=0 ; i<size ; i++)
	{
		it = gDb.GetFieldWithOrder(tableName.toStdString(),i);
		
		if(it->second.mVisible)
		{
			
			// Store the descriptive name and the database table name in the table header
			QTableWidgetItem* headerItem = new QTableWidgetItem(QString::fromStdString(it->second.mLabel));
			headerItem->setData(Qt::UserRole,QVariant::QVariant(QString::fromStdString(it->first)));
			
			table->setColumnCount(j+1);
			
			// Setup the column width
			if(10*headerItem->data(Qt::DisplayRole).toString().length() > 70)
				table->setColumnWidth(j,10*headerItem->data(Qt::DisplayRole).toString().length());
			else
				table->setColumnWidth(j,70);
			
			table->setHorizontalHeaderItem(j,headerItem);
			
			// Check the user interface data type and if it is a dropdown box then create a combo delegate
			cDatabase::FieldUiType uiType = gDb.GetFieldUiType(tableName.toStdString(),it->first);
			
			if( (uiType!=cDatabase::utNormal) )
			{
				ComboDelegate* combo = new ComboDelegate(parent);
				combo->tableName = tableName;
				combo->fieldName = QString::fromStdString(it->first);
				
				// Set the column delegate to be the combobox
				table->setItemDelegateForColumn(j,combo);
			} // if
			
			j++;
		} // if
		
		// check if this table is defined by a custom view
		if(i==0 && view==true)
		{
			if( dbs[tableName.toStdString()].mViews.size()>0)
			{
				StringMap::const_iterator viewFieldIt;
				
				// There is only one view per table
				DbView viewStruct = dbs[tableName.toStdString()].mViews[tableName.toStdString()+"_"+"view"];
				
				for(viewFieldIt=viewStruct.mFieldLabels.begin() ; viewFieldIt!=viewStruct.mFieldLabels.end() ; viewFieldIt++)
				{
					QTableWidgetItem* header = new QTableWidgetItem(QString::fromStdString(viewFieldIt->second));
					header->setData(Qt::UserRole,QVariant::QVariant(QString::fromStdString(viewFieldIt->first)));
					table->setColumnCount(j+1);
					table->setHorizontalHeaderItem(j,header);
					
					// Setup the column width
					if(10*header->data(Qt::DisplayRole).toString().length() > 70)
						table->setColumnWidth(j,10*header->data(Qt::DisplayRole).toString().length());
					else
						table->setColumnWidth(j,70);
					
					j++;
				} // for
			} // if dbs
		} // if i && view
		
	} // for
}


//********************************************************************************
void RapDbCommunicator::GetCellDataType(std::string tableName,
					std::string fieldName,
					QVariant& val,
				        DegreeFormat locationFormat)
{
	cDatabase::FieldUiType uiType;
	cDatabase::FieldDataType type;
	UnitType unit;
	
	if(dbs[tableName].mFields.count(fieldName)>0)
	{
		uiType = gDb.GetFieldUiType(tableName,fieldName);
		type = gDb.GetFieldDataType(tableName,fieldName);
		if(dbs[tableName].mFields.count(fieldName)>0)
			unit = dbs[tableName].mFields[fieldName].mUnitType;
		else unit = utNone;
	}
	else
	{
		uiType = cDatabase::utNormal;
		type = cDatabase::dtString;
		unit = utNone;
	}
	string latitude,longitude;
	
	// Check what the type of the field is
	if(uiType == cDatabase::utNormal)
	{
		switch(type)
		{
			case cDatabase::dtPostGisPoint:
				ExtractLatLongFromPoint(val.toString().toStdString(),locationFormat,latitude,longitude);
				val.clear();
				val.setValue(QString::fromStdString(latitude + " | " + longitude));
				// Force the location column to be non-editable
				break;
			case cDatabase::dtString:
				break;
			case cDatabase::dtInteger:
				val.convert(QVariant::Int);
				break;
			case cDatabase::dtReal:
				val.convert(QVariant::Double);
				break;
			case cDatabase::dtDate:
				val.convert(QVariant::Date);
				break;
			case cDatabase::dtDateTime:
				val.convert(QVariant::DateTime);
				break;
			default:
				break;
		} // switch
	} // if
	else
	{
		// Get the parameters for the combo box
		StringIntArray comboParams;
		QString temp;
		
		if(uiType==cDatabase::utDropdownTable)
		{
			
			gDb.GetFieldUiParams(tableName,fieldName,comboParams);
			
			// If the uiType is not normal then it is a dropDown box which means that the TableWidgetItem needs two results
			val.convert(QVariant::Int);
			
			// Get the name that is related to the foreign key stored in val
			temp = QString::number(val.toInt());
			
			if(comboParams[val.toInt()]=="")
				temp += ":NULL";
			else
				temp += ":" + QString::fromStdString(comboParams[val.toInt()]);
		} // if
		else
		{
			gDb.GetFieldUiParams(tableName,fieldName,comboParams);
			
			StringIntArrayIterator it;
			
			// Check if the fixed column type is a boolean
			if(type==cDatabase::dtBoolean)
			{
				if(val.toString() == "t")
					temp = "1:TRUE";
				else
					temp = "0:FALSE";
			} // if dtBoolean
			else
			{
				for( it=comboParams.begin() ; it != comboParams.end() ; it++)
				{
					if (val.toString() == QString::fromStdString(it->second) )
					{
						temp = QString::number(it->first) + ":" + QString::fromStdString(it->second);
						break;
					}
					else
						temp = "";
				} // for
			} // else dtBoolean
		} // else
		
		val.setValue(temp);
	} // else
}


//*************************************************************************
// Populate the selected table
void RapDbCommunicator::PopulateTable (QTableWidget* table,
				       const QString& tableName,
				       const QString& keyField,
				       int currentId,
				       bool site,
				       bool view,
				       string search)
{
	cout << "Entering RapDbCommunicator::PopulateTable tableName = " << tableName.toStdString().c_str() << endl;
	if(table!=NULL)
	{
		
		string selectQuery;
		
		// Disable sorting
		table->setSortingEnabled(false);
		
		// Make sure that the table is cleared of it's current data
		table->clear();
		table->setRowCount(0);
		
		int size = dbs[tableName.toStdString()].mFields.size();
//		int j = 0;
		DbFieldMap::const_iterator it;
		
 		cout << "RapDbCommunicator::PopulateTable size = " << size << endl;
		for( int i=0; i<size; i++)
		{
			it = gDb.GetFieldWithOrder(tableName.toStdString(),i);
			
			if(it->second.mVisible)
			{
				if(i!=0)
					selectQuery += ",";
				
				if(it->second.mUnitType!=utNone)
					selectQuery += ConvertFromdBm(it->second.mUnitType,it->first);
				else if (it->second.mPostGis)
					selectQuery +="AsText ("+it->first+") as "+it->first;
				else
					selectQuery += it->first;
			} // if mVisible
			
			// append the extra columns to the select query 
			if(i==0 && view==true)
			{
				if(dbs[tableName.toStdString()].mViews.size()>0)
				{
					cout << "RapDbCommunicator::PopulateTable in double if"  << endl;
					StringMap::const_iterator viewFieldIt;
				
					// There is only one view per table
					DbView viewStruct = dbs[tableName.toStdString()].mViews[tableName.toStdString()+"_"+"view"];
				
					for(viewFieldIt=viewStruct.mFieldLabels.begin() ; viewFieldIt!=viewStruct.mFieldLabels.end() ; viewFieldIt++)
					{
						selectQuery += ",\"" + viewFieldIt->first + '"';
					} // for
				} // if dbs
			} // if i && view
		} // for
		
		LoadTableHeaders(table,tableName,keyField,view);
				
		string whereClause;
		bool hasWriteAccess = true;
		bool success;
		
		// Check if a where clause  is needed
		if(currentId!=-1)
		{
			whereClause = keyField.toStdString() + " = " + QString::number(currentId).toStdString();
		} // if currentId
		else
		{
			whereClause = search;
		} // else currentId
		
		// Perform a different query if this is for a view
		if(view)
		{
			if(gDb.GetSetting("mode")=="defaults")
				selectQuery = "select distinct " +  selectQuery + " from " + tableName.toStdString() + "_"+"view where id=0;";
			else
			{
				selectQuery = "select distinct " +  selectQuery + " from " + tableName.toStdString() + "_"+"view where id>0";
			
				if(currentId!=-1) 
					selectQuery +=" and " +whereClause +";";
				else selectQuery +=";";
			}
			
			success = gDb.PerformRawSql(selectQuery);
		} // if view
		else 
		{
			if(gDb.GetSetting("mode")=="defaults")
				success = gDb.Select(selectQuery,tableName.toStdString(),"id=0","id",true,false);
			else 
				success = gDb.Select(selectQuery,tableName.toStdString(),whereClause);
		} // else view
		
		
		if(!success)
		{
			cout << "Error in query to find data...in RapDbCommunicator::PopulateTable" << endl;
		}
		else
		{
			// Get the result for the select query
			pqxx::result r;
			
			gDb.GetLastResult(r);
			
			int rowCount = (int)r.size();
			int colCount = table->columnCount();
			QAbstractItemModel* tableModel = table->model();
			QModelIndex index;
			
			if(progress!=NULL)
			{
				progress->setVisible(true);
				progress->reset();
				progress->setMinimum(0);
				progress->setMaximum(rowCount-1);
			} // if
			
			// Get the location format from the settings table
			DegreeFormat locationFormat;
			
			string setting = gDb.GetSetting("location");
			
			if (setting=="DD:MM:SS")
				locationFormat = dfDegMinSec;
			else
			if (setting=="DD:MM.mm")
				locationFormat = dfDegMin;
			else
			if (setting=="DD.dddd")
				locationFormat = dfDeg;
			else
				locationFormat = dfDeg;
				
			string FilterString = "SELECT id FROM site_view";
			pqxx::result filterRes;
			if (!gDb.PerformRawSql(FilterString))
			{
				cout << "Error finding filtered data...in RapDbCommunicator::PopulateTable" << endl;
				filterRes.clear();;
			}
			else
			{
				gDb.GetLastResult(filterRes);
			}
			
			cout << "RapDbCommunicator::PopulateTable	rowCount =" << rowCount << endl;
			for( int i=0 ; i<rowCount ; i++)
			{
				tableModel->insertRow(i);
				
				// Indicate progress
				if(progress!=NULL)
					progress->setValue(i);
				
				// Check whether the user has access to the current row based on the site status, note that this is specific to the site table.
				if(site)
				{
					if( string(r[i]["status"].c_str())!="Active" )
						hasWriteAccess = true;
					else
						hasWriteAccess = false;
				} // if site
				
				bool isFiltered = false;
//				cout << "RapDbCommunicator::PopulateTable	colCount =" << colCount << endl;
				for( int j=0 ; j<colCount ; j++)
				{
					// Get the necessary data
					string fieldName = table->horizontalHeaderItem(j)->data(Qt::UserRole).toString().toStdString();
//					cout << "RapDbCommunicator::PopulateTable	fieldname =" << fieldName.c_str() << endl;
					QVariant val( r[i][fieldName].c_str() );
					index = tableModel->index(i,j);
//					cout << "RapDbCommunicator::PopulateTable	field =" << r[i][fieldName].c_str() << endl;
					if ((site) && (j == 0) && (filterRes.size() > 0))
					{
						for (unsigned g = 0; g < filterRes.size() ; g++)
						{
							if (val.toString().toStdString() == filterRes[g]["id"].c_str())
							{
								isFiltered = false;
								break;
							}
						}
					}
					else
					{
						if ((filterRes.size() > 0) && (!site))
						{
							for (unsigned g = 0; g < filterRes.size() ; g++)
							{
								if (val.toString().toStdString() == filterRes[g]["id"].c_str())
								{
									isFiltered = false;
									break;
								}
							}
						}
					}
												
					// Get the dataype of the cell
					GetCellDataType(tableName.toStdString(),fieldName,val,locationFormat);

//					cout << "RapDbCommunicator::PopulateTable 	val = " <<val.toString().toStdString().c_str() << endl;
//					cout << "RapDbCommunicator::PopulateTable	voor tableModel->setData" << endl;
					tableModel->setData(index,val,Qt::DisplayRole);
//					cout << "RapDbCommunicator::PopulateTable	after tableModel->setData" << endl;
					//Check if the item has read access or not
					if(dbs[tableName.toStdString()].mFields.count(fieldName)>0)
					{
						cDatabase::FieldDataType type = gDb.GetFieldDataType(tableName.toStdString(),fieldName);
//						cout << "RapDbCommunicator::PopulateTable	type = " << type << endl;
						if (isFiltered)
						{
							table->item(i,j)->setBackground(QBrush(Qt::lightGray));
						}
						if( (hasWriteAccess == false) || (dbs[tableName.toStdString()].mFields[fieldName].mReadOnly) || type==cDatabase::dtPostGisPoint)
						{
							table->item(i,j)->setFlags(Qt::ItemIsSelectable);
						} // if
					} // if dbs
					else
						table->item(i,j)->setFlags(Qt::ItemIsSelectable);
//					cout << "RapDbCommunicator::PopulateTable	after formatting of cells" << endl;
				} // for
			} // for
			
		} // else
		
		// Enabled sorting
		table->setSortingEnabled(true);
//		cout << " RapDbCommunicator::PopulateTable tableName = " <<tableName.toStdString().c_str()
//				<<"		rowCount=" << table->rowCount() << endl;
		
		// Reset the progress bar
		if(progress!=NULL)
			progress->setVisible(false);
	} // if
	cout << "Leaving RapDbCommunicator::PopulateTable tableName = " << tableName.toStdString().c_str() << endl;
}


//**********************************************************************
// Convert from a specified format to dBm
std::string RapDbCommunicator::ConvertTodBm (UnitType unit,
			  double value)
{
	string RValue;
	char text[33];
	string setting;
	
	switch(unit)
	{
		case utPower:
			setting = gDb.GetSetting("power");
			break;
		case utSensitivity:
			setting = gDb.GetSetting("sensitivity");
			break;
		case utEirp:
			setting = gDb.GetSetting("eirp");
			break;
		default:
			setting = "";
			break;
	} // switch
	
	if (setting=="dBW")
	{
		gcvt(value,10,text);
		RValue = text;
		RValue += " + 30"; 
		return RValue;
	}
	else if(setting=="dBm")
	{
		gcvt(value,10,text);
		RValue = text;
		return RValue;
	}
	else if(setting=="W")
	{
		if (value<=0.0)
		{		
			string err = "You have entered 0 Watt or negative value transmitting power ... assuming a dBm value ? ";
			QRAP_WARN(err.c_str());
			gcvt(value,10,text);
			RValue = text;
			return RValue;
		}
		else 
		{
			RValue="10*log(";
			gcvt(value,10,text);
			RValue += text;
			RValue += ") + 30";
			return RValue;
		}
	}
	else if(setting=="dB\316\274V")
	{
		gcvt(value,10,text);
		RValue = text;
		RValue += " - 10*log(";
		RValue += gDb.GetSetting("impedence");
		RValue += ") - 90";
		return RValue;
	}
	else 
	{
		gcvt(value,10,text);
		RValue = text;
		return RValue;
	}
}

//***********************************************************************
// Convert from dBm to a specified format
std::string RapDbCommunicator::ConvertFromdBm (UnitType unit,
					       const string& fieldName)
{
	string setting;
	
	switch(unit)
	{
		case utPower:
			setting = gDb.GetSetting("power");
			break;
		case utSensitivity:
			setting = gDb.GetSetting("sensitivity");
			break;
		case utEirp:
			setting = gDb.GetSetting("eirp");
			break;
		default:
			setting = "";
			break;
	} // switch
	
	
	if(setting=="dBm")
		return fieldName;
	else
	if(setting=="dBW")
		return fieldName + " - 30 as " + fieldName;
	else
	if(setting=="W")
		return  "(power(10,(" + fieldName + " - 30)/10)) as " + fieldName;
	else
	if(setting=="dB\316\274V")
		return fieldName + " + 10*log(" + gDb.GetSetting("impedence") + ") + 90 as " + fieldName;
	else return fieldName;
}
