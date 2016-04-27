 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cDatabase.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Thane Thomson (roeland.frans@gmail.com)
 *		  : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Database engine encapsulation, implementation
 *
**************************************************************************
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

#include "cDatabase.h"

using std::string;
using namespace Qrap;

/*
 * Global variable declaration.
 */
cDatabase Qrap::gDb;

//*****************************************************************
// the internal cTransactor class
void cDatabase::cTransactor::operator () (argument_type& t)
{
	// execute the stored query
	mOwner->mLastResult = t.exec(mQuery);
}

//*****************************************************************
void cDatabase::cTransactor::on_doubt ()
{
	throw cAlert(acTransactionFailed);
}

//******************************************************************
cDatabase::cDatabase (): mConnection(NULL)
{
	mHostAddr = "";
	globalMachineID=0;
}

//**************************************************************************
cDatabase::~cDatabase ()
{
	if (mConnection)
	{
		//QRAP_INFO("Shutting down database connection.");
		try
		{
			delete mConnection;
		} catch (...)
		{
			//QRAP_ERROR_CODE("Unable to destroy database connection.", acDbDisconnect);
			cout << "cDatabase::~cDatabase:  Unable to destroy database connection." << endl;
		}
	}
}


//***********************************************************************************
bool cDatabase::Connect (const string& username, const string& password, bool Create,
			const string& database, const string& host, const int Iport)
{
/*	cout << "In cDatabase::Connect: " << username << endl;
	cout << "In cDatabase::Connect: " << password << endl;
	cout << "In cDatabase::Connect: " << host << endl;
	cout << "In cDatabase::Connect: " << database << endl;
	cout << "In cDatabase::Connect: " << Iport << endl;
	msAlertCode = acOk;
	mHostAddr = host;
	mName = database;
	mPort = Iport;
*/
	mHostAddr = gSettings.GetValue("database.host");
	mName = gSettings.GetValue("database.name");
	string port = gSettings.GetValue("database.port");
	mPort = (port.length() == 0) ? 5432 : atoi(port.c_str());
	//	mPort =5433;
	gSettings.GetDbStructure(mStructure);
	mConnected=true;
	// try to connect to the database
	try
	{
		QRAP_INFO("Authenticating user "+username+"...");
		string query;
		char * text = new char[33];
		gcvt(mPort,8,text);
		query = "hostaddr="+mHostAddr+" dbname="+mName+" port=";
		query += text;
		query += " user="+username+" password="+password;
		delete [] text;
		cout << query << endl;
//		pqxx::connection *mConnection;
		mConnection = new pqxx::connection(query);
	} catch (pqxx::broken_connection& e)
	{
		mConnected=false;
		QRAP_WARN("Invalid username or password specified. ... or port ... or host. Did you create the database? See manual for instructions on creating the database");
//		QRAP_FATAL_CODE("Invalid username or password specified. ... or port ... or host" , acDbAuth);
	} catch (pqxx::internal_error& e)
	{
		mConnected=false;
		QRAP_WARN("Internal PQXX error. Did you create the database? See manual for instructions on creating the database.");
//		QRAP_FATAL_CODE("Internal PQXX error.", acInternalError);
	} catch (...)
	{
		mConnected=false;
		QRAP_WARN("Unknown exception caught while connecting. Did you create the database? See manual for instructions on creating the database.");
//		QRAP_FATAL_CODE("Unknown exception caught while connecting.", acDbConnect);
	}
	
	cout << "After connection attempt" << endl;
	if ((msAlertCode != acOk)||(!mConnected))
	{
		mConnected =false;
		return false;
	}
	else mConnected = true;
	cout << "returning true ? ... " << endl;
	if (!Create)
	{
		mUsername = username;
		cout << "cDatabase::Connect. Not creating.  mUsername = " << mUsername << endl;
		mUserGroup = GetGroupName(mUsername);
		if (!PerformRawSql("select name, value from qrap_config where type='local' and username='"+mUsername+"';"))
		{
			mConnected=false;
			QRAP_WARN("Unable to load settings for user "+mUsername+".");
		} 
		else
		{
			int i, count = mLastResult.size();
			for (i=0;i<count;i++)
			{
				mSettings[string(mLastResult[i]["name"].c_str())] = mLastResult[i]["value"].c_str();
			}
		
		}

		getsetMachineID();
		for (DbStructureIterator table=mStructure.begin();
			table != mStructure.end(); table++)
		{
			string query= "SELECT setval('public.";
			query +=table->first;
			query +="_id_seq', max(id)) from ";
			query +=table->first;
			query += ";";	
			cout << query << endl;	
			if (!PerformRawSql(query))
				cout << "Ag bliksem: " << table->first << endl;

		}
	}

	//create any tables that might be new.
//	cout << "Implementing any possible table additions to the database structure" << endl;
//	gDb.Create (username, password);
	mConnected=true;	
	return true;
}


//******************************************************************************
bool cDatabase::Disconnect ()
{
	mConnected=false;
	if (mConnection)
	{
		// clear the qrap_undo table
		if (!PerformRawSql("delete from qrap_undo;"))
		{
			QRAP_WARN("Unable to clear the qrap_undo table.");
		}
	
		bool success = true;
		
		QRAP_INFO("Shutting down database connection.");
		try
		{
			delete mConnection;
		} catch (...)
		{
			QRAP_ERROR_CODE("Unable to destroy database connection.", acDbDisconnect);
			success = false;
		}
		
		mConnection = NULL;
		mUsername = "";
		
		return success;
	}
	
	return false;
}

//******************************************************************************
bool cDatabase::PerformRawSql (const string& command)
{
	msAlertCode = acOk;

	#ifdef QRAP_DEBUG
//	cout << "[SQL] " << command << endl;
	#endif
	
	try
	{
		cDatabase::cTransactor trans(this, command);

//cout << "cDatabase::PerformRawSql: After construct trans" << endl;
		
		mConnection->perform(trans);
//cout << "cDatabase::PerformRawSql: executing trans" << endl;

	} catch (pqxx::broken_connection& e)
	{
		QRAP_FATAL_CODE("Broken connection while attempting query.", acBrokenConnection);
	} catch (pqxx::internal_error& e)
	{
		QRAP_FATAL_CODE("Internal pqxx error while attempting query.", acInternalError);
	} catch (pqxx::data_exception& e)
	{
		QRAP_FATAL_CODE("Invalid parameters supplied for SQL query.", acDbInvalidData);
	} catch (pqxx::integrity_constraint_violation& e)
	{
		QRAP_FATAL_CODE("Integrity constraint violation.", acDbIntegrityViolation);
	} catch (pqxx::invalid_sql_statement_name& e)
	{
		QRAP_FATAL_CODE("Invalid SQL statement name.", acDbInvalidStatement);
	} 
	catch (pqxx::undefined_column& e)
	{
		QRAP_FATAL_CODE("Undefined column in statement.", acDbFieldNotExist);
	} 
	catch (pqxx::undefined_function& e)
	{
		QRAP_FATAL_CODE("Undefined function in statement.", acDbFunctionNotExist);
	} catch (pqxx::undefined_table& e)
	{
		QRAP_FATAL_CODE("Undefined table in statement.", acDbTableNotExist);
	} catch (pqxx::syntax_error& e)
	{
		QRAP_FATAL_CODE("Syntax error in statement.", acDbSyntax);
	} catch (pqxx::insufficient_privilege& e)
	{
		QRAP_FATAL_CODE("Insufficient privileges for statement.", acDbNotAllowed);
	} catch (...)
	{
//		QRAP_FATAL_CODE("General error while attempting query.", acTransactionFailed);
	}
	
	if (msAlertCode != acOk)
	{
		Log("Offending command: "+command);
	}
	
	return (msAlertCode == acOk);
}

//*******************************************************************************
bool cDatabase::Select (const string& columns, const string& tableName, const string& where,
	const string& orderBy, bool ascending, bool hideDefault)
{
	StringArray cols;
	StringArray newCols;
	string      fixedCols, actTable, whereStr;
	int         i, count;
	
	
	msAlertCode = acOk;
	
	// if all columns are to be selected
	if (columns == "*")
	{
		for (DbFieldMapIterator field=mStructure[tableName].mFields.begin();
			field != mStructure[tableName].mFields.end(); field++)
		{
			cols.push_back(field->first);
		}
	} else
	{
		// otherwise, if we're selecting custom columns
		// split up the columns
		SplitString(columns, ',', cols);
	}
	
	count = cols.size();
	for (i=0;i<count;i++)
	{
		// if this field contains a formula or sub-select query of some type
		if (!IsKeyword(cols[i]))
		{
			newCols.push_back(cols[i]);
			continue;
		}
		
		// skip the field if it's not visible
		if (!mStructure[tableName].mFields[cols[i]].mVisible)
			continue;
			
		// if we're compiling the server edition
		#ifdef QRAP_SERVER_EDITION
		// and this field is meant for the client only
		if (mStructure[tableName].mFields[cols[i]].mClientOnly)
			continue;
		// otherwise, if we're compiling the client edition
		#else
		// and this field is meant for the server only
		if (mStructure[tableName].mFields[cols[i]].mServerOnly)	
			continue;
		#endif
		
		newCols.push_back(cols[i]);
	}
	cols = newCols;
	
	// put the columns back together
	count = cols.size();
	for (i=0;i<count;i++)
	{
		// if this field contains a formula of some type
		if (!IsKeyword(cols[i]))
			fixedCols += cols[i];
		else
		// if it's a PostGIS field, it needs to be returned in a specific format
		if (mStructure[tableName].mFields[cols[i]].mPostGis)
			fixedCols += "ST_AsEWKT("+cols[i]+") as "+cols[i];
		else
			fixedCols += cols[i];
		if ((i+1) < count)
			fixedCols += ",";
	}
	
	// if a limiting view exists for this user's group
	actTable = tableName+"_"+mUserGroup;
	if (!ViewExists(actTable))
		actTable = tableName;
	
	if (hideDefault)
	{
		whereStr = "(id > 0)";
		if (where.length() > 0)
			whereStr += " and ";
	}
	whereStr += where;
	if (whereStr.length() > 0)
		whereStr = " where "+whereStr;
	
	// put the query together
	if (!PerformRawSql("select "+fixedCols+" from "+actTable+
		whereStr+
		" order by "+orderBy+" "+
		((ascending) ? string("asc") : string("desc"))+";"))
		return false;
	
	return true;
}


//*************************************************************************
int cDatabase::Insert (const StringMap& values, const string& tableName)
{
	StringMap         vals = values;
	string            colStr, valStr;
	StringMapIterator it;
	int  		  id;
	string            srid = gSettings.GetValue("postgis.srid"); // for PostGIS fields
	bool              idSpecified; // was the ID manually specified?
	
	// set to the default SRID
	if (srid.length() == 0)
		srid = cPostGisDefaultSrid;

	// make sure to override any user-specified values for these two columns
	#ifndef QRAP_SERVER_EDITION
	vals["serverid"] = "null";
	#else
	if (vals.count("serverid") > 0)
		vals.erase(vals.find("serverid"));
	#endif
	idSpecified = (vals.count("id") > 0) ? true : false;
// 	if (vals.count("id") > 0)
// 		vals.erase(vals.find("id"));
	vals["lastmodified"] = "now()";
	if ((tableName!="machine")&&(tableName!="receiver_capabilities")&&(tableName!="height_costs")&&(tableName!="cable_type")&&(tableName!="combiner_splitter_type"))
	{
		char *temp;
		temp = new char[10];
		gcvt(globalMachineID,8,temp);
		vals["machineid"]=temp;
		delete [] temp;
	}
	msAlertCode = acOk;
	
	it = vals.begin();
	while (it != vals.end())
	{
		colStr += it->first;
		// if this column's a PostGIS field - insert it with the GeomFromText function
		if (mStructure[tableName].mFields[it->first].mPostGis)
			valStr += "ST_GeomFromText('"+it->second+"', "+srid+")";
		else
			valStr += it->second;
		
		it++;
		if (it != vals.end())
		{
			colStr += ",";
			valStr += ",";
		}
	}
	
	// perform the insert
	if (!PerformRawSql("insert into "+tableName+" ("+colStr+") values ("+valStr+");"))
	{
		return -1;
	}
	
	// if no rows were affected
	if (mLastResult.affected_rows() == 0)
	{
		// it means that the user does not have access to perform this particular action
		QRAP_ERROR_CODE("User not authorised to perform action.", acDbNotAllowed);
		return -1;
	}
	
	// get the item's ID from the table
	if (idSpecified)
		id = atoi(vals["id"].c_str());
	else
	{
		if (!PerformRawSql("select currval('"+tableName+"_id_seq');"))
		{
			QRAP_ERROR("Unable to select item ID from database.");
			return -1;
		}
		id = atoi(mLastResult[0][0].c_str());
	}
	
	return id;
}

//*******************************************************************************
bool cDatabase::Update (const StringMap& values, const string& tableName,
	const string& where, bool skipDefault)
{
	StringMap         vals = values;
	string            valStr, whereStr;
	StringMapIterator it;
	string            srid = gSettings.GetValue("postgis.srid"); // for PostGIS fields
	
	// set to the default SRID
	if (srid.length() == 0)
		srid = cPostGisDefaultSrid;

	msAlertCode = acOk;
	// make sure to override any user-specified values
	if (vals.count("serverid") > 0)
		vals.erase(vals.find("serverid"));
	if (vals.count("id") > 0)
		vals.erase(vals.find("id"));
	
	vals["lastmodified"] = "now()";
	
	it = vals.begin();
	while (it != vals.end())
	{
		valStr += it->first+"=";
		
		// if this field's a PostGIS field
		if (mStructure[tableName].mFields[it->first].mPostGis)
			valStr += "ST_GeomFromText('"+it->second+"', "+srid+")";
		else
			valStr += it->second;
		
		it++;
		if (it != vals.end())
			valStr += ",";
	}
	
	if (skipDefault)
	{
		whereStr = "(id > 0)";
		if (where.length() > 0)
			whereStr += " and ";
	}
	
	whereStr += where;
	if (whereStr.length() > 0)
		whereStr = " where "+whereStr;
	
	if (!PerformRawSql("update "+tableName+" set "+valStr+whereStr+";"))
	{
		return false;
	}
	
	// if no rows were affected by the update, chances are the user doesn't have access to
	// perform this particular update - it might also be simply that this particular action
	// affected no rows, but it is assumed that this function will be called from the user
	// interface, and therefore all updates will be meaningful
	if (mLastResult.affected_rows() == 0)
	{
		QRAP_ERROR_CODE("User not authorised to perform action.", acDbNotAllowed);
		return false;
	}

	return true;
}

//***************************************************************************
bool cDatabase::Delete (const string& tableName, const string& where)
{
	msAlertCode = acOk;
	
	if (!PerformRawSql("delete from "+tableName+" where (id > 0) "+
		((where.length() > 0) ? string("and ("+where+")") : string(""))+";"))
	{
		return false;
	}
	
	// if no rows were affected, and, like Database::Update, we assume that all deletions
	// come from the user interface and that they should be meaningful, this would mean
	// that the user does not have access to delete this/these item/items
	if (mLastResult.affected_rows() == 0)
	{
		QRAP_ERROR_CODE("User not authorised to perform action.", acDbNotAllowed);
		return false;
	}
	
	return true;
}

//*******************************************************************************
string cDatabase::GetGroupName (const string& username)
{
	msAlertCode = acOk;
	
	if (!PerformRawSql("select groname from pg_group where (select usesysid from pg_user where usename='"+
		username+"') = any (pg_group.grolist);"))
	{
		QRAP_ERROR("Unable to retrieve group name for user "+username+".");
		return string("");
	}
	
	if (mLastResult.size() == 0)
	{
		QRAP_WARN("User "+username+" does not belong to a group.");
		return string("");
	}
	
	return string(mLastResult[0][0].c_str());
}

//*************************************************************************8
bool cDatabase::Create (const string& username, const string& password)
{
	msAlertCode = acOk;
	
	StringArray createdTables, forwardTables, backwardTables;
	string      query;
	
	// now log in to the newly created database
	if (!Connect(username, password, true))
	{
		QRAP_FATAL("Strange: could not connect to newly created database "+mName+".");
		return false;
	}
	cout << "Connected" << endl;

	// check that PostGIS has been installed
	if (((!ViewExists("geometry_columns"))&&(!TableExists("geometry_columns"))) || (!TableExists("spatial_ref_sys",true)))
	{
		QRAP_FATAL("Cannot find PostGIS. Please install PostGIS first - consult the user manual for details.");
		return false;
	}
	cout << "Checked PostGis" << endl;

	// create the engine group
	if (!PerformRawSql("create group engine;"))
	{
		QRAP_WARN("Unable to create engine group - probably already exists.");
	}
	cout << "Created Engine Group" << endl;	

	// grant the engine group the right to create tables
	if (!PerformRawSql("grant create on database "+mName+" to group engine;"))
	{
		QRAP_FATAL("Unable to grant table creation access to group engine.");
		return false;
	}
	cout << "Granting table creation rights Engine Group" << endl;	

	// create all the tables
	for (DbStructureIterator table=mStructure.begin();
		table != mStructure.end(); table++)
	{
		cout << "Creating table " << table->first << endl;	
		if (!CreateTable(table->first))
			return false;
	}
	
	// create all the views
	for (DbStructureIterator table=mStructure.begin();
		table != mStructure.end(); table++)
	{
		// run through this table's views
		for (DbViewMapIterator view=table->second.mViews.begin();
			view != table->second.mViews.end(); view++)
		{
			cout << "Creating view " << view->first << endl;
			// create the view
			if (!PerformRawSql("create view "+view->first+" as "+view->second.mAs+";"))
				QRAP_WARN("Unable to create view \""+view->first+"\".");
		}
	}
	
	// now create the system tables
	if (!PerformRawSql("create table qrap_config (id serial primary key unique not null, lastmodified timestamp, "
		"type varchar(20), username varchar(100), name varchar(100), value text);"))
		QRAP_WARN("Unable to create qrap_config table.");
	cout << "Created qrap_config "  << endl;	

	if (!PerformRawSql("create table qrap_undo (id serial primary key unique not null, "
		"username varchar(100), cmd text);"))
		QRAP_WARN("Unable to create qrap_undo table.");
	cout << "Created qrap_undo "  << endl;
	
	if (!PerformRawSql("create table qrap_regions (id serial primary key unique not null, "
		"lastmodified timestamp, "
		"name varchar(100), parent integer references qrap_regions(id), description text, region polygon);"))
		QRAP_WARN("Unable to create qrap_regions table.");
	cout << "Created qrap_regions "  << endl;
	
	#ifdef QRAP_SERVER_EDITION
	if (!PerformRawSql("create table qrap_users (id serial primary key unique not null, "
		"username varchar(100), password varchar(40), groupname varchar(100), fullnames varchar(300), "
		"idno varchar(30), email varchar(200));"))
		QRAP_WARN("Unable to create qrap_users table.");
	cout << "Created qrap_users "  << endl;
	if (!PerformRawSql("create table qrap_groups (id serial primary key unique not null, "
		"groupname varchar(100), rulename varchar(100), tablename varchar(100), "
		"cmd varchar(20), limiter text, docmd text);"))
		QRAP_WARN("Unable to create qrap_groups table.");
	cout << "Created qrap_groups "  << endl;
	#endif

	getsetMachineID();
	cout << " Na getsetMachineID() in cDatabase::Create " << endl;
	gDb.SetSetting("location","DD:MM:SS");
	gDb.SetSetting("predictioneirp","explicit");
	gDb.SetSetting("impedence","50");
	gDb.SetSetting("eirp","dBm");
	gDb.SetSetting("power","W");
	gDb.SetSetting("sensitivity","dBm");
	gDb.SetSetting("mode","normal");
	gDb.SetSetting("DownLink","true");
	gDb.SetSetting("BTLDir","Data/BTL");
	gDb.SetSetting("OutputDir","Data/Output");
	gDb.SetSetting("UseClutter","true");
	gDb.SetSetting("ClutterSource","2");
	gDb.SetSetting("DEMsource","1");
	gDb.SetSetting("PlotResolution","90");
	gDb.SetSetting("kFactorServer","1");
	gDb.SetSetting("kFactorInt","1.33");

	// disconnect
	if (!Disconnect())
	{
		QRAP_ERROR("Strange: could not disconnect from database.");
		return false;
	}

	return true;
}

//*************************************************************************
string cDatabase::ConstructCreateTableQuery (const string&  tableName,
					const DbTable& table,
					bool temporary)
{
	string query;
	DbFieldMap::const_iterator fi;
	int i, count = table.mFields.size();
	IntArray nonPostGisFields;
	nonPostGisFields.clear();
	
	query = "create "+((temporary) ? string("temporary") : string(""))+
		"table "+tableName+" (";

	// run through the fields and add the field IDs that aren't PostGIS fields to the list
	for (i=0;i<count;i++)
		if (!GetFieldWithOrder(tableName, i)->second.mPostGis)
			nonPostGisFields.push_back(i);
	
	count = nonPostGisFields.size();
	
	// add all the fields
	for (i=0;i<count;i++)
	{
		fi = GetFieldWithOrder(tableName, nonPostGisFields[i]);
		
		// if we're compiling the client version
		#ifndef QRAP_SERVER_EDITION
		// if this field is destined for the server version only
		if (fi->second.mServerOnly)
			continue;
		#else
		// if we're compiling the server version
		// and this field is destined for the client only
		if (fi->second.mClientOnly)
			continue;
		#endif
	
		// fi->first = string, fi->second = Field
		query += fi->first+" "+fi->second.mType;

		// if there are still more fields, delimit them
		if ((i+1) < count)
			query += ", ";
	}
	
	// add the footer
	if (table.mFooter.length() > 0)
		query += ", "+table.mFooter;
	
	// close it off
	query += ");";
	
	return query;
}

//****************************************************************************
bool cDatabase::CreateTable (const string& tableName, const string& tableSuffix, bool temporary)
{
	msAlertCode = acOk;

	// if the table does not exist in the database structure template
	if (mStructure.count(tableName) == 0)
	{
		QRAP_ERROR_CODE("Table "+tableName+" does not exist in the database structure.", acInternalError);
		return false;
	}

	if (TableExists(tableName))
		return true;
	
	StringArray forwardTables, backwardTables;
	int         i, count;
	string      srid;
	
	// get the related tables
	GetRelatedTables(tableName, StringArray(), forwardTables, backwardTables);
	
	// attempt to create all the forward tables first
	count = forwardTables.size();
	for (i=0;i<count;i++)
	{
		// is there an internal foreign key reference within this table to itself?
		if (forwardTables[i] == tableName) continue;
		
		if (mStructure.count(forwardTables[i]) == 0)
		{
			QRAP_ERROR_CODE("Table "+forwardTables[i]+" does not exist in the database structure.", acInternalError);
			return false;
		}
		// attempt to create the table
		if (!CreateTable(forwardTables[i], tableSuffix, temporary))
			return false;
	}
	
	bool customSeqCreated = false;
	
	// if this table has a custom sequence type... create it first
	if (mStructure[tableName].mCustomSequence)
	{
		// create the custom sequence for this table
		if (!PerformRawSql("create sequence "+tableName+"_id_seq;"))
		{
			// give a warning in the debug file - no dying here
			QRAP_ERROR("Unable to create sequence "+tableName+"_id_seq - it probably already exists.");
		} else
			customSeqCreated = true;
	}
	
	// create this table
	if (!PerformRawSql(ConstructCreateTableQuery(tableName+tableSuffix, mStructure[tableName], temporary)))
	{
		QRAP_ERROR("Unable to create table "+tableName+".");
		return false;
	}
	
	if (customSeqCreated)
	{
		if (!PerformRawSql("alter sequence "+tableName+"_id_seq owned by "+tableName+tableSuffix+".id;"))
		{
			// give a warning
			QRAP_ERROR("Unable to assign owner to sequence "+tableName+"_id_seq.");
		}
	}
	
	// get the PostGIS SRID
	srid = gSettings.GetValue("postgis.srid");
	// default to this if not specified
	if (srid.length() == 0)
		srid = cPostGisDefaultSrid;
	
	string query;
	query = "COMMENT ON TABLE ";
	query += tableName;
	query += " IS '";
	query += mStructure[tableName].mDescription;
	query += "';";
	PerformRawSql(query);

	for (DbFieldMapIterator field=mStructure[tableName].mFields.begin();
		field != mStructure[tableName].mFields.end(); field++)
	{
		if (field->second.mPostGis)
		{
			// add the PostGIS field
			if (!PerformRawSql("select AddGeometryColumn('"+tableName+"', '"+
				field->first+"', "+srid+", '"+field->second.mType+"', 2);"))
			{
				QRAP_FATAL_CODE("Unable to add PostGIS column \""+field->first+"\".", acInternalError);
				return false;
			}
		}
		query = "COMMENT ON COLUMN ";
		query += tableName;
		query +=".";
		query += field->first;
		query += " IS '";
		query += field->second.mDescription;
		query += "';";
		PerformRawSql(query);
	}
	
	// give the engine group full access to the table
	if (!PerformRawSql("grant all on table "+tableName+tableSuffix+" to group engine;"))
	{
		QRAP_ERROR("Unable to grant engine group full access to table "+tableName+tableSuffix+".");
		return false;
	}
	
	return true;
}

//*********************************************************************************
void cDatabase::GetRelatedTables (const string& tableName, const StringArray& skipTables,
	StringArray& forwardTables, StringArray& backwardTables)
{
	// run through the mStructure member to find all the required info
	for (DbStructureIterator table=mStructure.begin();
		table != mStructure.end(); table++)
	{
		// skip this table if necessary
		if (StringArrayPos(skipTables, table->first) > -1) continue;
		
		// if this is the table in question... check for forward references
		if (table->first == tableName)
		{
			// run through its foreign key references
			for (DbFieldMapIterator field=table->second.mFields.begin();
				field != table->second.mFields.end(); field++)
			{
				if (field->second.mIsForeign)
				{
					// if the foreign table is not yet recorded
					if (StringArrayPos(forwardTables, field->second.mForeignTable) == -1)
					{
						// add the foreign table's name
						forwardTables.push_back(field->second.mForeignTable);
					}
				}
			}
		} else
		// search for backward references
		{
			for (DbFieldMapIterator field=table->second.mFields.begin();
				field != table->second.mFields.end(); field++)
			{
				if (field->second.mIsForeign && (field->second.mForeignTable == tableName))
				{
					//if this table doesn't appear in the backwardTables list, add it
					if (StringArrayPos(backwardTables, table->first) == -1)
					{
						backwardTables.push_back(table->first);
					}
				}
			}
		}
	}
}

//*********************************************************************************
string cDatabase::GetForeignField (const string& tableName, const string& foreignTable)
{
	for (DbFieldMapIterator field=mStructure[tableName].mFields.begin();
		field != mStructure[tableName].mFields.end(); field++)
	{
		if (field->second.mForeignTable == foreignTable)
			return field->first;
	}
	
	// not found
	return "";
}

//*********************************************************************************
bool cDatabase::TableExists (const string& tableName, bool Spatial)
{
	msAlertCode = acOk;

	if (!PerformRawSql("select tablename from pg_tables where tablename='"+tableName+"';"))
	{
		QRAP_ERROR("Unable to perform select query to check if table exists.");
		
		throw cAlert(msAlertCode);
	}
	
	if (mLastResult.size() == 0)
		return false;
	
	if (!Spatial)
	{
		string query= "SELECT setval('";
		query += mName;
		query +=".";
		query +=tableName;
		query +="_id_seq', (select max(id) from ";
		query += mName;
		query +=".";
		query +=tableName;
		query += "));";
		if (!PerformRawSql(query))
		{
//			QRAP_ERROR("Unable to get current Sequence value for table");
			cout << "Unable to get current Sequence value for table" << endl;
//			throw cAlert(msAlertCode);
		}
	}	
	return true;
}

//*******************************************************************************
bool cDatabase::ViewExists (const string& viewName)
{
	msAlertCode = acOk;
	
	if (!PerformRawSql("select viewname from pg_views where viewname='"+viewName+"';"))
	{
		QRAP_ERROR("Unable to perform select query to check if view exists.");
		
		throw cAlert(msAlertCode);
	}
	
	if (mLastResult.size() == 0)
		return false;
	
	return true;
}

//*****************************************************************************
bool cDatabase::GetDefaults (const string& tableName, StringMap& output)
{
	if (!Select("*", tableName, "id=0", "id", true, false))
		return false;
	
	string fieldName, fieldVal;
	
	if(mLastResult.size() == 0)
		return true;
	
	// run through the fields
	for (pqxx::const_tuple_iterator field=mLastResult[0].begin();
		field != mLastResult[0].end(); field++)
	{
		fieldName = field->name();
		if ((fieldName == "id") || (fieldName == "serverid") ||
			(fieldName == "lastmodified"))
			continue;
		// add it as a default - if it isn't null
		fieldVal = field->c_str();
		if (fieldVal.length() > 0)
			output[fieldName] = fieldVal;
	}
	
	return true;
}


//******************************************************************************
bool cDatabase::SetDefaults (const string& tableName, const StringMap& defaults)
{
	// check if there is a default
	if (!PerformRawSql("select id from "+tableName+" where id=0;"))
	{
		QRAP_ERROR("Unable to check if there is a default for table "+tableName+".");
		return false;
	}
	
	StringMap         defs = defaults;
	StringMapIterator it;
	string            srid = gSettings.GetValue("postgis.srid"); // for PostGIS fields
	
	// set to the default SRID
	if (srid.length() == 0)
		srid = cPostGisDefaultSrid;
	
	// make sure to remove the important fields
	defs["id"] = "0";
	defs["lastmodified"] = "now()";
	
	if (defs.count("serverid"))
		defs.erase(defs.find("serverid"));
	
	// if there is no default
	if (mLastResult.size() == 0)
	{
		string colStr, valStr;
		
		// the only entry whose server ID we know for sure
		defs["serverid"] = "0";
		
		// build the columns and values strings
		/*it = defs.begin();
		while (it != defs.end())
		{
			colStr += it->first;
			// if this column's a PostGIS field
			if (mStructure[tableName].mFields[it->first].mPostGis)
				valStr += "ST_GeomFromText('"+it->second+"', "+srid+")";
			else
				valStr += it->second;
			
			it++;
			if (it != defs.end())
			{
				colStr += ",";
				valStr += ",";
			}
		}
		
		// insert one
		if (!PerformRawSql("insert into "+tableName+" ("+colStr+") values ("+valStr+");"))
		{
			return false;
		}*/
		if (Insert(defs, tableName) != 0)
			return false;
	} else
	{
		/*string valStr;
		
		it = defs.begin();
		while (it != defs.end())
		{
			valStr += it->first+"=";
			if (mStructure[tableName].mFields[it->first].mPostGis)
				valStr += "ST_GeomFromText('"+it->second+"', "+srid+")";
			else
				valStr += it->second;
			it++;
			if (it != defs.end())
				valStr += ",";
		}
		
		if (!PerformRawSql("update "+tableName+" set "+valStr+" where id=0;"))
		{
			return false;
		}*/
		if (!Update(defs, tableName, "id=0", false))
			return false;
	}
	
	return true;
}

//*******************************************************************************
bool cDatabase::UserExists (const string& username)
{
	if (!PerformRawSql("select usename from pg_user where usename='"+username+"';"))
		return false;
	
	return (mLastResult.size() == 0) ? false : true;
}

//*******************************************************************************
bool cDatabase::GroupExists (const string& groupName)
{
	if (!PerformRawSql("select groname from pg_group where groname='"+groupName+"';"))
		return false;
	
	return (mLastResult.size() == 0) ? false : true;
}

//*******************************************************************************
bool cDatabase::AddUser (const string& username, const string& password, const string& group,
	const string& fullNames, const string& idNo, const string& email)
{
	msAlertCode = acOk;
	
	// if the user already exists
	if (UserExists(username))
	{
		// delete the user
		if (!RemoveUser(username))
			return false;
	}
	
	// create the user
	if (!PerformRawSql("create user "+username+" with password '"+password+"' in group "+group+";"))
	{
		QRAP_ERROR("Unable to create user "+username+".");
		return false;
	}
	
	// create an engine user
	if (!PerformRawSql("create user "+username+"_engine with password '"+password+"_engine' in group engine;"))
	{
		QRAP_ERROR("Unable to create engine user "+username+"_engine.");
		return false;
	}
	
	#ifdef QRAP_SERVER_EDITION
	// create the user in the qrap_users table
	if (!PerformRawSql("insert into qrap_users (username,password,groupname,fullnames,idno,email) values ('"+
		username+"','"+password+"','"+group+"','"+fullNames+"','"+idNo+"','"+email+"');"))
		return false;
	#endif
	
	return true;
}


//**********************************************************************************
bool cDatabase::RemoveUser (const string& username)
{
	msAlertCode = acOk;
	
	if (!UserExists(username))
	{
		QRAP_ERROR_CODE("User "+username+" does not exist.", acInternalError);
		return false;
	}
	
	if (!PerformRawSql("drop user "+username+";"))
	{
		QRAP_ERROR("Unable to delete user "+username+".");
		return false;
	}
	
	// attempt to remove the engine user for the given user
	PerformRawSql("drop user "+username+"_engine;");
	
	#ifdef QRAP_SERVER_EDITION
	// remove the user from the qrap_users table
	if (!PerformRawSql("delete from qrap_users where username='"+username+"';"))
		return false;
	#endif
	
	return true;
}

//*********************************************************************************
bool cDatabase::AddGroup (const string& groupName, const GroupPrivileges& privileges)
{
	msAlertCode = acOk;

	if (GroupExists(groupName))
	{
		if (!RemoveGroup(groupName))
			return false;
	}
	
	string groupPriv;
	int    i, count;
	
	if (privileges.mCreateUsers)
	{
		groupPriv += " createuser";
		#ifdef QRAP_SERVER_EDITION
		if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values ('"+
			groupName+"','privilege','createuser');"))
			return false;
		#endif
	}
	if (privileges.mCreateRoles)
	{
		groupPriv += " createrole";
		#ifdef QRAP_SERVER_EDITION
		if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values ('"+
			groupName+"','privilege','createrole');"))
			return false;
		#endif
	}
	
	if (groupPriv.length() > 0)
		groupPriv = " with"+groupPriv;
	
	if (!PerformRawSql("create group "+groupName+groupPriv+";"))
	{
		QRAP_ERROR("Unable to create group "+groupName+".");
		return false;
	}
	
	// run through the tables
	for (TablePrivilegesMap::const_iterator table=privileges.mTablePrivileges.begin();
		table != privileges.mTablePrivileges.end(); table++)
	{
		if (table->second.mSelect)
		{
			// grant selecting on the table to this group
			if (!PerformRawSql("grant select on "+table->first+" to "+groupName+";"))
				return false;

			if (table->second.mViewLimiter.length() > 0)
			{
				if (!CreateView(table->first, table->second.mViewLimiter, groupName))
					return false;
			}
			#ifdef QRAP_SERVER_EDITION
			else
			{
				// grant the user simple select access - without having to create a view
				if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values "
					"('"+groupName+"','"+table->first+"','select');"))
					return false;
			}
			#endif
		}
		if (table->second.mUpdate)
		{
			if (!PerformRawSql("grant update on "+table->first+" to "+groupName+";"))
				return false;
			
			count = table->second.mUpdateRules.size();
			
			#ifdef QRAP_SERVER_EDITION
			if (!count)
			{
				// grant the user simple update access
				if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values "
					"('"+groupName+"','"+table->first+"','update');"))
					return false;
			}
			#endif
			
			// run through the update rules
			for (i=0;i<count;i++)
			{
				// create the rule
				if (!CreateRule("update", table->first, table->second.mUpdateRules[i], groupName))
					return false;
			}
		}
		if (table->second.mInsert)
		{
			if (!PerformRawSql("grant insert on "+table->first+" to "+groupName+";"))
				return false;
			
			// grant access to the sequence for this table
			if (!PerformRawSql("grant all on sequence "+table->first+"_id_seq to "+groupName+";"))
				return false;
			
			count = table->second.mInsertRules.size();
			
			#ifdef QRAP_SERVER_EDITION
			if (!count)
			{
				// grant the user simple insert access
				if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values "
					"('"+groupName+"','"+table->first+"','insert');"))
					return false;
			}
			#endif
			
			// run through the insert rules
			for (i=0;i<count;i++)
			{
				// create the rule
				if (!CreateRule("insert", table->first, table->second.mInsertRules[i], groupName))
					return false;
			}
		}
		if (table->second.mDelete)
		{
			if (!PerformRawSql("grant delete on "+table->first+" to "+groupName+";"))
				return false;
			
			count = table->second.mDeleteRules.size();
			
			#ifdef QRAP_SERVER_EDITION
			if (!count)
			{
				// grant the user simple insert access
				if (!PerformRawSql("insert into qrap_groups (groupname,tablename,cmd) values "
					"('"+groupName+"','"+table->first+"','delete');"))
					return false;
			}
			#endif
			
			// run through the delete rules
			for (i=0;i<count;i++)
			{
				// create the rule
				if (!CreateRule("delete", table->first, table->second.mDeleteRules[i], groupName))
					return false;
			}
		}
	}
	
	// grant access to the appropriate qrap tables
	if (!PerformRawSql("grant select, insert, update, delete on qrap_config, qrap_regions, qrap_undo to group "+groupName+";"))
	{
		QRAP_ERROR("Unable to grant group "+groupName+" access to system tables.");
		return false;
	}
	
	// grant sequence access to the qrap tables
	if (!PerformRawSql("grant all on sequence qrap_config_id_seq, qrap_regions_id_seq, qrap_undo_id_seq to group "+groupName+";"))
		return false;
	
	return true;
}


//**********************************************************************************
bool cDatabase::RemoveGroup (const string& groupName)
{
	// search for views in the pg_views table
	if (!PerformRawSql("select viewname from pg_views where viewname like '%_"+groupName+"';"))
		return false;

	int i, count = mLastResult.size();
	
	// drop the views one-by-one
	for (i=0;i<count;i++)
	{
		if (!RemoveView(mLastResult[i]["viewname"].c_str()))
			return false;
	}
	
	// search for rules in the pg_rules table
	if (!PerformRawSql("select tablename,rulename from pg_rules where rulename like '%_"+groupName+"';"))
		return false;
	
	count = mLastResult.size();
	
	for (i=0;i<count;i++)
	{
		if (!RemoveRule(mLastResult[i]["tablename"].c_str(), mLastResult[i]["rulename"].c_str()))
			return false;
	}
	
	// revoke all the privileges for this group
	for (DbStructureIterator table=mStructure.begin(); table != mStructure.end(); table++)
	{
		if (!PerformRawSql("revoke all privileges on table "+table->first+" from group "+groupName+";"))
			return false;
		// revoke privileges to access the sequences
		if (!PerformRawSql("revoke all privileges on sequence "+table->first+"_id_seq from group "+groupName+";"))
			return false;
	}
	
	#ifdef QRAP_SERVER_EDITION
	if (!PerformRawSql("revoke all privileges on table qrap_users from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on sequence qrap_users_id_seq from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on table qrap_undo from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on sequence qrap_undo_id_seq from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on table qrap_regions from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on sequence qrap_regions_id_seq from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on table qrap_config from group "+groupName+";"))
		return false;
	if (!PerformRawSql("revoke all privileges on sequence qrap_config_id_seq from group "+groupName+";"))
		return false;
	#endif
	
	// drop the group
	if (!PerformRawSql("drop group "+groupName+";"))
		return false;
	
	#ifdef QRAP_SERVER_EDITION
	// remove all the group's privileges from the qrap_groups table
	if (!PerformRawSql("delete from qrap_groups where groupname='"+groupName+"';"))
		return false;
	#endif
	
	return true;
}

//********************************************************************************
bool cDatabase::CreateRule (const string& on, const string& tableName,
	const SqlRule& rule, const string& groupName)
{
	string ruleName;
	bool   found = false;
	
	// generate a random 4-digit number representing the rule for this table
	while (!found)
	{
		ruleName = tableName+"_";
		if (!PerformRawSql("select trunc(random()*10000.0);"))
			return false;
		ruleName += mLastResult[0][0].c_str();
		ruleName += "_"+on+"_"+groupName;
		// check if the rule exists
		if (!PerformRawSql("select rulename from pg_rules where rulename='"+ruleName+"';"))
			return false;
		// if the rule does not exist
		if (mLastResult.size() == 0)
			found = true;
	}
	
	// grant this group access to perform this particular action
	if (!PerformRawSql("grant "+on+" on "+tableName+" to group "+groupName+";"))
		return false;
	
	if (on == "select")
	{
		// create a view instead
		return CreateView(tableName, rule.mLimiter, groupName);
	} else
	if ((rule.mLimiter.length() > 0) || (rule.mDo.length() > 0))
	{
		if (!PerformRawSql("create rule "+ruleName+" as on "+on+
			" to "+tableName+" where ((select groname from pg_group where (select usesysid from pg_user where usename=current_user) = any (pg_group.grolist)) = '"+
			groupName+"' "+
			((rule.mLimiter.length() > 0) ? string("and ("+rule.mLimiter+")") : string(""))+
			") do "+rule.mDo+";"))
			return false;
	}
	
	#ifdef QRAP_SERVER_EDITION
	// add the rule to the qrap_groups table if it doesn't already exist
	if (!PerformRawSql("insert into qrap_groups (groupname,rulename,tablename,cmd,limiter,docmd) values ('"+
		groupName+"','"+ruleName+"','"+tableName+"','"+on+"',"+StrQuote(rule.mLimiter)+","+
		StrQuote(rule.mDo)+");"))
		return false;
	#endif
	
	return true;
}

//************************************************************************************
bool cDatabase::RemoveRule (const string& ruleName, const string& tableName, const string& groupName)
{
	bool   isPrivilege = (ruleName.substr(0, 4) == "priv") ? true : false, isView = false;
	string privType;
	
	if (isPrivilege)
	{
		privType = ruleName.substr(10);
		// revoke the privilege
		/*if (!PerformRawSql("revoke "+privType+" on table "+tableName+" from group "+groupName+";"))
			return false;*/
	} else
	// if the rule is a view
	if (ruleName.find("_select_") < ruleName.length())
		isView = true;
	
	#ifdef QRAP_SERVER_EDITION
	// if it's a privilege, delete it
	if (isPrivilege)
	{
		if (!PerformRawSql("delete from qrap_groups where tablename='"+tableName+"' and "
			"groupname='"+groupName+"' and rulename is null and cmd='"+privType+"';"))
			return false;
	}
	// remove the rule from the qrap_groups table
	if (!PerformRawSql("delete from qrap_groups where rulename='"+ruleName+"';"))
		return false;
	#endif
	
	if (isPrivilege)
		return true;
	
	if (isView)
		return RemoveView(tableName+"_"+groupName);

	// drop the rule
	return PerformRawSql("drop rule "+ruleName+" on "+tableName+";");
}

//***********************************************************************************
bool cDatabase::CreateView (const string& tableName, const string& limiter, const string& groupName)
{
	string viewName = tableName+"_"+groupName;
	
	if (!PerformRawSql("create view "+viewName+" as select * from "+tableName+
		((limiter.length() > 0) ? string(" where "+limiter) : string(""))+";"))
		return false;
	
	// grant the group access to the view
	if (!PerformRawSql("grant select on "+viewName+" to "+groupName+";"))
		return false;
	
	#ifdef QRAP_SERVER_EDITION
	if (!PerformRawSql("insert into qrap_groups (groupname,rulename,tablename,cmd,limiter,docmd) values ('"+
		groupName+"','"+tableName+"_select_"+groupName+"','"+tableName+"','select',"+StrQuote(limiter)+",null);"))
		return false;
	#endif

	return true;
}

//*************************************************************
bool cDatabase::RemoveView (const string& viewName)
{
	return PerformRawSql("drop view "+viewName+";");
}

//**********************************************************************
string cDatabase::GetSetting (const string& name)
{
	if (!PerformRawSql("select value from qrap_config where name='"+name+"' and type='local' and username='"+mUsername+"';"))
		return string("");
	
	if (mLastResult.size() == 0)
		return string("");
	
	return string(mLastResult[0]["value"].c_str());
	
//	if (mSettings.count(name) > 0)
//		return mSettings[name];
	
	return string("");
}

//****************************************************************************
bool cDatabase::SetSetting (const string& name, const string& value)
{
	// check if the setting exists for this user
	if (!PerformRawSql("select id from qrap_config where name='"+name+"' and type='local' and username='"+mUsername+"';"))
		return false;
	
	// if the entry should be inserted
	if (mLastResult.size() == 0)
	{
		if (!PerformRawSql("insert into qrap_config (lastmodified,type,username,name,value) values (now(),'local','"+
			mUsername+"','"+name+"',"+StrQuote(value)+");"))
			return false;
	} else
	// if the entry should be updated
	{
		if (!PerformRawSql("update qrap_config set lastmodified=now(),value="+StrQuote(value)+
			" where type='local' and username='"+mUsername+"' and name='"+name+"';"))
			return false;
	}
	
	// set the setting in memory
	mSettings[name] = value;
	
	return true;
}

//***********************************************************************************
cDatabase::FieldDataType cDatabase::GetFieldDataType (const string& tableName, const string& fieldName)
{
	string type = ExtractKeyword(mStructure[tableName].mFields[fieldName].mType);

	if (mStructure[tableName].mFields[fieldName].mPostGis)
	{
		// only support POINT, LINESTRING and POLYGON types at the moment
		if (type == "point")
			return cDatabase::dtPostGisPoint;
		if (type == "linestring")
			return cDatabase::dtPostGisLineString;
		if (type == "polygon")
			return cDatabase::dtPostGisPolygon;
		return cDatabase::dtString;
	}
	
	if ((type == "integer") || (type == "smallint") || (type == "bigint") || (type == "serial") || (type == "bigserial"))
		return cDatabase::dtInteger;
	if ((type == "real") || (type == "double") || (type == "numeric"))
		return cDatabase::dtReal;
	if (type == "boolean")
		return cDatabase::dtBoolean;
	if (type == "date")
		return cDatabase::dtDate;
	if (type == "timestamp")
		return cDatabase::dtDateTime;
	
	// default, return a string type
	return cDatabase::dtString;
}

//*******************************************************************************
bool cDatabase::GetFieldUiParams (const string& tableName, const string& fieldName, StringIntArray& params)
{
	msAlertCode = acOk;

	// check that the table and field exist
	if (mStructure.count(tableName) == 0)
	{
		msAlertCode = acDbTableNotExist;
		return false;
	}
	if (mStructure[tableName].mFields.count(fieldName) == 0)
	{
		msAlertCode = acDbFieldNotExist;
		return false;
	}
	
	string ui(mStructure[tableName].mFields[fieldName].mUi);
	
	// clear the output parameters
	params.clear();
	
	// is this a drop-down box?
	if (ui.substr(0, 8) == "dropdown")
	{
		string      paramStr, ddType;
		StringArray ddParams;
		int         startPos, endPos, i, len;
		
		// search from the front for the first opening brace "("
		startPos = ui.find('(');
		// search from the back for the last closing brace ")"
		endPos = ui.rfind(')');
		
		// extract the parameters for the dropdown() function
		paramStr = ui.substr(startPos+1, endPos-startPos-1);
		// extract the first parameter from the string to check what type of combo it is
		ddType = ExtractKeyword(paramStr, 0);
		// is this a fixed drop-down combo?
		if (ddType == "fixed")
		{
			// extract the string containing the array
			startPos = paramStr.find('{', ddType.length()+1);
			endPos = paramStr.rfind('}');
			paramStr = paramStr.substr(startPos, endPos-startPos+1);
			// extract the parameters and store them in the output array
			if (!ParseSqlArray(paramStr, ddParams))
			{
				msAlertCode = acParse;
				return false; // parsing error
			}
			
			len = ddParams.size();
			// otherwise push them one-by-one onto the output array
			for (i=0;i<len;i++)
			{
				params[i] = ddParams[i];
			}
		} else
		if (ddType == "fromtable")
		{
			string      table, field;
			StringArray cols;
			int         id;
			
			// skip past the comma and any whitespace
			i = FindNonWhitespace(paramStr, ddType.length()+1);
			// extract the table's name
			table = ExtractKeyword(paramStr, i);
			// skip past the comma again and any whitespace
			i = FindNonWhitespace(paramStr, i+table.length()+1);
			// extract the field's name to use
			field = ExtractKeyword(paramStr, i);
			
			// select the id along with the field
			cols.push_back("id");
			cols.push_back(field);
			
//			cout <<  "now attempt to select the field from the given table=";
//			cout << table <<"	field=" << field << endl;
			if (!Select("id,"+field, table, ""))
			{
				msAlertCode = acDbNotAllowed;
				// user probably doesn't have access to this field
				return false;
			}
			
			// run through the results and populate the output parameters list
			len = mLastResult.size();
//			cout << "success: len=" << len << endl;
			for (i=0;i<len;i++)
			{
				id = atoi(mLastResult[i]["id"].c_str());
				// push it onto the output parameters list
				params[id] = string(mLastResult[i][field].c_str());
			}
		} 
		else
		{
			msAlertCode = acParse;
			return false; // parsing error
		}
	}
	
	return true;
}

//**************************************************************************
cDatabase::FieldUiType cDatabase::GetFieldUiType (const string& tableName, const string& fieldName)
{
	string ui(mStructure[tableName].mFields[fieldName].mUi);
	
	
	if (ui.length() > 0)
	{
		if (ui.length() > 8)
		{
			// if this field requires a drop-down menu
			if (ui.substr(0, 8) == "dropdown")
			{
				int    startPos, endPos;
				string paramStr, ddType;
			
				// search from the front for the first opening brace "("
				startPos = ui.find('(');
				// search from the back for the last closing brace ")"
				endPos = ui.rfind(')');
		
				// extract the parameters for the dropdown() function
				paramStr = ui.substr(startPos+1, endPos-startPos-1);
				// extract the first parameter from the string to check what type of combo it is
				ddType = ExtractKeyword(paramStr, 0);
			
				if (ddType == "fixed")
					return cDatabase::utDropdownFixed;
				if (ddType == "fromtable")
					return cDatabase::utDropdownTable;
				// otherwise it will just return utNormal
			}
		}
	}
	
	return cDatabase::utNormal;
}

//******************************************************************************
DbFieldMap::const_iterator cDatabase::GetFieldWithOrder (const string& tableName, int order)
{
	DbFieldMap::const_iterator it;
	
	// run through the fields in the table, hunting for the one with the same order as the given one
	for (it=mStructure[tableName].mFields.begin(); it != mStructure[tableName].mFields.end(); it++)
	{
		if (it->second.mOrder == order)
			return it;
	}
	
	return mStructure[tableName].mFields.begin();
}

//***********************************************************************************************
short int cDatabase::getsetMachineID()
{
	cout << "In cDataBase::getsetMachineID()" << endl;
	char* HostID;
	 
	bool found = false;
//	bool HostIDnew = false;
	HostID = getenv("QRAPINST");
	if (HostID==NULL)
		HostID = getenv("HOSTID");
	if (HostID==NULL)
		HostID = getenv("HOSTNAME");
	if (HostID==NULL)
		HostID = getenv("USERNAME");
	if (HostID==NULL)
		HostID = getenv("UID");
	if (HostID==NULL)
	{
		HostID = new char[10];
		strcpy(HostID,"LOCAL");
//		HostIDnew = true;
	}
	string query = "select id from machine where qrapinst='";
	query += HostID;
	query += "';"; 
	
//	query = "select max(machineid) from sourcefiles;";

	cout << query << endl;
	pqxx::result r;
	if (!gDb.PerformRawSql(query))
	{
		cout << "Could not get Machine ID"<< endl;
	}
	else
	{
		gDb.GetLastResult(r);
		if (r.size() >0)
		{
			found = true;
			globalMachineID = (short int)atoi(r[0]["id"].c_str());
//			globalMachineID = (short int)atoi(r[0]["machineid"].c_str());
		}
	}
	
	// if ID does not exist set one
	if (!found)
	{
		cout << "NOT found in cDataBase::getsetMachineID()" << endl;
		StringMap values;
		string tableName = "machine";
		query ="'";
		query += HostID;
		query +="'";
		values["qrapinst"]=query;	
		globalMachineID = (short int) Insert(values, tableName);
	}
	cout << "Verlaat cDataBase::getsetMachineID():  globalMachineID=" << globalMachineID << endl;
//	if (HostID!=NULL) delete [] HostID;
	return globalMachineID;
}



//******************************************************************************
string cDatabase::GetUiForeignField (const string& tableName, const string& fieldName)
{
	if (!mStructure[tableName].mFields[fieldName].mIsForeign)
		return string("");
	
	string      ui(mStructure[tableName].mFields[fieldName].mUi);
	string      paramStr, ddType;
	StringArray ddParams;
	int         startPos, endPos, i;
	
	// search from the front for the first opening brace "("
	startPos = ui.find('(');
	// search from the back for the last closing brace ")"
	endPos = ui.rfind(')');
	
	// extract the parameters for the dropdown() function
	paramStr = ui.substr(startPos+1, endPos-startPos-1);
	// extract the first parameter from the string to check what type of combo it is
	ddType = ExtractKeyword(paramStr, 0);
	
	string table, field;
	
	// skip past the comma and any whitespace
	i = FindNonWhitespace(paramStr, ddType.length()+1);
	// extract the table's name
	table = ExtractKeyword(paramStr, i);
	// skip past the comma again and any whitespace
	i = FindNonWhitespace(paramStr, i+table.length()+1);
	// extract the field's name to use
	field = ExtractKeyword(paramStr, i);
	
	return field;
}

//*******************************************************************
DbStructure::const_iterator cDatabase::GetTableWithOrder (int order)
{
	for (DbStructure::const_iterator table=mStructure.begin(); table != mStructure.end(); table++)
	{
		if (table->second.mCreateOrder == order)
			return table;
	}
	
	return mStructure.begin();
}


//***********************************************************************
bool cDatabase::GetUsers (StringArray& users)
{
	if (!PerformRawSql("select username from qrap_users;"))
		return false;
	
	users.clear();
	int i, count = mLastResult.size();
	
	for (i=0;i<count;i++)
		users.push_back(mLastResult[i]["username"].c_str());
	
	return true;
}

//*********************************************************************
bool cDatabase::GetGroups (StringArray& groups)
{
	if (!PerformRawSql("select distinct(groupname) from qrap_groups;"))
		return false;
	
	groups.clear();
	int i, count = mLastResult.size();
	
	for (i=0;i<count;i++)
		groups.push_back(mLastResult[i]["groupname"].c_str());
	
	return true;
}

//*******************************************************************
bool cDatabase::ExportToCsv (const std::string& tableName, const std::string& fileName)
{
	string     query = "select ", actTable;
	int        i = 0, fieldCount;
	DbFieldMap fields;
	
	actTable = tableName+"_"+mUserGroup;
	if (!ViewExists(actTable))
		actTable = tableName;
	
	for (DbFieldMap::iterator field=mStructure[tableName].mFields.begin();
		field != mStructure[tableName].mFields.end(); field++)
	{
		if (field->second.mVisible)
		{
			#ifdef QRAP_SERVER_EDITION
			if (field->second.mClientOnly)
				continue;
			#else
			if (field->second.mServerOnly)
				continue;
			#endif
			fields[field->first] = field->second;
		}
	}
	
	fieldCount = fields.size();
	
	// append the fields' names for this table
	for (DbFieldMap::iterator field=fields.begin(); field != fields.end(); field++)
	{
		if (field->second.mPostGis)
			query += "ST_AsEWKT("+field->first+") as "+field->first;
		else
			query += field->first;
		i++;
		if (i < fieldCount)
			query += ",";
	}
	
	query += " from "+actTable+";";
	
	return ExportCmdToCsv(query, fileName);
}

//*************************************************************************
bool cDatabase::ExportCmdToCsv (const string& cmd, const string& fileName)
{
	FILE*       fp;
	string      buf, val;
	double	mLat,mLon;
	StringArray headers;
	int         i, j, colCount, count;
	
	
	msAlertCode = acOk;
	
	if ((fp = fopen(fileName.c_str(), "wt")) == NULL)
	{
		QRAP_ERROR_CODE("Unable to write to CSV file \""+fileName+"\".", acFileOpenWrite);
		return false;
	}
	
	// execute the command
	if (!PerformRawSql(cmd))
	{
		QRAP_ERROR("Unable to export to CSV file.");
		fclose(fp);
		return false;
	}
	
	if (mLastResult.size() == 0)
	{
		QRAP_WARN("Result of CSV export command is empty.");
		fclose(fp);
		return true;
	}
	
	// get the headers for this table
	i = 0;
	pqxx::const_tuple_iterator field=mLastResult[0].begin();
	
	while (field != mLastResult[0].end())
	{
		if ((field->name()=="location")||(field->name()=="position"))
		{
			headers.push_back("latitude");
			buf += headers[i];
			buf += ",";
			i++;
			headers.push_back("longitude");
		}
		else
			headers.push_back(field->name());

		buf += headers[i];
		i++;
		field++;
		if (field != mLastResult[0].end())
			buf += ",";
	}
	
	// write the headers to the file
	fputs(string(buf+"\n").c_str(), fp);
	
	// run through the resulting fields
	string location;
	QString Lat,Lon;
	
	DegreeFormat locationFormat;
	string setting = gDb.GetSetting("location");
	if (setting=="DD:MM:SS")
		locationFormat = dfDegMinSec;
	else if (setting=="DD:MM.mm")
		locationFormat = dfDegMin;
	else 
		locationFormat = dfDeg;

	count = mLastResult.size();
	colCount = headers.size();
	int pos1,pos2;
	for (i=0;i<count;i++)
	{
		// clear the output buffer
		buf = "";
		// add all the fields' data
		for (j=0;j<colCount;j++)
		{
			// get the value for this field
			val = mLastResult[i][headers[j]].c_str();
			if ((headers[j]=="location")||(headers[j]=="position"))
			{
				pos1 = val.find("(");
				pos2 = val.find(" ");
				location= val.substr(pos1,pos2-1);
				cout << location << "	" << endl;
				mLat = GetDecimalDegrees(location,dfDeg,false, true);
				Lat = MakeStringFromDecDegrees (mLat, locationFormat, true);
				buf += StrQuote(Lat.toStdString(), '"');
				buf += ",";
				j++;
				location= val.substr(pos2+1,val.length()-1);
				cout << location << endl;
				mLon = GetDecimalDegrees(location,dfDeg,false, false);
				Lon = MakeStringFromDecDegrees (mLon, locationFormat, false);
				buf += StrQuote(Lon.toStdString(), '"');
			}
			else
			{		
				if (IsNumeric(val))
					buf += val;
				else
					buf += StrQuote(val, '"');
			}
			
			if ((j+1) < colCount)
				buf += ",";
		}
		
		buf += "\n";
		// write the output
		fputs(buf.c_str(), fp);
	}
	
	fclose(fp);
	
	return true;
}

//*************************************************************************************
bool cDatabase::ImportFromCsv (const string& fileName, const string& tableName, IntArray& failed,
	DegreeFormat format)
{
	FILE*       fp;
	char        buf[cCsvLineBufferSize];
	StringArray lineArr, headers, multiDim;
	StringMap   vals;
	unsigned         line = 1, i, colCount;
	string      fieldName, temp, foreignField;
	
	
	msAlertCode = acOk;
	failed.clear();
	
	if (mStructure.count(tableName) < 1)
	{
		QRAP_ERROR_CODE("Table \""+tableName+"\" does not exist.", acDbTableNotExist);
		return false;
	}
	
	if ((fp = fopen(fileName.c_str(), "rt")) == NULL)
	{
		QRAP_ERROR_CODE("Unable to read from CSV file \""+fileName+"\".", acFileOpenRead);
		return false;
	}
	
	if (feof(fp))
	{
		QRAP_ERROR_CODE("CSV file \""+fileName+"\" is empty.", acFileOpenRead);
		fclose(fp);
		return false;
	}
	
	while (!feof(fp))
	{
		// clear the buffer
		buf[0] = '\0';
		// read a line of data from the file
		fgets(buf, cCsvLineBufferSize, fp);
		if (strlen(buf) == 0)
			break;
		// strip the newline
		buf[strlen(buf)-1] = '\0';
		if (strlen(buf) < 1) break;
		
		// extract the data
		if (!SplitString(buf, ',', lineArr))
		{
			char *Temp = new char[33];
			gcvt(line, 8, Temp);
			string error = "Parsing error in CSV file \""+fileName+"\" on line ";
			error +=Temp;
			QRAP_ERROR_CODE(error, acParse);
			fclose(fp);
			return false;
		}
		
		// if we're on the first line, these must be the headers
		if (line == 1)
		{
			colCount = lineArr.size();
			// run through the headers, checking that they correspond
			for (i=0;i<colCount;i++)
			{
				headers.push_back(StrToLower(lineArr[i]));
				// check that the field exists
				if (mStructure[tableName].mFields.count(headers[i].substr(0, lineArr[i].find('.'))) < 1)
				{
					QRAP_ERROR_CODE("Field \""+(headers[i].substr(0, lineArr[i].find('.')))+"\" does not exist in table \""+tableName+"\".", acDbFieldNotExist);
					fclose(fp);
					return false;
				}
			}
		} else
		{
			// check that we have at least [colCount] lines from which we can extract data
			if (lineArr.size() < colCount)
			{
				char *Temp = new char[33];
				gcvt(line, 8, Temp);
				string error = "Too few fields found on line ";
				error +=Temp;
				error +=" in file \""+fileName+"\".";
				QRAP_ERROR(error);
				fclose(fp);
				return false;
			}
			
			// add the values
			for (i=0;i<colCount;i++)
			{
				// if this is a multi-dimensional field
				if (headers[i].find('.') < headers[i].length())
				{
					multiDim.push_back(lineArr[i]);
					fieldName = headers[i].substr(0, headers[i].find('.'));
					// if it's a PostGIS point type, we're expecting 2 parameters
					if (mStructure[tableName].mFields[fieldName].mType == "POINT")
					{
						if (multiDim.size() >= 2)
						{
							// add the points as a PostGIS point
							temp = ExtractDecimalDegrees(multiDim[1], format, false);
							// if there was an error
							if (temp.length() == 0)
							{
								char *Temp = new char[33];
								gcvt(line, 8, Temp);
								string error = "Invalid point format specified on line ";
								error +=Temp;
								error +=" of CSV file. Using 0.0 instead.";
								QRAP_WARN(error);
								temp = "0.0";
							}
							vals[fieldName] = "POINT("+temp+" ";
							temp = ExtractDecimalDegrees(multiDim[0], format, false);
							if (temp.length() == 0)
							{
								
								char *Temp = new char[33];
								gcvt(line, 8, Temp);
								string error = "Invalid point format specified on line ";
								error +=Temp;
								error +=" of CSV file. Using 0.0 instead.";
								QRAP_WARN(error);
								temp = "0.0";
							}
							vals[fieldName] += temp+")";
							multiDim.clear();
						}
					}
				} else
				{
					// handle empty values
					if (lineArr[i].length() == 0)
						lineArr[i] = "NULL";
					else
					// if this is a foreign key link and there isn't a numeric value in the field
					if (mStructure[tableName].mFields[headers[i]].mIsForeign)
					{
						if (IsNumeric(lineArr[i]))
						{
							vals[headers[i]] = lineArr[i];
						} else
						{
							foreignField = GetUiForeignField(tableName, headers[i]);
							if (foreignField.length() == 0)
								foreignField = "id";
							vals[headers[i]] = "(select id from "+mStructure[tableName].mFields[headers[i]].mForeignTable+
								" where "+mStructure[tableName].mFields[headers[i]].mForeignTable+"."+foreignField+"=";
							vals[headers[i]] += StrQuote(lineArr[i]);
							vals[headers[i]] += ")";
						}
					}
					else if (!IsNumeric(lineArr[i]) && (lineArr[i].substr(0, 6) != "POINT("))
						vals[headers[i]] = StrQuote(lineArr[i]);
					else
						vals[headers[i]] = lineArr[i];
				}
			}
			
			// insert the entry
			if (Insert(vals, tableName) == -1)
			{
				// add this entry to the list of failed insertions
				failed.push_back(line);
			}
		}

		line++;
	}
	
	fclose(fp);
	
	return true;
}


//************************************************************************************
/*bool cDatabase::ImportFromCsvSaps (const string& fileName, DegreeFormat format, IntArray& failed)
{
	FILE*          fp;
	char           buf[cCsvLineBufferSize];
	int            curLine = 0, siteId, i;
	StringArray    vals;
	StringIntArray structureTypes, fencingTypes, powerTypes;
	StringMap      insVals;
	bool           fail, getTown = true;
	string         townName;
	
	if ((fp = fopen(fileName.c_str(), "rt")) == NULL)
	{
		QRAP_ERROR_CODE("Cannot open \""+fileName+"\" for reading.", acFileOpenRead);
		return false;
	}
	
	// skip the first 5 lines
	while (curLine < 5)
	{
		if (feof(fp))
		{
			QRAP_ERROR_CODE("Invalid format of input file.", acParse);
			fclose(fp);
			return false;
		}
		fgets(buf, cCsvLineBufferSize, fp);
		curLine++;
	}
	
	// get the structure types
	GetFieldUiParams("sitedescription", "structuretype", structureTypes);
	// get the fencing types
	GetFieldUiParams("sitedescription", "fencing", fencingTypes);
	// get the power supply types
	GetFieldUiParams("sitedescription", "powersupply", powerTypes);
	
	while (!feof(fp))
	{
		fgets(buf, cCsvLineBufferSize, fp);
		curLine++;
		// attempt to split the string
		if (!SplitString(buf, ',', vals))
		{
			QRAP_ERROR_CODE("Parsing error on line "+IntToString(curLine)+" of file \""+fileName+"\".", acParse);
			fclose(fp);
			return false;
		}
		// must have at least 7 columns
		if (vals.size() < 7)
		{
			QRAP_ERROR_CODE("Parsing error on line "+IntToString(curLine)+" of file \""+fileName+"\".", acParse);
			fclose(fp);
			return false;
		}
		
		if (getTown)
		{
			townName = StrUnquote(vals[0]);
			getTown = false;
			continue;
		}
		
		// we're at the end of a chunk of data
		if (vals[0] == "0")
		{
			getTown = true;
			continue;
		}
		
		fail = false;
		
		insVals.clear();
		// construct the site parameters
		insVals["sitename"] = StrQuote(StrUnquote(vals[0]));
		try
		{
			insVals["location"] = "POINT("+ExtractDecimalDegrees(StrUnquote(vals[2]), format)+
				" "+ExtractDecimalDegrees(StrUnquote(vals[1]), format)+")";
		} catch (cAlert& e)
		{
			QRAP_WARN(e.GetMsg());
			fail = true;
		}
		
		// skip this entry
		if (fail)
		{
			failed.push_back(curLine);
			continue;
		}
		
		// insert the site
		if ((siteId = Insert(insVals, "site")) == -1)
		{
			QRAP_WARN("Unable to insert entry from line "+IntToString(curLine)+" of file \""+fileName+"\". Skipping entry.");
			continue;
		}
		
		insVals.clear();
		// insert the site contact
		insVals["siteid"] = IntToString(siteId);
		insVals["ownername"] = StrQuote(StrUnquote(vals[3]));
		if (Insert(insVals, "sitecontacts") == -1)
		{
			QRAP_WARN("Unable to insert site contact details from line "+IntToString(curLine)+" of file \""+fileName+"\". Skipping entry.");				
			continue;
		}

		insVals.clear();
		// insert the site description
		insVals["siteid"] = IntToString(siteId);
		insVals["physicaladdress"] = StrQuote(townName);
		// get the power supply type
		i = FuzzyStringSetMatch(StrUnquote(vals[4]), powerTypes);
		if (i == -1)
			insVals["powersupply"] = "'Not certain'";
		else
			insVals["powersupply"] = StrQuote(powerTypes[i]);
			
		// get the structure type
		i = FuzzyStringSetMatch(StrUnquote(vals[5]), structureTypes);
		if (i == -1)
			insVals["structuretype"] = "'Not certain'";
		else
			insVals["structuretype"] = StrQuote(structureTypes[i]);
		
		// get the fencing type
		i = FuzzyStringSetMatch(StrUnquote(vals[6]), fencingTypes);
		if (i == -1)
			insVals["fencing"] = "'Not certain'";
		else
			insVals["fencing"] = StrQuote(fencingTypes[i]);
		
		// get the structure height
		insVals["structureheight"] = FuzzyValueExtract(StrUnquote(vals[5]), "M", false);
		if (insVals["structureheight"].length() == 0)
			insVals["structureheight"] = "null";
		
		// try to insert the site description
		if (Insert(insVals, "sitedescription") == -1)
		{
			QRAP_WARN("Unable to insert site description from line "+IntToString(curLine)+" of file \""+fileName+"\".");
			continue;
		}
	}
	
	fclose(fp);
	
	return true;
}

*/
//********************************************************************
/*bool cDatabase::LogTransaction (const string& op, const StringMap& values, 
				const string& tableName, const string& where)
{
	string      undo, whereStr = ((where.length() > 0) ? " where "+where : "");
	StringArray cols;
	string      valStr;
	int         i, count;
	
	if (op == "insert")
	{
		// the reverse of an insert transaction is a delete
		// ... assume that the entry has just been inserted
		if (!PerformRawSql("select currval('"+tableName+"_id_seq');"))
		{
			QRAP_WARN("Unable to select current value for "+tableName+".");
			return false;
		}
		
		undo = "delete from "+tableName+" where id="+string(mLastResult[0][0].c_str())+";";
	} else
	if (op == "update")
	{
		// assume that the entries are about to be updated...
		
		// get the columns' names
		for (StringMap::const_iterator field=values.begin(); field != values.end(); field++)
		{
			cols.push_back(field->first);
		}
		
		// no point
		if (!cols.size()) return true;
		
		if (StringArrayPos(
		
		// get the current values based on the given where clause
		if (!PerformRawSql("select "+colStr+" from "+tableName+whereStr+";"))
		{
			QRAP_WARN("Unable to log transaction.");
			return false;
		}
		
		// run through the entries, adding their reverse update commands to the undo command
		count = mLastResult.size();
		// no need to log this if there's nothing to update
		if (!count) return true;
		for (i=0;i<count;i++)
		{
			// build the list of column/value pairs
			for (pqxx::result::const_fielditerator f=mLastResult[i].begin();
				f != mLastResult[i].end(); f++)
			{
				if (string(f->name()) != "id")
				{
					valStr += string(f->name())+"=";
					switch (GetFieldDataType(f->name()))
					{
					case dtString:
						valStr
					}
				}
			}
			undo += "update "+tableName+" set "+valStr
		}
	}
	
	return true;
}*/

//****************************************************************************************************
/*bool cDatabase::LogTransaction (const string& op, const StringMap& values, 
				const string& tableName, const string& where)
{
	// save the last transaction result
	pqxx::result lastResult = mLastResult;
	StringArray  cols, vals;
	string       colStr, valStr, revEntry;
	int          i, j, count, colCount;
	
	for (StringMap::const_iterator it=values.begin(); it != values.end(); it++)
	{
		cols.push_back(it->first);
		vals.push_back(it->second);
	}
	
	// convert the column and value arrays to strings
	colStr = FormatSqlArray(cols);
	valStr = FormatSqlArray(vals);
	
	// figure out what the reverse entry should be
	if (op == "insert")
	{
		// get the item's ID from the table
		if (!PerformRawSql("select currval('"+tableName+"_id_seq');"))
		{
			#ifdef QRAP_DEBUG
			Log("Unable to select item ID from database.");
			#endif
			return -1;
		}
		
		revEntry = "delete from "+tableName+" where id="+string(mLastResult[0][0].c_str())+";";
	} else
	if (op == "update")
	{
		if (
		// select all the affected items
		if (!Select(cols, tableName, where))
		{
			#ifdef QRAP_DEBUG
			Log("Unable to select items for update operation reversal.");
			#endif
			return false;
		}
		
		// run through the selection, creating reverse update entries
		count = mLastResult.size();
		colCount = cols.size();
		for (i=0;i<count;i++)
		{
			revEntry += "update "+tableName+" set ";
			// run through the columns and values, setting them to their old values
			for (
		}
	}
	
	// restore it
	mLastResult = lastResult;
	return true;
}*/

/*bool cDatabase::ImportFromCsv (const string& fileName, StringArrayMap& existing)
{
	FILE*          fp;
	char           buf[cCsvLineBufferSize];
	StringArray    vals, tables;
	StringArrayMap fieldNames;
	StringMap      insertVals;
	int            curLine;
	int            curTable;
	int            i, j, count, tableCount, k;
	map<string, struct{ int start, stop; }> tableRanges;
	
	msAlertCode = acOk;
	
	// clear the existing entries, just in case
	existing.clear();
	
	fp = fopen(fileName.c_str(), "rt");
	if (!fp)
	{
		#ifdef QRAP_DEBUG
		Log("Unable to open file "+fileName+" for CSV importing.");
		#endif
		msAlertCode = ecFileOpenRead;
		return false;
	}
	
	// if the file's empty
	if (feof(fp))
	{
		#ifdef QRAP_DEBUG
		Log("File "+fileName+" is empty. Cannot import it.");
		#endif
		fclose(fp);
		msAlertCode = ecFileOpenRead;
		return false;
	}
	
	// read a line of data
	fgets(buf, cCsvLineBufferSize, fp);
	// strip the newline
	buf[strlen(buf)-1] = '\0';
	
	if (!SplitString(buf, ',', vals))
	{
		#ifdef QRAP_DEBUG
		Log("Parsing error in CSV file "+fileName+" on line 1.");
		#endif
		msAlertCode = ecParse;
		fclose(fp);
		return false;
	}
	
	count = vals.size();
	
	// line 1 contains the tables' names just for display purposes...
	// add all the tables' names to the list
	for (i=0;i<count;i++)
	{
		if (vals[i].length() > 0)
		{
			// check that the table exists
			if (mStructure.count(vals[i]) == 0)
			{
				#ifdef QRAP_DEBUG
				Log("Unknown table \""+vals[i]+"\" on line 1, field "+IntToString(i+1)+" in CSV file "+fileName+".");
				#endif
				msAlertCode = ecParse;
				fclose(fp);
				return false;
			}
			tables.push_back(vals[i]);
			
			// check if the temporary table exists
			if (TableExists(vals[i]+"_temp"))
			{
				// if the temporary table already exists, make sure we empty it thoroughly
				if (!PerformRawSql("delete from "+vals[i]+"_temp;"))
				{
					#ifdef QRAP_DEBUG
					Log("Unable to clear temporary table "+vals[i]+"_temp.");
					#endif
					msAlertCode = ecInternalError;
					fclose(fp);
					return false;
				}
			} else
			{
				// otherwise create it as a temporary table (and all its subsidiary tables)
				if (!PerformRawSql("create temporary table "+vals[i]+"_temp (like "+vals[i]+" excluding constraints);")
				{
					#ifdef QRAP_DEBUG
					Log("Unable to create temporary table "+vals[i]+"_temp.");
					#endif
					msAlertCode = ecInternalError;
					fclose(fp);
					return false;
				}
			}
		}
	}
	
	tableCount = tables.size();
	
	// if the file's missing a fields line
	if (feof(fp))
	{
		#ifdef QRAP_DEBUG
		Log("File "+fileName+" is empty. Cannot import it.");
		#endif
		fclose(fp);
		msAlertCode = ecFileOpenRead;
		return false;
	}
	
	// read a line of data
	fgets(buf, cCsvLineBufferSize, fp);
	// strip the newline
	buf[strlen(buf)-1] = '\0';
	
	if (!SplitString(buf, ',', vals))
	{
		#ifdef QRAP_DEBUG
		Log("Parsing error in CSV file "+fileName+" on line 2.");
		#endif
		msAlertCode = ecParse;
		fclose(fp);
		return false;
	}
	
	count = vals.size();

	// line 2 contains the field names for each table - this will allow us to get the
	// fields' names as well as the boundaries for each table
	curTable = 0;
	curField = 0;
	tableRanges[tables[curTable]].start = 0;
	for (i=0;i<count;i++)
	{
		// if we've reached an end-of-table indicator
		if (vals[i].length() == 0)
		{
			tableRanges[tables[curTable]].stop = i-1;
			curTable++;
			curField = 0;
			if ((i+1) < count)
				tableRanges[tables[curTable]].start = i+1;
		} else
		{
			// otherwise, check if a field like this exists for this table
			if (mStructure[table[curTable]].mFields.count(vals[i]) == 0)
			{
				#ifdef QRAP_DEBUG
				Log("Unknown field on line 2 for table "+tables[curTable]+": \""+vals[i]+"\".");
				#endif
				msAlertCode = ecParse;
				fclose(fp);
				return false;
			}
			
			// if it does exist, add it to the list of entry fields for this table
			fieldNames[tables[curTable]].push_back(vals[i]);
			curField++;
		}
	}
	
	curLine = 3;
	
	while (!feof(fp))
	{
		// read a line of data
		fgets(buf, cCsvLineBufferSize, fp);
		// strip the newline
		buf[strlen(buf)-1] = '\0';
		
		if (!SplitString(buf, ',', vals))
		{
			#ifdef QRAP_DEBUG
			Log("Parsing error in CSV file "+fileName+" on line "+IntToString(curLine)+".");
			#endif
			msAlertCode = ecParse;
			fclose(fp);
			return false;
		}
		
		count = vals.size();

		// run through the fields, skipping and dancing like a fairy
		
		// actually, run through the tables first
		for (i=0;i<tableCount;i++)
		{
			insertVals.clear();
			k = 0;
			// run through this table's fields
			for (j=tableRanges[tables[i]].start;j < tableRanges[tables[i]].stop;j++)
			{
				// skip this field if it's empty
				if (vals[i].length() == 0) continue;
				
				// get the values for these fields
				insertVals[fieldNames[tables[i]][k]] = vals[i];
				k++;
			}
			
			// if there is an entry to be inserted
			if (!insertVals.empty())
			{
				// insert it into the temporary table
				if (!Insert(insertVals, tables[i]+"_temp"))
				{
					#ifdef QRAP_DEBUG
					Log("Error while inserting entry from line "+IntToString(curLine)+" from CSV file "+fileName+". Probably a duplicate primary key.");
					#endif
				}
			}
		}
	}
	
	fclose(fp);
	
	// now run through the tables and check if there are any existing entries in the temporary tables
	for (i=0;i<tableCount;i++)
	{
		// if this table has its ID manually specified... check if a similar entry already exists
		if (StringArrayPos(fieldNames[tables[i]], "id") > -1)
		{
			// get the IDs of similar entries from the database
			if (!PerformRawSql("select id from "+tables[i]+" where id in (select id from "+tables[i]+"_temp);"))
			{
				#ifdef QRAP_DEBUG
				Log("Unable to select existing entries from table "+tables[i]+" from temporary table.");
				#endif
				msAlertCode = ecInternalError;
				return false;
			}
			
			// run through the IDs
			count = mLastResult.size();
			for (j=0;j<count;j++)
			{
				// add this ID to the existing entries for this table
				existing[tables[i]].push_back(mLastResult[j]["id"].c_str());
				// delete the entry from the temporary table
				if (!PerformRawSql("delete from "+tables[i]+"_temp where id = "+string(mLastResult[j]["id"].c_str())+";"))
				{
					#ifdef QRAP_DEBUG
					Log("Unable to delete duplicate entry with id "+string(mLastResult[j]["id"].c_str())+" from temporary table for "+tables[i]+".");
					#endif
					msAlertCode = ecInternalError;
					return false;
				}
			}
		}
	}
	
	// finally, copy the entries over into the real table
	if (!PerformRawSql(
	
	return true;
}*/

