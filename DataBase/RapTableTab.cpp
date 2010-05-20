 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : RapTableTab.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that will display a table view in  
 *                  the one tab and edit form on the other tab.
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
 
#include <QMessageBox>
#include "Config.h"
#include "RapTableTab.h"

using namespace std;
using namespace Qrap;

// Constructor that creates all the GuiEngine classes for each database table
RapTableTab::RapTableTab (QWidget* parent, QString table) : QTabWidget(parent)
{
	tableName = table;
	form = NULL;
	inserting = false;
	tableView = new QTableWidget(this);
	
	// Create the scroll area
	scroller = new QScrollArea;
	
	// Add the table widget to the tab
	addTab(tableView,"Table View");
	addTab(scroller,"Form View");
	
	//Customize the tab widget
	setTabShape(QTabWidget::Triangular);
	setTabPosition(QTabWidget::West);
	
	connect(this,SIGNAL(currentChanged(int)),this,SLOT(TabIndexChanged(int)));

}



// Create the insert form
void RapTableTab::CreateInsertForm ()
{
	cout << "Creating the insert form" << endl;
	
	// First delete the existing form
	TabIndexChanged(0);
	
	// Create the new insert form
	inserting = true;
	
	if(currentIndex()!= 1)
		setCurrentIndex(1);
	else
		TabIndexChanged(1);
}



// Manage the insertRowAct, deleteRowAct and reloadTableAct actions
void RapTableTab::TabIndexChanged (int index)
{
	QWidget* formWidget;
	
	cout << "The current index changed" << endl;
	
	switch (index)
	{
		case 0:
			// Destroy the formWidget
			formWidget = scroller->takeWidget();
			//formWidget->close();
			form = NULL;
			inserting = false;
			break;
		case 1:
			// Create the form
			if(inserting)
			{
				form = new RapFormBuilder(this,tableName,tableView,true);
			}
			else
				form = new RapFormBuilder(this,tableName,tableView);
			
			scroller->setWidget(form);
			break;
		default:
			// Do nothing
			break;
	} // switch
}
