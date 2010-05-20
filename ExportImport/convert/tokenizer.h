 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : tokenizer.h
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

#ifndef TOKENIZER_H_
#define TOKENIZER_H_


/// A string tokenizer
class Tokenizer
{
public:
	Tokenizer();
	~Tokenizer();

	/// Sets the string that will be used for the tokenization.
	void setString(const char *c,const char *_delim);

	/// Returns a token. If there are no tokens left, the function 
	///  will return NULL.
	char* getToken(char *sbuf,int len);
protected:
	char *strbuf;
	char *delim;
	char *current;
	int len;
	int tokens;
	int dlen;

	int isDelimiter(char c);
};

#endif // TOKENIZER_H_
