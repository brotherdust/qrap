/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAlert.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson
 *                : email: (roeland.frans@gmail.com)
 *    Description : Error code and exception implementation
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

#ifndef Qrap_Alert_h
#define Qrap_Alert_h

/*
 * ERROR HANDLING MACROS
 * 
 * All of these macros must be called within a function derived from cBase.
 */

/**
 * Throws an alert of type alError.
 */
#define QRAP_THROW_ERROR(msg)               throw Qrap::cAlert(Qrap::alError, msg)

/**
 * Throws an alert of type alFatal.
 */
#define QRAP_THROW_FATAL(msg)               throw Qrap::cAlert(Qrap::alFatal, msg)

/**
 * Sets the callback function for when one of QRAP_INFO, QRAP_WARN, QRAP_ERROR or QRAP_FATAL
 * are called.
 */
#define QRAP_SET_ALERT_CALLBACK(callback)   Qrap::cBase::msAlertCallback = callback

/**
 * Logs an information alert.
 */
#define QRAP_INFO(msg)                      Qrap::Log(Qrap::alInfo, msg, Qrap::acOk, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * Logs a warning alert.
 */
#define QRAP_WARN(msg)                      Qrap::Log(Qrap::alWarning, msg, Qrap::acOk, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * Logs an error alert. Carries the current alert code through.
 */
#define QRAP_ERROR(msg)                     Qrap::Log(Qrap::alError, msg, cBase::msAlertCode, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * Logs an error alert - with alert code specified.
 */
#define QRAP_ERROR_CODE(msg,code)           Qrap::Log(Qrap::alError, msg, code, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * Logs a fatal error alert. Carries current alert code through.
 */
#define QRAP_FATAL(msg)                     Qrap::Log(Qrap::alFatal, msg, cBase::msAlertCode, __PRETTY_FUNCTION__, __FILE__, __LINE__)

/**
 * Logs a fatal error alert - with alert code specified.
 */
#define QRAP_FATAL_CODE(msg,code)           Qrap::Log(Qrap::alFatal, msg, code, __PRETTY_FUNCTION__, __FILE__, __LINE__)



namespace Qrap
{
	/**
	 * Error code specification.
	 */
	enum AlertCode
	{
		acOk,                            ///< No error.
		acDbConnect,                     ///< Could not connect to local database. Check PostgreSQL configuration.
		acDbDisconnect,                  ///< Could not disconnect from database. Probably not connected or connection dropped.
		acDbAuth,                        ///< Database authentication failed. Invalid user name or password.
		acDbDropped,                     ///< Connection to database dropped.
		acDbTableNotExist,               ///< The specified database table does not exist.
		acDbFieldNotExist,               ///< The specified field does not exist.
		acDbFunctionNotExist,            ///< A function in the query does not exist.
		acDbEntryNotExist,               ///< The specified entry does not exist.
		acDbNotAllowed,                  ///< The specified operation is not allowed for this user.
		acDbParams,                      ///< The parameters of the function are in error.
		acDbLog,                         ///< An error occurred while attempting to log the transaction.
		acDbPrimaryKey,                  ///< An error occurred while attempting to obtain the entry's primary key. The transaction will not be logged and no UNDO will be available.
		acDbUndoExtract,                 ///< Could not extract data for UNDO operation.
		acDbUndoExec,                    ///< Could not execute UNDO query.
		acDbNotLoggedIn,                 ///< The user is not logged in yet.
		acDbInvalidData,                 ///< Invalid data supplied for SQL entry.
		acDbIntegrityViolation,          ///< Integrity constraint violation in query.
		acDbInvalidStatement,            ///< Invalid SQL statement.
		acDbSyntax,                      ///< Syntax error in SQL statement.
		acCredDecrypt,                   ///< An error occurred while decrypting the user's credentials from the settings store.
		acParse,                         ///< A parsing error occurred.
		acFileOpenWrite,                 ///< Could not open file for writing.
		acFileOpenRead,                  ///< Could not open file for reading.
		acBrokenConnection,              ///< Broken connection to database.
		acInternalError,                 ///< Internal error.
		acMissingCredentials,            ///< Database credentials missing from configuration file.
		acMissingEntry,                  ///< Missing compulsory entry from configuration file.
		acTransactionFailed,             ///< Database transaction failed.
		acQrapAuth,                    ///< QRAP authentication failed.
		acQrapNoGroupFound,            ///< No groups matching the given group name found.
		acQrapNoGroups,                ///< User belongs to no groups - not allowed.
		acNoPrimaryKey,                  ///< No primary key has been specified for this table (through the SetPrimaryKeys() function).
		acSpawnThread,                   ///< An error occurred while attempting to spawn a new thread.
		acClientAlreadyConnected,        ///< The client is already connected to a server.
		acClientCommand,                 ///< Error while transmitting command to server.
		acServerResponse,                ///< Error while transmitting a response to a client.
		acCrcError,                      ///< CRC check failure - data is corrupt.
		acClientCommandTooLong,          ///< The client's previous command exceeded the maximum command length.
		acServerSocketCreate,            ///< Unable to create server socket.
		acFileEncode,                    ///< Unable to encode the specified file.
		acNotEnoughPrimaryKeys,          ///< Not enough primary keys allocated for this table to insert new entries.
		acDbUserRemoval,                 ///< Cannot remove a user while still logged in.
		acDbGroupRemoval,                ///< Cannot remove a group while still logged in.
		acDbMustLogIn,                   ///< User must log in to perform this action.
		acUserMustCheckOut,              ///< User must check out data from server first.
		acCreatePath,                    ///< Unable to create the path for the downloaded file. Please check program access privileges.
		acGeneral,                       ///< General information/warning/error.
		acInvalidInputFormat             ///< Invalid input format.
	};
	
	/**
	 * Alert level specification
	 */
	enum AlertLevel
	{
		alInfo,          ///< Information.
		alWarning,       ///< Warning.
		alError,         ///< Error.
		alFatal          ///< Fatal error.
	};
	
	/**
	 * Callback function type for logging and alert purposes. The first parameter
	 * specifies the alert level, the second specifies the message attached to the alert.
	 * Only called within cBase::Log.
	 */
	typedef void (*AlertCallback) (AlertLevel, const std::string);
	
	/**
	 * Translates the given alert code into a string describing the error.
	 * 
	 * @param alertCode The error's code.
	 * 
	 * @return A string containing a textual description of the given error.
	 */
	std::string TranslateAlertCode (AlertCode alertCode);
	
	/**
	 * Exception and alert handler class for QRAP.
	 */
	 
	class cAlert
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param code The error code associated with this object.
		 */
		cAlert (AlertCode code, const std::string& param = "", AlertLevel level = alError):
			mAlertCode(code), mAlertLevel(level), mUseCode(true)
		{
		};
		
		/**
		 * Alternative constructor - for one with an associated error message instead of a code.
		 */
		cAlert (AlertLevel level, const std::string& msg, const std::string& param = ""):
			mAlertLevel(level), mAlertMsg(msg),
			mUseCode(false) { };
		
		/**
		 * Destructor.
		 */
		~cAlert () { };
		
		
		/**
		 * Returns the error code associated with this object.
		 *
		 * @return An integer specifying the error code.
		 */
		AlertCode GetCode () const { return mAlertCode; };
		
		/**
		 * Returns the level associated with this alert code.
		 */
		AlertLevel GetLevel () const { return mAlertLevel; };
		
		/**
		 * Returns whether or not this is a code-based alert.
		 */
		bool IsCode () const { return mUseCode; };
		
		/**
		 * Returns the message associated with this code.
		 */
		std::string GetMsg () const { return mAlertMsg; };
		
		/**
		 * Converts this particular alert into a string, based on
		 * whether it is a code-alert or message-alert.
		 */
		std::string ToString () const
		{
			if (mUseCode)
				return TranslateAlertCode(mAlertCode);
			return mAlertMsg;
		};
	
	private:
		AlertCode   mAlertCode;           ///< The alert code associated with this object.
		AlertLevel  mAlertLevel;          ///< The alert level associated with this object.
		std::string mAlertMsg;            ///< A message associated with this alert object.
		bool        mUseCode;             ///< Is the mAlertCode member in use?
	};

}

#endif
