 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSettings.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Thane Thomson (roeland.frans@gmail.com)
 *		  : (Magdaleen Ballot (magdaleen.ballot@up.ac.za))
 *    Description : Settings and Structure store encapsulation, implementation
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
//\TODO We need to get rid of Poco for the xml parsing and rather use Qt

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <netinet/in.h>

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/AttrMap.h>
#include <Poco/DOM/Attr.h>
#include <Poco/DOM/ElementsByTagNameList.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/Exception.h>

#include "Config.h"

using namespace std;
using namespace Qrap;
using namespace Poco::XML;
using Poco::Exception;


// global settings store
cSettings Qrap::gSettings;

/*
 * cSettings class implementation
 */

//**********************************************************************************************************************
// constructor
cSettings::cSettings ()
{
	// nothing necessary
}

//**********************************************************************************************************************
bool cSettings::LoadFromFile (const string& fileName)
{
	ifstream settingsFile(fileName.c_str());
	
	msAlertCode = acOk;
	
	if (!settingsFile.is_open())
	{
		cout << "Cannot open \"" << fileName << "\" for reading." << endl;
		return false;
	}
	
	InputSource settingsSrc(settingsFile);
	
	// attempt to parse the settings file
	try
	{
		mSettingsDoc = mSettingsParser.parse(&settingsSrc);
		
		mSettingsRoot = mSettingsDoc->documentElement();
		
		// check that the settings root element's name is "settings"
		if (mSettingsRoot->nodeName() != "settings")
		{
			cout << fileName << " must contain a <settings> element as its root element." << endl;
			return false;
		}
		
		Element* el = mSettingsRoot->getChildElement("logfile");
		if ((!el) || (!el->hasAttribute("location")))
		{
			cout << "WARNING: No log file location specified in settings file. Using \"./qrap.log\"." << endl;
			msLogFileName = "./qrap.log";
		} else
		{
			msLogFileName = el->getAttribute("location");
		}
		
		QRAP_INFO("-----------Begin debug logging-----------");
		// check the database element
		el = mSettingsRoot->getChildElement("database");
		if ((!el) || (!el->hasAttribute("host")) || (!el->hasAttribute("name")))
		{
			QRAP_FATAL_CODE(fileName+" must contain a <database> element with connection parameters.", acInternalError);
			return false;
		}
		
		#ifndef QRAP_SERVER_EDITION
		// check that the synchronisation server details are in
		el = mSettingsRoot->getChildElement("syncserver");
		if ((!el) || (!el->hasAttribute("host")) || (!el->hasAttribute("port")))
		{
			QRAP_FATAL_CODE(fileName+" must contain a <syncserver> element with connection parameters.", acInternalError);
			return false;
		}
		#endif
		
		// check the structure element
		el = mSettingsRoot->getChildElement("structure");
		if ((!el) || (!el->hasAttribute("location")))
		{
			QRAP_FATAL_CODE(fileName+" must contain a reference to a structure XML file.", acInternalError);
			return false;
		}
		
		// open up the structure file
		string   structFileName = el->getAttribute("location");
		ifstream structFile(structFileName.c_str());

		// check that the file's open
		if (!structFile.is_open())
		{
			QRAP_FATAL_CODE("Cannot open \""+structFileName+"\" for reading.", acFileOpenRead);
			return false;
		}
		
		// create an input source
		InputSource       structSrc(structFile);
		DOMParser         structParser;
		AutoPtr<Document> structDoc = structParser.parse(&structSrc);
		Element*          structRoot = structDoc->documentElement();

		// check that the structure file contains a <structure> element
		if ((!structRoot) || (structRoot->nodeName() != "structure"))
		{
			QRAP_FATAL_CODE(structFileName+" must contain a <structure> root element.", acParse);
			return false;
		}
		
		// get the "table" children
		ElementsByTagNameList* tables = dynamic_cast<ElementsByTagNameList*>(structRoot->getElementsByTagName("table"));
		ElementsByTagNameList* fields, *views;
		int                 i, tableCount, j, k, fieldCount, viewCount;
		unsigned	refPos;
		Element*            curTable, *curField, *curView;
		string              tableName, fieldName, temp, viewName, viewAs;
		
		tableCount = tables->length();
		if (!tableCount)
		{
			QRAP_FATAL_CODE(structFileName+" must contain a database structure with at least one <table> tag.", acParse);
			return false;
		}

		// run through the tables
		for (i=0;i<tableCount;i++)
		{
			// get the current table
			curTable = dynamic_cast<Element*>(tables->item(i));
			// skip the table if it doesn't have a name
			if (!curTable->hasAttribute("name"))
			{
				char *text = new char[33];
				gcvt(i,8,text);
				string error = "Table found in structure file without name. Entry ";
				error+= text;
				QRAP_WARN(text);
				delete [] text;
				continue;
			}
			
			// get the table's name
			tableName = curTable->getAttribute("name");
			
			// get this table's views
			views = dynamic_cast<ElementsByTagNameList*>(curTable->getElementsByTagName("view"));
			
			// if there are any views
			if (views && (views->length() > 0))
			{
				// run through the views
				viewCount = views->length();
				for (j=0;j<viewCount;j++)
				{
					curView = dynamic_cast<Element*>(views->item(j));
					// check that the view has a name
					if (!curView->hasAttribute("name"))
					{
						QRAP_WARN("View found in table \""+tableName+"\" with no name. Skipping.");
						continue;
					}
					viewName = curView->getAttribute("name");
					// check that the view has an "as" clause
					if (!curView->hasAttribute("as"))
					{
						QRAP_WARN("View \""+tableName+"."+viewName+"\" has no \"as\" clause. Skipping.");
						continue;
					}
					// get the "as" clause
					mStructure[tableName].mViews[viewName].mAs = curView->getAttribute("as");
					// get any extra field labels from the view
					fields = dynamic_cast<ElementsByTagNameList*>(curView->getElementsByTagName("field"));
					if (fields && (fields->length() > 0))
					{
						// save the extra field labels
						fieldCount = fields->length();
						for (k=0;k<fieldCount;k++)
						{
							curField = dynamic_cast<Element*>(fields->item(k));
							// check that the field has both "name" and "label" attributes
							if (!curField->hasAttribute("name") || !curField->hasAttribute("label"))
							{
								QRAP_WARN("Fields in a view must have both a name and label. Skipping entry.");
								continue;
							}
							// add the field to the view
							fieldName = curField->getAttribute("name");
							mStructure[tableName].mViews[viewName].mFieldLabels[fieldName] = curField->getAttribute("label");
						}
					}
				}
			}

			// get the fields
			fields = dynamic_cast<ElementsByTagNameList*>(curTable->getElementsByTagName("field"));
			
			if ((!fields) || (fields->length() == 0))
			{
				QRAP_WARN("Empty table \""+tableName+"\" found in structure file.");
				continue;
			}
			// get the number of fields in this table
			fieldCount = fields->length();
			// set up this table
			mStructure[tableName].mLabel = curTable->getAttribute("label");
			mStructure[tableName].mDescription = curTable->getAttribute("description");
			mStructure[tableName].mFooter = curTable->getAttribute("footer");
			// if the table is explicitly invisible, set it to invisible
			mStructure[tableName].mVisible = (curTable->getAttribute("visible") == "false") ? false : true;
			mStructure[tableName].mCreateOrder = i;
			// if the table is explicitly set as full download
			mStructure[tableName].mFullDownload = (curTable->getAttribute("fulldownload") == "true") ? true : false;
			// if the table is explicitly set as having a custom sequence
			mStructure[tableName].mCustomSequence = (curTable->getAttribute("customsequence") == "true") ? true : false;

			// run through the fields
			for (j=0;j<fieldCount;j++)
			{
				curField = dynamic_cast<Element*>(fields->item(j));
				
				if (!curField->hasAttribute("name"))
				{
					QRAP_WARN("Field found in table \""+tableName+"\" with no name. Skipping it.");
					continue;
				}
				
				fieldName = curField->getAttribute("name");
				
				if (!curField->hasAttribute("type"))
				{
					QRAP_WARN("Field \""+fieldName+"\" in table \""+tableName+"\" has no type. Skipping.");
					continue;
				}
				
				// set up the field
				mStructure[tableName].mFields[fieldName].mType = curField->getAttribute("type");
				mStructure[tableName].mFields[fieldName].mLabel = curField->getAttribute("label");
				mStructure[tableName].mFields[fieldName].mDescription = curField->getAttribute("description");
				// set the visibility flag if it is explicitly set
				mStructure[tableName].mFields[fieldName].mVisible = (curField->getAttribute("visible") == "false") ? false : true;
				// set the read-only flag if it is explicitly set
				mStructure[tableName].mFields[fieldName].mReadOnly = (curField->getAttribute("readonly") == "true") ? true : false;
				mStructure[tableName].mFields[fieldName].mUi = curField->getAttribute("ui");
				mStructure[tableName].mFields[fieldName].mOrder = j;
				
				// check if this field is a foreign key reference...
				temp = mStructure[tableName].mFields[fieldName].mType;
				refPos = temp.find("references");
				// if this key is a reference to a foreign key
				if (refPos < temp.length())
				{
					mStructure[tableName].mFields[fieldName].mIsForeign = true;
					// find the first non-whitespace character after the "references" keyword (length 10 characters)
					refPos = FindNonWhitespace(temp, refPos+10);
					mStructure[tableName].mFields[fieldName].mForeignTable = ExtractKeyword(temp, refPos);
					// if there is no foreign table
					if (mStructure[tableName].mFields[fieldName].mForeignTable.length() == 0)
					{
						QRAP_FATAL_CODE("Invalid data type specified in \""+tableName+"."+fieldName+"\". Reference must be to a table's primary key.", acParse);
						return false;
					}
				}
				
				// is this field a file link?
				mStructure[tableName].mFields[fieldName].mIsFileLink = (curField->getAttribute("filelink") == "true") ? true : false;
				// is this field specific to the server?
				mStructure[tableName].mFields[fieldName].mServerOnly = (curField->getAttribute("serveronly") == "true") ? true : false;
				// is this field specific to the client?
				mStructure[tableName].mFields[fieldName].mClientOnly = (curField->getAttribute("clientonly") == "true") ? true : false;
				// is this field a PostGIS field?
				mStructure[tableName].mFields[fieldName].mPostGis = (curField->getAttribute("postgis") == "true") ? true : false;
				// does this field contain a unit type? default: utNone
				temp = StrToLower(curField->getAttribute("unittype"));
				if (temp == "power")
					mStructure[tableName].mFields[fieldName].mUnitType = utPower;
				else if (temp == "sensitivity")
					mStructure[tableName].mFields[fieldName].mUnitType = utSensitivity;
				else if (temp == "eirp")
					mStructure[tableName].mFields[fieldName].mUnitType = utEirp;
				else if (temp == "height")
					mStructure[tableName].mFields[fieldName].mUnitType = utHeight;
				else if (temp == "dbell")
					mStructure[tableName].mFields[fieldName].mUnitType = utLossGain;
				else if (temp == "dbell")
					mStructure[tableName].mFields[fieldName].mUnitType = utLossGain;
				else if (temp == "megahertz")
					mStructure[tableName].mFields[fieldName].mUnitType = utMegaHertz;
				else if (temp == "kilohertz")
					mStructure[tableName].mFields[fieldName].mUnitType = utkiloHertz;
				else if (temp == "degrees")
					mStructure[tableName].mFields[fieldName].mUnitType = utAngle;
				else if (temp == "distance")
					mStructure[tableName].mFields[fieldName].mUnitType = utDistance;
			}
		}
		
	} catch (Exception& e)
	{
		QRAP_FATAL_CODE("XML error: "+e.displayText(), acFileOpenRead);
	}
	if (msAlertCode != acOk)
		return false;
	
	return true;
}


//********************************************************************************************************
cSettings::~cSettings ()
{
	//QRAP_INFO("-----------End debug logging-----------\n");
}

//************************************************************************************************************
string cSettings::GetValue (const string& name) const
{
	StringArray arr;
	int         i, count;
	Element*    el = mSettingsRoot;
	
	// split the specified name
	if (!SplitString(name, '.', arr)) return "";
	
	// traverse the path to the setting
	count = arr.size()-1;
	for (i=0;i<count;i++)
	{
		el = el->getChildElement(arr[i]);
		if (!el)
		{
			QRAP_WARN("No path to settings element \""+name+"\".");
			return "";
		}
	}
	
	if (!el->hasAttribute(arr[count]))
	{
		QRAP_WARN("No path to settings attribute \""+name+"\".");
		return "";
	}
	
	return el->getAttribute(arr[count]);
}
