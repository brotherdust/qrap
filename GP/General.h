/*
 *  General.h
 *
 *  Created by Peter Harrington on 6/28/11.
 *  Edited by Magdaleen Ballot 2018-02-13
 *
 *	This file contains general purpose tools and #includes.
 *
 */

#ifndef General_h
#define General_h

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <time.h>

//general debugging print macro
#ifndef   DEBUG
	#define   PRINTD(arg, ...)
#else
	#define   PRINTD(arg, ...)     printf(arg, ##__VA_ARGS__)
#endif

#endif
