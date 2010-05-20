 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : GroupComboDelegate.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : A custom Qt4 delegate that builds a combobox to edit 
 *		    items in a model view setup. This combo delegate is 
 *		    specifically used for the group admin window.
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

#include "Config.h"

#include <QMessageBox>

#include "GroupComboDelegate.h"

using namespace std;
using namespace Qrap;

// Constructor
GroupComboDelegate::GroupComboDelegate (QObject* parent,
					QStringList vals,
					QStringList dVals) : QItemDelegate(parent)
{
	values = vals;
	displayValues = dVals; 
}



QWidget* GroupComboDelegate::createEditor (QWidget* parent,
				      const QStyleOptionViewItem &option,
				      const QModelIndex &index) const
{
	// Create the combobox
	QComboBox* editor = new QComboBox(parent);
	
	return editor;
}



void GroupComboDelegate::setEditorData (QWidget* editor,
				   const QModelIndex &index) const
{
	QVariant val = index.model()->data(index, Qt::DisplayRole);
	QComboBox* combo = static_cast<QComboBox*>(editor);
	
	// Populate the combobox with actual values
	combo->addItems(displayValues);
	
	// Populate the combobox with display values
	for( int i=0 ; i<values.count() ; i++ )
	{
		combo->setItemData(i,QVariant::QVariant(values[i]),Qt::UserRole);
	} // for
	
	combo->setCurrentIndex(combo->findData(val,Qt::UserRole));
}



void GroupComboDelegate::setModelData (QWidget* editor,
				  QAbstractItemModel *model,
				  const QModelIndex &index) const
{
	QComboBox* combo = static_cast<QComboBox*>(editor);
	
	// Set the data in the given model
	model->setData(index,combo->itemData(combo->currentIndex(),Qt::UserRole),Qt::EditRole);
}



void GroupComboDelegate::updateEditorGeometry (QWidget* editor,
					  const QStyleOptionViewItem &option,
					  const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

