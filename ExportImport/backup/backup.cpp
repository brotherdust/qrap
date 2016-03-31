 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : backup.cpp
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
#include <string.h>

#include <iostream>
#include <string>

#include "backup.h"

const char *tableNames[] = {
	"machine",
	"antenna",
	"antennadevice",
	"site",
	"technology",
	"antennapattern",
	"btl",
	"cell",
	"equipment",
	"equipmenttype",
	"filesets",
	"filesetsused",
	"frequencyallocationlist",
	"geometry_columns",
	"globaltype",
	"landcover",
	"links",
	"mobile",
	"sitecontacts",
	"sitedescription",
	"sourcefiles",
	"radioinstallation",
	"importtable",
	"exporttable"};


Backup::Backup() :
	conn(NULL)
{}

Backup::~Backup()
{
	// free the connection if allocated
	if (conn) {
		delete conn;
	}
}

int Backup::backupList(const char *tableListfname, const char *outputfname)
{
	FILE *tableList;
	char fname[4096];
	char query[4096];
	char cmd[4096];
	
	char flist[8192];
	char fn[4096];
	int n;
	char cwd[4096];

	// open the file list
	tableList = fopen(tableListfname, "rt");
	if (tableList == NULL) {
		printf("Backup::backupList: failed to open [%s]\n",tableListfname);
		return 0;
	}

	// set the string to the filelist
	strcpy(flist,"");

	// get the cwd
	getcwd(cwd,4096);
	printf("[%s]\n",cwd);

	// write all the tables
	while (1) {
		// read the filename
		n = fscanf(tableList,"%s",fname);
		if (n<=0) {
			break;
		}

		// create the query to export the table
		sprintf(fn,"%s.txt",fname);
		sprintf(query, "COPY %s TO '%s/%s.txt'",fname,cwd,fname);

		// execute the query
		execSQLCommandF(query);
	
		// add the filename to the string
		strcat(flist," ");
		strcat(flist,fn);
	}

	// close the file list
	fclose(tableList);

	// copy the filelist as a restore table list
	sprintf(cmd,"cp %s backuplist.txt",tableListfname);
	system(cmd);

	// create a tar archive of all the files
	sprintf(cmd,"tar -cvf %s %s %s\n",outputfname,flist,"backuplist.txt");
	system(cmd);

	return 1;
}



void Backup::execSQLCommand(const char *query)
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


void Backup::execSQLCommandF(const char *query)
{
	try {
		pqxx::work *wv;

		// create a new work object
		wv = new pqxx::work(*conn);

		// execute and commit a command
		wv->exec(query);
		wv->commit();

		// delete the query object
		delete wv;
	}catch (const exception &ex)
	{
		cout << "failed SQL command: " << query << endl << ex.what() << endl;
	}
}

int Backup::connectDB(const char* hname,const char* dbname,const char *uname,const char *password)
{
	char cstr[512];
	try {
		// set up the connection string
		sprintf(cstr,"host=%s dbname=%s user=%s password=%s",hname,dbname,uname,password);
		printf("[%s]\n",cstr);

		// set up the connection
		conn = new pqxx::connection(cstr);
	}catch (const exception &e) {
		cout << e.what() << endl;
		return 0;
	}
	return 1;
}


int Backup::backup(const char *outputfname)
{
	FILE *bkfile;
	char fname[4096];
	char query[4096];
	char cmd[4096];
	int N,i;
	
	char flist[8192];
	char fn[4096];
	int n;
	char cwd[4096];

	N = sizeof(tableNames)/sizeof(char*);

	// set the string to the filelist
	strcpy(flist,"");

	// get the cwd
	getcwd(cwd,4096);
	printf("[%s]\n",cwd);

	// write all the tables
	for (i=0;i<N;i++) {
		// read the filename
		strcpy(fname,tableNames[i]);

		// create the query to export the table
		sprintf(fn,"%s.txt",fname);
		sprintf(query, "COPY %s TO '%s/%s.txt'",fname,cwd,fname);

		// execute the query
		execSQLCommandF(query);
	
		// add the filename to the string
		strcat(flist," ");
		strcat(flist,fn);
	}

	// create a filename that lists all the relevant tables
	bkfile = fopen("backuplist.txt","wt");
	for (i=0;i<N;i++) {
		fprintf(bkfile,"%s\n",tableNames[i]);
	}
	fclose(bkfile); 

	// create a tar archive of all the files
	sprintf(cmd,"tar -cvf %s %s %s\n",outputfname,flist,"backuplist.txt");
	system(cmd);

	return 1;
}
