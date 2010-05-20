#include <stdio.h>
#include <string.h>

#include "tokenizer.h"

Tokenizer::Tokenizer():
	strbuf(NULL),current(NULL)
{
}


Tokenizer::~Tokenizer()
{
	if (strbuf) {
		delete strbuf;
		delete delim;
	}
}

void Tokenizer::setString(const char *c,const char *_delim)
{
	// get the length and allocate the string
	len = strlen(c);
	strbuf = new char[len+1];
	current = strbuf;

	// copy the string
	strcpy(strbuf,c);

	// get the length of the delemiter and allocate
	dlen = strlen(_delim);
	delim = new char[dlen+1];

	// copy the string
	strcpy(delim,_delim);

	// set the number of tokens
	tokens = 0;	
}

char* Tokenizer::getToken(char *buf,int len)
{
	int ilen;
	int itap = 1;

	// copy while no delimiter is read
	ilen = 0;
	while (*current!='\0')
	{

		if (isDelimiter(*current)){
			current++;
			itap = 0;
			break;
		}
		*buf = *current;
		buf++;
		current++;
		ilen++;
	}

	*buf = '\0';

	//
	if (ilen==0&&*current=='\0'&&itap)
		return NULL;

	return buf;
}

int Tokenizer::isDelimiter(char c)
{
	int i;

	for (i=0;i<dlen;i++) {
		if (delim[i]==c)
			return 1;
	}

	return 0;
}
