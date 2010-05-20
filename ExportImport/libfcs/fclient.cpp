 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : fclient.cpp
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
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#include "fclient.h"
#include "request.h"
#include "rc4hash.h"

FClient::FClient()
{
	char buf[PATH_MAX];
	
	/* set the default server name */
	setServerName("localhost");

	/* set the default port number */
	setServerPort(10002);

	/* set the directory to the current working directory */
	getcwd(buf,PATH_MAX);
	setTargetDirectory(buf);
	
}

FClient::~FClient()
{}

int FClient::setServerName(const char *hname)
{
	strcpy(serverhname,hname);
	return 1;
}

void FClient::setTargetDirectory(const char *dir)
{
	/* copy the path */
	strcpy(targetpath,dir);
	chdir(targetpath);
}


void FClient::setServerPort(unsigned short _portnum)
{
	serverport = _portnum;
}



unsigned int FClient::getHash(const char *fname)
{
	int socketfd;
	unsigned char buf[4];
	int i;
	unsigned int hashval;
	unsigned int id;

	/* connect to the server */
	socketfd = connectServer();

	/* send the request */
	sendMessage(socketfd,fname,FREQUESTHASH);

	/* read the hash value */
	read(socketfd,&id,4);

	/* close the socket */
	close(socketfd);

	return id;
}

int FClient::getFile(const char *fname)
{
	int socketfd;
	int bytesread;
	FILE *outf;
	unsigned char buf[256];

	/* connect to the server */
//	printf("connect server\n");
	socketfd = connectServer();
	if (socketfd<=0) {
		return 0;
	}

	/* send the request */
//	printf("send request\n");
	sendMessage(socketfd,fname,FREQUESTFILE);

	/* create the file */
	outf = fopen(fname,"wb");

	/* read the file from the socket and write  */
	while (1) {
		/* read the bytes from the socket */
		bytesread = read(socketfd,buf,256);
//		printf("%d ",bytesread);
		if (bytesread<=0) {
			break;
		}

		/* write the bytes to the file */
		fwrite(buf,1,bytesread,outf);		
	}

	/* close the file */
	fclose(outf);

	/* close the socket */
	close(socketfd);

	return 1;
}

int FClient::connectServer()
{
	struct hostent* serverent; /* the entry of the server in the hosts database */
	struct sockaddr_in serverAddress; /* the address of the server */
	int socketfd; /* the file descriptor of the connected socket */
	int ret;

	/* create the socket */
	socketfd = socket(AF_INET,SOCK_STREAM,0);
	if (socketfd<0) {
		printf("failed to create socket\n");
		printf("errno %d",errno);
		return -1;
	}

	/* translate the text hostname to hostent structure */
	serverent = gethostbyname(serverhname);
	if (serverent == NULL) {
		printf("unable to translate the hostname [%s]\n",serverhname);
		printf("error number: %d HOSTNOTFOUND %d NO_ADDRESS %d NORECOVERY %d\n",h_errno,HOST_NOT_FOUND,NO_ADDRESS,NO_RECOVERY);
		return -1;
	}

	/* setup the address */
	serverAddress.sin_family = AF_INET;
	memcpy((void*)&serverAddress.sin_addr.s_addr,serverent->h_addr,serverent->h_length);
	serverAddress.sin_port = htons(serverport);

	/* connect */
	ret = connect(socketfd,(struct sockaddr*) &serverAddress,sizeof(serverAddress));
	if (ret<0) {
		printf("failed to connect to host [%s]\n",serverhname);
		return -1;
	}

	return socketfd;	
}

void FClient::sendMessage(int socketfd,const char *fname,int message)
{
	filerequest_t fr;
	int ret;

	/* clear the structure */
	memset(&fr,0,sizeof(filerequest_t));

	/* copy the file name */
	memcpy(&fr.fname,fname,strlen(fname)+1);
	fr.request = message;

	/* set the host id */
	fr.hid = gethostid();

	/* write to the server */
	ret = write(socketfd, &fr,sizeof(filerequest_t));
	if (ret==-1) {
		printf("sendMessage: failed to send message\n");
	}


//	printf("RET %d\n",ret);
}

int FClient::updateFile(const char *fname)
{
	unsigned int hashval,serverhash;

	/* get the hash value of the file */
	hashval = rc4_hash(fname);

	/* get the hash value from the server */
	serverhash = getHash(fname);	

	/* check if the hash values differ */
	if (hashval != serverhash)
	{
		/* download the file */
		getFile(fname);	
		printf("UPDATE: %s\n",fname);
	} else {
//		printf("HASHMATCH: %s\n",fname);
//		printf("hash values are the same (%X)\n",hashval);
	}

	return 1;
}

void FClient::requestFileListRebuild()
{
	unsigned int socketfd;

	/* connect to the server */
	socketfd = connectServer();

	/* send the message */
	sendMessage(socketfd,"",FREQUESTDIRLISTING);
}

int FClient::updateDirectoryStructure(const char *dfile)
{
	FILE *d;
	char dir[PATH_MAX];
	int ret;

	/* open the directory list */
	d = fopen(dfile,"rt");
	if (!d) {
		printf("failed to open [%s]\n",dfile);
		return 0;
	}

	while (1) {	
		/* read a line */
		ret = fscanf(d,"%s\n",dir);
		if (ret<=0) {
			break;
		}

		/*  create the directory (if it exists, this will fail) */
		printf("[%s]\n",dir);
		mkdir(dir,0);		
	}


	/* close the directory list */
	fclose(d);

	return 1;
}

int FClient::updateFiles(const char *ffile)
{
	FILE *f;
	char fname[PATH_MAX];
	int ret;

	/* open the file */
	f = fopen(ffile,"rt");
	if (!f) {
		printf("failed to open [%s]\n",ffile);
		return -1;
	}

	while (1) {	
		/* read a line */
		ret = fscanf(f,"%s\n",fname);
		if (ret<=0) {
			break;
		}

//		printf("[%s]\n",fname);
		updateFile(fname);
	}

	/* close the file */
	fclose(f);
}

void FClient::updateAll()
{
	/* request that the file and directory lists be rebuilt */
	printf("updating file list\n");
	requestFileListRebuild();

	/* get local copies of the file and directory lists */
	printf("filelist.txt\n");
	updateFile("filelist.txt");
	printf("dirlist.txt\n");
	updateFile("dirlist.txt");

	/* process each of these files */
	printf("update directory structure\n");
	updateDirectoryStructure("dirlist.txt");

	/* update the files */
	printf("updating the files\n");
	updateFiles("filelist.txt");
}

int FClient::sendFile(const char *fname)
{
	int socketfd;
	unsigned char buf[128];
	int i;
	unsigned int hashval;
	FILE *f;

	/* connect to the server */
	socketfd = connectServer();
	if (socketfd<0) {
		return 0;
	}
	

	/* send the request */
	sendMessage(socketfd,fname,FSENDFILE);

	/* send the file */
	writeFile(fname,socketfd);


	/* close the socket */
	close(socketfd);

	return 1;
}

int FClient::requestID(const char *desc)
{
	int socketfd;
	unsigned char buf[128];
	int i;
	unsigned int uret;
	FILE *f;
	int hid;

	hid = gethostid();

	/* connect to the server */
	socketfd = connectServer();
	if (socketfd<0) {
		return 0;
	}
	
	/* send the request */
	sendMessage(socketfd,"",FGETMACHINEID);

	/* read the id */
	uret = readU32(socketfd);

	/* close the socket */
	close(socketfd);

	return uret;
}

int FClient::writeFile(const char *fname,int socketfd)
{
	FILE *f;
	unsigned char buf[128];
	int bytesread;

	/* open the file */
	f = fopen(fname,"rb");
	if (!f) {
		printf("failed to open [%s]\n",fname);
		close(socketfd);
		return 0;
	}

	printf("file opened\n");

	/* write the file */
	while (1) {
		/* read the bytes */
		bytesread = fread(buf,1,128,f);
		if (bytesread<=0) {
			break;
		}

		/* write it to the socket */
		write(socketfd,buf,bytesread);
	}

	/* close the file */
	fclose(f);

	return 1;
}

unsigned int FClient::readU32(int socketfd)
{
	unsigned char buf[4];
	int i;
	unsigned int uout;

	/* read the values into the buffer */
	read(socketfd,buf,4);

	/* set the integer */
	uout = 0;
	for (i=0;i<4;i++) {
		uout = uout + (((int)buf[i])<<(i*8));
	}

	return uout;
}	
