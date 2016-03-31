 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : exportf.cpp
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

using namespace std;


#include "exportf.h"

ExportF::ExportF()
{
	conn = NULL;
}
ExportF::~ExportF()
{
	// free the connection if allocatedf
	if (conn) {
		delete conn;
	}
}

int ExportF::connectDB(const char *hname, const char *dbname, const char *uname, const char *password)
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


int ExportF::exportFile(const char *fname, const char *machineid)
{
	return exportFile(fname, machineid,NULL);
}


int ExportF::exportFile(const char *fname, const char *machineid,const char *startDate)
{
	char query[1024];

	printf("\n\nQWERYexportFile(const char *fname, const char *machineid) --------------------------------\n");

	// drop the temporary tables
	dropTempTables();
	execSQLCommand("DROP TABLE IF EXISTS exporttest");

	// create a temporary table for site, sitecontents, and sitedescription
	createTempTable("site");
	createTempTable("sitecontacts");
	createTempTable("sitedescription");
	createTempTable("radioinstallation");
	createTempTable("cell");
	createTempTable("antennapattern");
	createTempTable("technology");
	createTempTable("frequencyallocationlist");

	// create a filter table
	execSQLCommand("DROP TABLE IF EXISTS filtersite");
	if (machineid!=NULL) {
		// create the filter table
		sprintf(query,"CREATE TABLE filtersite AS SELECT * FROM tempsite WHERE sitemachineid = '%s'",machineid);
		execSQLCommand(query);
		printf("query [%s]\n",query);
		printf("==machine id %s\n",machineid);
 	} else {
		printf("creating the filter table <<>><><><\n");
		// create the filter table
		sprintf(query,"CREATE TABLE filtersite AS SELECT * FROM tempsite");
		execSQLCommand(query);
	}

	// filter table by date
	execSQLCommand("DROP TABLE IF EXISTS filtersite2;");
	if (startDate!=NULL) {
		sprintf(query,"CREATE TABLE filtersite2 AS SELECT * FROM filtersite WHERE sitelastmodified > '%s';",startDate);
		execSQLCommand(query);
	} else {
		execSQLCommand("CREATE TABLE filtersite2 AS SELECT * FROM filtersite;");
	}

	// merge the tables
	mergeTables();
#if 0
	sprintf(query,"COPY exporttest TO '%s'",fname);
	execSQLCommand(query);
	printf("[%s]\n",query);
#endif

	// drop the unused columns from exporttest
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN sitecontactsid;");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN siteid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN sitedescriptionid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN sitedescriptionsiteid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN radioinstallationid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN radioinstallationsiteid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN cellid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN sitecontactssiteid");
	execSQLCommand("ALTER TABLE exporttest DROP COLUMN frequencyallocationlistid");

	// drop the exporttable
	sprintf(query,"DROP TABLE exporttext;");
	execSQLCommand(query);

	// create a new table for text
	sprintf(query,"CREATE TABLE exporttext AS SELECT * FROM exporttest;");
	execSQLCommand(query);

	// create a new geometry column
	sprintf(query,"ALTER TABLE exporttext ADD locationtext varchar(100);");
	execSQLCommand(query);

	// get the text from the location
	sprintf(query,"UPDATE exporttext SET locationtext = AsText(sitelocation);");
	execSQLCommand(query);

	// drop the original location
	sprintf(query,"ALTER TABLE exporttext DROP COLUMN sitelocation");
	execSQLCommand(query);

	// save the table 	
	sprintf(query,"COPY exporttext TO '%s'",fname);
	execSQLCommand(query);

	// create an import table from exporttext table
	execSQLCommand("DROP TABLE importtexttable");
	execSQLCommand("CREATE TABLE importtexttable AS SELECT * FROM exporttext");
	execSQLCommand("TRUNCATE TABLE importtexttable");

	// create an import test table
	execSQLCommand("DROP TABLE IF EXISTS importtable;");
	execSQLCommand("CREATE TABLE importtable AS SELECT * FROM exporttest;");
	execSQLCommand("TRUNCATE TABLE importtable;");

	execSQLCommand("DROP TABLE IF EXISTS exporttest");

	// drop all the temporary tables
	dropTempTables();

	return 1;
}

void ExportF::createTempTable(const char *tname)
{
	char query[4096];
	int i,j;
	string collist;
	pqxx::work *wv;
	int N;

	printf("ExportF::createTempTable(%s)\n",tname);

	// get the column names of the table
	sprintf(query,"SELECT * FROM %s",tname);

	wv = new pqxx::work(*conn);
	pqxx::result R = wv->exec(query);
	delete wv;

	// build the select list
	N = R.columns();
	for (int j=0;j<N;j++) {
		if (j>0) {
			collist = collist + ", ";
		}
		collist = collist + string(tname) + string(".") + R.column_name(j) + string(" AS ") + string(tname) +R.column_name(j);
	}

	// create the query
	sprintf(query,"CREATE TABLE temp%s AS SELECT %s FROM %s",tname,collist.c_str(),tname);
//	printf("query [%s]\n",query);

	execSQLCommand(query);
}

void ExportF::mergeTables()
{
	printf("ExportF::mergeTables()\n");
	try {
		// merge the site contacts table with the site table
		execSQLCommand("CREATE TABLE tablea AS SELECT * FROM filtersite LEFT OUTER JOIN tempsitecontacts ON siteid = sitecontactssiteid");
		// merge the site description table
		execSQLCommand("CREATE TABLE tableb AS SELECT * FROM tablea LEFT OUTER JOIN tempsitedescription ON siteid = sitedescriptionsiteid");
		// merge the radio installation table
		execSQLCommand("CREATE TABLE tablec AS SELECT * FROM tableb FULL OUTER JOIN tempradioinstallation ON radioinstallationsiteid = siteid");
		// merge the cell table
		execSQLCommand("CREATE TABLE tabled AS SELECT * FROM tablec LEFT OUTER JOIN tempcell ON cellrisector = radioinstallationid");

		// create a temporary table that contains the index and the filename
		execSQLCommand("DROP TABLE IF EXISTS tablee");
		execSQLCommand("CREATE TABLE tablee AS SELECT antennapatternid,antennapatternpatternfile FROM tempantennapattern");

		// merge the antenna patterns with the pattern file
/*		execSQLCommand("CREATE TABLE tablef AS SELECT * FROM tabled LEFT OUTER JOIN tablee ON antennapatternid = radioinstallationtxantpatternkey");*/


printf("abcdefg\n");
		execSQLCommandF("DROP TABLE IF EXISTS tablef");
		execSQLCommandF("DROP TABLE IF EXISTS tableg");
		execSQLCommandF("DROP TABLE IF EXISTS tableh");


		// merge the antenna pattern files
		execSQLCommand("CREATE TABLE tablef AS SELECT * FROM tabled LEFT OUTER JOIN tablee ON antennapatternid = radioinstallationtxantpatternkey");

		// drop the antenna id
		execSQLCommand("ALTER TABLE tablef RENAME antennapatternpatternfile TO txpatternfile");

		// merge the antenna pattern files
		execSQLCommand("ALTER TABLE tablef DROP COLUMN antennapatternid");
		execSQLCommand("CREATE TABLE tableg AS SELECT * FROM tablef LEFT OUTER JOIN tablee ON antennapatternid = radioinstallationrxantpatternkey");
		execSQLCommand("ALTER TABLE tableg DROP COLUMN antennapatternid");

		// drop the antenna id
		execSQLCommand("ALTER TABLE tableg RENAME antennapatternpatternfile TO rxpatternfile");


/*		execSQLCommand("ALTER TABLE tablef ADD COLUMN txpatternfile VARCHAR (100);");
		execSQLCommand("ALTER TABLE tablef ADD COLUMN rxpatternfile VARCHAR (100);");
		*/



		// select the relevant patterns in the technology table
		execSQLCommand("CREATE TABLE temptechtable AS SELECT technologyid,technologytechnologytype FROM temptechnology");

		// join with the main table
		execSQLCommand("CREATE TABLE tableh AS SELECT * FROM tableg LEFT OUTER JOIN temptechtable ON radioinstallationtechkey = technologyid");

		// drop the irrelevant technology columns
		execSQLCommand("ALTER TABLE tableh DROP COLUMN technologyid");

		// join tableh with the filter table
		execSQLCommand("DROP TABLE IF EXISTS tablei");
		execSQLCommand("CREATE TABLE tablei AS SELECT * FROM tableh LEFT OUTER JOIN tempfrequencyallocationlist ON frequencyallocationlistci = cellid;");

		execSQLCommand("DROP TABLE IF EXISTS exporttest");
		execSQLCommand("CREATE TABLE exporttest AS SELECT * FROM tablei");
	}catch(const exception &e) {
		cout << "exception : " << e.what() << endl;
	}
}

int ExportF::writeTable(const char *tname,const char *fname)
{
	int N;
	char query[1024];
	FILE *f;
	pqxx::work *wv;

	wv = new pqxx::work(*conn);

	try {

		// open the file
		f = fopen(fname,"wt");
		if (!f) {
			return 0;
		}

		// construct the query
		sprintf(query,"SELECT * FROM %s",tname);

		// execute the query
		pqxx::result R = wv->exec(query);

		if (R.size()==0) {
			printf("%s does not exist\n",tname);
			delete wv;

			return 0;
		}

		// print the column names
		N = R.columns();
		for (int j=0;j<N;j++) {
			fprintf(f,"%s,",R.column_name(j));
		}
		fprintf(f,"\n");

		// print the sites
		for (pqxx::result::const_iterator r = R.begin();r!=R.end(); ++r)
		{
			// get the number of columns			
			N = r.size();

			// write the data 
			for (int j=0;j<N;j++) {
				fprintf(f,"%s,",r[j].c_str());
			}
			fprintf(f,"\n");
		}

		// close the file
		fclose(f);
	} catch (exception e) {
		cout << "err: " << e.what() << endl;
		cout << tname << endl;
	}

	// delete the work query object
	delete wv;

	return 1;
}

void ExportF::execSQLCommand(const char *query)
{
	execSQLCommandF(query);
#if 0
	work *wv;

	printf("ExportF::execSQLCommand\n");

	// create a new work object
	wv = new work(*conn);

	// execute and commit a command
	wv->exec(query);
	wv->commit();

	// delete the query object
	delete wv;
#endif
}

void ExportF::execSQLCommandF(const char *query)
{
	try {
		pqxx::work *wv;

		// create a new work object
		wv = new pqxx::work(*conn);

		#ifdef EXPORTF_PRINTSQL
			printf("[EXPORTF_SQL] [%s]\n",query);
		#endif


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

void ExportF::dropTempTables()
{
	try {
		// drop the tables
		execSQLCommandF("DROP TABLE IF EXISTS tempsite");
		execSQLCommandF("DROP TABLE IF EXISTS tempsitecontacts");
		execSQLCommandF("DROP TABLE IF EXISTS tempsitedescription");
		execSQLCommandF("DROP TABLE IF EXISTS tempradioinstallation");
		execSQLCommandF("DROP TABLE IF EXISTS tempcell");
		execSQLCommandF("DROP TABLE IF EXISTS tempantennapattern");
		execSQLCommandF("TRUNCATE TABLE importtable");
		execSQLCommandF("DROP TABLE IF EXISTS temptechtable");
		execSQLCommandF("DROP TABLE IF EXISTS temptechnology");
		execSQLCommandF("DROP TABLE IF EXISTS tempfrequencyallocationlist");

		execSQLCommandF("DROP TABLE IF EXISTS tablea");
		execSQLCommandF("DROP TABLE IF EXISTS tableb");
		execSQLCommandF("DROP TABLE IF EXISTS tablec");
		execSQLCommandF("DROP TABLE IF EXISTS tabled");	
		execSQLCommandF("DROP TABLE IF EXISTS tablee");
		execSQLCommandF("DROP TABLE IF EXISTS tablef");
		execSQLCommandF("DROP TABLE IF EXISTS tableg");
	}catch (const exception &ex)
	{
		cout << "failed to drop tables" << endl;
	}
}


void ExportF::dropAllTempTables()
{
		execSQLCommandF("DROP TABLE IF EXISTS tempsite");
		execSQLCommandF("DROP TABLE IF EXISTS tempsitecontacts");
		execSQLCommandF("DROP TABLE IF EXISTS tempsitedescription");
		execSQLCommandF("DROP TABLE IF EXISTS tempradioinstallation");
		execSQLCommandF("DROP TABLE IF EXISTS tempcell");
		execSQLCommandF("DROP TABLE IF EXISTS tempantennapattern");
		execSQLCommandF("TRUNCATE TABLE importtable");
		execSQLCommandF("DROP TABLE IF EXISTS temptechtable");
		execSQLCommandF("DROP TABLE IF EXISTS temptechnology");
		execSQLCommandF("DROP TABLE IF EXISTS tempfrequencyallocationlist");

		execSQLCommandF("DROP TABLE IF EXISTS tablea");
		execSQLCommandF("DROP TABLE IF EXISTS tableb");
		execSQLCommandF("DROP TABLE IF EXISTS tablec");
		execSQLCommandF("DROP TABLE IF EXISTS tabled");	
		execSQLCommandF("DROP TABLE IF EXISTS tablee");
		execSQLCommandF("DROP TABLE IF EXISTS tablef");
		execSQLCommandF("DROP TABLE IF EXISTS tableg");
		execSQLCommandF("DROP TABLE IF EXISTS filtersite");
		execSQLCommandF("DROP TABLE IF EXISTS filtersite2");

}
