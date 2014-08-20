 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBase.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson (roeland.frans@gmail.com)
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : A number of useful functions that occur often
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

#ifndef Qrap_cBase_h
#define Qrap_cBase_h

#include <QObject>
#include <stdlib.h>
#include "Types.h"
#include "cAlert.h"

namespace Qrap
{
	/**
	 * Logs a particular message along with other debug information, depending upon
	 * the program settings.
	 * 
	 * @param level The information logging level (equivalent to an AlertLevel).
	 * @param message The message to be logged.
	 * @param code The alert code to be logged.
	 * @param funcName The name of the calling function.
	 * @param srcFile The name of the source file in which the message is originating.
	 * @param srcLine The source line number where the message is originating.
	 *
	 * @return true on success, false if the log file could not be appended. Usual
	 * causes of failure would be not having created the file (touch) or
	 * not having made the file's owner the same user as the one running the QRAP application.
	 */
	bool Log (AlertLevel level, const std::string& message, AlertCode code,
		  const std::string& funcName, const std::string& srcFile,
	          int srcLine);
	
	/**
	 * Generic base class for all QRAP classes.
	 *
	 * Allows all derived classes to log debug information and process error messages. In the debug version,
	 * this class also contains debug information logging through
	 * a member mLogFileName (defining the current log file location), defined as static, and a function
	 * called DebugLog. mLogFileName is declared static in order to ensure a package-wide understanding
	 * of the current log file. Therefore, the first class to be created in any QRAP-used code
	 * is the Settings class, which will allow one to define the log file.
	 *
	 * Various levels of debugging are available, and the debugging levels (should the debug option
	 * be compiled into the system) 
	 */
	
	class cBase
	{
	public:
		static AlertCallback msAlertCallback;
		static AlertCode     msAlertCode;       ///< The error code associated with the last error or exception raised.
		static std::string   msLogFileName;     ///< The name of the log file in which debug info is to be stored.
		static int           msAlertDepth;      ///< The depth of an error message call.
		
		/**
		 * Constructor
		 */
		cBase ();
		
		/**
		 * Destructor
		 */
		~cBase () { };
		
		
		/**
		 * Returns the error code associated with the last function call.
		 *
		 * Should a function fail, calling GetLastError() immediately after the function call
		 * (and before another call to a function within the class) will return an error code
		 * describing the reason for failure.
		 *
		 * @return An error code or 0 if no error occurred.
		 */
		AlertCode GetLastAlert () const { return msAlertCode; };
		
		/**
		 * Old log function - deprecated.
		 */
		bool Log (const std::string& msg)
		{
			return Qrap::Log(alError, msg, msAlertCode, "(Unknown)", "(Unknown)", 0);
		};
		
	};

	

	/**
	 * The default console alert callback.
	 */
	void ConsoleAlert (AlertLevel level, const std::string& msg);
	
	/**
	 * Returns the index in the string of the first non-whitespace character.
	 * 
	 * @param str The string through which to search.
	 * @param start The starting position in the string from which to search. Default: 0.
	 * @param skipSpecial Whether or not to skip special characters (such as -+,./)
	 * 
	 * @return The integer value of the first character's index in the given string where that
	 * character is not whitespace.
	 */
	int FindNonWhitespace (const std::string& str, unsigned start = 0, bool skipSpecial = false);
	
	/**
	 * Extracts a literal (string or integer) value from the given string, starting
	 * at the given position.
	 * 
	 * @param str The string through which to search.
	 * @param start The starting position from which to search. Default: 0.
	 * @param fullStopDelim Whether or not to treat a full stop as a delimiter. Default: false.
	 * 
	 * @return A string containing the literal value starting at the given position.
	 */
	std::string ExtractLiteral (const std::string& str, int start = 0, bool fullStopDelim = false);
		
	/**
	 * Extracts a keyword from the given string, starting at the given position.
	 * 
	 * @param str The string through which to search.
	 * @param start The starting position in the string from which to search. Default: 0.
	 * 
	 * @return A string containing the keyword starting at the given starting point.
	 */
	std::string ExtractKeyword (const std::string& str, int start = 0);
	
	/**
	 * Extracts an operator from the given string, starting at the given position.
	 * 
	 * @param str The string through which to search.
	 * @param start The starting position in the string from which to search. Default: 0.
	 * 
	 * @return A string containing the operator starting at the given starting point.
	 */
	std::string ExtractOperator (const std::string& str, int start = 0);
	
	/**
	 * Converts a string to its lowercase equivalent.
	 * 
	 * @param str The string to convert.
	 * 
	 * @return A string containing the lowercase equivalent of <i>str</i>.
	 */
	std::string StrToLower (const std::string& str);
	
	/**
	 * Quotes a string for insertion into an SQL field. Escapes string literals and
	 * escape characters automatically.
	 * 
	 * @param str The string to quote.
	 * @param strChar The quote character to use.
	 * 
	 * @return A string containing the quoted version of <i>str</i>.
	 */
	std::string StrQuote (const std::string& str, char strChar = '\'');
	
	/**
	 * Unquotes a quoted string.
	 * 
	 * @param str The string to unquote.
	 * 
	 * @return A string containing the unquoted version of <i>str</i>.
	 */
	std::string StrUnquote (const std::string& str);
	
	/**
	 * Extracts the path from the given file name string.
	 * 
	 * @param fileName The name of the file, with its path.
	 * 
	 * @return A string containing the file path of the given file.
	 */
	std::string ExtractFilePath (const std::string& fileName);
	
	/**
	 * Extracts the given file path and splits the path into sub-directories. Sub-directories
	 * are stored in order of depth, e.g. if <i>path</i> is "./folder/sub/new/", <i>output</i>
	 * will be an array where the first element is ".", the second is "folder", third "sub" and
	 * fourth "new".
	 * 
	 * @param path A string containing the path to be split.
	 * @param output The variable in which the output path will be stored.
	 */
	void SplitPath (const std::string& path, StringArray& output);
	
	/**
	 * Attempts to create the specific path structure.
	 * 
	 * @param path The path which must exist after the function call.
	 * 
	 * @return true on success, false if the process does not have the required privileges.
	 */
	bool CreatePath (const std::string& path);
	
	/**
	 * Checks whether the given string array contains the specified string, and returns the index
	 * in the string array of the specified string.
	 * 
	 * @param arr The array through which to search.
	 * @param str The string for which to search.
	 * 
	 * @return An integer specifying the position in the array, or -1 if it does not exist.
	 */
	int StringArrayPos (const StringArray& arr, const std::string& str);
	
	/**
	 * Checks whether the given string is contained within the given StringIntArray. The indexes
	 * may not be negative.
	 * 
	 * @param arr The array through which to search.
	 * @param str The string for which to search.
	 * 
	 * @return An integer specifying the position in the array, or -1 if it does not exist.
	 */
	int StringIntArrayPos (const StringIntArray& arr, const std::string& str);
	
	/**
	 * Removes duplicate items from the given StringArray.
	 */
	void RemoveDuplicates (StringArray& arr);
	
	/**
	 * Splits a string into a string array by the given delimiter.
	 * 
	 * @param str The string to be split.
	 * @param delim The delimiter character.
	 * @param output The output array in which to store the result.
	 * 
	 * @return true on success, false on failure (parsing error).
	 */
	bool SplitString (const std::string& str, char delim, StringArray& output);
	
	/**
	 * Combines the given string array with the given delimiter.
	 */
	std::string JoinString (const StringArray& str, char delim);
	
	/**
	 * Removes whitespace and newlines from the given string.
	 */
	std::string StripWhitespace (const std::string& str);
	
	/**
	 * Computes the MD5 hash of the given file.
	 */
//	std::string ComputeMD5 (const std::string& fileName);
	
	/**
	 * Converts the given string array into a single string capable of being inserted into
	 * the database.
	 * 
	 * @param arr The array that must be converted.
	 * 
	 * @return A string containing the formatted version of the array.
	 */
	std::string FormatSqlArray (const StringArray& arr);
	
	/**
	 * Parses an input string (defining an SQL array) into a one-dimensional array of strings.
	 *
	 * @param input The input string defining the SQL array.
	 * @param output The output string array in which the strings making up the array must be stored.
	 * @param strict Whether or not to treat the input array strictly. Set to false if
	 * the parser must continue even if the array is multi-dimensional. Default is true.
	 *
	 * @return true on success, false on parsing error.
	 */
	bool ParseSqlArray (const std::string& input, StringArray& output, bool strict = true);
	
	/**
	 * Splits the given string into an array of lowercase words, skipping all special characters.
	 */
	void SplitWords (const std::string& str, StringArray& output);
	
	/**
	 * Returns a double value representing the percentage match between the given test
	 * string and given master string. 0 is no match, 1 is full match. Matching is
	 * case-insensitive.
	 * 
	 * @param testWords An array of words - the result of a splitting of a test string
	 * into its words (all in lowercase). This can be accomplished with the SplitWords
	 * function.
	 * @param masterWords The master string against which the test string will be compared,
	 * split into its component words by the SplitWords function.
	 * 
	 * @return A double value representing the percentage match between the two.
	 */
	double FuzzyStringMatch (const StringArray& testWords, const StringArray& masterWords);
	
	/**
	 * Performs a fuzzy logic string comparison between the given master set of strings
	 * and the test string. The return value will be the index in the master set of strings
	 * of the string that most closely matches the test string.
	 * 
	 * @param testStr The test string.
	 * @param masterSet The master set of strings against which testStr will be compared.
	 * 
	 * @return -1 if no strings match at all, or the index in the masterSet array of the most
	 * closely matching string.
	 */
	int FuzzyStringSetMatch (const std::string& testStr, const StringIntArray& masterSet);
	
	/**
	 * Extracts a value from the given string where the units of the value are specified by
	 * the units string. Assumes that the value is bounded by whitespace.
	 * 
	 * @param str The string through which to search.
	 * @param units The units of the value for which to search.
	 * @param before Whether the units are expected to appear before the value or not.
	 * 
	 * @return A string containing the value to be extracted.
	 */
	std::string FuzzyValueExtract (const std::string& str, const std::string& units, bool before);
	
	/**
	 * Returns whether or not the specified string contains a numeric value.
	 */
	bool IsNumeric (const std::string& str);
	
	/**
	 * Returns whether or not the specified string contains a pure keyword, with no special
	 * characters.
	 */
	bool IsKeyword (const std::string& str);
}

#endif
