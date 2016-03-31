 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : mergedelete.cpp
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

#include <stdlib.h>
#include <stdio.h>

#include "mergedelete.h"

MergeDelete::MergeDelete(): conn(NULL)
{}

MergeDelete::~MergeDelete()
{
	if (conn!=NULL) {
		delete conn;
	}
}


int MergeDelete::connectDB(const char *hname, const char *dbname, const char *uname, const char *password)
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

void MergeDelete::execSQLCommand(const char *query)
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


int MergeDelete::mergeDelete(const char *fname)
{
	char query[2048];

	// create a mergedelete table with the same structure as the importtable
	execSQLCommand("DROP TABLE mergedeletetxt");
	execSQLCommand("CREATE TABLE mergedeletetxt AS SELECT * FROM importtexttable");
	execSQLCommand("CREATE TABLE mergedelete AS SELECT * FROM importtable");
	execSQLCommand("TRUNCATE TABLE mergedelete");

	// load the table from the file
	sprintf(query,"COPY mergedeletetxt FROM '%s'",fname);
	printf("MergeDelete::mergeDelete query [%s]\n",query);
	printf("[%s]\n",fname);
	execSQLCommand(query);

	// copy the right fields into the merge delete table
	execSQLCommand("INSERT INTO mergedelete (sitelastmodified,sitemachineid,sitesitename,sitestatus,sitegroundheight,sitelocation,sitecontactslastmodified,sitecontactsmachineid,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionmachineid,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationmachineid,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmachineid,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,antennapatternpatternfile,technologytechnologytype,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel) SELECT sitelastmodified,sitemachineid,sitesitename,sitestatus,sitegroundheight,GeomFromText(locationtext,4326),sitecontactslastmodified,sitecontactsmachineid,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionmachineid,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationmachineid,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmachineid,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,antennapatternpatternfile,technologytechnologytype,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel FROM mergedeletetxt;");

	// delete all the sites that is not on the server
	execSQLCommand("DELETE FROM site WHERE sitename NOT IN (SELECT sitesitename FROM mergedelete)");
 
	return 1;
}

