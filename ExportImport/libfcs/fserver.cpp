 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : fserver.cpp
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

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pqxx>

#include "fserver.h"
#include "request.h"
#include "rc4hash.h"
#include "filelist.h"
#include "../flatimport/importf.h"
#include "../flatexport/exportf.h"
#include "../libfcs/idresolve.h"

#define SMALLBUF 256

void* FServer::threadFunc(void* cdata)
{
	FServer *fs;
	int clientsocketfd;
	socklen_t clientAddressLen;
	struct sockaddr_in clientAddress;
	int lshouldexit = 0;

	/* get a pointer to the server */
	fs = (FServer*)cdata;

	/* listen for client connections and handle the requests */
	while (1){
		/* lock the mutex and check if the thread should be exited */
		pthread_mutex_lock(&fs->semutex);
		lshouldexit = fs->shouldexit;
		pthread_mutex_unlock(&fs->semutex);

		/* check if the thread should exit */
		if (lshouldexit) {
			break;
		}

		/* accept a connection (non-blocking call,polling) */
		clientAddressLen = sizeof(struct sockaddr_in);
		clientsocketfd = accept(fs->serversocketfd,(struct sockaddr*)&clientAddress,&clientAddressLen);

		/* check the return value (if blocking) */	
		if (clientsocketfd>=0){
			/* handle the request */
//			printf("connection made\n");
			fs->processRequest(clientsocketfd);
		} else {
			/* check if there was an error */
			if (errno!=EAGAIN) {
				printf("accept failed\n");
				return NULL;
			}
		}
	}

	/* close the socket */
	close(fs->serversocketfd);
	
	return NULL;
}

FServer::FServer()
{
	char tmp[PATH_MAX];

	/* set the port number to default */
	setPortNumber(10100);

	/* set the root directory to the current directory */
	getcwd(tmp,PATH_MAX);
	setRootDirectory(tmp);
}

FServer::~FServer()
{}

class SimpleException
{
	public :
		int a;
};

void FServer::setPortNumber(unsigned short int _portnum)
{
	try {
		portnum = _portnum;
	} catch (const SimpleException &se) {
	}
}

void FServer::setRootDirectory(const char *rtdir)
{
	/* copy the root directory */
	strncpy(rootDir,rtdir,PATH_MAX);

	/* set the root directory */
	chdir(rootDir);	
}

void FServer::runServer()
{
	int flags;

	/* create the server socket (internet, TCP/IP) */
	serversocketfd = socket(AF_INET,SOCK_STREAM,0);
	if (serversocketfd == -1) {
		printf("failed to create server socket: %d %s\n",errno,strerror(errno));
		return ;	
	}

	/* create the address on which the socket should listen on */
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(portnum);

	/* bind the address to the socket */
	bind(serversocketfd,(struct sockaddr*)&serverAddress,sizeof(serverAddress));

	/* listen on the socket */
	listen(serversocketfd,10);

	/* set the server socket as non-blocking */
	flags = fcntl(serversocketfd, F_GETFL, 0);
	fcntl(serversocketfd,F_SETFL,flags|O_NDELAY);

	/* set the attributes before the thread is created */
	shouldexit = 0;

	/* initialize the mutex */
	pthread_mutex_init(&semutex,NULL);

	/* create the thread */
	pthread_create(&serverthread,NULL,threadFunc,this);
}

void FServer::stopServer()
{
	/* send a stop signal to the server */
	pthread_mutex_lock(&semutex);
	shouldexit = 1;
	pthread_mutex_unlock(&semutex);


	/* join this thread, to force it to wait */
	pthread_join(serverthread,NULL);

	/* destroy the mutex (release resources) */
	pthread_mutex_destroy(&semutex);

	/* close the server socket */
	close(serversocketfd);
}

int FServer::readBuffer(void* buf,int size,int socketfd)
{
	unsigned char *p;
	int toread;
	int bytesread,bytestoread;

	/* set the pointer and the number of bytes to read */
	p = (unsigned char*)buf;
	toread = size;
	 

	while (toread>0)
	{
		/* calculate the number of bytes to read */
		bytestoread = (SMALLBUF<toread)?SMALLBUF:toread;

		// ssize_t read(int fd, void *buf, size_t count);

		/* read the bytes */
		bytesread = read(socketfd,p,bytestoread);
		if (bytesread<0) {
			break;
		}
		
		/* decrement the number of remaining bytes */
		p+=bytesread;
		toread -= bytesread;
	}

	return 1;
}

void FServer::processRequest(int socketfd)
{
	filerequest_t fr;
	int ret;



	/* read the request from the stream */
	ret = readBuffer(&fr,sizeof(filerequest_t),socketfd);
	if (ret<0) {
		printf("failed to read buffer\n");
	}

	/* print the request */
	printf("[%s]\n",fr.fname);
	if (fr.request == FREQUESTFILE) {
//		printf("request file\n");
	} else if (fr.request == FREQUESTHASH) {
//		printf("request hash\n");
	} else if (fr.request == FREQUESTDIRLISTING) {
		printf("directory listing update\n");
	} else if (fr.request == FSENDFILE) {
		printf("send file request\n");
	} else if (fr.request == FGETMACHINEID) {
		printf("request id \n");
	}

	/* check if a file request should be processed */
	if (fr.request == FREQUESTFILE) {
		processRequestFile(&fr,socketfd);
	} else if (fr.request == FREQUESTHASH) { /* process a hash request */
		processRequestHash(&fr,socketfd);
	} else if (fr.request == FREQUESTDIRLISTING) {
		processRequestDirListing(&fr,socketfd);
	} else if (fr.request == FSENDFILE) {
		processSendFile(&fr,socketfd);
	} else if (fr.request == FGETMACHINEID) {
		processRequestID(&fr,socketfd);
	}
}

void FServer::processRequestDirListing(filerequest_t *fr,int socketfd)
{
	/* build the file listing */
	buildFileListing("filelist.txt","dirlist.txt");

	/* close the socket */
	close(socketfd);
}

void FServer::processRequestFile(filerequest_t *fr,int socketfd)
{
	int bytesread;
	unsigned char buf[SMALLBUF];
	FILE *f;

	/* open the file */
	f = fopen(fr->fname,"rb");
	if (!f) {
		printf("failed to open [%s]\n",fr->fname);
		close(socketfd);
		return;
	}

	printf("file opened\n");

	/* write the file */
	while (1) {
		/* read the bytes */
		bytesread = fread(buf,1,SMALLBUF,f);
//		printf("bytesread %d \n",bytesread);
		if (bytesread<=0) {
			break;
		}

		/* write it to the socket */
		write(socketfd,buf,bytesread);
	}

	/* close the file */
	fclose(f);

	/* close the client socket */
	close(socketfd);	
}

void FServer::processRequestHash(filerequest_t *fr,int socketfd)
{
	unsigned int hashval;
	unsigned char buf[4];
	int i;


	/* get the hash value for the file  */
	//printf("calculating hash value \n");
	hashval = rc4_hash(fr->fname);
	//printf("Server hash value %X\n",hashval);

	/* read the values into a buffer (to assure same endianess) */
	for (i=0;i<4;i++) {
		buf[i] = 0xFF&(hashval >> (i*8));
	}

	/* write the buffer to the client */
	write(socketfd,buf,4);

	/* close the socket */
	close(socketfd);
}

void FServer::processRequestID(filerequest_t *fr,int socketfd)
{
	int id;
	int byteswritten;
	IdResolve ir;

	printf("FServer::processRequestID\n");

	// connect to the database
	if (!ir.connectDB(hostname.c_str(), dbname.c_str(), username.c_str(), password.c_str())) {
		printf("failed to connect to the database\n");
		close(socketfd);
		return ;
	}

	printf("FServer::1\n");
	// resolve the id
	id = ir.resolveId(fr->hid," ");

	// return the id in the socket 
	writeU32Socket(id,socketfd);
	
	// close the socket 
	close(socketfd);

	printf("FServer::processRequestID ID written %d\n",id);
}

void FServer::buildFileListing(const char *fname,const char *dname)
{
	fl_createlist(".",fname,dname);
}

void FServer::processSendFile(filerequest_t *fr,int socketfd)
{
	FILE *f;
	int bytesread;
	unsigned char buf[256];
	char fullfname[1024];
	char cwd[1024];
	char hid[1024];
	
	printf("process send file\n");

	/* create the file */
	f = fopen(fr->fname,"wb");
	if (!f) {
		printf("failed to open file (FServer::processSendFile\n");
	}

	/* read the data from the socket */
	while (1) {
		/* read the bytes from the socket */
		bytesread = read(socketfd,buf,256);
		printf("%d ",bytesread);
		if (bytesread<=0) {
			break;
		}

		/* write the bytes to the file */
		fwrite(buf,1,bytesread,f);		
	}

	/* close the file */
	fclose(f);

	/* create an instance of the importf class */
	ImportF *imf;
printf("IMPORTING------------------------------------------------------------------------\n");
	imf = new ImportF();

	/* open the database connection */
	if (!imf->connectDB(hostname.c_str(), dbname.c_str(), username.c_str(),password.c_str())) {
		printf("FServer::processSendFile Failure to connect to the database\n");
		printf("connection settings: dbname %s username %s password %s hostname %s\n",dbname.c_str(),username.c_str(),password.c_str(),hostname.c_str());
	}

	/* get the full name of the file */
	getcwd(cwd,1024);
	sprintf(fullfname,"%s/%s",cwd,fr->fname);


	/* import the file to the server's database */
	printf("importing [%s]\n",fullfname);
	imf->importFile(fullfname);

	/* free the importf class */
	delete imf;

	/* get the hid as a string */
	sprintf(hid,"%d",fr->hid);

	/* build the update file */
printf("BUILDIG UPDATE FILE (EXPORTING)------------------------------------------------------------------------");
	buildUpdateFile(hid);
}


void FServer::setDBInfo(const char *_username,const char *_password,const char *_dbase,const char *_hostname)
{

	dbname = string(_dbase);
	username = string(_username);
	password = string(_password);
	hostname = string(_hostname);
}


void FServer::buildUpdateFile(const char *hid)
{
	ExportF *ef;
	string shid;
	char buf[1024];

	printf("\n\nFServer::buildUpdateFile\n");

	/* create a new export structure */
	ef = new ExportF();

	/* connect to the database */
	ef->connectDB(hostname.c_str(), dbname.c_str(), username.c_str(),password.c_str());

	/* build the update file */
	sprintf(buf,"%supdate.txd",rootDir);
	printf("buf [%s]\n",buf);
	if (!ef->exportFile(buf,NULL)) {
		printf("FServer::buildUpdateFile - failed to export file\n");
	}

	/* delete the exportf structure */
	delete ef;

	/* print the location of the update file */
	printf("Update file written to [%s]\n",rootDir);
}


void FServer::writeU32Socket(unsigned int u32,int socketfd)
{
	unsigned char buf[4];
	int i;

	/* read the values into a buffer (to assure same endianess) */
	for (i=0;i<4;i++) {
		buf[i] = 0xFF&(u32 >> (i*8));
	}

	/* write the buffer to the client */
	write(socketfd,buf,4);
}
