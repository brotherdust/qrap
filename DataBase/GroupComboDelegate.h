 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : GroupComboDelegate.h
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


#ifndef Qrap_GroupComboDelegate_h
#define Qrap_GroupComboDelegate_h

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
	
	/**
	 * The class inherits the standard QItemDelegate and overloads some of the functions in order
	 * to provide a custom delegate class that will display a combo box as the data editor. This class
	 * is different to the ComboDelegate class in the sense that it is a simple combo box that takes 
	 * a QStringList as a list of values to populate the combo box instead of getting the values from 
	 * the database engine.
	 */
	
	class GroupComboDelegate : public QItemDelegate
	{
		Q_OBJECT
	
	public:
		/**
		 * Constructor, this creates the custom delegate.
		 */
		GroupComboDelegate (QObject *parent = 0, 
				    QStringList vals = QStringList::QStringList(), 
				    QStringList dVals = QStringList::QStringList());
		
		/**
		 * Returns the widget used to edit the item specified by index for editing. 
		 * The parent widget and style option are used to control how the editor widget appears.
		 */
		QWidget* createEditor (QWidget* parent,
				const QStyleOptionViewItem &option,
				const QModelIndex &index) const;
		
		/**
		 * Sets the data to be displayed and edited by the editor for the item specified by index.
		 */
		void setEditorData (QWidget* editor,
				const QModelIndex &index) const;
		
		/**
		 * Sets the data for the specified model and item index from that supplied by the editor.
		 */
		void setModelData (QWidget* editor,
				QAbstractItemModel *model,
				const QModelIndex &index) const;
		
		/**
		 * Updates the editor for the item specified by index according to the style option given.
		 */
		void updateEditorGeometry (QWidget* editor,
					const QStyleOptionViewItem &option,
					const QModelIndex &index) const;
		
	private:
		QStringList displayValues;	///< A Qt4 String list that holds a more descriptive text describing each item value in the combo box.
		QStringList values;		///< A Qt4 String list that holds the values of the combo delegate.
	
	};
}
#endif
