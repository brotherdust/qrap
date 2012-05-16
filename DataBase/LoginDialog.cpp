 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : LoginDialog.cpp
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

#include "LoginDialog.h"
#include "MainWindow.h"

using namespace std;
using namespace Qrap;

//*******************************************************************************
// Constructor that creates all the widgets
LoginDialog::LoginDialog (QWidget *parent) : QDialog(parent)
{
	// Create all the widget objects
	mUsernameLabel = new QLabel(tr("Username:"),this);
	mPasswordLabel = new QLabel(tr("Password:"),this);
	mUsernameEdit = new QLineEdit(this);
	mUsernameEdit->insert("postgres");
	mPasswordEdit = new QLineEdit(this);
	mDbLabel = new QLabel(tr("Database:"),this);
	mHostLabel = new QLabel(tr("Host:"),this);
	mPortLabel = new QLabel(tr("port:"),this);
	mDbEdit = new QLineEdit(this);
	mDbEdit->insert("qrap");
	mHostEdit = new QLineEdit(this);
	mHostEdit->insert("localhost");
	mPortEdit = new QLineEdit(this);
	mPortEdit->insert("5432");
	loginButton = new QPushButton(tr("&Login"),this);
	cancelButton = new QPushButton(tr("C&ancel"),this);
	//syncButton = new QPushButton(tr("&Synchronise"),this);
	
	// Create the layouts
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	QGridLayout* mainLayout = new QGridLayout(this);
	
	// Setup the main grid layout and button layout
	mainLayout->addWidget(mUsernameLabel,0,0,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mPasswordLabel,1,0,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mDbLabel,2,0,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mHostLabel,4,0,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mPortLabel,3,0,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mUsernameEdit,0,1,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mPasswordEdit,1,1,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mDbEdit,2,1,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mHostEdit,4,1,1,1,Qt::AlignLeft);
	mainLayout->addWidget(mPortEdit,3,1,1,1,Qt::AlignLeft);
	
	buttonLayout->addWidget(loginButton,Qt::AlignLeft);
	//buttonLayout->addWidget(syncButton,Qt::AlignLeft);
	buttonLayout->addWidget(cancelButton,Qt::AlignLeft);
	
	mainLayout->addLayout(buttonLayout,5,1,1,1,Qt::AlignLeft);
	
	//Setup the password line edit
	mPasswordEdit->setEchoMode(QLineEdit::Password);
	
	// Setup the dialog window
	setLayout(mainLayout);
	setWindowTitle(tr("Q-Rap Login"));
	setModal(true);
	
	//Move this dialog to the center of the screen
	move(400,300);
	setWindowIcon(QIcon(":images/logo_icon.png"));
	
	// Setup the SIGNAL SLOT connections
	connect(loginButton,SIGNAL(released()),this,SLOT(AuthenticateUser()));
	connect(cancelButton,SIGNAL(released()),this,SLOT(Close()));
}

//************************************************************************************
// Authenticate the Qrap user
void LoginDialog::AuthenticateUser ()
{
	mUser = mUsernameEdit->text().toStdString();
	mPass = mPasswordEdit->text().toStdString();
	mHost = mHostEdit->text().toStdString();
	mDb = mDbEdit->text().toStdString();
	mPort = atoi(mPortEdit->text().toStdString().c_str());
	
	// Create a new connections
	if(!gDb.Connect(mUser,mPass,false,mDb,mHost,mPort))
	{
		mPasswordEdit->insert("postqrap");
		AlertCode alert = gDb.GetLastAlert();
		string lastAlert = TranslateAlertCode(alert);
		cout << "Database connect failed:" + lastAlert << endl;
		QMessageBox::critical(this,"QRAP Error","Database connect failed:" + QString::fromStdString(lastAlert));
	}
	else
	{
		// Create the main window
		gDb.getsetMachineID();
//		gMainWindow = new MainWindow();
//		gMainWindow->show();
		close();
	}
}

//***************************************************************************************
void LoginDialog::Close()
{
	mUser = mUsernameEdit->text().toStdString();
	mPasswordEdit->insert("postqrap");
	mPass = mPasswordEdit->text().toStdString();
	mHost = mHostEdit->text().toStdString();
	mDb = mDbEdit->text().toStdString();
	mPort = atoi(mPortEdit->text().toStdString().c_str());
	close();
}

//****************************************
string LoginDialog::getUserName()
{
	return mUser;
}

//****************************************
string LoginDialog::getPassword()
{
	return mPass;
}

//*****************************************
string LoginDialog::getDB()
{
	return mDb;
}

//*****************************************
string LoginDialog::getHost()
{
	return mHost;
}

//*****************************************
int LoginDialog::getPort()
{
	return mPort;
}
