 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : rc4.h
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

#ifndef RC4_H_
#define RC4_H_

typedef unsigned char U8;

typedef struct {
	int s[256];
	int i;
	int j;
}rc4info_t;

/**
	initializes the RC4 algorithm with a key.
	\param rc4i the rc4 context to be initialized
	\param key the key to be set
	\param keylen the length of the key
  */
void rc4_init(rc4info_t *rc4i,unsigned char *key,int keylen);

/**
	returns a byte that can be XOR'ed with the plaintext to provide the 
	 ciphertext
	\param rc4i the RC4 context to be used
	\return returns a char that can be XOR'ed with the plaintext
*/
unsigned char rc4_getbyte(rc4info_t* rc4i);

#endif // RC4_H_


