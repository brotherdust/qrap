 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cDatabase.h
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

#ifndef Qrap_cDatabase_h
#define Qrap_cDatabase_h


#include "cSiteEditTools.h"
#include "Config.h"

using std::string;

namespace Qrap
{
	const int         cCsvLineBufferSize  = 32768;   ///< The maximum number of bytes in a single line from a CSV file.
	const std::string cPostGisDefaultSrid = "4326";  ///< The default PostGIS SRID if one is not specified in the settings file.
	
	/**
	 * The main database engine encapsulation class.
	 *
	 * Acts as an interface to a local PostgreSQL database, providing PostgreSQL database authentication,
	 * QRAP user authentication, query authorisation, SQL query construction,
	 * transaction logging, interaction with the authentication server using the
	 * authentication protocol via SSL (OpenSSL), and handles UNDO commands.
	 * 
	 * <b>CONNECTING</b><br />
	 * For convenience (and internal use) there are two global variables in the Qrap namespace:
	 * <i>gSettings</i> and <i>gDb</i>.
	 * 
	 * - Call gSettings.LoadFromFile to load the database settings BEFORE calling any database
	 *   functions.
	 * - Call gDb.Connect with the username and password.
	 *
	 */
	
	class cDatabase: public cBase
	{
		friend class cTransactor;
		friend class cClient;
	public:
		short int globalMachineID;
		
		/**
		 * @name Class-specific type definitions
		 */
		
		/**
		 * User interface element type.
		 */
		enum FieldUiType
		{
			utNormal,         ///< A normal field element, whose type is determined by Qt.
			utDropdownFixed,  ///< The field is a dropdown combo box, having literal string values as values.
			utDropdownTable   ///< The field is a dropdown combo box with its values loaded from a table.
		};
		
		/**
		 * Field data type.
		 */
		enum FieldDataType
		{
			dtString,             ///< For VARCHAR and TEXT types.
			dtInteger,            ///< For any INTEGER type.
			dtReal,               ///< For REAL or DOUBLE PRECISION types.
			dtBoolean,            ///< For BOOLEAN types.
			dtDate,               ///< For DATE types.
			dtDateTime,           ///< For TIMESTAMP types.
			dtPostGisPoint,       ///< For PostGIS POINT types.
			dtPostGisLineString,  ///< For PostGIS LINESTRING types.
			dtPostGisPolygon      ///< For PostGIS POLYGON types.
		};
		
		/**
		 * A single SQL rule.
		 */
		struct SqlRule
		{
			std::string mLimiter;       ///< The limiting where clause(s) for the rule.
			std::string mDo;            ///< The string to be appended after the "do" keyword in the "create rule" command.
		};
		
		/**
		 * An array of SQL rules.
		 */
		typedef std::vector<SqlRule> SqlRules;
		
		/**
		 * Privileges specific to a table.
		 */
		struct TablePrivileges
		{
			std::string mViewLimiter;    ///< The view limiter for this table - only one view is created for each group.
			SqlRules    mInsertRules;
			SqlRules    mUpdateRules;
			SqlRules    mDeleteRules;
			
			bool        mSelect;         ///< Does the user have select access to this table?
			bool        mUpdate;         ///< Does the user have update access to this table?
			bool        mInsert;         ///< Insert access?
			bool        mDelete;         ///< Delete access?
			
			TablePrivileges (): mSelect(true), mUpdate(true),
				mInsert(true), mDelete(true) {};
		};
		
		/**
		 * An array of table privileges, representing the table privileges for the
		 * whole database.
		 */
		typedef std::map<std::string,TablePrivileges> TablePrivilegesMap;
		typedef TablePrivilegesMap::iterator          TablePrivilegesMapIterator;
		
		/**
		 * Group privilege structure.
		 */
		struct GroupPrivileges
		{
			bool               mCreateUsers;
			bool               mCreateRoles;
			TablePrivilegesMap mTablePrivileges;
			
			GroupPrivileges (): mCreateUsers(false),
				mCreateRoles(false) {};
		};
	
		/**
		 * @name Internal classes
		 */
		class cTransactor: public pqxx::transactor<>
		{
		public:
			cTransactor (cDatabase* owner, const std::string& query)
				: mQuery(query), mOwner(owner)
				{ };
			
			/**
			 * Called by pqxx::connection::perform to handle database transaction.
			 */
			void operator () (argument_type& t);
			
			/**
			 * Function overrides inherited function to handle "in doubt" errors.
			 */
			void on_doubt ();
			
		private:
			// the query string to perform in the () operator
			std::string mQuery;
			// pointer to the owner of this transactor
			cDatabase*   mOwner;
		};
		
		
		/**
		 * @name Construction/destruction
		 */
		
		/**
		 * Constructor.
		 */
		cDatabase ();
		
		/**
		 * Destructure. Closes any open connection to the database.
		 */
		~cDatabase ();
		
		
		/**
		 * @name Connection
		 */
		 
		/**
		 * Connects to the local database using the given username and password. Loads the
		 * connection settings from the global gSettings variable.
		 * 
		 * @param username The username with which to connect.
		 * @param password The password for the specified user.
		 * 
		 * @return true on success, false on failure. GetLastError() will return
		 * extended information on the reason for failure.
		 */
		bool Connect (const string& username, const string& password, bool Create=false,
			const string& database="qrap", const string& host="localhost", const int Iport=5432);
		

		bool Connected() { return mConnected;}
		/**
		 * Disconnects from the local database.
		 * 
		 * @return true on success, false on error.
		 */
		bool Disconnect ();
		
		/**
		 * @name SQL functions
		 */
		
		/**
		 * Performs a raw SQL query.
		 * 
		 * @param command The raw SQL query string.
		 * 
		 * @return true on success, false on error. Extended information is available by
		 * calling GetLastError().
		 */
		bool PerformRawSql (const std::string& command);
		
		/**
		 * Performs a simple select query. Forcefully omits the <i>serverid</i> and
		 * <i>lastmodified</i> columns.
		 * 
		 * @param columns The columns to select, separated by commas.
		 * @param tableName The name of the table from which to select.
		 * @param where The additional where clause specifiers to add to the command.
		 * @param orderBy The field name by which to order the output.
		 * @param ascending Whether or not ordering should be done in an ascending manner.
		 * @param hideDefault Whether or not to hide the default value in this table
		 * when selecting.
		 * 
		 * @return true on success, false on SQL error.
		 */
		bool Select (const std::string& columns, const std::string& tableName,
			const std::string& where, const std::string& orderBy = "id",
			bool ascending = true, bool hideDefault = true);
		
		/**
		 * Performs an insert query.
		 * 
		 * @param values An associative array of values to insert into the table, where the
		 * keys of the array represent the column names.
		 * @param tableName The name of the table into which the entry must be inserted.
		 * 
		 * @return The ID of the entry just inserted on success, or -1 on failure or error.
		 * GetLastError() returns more detailed information on a failure.
		 */
		int Insert (const StringMap& values, const std::string& tableName);
		
		/**
		 * Performs an update query.
		 * 
		 * @param values An associative array of values to update in the table, where the
		 * keys of the array represent the column names.
		 * @param tableName The name of the table to be updated.
		 * @param where The where clause to narrow the update.
		 * @param skipDefault Whether or not the default value should be skipped.
		 * 
		 * @return true on success, or false on error.
		 */
		bool Update (const StringMap& values, const std::string& tableName,
			const std::string& where, bool skipDefault = true);
		
		/**
		 * Performs a delete query.
		 * 
		 * @param tableName The name of the table from which entries should be deleted.
		 * @param where The where clause narrowing the deletion.
		 * 
		 * @return true on success, false on error.
		 */
		bool Delete (const std::string& tableName, const std::string& where);
		
		short int getsetMachineID();

//		bool GetCloseBySite(QString Lat,QString Lon,bool Edit,double scale);

		/**
		 * Starts an import of data into the database from a CSV file. The structure
		 * is relatively straightforward: the first line must contain the columns' names,
		 * and all the lines from the second one contain one single entry.
		 * 
		 * @param fileName The name of the CSV file to import.
		 * @param tableName The name of the table into which the CSV data must be imported.
		 * @param failed An array indicating the line numbers of those entries that were
		 * not successfully imported.
		 * @param format The format in which degrees will be interpreted, should they be
		 * encountered.
		 * 
		 * @return true on success, false on failure.
		 */
		bool ImportFromCsv (const std::string& fileName, const std::string& tableName,
			IntArray& failed, DegreeFormat format = dfDeg);
		
		/**
		 * Imports the CSV data regarding high sites from the given CSV file.
		 * This parser is SAPS-specific.
		 * 
		 * @param fileName The name of the file to import.
		 * @param format The format in which degrees are to be read.
		 * @param failed The line numbers of the entries that failed.
		 * 
		 * @return true on success, false on error.
		 */
		bool ImportFromCsvSaps (const std::string& fileName, DegreeFormat format,
			IntArray& failed);
		
		/**
		 * Exports the result of the given command to the given file.
		 * 
		 * @param cmd The command to execute.
		 * @param fileName The name of the file into which the result should be saved.
		 * 
		 * @return true on success, false on failure.
		 */
		bool ExportCmdToCsv (const std::string& cmd, const std::string& fileName);
		
		/**
		 * Exports the entire contents of the specified table to the specified CSV file.
		 * 
		 * @param tableName The name of the table to be exported.
		 * @param fileName The file in which the data will be stored.
		 * 
		 * @return true on success, false on failure.
		 */
		bool ExportToCsv (const std::string& tableName, const std::string& fileName);
		
		/**
		 * Logs the specified transaction in the qrap_undo table for undo purposes.
		 * 
		 * @param op The SQL operation (insert, update or delete).
		 * @param values An associative array of values (column/value pairs), if applicable.
		 * @param tableName The name of the table on which the operation took place.
		 * @param where The where clause associated with the operation.
		 * 
		 * @return true on success, false on failure.
		 */
// 		bool LogTransaction (const std::string& op, const StringMap& values,
// 			const std::string& tableName, const std::string& where);
		
		/**
		 * Creates the entire database as the given user.
		 * 
		 * @param name The name of the database to be created.
		 * @param username The root user, capable of creating a database and tables.
		 * @param password The password for the root user.
		 * 
		 * @return true on success, false on failure. GetLastError() returns extended
		 * error information.
		 */
		bool Create (const std::string& username, const std::string& password);
		
		/**
		 * Creates the specified table.
		 * 
		 * @param tableName The name of the table which must be created.
		 * @param tableSuffix A suffix to be appended to the end of the table name to be created.
		 * @param temporary Is the table a temporary one?
		 * 
		 * @return true on success, false on failure.
		 */
		bool CreateTable (const std::string& tableName, const std::string& tableSuffix = "",
			bool temporary = false);
		
		
		/**
		 * Retrieves the names of the tables related to this site somehow (by foreign
		 * key reference backward or forward).
		 * 
		 * @param tableName The name of the table for which to search.
		 * @param skipTables The names of the tables to skip when searching.
		 * @param forwardTables An array in which to store the forward-linked tables' names.
		 * @param backwardTables An array in which to store the backward-linked tables' names.
		 */
		void GetRelatedTables (const std::string& tableName, const StringArray& skipTables,
			StringArray& forwardTables, StringArray& backwardTables);
		
		/**
		 * Retrieves the name of the field in the specified table that links to the given
		 * foreign table by foreign key reference.
		 * 
		 * @param tableName The name of the table whose fields must be searched.
		 * @param foreignTable The foreign table for which to search.
		 * 
		 * @return A string containing the name of the field in the given table that
		 * links to the foreign table.
		 */
		std::string GetForeignField (const std::string& tableName, const std::string& foreignTable);
		
		/**
		 * Returns whether or not the given table exists in the database.
		 * 
		 * @param tableName The name of the table whose existence must be verified.
		 * 
		 * @return true if it exists, false otherwise.
		 */
		bool TableExists (const std::string& tableName, bool Spatial=false);
		
		/**
		 * Returns whether or not the given view exists in the database.
		 */
		bool ViewExists (const std::string& viewName);
		
		/**
		 * Retrieves the default values for the given table.
		 * 
		 * @param tableName The name of the table whose defaults must be retreived.
		 * @param output The output variable in which an associative map of defaults will be stored.
		 * Each entry's key will correspond to a column name, and its value will represent the default
		 * value for that column.
		 * 
		 * @return true on success, false if it could not access the <i>tabledefaults</i> table.
		 */
		bool GetDefaults (const std::string& tableName, StringMap& output);
		
		/**
		 * Sets the default values for the given table. Sets these values in entry
		 * 0 of the given table.
		 * 
		 * @param tableName The table whose defaults are to be set.
		 * @param defaults An associative map of defaults, where the key/value pair represents
		 * the column name/column default pair.
		 * 
		 * @return true on success, false on error.
		 */
		bool SetDefaults (const std::string& tableName,
				  const StringMap& defaults);
				 
		
		/**
		 * Returns a pointer to the field (DbField object) whose creation order is the specified
		 * number.
		 * 
		 * @param tableName The name of the table to which the field belongs.
		 * @param order The creation order of the field required. The first field
		 * created is the "id" field, with index 0.
		 * 
		 * @return A pointer to the field, or otherwise, a pointer to the first field it finds
		 * in the given table's array of fields.
		 */
		DbFieldMap::const_iterator GetFieldWithOrder (const std::string& tableName, int order);
		
		/**
		 * Returns a pointer to the table (DbTable object) whose creation order is
		 * the specified number.
		 * 
		 * @param order The creation order of the required table.
		 * 
		 * @return A pointer to the table with the given order.
		 */
		DbStructure::const_iterator GetTableWithOrder (int order);
		
		/**
		 * @name User and group administration
		 */
		
		/**
		 * Retrieves the group name to which the specified user belongs.
		 * 
		 * @param username The username of the user whose group name must be established.
		 * 
		 * @return A string containing the group name, or an empty string on error.
		 */
		std::string GetGroupName (const std::string& username);
		
		/**
		 * Creates a rule with regard to the specified table. The name of the rule
		 * has the format <pre>tableName_ruleNum_on</pre>. Automatically assigns a random
		 * rule name.
		 *
		 * @param on The SQL command on which this rule must be applied (insert, update or delete only).
		 * @param tableName The table to which the rule must be applied.
		 * @param rule The rule itself.
		 * @param groupName The group to which this rule must be applied.
		 * 
		 * @return true on success, false on failure.
		 */
		bool CreateRule (const std::string& on,
			const std::string& tableName, const SqlRule& rule, const std::string& groupName);
		
		/**
		 * Removes the given rule (or privilege) from the given table. If it is a privilege,
		 * the ruleName variable must contain "privilege_select" for a select privilege,
		 * "privilege_update" for an update privilege, etc.
		 * 
		 * @param ruleName The name of the rule to be removed.
		 * @param tableName The name of the table on which the rule has been applied.
		 * @param groupName The group to which this rule or privilege belongs.
		 * 
		 * @return true on success, false on failure.
		 */
		bool RemoveRule (const std::string& ruleName, const std::string& tableName,
			const std::string& groupName = "");
		
		/**
		 * Creates a view with regard to the specified table. The view is named
		 * <pre>tableName_groupName</pre>.
		 *
		 * @param tableName The name of the table to which the view applies.
		 * @param limiter The limiter to apply to the view.
		 * @param groupName The name of the group to which the view will apply.
		 * 
		 * @return true on success, false on failure.
		 */
		bool CreateView (const std::string& tableName, const std::string& limiter,
			const std::string& groupName);
		
		/**
		 * Removes the view with the given name.
		 * 
		 * @param viewName The name of the view to be removed.
		 * 
		 * @return true on success, false on failure.
		 */
		bool RemoveView (const std::string& viewName);
		
		/**
		 * Returns whether or not the specified user exists.
		 */
		bool UserExists (const std::string& username);
		
		/**
		 * Returns whether or not the specified group exists.
		 */
		bool GroupExists (const std::string& groupName);
		
		/**
		 * Creates a user with the given name, belonging to the given group. Must
		 * be logged in with user creation privileges to perform this action.
		 * 
		 * @param username The username of the user.
		 * @param password The user's password.
		 * @param group The group to which the user belongs.
		 * @param fullNames The user's full names.
		 * @param idNo The user's ID number.
		 * @param email The user's e-mail address.
		 * 
		 * @return true on success, false on failure.
		 */
		bool AddUser (const std::string& username, const std::string& password,
			const std::string& group, const std::string& fullNames = "",
			const std::string& idNo = "", const std::string& email = "");
		
		/**
		 * Removes the user with the given name. It is not possible to remove a user while
		 * still logged in.
		 * 
		 * @param username The username of the user to be removed.
		 * 
		 * @return true on success, false on failure.
		 */
		bool RemoveUser (const std::string& username);
		
		/**
		 * Creates a group with the given group privileges.
		 * 
		 * @param groupName The name of the group to be created.
		 * @param privileges The privileges to assign to the group.
		 * 
		 * @return true on success, false on failure.
		 */
		bool AddGroup (const std::string& groupName, const GroupPrivileges& privileges);
		
		/**
		 * Removes the specified group and all its associated privileges.
		 * 
		 * @param groupName The name of the group to be removed.
		 * 
		 * @return true on success, false on failure.
		 */
		bool RemoveGroup (const std::string& groupName);
		
		/**
		 * Retrieves a list of all the users in the qrap_users table.
		 * 
		 * @param users The output array in which all the usernames will be stored.
		 * 
		 * @return true on success, false on SQL error.
		 */
		bool GetUsers (StringArray& users);
		
		/**
		 * Retrieves a list of all the groups in the qrap_groups table.
		 * 
		 * @param groups The output array in which all the group names will be stored.
		 * 
		 * @return true on success, false on SQL error.
		 */
		bool GetGroups (StringArray& groups);
		
		/**
		 * @name Local configuration access
		 */
		 
		/**
		 * Retrieves a configuration setting from the local database for the current user.
		 * 
		 * @param name The name of the setting to retrieve.
		 * 
		 * @return A string containing the value of the setting on success, or
		 * an empty string on error.
		 */
		std::string GetSetting (const std::string& name);
		
		/**
		 * Sets the value of the specified configuration setting for the current user.
		 * 
		 * @param name The name of the setting that must be set.
		 * @param value The value of the setting.
		 * 
		 * @return true on success, false on failure.
		 */
		bool SetSetting (const std::string& name, const std::string& value);
		
		/**
		 * @name Member retrieval
		 */
		 
		/**
		 * Retrieves the current user's username.
		 */
		std::string GetUsername () const { return mUsername; };
		
		/**
		 * Retrieves a copy of the database structure.
		 */
		void GetDbStructure (DbStructure& output) const { output = mStructure; };
		
		/**
		 * Retrieves a copy of the last database transaction result.
		 */
		void GetLastResult (pqxx::result& output) const { output = mLastResult; };
		
		/**
		 * @name Field-specific routines for GUI
		 */
		
		/**
		 * Retrieves the specified field's general data type (for the user-interface).
		 */
		FieldDataType GetFieldDataType (const std::string& tableName,
						const std::string& fieldName);
		
		/**
		 * Retrieves the specified field's special user interface parameters as
		 * a list of strings.
		 * 
		 * @param tableName The name of the table in which the required field resides.
		 * @param fieldName The name of the required field.
		 * @param params An array of strings in which to place the user interface parameters
		 * for the given field. In the case of a drop-down menu whose elements are extracted
		 * from a table, the indexes of the array will correspond to the primary keys (id field)
		 * of each of the entries.
		 * 
		 * @return true on success, false on parsing error or if the table or field do not exist.
		 */
		bool GetFieldUiParams (const std::string& tableName,
				       const std::string& fieldName,
				       StringIntArray&    params);
		
		/**
		 * Retrieves the name of the foreign field with which the given field is associated
		 * in the user interface.
		 * 
		 * @param tableName The name of the applicable table.
		 * @param fieldName The name of the applicable field.
		 * 
		 * @return A string containing the name of the foreign table's field associated with
		 * the specified field.
		 */
		std::string GetUiForeignField (const std::string& tableName,
					       const std::string& fieldName);
		
		/**
		 * Retrieves the field user interface type for the specified field.
		 * 
		 * @param tableName The name of the table in which the required field resides.
		 * @param fieldName The name of the required field.
		 * 
		 * @return A FieldUiType enumeration indicating what type of field the
		 * requested field is.
		 */
		FieldUiType GetFieldUiType (const std::string& tableName,
					    const std::string& fieldName);
		
	private:
		/**
		 * @name Internal variables
		 */
		std::string       mHostAddr;               ///< The IP address of the local database.
		std::string       mName;                   ///< The name of the local database to which to connect.
		int               mPort;                   ///< The local database port number through which to connect.
		pqxx::connection* mConnection;             ///< The connection to the database.
		DbStructure       mStructure;              ///< The database structure template, loaded from the Settings parameter of the constructor.
		pqxx::result      mLastResult;             ///< The result of the last query to the database.
		
		std::string       mUsername;               ///< The username of the currently logged in user.
		std::string       mUserGroup;              ///< The group name to which the currently logged in user belongs.
		StringMap         mSettings;               ///< The internal settings structure.
		bool		mConnected;
		
		
		/**
		 * @name Table creation-related functions
		 */
		
		/**
		 * Constructs the query to create the table specified by the given table data.
		 * 
		 * @param tableName The name of the table to create.
		 * @param table The DbTable structure specifying the structure of the table.
		 * @param temporary Whether or not the table to be created is a temporary one.
		 * 
		 * @return A query string that can be passed to the database to create the table.
		 */
		std::string ConstructCreateTableQuery (const std::string& tableName,
						       const DbTable&     table,
						       bool               temporary = false);
	};
	
	/**
	 * Global database pointer.
	 */
	extern cDatabase gDb;
}

/**
 * \example cDatabase
 * <b>Initialisation</b><br /><br />
 * 
 * Firstly, one must realise that there are two global variables provided by the database
 * class set: gSettings and gDb. The gSettings variable represents a global settings object,
 * and one must call its LoadFromFile member prior to any calls to a cDatabase object. All
 * cDatabase objects, including the gDb object, will load their settings from the gSettings global
 * object.<br />
 * <br />
 * The gSettings object will load information from an XML file pertaining to the database host,
 * name and port.
 * 
 * \code
 * if (!gSettings.LoadFromFile("settings.xml"))
 * 	// handle error here and exit program - no point in continuing
 * \endcode
 * 
 * <b>Creating the tables</b>
 * 
 * Before one can use the database, one must create all the tables. The file <i>structure.xml</i>
 * will contain the full structure of the database to be created, and to create the structure
 * one must call the cDatabase::Create function. This creation function takes two parameters: a
 * username and password. These should be your root (postgres) username and password, as the user
 * calling the function must have table creation privileges in the database.
 * It is only necessary to create the tables once.
 * 
 * It is only necessary to create the tables once. Note, however, that one must first create
 * the database manually and install PostGIS before calling this Create function. These instructions
 * will be available in the QRAP installation manual.
 * 
 * \code
 * if (!gDb.Create("postgres", "rootpassword"))
 * 	// handle error here and exit
 * \endcode
 * 
 * <b>Connecting to the database</b>
 * 
 * Once the database is created, and the tables are created, one can connect to the database
 * using the cDatabase class. To connect, simply call the cDatabase::Connect function with your
 * username and password. It is not recommended that you connect using the root username and
 * password - you should create groups and then users with the functions cDatabase::AddGroup
 * and cDatabase::AddUser.
 * 
 * \code
 * if (!gDb.Connect("postgres", "rootpassword"))
 * 	// handle error here and exit
 * \endcode
 * 
 * <b>Users, groups and privileges</b>
 * 
 * The QRAP database authentication system relies on PostgreSQL groups and users to assign
 * privileges. Groups are created with privileges, and then users are assigned to those groups -
 * no special privileges are assigned to users directly.
 * 
 * Privileges are assigned based on SQL operation, i.e. SELECT, INSERT, UPDATE and DELETE.
 * Select privileges are created by virtue of a <i>view</i>, and insert, update and delete privileges
 * are created through <i>rule</i> creation. For more information on views and rules, please see
 * the PostgreSQL documentation.
 * 
 * The example below shows how a group <i>fieldtech</i> is created with limited access to sites,
 * where the users in the group are only allowed to:
 * 
 * - view sites whose status field is 'Experimental', 'Draft' and 'Planned' (i.e. excluding 'Active'
 *   sites),
 * - insert and update sites whose status field is 'Experimental' or 'Draft', and
 * - delete sites whose status field is 'Experimental'.
 * 
 * \code
 * GroupPrivileges priv;
 * SqlRule         rule;
 * 
 * // allow the user to perform select, insert, update and delete commands on table "site"
 * priv.mTablePrivileges["site"].mSelect = true;
 * priv.mTablePrivileges["site"].mInsert = true;
 * priv.mTablePrivileges["site"].mUpdate = true;
 * priv.mTablePrivileges["site"].mDelete = true;
 * 
 * // limit the view to only 'Experimental', 'Draft' and 'Planned' sites
 * // (the "lower" command simply converts the existing values in the status field to lowercase,
 * //  thereby making the comparison case-insensitive)
 * priv.mTablePrivileges["site"].mViewLimiter = "lower(status) = 'experimental' or lower(status) = 'draft' or lower(status) = 'planned'";
 * 
 * // limit inserting and updating to 'Experimental' and 'Draft' sites
 * rule.mLimiter = "lower(status) = 'experimental' or lower(status) = 'draft'";
 * rule.mDo = "instead nothing";
 * priv.mTablePrivileges["site"].mInsertRules.push_back(rule);
 * priv.mTablePrivileges["site"].mUpdateRules.push_back(rule);
 * 
 * // 
 * \endcode
 */

#endif
