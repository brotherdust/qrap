 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : FormCombo.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : A custom Qt4 widget that inherits from QComboBox and defines a 
 *    		    custom SIGNAL to connec to with the table view tableWidgetItem.
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

#ifndef Qrap_FormCombo_h
#define Qrap_FormCombo_h

#include <QComboBox>
#include <QString>

#include "Config.h"

namespace Qrap
{
	/**
	* The FormCombo class is a custom class that inherits the QComboBox class. The purpose of this
	* class is to define a custom SIGNAL and SLOT that can be connected to the GuiEngine class item 
	* that will update the relevant cell's data.
	*/
	
	class FormCombo : public QComboBox
	{
		Q_OBJECT
		Q_PROPERTY(QString caption READ caption WRITE setCaption)
		
	public:
		/**
		* The constructor, this does not doe anything specific except pass the QComboBox constructor the 
		* necessary parent pointer.
		*/
		FormCombo (QWidget* parent=0);
		
		QString caption () const;
		void setCaption (QString val);
	};
}
#endif

