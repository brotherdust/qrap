 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : fserver.h
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

#ifndef FSERVER_H_
#define FSERVER_H_


/// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <errno.h>

using std::string;

#include "request.h"


/**
	A simple file server. This server serves files and returns their hash values signatures. The server will create a new thread.

	Thread handling:
	 Once the server is run (i.e. runServer is called), no other functions or members should be used. stopServer creates a mutex for exiting.
*/
class FServer
{
public:
	/** ctor */
	FServer();
	/** dtor */
	~FServer();

	/** 
		Set the port number on which the server should listen 
		\param _portnum The port number on which the server should listen on
	*/
	void setPortNumber(unsigned short int _portnum);

	/** 
		Sets the directory that will serve as the root.
		\param rtdir The root directory of the files.
	*/
	void setRootDirectory(const char *rtdir);

	/**
		Sets the information for the server's database.
	*/
	void setDBInfo(const char *_username,const char *_password,const char *_dbase,const char *_hostname);

	/**
		Runs the server. The server will accept connections in this mode. 
		BLOCKING CALL
	*/
	void runServer();

	/** 
		Stops a running server and closes the tread. This will block until the thread is exited.
	*/
	void stopServer();

	/**
		Creates a directory listing of the current directory. The result is stored as fname.
		\param fname The filename under which the file listing.
		\param dname The name of the directory.
	*/
	void buildFileListing(const char *fname,const char *dname);
protected:
	string dbname,hostname;
	string username,password;

	/** The thread entrypoint */
	static void* threadFunc(void* cdata);

	/** This indicates that the thread should exit (since this is called by both threads, this is protected by a mutex) */
	int shouldexit;
	pthread_mutex_t semutex;

	/** The thread for the server */
	pthread_t serverthread;

	/** The filedescriptor of the socket */
	int serversocketfd;
	/** The address of the server socket */
	struct sockaddr_in serverAddress;
	/** The port number on which the server will listen */
	unsigned short int portnum;
	/** The root director (from which the files will be served) */
	char rootDir[PATH_MAX];
	/** 
		Process either a hash or a file request.
		\param socketfd The file-descriptor of the socket.
	*/
	void processRequest(int socketfd);

	/**
		Process the file request part of a request.
		\param fr A filerequest structure containing the request information.
		\param socketfd The socket file descriptor from which further data is read.
	*/
	void processRequestFile(filerequest_t *fr,int socketfd);

	/**
		Processes a request for a hash value for a file. 
		\param fr A filerequest structure containing the request information.
		\param socketfd The socket file descriptor from which the data is read.
	*/
	void processRequestHash(filerequest_t *fr,int socketfd);

	/**
		Process the directory listing build request.
	*/
	void processRequestDirListing(filerequest_t *fr,int socketfd);

	/** 
		Process a request for an ID.
	*/
	void processRequestID(filerequest_t *fr,int socketfd);

	/** 
		Reads from the file until size bytes are read into the buffer. This will call read multiple times.
		\param buf The buffer to be filled.
		\param size The size of the data to read into the buffer
		\param socketfd The filedescriptor of the socket.
		\return Returns 1 if successful.
	*/
	int readBuffer(void* buf,int size,int socketfd);


	/**
		Processes a send file request.
	*/
	void processSendFile(filerequest_t *fr,int socketfd);

	/** 
		Builds the update file.
	*/
	void buildUpdateFile(const char* hid);


	/**
		Writes a 32 bit integer to a socket.
	*/
	void writeU32Socket(unsigned int u32,int socketfd);
};


#endif // FSERVER_H_
