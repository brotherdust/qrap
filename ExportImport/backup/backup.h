 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : backup.h
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

#ifndef BACKUP_H_
#define BACKUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>

//using namespace pqxx;
using namespace std;

/// Backs up a list of tables to a single tar archive.
///
class Backup
{
public:

	// ctor
	Backup();
	// dtor
	virtual ~Backup();

	/// Connects to the DB	
	int connectDB(const char* hname,const char* dbname,const char *uname,const char *password);

	/// Clones all the tables in the text file specified by tablelist. The result is written
	///  to the outputfile indicated by outputfilename
	int backupList(const char *tableList, const char *outputfname);

	// Backup all the relevant tables (using an internal list)
	// 
	int backup(const char *outputfname);

protected:
	// the connection
	pqxx::connection *conn;
	// executes the query. throws an exception if unsuccessful.
	void execSQLCommand(const char *query);

	// executes the query. will print an error message to stdout if unsuccessful.
	void execSQLCommandF(const char *query);
};

#endif // BACKUP_H_

