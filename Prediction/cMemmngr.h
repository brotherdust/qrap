/*
 *    QRAP Project
 *
 *    Version     : 0.2
 *    Date        : 2008/03/05
 *    License     : GNU GPL
 *    File        : cMemmngr.h
 *    Description : Header file for memory management routines, particularly 2D arrays.
 *
 *    Revision History
 *    ----------------
 *    2008/03/05 - Coded the cMemmngr header.
 *                 (magdaleen, tuks)
 *    2008/03/05 - Added database integration and doxygen commentry.
 *                 (roeland, bluntchisel)
 *
 */

using namespace std;

#include <iostream>
#include <string.h>
#include <stdlib.h>


#ifndef Qrap_cMemmngr_h
#define Qrap_cMemmngr_h

/**
 * Decription of Double2DArray
 */
typedef double** Double2DArray;

/**
 * Decription of Floar2DArray
 */
typedef float** Float2DArray;


/**
 * Allocates memory for a 2D array.
 * 
 * @param rows Contain 0 if allocation failed 
 * @param cols Contain 0 if allocation failed.
 *
 * @return A Double2DArray.
 */
Double2DArray new_Double2DArray(unsigned rows=1 , unsigned cols=1);


/**
 * Deletes a 2D array.
 * 
 * @param array Description
 * 
 * @return TRUE if successful
 */
int delete_Double2DArray(Double2DArray &array);


/**
 * Copies a 2D array.
 * 
 * @param rows Contain 0 if allocation failed 
 * @param cols Contain 0 if allocation failed.
 * 
 * @return returns dest.
 */
Double2DArray copy_Double2DArray (Double2DArray &dest,
				  const Double2DArray source,
				  unsigned &rows, unsigned &cols);

/**
 * Sets each element of the array to the same (double) value.
 * 
 * @param dest Description
 * @param rows Description
 * @param cols Description
 * @param dconst Description
 */
void init_Double2DArray (Double2DArray &dest,
			 unsigned rows,
			 unsigned cols,
			 double dconst); 

/**
 * Allocates memory for a 2D array.
 * 
 * @param rows Contain 0 if allocation failed 
 * @param cols Contain 0 if allocation failed.
 *
 * @return A Float2DArray
 */
Float2DArray new_Float2DArray (unsigned rows=1, unsigned cols=1);

/**
 * Deletes a 2D array
 * 
 * @param arrap Description
 * 
 * @return TRUE if successful.
 */
int delete_Float2DArray (Float2DArray &array);


/**
 * Copies a 2D array and returns dest.
 *
 * @param rows Contain 0 if allocation failed 
 * @param cols Contain 0 if allocation failed.
 *
 * @return A Float2DArray.
 */
Float2DArray copy_Float2DArray (Float2DArray &dest,
			        const Float2DArray source,
			        unsigned &rows,
			        unsigned &cols);

/**
 * Sets each element of the array to the same (double) value.
 * 
 * @param dest Description
 * @param rows Description
 * @param cols Description
 * 
 * @return An integer.
 */
int init_Double2DArray(Double2DArray &dest, unsigned rows, unsigned cols); 

#endif
