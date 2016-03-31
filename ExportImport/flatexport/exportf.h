 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : exportf.h
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
#ifndef EXPORTF_H_
#define EXPORTF_H_

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

#define EXPORTF_PRINTSQL // if defined, the SQL statements executed will be printed to STD out


//using pqxx::connection;

class ExportF
{
public:
	ExportF();
	~ExportF();

	/// Connects to the database
	/// \param dbname The name of the database
	/// \return returns 1 if successful.
	int connectDB(const char *hname, const char *dbname, const char *uname, const char *password);

	/// Starts the export process.Filters by a machine id. If no filtering should be used.
	///  Specify this parameter as zero.
	int exportFile(const char *fname, const char *machineid);

	/// Export a file by date.
	int exportFile(const char *fname, const char *machineid,const char *startDate);


	/// Creates temporary tables with the right parameters
	void createTempTable(const char *tname);

	// merge the tables
	void mergeTables();


protected:
	pqxx::connection *conn;

	/// Writes a table to a comma separated text file
	/// \param tname The name of the table to export
	/// \param fname The name of the file to be created
	/// \return Returns 1 if successful, otherwise 0
	int writeTable(const char *tname,const char *fname);

	/// Drops all the temporary tables
	void dropTempTables();

	/// Executes a SQL command (that does not return an output.
	///  This function may throw SQL exceptions (should be handled in the 
	///    caller function).
	void execSQLCommand(const char *query);

	/// Executes a SQL command that may fail. Will not throw exception.
	void execSQLCommandF(const char *query);

	void dropAllTempTables();
};

#endif // EXPORTF_H_
