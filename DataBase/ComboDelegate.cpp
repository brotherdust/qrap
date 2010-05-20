 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : ComboDelegate.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : A custom Qt4 delegate that builds a combobox to edit 
 *		    items in a model view setup.
 *
**************************************************************************
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

#include "ComboDelegate.h"

using namespace std;
using namespace Qrap;

ComboDelegate::ComboDelegate (QObject* parent) : QItemDelegate(parent)
{

}



QWidget* ComboDelegate::createEditor (QWidget* parent,
				      const QStyleOptionViewItem &option,
				      const QModelIndex &index) const
{
	// Create the combobox
	QComboBox* editor = new QComboBox(parent);
	
	return editor;
}



void ComboDelegate::setEditorData (QWidget* editor,
				   const QModelIndex &index) const
{
	QVariant val = index.model()->data(index, Qt::DisplayRole);
	QComboBox* combo = static_cast<QComboBox*>(editor);
	
	// Populate the combobox with selection values
	StringIntArray vals;
	StringIntArrayIterator it;
	int i=1;
	
	gDb.GetFieldUiParams(tableName.toStdString(),fieldName.toStdString(),vals);
	
	combo->insertItem(0,"0:NULL");
	
	for( it=vals.begin() ; it!=vals.end() ; it++ )
	{
		QString temp = QString::number(it->first) + ":" + QString::fromStdString(it->second);
		combo->insertItem(i,temp,qVariantFromValue(it->first));
		i++;
	}
	
	combo->setCurrentIndex(combo->findText(val.toString(),Qt::MatchCaseSensitive));
}



void ComboDelegate::setModelData (QWidget* editor,
				  QAbstractItemModel *model,
				  const QModelIndex &index) const
{
	QComboBox* combo = static_cast<QComboBox*>(editor);
	
	// Set the data in the given model
	model->setData(index,combo->itemText(combo->currentIndex()),Qt::EditRole);
}



void ComboDelegate::updateEditorGeometry (QWidget* editor,
					  const QStyleOptionViewItem &option,
					  const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

