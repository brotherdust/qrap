 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : importf.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Christo (magdaleen.ballot@up.ac.za) 
 *    Description : Not really ready to be used
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


#ifndef FLATIMPORT_H_
#define FLATIMPORT_H_

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

// if defined, the SQL statements executed will be printed to STD out
//#define IMPORTF_PRINTSQL 

//using namespace pqxx;
using namespace std;

class ImportF
{
public:
	ImportF();
	~ImportF();

	/// connect to the database
	int connectDB(const char *hname, const char *dbname, const char *uname, const char *password);

	/// Imports the file specified by fname
	/// \param fname The name of the file
	/// \return Returns 1 if successful, otherwise 0
	int importFile(const char *fname);

private:
	/// Returns the maximum id in the sequence
	int getMaxId(const char *tableName,const char *columnName);

	/// Set the sequence to point to the maximum value
	void fixSequence(const char *tableName,const char *columnName);

	/// The database connection
	pqxx::connection *conn;

	/// Executes a SQL command (that does not return an output.
	///  This function may throw SQL exceptions (should be handled in the 
	///    caller function).
	void execSQLCommand(const char *query);


	/// performs queries to update the site table
	void updateSiteTable();

	// updates the site contacts
	void updateSiteContacts();

	// updates the site descriptions
	void updateSiteDescription();

	// update all the radio installations
	void updateRadioInstallations();

	// update the cells
	void updateCells();

	void updateFrequencyAllocation();


	/// Drop all the temporary tables
	void dropTmpTables();

	/// Drop a table
	void dropTable(const char *tname);
};

#endif // FLATIMPORT_H_

