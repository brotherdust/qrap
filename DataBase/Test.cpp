 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Test.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Thane Thomson (roeland.frans@gmail.com)
 *    Description : Mian type Test, also used in Database creation utility
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

#define QRAP_DEBUG
#include "Config.h"
#include "cDatabase.h"

using std::string;
using namespace Qrap;


int main (int argc, char** argv)
{
	cout << "Loading settings..." << endl;
	if (!gSettings.LoadFromFile("settings.xml"))
	{
		cout << "Error while loading settings from settings.xml" << endl;
		return 1;
	}
	cout << "Loaded settings." << endl;
	
	if (argc >= 2)
	{
		if (argv[1][0] == 'c')
		{
			cDatabase::GroupPrivileges priv;
			
			// create database
			cout << "Creating database..." << endl;
			if (!gDb.Create("postgres", "postqrap"))
			{
				cout << "Unable to create database." << endl;
				return 1;
			}
			cout << endl << "Success." << endl;
			
			return 0;
		}
	
		// log in as root
		if (!gDb.Connect("postgres", "postqrap"))
		{
			cout << "Could not log in." << endl;
		} else
		{
			cout << "Successfully logged in." << endl;
			
			StringArray    cols;
			StringIntArray vals;
			IntArray       failed;
			StringMap      values;
			StringIntArray params;
			unsigned char  md5[16];
			string         str, la, lo;
			int            i;
			
			switch (argv[1][0])
			{
			// insert test
			case 'i':
				cout << "Attempting INSERT..." << endl;
			
				values["groundheight"] = "19";
				values["location"] = "POINT(4 5)";
				values["sitename"] = "'Kalahari'";
				values["status"] = "'Experimental'";
			
				if (!gDb.Insert(values, "site"))
				{
					cout << "Insert failed." << endl;
				} else
				{
					cout << "Successful." << endl;
				}
				break;
			// select test
			case 's':
				cout << "Attempting SELECT..." << endl;
				
				if (!gDb.Select("*", "site", ""))
				{
					cout << "Select failed." << endl;
				} else
				{
					pqxx::result r;
					
					gDb.GetLastResult(r);
					
					cout << "Successful. Results: " << (int)r.size() << ". Listing:" << endl;
					for (int i=0;i<r.size();i++)
					{
						cout << "Entry " << i << endl;
						// output the results
						for (pqxx::result::const_fielditerator it=r[i].begin();it != r[i].end();it++)
						{
							cout << string(it->name()) << ": " << it->c_str() << endl;
						}
						cout << endl;
					}
				}
				break;
			// update test
			case 'u':
				cout << "Attempting UPDATE..." << endl;
				
				values["sitename"] = "'Bethal'";
				
				if (!gDb.Update(values, "site", "id < 4"))
				{
					cout << "Update failed." << endl;
				} else
				{
					cout << "Successful." << endl;
				}
				break;
			// delete test
			case 'd':
				cout << "Attempting DELETE..." << endl;
				
				if (!gDb.Delete("site", "id = 6"))
				{
					cout << "Delete failed." << endl;
				} else
				{
					cout << "Successful." << endl;
				}
				break;
			// extended UI params test
			case 'p':
				cout << "site.status...";
				switch (gDb.GetFieldUiType("site", "status"))
				{
				case cDatabase::utNormal:
					cout << "Normal." << endl;
					break;
				case cDatabase::utDropdownFixed:
					cout << "Drop-down box, fixed." << endl;
					break;
				case cDatabase::utDropdownTable:
					cout << "Drop-down box, from table." << endl;
					break;
				}
				if (!gDb.GetFieldUiParams("site", "status", params))
					cout << "Cannot find \"site\" table with \"status\" field." << endl;
				else
				{
					for (StringIntArrayIterator it=params.begin();it != params.end();it++)
						cout << "Parameter " << it->first << ": \"" << it->second << "\"" << endl;
				}
				
				cout << "sitedescription.siteid...";
				switch (gDb.GetFieldUiType("sitedescription", "siteid"))
				{
				case cDatabase::utNormal:
					cout << "Normal." << endl;
					break;
				case cDatabase::utDropdownFixed:
					cout << "Drop-down box, fixed." << endl;
					break;
				case cDatabase::utDropdownTable:
					cout << "Drop-down box, from table." << endl;
					break;
				}
				if (!gDb.GetFieldUiParams("sitedescription", "siteid", params))
					cout << "Error finding \"sitedescription\" table, field \"siteid\"." << endl;
				else
				{
					for (StringIntArrayIterator it=params.begin();it != params.end();it++)
						cout << "Parameter " << it->first << ": \"" << it->second << "\"" << endl;
				}
				break;
			case 'm':
				// get the second argument
				if (argc < 3)
				{
					cout << "Usage:" << endl;
					cout << argv[0] << " m <fileName>" << endl << endl;
				} else
				{
					cout << "Computing MD5 hash..." << endl;
//					cout << ComputeMD5(argv[2]) << endl;
				}
				break;
			case 'e':
				// export the contents of the site table to a CSV file
				if (!gDb.ExportCmdToCsv("select id,lastmodified,sitename,status,AsText(location) as location from site;", "site.csv"))
				{
					cout << "Unable to export to CSV file." << endl;
				}
				break;
			case 'a':
				// do a fuzzy string set match
				if (argc < 3)
				{
					cout << "Usage: " << argv[0] << " a 'test string to match'" << endl;
					break;
				}
				// get the UI params for sitedescription.structuretype
				gDb.GetFieldUiParams("sitedescription", "structuretype", vals);
				i = FuzzyStringSetMatch(argv[2], vals);
				cout << "For string \"" << argv[2] << "\", closest matching string is:" << endl;
				if (i == -1)
					cout << "[no matching string]" << endl;
				else
					cout << vals[i] << endl;
				break;
			case 'v':
				// do a fuzzy value extraction
				if (argc < 3)
				{
					cout << "Usage: " << argv[0] << " v '12,4405,3 M'" << endl;
					break;
				}
				cout << "Output: " << FuzzyValueExtract(argv[2], "M", false) << endl;
				break;
			case 'x':
				// do a fuzzy decimal degrees extraction
				if (argc < 3)
				{
					cout << "Usage: " << argv[0] << " x '29 36 25'" << endl;
					break;
				}
				try
				{
					cout << "Output: " << ExtractDecimalDegrees(argv[2], dfDegMinSec, false) << endl;
				} catch (cAlert& a)
				{
					cout << "Exception caught: " << a.GetMsg() << endl;
				}
				break;
			case 'o':
				// SAPS CSV file import test
				if (argc < 4)
				{
					cout << "Usage: " << argv[0] << " o [tablename] [filename.csv]" << endl;
					break;
				}
				cout << "Importing..." << endl;
				//gDb.ImportFromCsvSaps(argv[2], false, failed);
				gDb.ImportFromCsv(argv[3], argv[2], failed);
				cout << "Complete." << endl;
				break;
			case 'l':
				if (argc < 3)
				{
					cout << "Usage: " << argv[0] << " l 'POINT(-25.3459 22.4504)'" << endl;
					break;
				}
				if (!ExtractLatLongFromPoint(argv[2], dfDegMinSec, la, lo))
					cout << "Parsing error." << endl;
				else
					cout << "Latitude: " << la << ", Longitude: " << lo << endl;
				break;
			default:
				cout << endl << "Unknown argument: " << argv[1] << endl;
				break;
			}
		}
	
	} else
	{
		cout << "Usage: " << argv[0] << " [c|i|s|u|d|w|n|p|m|e|a|v|x]" << endl;
		cout << endl;
		cout << "Tests:" << endl;
		cout << "------" << endl;
		cout << "  c  - Create database from settings.xml." << endl;
		cout << "  i  - Insert test. Inserts bogus data into the [site] table." << endl;
		cout << "  s  - Select test. Selects all items from the [site] table." << endl;
		cout << "  u  - Update test. Updates the [site] table, updating all items with id between 3 and 8." << endl;
		cout << "  d  - Delete test. Deletes the entry with primary key 6 from [site]." << endl;
		cout << "  p  - User-interface extended parameters' parsing test." << endl;
		cout << "  m  - MD5 hash demonstration." << endl;
		cout << "  e  - Test CSV exporting." << endl;
		cout << "  a  - Does a fuzzy string match against the sitedescription.structuretype values." << endl;
		cout << "  v  - Does a fuzzy value extraction." << endl;
		cout << "  o  - Test CSV importing." << endl;
		cout << "  x  - Does a fuzzy decimal degrees extraction." << endl << endl;
	}
	
	return 0;
}
