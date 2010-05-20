 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : convert.cpp
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

#include "convert.h"

#include <string.h>
#include <string>

#include "tokenizer.h"

using std::string;

#define MAXLINE 16384

const char *tmptablename = "importpolice";

Convert::Convert():conn(NULL)
{
}

Convert::~Convert()
{
	if (conn!=NULL) {
		delete conn;
	}
}


void Convert::execSQLCommand(const char *query)
{
	work *wv;

	try {

		// create a new work object
		wv = new work(*conn);

		#ifdef IMPORTF_PRINTSQL
			printf("[IMPORTF_SQL] [%s]\n",query);
		#endif

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

int Convert::connectDB(const char *hname, const char *dbname, const char *uname, const char *password)
{
	char cstr[512];

	try {
		// create the connection string
		sprintf(cstr,"user=%s password=%s dbname=%s host=%s",uname,password,dbname,hname);
		
		// set up the connection
		conn = new connection(cstr);

	} catch(const exception &e) {
		cout << e.what() << endl;
		return 0;
	}

	return 1;
}

void Convert::createTable()
{
	// drop the table if it exists
	execSQLCommand("DROP TABLE IF EXISTS importpolice;");

	// create the table
	execSQLCommand("CREATE TABLE importpolice AS SELECT sitelastmodified,sitesitename,sitestatus,sitegroundheight,sitecontactslastmodified,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,txpatternfile,rxpatternfile,technologytechnologytype,locationtext FROM importtexttable;");
}

void Convert::createInsertQuery(const char *input,char *query,int n,const char *tablename)
{
	int len,i;
	char buf[1024];
	char *c;
	string qr,tk;
	int fs=0;
	string ts;

	Tokenizer tok;

	// setup the tokenizer
	tok.setString(input,"\t");

	// create the query
	qr = "INSERT INTO " + string(tablename) + " VALUES (";

	// start the tokenization
	i = 0;
	while (1) {
		// get the token
		c = tok.getToken(buf,1024);

		// check if last token
		if (c==NULL) {
			break;
		}

		// set the token
		tk = string(buf);


		if (fs) {
			qr = qr + string(",");
		}

		if (tk=="\\N" || tk=="") {
			ts = string("NULL");		
		} else {
			ts = "'" + tk + "'";
		}
		qr = qr + ts;

		// print the token
		fs = 1;

		i++;
	}

	qr = qr + ");";

	// copy the string
	strncpy(query,qr.c_str(),n);

}

int Convert::convertFile(const char *inputfname,const char *outputfname)
{
	char query[1024];

	// drop the table if it exists
	sprintf(query,"DROP TABLE IF EXISTS %s",tmptablename);
	execSQLCommand(query);

	// create a new table
	createTable();

	// insert the data
	insertData(inputfname,tmptablename);

	// truncate the importtexttable
	execSQLCommand("TRUNCATE TABLE importtexttable");

	// copy the current values into the import text table
	insertMainTable();

	// copy the importtexttable to the output 
	sprintf(query,"COPY importtexttable TO '%s';",outputfname);
	execSQLCommand(query);

	// drop the table again
	//sprintf(query,"DROP TABLE IF EXISTS %s",tmptablename);
//	execSQLCommand(query);
}

void Convert::insertMainTable()
{
	// truncate the importtext table
	execSQLCommand("TRUNCATE TABLE importtexttable;");

	// insert into the main table
	execSQLCommand("INSERT INTO importtexttable (sitelastmodified,sitesitename,sitestatus,sitegroundheight,sitecontactslastmodified,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,txpatternfile,rxpatternfile,technologytechnologytype,locationtext) SELECT * FROM importpolice");

	// set the machine id
	execSQLCommand("UPDATE importtexttable SET sitemachineid=1,sitedescriptionmachineid=1,radioinstallationmachineid=1,cellmachineid=1;");
}

void Convert::insertData(const char *inputfname,const char *tablename)
{
	FILE *inputf;
	char line[MAXLINE];
	char *c;
	char query[MAXLINE];

	// open the input file
	inputf = fopen(inputfname,"rt");

	// read the lines
	while (1) {
		// read the line 
		c = fgets(line,MAXLINE,inputf);

		// check for eof
		if (c==NULL)
			break;

		// remove the newline
		while (*c!='\0') {
			if (*c=='\n') {
				*c = '\0';
			}
			c++;
		}

		// create the insert query
		createInsertQuery(line,query,MAXLINE,tablename);
		
		// execute the query
		execSQLCommand(query);
	}

	// close the input file
	fclose(inputf);
}

