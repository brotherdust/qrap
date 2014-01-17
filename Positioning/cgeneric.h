// File cgeneric.h
//
// Copyright Mikomtek, CSIR 1996
//
// Author: Martin Drewes& Miranda JvRensburg

#ifndef cgeneric_h
#define cgeneric_h 1

#define DEBUG_LEVEL 2

#define TRUE 1
#define FALSE 0
#define MAX(x,y) ((x>y) ? x:y)
#define MIN(x,y) ((x<y) ? x:y)
#define PI 3.1415926535897932384626433832795
#define RAD(x) ((x)*PI/180.0) 

// Make sure that DOS is defined for DOS platform (e.g. 'dir', etc.)
#if defined(MSDOS) || defined(__MSDOS__)
  #ifndef DOS
	 #define DOS
  #endif
#endif

#if defined(_Windows)||defined(_WINDOWS)||defined(DOS)
	#define Not_Unix
#endif


// required include files
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <new>
#include <limits>
#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <values.h>
#include <unistd.h>
#include <dirent.h>
#include <ctime>
#include <list>
#include <map>
#include <sstream>
#include <vector>
#include <cstddef>
#include <algorithm> 
#include <iterator>
#include <libpq-fe.h>
#include <proj_api.h>

using namespace std;
using std::cout;
// WinExec replacement for system call
/*#if defined(_Windows) || defined(_WINDOWS)
  #if !(( defined(system) )||( defined(WinExec) ))
	 extern "C" {
	   unsigned int _far _pascal WinExec(const char _far*, unsigned int);
	   }
	 #define SW_SHOWNORMAL 	1
	 #define system(commandline) WinExec(commandline,SW_SHOWNORMAL)
  #endif
#endif
*/
#ifdef Not_Unix
	#include <dir.h>
	#define ANTSIZE MAXFILE+MAXEXT+1   // size of antenna filename??
	#define DIRSIZE MAXPATH-ANTSIZE	// size of directory
	#define MAXDOUBLE 3.402823e+38
#else
	#include <sys/types.h>
	#include <sys/dir.h>
	#define ANTSIZE 13
	#define DIRSIZE 100
	#define MAXPATH 114
#endif
#define NAMESIZE 80

extern FILE *fpreturn;   // filepointer for return.res

#if defined(Not_Unix)
	#define MAX(x,y)  (((x)>(y))?(x):(y))
	#define MIN(x,y)  (((x)<(y))?(x):(y))
#endif


#ifndef DEBUG_LEVEL
  #define DEBUG_LEVEL 0
#endif


#ifndef NO_DEBUG
  #define trace(err_msg) cout << err_msg << endl;
  #if DEBUG_LEVEL<1
	 #define trace1(err_msg) cout << err_msg << endl;
  #else
	 #define trace1(err_msg)
  #endif
  #if DEBUG_LEVEL<2
	 #define trace2(err_msg) cout << err_msg << endl;
  #else
	 #define trace2(err_msg)
  #endif
#else
  #define trace(err_msg)
  #define trace1(err_msg)
  #define trace2(err_msg)
#endif

#define BUFSIZE 8192

typedef int Counter;  // typedef for loop counter variables
typedef char *charp; 

//## Class: CGenericBaseClass
//	This class is the base class from which almost all other classes are
// derived.  It contains code for error and debugging messages, as well as
// the write-back code from the engine to the GIS
class CGenericBaseClass
{
	public:

		int error(const char* err_mesg) const;
		// prints an error message on the standard output

		//int trace(const char* err_mesg);				// kr 01/07/96  Made trace a macro for efficiency
		// prints a debugging string to the standard output]

		// int trace(const char* err_mesg, int level); 	// kr 01/07/96  Made trace a macro for efficiency
		// prints a debugging string to the standard output is the level
		// specified exceeds the level #defined during compilation
		char * error_handler(int error, const char * string);

		//char message_returned[90];	// kr 30/06/96 - moved to local in cgeneric.cpp
};

#ifndef _MSC_VER			// KR 29/06/96 for msvc compilation
void memory_err();		// memory allocation error handler
#else
int memory_err(size_t);
#endif



void delete_project ( char * name );
// routine to delete a project and all its associated files

void write_return_file(int option, const char * message);


// Classes for streaming object in binary form to and from files
// This class is used for all binary file io to identify in the objects
class bin_fstream : public fstream
{
};

#endif
