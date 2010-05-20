 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : mhash.cpp
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

#include "mhash.h"

unsigned int hash(const char *fname)
{
	FILE *f;
	unsigned int totalval;
	int cval;
	unsigned int ki;
	unsigned int h;
	
	/* open the file */
	f = fopen(fname,"rb");
	if (!f) {
		printf("hash: file [%s] failed to open\n");
		return 0;
	}

	/* read the file and perform a CRC-type hash */
	h = 0;
	while (1) {
		/* read the character */
		cval = getc(f);

		/* check for EOF */
		if (cval == EOF) {
			break;
		}
		ki = (unsigned int)cval;

		/* XOR together (replace this with CRC) */
		h ^= ki;
		
	}
	

	/* close the file */
	fclose(f);

	return h;
}

