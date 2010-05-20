/*
 *    QRAP Project
 *
 *    Version     : 0.2
 *    Date        : 2008/03/05
 *    License     : GNU GPL
 *    File        : cMemmngr.cpp
 *    Description : Source file for memory management routines, particularly 2D arrays.
 *
 *    Revision History
 *    ----------------
 *    2008/03/05 - Coded the cMemmngr source.
 *                 (magdaleen, tuks)
 *    2008/03/05 - Added database integration and doxygen commentry.
 *                 (roeland, bluntchisel)
 *
 */


#include <iostream>
#include <string.h>

// Include local headers
#include "cMemmngr.h"
using namespace std;
//extern ofstream er("memchk.txt");
/**************************************************************************
@func allocates memory for a raster


@rdesc raster

**************************************************************************/
// allocates memory for a 2D array; rows and cols contain 0 if allocation failed
Double2DArray new_Double2DArray(unsigned rows, unsigned cols)
{
//	er << "new " << rows << " " << cols;
  	double **index_array;   // index array
	#ifndef DOS
	double *data_array;   // 'long' array
	#endif

	unsigned i;

	if ((rows == 0) || (cols == 0))
	{
		cout << "[MemMngr.cpp] new_Double2DArray was requested to allocate 0 memory!";
		exit(1);
	}

	#if defined(DOS)
	unsigned j;
	// kr 01/07/96 Added lines between #if and #else
	index_array = new double*[rows+1];     // +1 to allow for last on = NULL
//        er << index_array << "              " << "index array; new double* of " << rows+1 << endl;
	if (index_array == NULL)
	{
      		rows = 0;
      		cols = 0;
      		cout << "[MemMngr.cpp] Error allocating array of ";
      		cout << rows << " double*'s for Double2DArray.";
      		cout << endl;
      		return (NULL);
   	}

   	index_array[rows] = NULL;	// Indication of end of array

   	for (i=0; i<rows; i++)
   	{
		index_array[i] = new double[cols];
 //               er << index_array[i] <<
 //               "            " << "index array[i] " << i <<
 //               " of " << cols << endl;
	   	if (index_array[i] == NULL)
	   	{
//			er << "Free the memory already allocated!" << endl;

			for (j=0; j<i; j++)
			{
//				 er << "               " << index_array[j] << " index array j "
             << j << endl;
				 delete [] index_array[j];
                        }
	      		rows = 0;
	      		cols = 0;
	      		cout << "[MemMngr.cpp] Error allocating array of ";
	      		cout << rows << " by " << cols << "doubles for Double2DArray.";
	      		cout << endl;
	      		return (NULL);
	   	}
   	}


   #else
   	cout << "Mem:" << rows << " RxC " << cols << endl;
   data_array = new double[rows*cols];
//      er << data_array << "             " << "data_array of " << rows*cols;
   	if (data_array == NULL)
   	{
      	rows = 0;
		cols = 0;
      	cout << "[MemMngr.cpp] Error allocating array of ";
      	cout << rows << " by " << cols << "doubles for Double2DArray.";
      	cout << endl;
      	return (NULL);
   	}


   	index_array = new double*[rows];
//   	er << index_array << "             " << "index array of double* of "
//      	<< rows << endl;
   	if (index_array == NULL)
   	{
//	   	er << "           " << data_array << "data array" << endl;
	    delete [] data_array;
//      delete data_array; //MMD 31/05
	    rows = 0;
      	cols = 0;
	    cout << "[MemMngr.cpp] Error allocating array of ";
      	cout << rows << " double*'s for Double2DArray.";
        cout << cols;
	    cout << endl;
      	return (NULL);
	}

   	for (i=0; i<rows; i++)
   	{
       index_array[i] = &(data_array[i*cols]);
//     er << "index_array[" << i << "] " << index_array[i] << endl;
   	}
   	#endif

 //  cout << " at " << index_array << endl;
   	return(index_array);
}



/**************************************************************************
@func function to delete a 2D array of doubles
@rdesc TRUE if completed successfully

**************************************************************************/

// deletes a 2D array; returns TRUE if successful
int delete_Double2DArray(Double2DArray &array)
{
//	cout << "delete " << array << endl;
   if (array == NULL)  //safety check
   {
      cout << "[MemMngr.cpp] Error de-allocating array:";
      cout << " pointer was not initialised.";
      cout << endl;
      return (false);
   }

	#if defined(DOS)
	// kr 01/07/96 Added code for DOS model
	unsigned i;
	i=0;
	while (array[i]!=NULL)
	{
//	        er << "             " << array[i] << " array[i] " << i << endl;
	  	delete [] array[i];
	  	i++;
	}
//        er << "             " << array << " array!"<<endl;  
   	delete [] array;
   
	   #else
   
//   delete array[0];    // delete the 'long' array
//	er << "              " << array[0] << " array[0]" << endl;
//	        er << "              " << array << " array!" << endl;
   delete [] array[0];    // delete the 'long' array ?????????
   delete [] array;       // delete the index array

  	 #endif

   array = NULL;		// kr 30/06/96

   return (true);
}

/**************************************************************************
@func function to copy a 2D array of doubles

@rdesc raster

*************************************************************************/
// copies a 2D array and returns dest; rows and cols contain 0 if allocation failed
Double2DArray copy_Double2DArray(Double2DArray &dest, const Double2DArray source, unsigned &rows, unsigned &cols)
{
	#if !(defined(DOS))
	double *data_array;   // new 'long' array
	#endif

	unsigned i;

	if ((rows == 0) || (cols == 0))
	{
		cout << "[MemMngr.cpp] copy_Double2DArray cannot copy 0 elements!";
		exit(1);
	}

	if (source == NULL)
	{
		cout << "[MemMngr.cpp] Error copying array:";
		cout << " source array was not initialised.";
		cout << endl;
		rows = 0;
		cols = 0;
		return (NULL);
	}

	if (dest != NULL)  //safety check
							// kr 30/06/96 What if dest is not initialised and therefore != NULL ???
	{
		delete_Double2DArray(dest);
	}


	#if defined(DOS)
	// kr 01/07/96 Added code for DOS model
	dest = new_Double2DArray(rows,cols);
	for (i=0; i<rows; i++)
     // memcpy(dest[rows],source[rows], (cols*sizeof(double)));
		memcpy(dest[i],source[i], (cols*sizeof(double)));		// STUPID ERROR!!! - See above line
   	#else
	data_array = new double[rows*cols];
//        er << data_array << "             " << "data_array of " << rows*cols << endl;
	if (data_array == NULL)
	{
		rows = 0;
		cols = 0;
		cout << "[MemMngr.cpp] Error allocating array of ";
		cout << rows << " by " << cols << "doubles for copying.";
		cout << endl;
		return (NULL);
	}

	memcpy(data_array, source[0], (rows*cols*sizeof(double)));  // copy 'lon' array

	dest = new double*[rows];
//   er << dest << "               " << "dest of double* of " << rows << endl;
	if (dest == NULL)
	{
//   	er << "               " << data_array << "data array" << endl;
		delete [] data_array; //MMD 31/05/96
		rows = 0;
		cols = 0;
		cout << "[MemMngr.cpp] Error allocating array of ";
		cout << rows << " double*'s for copying.";
		cout << endl;
		return (NULL);
	}

	for (i=0; i<rows; i++)
	{
       dest[i] = &(data_array[i*cols]);
//       er << "dest[" << i << "] " << dest[i] << endl;
	}
   	#endif

	return ((Double2DArray) dest);
}

//***********************************************************************************
// Set each element of the array to the same (double) value
void init_Double2DArray(Double2DArray &dest, unsigned rows, unsigned cols, double dconst)
{
	unsigned irow, icol;
	for  ( irow = 0 ; irow < rows ; irow++ )
	for  ( icol = 0 ; icol < cols ; icol++ )
	{
		dest[ irow][ icol] = dconst ;
	}
}

/**************************************************************************
@func function to allocate memory for a raster of floats

@rdesc raster

**************************************************************************/
// allocates memory for a 2D array; rows and cols contain 0 if allocation failed
// kr 30/06/96 - What if array is already allocated ??? This can be handled if module is C++ based
Float2DArray new_Float2DArray(unsigned rows, unsigned cols)
{
//er << "Float arrays" << endl;
	float **index_array;   // index array
//	#if !(defined(DOS))
	float *data_array;   // 'long' array
//	#endif

   unsigned i;

	if ((rows == 0) || (cols == 0))
   {
      cout << "[MemMngr.cpp] new_Float2DArray was asked to allocate 0 bytes of memory";
      exit(1);
   }
   
	#if defined(DOS)
	unsigned j;
	// kr 30/06/96 Changed function to allocate per row.  DOS Allow max of 64000 bytes.
	index_array = new float*[rows+1];	// +1 to allow for last on = NULL
	if (index_array == NULL)
	{
		//delete [] data_array;
		// delete data_array;  //MMD 31/05/96
      						 //kr 01/07/96 Comment out for DOS case
      rows = 0;
      cols = 0;
      cout << "[MemMngr.cpp] Error allocating array of ";
      cout << rows << " float*'s for Double2DArray.";
      cout << endl;
      return (NULL);
   }

   index_array[rows] = NULL;	// Indication of end of array

	for (i=0; i<rows; i++)
   {
       index_array[i] = new float[cols];
	   if (index_array[i] == NULL)
	   {
		   // Delete the memory already allocated
		   for (j=0; j<i; j++)
		   {
			delete [] index_array[j];
                   }
	      rows = 0;
			cols = 0;
	      cout << "[MemMngr.cpp] Error allocating array of ";
	      cout << rows << " by " << cols << "floats for Double2DArray.";
	      cout << endl;
	      return (NULL);
	   }
   }
  
   #else
//   	cout << "Mem:" << rows << " RxC " << cols << endl;
	data_array = new float[rows*cols];	// kr 30/06/96 Taken out of if statement for clarity
   										// kr 30/06/96 !!! This doesn't generate error when not
   										// enough memory exist.  ??? -- unsigned=int=max 65536
	if (data_array == NULL)
   {
      rows = 0;
      cols = 0;
      cout << "[MemMngr.cpp] Error allocating array of ";
      cout << rows << " by " << cols << "floats for Double2DArray.";
		cout << endl;
      return (NULL);
   }

//	cout << "Rows: " << rows << endl;
   index_array = new float*[rows];	// kr 30/06/96 Taken out of if statement for clarity
   if (index_array == NULL)
   {
      delete [] data_array;
      // delete data_array;  //MMD 31/05/96
      rows = 0;
      cols = 0;
      cout << "[MemMngr.cpp] Error allocating array of ";
		cout << rows << " float*'s for Float2DArray.";
      cout << endl;
      return (NULL);
   }

 //  cout << "InArray: "<< rows << endl;
   for (i=0; i<rows; i++)
		 index_array[i] = &(data_array[i*cols]);
   #endif
   
   return(index_array);
}


/**************************************************************************
@func deletes the raster of floats

@rdesc TRUE if completed successfully

**************************************************************************/

// deletes a 2D array; returns TRUE if successful
int delete_Float2DArray(Float2DArray &array)

{
//er << "Float arrays!" << endl;
	if (array == NULL)  //safety check
	{
      cout << "[MemMngr.cpp] Error de-allocating array:";
      cout << " pointer was not initialised.";
      cout << endl;
      return (false);
   }


	#if defined(DOS)
	// kr 30/06/96 - On dos max memblock is 64000.  Do allocation/de-allocation per row
	unsigned i;
	i=0;
	while (array[i]!=NULL)
	{
     delete [] array[i];
     i++;
   }     
   delete [] array;
	#else
//	delete array[0];    // delete the 'long' array
   delete [] array[0];    // delete the 'long' array ?????????
	delete [] array;       // delete the index array
   #endif
   
	array = NULL;		// kr 30/06/96
   
	return (true);
}


/**************************************************************************
@func Function to copy a 2D array of floats

@rdesc FALSE if completed successfully

**************************************************************************/
// copies a 2D array and returns dest; rows and cols contain 0 if allocation failed
Float2DArray copy_Float2DArray(Float2DArray &dest, const Float2DArray source, unsigned &rows, unsigned &cols)
{
//	#if !(defined(DOS))
	float *data_array;   // new 'long' array
//	#endif
   unsigned i;

   if ((rows==0) || (cols==0))
   {
      cout << "[MemMngr.cpp] copy_Float2DArray cannot copy an array of size 0!";
      exit(1);
   }

   if (source == NULL)
   {
      cout << "[MemMngr.cpp] Error copying array:";
      cout << " source array was not initialised.";
      cout << endl;
      rows = 0;
      cols = 0;
		return (NULL);
   }

   if (dest != NULL)  //safety check
					  // kr 30/06/96 What if dest is not initialised and therefore != NULL ???
   {
      delete_Float2DArray(dest);
   }

	#if defined(DOS)

	 // kr 01/07/96 Added code for DOS model
	dest = new_Float2DArray(rows,cols);
	for (i=0; i<rows; i++)
	  memcpy(dest[i],source[i], (cols*sizeof(float)));

 	#else
	data_array = new float[rows*cols];		// kr 30/06/96 Taken out of if statement for clarity
	if (data_array == NULL)
   {
      rows = 0;
      cols = 0;
      cout << "[MemMngr.cpp] Error allocating array of ";
      cout << rows << " by " << cols << "floats for copying.";
      cout << endl;
      return (NULL);
   }

   memcpy(data_array, source[0], (rows*cols*sizeof(float)));  // copy 'lon' array
                  
   dest = new float*[rows];                  // kr 30/06/96 Taken out of if statement
   if (dest == NULL)
   {
      delete [] data_array; //MMD 31/05/96
      rows = 0;
      cols = 0;
		cout << "[MemMngr.cpp] Error allocating array of ";
      cout << rows << " float*'s for copying.";
      cout << endl;
      return (NULL);
   }

   for (i=0; i<rows; i++)
       dest[i] = &(data_array[i*cols]);
   #endif

   return ((Float2DArray) dest);
}
