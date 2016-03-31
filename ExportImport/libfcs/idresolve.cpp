 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : idresolve.cpp
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

#include <stdio.h>
#include <stdlib.h>

#include "idresolve.h"

IdResolve::IdResolve()
{
	conn = NULL;
}
IdResolve::~IdResolve()
{
	if (conn!=NULL) {
		delete conn;
	}
}

int IdResolve::connectDB(const char *hname, const char *dbname, const char *uname, const char *password)
{
	char cstr[512];

	try {
		// create the connection string
		sprintf(cstr,"user=%s password=%s dbname=%s host=%s",uname,password,dbname,hname);
		
		// set up the connection
		conn = new pqxx::connection(cstr);

	} catch(const exception &e) {
		cout << e.what() << endl;
		return 0;
	}

	return 1;
}



void IdResolve::execSQLCommand(const char *query)
{
	pqxx::work *wv;

	try {
		// create a new work object
		wv = new pqxx::work(*conn);

		// execute and commit a command
		wv->exec(query);
		wv->commit();

		// delete the query object
		delete wv;
	}catch (const exception &ex) {
		cout << "SQL ERROR ImportF::execSQLCommand: " << endl;
		cout << ex.what() << endl;
		cout << "original query: [" << query << "]" << endl;
	}
}


int IdResolve::resolveId(unsigned int hostid,const char *desc)
{
	char query[2048];
	int idx;
	string a;

	// create a new working variable
	pqxx::work *wv = new pqxx::work(*conn);

	// create the query 
	sprintf(query,"SELECT id FROM machine WHERE mainuserid = '%d'",hostid);

	// perform a query
	pqxx::result R = wv->exec(query);

	printf("result site %d\n",(int)R.size());
	if (R.size()==1) {
printf("b\n");
		pqxx::result::const_iterator r = R.begin();
		a = string(r[0].c_str());
		printf("b - string return %s\n",a.c_str());
	
		idx = atoi(a.c_str());//atoi(r[0].c_str());

	} else {
printf("c\n");
		// kill the previous transaction
		wv->commit();
		delete wv;

		// not in database, insert
		sprintf(query,"INSERT INTO machine (lastmodified,qrapinst,mainuserid) VALUES (now(),'%s','%d')",desc,hostid);
		execSQLCommand(query);

		// get a new working structure 
		wv = new pqxx::work(*conn);

		// set up the query
		sprintf(query,"SELECT id FROM machine WHERE mainuserid = '%d'",hostid);
	
		// perform a query
		pqxx::result R = wv->exec(query);
	
		if (R.size()==1) {
printf("d\n");
			pqxx::result::const_iterator r = R.begin();
			a = string(r[0].c_str());
			printf("d result %s\n",a.c_str());
			idx = atoi(a.c_str());//atoi(r[0].c_str());
	
		} else {
			idx = -1;
		}
	}

	// delete the working variable
printf("e\n");
	delete wv;

	printf("idx %d\n",idx);
	return idx;
}


int IdResolve::getHostId()
{
	char query[2048];
	int idx;
	string a;
	unsigned int hostid;

	hostid = gethostid();
	
	// set an initial id 
	idx = -1;

	// create a new working variable
	pqxx::work *wv = new pqxx::work(*conn);

	// create the query 
	sprintf(query,"SELECT id FROM machine WHERE mainuserid = '%d'",hostid);

	// perform a query
	pqxx::result R = wv->exec(query);

	if (R.size()==1) {
		pqxx::result::const_iterator r = R.begin();
		a = string(r[0].c_str());
	
		idx = atoi(a.c_str());
	}

	// delete the working variable
	delete wv;

	return idx;
}

void IdResolve::setHostId(unsigned int index,unsigned int hostid)
{
	char query[2048];
	int idx;
	string a;

	// not in database, insert
	sprintf(query,"INSERT INTO machine (id,lastmodified,qrapinst,mainuserid) VALUES (%d,now(),'%s','%d')",index,"localid",hostid);
	execSQLCommand(query);
}
