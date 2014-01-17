// file cgeneric.cpp
//
// Copyright Mikomtek, CSIR 1996
//
// Authors:	Martin Drewes (debugging stuff)
//              Miranda J v Rensburg (the rest)
// Date:        April 1996

#define NO_DEBUG 1
#if defined(_Windows) || defined(_WINDOWS)
#include <windows.h>
#endif
#include "cgeneric.h"

FILE *fpreturn;	// filepointer for return.txt
			    // kr 30/06/96 Moved from main.cpp

char *message_returned ;		// kr 30/06/96 moved from CGenericBaseClass definition
								// Changed from message_returned[90] to *message_returned
								//  for function to return pointer.  Added malloc to boot
								//  function

//char CGenericBaseClass::message_returned[90] ;

/**************************************************************************
@mfunc error indicator function

@rdesc FALSE if completed successfully

**************************************************************************/

int CGenericBaseClass::error(const char* err_mesg) const
{

// MMD removed NO_DEBUG define 20.09.97
//#ifndef NO_DEBUG
//   char ch;
   static ofstream errlog("error.log");
   cout << err_mesg;
   cout << endl;
   errlog << err_mesg << endl;
   //cin >> ch;

//#endif
   return(FALSE);
}

/**************************************************************************
@mfunc debugging function

@rdesc FALSE if completed successfully

**************************************************************************/
/*
int CGenericBaseClass::trace(const char* err_mesg)
{
   #ifndef NO_DEBUG
   cout << err_mesg;
   cout << endl;
   #endif
   return(FALSE);
}

// *************************************************************************
int CGenericBaseClass::trace(const char* err_mesg, int level)
{
   #ifndef DEBUG_LEVEL
   #define DEBUG_LEVEL 0
   #endif
   #ifndef NO_DEBUG
   if (level > DEBUG_LEVEL)   // only print trace statements higher than a certain level
   {
   cout << err_mesg;
   cout << endl;
   }
   #endif
   return(FALSE);
}
*/

/**************************************************************************
@func error indicator function

@rdesc FALSE if completed successfully

**************************************************************************/

#ifndef _MSC_VER			// KR 29/06/96 for msvc compilation
void memory_err()
#else
int memory_err(size_t size)
#endif
{
	cout << "This is your friendly memory sentry:  A memory allocation error ocurred. " << endl;
	#ifdef _MSC_VER
	cout << "Tried to allocate " << size << " bytes" << endl;	// kr 30/06/96.
	#endif
	write_return_file( 2, "memory allocation error");
}

/**************************************************************************
@func  function to delete projects
**************************************************************************/
void delete_project ( char * name )
{

  char *messagestring;
  int i = strlen(name);

	messagestring = new char[i+10];
	#if defined(DOS)
	strcpy(messagestring,"del ");
	#else
	strcpy(messagestring,"rm ");
	#endif

	strcat(messagestring,name);
	strcat(messagestring,".*");
	system(messagestring);
	delete [] messagestring;
}


/**************************************************************************
@func writes the .raw and binary file for Genamap

**************************************************************************/
void write_return_file(int option, const char * message)
{


   fprintf(fpreturn,"%d \n",option);

//   if (option == 2)
   fprintf(fpreturn,"%s \n",message);
   cout << option << endl << message << endl;

   if (option==0) return;

   fclose(fpreturn);
   exit(0);
}


/**************************************************************************
@mfunc writes the string containing the error message

@rdesc error message
**************************************************************************/
char * CGenericBaseClass::error_handler(int error, const char * string)
{


   switch(error)
   {
   case 1:
       strcpy(message_returned,"cannot open ");
       strcat(message_returned,string);
      break;
   case 2:
		 strcpy(message_returned,
		 "rap 0 inputfilename outputfilename (if predictions) \n"
		 "rap 1 (if cancel) \n"
		 "rap 2 projectname (delete project)");		// KR 29/06/96 add \n
		break;
	case 3:
		 strcpy(message_returned,"invalid input parameter: ");
		 strcat(message_returned,string);
		break;
	case 4:
		 strcpy(message_returned,"cannot find ");
		 strcat(message_returned,string);
		break;

   default:
   		 strcpy(message_returned,"Don't know how to handler error type. "); // kr 30/06/96
   		 strcat(message_returned,string);

      break;
   }
  return (message_returned);
}



/**************************************************************************
@mfunc Setup memory handler by default.  Does this before execution of main
	   because instance is created and constructor does setup.

**************************************************************************/
class CBootByDefault
{
	public:
	CBootByDefault()
	{
		// Set up memory handler for 'new' function
		#ifndef _MSC_VER
		set_new_handler(memory_err);
		#else
		_set_new_handler(memory_err);
		#endif

		// Allocate for message_returned variable
		message_returned = new char[255];

	}
	~CBootByDefault()
   {
   	delete [] message_returned;
   }   
};

CBootByDefault InstanceOfCBootByDefault;

