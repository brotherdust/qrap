 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : LoginDialog.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *                : (Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 dialog widget that defines the interface to manage 
 *                  the user login to the Qrap system
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

#ifndef Qrap_LoginDialog_h
#define Qrap_LoginDialog_h

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QIcon>
#include <string>

#include "Config.h"

using namespace std;

namespace Qrap
{
	/**
	* This class inherits QDialog and forms a simple dialog that captures the username and password
	* of the Qrap user and creates a database engine class that will be used to communicate with 
	* the gisrap database system.
	*/
	
	class LoginDialog : public QDialog
	{
		Q_OBJECT
	
	public:
		/**
		* Constructor that creates the dialog widgets and establishes the signal and slot connections.
		* 
		* @param parent A pointer to the parent object.
		*/
		LoginDialog (QWidget *parent = 0);

		/** 
			Returns the username.
			mcd
		*/
		string getUserName();

		/**
			Returns the password for the user.
		*/
		string getPassword();
		string getDB();
		string getHost();
		int getPort();
		
	private slots:
		/**
		* Private SLOT - This slot is called when the user presses the Login button and will try authenticate
		* the Qrap user.
		*/
		void AuthenticateUser ();
		void Close();		

	private:
		QLineEdit*	mUsernameEdit;	///< A pointer to a Qt4 line edit widget that will allow the user to enter their username for the gisrap system.
		QLineEdit*	mPasswordEdit;	///< A pointer to a Qt4 line edit widget that will allow the user to enter their password.
		QLineEdit*	mDbEdit;
		QLineEdit*	mHostEdit;
		QLineEdit*	mPortEdit;
		QLabel*		mUsernameLabel;	///< A pointer to a Qt4 label widget that displays the username label.
		QLabel*		mPasswordLabel;	///< A pointer to a Qt4 label widget that displays the password label.
		QLabel*		mDbLabel;
		QLabel*		mHostLabel;
		QLabel*		mPortLabel;

		QPushButton*	loginButton;	///< A pointer to a Qt4 push button widget that calls the database authentication routine to authenticate the user.
		QPushButton*	cancelButton;	///< A pointer to a Qt4 push button widget that will close the gisrap program all together.
		QPushButton*	syncButton;	///< A pointer to a Qt4 push button widget that will open the sync dialog.

		string mUser;
		string mPass;
		string mDb;
		string mHost;
		int mPort;
	};
	// username and password strings
}

#endif
