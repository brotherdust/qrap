 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBase.cpp
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

#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <errno.h>
#include <math.h>
//#include <openssl/md5.h>

#include <Poco/RegularExpression.h>

#include "Config.h"
#include "cBase.h"

/*
 * Explicitly define the classes we're using from each namespace.
 */
using std::string;
using namespace Qrap;

/*
 * Static member definition
 */

// Initialise the default log file name and location (static member)
string        cBase::msLogFileName("/var/log/gisrap.log");
AlertCode     cBase::msAlertCode = acOk;
AlertCallback cBase::msAlertCallback = (Qrap::AlertCallback)QRAP_ALERT_DEFAULT_CALLBACK;
int           cBase::msAlertDepth = 0;



/*
 * cBase class implementation
 */
//************************************************************************
bool Qrap::Log (AlertLevel level, const string& message, AlertCode code, const string& funcName,
	const string& srcFile, int srcLine)
{
	//cout << "gLogFileName = " << cBase::gLogFileName << endl;
	
	// open the log file for appending
	ofstream   logFile(cBase::msLogFileName.c_str(), std::ios::app);
	// the string that will be sent to the output
	string     logStr, indent;
	// get the current time
	time_t     nowTime = time(NULL);
	struct tm* now     = localtime(&nowTime);
	
	
	// start off optimistically
	cBase::msAlertCode = acOk;
	
	// have we opened it successfully?
	if (!logFile.is_open())
		cBase::msAlertCode = acFileOpenWrite;
	else
	{
		// set up the output line...
		// set the fill character for numbers
		logFile.fill('0');
	
		// output it all
		logFile << "[" << now->tm_year+1900 << "-" << setw(2) << now->tm_mon+1 << "-"
			<< setw(2) << now->tm_mday << " "
			<< setw(2) << now->tm_hour << ":"
			<< setw(2) << now->tm_min << ":"
			<< setw(2) << now->tm_sec << "] ";
		switch (level)
		{
		case alWarning:
			logFile << "< WARN  > ";
			break;
		case alError:
			cBase::msAlertDepth++;
			logFile << "< ERROR > ";
			break;
		case alFatal:
			cBase::msAlertDepth++;
			logFile << "< FATAL > ";
			break;
		default:
			logFile << "< INFO  > ";
			break;
		}
		
		indent.append(32, ' ');
		
		logFile << message << endl;
		// logging level check
		if (level > alInfo)
			logFile  << indent << "From: " << funcName << ", in " << srcFile <<
			", line " << srcLine << endl;
		
		// call the callback function
		if (cBase::msAlertCallback != NULL)
			cBase::msAlertCallback(level, message);
	}
	
	// file will be closed automatically upon logFile destruction
	
	bool success = (cBase::msAlertCode == acOk);
	cBase::msAlertCode = code;
	return success;
}

//*******************************************************************
cBase::cBase ()
{
	// get the current time
	time_t         nowTime = time(NULL);
	struct tm*     now     = localtime(&nowTime);
	struct timeval tv;
	
	// get the high-res portion of the time
	gettimeofday(&tv, NULL);
	
	// seed the random number generator
	srand((unsigned int)((now->tm_year+now->tm_mon-now->tm_mday+now->tm_hour+
		now->tm_min-now->tm_sec)*tv.tv_usec));
}

//*********************************************************************
int Qrap::FindNonWhitespace (const string& str, unsigned start, bool skipSpecial)
{
	int  i = start, len = str.length();
	char ch;
	
	while (i < len)
	{
		if (skipSpecial)
		{
			// if it's not an alphanumeric character (keeping the minus character)
			ch = tolower(str[i]);
			if ((((ch < 'a') || (ch > 'z')) && ((ch < '0') || (ch > '9'))) && (ch != '-'))
			{
				i++;
				continue;
			}
		}
		
		if (str[i] != ' ')
			break;
		i++;
	}
	
	return i;
}

//***********************************************************************
string Qrap::ExtractLiteral (const string& str, int start, bool fullStopDelim)
{
	int       i = start, len = str.length(), escapeCount = 0;
	string    output;
	bool      inString = false, inBrackets = false;
	char      stringChar = '\0';
	AlertCode alertCode;
	
	// start optimistically
	alertCode = acOk;
	
	while (i < len)
	{
		// if we've hit an escape character
		if (str[i] == '\\')
		{
			// count it
			escapeCount++;
			// append it to the output string
			output += str[i];
		} else
		{
			// if we've hit a string character and we're not in a parenthesised expression
			if (((str[i] == '\'') || (str[i] == '"')) && (!inBrackets))
			{
				// if we have an even number of escape characters before this string character
				if ((escapeCount % 2) == 0)
				{
					output += str[i];
					// it must mean that this is either the beginning or the end of
					// a string
					if (inString)
					{
						// if this character is not the same as the one that started
						// the string, it's just part of the literal
						if (str[i] == stringChar)
						{
							// this must be the end of the string
							inString = false;
							break;
						}
						// otherwise, on we go...
					} else
					{
						// otherwise we're starting a string
						inString = true;
						stringChar = str[i];
					}
				} else
				{
					// otherwise it's just a plain old literal character within a string or brackets
					output += str[i];
				}
			} else
			// if we've hit whitespace or a delimiter, and we're not within a string or within brackets
			if (((str[i] == ' ') || (str[i] == ',') || (str[i] == ';') || (str[i] == '}') ||
				(str[i] == '{') || (str[i] == ':') || ((fullStopDelim) && (str[i] == '.'))) && (!inBrackets) && (!inString))
			{
				// it means we're finished with a literal expression, and we
				// don't want the space character
				break;
			} else
			// if we've found a bracket, and we're not already within brackets and we're not within a string
			if ((str[i] == '(') && (!inBrackets) && (!inString))
			{
				// add the current character to the string
				output += str[i];
				// we're now in brackets
				inBrackets = true;
			} else
			// if we've found a closing bracket, and we're in brackets and not within a string
			if ((str[i] == ')') && (inBrackets) && (!inString))
			{
				// add the closing bracket to the string
				output += str[i];
				// we're outta here
				inBrackets = false;
				break;
			} else
			// otherwise, it's just some other old character
			{
				output += str[i];
			}
			escapeCount = 0;
		}
		
		i++;
	}
	
	// if we're still in a string or brackets, there's a problem
	if (inString || inBrackets)
	{
		alertCode = acParse;
	}
	
	return (alertCode == acOk) ? output : "";
}


//**********************************************************************
string Qrap::ExtractKeyword (const string& str, int start)
{
	int    i = start, len = str.length();
	string output;
	
	while (i < len)
	{
		// only accept alphanumeric characters
		if (((str[i] >= 'a') && (str[i] <= 'z')) ||
		    ((str[i] >= 'A') && (str[i] <= 'Z')) ||
		    ((str[i] >= '0') && (str[i] <= '9')) ||
		    (str[i] == '_'))
		{
			output += tolower(str[i]);
		} else
			break;
		i++;
	}
	
	return output;
}

//***************************************************************
string Qrap::ExtractOperator (const string& str, int start)
{
	int    i = start, len = str.length();
	string output;
	bool   nonAlpha = false;
	
	while (i < len)
	{
		// if we've started with non-alphanumeric characters
		if (((output.length() == 0) || nonAlpha) &&
		    ((str[i] == '=') || (str[i] == '>') || (str[i] == '<') || (str[i] == '!') || (str[i] == '@')))
		{
			nonAlpha = true;
			output += str[i];
		} else
		if (((str[i] >= 'a') && (str[i] <= 'z')) ||
		    ((str[i] >= 'A') && (str[i] <= 'Z')) ||
		    ((str[i] >= '0') && (str[i] <= '9')))
		{
			// if we're receiving a non-alphanumeric operator, but now we've hit an
			// alphanumeric character... exit
			if (nonAlpha) break;
			
			output += tolower(str[i]);
		} else
			break;
		
		i++;
	}
	
	return output;
}

//********************************************************************
// converts the given string into lowercase
string Qrap::StrToLower (const string& str)
{
	int    i, len = str.length();
	string temp;
	
	// run through the string and convert it to lowercase
	for (i=0;i<len;i++)
	{
		temp += (char)tolower((int)str[i]);
	}
	
	return temp;
}

//********************************************************************
// if the given string is not surrounded by apostrophes, put them there
// and make sure that every other quote character is escaped
string Qrap::StrQuote (const string& str, char strChar)
{
	int    i, len = str.length();
// 	int    slashcount = 0;
	string temp;
	bool   alreadyInString = false;

	if ((str[0] == strChar) && (str[len-1] == strChar))
	{
		alreadyInString = true;
		i = 1;
	} else
		i = 0;

	// first run through the string and check for the backslashes,
	// assuming that a double-backslash represents an escaped backslash character
	for (;i<len;i++)
	{
		if (((i == (len-1)) && alreadyInString) && (str[i] == strChar))
			break;

		if (str[i] == '\\')
		{
			// escape the slash
			temp += '\\';
			//slashcount++;
		} else
		// if we've found the first character that isn't a backslash
		// but there were backslashes directly before it
		/*if (slashcount > 0)
		{
			// if it's an odd number of slashes, it must mean that the current
			// character is an escaped character
			if ((slashcount % 2) == 0)
			{
				// otherwise, this character is not escaped, but there are a
				// series of backslashes behind it which do not apply to it
				if ((str[i] == '\'') || (str[i] == '"'))
				{
					// if it's a string specifier, escape it
					temp += '\\';
				}
			}
			
			// reset the slash counter
			slashcount = 0;
		} else*/
		// if this character is a quotation mark, but has no slashes before it
		if (str[i] == strChar)
		{
			temp += '\\';
		}
		
		// append this character to the temporary string
		temp += str[i];
	}
	
	return strChar+temp+strChar;
}

//**********************************************************
string Qrap::StrUnquote (const string& str)
{
	string ret;
	int    i = 0, len = str.length(), escapeCount = 0;
	bool   inString = false;
	char   strChar = '\0';
	
	// run through the string, converting double escapes to single ones and removing single ones
	// where they are followed by an apostrophe
	while (i < len)
	{
		if (str[i] == '\\')
		{
			escapeCount++;
			// if we've found two escape characters, replace them with a single one
			if (escapeCount == 2)
			{
				ret += "\\";
				// reset the escape counter
				escapeCount = 0;
			}
		} else
		{
			// if we've hit some form of string character
			if ((str[i] == '\'') || (str[i] == '\"'))
			{
				// if there's an even number of escape characters before this character
				if ((escapeCount % 2) == 0)
				{
					// if we're already within the string
					if (inString)
					{
						// if this is the equivalent of the opening string character
						if (strChar == str[i])
						{
							// we've found the end of the literal
							inString = false;
							break;
						}
						// otherwise, it's just an internal character
					} else
					// otherwise, the fun's just starting
					{
						inString = true;
						strChar = str[i];
					}
				} else
				// otherwise, just simply add this string character to the output
				{
					ret += str[i];
				}
			} else if (!inString)
			{
				// if we haven't reached a string yet, it must mean that this string
				// contains both normal strings and other text - ignore it
				return str;
			} else
			// otherwise, it's just some other random character
				ret += str[i];
			
			// reset the escape counter
			escapeCount = 0;
		}
		
		i++;
	}
	
	return ret;
}

//********************************************************************
string Qrap::ExtractFilePath (const string& fileName)
{
	StringArray pathArr;
	string      path;
	int         i, count;
	
	SplitPath(fileName, pathArr);
	// drop off the last one (the file name)
	count = pathArr.size()-1;
	for (i=0;i<count;i++)
		path += pathArr[i]+"/";
	
	return path;
}

//**********************************************************************
void Qrap::SplitPath (const string& path, StringArray& output)
{
	int    i, len = path.length();
	string curDir;
	
	for (i=0;i<len;i++)
	{
		if (path[i] != '/')
		{
			curDir += path[i];
		} else
		// otherwise, if it is a slash
		{
			// append it to the output array
			output.push_back(curDir);
			// reset the variable
			curDir = "";
		}
	}
	
	// if there's any remaining info in the curDir variable
	if (curDir.length() > 0)
		output.push_back(curDir);
}

//********************************************************************
bool Qrap::CreatePath (const string& path)
{
	StringArray pathArr;
	int         i, count;
	DIR*        dir;
	string      curPath;
	
	// first split the path
	SplitPath(path, pathArr);
	
	// now run through each directory and attempt to create it if it does not exist
	count = pathArr.size();
	for (i=0;i<count;i++)
	{
		// if this is not referring to the root directory
		if (pathArr[i].length() > 0)
		{
			curPath += pathArr[i];
			
			if ((dir = opendir(curPath.c_str())) == NULL)
			{
				// if the directory does not exist
				if (errno == ENOENT)
				{
					// attempt to create it
					if (mkdir(curPath.c_str(), 0755) == -1)
						return false;
				} else
					// we probably don't have access to it
					return false;
			} else
				closedir(dir);
		}
		
		// append a slash
		curPath += "/";
	}
	
	return true;
}

//*********************************************************************
int Qrap::StringArrayPos (const StringArray& arr, const string& str)
{
	int i, count = arr.size();
	
	for (i=0;i<count;i++)
		if (arr[i] == str)
			return i;
	
	return -1;
}

//**************************************************************************
void Qrap::RemoveDuplicates (StringArray& arr)
{
	StringArray temp;
	int         i, count = arr.size();
	
	for (i=0;i<count;i++)
	{
		// if this item does not yet exist in the temporary array, add it
		if (StringArrayPos(temp, arr[i]) == -1)
			temp.push_back(arr[i]);
	}
	
	// copy the temporary array out
	arr = temp;
}

//**********************************************************************
bool Qrap::SplitString (const string& str, char delim, StringArray& output)
{
	int    i, len = str.length();
	string temp;
	bool   inString = false;
	char   strChar = '\0';
	
	output.clear();
	
	for (i=0;i<len;i++)
	{
		if ((str[i] == '\'') || (str[i] == '"'))
		{
			/*if (inString && (i < (len-1)) && (str[i+1] != delim))
			{
				cout << "1" << endl;
				return false;
			}*/
			inString = !inString;
			strChar = (inString) ? str[i] : '\0';
		} else
		if (str[i] != delim)
			temp += str[i];
		else
		{
			if (inString)
				temp += str[i];
			else
			{
				output.push_back(temp);
				temp = "";
			}
		}
	}
	
	// parsing error
	if (inString)
	{
		return false;
	}
	
	if (temp.length() > 0)
		output.push_back(temp);
	// if the last character in the string is a delimiter, it means there's an empty final entry
	if (str[len-1] == delim)
		output.push_back("");
	
	return true;
}

//************************************************************************
string IntToHex (int val, unsigned digits = 2)
{
	string      ret;
	int         quo, rem;
	static char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	
	while (val != 0)
	{
		quo = val/16;
		rem = val % 16;
		// convert the remainder to a hex character
		ret.insert(0, 1, hexChars[rem]);
		val = quo;
	}
	
	if (ret.length() < digits)
		ret.insert(0, digits-ret.length(), hexChars[0]);
	
	return ret;
}

//****************************************************************
/*string Qrap::ComputeMD5 (const string& fileName)
{
	FILE*         fp;
	unsigned char buf[65536], hash[16];
	string        ret;
	MD5_CTX c;
	int     br, i;
	
	fp = fopen(fileName.c_str(), "rb");
	if (!fp)
		return "";
	
	// initialise the context
	MD5_Init(&c);
	
	// run through the file, computing the MD5 hash
	while (!feof(fp))
	{
		// read a chunk of data
		br = fread(buf, sizeof(unsigned char), 65536, fp);
		MD5_Update(&c, buf, br);
	}
	
	// extract the hash
	MD5_Final(hash, &c);

	fclose(fp);
	
	// now convert the hash to hexadecimal
	for (i=0;i<16;i++)
	{
		ret += IntToHex((int)hash[i]);
	}
	
	return ret;
}
*/
//********************************************************************
string Qrap::FormatSqlArray (const StringArray& arr)
{
	string ret;
	int    i, count = arr.size();
	
	ret = "{";
	
	for (i=0;i<count;i++)
	{
		ret += arr[i];
		if ((i+1) < count)
			ret += ",";
	}
	
	ret += "}";
	
	return ret;
}

//*************************************************************************
bool Qrap::ParseSqlArray (const string& input, StringArray& output, bool strict)
{
	string temp, literal;
	int    i = 0, len = input.length(), braceDepth = 0;
	
	output.clear();
	
	while (i < len)
	{
		// skip ahead to any non-whitespace area
		i = FindNonWhitespace(input, i);
		// check if there's a brace
		if (input[i] == '{')
		{
			braceDepth++;
			if ((braceDepth > 1) && strict)
			{
				return false;
			}
			
			if (braceDepth > 1)
			{
				bool inString = false;
				char stringChar = '\0';
				int  escapeCount = 0;
				
				temp += input[i];
				i++;
				
				// extract everything up to and including the closing brace
				while (i < len)
				{
					temp += input[i];
					if ((input[i] == '}') && (!inString))
					{
						braceDepth--;
						// if we're back to normal brace depth again
						if (braceDepth < 2)
						{
							i++;
							break;
						}
					} else
					if ((input[i] == '{') && (!inString))
					{
						braceDepth++;
					} else
					if (input[i] == '\\')
					{
						escapeCount++;
					} else
					if (((input[i] == '\'') || (input[i] == '\"')) && ((escapeCount % 2) == 0))
					{
						if (inString)
						{
							// if we have an odd number of escape characters before this one
							if ((input[i] == stringChar) && ((escapeCount % 2) == 0))
							{
								inString = false;
								stringChar = '\0';
							}
						} else
						{
							inString = true;
							stringChar = input[i];
						}
					}
					
					if (input[i] != '\\')
						escapeCount = 0;
					
					i++;
				}
				
				if (temp.length() > 0)
				{
					output.push_back(StrUnquote(temp));
					temp = "";
				}
			} else
				i++;
		} else
		// if we've reached the end of the array
		if (input[i] == '}')
		{
			braceDepth--;
			i++;
		} else
		// if it's some form of separator
		if (input[i] == ',')
		{
			// next character
			i++;
		} else
		// extract from this point on as a literal value (string, integer or non-spaced value)
		{
			literal = ExtractLiteral(input, i);
			// skip past the literal
			i += literal.length();
			temp += literal;
// 			cout << "[literal] " << literal << endl;
			output.push_back(StrUnquote(temp));
			temp = "";
		}
	}
	
	if (braceDepth > 0)
	{
		return false;
	}
	// flush any leftover data
	if (temp.length() > 0)
		output.push_back(temp);
	
	return true;
}

//***************************************************************
string Qrap::JoinString (const StringArray& str, char delim)
{
	string ret;
	int    i, count = str.size();
	
	for (i=0;i<count;i++)
	{
		ret += str[i];
		if ((i+1) < count)
			ret += delim;
	}
	
	return ret;
}

//**********************************************************************
string Qrap::StripWhitespace (const string& str)
{
	string ret;
	int    i, len = str.length(), spaceCount = 0;
	
	for (i=0;i<len;i++)
	{
		if (str[i] == ' ')
		{
			spaceCount++;
			// only append one single space character
			if (spaceCount == 1)
			{
				ret += ' ';
			}
			continue;
		}
		
		spaceCount = 0;
		if ((str[i] != '\n') && (str[i] != '\r') && (str[i] != '\t'))
			ret += str[i];
	}
	
	return ret;
}

//*******************************************************************
void Qrap::ConsoleAlert (AlertLevel level, const std::string& msg)
{
	switch (level)
	{
	case alWarning:
		std::cout << "[QRAP-WARNING] ";
		break;
	case alError:
		std::cout << "[ QRAP-ERROR ] ";
		break;
	case alFatal:
		std::cout << "[ QRAP-FATAL ] ";
		break;
	default:
		std::cout << "[ QRAP-INFO  ] ";
		break;
	}
	
	std::cout << msg << std::endl;
}

//**********************************************************************
void Qrap::SplitWords (const string& str, StringArray& output)
{
	int i = FindNonWhitespace(str, 0, true), len = str.length(), outSize = 0;
	
	output.clear();
	
	while (i < len)
	{
		output.push_back(ExtractKeyword(str, i));
		// skip past the new keyword
		i += output[outSize].length();
		outSize++;
		// find the next keyword
		i = FindNonWhitespace(str, i, true);
	}
}

//**********************************************************************8
double Qrap::FuzzyStringMatch (const StringArray& testWords, const StringArray& masterWords)
{
	double wordMatch;
	int    i, j, count, testCount = testWords.size(), testLen, masterLen, matches;
	
	// run through the master string
	count = masterWords.size();
	for (i=0;i<count;i++)
	{
		// no use in comparing to nothing
		if (i >= testCount) break;
		
		// run through the test string's word
		testLen = testWords[i].length();
		masterLen = masterWords[i].length();
		matches = 0;
		for (j=0;j<testLen;j++)
		{
			if (j >= masterLen) break;
			if (testWords[i][j] == masterWords[i][j])
				matches++;
		}
		wordMatch += ((double)matches)/((double)j);
	}
	
	if (i > 0)
		return wordMatch / ((double)i);
	else
		return 0.0;
}

//********************************************************************8
int Qrap::FuzzyStringSetMatch (const string& testStr, const StringIntArray& masterSet)
{
	StringArray testWords, masterWords;
	double      score, highScore = 0.0;
	int         highest = -1;
	
	
	// split the test string
	SplitWords(testStr, testWords);
	// run through the master strings
	for (StringIntArray::const_iterator it=masterSet.begin(); it != masterSet.end(); it++)
	{
		// split the master string
		SplitWords(it->second, masterWords);
		// check the score
		score = FuzzyStringMatch(testWords, masterWords);
		if (score > highScore)
		{
			highScore = score;
			highest = it->first;
		}
	}
	
	return highest;
}

//***********************************************************************
string Qrap::FuzzyValueExtract (const string& str, const string& units, bool before)
{
	string exp, val;
	
	exp = "[0-9](([0-9])*[0-9,\\.]([0-9])*)*";
	if (before)
		exp = units+"(\\ )*"+exp;
	else
		exp = exp+"(\\ )*"+units;
		
	Poco::RegularExpression regexp(exp);
	
	regexp.extract(str, val);
	
	// now just extract the value itself
	exp = "[0-9](([0-9])*[0-9,\\.]([0-9])*)*";
	Poco::RegularExpression regexp2(exp);
	regexp2.extract(val, val);
	return val;
}


//************************************************************************
int Qrap::StringIntArrayPos (const StringIntArray& arr, const string& str)
{
	for (StringIntArray::const_iterator it=arr.begin(); it != arr.end(); it++)
	{
		if (it->second == str)
			return it->first;
	}
	return -1;
}

//*********************************************************************
bool Qrap::IsNumeric (const string& str)
{
	int i, len = str.length();
	
	for (i=0;i<len;i++)
	{
		if (((str[i] < '0') || (str[i] > '9')) && (str[i] != '.'))
			return false;
	}
	
	// assume it's numeric
	return true;
}

//******************************************************************
bool Qrap::IsKeyword (const string& str)
{
	int  i, len = str.length();
	char ch;
	
	// if there's nothing in here, it's not a keyword
	if (!len) return false;
	
	for (i=0;i<len;i++)
	{
		ch = tolower(str[i]);
		// if it's not alphanumeric
		if (((ch < 'a') || (ch > 'z')) && ((ch < '0') || (ch > '9')) && (ch != '_'))
			return false;
	}
	
	return true;
}
