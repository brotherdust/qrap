 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : rc4hash.cpp
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
#include <stdlib.h>

#include "rc4.h"

#define KEYLENGTH 16

unsigned int rc4_hash(const char *fname)
{
	FILE *f;
	rc4info_t rc4i;
	unsigned char key[KEYLENGTH];
	int i;
	unsigned int keybyte,filebyte,hashval;
	int bytesread;
	unsigned char b;

	hashval = 0;
	
	/* open the file */
	f = fopen(fname,"rb");
	if (!f) {
		return 0;
	}

	/* set the RC4 key and initialize the structure */
	for (i=0;i<KEYLENGTH;i++) {
		key[i] = 'a'+i;
	}
	rc4_init(&rc4i,key,KEYLENGTH);
	
	/* read bytes from the file */
	i = 0;
	while (1) {
		/* read the byte */
		bytesread = fread(&b,1,sizeof(unsigned char),f);
		if (bytesread<=0) {
			break;
		}	
		filebyte = (unsigned int)b;
	
		/* get the key byte */		
		keybyte = (unsigned int)rc4_getbyte(&rc4i);

		/* add it to the hash value */
		hashval ^= (filebyte ^keybyte) << (i*8);
		i = (i+1) % 4;
	}

	/* close the file */
	fclose(f);

	return hashval;
}
