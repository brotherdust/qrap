 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : fclient.h
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

#ifndef FCLIENT_H_
#define FCLIENT_H_

/// includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define MAX_HOST 1024
#define PATH_MAX 1024

class FClient
{
public:
	/** ctor */
	FClient();

	/** dtor */
	~FClient();

	/**
		Sets the server name
		\param hname The name of the server (e.g. localhost)
		\return Returns 1 if successful, otherwise 0
	*/
	int setServerName(const char *hname);

	/**
		Sets the directory into which the files should be copied.
		\param dir The directory name.
	*/
	void setTargetDirectory(const char *dir);

	/** 
		Sets the port number of the server. 
		\param _portnum The port number of the server.
	*/
	void setServerPort(unsigned short _portnum);

	/**
		Retrieves the hash value of the file from the server. 
	*/
	unsigned int getHash(const char *fname);

	/**
		Downloads the file from the server. 
		\param fname The relative file name of the file.
		\return Returns 1 if successful, otherwise 0
	*/
	int getFile(const char *fname);

	/**
		Calculates the hash of a file. If the hash differs with the one on the server, it will be updated.
		\param fname The filename
		\return Returns 1 if successful.
	*/
	int updateFile(const char *fname);


	/** 
		Request that the file list be rebuilt.
	*/
	void requestFileListRebuild();
	
	/**
		returns the id of the host
	*/
	int requestID(const char *desc);

	/**
		Update all files and folders
	 */
	void updateAll();

	/** 
		Sends the file to the server for an update.
	*/
	int sendFile(const char *fname);

	/**
		Writes a file to the socket.
	*/
	int writeFile(const char *fname,int socketfd);

protected:
	/** The hostname of the server */
	char serverhname[MAX_HOST];
	/** The port of the server */
	unsigned int serverport;
	/** The target path */
	char targetpath[MAX_HOST];

	/** updates the entire directory structure */
	int updateDirectoryStructure(const char *dfile);

	/** updates the entire file structure */
	int updateFiles(const char *ffile);

	/* 
		Creates a connection to the server. 
		\return Returns a socket file descriptor (-1 if failed) */
	int connectServer();

	/* 
		Sends a message to the server. 
		\param socketfd The socket which will be used to send the message
		\param fname The filename of the file.
		\parm message The message to be sent.
	*/
	void sendMessage(int socketfd,const char *fname,int message);

	/** 
		Reads an unsigned integer from a socket.
	*/
	unsigned int readU32(int socketfd);
};

#endif // FCLIENT_H_
