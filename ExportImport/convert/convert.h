 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : convert.h
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

#ifndef CONVERT_H_
#define CONVERT_H_

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

//using namespace pqxx;
using namespace std;

/// Converts to the new format
class Convert
{
public:
	Convert();
	~Convert();

	/// connect to the database
	int connectDB(const char *hname, const char *dbname, const char *uname, const char *password);

	/// Imports the file specified by fname
	/// \param fname The name of the file
	/// \return Returns 1 if successful, otherwise 0
	int convertFile(const char *inputfname,const char *outputfname);

	
protected:
	/// The database connection
	connection *conn;

	/// Executes a SQL command (that does not return an output.
	///  This function may throw SQL exceptions (should be handled in the 
	///    caller function).
	void execSQLCommand(const char *query);

	// create the temporary table with the correct format
	void createTable();

	/// Creates an insert query from the input data and writes it to the output data.
	void createInsertQuery(const char *input,char *query,int n,const char *tablename);

	/// Inserts the data by reading the data and inserting it with explicit queries.
	void insertData(const char *inputfname,const char *tablename);

	/// inserts the data into the main table
	void insertMainTable();
};

#endif // CONVERT_H_
