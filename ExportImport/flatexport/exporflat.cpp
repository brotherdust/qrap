 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : exporflat.cpp
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

#include <iostream>
#include <string>
#include <vector>
#include <pqxx>


//using namespace pqxx;
using std::string;

#include "exportf.h"

int main(int argc,char *argv[])
{	
	char *fname;
	ExportF ef;

	// check the command line arguments
	if (argc!=2) {
		printf("usage: export fullfilename\n");
		return 0;
	}
	fname = argv[1];

	// connect
	ef.connectDB("localhost","qrap","postgres","postqrap");

	// start the export process
	ef.exportFile(fname,NULL);

	return 0;
}


