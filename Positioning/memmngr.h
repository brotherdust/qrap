// header file for memory management routines
// particularly 2D arrays

#ifndef memmngr_h
#define memmngr_h 1

#include "cgeneric.h"

typedef double** Double2DArray;
typedef float** Float2DArray;


// allocates memory for a 2D array; rows and cols contain 0 if allocation failed
Double2DArray new_Double2DArray(Counter rows=1 , Counter cols=1);


// deletes a 2D array; returns TRUE if successful
int delete_Double2DArray(Double2DArray &array);


// copies a 2D array and returns dest; rows and cols contain 0 if allocation failed
Double2DArray copy_Double2DArray(Double2DArray &dest,
											const Double2DArray source,
                                 Counter &rows, Counter &cols);

// sets each element of the array to the same (double) value
void init_Double2DArray(Double2DArray &dest, Counter rows, Counter cols, double dconst); 

// allocates memory for a 2D array; rows and cols contain 0 if allocation failed
Float2DArray new_Float2DArray(Counter rows=1 , Counter cols=1);


// deletes a 2D array; returns TRUE if successful
int delete_Float2DArray(Float2DArray &array);


// copies a 2D array and returns dest; rows and cols contain 0 if allocation failed
Float2DArray copy_Float2DArray(Float2DArray &dest, const Float2DArray source, Counter &rows, Counter &cols);

// sets each element of the array to the same (double) value
int init_Double2DArray(Double2DArray &dest, Counter rows, Counter cols); 

#endif            
