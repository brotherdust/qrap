 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : ComboDelegate.h
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

#ifndef Qrap_ComboDelegate_h
#define Qrap_ComboDelegate_h

#include <QWidget>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QStringList>
#include <QAbstractItemModel>
#include <QComboBox>

#include "Config.h"

namespace Qrap
{
	class ComboDelegate : public QItemDelegate
	{
		Q_OBJECT
	
	public:
		int formCombo;
		QString fieldName;
		QString tableName;
		
		ComboDelegate (QObject *parent = 0);
		
		QWidget* createEditor (QWidget* parent,
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
		
		void setEditorData (QWidget* editor,
				const QModelIndex &index) const;
		
		void setModelData (QWidget* editor,
				QAbstractItemModel *model,
				const QModelIndex &index) const;
		
		void updateEditorGeometry (QWidget* editor,
					const QStyleOptionViewItem &option,
					const QModelIndex &index) const;
	
	};
}
#endif
