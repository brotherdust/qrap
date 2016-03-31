 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : restore.h
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


#ifndef RESTORE_H_
#define RESTORE_H_

class Restore
{
public:
	// ctor
	Restore();
	
	// dtor
	~Restore();

	/// Connects to the DB	
	int connectDB(const char* hname,const char* dbname,const char *uname,const char *password);

	// restore the database from the file 
	int restoreFile(const char *fname);
protected:
	// the connection
	pqxx::connection *conn;
	// executes the query. throws an exception if unsuccessful.
	void execSQLCommand(const char *query);

	// executes the query. will print an error message to stdout if unsuccessful.
	void execSQLCommandF(const char *query);

	/// Restores the table with tableName. Assumes that the table is stored in the current
	///  working directory.
	int restoreTable(const char *tableName);
	
};

#endif // RESTORE_H_
