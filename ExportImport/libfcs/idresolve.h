 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : idresolve.h
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

#ifndef IDRESOLVE_H_
#define IDRESOLOVE_H_

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <pqxx/pqxx>


//using namespace pqxx;
using namespace std;

/// Resolves the ID of the system.
/// This class will check if a specific ID is in the database. There is also functionality to add a new ID - so this class will be used by both the client and the server.
class IdResolve
{
public:
	IdResolve();
	~IdResolve();

	/// connect to the database
	int connectDB(const char *hname, const char *dbname, const char *uname, const char *password);

	// Returns and index for the host id. If the hostid is not in the table, it 
	//  will insert it and return the id
	int resolveId(unsigned int hostid,const char *desc);

	// Returns the index of the host id. If the host id is not in the table, a negative value will be returned.  
	int getHostId();

	/// Sets the index and host id. This will be used on the client where the host id and index is received from
	///  the server.
	void setHostId(unsigned int index,unsigned int hostid);
protected:
	pqxx::connection *conn;

	/// Executes a SQL command (that does not return an output.
	///  This function may throw SQL exceptions (should be handled in the 
	///    caller function).
	void execSQLCommand(const char *query);
};


#endif 
