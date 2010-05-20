 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : LogDialog.h
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

#ifndef Qrap_LogDialog_h
#define Qrap_LogDialog_h


// Include Qt headers
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QTextEdit>

// Include local headers
#include "Config.h"
#include "MainWindow.h"

namespace Qrap
{
	/**
	* This class inherits QDialog and forms a simple dialog that displays the contents of a log message.
	*/
	
	class LogDialog : public QDialog
	{
		Q_OBJECT
	
	public:
		/**
		* Constructor that creates the dialog widgets and establishes the signal and slot connections.
		* 
		* @param parent A pointer to the parent object.
		* @param logMessage The log message to be displayed.
		*/
		LogDialog (QWidget *parent = 0, QString logMessage = "");
		
	private:
		QTextEdit*	log;			///< A pointer to a Qt4 text edit widget which will display the log message.
		QPushButton*	cancelButton;		///< A pointer to a Qt4 push button that closes the dialog.
		QLabel*		logLabel;		///< A pointer to a Qt4 label widget that displays the log title.
	};
}
#endif
