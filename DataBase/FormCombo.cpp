 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : FormCombo.cpp
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


#include "FormCombo.h"

using namespace std;
using namespace Qrap;

// Constructor
FormCombo::FormCombo (QWidget* parent) : QComboBox(parent)
{

}


//*****************************************************************************
QString FormCombo::caption () const
{
	int currentId = currentIndex();
	int id = itemData(currentId,Qt::UserRole).toInt();
	QString str = QString::number(id);
	
	str += ":" + itemData(currentId,Qt::DisplayRole).toString();
	
	
	return str;
}


//******************************************************************************
void FormCombo::setCaption (QString val)
{
	if(val.length()!=0)
	{
		QStringList parts = val.split(":");
		int id = parts[0].toInt();
		
		setCurrentIndex(findData(QVariant::QVariant(id),Qt::UserRole));
	}
}
