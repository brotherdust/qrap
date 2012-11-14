 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapTablesTab.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that will display a table view in the  
 *                  one tab and a edit form on the other tab.
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
#include "cRapTableTab.h"

using namespace std;
using namespace Qrap;

// Constructor that creates all the GuiEngine classes for each database table
cRapTableTab::cRapTableTab (QWidget* parent, QString table) : QTabWidget(parent)
{
	mTableName = table;
	mForm = NULL;
	mInserting = false;
	mTableView = new QTableWidget(0,1,this);
	QSizePolicy sizePolicy;
	sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
	sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
	mTableView->setSizePolicy(sizePolicy);
	
	// Create the scroll area
	mScroller = new QScrollArea;
	
	// Add the table widget to the tab
	addTab(mTableView,"Table View");
	addTab(mScroller,"Form View");
	
	//Customize the tab widget
	setTabShape(QTabWidget::Triangular);
	setTabPosition(QTabWidget::West);
	
	
	connect(this,SIGNAL(currentChanged(int)),this,SLOT(TabIndexChanged(int)));

}


//*****************************************************************************************
// Create the insert form
void cRapTableTab::CreateInsertForm ()
{
	cout << " In cRapTableTab::CreateInsertForm () " << endl;
	// First delete the existing form
	TabIndexChanged(0);
	
	// Create the new insert form
	mInserting = true;
	
	if(currentIndex()!= 1)
		setCurrentIndex(1);
	else
		TabIndexChanged(1);
}

//************************************************************************************************
//
void cRapTableTab::CreateUpdateForm ()
{
	cout << " In cRapTableTab::CreateUpdateForm () " << endl;
	// First delete the existing form
	TabIndexChanged(0);
	
	// Create the new insert form
	if(currentIndex()!= 1)
		setCurrentIndex(1);
	else
		TabIndexChanged(1);
}

//***************************************************************************************************
// Manage the insertRowAct, deleteRowAct and reloadTableAct actions
void cRapTableTab::TabIndexChanged (int index)
{
	QWidget* formWidget;
	cout << "cRapTableTab::TabIndexChanged mTableView->rowCount()=" << mTableView->rowCount() << endl;	
	switch (index)
	{
		case 0:
			// Destroy the formWidget
			formWidget = mScroller->takeWidget();
			//formWidget->close();
			mForm = NULL;
			mInserting = false;
			*mTableViewSelected = true;
			break;
		case 1:
			// Check if there is data in the tableview
			if(mTableView->rowCount()<=0)
			{
				mInserting = true;
			}
						
			// Create the form
			if(mInserting)
			{
				cout << "cRapTableTab::TabIndexChanged:  Voor vorm ... inserting" << endl;
				mForm = new cRapFormBuilder(mRef,this,mTableName,mTableView,mInserting);
				mForm->SetReferences(mRef);
			}
			else
			{
				cout << "cRapTableTab::TabIndexChanged:  Voor vorm ... not inserting" << endl;
				mForm = new cRapFormBuilder(this,mTableName,mTableView,mInserting);
				mForm->SetReferences(mRef);
			}
			cout << "cRapTableTab::TabIndexChanged:  Na vorm." << endl;
			mScroller->setWidget(mForm);
			*mTableViewSelected = false;
			break;
		default:
			// Do nothing
			break;
	} // switch
}
