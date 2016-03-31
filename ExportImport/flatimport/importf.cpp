 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : importf.cpp
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

#include "importf.h"

ImportF::ImportF():conn(NULL)
{}

ImportF::~ImportF()
{
	if (conn!=NULL) {
		delete conn;
	}
}

int ImportF::connectDB(const char *hname, const char *dbname, const char *uname, const char *password)
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

int ImportF::importFile(const char *fname)
{
	char query[1024];

	printf("\n\n\nImportF::importFile--------------------------------------------------------\n");

	// copy the import file to a table
	execSQLCommand("TRUNCATE TABLE importtexttable;");
	sprintf(query,"COPY importtexttable FROM '%s'",fname);
	execSQLCommand(query);

	// copy to the importtable
	execSQLCommand("TRUNCATE TABLE importtable;");
	execSQLCommand("INSERT INTO importtable (sitelastmodified,sitemachineid,sitesitename,sitestatus,sitegroundheight,sitelocation,sitecontactslastmodified,sitecontactsmachineid,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionmachineid,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationmachineid,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmachineid,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,txpatternfile,rxpatternfile,technologytechnologytype,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel) SELECT sitelastmodified,sitemachineid,sitesitename,sitestatus,sitegroundheight,GeomFromText(locationtext,4326),sitecontactslastmodified,sitecontactsmachineid,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel,sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontactsmonthlyrental,sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno,sitedescriptionlastmodified,sitedescriptionmachineid,sitedescriptionphysicaladdress,sitedescriptionstructuretype,sitedescriptionstructureheight,sitedescriptionstructurecapacity,sitedescriptionavailablecapacity,sitedescriptionpowersupply,sitedescriptionfencing,sitedescriptionaccess,sitedescriptioncontainertype,sitedescriptionsitedocumentation,radioinstallationlastmodified,radioinstallationmachineid,radioinstallationsector,radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey,radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt,radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,celllastmodified,cellmachineid,cellmastercell,cellrisector,cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,txpatternfile,rxpatternfile,technologytechnologytype,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel FROM importtexttable;");


/*

frequencyallocationlistlastmodified, frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel,locationtext
*/


#if 0
	execSQLCommand("UPDATE importtable SET id = importtexttable.id");

	// drop the import table
	execSQLCommand("DROP TABLE importtmptable");
	
	// copy the table to the import table
	execSQLCommand("CREATE TABLE importtable AS SELECT * FROM importtexttable");

	// add an extra postgis type column
	execSQLCommand("ALTER TABLE importtable ADD sitelocation geometry");

	// convert the text column as a binary column
	execSQLCommand("UPDATE importtable SET sitelocation = GeomFromText(locationtext);");

	// drop the location text column
	execSQLCommand("ALTER TABLE importtable DROP COLUMN locationtext;");

	// load the table from file
	sprintf(query,"COPY importtable FROM '%s'",fname);
	execSQLCommand(query);
	printf("ImportF::importFile [%s]\n",query);
#endif
	// drop the temporary table
	dropTmpTables();

	// update the site table
	printf("\n\n\n\nupdateSiteTable();\n");
	updateSiteTable();

	// update all the ratio installations
	updateRadioInstallations();

	// update all the cells
	updateCells();

	// update the frequency allocation
	updateFrequencyAllocation();

	// drop all the temporary tables
//	dropTmpTables();

	return 1;
}

void ImportF::execSQLCommand(const char *query)
{
	pqxx::work *wv;

	try {

		// create a new work object
		wv = new pqxx::work(*conn);

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

void ImportF::updateSiteTable()
{
	printf("ImportF::updateSiteTable()\n");

	// set the new sequence values
	fixSequence("site","id");


	// add a new id column to the table
//	execSQLCommand("ALTER TABLE importtable ADD COLUMN siterandid INTEGER;");

	// insert into the table (changed)
	execSQLCommand("INSERT INTO site(lastmodified,machineid,sitename,status,groundheight,location) SELECT DISTINCT sitelastmodified,sitemachineid,sitesitename,sitestatus,sitegroundheight,sitelocation FROM importtable WHERE sitesitename NOT IN (SELECT sitename FROM site);");

	// update the site contacts
	updateSiteContacts();

	// update the site descriptions
	updateSiteDescription();
}

void ImportF::updateSiteContacts()
{
	printf("ImportF::updateSiteContacts()\n");
/* update existing */
	// create a temporary table with newer entries
	dropTable("updatecontacts");
	execSQLCommand("CREATE TABLE updatecontacts AS SELECT DISTINCT sitecontacts.id, sitecontactslastmodified,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel, sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate,sitecontacts.monthlyrental, sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno FROM site,importtable,sitecontacts WHERE (site.sitename = sitesitename AND sitecontacts.siteid = site.id AND importtable.sitesitename = site.sitename AND sitecontactslastmodified>sitecontacts.lastmodified);");

	// update based on the new entries
	execSQLCommand("UPDATE sitecontacts SET lastmodified=sitecontactslastmodified,ownername=sitecontactsownername,naturalperson=sitecontactsnaturalperson, contactname=sitecontactscontactname, postaladdress=sitecontactspostaladdress, tel=sitecontactstel, idnumber=sitecontactsidnumber, contract=sitecontactscontract, contractstartdate=sitecontactscontractstartdate, contractenddate=sitecontactscontractenddate, electricitysupplier=sitecontactselectricitysupplier,  electricityaccno=sitecontactselectricityaccno, electricitypoleno=sitecontactselectricitypoleno FROM updatecontacts WHERE sitecontacts.id = updatecontacts.id;");

/* insert new */	
	// create a table with all the sitecontacts that is not yet in the dbase
	dropTable("tmpallcontacts");
	execSQLCommand("CREATE TABLE tmpallcontacts AS SELECT DISTINCT sitesitename, sitecontactslastmodified,sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress,sitecontactstel, sitecontactsidnumber,sitecontactscontract,sitecontactscontractstartdate,sitecontactscontractenddate, sitecontactselectricitysupplier,sitecontactselectricityaccno,sitecontactselectricitypoleno FROM importtable WHERE (sitesitename NOT IN (SELECT site.sitename FROM site,sitecontacts WHERE (site.id = sitecontacts.siteid)));");

	// insert the new data into the database
	execSQLCommand("INSERT INTO sitecontacts (lastmodified,siteid,ownername,naturalperson,contactname,postaladdress,tel,idnumber,contract) SELECT DISTINCT sitecontactslastmodified, id, sitecontactsownername,sitecontactsnaturalperson,sitecontactscontactname,sitecontactspostaladdress, sitecontactstel,sitecontactsidnumber,sitecontactscontract FROM tmpallcontacts,site WHERE site.sitename = sitesitename;");

}

void ImportF::updateSiteDescription()
{
	printf("ImportF::updateSiteDescription()\n");
/* update existing */
	// create a temporary table with newer entries
	dropTable("updatesitedescription");

	execSQLCommand("CREATE TABLE updatesitedescription AS SELECT DISTINCT sitedescription.id, sitedescriptionlastmodified, sitedescriptionphysicaladdress, sitedescriptionstructuretype,sitedescriptionstructureheight, sitedescriptionstructurecapacity, sitedescriptionavailablecapacity, sitedescriptionpowersupply, sitedescriptionfencing, sitedescriptionaccess, sitedescriptioncontainertype, sitedescriptionsitedocumentation FROM site,importtable,sitedescription WHERE (site.sitename = sitesitename AND sitedescription.siteid = site.id AND sitedescriptionlastmodified>sitedescription.lastmodified)");

	// update based on the newer entries
	execSQLCommand("UPDATE sitedescription SET lastmodified=sitedescriptionlastmodified, physicaladdress = sitedescriptionphysicaladdress, structuretype = sitedescriptionstructuretype, structureheight=sitedescriptionstructureheight, structurecapacity=sitedescriptionstructurecapacity, availablecapacity=sitedescriptionavailablecapacity, powersupply=sitedescriptionpowersupply, fencing=sitedescriptionfencing, access=sitedescriptionaccess, containertype=sitedescriptioncontainertype, sitedocumentation=sitedescriptionsitedocumentation FROM updatesitedescription WHERE sitedescription.id = updatesitedescription.id;");

/* insert new */
	// create a table with all the site descriptions that are not yet in the database
	dropTable("tmpalldescription");
	execSQLCommand("CREATE TABLE tmpalldescription AS SELECT DISTINCT sitesitename, sitedescriptionlastmodified, sitedescriptionphysicaladdress,  sitedescriptionstructuretype, sitedescriptionstructureheight, sitedescriptionstructurecapacity, sitedescriptionavailablecapacity, sitedescriptionpowersupply, sitedescriptionfencing, sitedescriptionaccess, sitedescriptioncontainertype, sitedescriptionsitedocumentation FROM importtable WHERE (sitesitename NOT IN (SELECT site.sitename FROM site,sitedescription WHERE (site.id = sitedescription.siteid)));");

	// insert the data into the database
	execSQLCommand("INSERT INTO sitedescription (lastmodified,siteid,physicaladdress,structuretype, structureheight, structurecapacity, availablecapacity, powersupply, fencing, access, containertype, sitedocumentation) SELECT DISTINCT sitedescriptionlastmodified, id, sitedescriptionphysicaladdress, sitedescriptionstructuretype, sitedescriptionstructureheight, sitedescriptionstructurecapacity, sitedescriptionavailablecapacity, sitedescriptionpowersupply, sitedescriptionfencing, sitedescriptionaccess, sitedescriptioncontainertype, sitedescriptionsitedocumentation FROM tmpalldescription,site WHERE site.sitename = sitesitename;");

}

void ImportF::updateRadioInstallations()
{
	printf("ImportF::updateRadioInstallations()\n");

	// ensure that the sequence is correct
	fixSequence("radioinstallation","id");

/* update old entries */
	// create a table with all the radio installations that are not newer
	dropTable("tmpupdateradioinstallations");
	printf("1\n");
	execSQLCommand("CREATE TABLE tmpupdateradioinstallations AS SELECT DISTINCT ON (radioinstallation.siteid,radioinstallationsector)  radioinstallation.siteid,radioinstallation.lastmodified, radioinstallationlastmodified,radioinstallationsector, radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity, radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey, radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses, radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,technologytechnologytype FROM importtable,site,radioinstallation WHERE (site.sitename = sitesitename AND radioinstallation.siteid=site.id AND radioinstallationlastmodified>radioinstallation.lastmodified AND radioinstallation.sector = radioinstallationsector); ");

	// update all of the radioinstallations based on its sector and the site id
	printf("2\n");
	execSQLCommand("UPDATE radioinstallation SET  lastmodified=radioinstallationlastmodified,sector=radioinstallationsector, permanent=radioinstallationpermanent,techkey=radioinstallationtechkey,eirp=radioinstallationeirp, diversity=radioinstallationdiversity,txpower=radioinstallationtxpower,txlosses=radioinstallationtxlosses, txantennaheight=radioinstallationtxantennaheight,txantpatternkey=radioinstallationtxantpatternkey,txbearing=radioinstallationtxbearing, txmechtilt=radioinstallationtxmechtilt,rxsensitivity=radioinstallationrxsensitivity,rxlosses=radioinstallationrxlosses, rxantennaheight=radioinstallationrxantennaheight,rxantpatternkey=radioinstallationrxantpatternkey,rxbearing=radioinstallationrxbearing, rxmechtilt=radioinstallationrxmechtilt,btlplot=radioinstallationbtlplot,flaga=radioinstallationflaga, flagb=radioinstallationflagb,flagc=radioinstallationflagc FROM  tmpupdateradioinstallations WHERE (radioinstallation.siteid = tmpupdateradioinstallations.siteid AND radioinstallation.sector = radioinstallationsector); ");

/* add new entries */
	// create a table with existing siteid and sector combinations (i.e existing radioinstallations)
	printf("3\n");
	dropTable("existri");
	execSQLCommand("CREATE TABLE existri AS SELECT DISTINCT site.sitename, radioinstallation.sector FROM site, radioinstallation WHERE radioinstallation.siteid = site.id");

	// create a table with all the radioinstallations that were created for sites (i.e. no existing radio installation in site)
	printf("4\n");
	dropTable("newinstallations");
	dropTable("newinstallationsTMP");
	execSQLCommand("CREATE TABLE newinstallationsTMP AS SELECT DISTINCT ON (sitesitename, radioinstallationsector) sitesitename,radioinstallationlastmodified,radioinstallationsector, radioinstallationpermanent,radioinstallationeirp,radioinstallationdiversity, radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey, radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses, radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,txpatternfile,rxpatternfile,technologytechnologytype  FROM importtable WHERE (sitesitename NOT IN (SELECT sitename FROM existri)) OR (radioinstallationsector NOT IN (SELECT sector FROM existri WHERE sitename = sitesitename));");

	// resolve the techkey
	printf("4.1\n");
	execSQLCommand("DROP TABLE IF EXISTS tmptechkey");
	execSQLCommand("CREATE TABLE tmptechkey AS SELECT id, technologytype FROM technology;");

	// merge the keys with the table
	printf("4.4\n");
	dropTable("newinstallationswithTech");
	execSQLCommand("CREATE TABLE newinstallationswithTech AS SELECT * FROM newinstallationsTMP LEFT OUTER JOIN tmptechkey ON technologytype = technologytechnologytype;");
	execSQLCommand("ALTER TABLE newinstallationswithTech DROP COLUMN technologytype");
	execSQLCommand("ALTER TABLE newinstallationswithTech RENAME id TO radioinstallationtechkey");

	// create the new installations table
	execSQLCommand("CREATE TABLE newinstallations AS SELECT * FROM newinstallationswithTech");


	// create a temporary table 
	printf("5\n");
	dropTable("tmpantennapatterntable");
	execSQLCommand("CREATE TABLE tmpantennapatterntable AS SELECT id, patternfile FROM antennapattern");

	// create a table with all of the same columns as above, but remove the antennapatternfile column and fill in radioinstallationantennapatternkey
	execSQLCommand("DROP TABLE IF EXISTS newinstallationswithantennaB");
	execSQLCommand("DROP TABLE IF EXISTS newinstallationswithantennaA");
	printf("6\n");
	dropTable("newinstallationswithantennaA");
	execSQLCommand("CREATE TABLE newinstallationswithantennaA AS SELECT * FROM newinstallations LEFT OUTER JOIN tmpantennapatterntable ON txpatternfile = tmpantennapatterntable.patternfile;");

	execSQLCommand("ALTER TABLE newinstallationswithantennaA DROP COLUMN txpatternfile");
	execSQLCommand("ALTER TABLE newinstallationswithantennaA DROP COLUMN patternfile");
	execSQLCommand("ALTER TABLE newinstallationswithantennaA RENAME id TO txantpatternkey");

	printf("6.5\n");
	execSQLCommand("CREATE TABLE newinstallationswithantennaB AS SELECT * FROM newinstallationswithantennaA LEFT OUTER JOIN tmpantennapatterntable ON rxpatternfile = tmpantennapatterntable.patternfile;");

	execSQLCommand("ALTER TABLE newinstallationswithantennaB DROP COLUMN rxpatternfile");
	execSQLCommand("ALTER TABLE newinstallationswithantennaB RENAME id TO rxantpatternkey");

	//ALTER TABLE tablef RENAME antennapatternpatternfile TO txpatternfile
#if 0
	dropTable("newinstallationswithantenna");
	execSQLCommand("CREATE TABLE newinstallationswithantenna AS SELECT * FROM newinstallations LEFT OUTER JOIN tmpantennapatterntable ON antennapatternpatternfile = tmpantennapatterntable.patternfile;");
	printf("7\n");
	execSQLCommand("ALTER TABLE newinstallationswithantenna DROP COLUMN patternfile");
	printf("8\n");
	execSQLCommand("ALTER TABLE newinstallationswithantenna RENAME COLUMN id TO newinstallationswithantennaid");
#endif

	// add the new radio installations to the sites
	printf("9\n");
	dropTable("tmpsi");
	execSQLCommand("CREATE TABLE tmpsi AS SELECT DISTINCT  radioinstallationlastmodified,site.id,radioinstallationsector,radioinstallationpermanent, radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower, radioinstallationtxlosses,radioinstallationtxantennaheight,txantpatternkey,rxantpatternkey,radioinstallationtxbearing, radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight, radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc FROM newinstallationswithantennaB,site WHERE (sitesitename = sitename)");

	printf("10\n");
	execSQLCommand("ALTER TABLE tmpsi RENAME COLUMN id TO siteid");


	// add a new id key to the table
/*	execSQLCommand("ALTER TABLE tmpsi ADD COLUMN tid INTEGER;");

	// set the keys to a random value
	execSQLCommand("UPDATE tmpsi SET tid = RANDOM()*2E9;");*/

	// insert into the database
	execSQLCommand("INSERT INTO radioinstallation (lastmodified,siteid,sector, permanent,techkey,eirp, diversity,txpower,txlosses, txantennaheight,txantpatternkey,txbearing, txmechtilt,rxsensitivity,rxlosses, rxantennaheight,rxantpatternkey,rxbearing, rxmechtilt,btlplot,flaga, flagb,flagc) SELECT  DISTINCT  radioinstallationlastmodified,siteid,radioinstallationsector,radioinstallationpermanent, radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower, radioinstallationtxlosses,radioinstallationtxantennaheight,txantpatternkey,radioinstallationtxbearing, radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight, rxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc FROM tmpsi ;");

	printf("11\n");
/*
	execSQLCommand("INSERT INTO radioinstallation (lastmodified,siteid,sector, permanent,techkey,eirp, diversity,txpower,txlosses, txantennaheight,txantpatternkey,txbearing, txmechtilt,rxsensitivity,rxlosses, rxantennaheight,rxantpatternkey,rxbearing, rxmechtilt,btlplot,flaga, flagb,flagc) SELECT  DISTINCT  radioinstallationlastmodified,siteid,radioinstallationsector,radioinstallationpermanent, radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower, radioinstallationtxlosses,radioinstallationtxantennaheight,newinstallationswithantennaid,radioinstallationtxbearing, radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight, radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc FROM tmpsi ;");*/

#if 0
	execSQLCommand("INSERT INTO radioinstallation (lastmodified,siteid,sector, permanent,techkey,eirp, diversity,txpower,txlosses, txantennaheight,txantpatternkey,txbearing, txmechtilt,rxsensitivity,rxlosses, rxantennaheight,rxantpatternkey,rxbearing, rxmechtilt,btlplot,flaga, flagb,flagc) SELECT DISTINCT  radioinstallationlastmodified,site.id,radioinstallationsector,radioinstallationpermanent, radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity,radioinstallationtxpower, radioinstallationtxlosses,radioinstallationtxantennaheight,newinstallationswithantennaid,radioinstallationtxbearing, radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses,radioinstallationrxantennaheight, radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc FROM newinstallationswithantenna,site WHERE (sitesitename = sitename);");
#endif

}


void ImportF::dropTmpTables()
{
	dropTable("updatecontacts");
	dropTable("tmpallcontacts");
	dropTable("updatesitedescription");
	dropTable("tmpalldescription");
	dropTable("tmpupdateradioinstallations");
	dropTable("existri");
	dropTable("newinstallations");
	dropTable("tmpantennapatterntable");
	dropTable("newinstallationswithantenna");
	dropTable("tmpsi");
	dropTable("tablecells");
	dropTable("tablecellunique");
	dropTable("newinstallationsTMP");
	dropTable("tmptechkey");
	dropTable("newinstallationswithTech");
}

void ImportF::dropTable(const char *tname)
{
	char buf[1024];
	pqxx::work *wv;

	// create a new work object
	wv = new pqxx::work(*conn);

	try {
		// create the SQL command
		sprintf(buf,"DROP TABLE %s",tname);

		#ifdef IMPORTF_PRINTSQL
		//printf("[IMPORTF_SQL] [%s]\n",buf);
		#endif

		// execute and commit a command
		wv->exec(buf);
		wv->commit();
	}catch (const exception &ex) {
		//cout << "exception in ImportF::dropTable" << endl;
		//cout << ex.what() << endl;
	}

	delete wv;
}

void ImportF::updateCells()
{
	// create an import table with all the relevant collumns
	dropTable("tablecells");
/* change here */
	execSQLCommand("CREATE TABLE tablecells AS SELECT DISTINCT ON (sitesitename,radioinstallationsector) sitesitename,sitelocation,radioinstallationsector,celllastmodified,cellmastercell,cellrisector, cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold FROM importtable;");

/*
	execSQLCommand("CREATE TABLE tmpupdateradioinstallations AS SELECT DISTINCT ON (radioinstallation.siteid,radioinstallationsector)  radioinstallation.siteid,radioinstallation.lastmodified, radioinstallationlastmodified,radioinstallationsector, radioinstallationpermanent,radioinstallationtechkey,radioinstallationeirp,radioinstallationdiversity, radioinstallationtxpower,radioinstallationtxlosses,radioinstallationtxantennaheight,radioinstallationtxantpatternkey, radioinstallationtxbearing,radioinstallationtxmechtilt,radioinstallationrxsensitivity,radioinstallationrxlosses, radioinstallationrxantennaheight,radioinstallationrxantpatternkey,radioinstallationrxbearing,radioinstallationrxmechtilt, radioinstallationbtlplot,radioinstallationflaga,radioinstallationflagb,radioinstallationflagc,technologytechnologytype FROM importtable,site,radioinstallation WHERE (site.sitename = sitesitename AND radioinstallation.siteid=site.id AND radioinstallationlastmodified>radioinstallation.lastmodified AND radioinstallation.sector = radioinstallationsector); ");

*/

	// get all the unique cells and their associated ids
	dropTable("tablecellunique");
	execSQLCommand("CREATE TABLE tablecellunique AS SELECT sitesitename,radioinstallationsector,cellmastercell,celllastmodified,cellrisector, cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,radioinstallation.id,site.sitename FROM tablecells,radioinstallation,site WHERE (site.sitename = sitesitename AND site.location = sitelocation AND radioinstallation.siteid = site.id AND radioinstallation.sector = radioinstallationsector );");
/*
	execSQLCommand("CREATE TABLE tablecellunique AS SELECT sitesitename,radioinstallationsector,cellmastercell,celllastmodified,cellrisector, cellpermanent,cellcstraffic,cellpstraffic,celltxpower,cellbeacon,celllayerthreshold,radioinstallation.id,site.sitename FROM tablecells,radioinstallation,site WHERE (site.sitename = sitesitename AND site.location = sitelocation AND radioinstallation.siteid = site.id );");
*/
	
	// if not in the table, insert it
	execSQLCommand("INSERT INTO cell (lastmodified, mastercell, risector, permanent, cstraffic, pstraffic, txpower, beacon, layerthreshold) SELECT celllastmodified,cellmastercell, id, cellpermanent, cellcstraffic, cellpstraffic, celltxpower, cellbeacon, celllayerthreshold FROM tablecellunique WHERE id NOT IN (SELECT risector FROM cell);");

	// update the newer cells
	execSQLCommand("UPDATE cell SET lastmodified=celllastmodified, mastercell=cellmastercell, risector=tablecellunique.id,permanent=cellpermanent, cstraffic=cellcstraffic,pstraffic=cellpstraffic, txpower=celltxpower, beacon=cellbeacon, layerthreshold=celllayerthreshold FROM tablecellunique WHERE (tablecellunique.id=risector AND celllastmodified>lastmodified);");
}

int ImportF::getMaxId(const char *tableName,const char *columnName)
{
	char query[512];
	int N;
	pqxx::work *wv;
	string output;

	// create a new work object
	wv = new pqxx::work(*conn);

	try {

		// create the query
		sprintf(query,"SELECT MAX(%s) FROM %s;",columnName,tableName);

		// perform the query
		pqxx::result R = wv->exec(query);

		// print the sites
		pqxx::result::const_iterator r = R.begin();

		output = string(r[0].c_str());
	
	}catch (const exception &ex) {
		cout << "exception in ImportF::getMaxId" << endl;
		cout << ex.what() << endl;
	}

	delete wv;

	return atoi(output.c_str());
}

void ImportF::fixSequence(const char *tableName,const char *columnName)
{
	int maxId;
	char query[512];

	// get the maximum id
	maxId = getMaxId(tableName,columnName);

	// calculate the new maximum id
	maxId++;

	// create the query
	sprintf(query,"SELECT setval('%s_%s_seq',%d);",tableName,columnName,maxId);
	printf("ImportF::fixSequence: [%s]\n",query);

	// execute the query
	execSQLCommand(query);
}



void ImportF::updateFrequencyAllocation()
{
	// create a merge table with the site
	execSQLCommand("CREATE TABLE famerge1 AS SELECT * FROM importtexttable LEFT OUTER JOIN site ON sitename = sitesitename;");

	// remove the unused columns
	execSQLCommand("CREATE TABLE famerge2 AS SELECT radioinstallationsector,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel,id AS sitesiteid FROM famerge1;");

	// get the radio installations
	execSQLCommand("CREATE TABLE famerge3 AS SELECT * FROM famerge2 LEFT OUTER JOIN radioinstallation ON radioinstallation.siteid = famerge2.sitesiteid AND radioinstallation.sector = radioinstallationsector;");

	// remove the unused columns
	execSQLCommand("CREATE TABLE famerge4 AS SELECT radioinstallationsector,frequencyallocationlistlastmodified,frequencyallocationlistmachineid,frequencyallocationlistci,frequencyallocationlistcarrier,frequencyallocationlistchannel,id AS radioinstallationid FROM famerge3;");

	// get the cells
	execSQLCommand("CREATE TABLE famerge5 AS SELECT * FROM famerge4 LEFT OUTER JOIN cell ON cell.risector = radioinstallationid");

	// perform the insert
	execSQLCommand("INSERT INTO frequencyallocationlist (lastmodified,machineid,ci,carrier,channel) SELECT frequencyallocationlistlastmodified,frequencyallocationlistmachineid,id,frequencyallocationlistcarrier,frequencyallocationlistchannel FROM famerge5");

	// drop the tables
	execSQLCommand("DROP TABLE famerge1");
	execSQLCommand("DROP TABLE famerge2");
	execSQLCommand("DROP TABLE famerge3");
	execSQLCommand("DROP TABLE famerge4");
	execSQLCommand("DROP TABLE famerge5");
}


