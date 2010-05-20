/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAlert.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson
 *                : email: (roeland.frans@gmail.com)
 *    Description : Error code and exception implementation
 *
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

#include <string>

#include "cAlert.h"

using namespace std;

string Qrap::TranslateAlertCode (AlertCode alertCode)
{
	switch (alertCode)
	{
	case acOk:
		return string("No error occurred.");
	case acDbConnect:
		return string("Could not connect to local database. Check PostgreSQL configuration.");
	case acDbDisconnect:
		return string("Could not disconnect from database. Probably not connected or connection dropped.");
	case acDbAuth:
		return string("Database authentication failed. Invalid user name or password.");
	case acDbDropped:
		return string("Connection to database dropped.");
	case acDbTableNotExist:
		return string("The specified database table does not exist.");
	case acDbFieldNotExist:
		return string("The specified field does not exist.");
	case acDbNotAllowed:
		return string("The specified operation is not allowed for this user.");
	case acDbNotLoggedIn:
		return string("Cannot perform transaction before logging in.");
	case acDbParams:
		return string("The parameters of the function are in error.");
	case acDbLog:
		return string("An error occurred while attempting to log the transaction.");
	case acDbPrimaryKey:
		return string("An error occurred while attempting to obtain the entry's primary key. The transaction will not be logged and no UNDO will be available.");
	case acDbUndoExtract:
		return string("Could not extract data for UNDO operation.");
	case acDbUndoExec:
		return string("Could not execute UNDO query.");
	case acDbInvalidData:
		return string("Invalid data supplied for SQL entry.");
	case acDbIntegrityViolation:
		return string("Integrity constraint violation in query.");
	case acDbInvalidStatement:
		return string("Invalid SQL statement.");
	case acDbSyntax:
		return string("Syntax error in SQL statement.");
	case acDbFunctionNotExist:
		return string("A function in the query does not exist.");
	case acDbEntryNotExist:
		return string("The specified entry does not exist.");
	case acCredDecrypt:
		return string("An error occurred while decrypting the user's credentials from the settings store.");
	case acParse:
		return string("A parsing error occurred.");
	case acFileOpenWrite:
		return string("Could not open file for writing.");
	case acFileOpenRead:
		return string("Could not open file for reading.");
	case acBrokenConnection:
		return string("Broken connection to database.");
	case acInternalError:
		return string("Internal error.");
	case acMissingCredentials:
		return string("Database credentials missing from configuration file.");
	case acMissingEntry:
		return string("Missing compulsory entry from configuration file.");
	case acTransactionFailed:
		return string("Database transaction failed.");
	case acQrapAuth:
		return string("QRAP authentication failed.");
	case acQrapNoGroupFound:
		return string("No groups matching the given group name found.");
	case acQrapNoGroups:
		return string("User belongs to no groups - not allowed.");
	case acNoPrimaryKey:
		return string("No primary key has been specified for this table (through the SetPrimaryKeys() function).");
	case acSpawnThread:
		return string("An error occurred while attempting to spawn a new thread.");
	case acClientAlreadyConnected:
		return string("The client is already connected to a server.");
	case acClientCommand:
		return string("Error while transmitting command to server.");
	case acServerResponse:
		return string("Error while transmitting a response to a client.");
	case acCrcError:
		return string("CRC check failure - data is corrupt.");
	case acClientCommandTooLong:
		return string("The client's previous command exceeded the maximum command length.");
	case acServerSocketCreate:
		return string("Unable to create server socket.");
	case acFileEncode:
		return string("Unable to encode the specified file.");
	case acNotEnoughPrimaryKeys:
		return string("Not enough primary keys allocated for this table to insert new entries.");
	case acDbMustLogIn:
		return string("User must log in to perform this action.");
	case acCreatePath:
		return string("Unable to create the path for the downloaded file. Please check program access privileges.");
	case acGeneral:
		return string("General information/warning/error.");
	case acInvalidInputFormat:
		return string("Invalid input format.");
	default:
		return string("An unknown error occurred.");
	}
}
