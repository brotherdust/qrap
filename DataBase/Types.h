 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Types.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Thane Thomson (roeland.frans@gmail.com)
 *    Description : Generic data types used throughout the package,
 *                  particularly in teh Database
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

#ifndef Qrap_Types_h
#define Qrap_Types_h

#include <string>
#include <vector>
#include <map>

namespace Qrap
{
	/**
	 * A data type encapsulating a 64-bit integer.
	 */
	typedef long long longint;
	
	/**
	 * An array of strings.
	 */
	typedef std::vector<std::string>           StringArray;
	typedef StringArray::iterator              StringArrayIterator;
	
	/**
	 * An associative map of strings to integer keys.
	 */
	typedef std::map<longint, std::string>     StringIntArray;
	typedef StringIntArray::iterator           StringIntArrayIterator;
	
	/**
	 * A 2D array of strings.
	 */
	typedef std::vector<StringArray>           String2dArray;
	typedef String2dArray::iterator            String2dArrayIterator;
	
	/**
	 * An associative string map, with keys and values being strings.
	 */
	typedef std::map<std::string,
			 std::string>              StringMap;
	typedef StringMap::iterator                StringMapIterator;
	
	/**
	 * An associative character map, with strings for keys and char's for values.
	 */
	typedef std::map<std::string,
			 char>                     CharMap;
	typedef CharMap::iterator                  CharMapIterator;
	
	/**
	 * A two-dimensional map of strings, indexed by string keys.
	 */
	typedef std::map<std::string, StringArray> StringArrayMap;
	typedef StringArrayMap::iterator           StringArrayMapIterator;
	
	/**
	 * An array of string maps.
	 */
	typedef std::vector<StringMap>             StringMapArray;
	
	/**
	 * A simple array of long integers.
	 */
	typedef std::vector<longint>               IntArray;
	
	/**
	 * An associative map of long integers.
	 */
	typedef std::map<std::string, longint>     IntMap;
	typedef IntMap::iterator                   IntMapIterator;
	
	/**
	 * Field unit type.
	 */
	enum UnitType
	{
		utNone,			///< No units associated with this field.
		utPower,		///< Watt, dBm, dBW.
		utSensitivity,	///< Sensitivity units, dBW, dBm , dBuV.
		utEirp,			///< EIRP units, Watt, dBm, dBW.
		utHeight,		///< meters
		utDistance,		///< kilometers
		utLossGain,		///< dB
		utMegaHertz,	///< MegaHertz
		utkiloHertz,	///< MegaHertz
		utAngle			///< Degrees
	};
	
	/**
	 * The field units that can is available in this system.
	 */
	enum Unit
	{
		udBm,	///< The system default = 10*log(w) + 30
		udBW,	///< Decibels Watt = 10*log(W)
		uW,		///< Normal Watt
		udBuV,	///< Decibels micro volts = dBm + 10*log(Zin) + 90
		um,		///< meters
		uKm,		///< kilometers
		udB,	///< deciBell
		uMHz,	///< MegaHertz
		ukHz,	///< kiloHertz
		uDeg,	///< Degrees
	};
	
	/**
	 * The field structure type - for use in the Settings class.
	 */
	struct DbField
	{
		std::string mType;                 ///< The SQL data type of this field.
		std::string mLabel;                ///< The label for this field.
		std::string mDescription;          ///< A more in-depth description of this field.
		bool        mVisible;              ///< Is this field visible? Default: true (if not specified).
		bool        mReadOnly;             ///< Is this field read-only? Default: false (if not specified).
		std::string mUi;                   ///< User interface parameters. Example: "dropdown(fixed, {'Option 1','Option 2','Option 3'})", or "dropdown(fromtable, tablename, tablefield)"
		int         mOrder;                ///< The creation order of this field - automatically assigned.
		bool        mIsForeign;            ///< Is this field a foreign key pointer?
		std::string mForeignTable;         ///< If this field is a foreign key, to which table does it point?
		bool        mIsFileLink;           ///< Is this field a link to a file on the hard drive?
		bool        mServerOnly;           ///< Is this field to be created on the server only?
		bool        mClientOnly;           ///< Is this field to be create on the client only?
		bool        mPostGis;              ///< Is this field to be added by PostGIS?
		UnitType    mUnitType;             ///< The base unit type of this field.
		
		// constructor - to set initial values
		DbField (): mVisible(true), mIsForeign(false), mIsFileLink(false),
			mServerOnly(false), mClientOnly(false), mPostGis(false),
			mUnitType(utNone) {};
	};
	
	/**
	 * An associative map of Field elements - for use in the Settings class. Each key is the name of the
	 * respective field in its associated table.
	 */
	typedef std::map<std::string, DbField>    DbFieldMap;
	typedef DbFieldMap::iterator              DbFieldMapIterator;
	
	/**
	 * A structure representing a table view.
	 */
	struct DbView
	{
		std::string mAs;                  ///< The view's AS clause.
		StringMap   mFieldLabels;         ///< A map of field names to field labels for additional fields in this view.
	};
	
	/**
	 * An associative array of DbView elements. Maps view names to view description structures.
	 */
	typedef std::map<std::string, DbView>     DbViewMap;
	typedef DbViewMap::iterator               DbViewMapIterator;
	
	/**
	 * A structure representing a table in the database.
	 */
	struct DbTable
	{
		std::string mLabel;               ///< An English label for the table.
		std::string mDescription;         ///< A more refined description of the table.
		DbFieldMap  mFields;              ///< The array of fields, indexed by name.
		std::string mFooter;              ///< Table creation footer string. Usually empty.
		bool        mVisible;             ///< Is this table visible? Default: true (if not specified).
		int         mCreateOrder;         ///< Creation order of this table.
		bool        mFullDownload;        ///< Should this table be downloaded in full when updating?
		bool        mCustomSequence;      ///< Does this table have its own custom sequence?
		DbViewMap   mViews;               ///< Views associated with this table.
		
		/**
		* Constructor. Merely used to initialise default values.
		*/
		DbTable (): mVisible(true), mFullDownload(false), mCustomSequence(false) {};
	};
	
	/**
	 * The database structure type - for use in the Settings class. Each key is the name of
	 * the respective table in the database.
	 */
	typedef std::map<std::string, DbTable>     DbStructure;
	typedef DbStructure::iterator              DbStructureIterator;
	
	/**
	 * A general byte type.
	 */
	typedef unsigned char byte;
	
	/**
	 * Server response codes.
	 */
	enum ServerCode
	{
		scNotConnected      = -1,                ///< Not yet connected.
		scConnected         = 0,                 ///< Not a response code - indicates that the client has just connected.
		scLoggedIn          = 1,                 ///< The client has just logged in successfully.
		
		scSuccess           = 200,               ///< 200 [Various possible messages]
		scUsername          = 201,               ///< 201 Username:
		scPassword          = 202,               ///< 202 Password:
		scTransmitFile      = 203,               ///< 203 Transmitting file [fileName] from next line.
		scTransmitTable     = 204,               ///< 204 Transmitting tables with entry summaries from next line.
		scTransmitEntries   = 205,               ///< 205 Transmitting selected entries from next line.
		scTransmitFullEntry = 206,               ///< 206 Transmitting entry and all related entries from next line.
		scTransmitKeyRanges = 207,               ///< 207 Transmitting key ranges for non-auto-increment tables.
		scTransmitMD5       = 208,               ///< 208 Transmitting MD5 hash of requested files.
		scDisconnect        = 250,               ///< 250 Disconnecting. Good bye.
		scMoreData          = 260,               ///< (No output)
		scCancelData        = 261,               ///< (No output)
		
		scErrorGeneral      = 300,               ///< 300 [Various possible error messages]
		scErrorLogin        = 301,               ///< 301 Invalid username or password.
		scErrorSyntax       = 302,               ///< 302 Syntax error in command.
		scErrorFile         = 303,               ///< 303 The specified file does not exist.
		scErrorFormat       = 304,               ///< 304 Invalid transaction format.
		scErrorFailed       = 305,               ///< 305 Transaction failed. Entry upon which transaction is operating has probably been removed by another user.
		scErrorNotAllowed   = 306,               ///< 306 The given transaction is not allowed by [userName]. Tampering suspected - this will be logged for the administrator.
		scErrorInternal     = 307,               ///< 307 An internal server error has occurred. Please contact the administrator.
		scErrorMustLogin    = 308,               ///< 308 You must be logged in to execute that command.
		scErrorDatabase     = 309,               ///< 309 An internal database error has occurred.
		scErrorSelectTable  = 310,               ///< 310 You must first download a table's details to select the active table.
		scErrorMustCheckIn  = 311,               ///< 311 You already have an open checkout session - please check in first.
		scErrorMustCheckOut = 312                ///< 312 You are not checked out - please check out first.
	};
	
	
	/**
	 * The format in which degrees are read/stored.
	 */
	enum DegreeFormat
	{
		dfDegMinSec,        ///< [degrees]:[minutes]:[seconds] [N/E/W/S]
		dfDegMin,           ///< [degrees]:[minutes].[decimal minutes] [N/E/W/S]
		dfDeg               ///< [degrees].[decimal degrees]
	};

}

#endif
