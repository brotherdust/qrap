 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : convertp.cpp
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

#include "convert.h"

int main(int argc,char* argv[]) 
{
	Convert *c;
	const char *ver;

	// check the args
	if (argc!=3) {
		printf("convert inputfilefullp outputfilefullp\n");
	/*	printf("version of the input file is either v1 or v2\nv1 - old format with no machineid\nv2 newer format with no frequency allocation\n");
	*/	return 0;
	}

	ver = argv[1];

	// create a new convert structure
	c = new Convert();

	// convert the file
	int ret = c->connectDB("localhost","qrap","postgres","postqrap");
	if (!ret) {
		printf("failed to open connection\n");
		return 0;
	}

	// convert a file 
	c->convertFile(argv[1],argv[2]);
	
	// delete the structure
	delete c;

	return 0;
}
