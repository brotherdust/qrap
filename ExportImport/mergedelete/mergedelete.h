 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : mergedelete.h
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

#ifndef MERGEDELETE_H_
#define MERGEDELETE_H_

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

//using namespace pqxx;
using namespace std;


/// Removes all the entries from the database which is not in the import file.
class MergeDelete
{
public:
	MergeDelete();
	~MergeDelete();

	/// connect to the database
	int connectDB(const char *hname, const char *dbname, const char *uname, const char *password);

	/// Remove all the entries which is not in the import file but which is in the database (i.e. deleted entries).
	/// \param fname The name of the file
	/// \return Returns 1 if successful, otherwise 0
	int mergeDelete(const char *fname);

private:
	pqxx::connection *conn;

	/// Executes a SQL command (that does not return an output.
	///  This function may throw SQL exceptions (should be handled in the 
	///    caller function).
	void execSQLCommand(const char *query);
};

#endif // MERGEDELETE_H_
