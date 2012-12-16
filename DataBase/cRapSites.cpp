 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapSites.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *		  : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class displays all the tables related to a sites and
 *                  and radio installations allows the user to edit the tables.
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
#include "cRapSites.h"
#include "cSearchForm.h"

using namespace Qrap;
using namespace std;


//****************************************************************
// The constructor
cRapSites::cRapSites (QWidget* parent) : QWidget(parent)
{
	cout << "In RabSites constructor" << endl;
	// Create all the widget required
	mTableList = new QListWidget(this);
	
	// Setup the mTableList
	mTableList->setMinimumSize(140,400);
	mTableList->setMaximumWidth(200);
	
	// create the mProgress bar
	mProgress = new QProgressBar(this);
	mProgress->setMaximumWidth(120);
	mProgress->setVisible(false);
	mProgress->reset();
	
	// Create the layouts
	mMainLayout = new QGridLayout(this);
	QVBoxLayout* listLayout = new QVBoxLayout();
	
	listLayout->addWidget(mTableList);
	listLayout->addWidget(mProgress);
	
	// Setup mMainLayout
	mMainLayout->addLayout(listLayout,0,0);
	mMainLayout->setSpacing(5);	
	setLayout(mMainLayout);
	
	// Create the database communicator class
	cout << " before RapDbCommunicator constructor" << endl;
	mDbCommunicator = new RapDbCommunicator(this,mProgress);
	
	// Populate the table list
	cout << " before PopulateTableList" << endl;
	PopulateTableList();
	
	mTableViewSelected = true;
	
	mInsertingRow = false;
		
	// Setup all the signals and slots
	connect(mTables["site"]->GetTable(),SIGNAL(itemSelectionChanged()),this,SLOT(SiteSelectionChanged()));
	connect(mTableList,SIGNAL(itemSelectionChanged()),this,SLOT(TableSelectionChanged()));
	// Set the default table selection to be the first row
	if(mCurrentTable->GetTable()->rowCount()>0)
	{
		mCurrentTable->GetTable()->setCurrentCell(0,0);
		mCurrentTable->GetTable()->selectRow(0);
	} // if
	else
	{
		mCurrentTable->TabIndexChanged(1); 
	}
	cout << " before the rest" << endl;


}


//********************************************************************************
// Destructor
cRapSites::~cRapSites ()
{
	// delete the RapDbCommunicator object
	delete mDbCommunicator;
}


//*******************************************************************************
// Insert a new row into the table and database engine
void cRapSites::InsertRow ()
{
	// Make sure that the object knows that we're inserting a new row
	mInsertingRow = true;
	
	// Create the insert form and run the insert query
	mCurrentTable->CreateInsertForm();
	
	// Perform the insert
//	mDbCommunicator->InsertRow(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"siteid",mCurrentSiteId,false,true);
	
	// Let the GuiEngine object know that inserting is complete
	mInsertingRow = false;
}


//*********************************************************************************
// Delete the selected rows
void cRapSites::DeleteRows ()
{
	// Perform the delete
	mDbCommunicator->DeleteRows(mCurrentTable->GetTable(),mTables.key(mCurrentTable));
}


//***********************************************************************************
// Reload the current table contents
void cRapSites::ReloadTable ()
{
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	cout << "In cRapSites::ReloadTable" << endl;
	if(mTables.key(mCurrentTable)!="site")
	{
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"siteid",mCurrentSiteId,false,true);
	}
	else
	{
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"id",-1,true,true);
	}
	if (!mTableViewSelected)
		mCurrentTable->CreateUpdateForm();
	
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}


//**********************************************************************************
// Show all the contents of the current Table
void cRapSites::ShowAllContents ()
{
	cout << "In cRapSites::ShowAllContents" << endl;	
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	if(mTables.key(mCurrentTable)!="site")
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"siteid",-1,false,true);
	else
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"id",-1,true,true);
	
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}


//***********************************************************************************
// Copy current cells contents into the clipboard
void cRapSites::Copy ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		clipboard->setText(mCurrentTable->GetTable()->currentItem()->data(Qt::DisplayRole).toString());
	}
}


//************************************************************************************
// Paste the current text from the clipboard to the current cell
void cRapSites::Paste ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		mCurrentTable->GetTable()->currentItem()->setData(Qt::DisplayRole,QVariant(clipboard->text()));
	}
}


//************************************************************************************
// Cut the data from the current cell
void cRapSites::Cut ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		clipboard->setText(mCurrentTable->GetTable()->currentItem()->data(Qt::DisplayRole).toString());
		mCurrentTable->GetTable()->currentItem()->setData(Qt::DisplayRole,QVariant(""));
	}
}


//***************************************************************************************
// Search in the currently selected table
void cRapSites::Search ()
{
	cSearchForm* searchForm = new cSearchForm(0,mCurrentTable->mTableName,this);
	
	searchForm->show();
}


//*************************************************************************************
// Excute the search
void cRapSites::ExecuteSearch (string search)
{
	QString table = mCurrentTable->mTableName;
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	cout << "In cRapSites::ExecuteSearch" << endl;	
	if(mTables.key(mCurrentTable)!="site")
	{
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),table,"",-1,false,true,search);
	}
	else
	{
		mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),table,"",-1,false,true,search);
	}
	//mCurrentTable->GetTable()->selectAll();
	//mCurrentTable = mTables[table];
	//mCurrentTable->GetTable()->selectAll();
	//mTables[mCurrentTable->tableName];
	// Set the default table selection to be the first row
	
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));

	if(mCurrentTable->GetTable()->rowCount()!=0)
	{
		mCurrentTable->GetTable()->setCurrentCell(0,0);
	} // if
	else
	{
		mCurrentTable->TabIndexChanged(0); 
	}

}


//*********************************************************************************
// Make sure that the correct table is made visible
void cRapSites::TableSelectionChanged ()
{
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	QListWidgetItem* selectedItem = mTableList->currentItem();
	cout << " Entering cRapSites::TableSelectionChanged	Table = " << selectedItem->data(Qt::UserRole).toString().toStdString().c_str() << endl;	
	
	// Remove the previous table from the Grid layout
	mCurrentTable->setVisible(false);
	mMainLayout->removeWidget(mCurrentTable);
	
	mCurrentTable = mTables[selectedItem->data(Qt::UserRole).toString()];
	mSiteTable = mTables["site"];
	mCurrentTable->mTableViewSelected=&mTableViewSelected;
	mMainLayout->addWidget(mCurrentTable,0,1);
	mDbCommunicator->PopulateTable(mSiteTable->GetTable(),selectedItem->data(Qt::UserRole).toString(),"id",mCurrentSiteId,false,true);
	// populate the current table
	if(mCurrentTable->GetTable()->rowCount()==0)
	{
		if(mTables.key(mCurrentTable)!="site")
		{
			mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),selectedItem->data(Qt::UserRole).toString(),"siteid",mCurrentSiteId,false,true);
		}
		else
		{
			mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),selectedItem->data(Qt::UserRole).toString(),"id",mCurrentSiteId,true,true);
		}
		if (mCurrentTable->GetTable()->rowCount()<=0)
		{
			StringMap ref;
			QString Site = QString("%1").arg(mCurrentSiteId);
			ref["siteid"] = Site.toStdString();
			mCurrentTable->SetReferences(ref);
			mCurrentTable->TabIndexChanged(1);
		} 
	}
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));	
	if(mCurrentTable->GetTable()->rowCount()>0)
	{
		mCurrentTable->GetTable()->setCurrentCell(0,0);
		mCurrentTable->GetTable()->selectRow(0);
	} // if
	else
	{
		mTableViewSelected=false; 
	}
	// Check whether the form view is selected or the table view
	if(mTableViewSelected)
		mCurrentTable->setCurrentIndex(0);
	else
		mCurrentTable->setCurrentIndex(1);
	
	// Make sure that the MainWindow object knows what the current table is
	static_cast<MainWindow*>(window())->currentTable = getCurrentTableName();

	mCurrentTable->setVisible(true);
}


//***********************************************************************************
// Populate the tables with details related to the selected site
void cRapSites::SiteSelectionChanged ()
{
	if(!mInsertingRow)
	{
		if (mCurrentTable->GetTable()->currentRow()==-1)
			return;

		if (mCurrentTable->GetTable()->rowCount()<2)
		{
			mCurrentSiteId=0;
			return;
		}
		
		cout << " cRapSites::SiteSelectionChanged Before mCurrentSiteID" << endl;
		// Store the current site Id
		mCurrentSiteId = mCurrentTable->GetTable()->item(mCurrentTable->GetTable()->currentRow(),0)->data(Qt::DisplayRole).toInt();
		
		cout << " cRapSites::SiteSelectionChanged After mCurrentSiteID" << endl;
		QMapIterator<QString,cRapTableTab*> it(mTables);
		
		// Populate the different tables
		while(it.hasNext())
		{
			it.next();
			
			// Clear all the contents of the tables
			if(it.key() != "site")
			{
				it.value()->GetTable()->clear();
				it.value()->GetTable()->setRowCount(0);
			}
		} // while
	} // if
	
}


//*********************************************************************************
// Update the database
void cRapSites::UpdateDatabase(int row, int col)
{
	if(!mInsertingRow)
	{
		// Check whether the 
		// Perform the update
		mDbCommunicator->UpdateDatabase(mCurrentTable->GetTable(),mTables.key(mCurrentTable),row,col);
	} // if
}


//**************************************************************************************
// Populate the mTableList
void cRapSites::PopulateTableList ()
{
	// Create the items
	QListWidgetItem* siteItem = new QListWidgetItem("Site");
	QListWidgetItem* siteDescriptionItem = new QListWidgetItem("Site Description");
	QListWidgetItem* siteContactsItem = new QListWidgetItem("Site Contacts");
	QListWidgetItem* radioInstallationItem = new QListWidgetItem("Radio Installation");
	
	// Setup the items
	siteItem->setData(Qt::UserRole,QVariant("site"));
	siteItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	siteDescriptionItem->setData(Qt::UserRole,QVariant("sitedescription"));
	siteDescriptionItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	siteContactsItem->setData(Qt::UserRole,QVariant("sitecontacts"));
	siteContactsItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	radioInstallationItem->setData(Qt::UserRole,QVariant("radioinstallation"));
	radioInstallationItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	
	// Add the items to the mTableList
	mTableList->addItem(siteItem);
	mTableList->addItem(siteDescriptionItem);
	mTableList->addItem(siteContactsItem);
	mTableList->addItem(radioInstallationItem);
	
	// Create the different table widgets
	mTables.insert("site",new cRapTableTab(this,"site"));
	mTables.insert("sitedescription",new cRapTableTab(this,"sitedescription"));
	mTables.insert("sitecontacts",new cRapTableTab(this,"sitecontacts"));
	mTables.insert("radioinstallation",new cRapTableTab(this,"radioinstallation"));
	
	QMapIterator<QString,cRapTableTab*> it(mTables);
	
	// Setup the different tables
	while(it.hasNext())
	{
		it.next();
		
		// Setup the table
		it.value()->GetTable()->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
		it.value()->GetTable()->setSelectionBehavior(QAbstractItemView::SelectRows);
		it.value()->GetTable()->setSelectionMode(QAbstractItemView::ContiguousSelection);
		it.value()->GetTable()->verticalHeader()->setVisible(false);
		//it.value()->GetTable()->setMinimumSize(600,400);
		it.value()->setVisible(false);
		it.value()->mTableViewSelected = &mTableViewSelected;
		
		// Load the headers for each table
		mDbCommunicator->LoadTableHeaders(it.value()->GetTable(),it.key(),"siteid");
	} // while
	
	// Make sure that the site table is visible
	mMainLayout->addWidget(mTables["site"],0,1);
	mTables["site"]->show();
	mCurrentTable = mTables["site"];
		
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	// Populate the site table first
	mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),"site","id",-1,true,true);
		
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}

//******************************************************************************************
void cRapSites::EditSite (int Id)
{

	mCurrentSiteId=Id;
	StringMap ref;
	QString Site = QString("%1").arg(mCurrentSiteId);
	ref["siteid"] = Site.toStdString();
	mCurrentTable->SetReferences(ref);
	mTableViewSelected=false;
	cout << "cRapSites::EditSite voor setCurrentRow(3)" << endl;
	mTableList->setCurrentRow(3);
	mCurrentTable->SetReferences(ref);
	cout << "cRapSites::EditSite voor setCurrentIndex(1)" << endl;
	mCurrentTable->setCurrentIndex(1);
}

