 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : restore.cpp
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
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>
#include <pqxx/pqxx>


//using namespace pqxx;
//using namespace std;

#include "restore.h"

#define STRSIZE 4096 // default string size

Restore::Restore():conn(NULL)
{}

Restore::~Restore()
{
	if (conn) 
	{
		delete conn;
	}
}

int Restore::restoreFile(const char *fname)
{
	char cmd[STRSIZE];	
	FILE *f;
	int n;
	char tname[STRSIZE];
	char query[STRSIZE];

	// print the working directory
	char cwd[512];
	getcwd(cwd,512);
	printf("%s\n",cwd);

	// extract the tar file to the cwd
	sprintf(cmd,"tar -xvf %s\n",fname);
	system(cmd);

	// open the file list file 
	f = fopen("backuplist.txt","rt");
	if (f==NULL) 
	{
		printf("failed to open backuplist.txt!\n");
		return 0;
	}

	// truncate all the tables
	while (1) 
	{
		// read the table name
		n = fscanf(f,"%s",tname);
		if (n<=0)
			break;

		// drop the table if it exists
		sprintf(query,"TRUNCATE TABLE %s CASCADE",tname);
		printf("\tSQL query [%s]\n",query);
		execSQLCommandF(query);
	}

	// seek to the start of the file
	fseek(f,0,SEEK_SET);

	// read the tablenames and process
	while (1) 
	{
		// read the table name
		n = fscanf(f,"%s",tname);
		if (n<=0)
			break;

		// restore the table
		restoreTable(tname);
	}

	// close the file
	fclose(f);

	return 1;
}

int Restore::restoreTable(const char *tableName)
{
	char cwd[STRSIZE];
	char query[STRSIZE];
	char cmd[STRSIZE];
	
	// get the cwd
	getcwd(cwd,STRSIZE);


	// copy the table from the existing file to the database
	sprintf(query,"COPY %s FROM '%s/%s.txt'",tableName,cwd,tableName);
	execSQLCommandF(query);

	// remove the restored table
	sprintf(cmd,"%s.txt",tableName);
//	unlink(cmd);
	
	return 1;
}

int Restore::connectDB(const char* hname,const char* dbname,const char *uname,const char *password)
{
	char cstr[512];
//	try 
//	{
		// set up the connection string
		sprintf(cstr,"host=%s dbname=%s user=%s password=%s",hname,dbname,uname,password);
		printf("[%s]\n",cstr);

		// set up the connection
		conn = new pqxx::connection(cstr);
//	}
//	catch (const exception &e) 
//	{
//		cout << e.what() << endl;
//		return 0;
//	}
	return 1;
}

#if 0
int Restore::connectDB(const char* hname,const char* dbname)
{
	char cstr[512];
	try {
		// set up the connection string
		sprintf(cstr,"dbname=%s",dbname);//host=%s dbname=%s password=postqrap",hname,dbname);

		// set up the connection
		conn = new connection(cstr);

	}catch (const exception &e) {
		cout << e.what() << endl;
		return 0;
	}
	return 1;
}
#endif

void Restore::execSQLCommand(const char *query)
{
	pqxx::work *wv;

	// create a new work object
	wv = new pqxx::work(*conn);

	// execute and commit a command
	wv->exec(query);
	wv->commit();

	// delete the query object
	delete wv;
}

void Restore::execSQLCommandF(const char *query)
{
//	try {
		pqxx::work *wv;

		// create a new work object
		wv = new pqxx::work(*conn);

		// execute and commit a command
		wv->exec(query);
		wv->commit();

		// delete the query object
		delete wv;
//	}catch (const exception &ex)
//	{
//		cout << "failed SQL command: " << query << endl << ex.what() << endl;
//	}
}

