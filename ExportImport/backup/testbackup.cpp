 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : testbackup.cpp
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


using namespace pqxx;
using namespace std;

#include "backup.h"

void backup(char *fname);

int main(int argc,char *argv[])
{
	if (argc!=2) {
		printf("usage: backup fname\n");
		return 0;
	}


	// perform the backup
	backup(argv[1]);

	return 0;
}

void backup(char *fname)
{
	Backup bk;

	// connect to the db
	bk.connectDB("localhost","qrap","postgres","postqrap");

	// backup the files
	bk.backupList("tables.txt",fname);
}

