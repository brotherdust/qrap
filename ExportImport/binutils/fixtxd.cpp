#include <stdio.h>
#include <stdlib.h>

#include <string>

using std::string;

#define MAXLINE 16384

#include "tokenizer.h"

int convertTxd(const char *fname1,const char *fname2);

int main(int argc,char* argv[])
{
	// check arguments
	if (argc!=3) {
		printf("usage: fixtxd inputfile.txd outputfile.csv\n\n");
		return 0;
	}

	// convert the file
	convertTxd(argv[1],argv[2]);
	
	return 0;
}

string processLine(char *input)
{
	int len,i;
	char buf[1024];
	char *c;
	string qr,tk;
	int fs=0;
	string ts;

	Tokenizer tok;

	// setup the tokenizer
	tok.setString(input,"\t");

	// create the query
	qr = "";

	// start the tokenization
	i = 0;
	while (1) {
		// get the token
		c = tok.getToken(buf,1024);

		// check if last token
		if (c==NULL) {
			break;
		}

		// set the token
		tk = string(buf);


		if (fs) {
			qr = qr + string("\t");
		}

		if (tk=="\\N" || tk=="") {
			ts = string("\\N");		
		} else {
			ts = tk;
		}
		qr = qr + ts;

		// print the token
		fs = 1;

		i++;
	}



	// copy the string
	return qr;
}

int convertTxd(const char *fname1,const char *fname2)
{
	FILE *ifile,*ofile;
	char buf[MAXLINE];
	char *c;

	string output;

	// open the files
	ifile = fopen(fname1,"rt");
	ofile = fopen(fname2,"wt");

	// check that the files are opened
	if (ifile==NULL || ofile==NULL) {
		printf("file not opened\n");
		return 0;
	}

	while (1) {
		// read a line of text
		c = fgets(buf,MAXLINE,ifile);
		if (c==NULL)
			break;

		// remove the newline
		while (*c!='\0') {
			if (*c=='\n')
				*c = '\0';
			c++;
		}

		// process the line, getting the result
		output = processLine(buf);

		// write the line to the output file
		fprintf(ofile,"%s\n",output.c_str());
	}

	// close the files 
	fclose(ifile);
	fclose(ofile);

	return 1;
}

