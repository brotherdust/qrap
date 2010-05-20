 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : rc4.cpp
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

#include "rc4.h"

void rc4_init(rc4info_t *rc4i,unsigned char *key,int keylen)
{
	int i;
	int k[256];
	int tmp;
	int j;
	int *s;
	int idx;

	s = rc4i->s;

	/* set the password */
	for (i=0;i<256;i++) {
		s[i] = i;

		idx = i % keylen;
		k[i] = key[idx];
	}

	/* initial permutation */
	j = 0;
	for (i=0;i<256;i++) {
		j = (j + s[i] + k[i]) & 0xFF;
		
		/* swap */
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	/* set the counters to zero */
	rc4i->i = 0;
	rc4i->j = 0;
}

unsigned char rc4_getbyte(rc4info_t* rc4i)
{
	U8 tmp;
	int index;
	int *s;

	s = rc4i->s;

	/* calculate the indices */
	rc4i->i = (rc4i->i + 1) & 0xFF;
	rc4i->j = (rc4i->j + s[rc4i->i]) & 0xFF;

	/* do the swap */
	tmp = s[rc4i->i];
	s[rc4i->i] = s[rc4i->j];
	s[rc4i->j] = tmp;

	/* calculate the index */
	index = (s[rc4i->i] + s[rc4i->j]) & 0xFF;

	/* return the byte */
	return (unsigned char)s[index];
}
