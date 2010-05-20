 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : fhashtest.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Christo (magdaleen.ballot@up.ac.za) 
 *    Description : Not really ready to be used
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

#include <stdio.h>

#include "rc4hash.h"

int main(int argc,char *argv[])
{
	unsigned int hashval;


	/* check the program arguments */
	if (argc!=2) {
		printf("usage: fhashtest filename\n");
		return 0;
	}	

	/* calculate the hash value */
	hashval = rc4_hash(argv[1]);

	/* print the hash value */
	printf("%s 0x%8X\n",argv[1],hashval);	

	return 0;
}
