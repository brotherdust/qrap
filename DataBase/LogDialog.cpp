 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : LogDialog.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
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

#include "LogDialog.h"

using namespace std;
using namespace Qrap;

// Constructor that creates all the widgets
LogDialog::LogDialog (QWidget *parent, QString logMessage) : QDialog(parent)
{
	// Create all the widget objects
	logLabel = new QLabel(tr("Log"),this);
	cancelButton = new QPushButton(tr("&Close"),this);
	log = new QTextEdit(logMessage,this);
	
	log->setMinimumSize(500,500);
	cancelButton->setMaximumWidth(80);
	cancelButton->setMinimumWidth(80);
	
	// Create the layouts
	QVBoxLayout* mainLayout = new QVBoxLayout();
	
	mainLayout->addWidget(logLabel);
	mainLayout->addWidget(log);
	mainLayout->addWidget(cancelButton,Qt::AlignRight);
	
	// Setup the dialog window
	setLayout(mainLayout);
	setWindowTitle(tr("Log"));
	
	//Move this dialog to the center of the screen
	move(400,300);
	setWindowIcon(QIcon(":images/logo_icon.png"));
	
	// Setup the SIGNAL SLOT connections
	connect(cancelButton,SIGNAL(released()),this,SLOT(close()));
}

