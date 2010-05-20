 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRapCells.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *		  : (Magdaleen Ballot (magdaleen.ballot@up.ac.za))
 *    Description : This class displays all the tables related to a cells and
 *                  and cellparameters allows the user to edit the tables.
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
#include "cRapCells.h"
#include "cSearchForm.h"

using namespace Qrap;
using namespace std;


//***************************************************************
// The constructor
cRapCells::cRapCells (QWidget* parent) : QWidget(parent)
{
	// Create all the widget required
	mTableList = new QListWidget(this);
	
	// Setup the mTableList
	mTableList->setMinimumSize(140,400);
	mTableList->setMaximumWidth(140);
	
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
	mDbCommunicator = new RapDbCommunicator(this,mProgress);
	
	// Populate the table list
	PopulateTableList();
	
	tableViewSelected = true;
	mInsertingRow = false;
	
	// Setup all the signals and slots
	connect(mTableList,SIGNAL(itemSelectionChanged()),this,SLOT(TableSelectionChanged()));
	connect(mTables["cell"]->GetTable(),SIGNAL(itemSelectionChanged()),this,SLOT(CellSelectionChanged()));
	
	// Set the default table selection to be the first row
	if(mCurrentTable->GetTable()->rowCount()!=0)
	{
		mCurrentTable->GetTable()->setCurrentCell(0,0);
	} // if
}


//**********************************************************************
// Destructor
cRapCells::~cRapCells ()
{
	// delete the RapDbCommunicator object
	delete mDbCommunicator;
}


//********************************************************************
// Insert a new row into the table and database engine
void cRapCells::InsertRow ()
{
	// Make sure that the object knows that we're inserting a new row
	mInsertingRow = true;
	
	// Create the insert form and run the insert query
	mCurrentTable->CreateInsertForm();
	
	// Let the GuiEngine object know that inserting is complete
	mInsertingRow = false;
}


//*******************************************************************
// Delete the selected rows
void cRapCells::DeleteRows ()
{
	// Perform the delete
	mDbCommunicator->DeleteRows(mCurrentTable->GetTable(),mTables.key(mCurrentTable));
}


//********************************************************************
// Reload the current table contents
void cRapCells::ReloadTable ()
{
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	PopulateTable();
	if (!tableViewSelected)
		mCurrentTable->CreateUpdateForm();
		
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}


//***********************************************************************
// Show all the contents of the current Table
void cRapCells::ShowAllContents ()
{
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	PopulateTable();
	
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}


//************************************************************************
// Copy current cells contents into the clipboard
void cRapCells::Copy ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		clipboard->setText(mCurrentTable->GetTable()->currentItem()->data(Qt::DisplayRole).toString());
	}
}


//*************************************************************************
// Paste the current text from the clipboard to the current cell
void cRapCells::Paste ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		mCurrentTable->GetTable()->currentItem()->setData(Qt::DisplayRole,QVariant::QVariant(clipboard->text()));
	}
}


//************************************************************************
// Cut the data from the current cell
void cRapCells::Cut ()
{
	if(mCurrentTable->GetTable()->currentItem()!=NULL)
	{
		QClipboard* clipboard = static_cast<MainWindow*>(window())->clipboard;
		clipboard->setText(mCurrentTable->GetTable()->currentItem()->data(Qt::DisplayRole).toString());
		mCurrentTable->GetTable()->currentItem()->setData(Qt::DisplayRole,QVariant::QVariant(""));
	}
}


//*************************************************************************
// Search in the currently selected table
void cRapCells::Search ()
{
	cSearchForm* searchForm = new cSearchForm(0,mCurrentTable->mTableName,this);
	
	searchForm->show();
}


//******************************************************************************
// Excute the search
void cRapCells::ExecuteSearch (string search)
{
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mCurrentTable->mTableName,"",-1,false,false,search);
	
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}

//*******************************************************************************
// Make sure that the correct table is made visible
void cRapCells::TableSelectionChanged ()
{
	QListWidgetItem* selectedItem = mTableList->currentItem();
	
	// Remove the previous table from the Grid layout
	mMainLayout->removeWidget(mCurrentTable);
	mCurrentTable->setVisible(false);
	
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	mCurrentTable = mTables[selectedItem->data(Qt::UserRole).toString()];
	mMainLayout->addWidget(mCurrentTable,0,1);
	
	// populate the current table
	if(mCurrentTable->GetTable()->rowCount()==0)
	{
		PopulateTable();
	}

	
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
	
	// Check whether the form view is selected or the table view
	if(tableViewSelected)
		mCurrentTable->setCurrentIndex(0);
	else
		mCurrentTable->setCurrentIndex(1);
	
	
	// Make sure that the MainWindow object knows what the current table is
	static_cast<MainWindow*>(window())->currentTable = getCurrentTableName();
	
	mCurrentTable->setVisible(true);
}


//***************************************************************************
// Populate the tables with details related to the selected site
void cRapCells::CellSelectionChanged ()
{
	if(!mInsertingRow)
	{
		if (mCurrentTable->GetTable()->currentRow()==-1)
			return;
		
		// Store the current site Id
		mCurrentCellId = mCurrentTable->GetTable()->item(mCurrentTable->GetTable()->currentRow(),0)->data(Qt::DisplayRole).toInt();
		
		QMapIterator<QString,cRapTableTab*> it(mTables);
		
		// Populate the different tables
		while(it.hasNext())
		{
			it.next();
			
			// Clear all the contents of the tables
			if(it.key() != "cell")
			{
				it.value()->GetTable()->clear();
				it.value()->GetTable()->setRowCount(0);
			}
		} // while
	} // if
	
}


//***********************************************************************
// Update the database
void cRapCells::UpdateDatabase(int row, int col)
{
	if(!mInsertingRow)
	{
		// Check whether the 
		// Perform the update
		mDbCommunicator->UpdateDatabase(mCurrentTable->GetTable(),mTables.key(mCurrentTable),row,col);
	} // if
}


//************************************************************************
// Populate the current table
void cRapCells::PopulateTable ()
{
	mDbCommunicator->PopulateTable(mCurrentTable->GetTable(),mTables.key(mCurrentTable),"ci",-1,false,true);
}


//**************************************************************************
// Populate the mTableList
void cRapCells::PopulateTableList ()
{
	// Create the items
	QListWidgetItem* cellItem = new QListWidgetItem("Cell");
	QListWidgetItem* frequencyAllocationItem = new QListWidgetItem("Frequency Allocation");
	QListWidgetItem* parametersettingsItem = new QListWidgetItem("Parameter Settings");
	QListWidgetItem* cellparameterItem = new QListWidgetItem("Cell Parameters List");
	
	// Setup the items
	cellItem->setData(Qt::UserRole,QVariant::QVariant("cell"));
	cellItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	frequencyAllocationItem->setData(Qt::UserRole,QVariant::QVariant("frequencyallocationlist"));
	frequencyAllocationItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	parametersettingsItem->setData(Qt::UserRole,QVariant::QVariant("parametersettings"));
	parametersettingsItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	cellparameterItem->setData(Qt::UserRole,QVariant::QVariant("cellparameters"));
	cellparameterItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	
	// Add the items to the mTableList
	mTableList->addItem(cellItem);
	mTableList->addItem(frequencyAllocationItem);
	mTableList->addItem(parametersettingsItem);
	mTableList->addItem(cellparameterItem);
	
	// Create the different table widgets
	mTables.insert("cell",new cRapTableTab(this,"cell"));
	mTables.insert("frequencyallocationlist",new cRapTableTab(this,"frequencyallocationlist"));
	mTables.insert("parametersettings",new cRapTableTab(this,"parametersettings"));
	mTables.insert("cellparameters",new cRapTableTab(this,"cellparameters"));
	
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
		it.value()->GetTable()->setMinimumSize(600,400);
		it.value()->setVisible(false);
		it.value()->mTableViewSelected = &tableViewSelected;
		
		// Load the headers for each table
		//mDbCommunicator->LoadTableHeaders(it.value()->GetTable(),it.key(),"ci");
	} // while
	
	// Make sure that the site table is visible
	mMainLayout->addWidget(mTables["cell"],0,1);
	mTables["cell"]->show();
	mCurrentTable = mTables["cell"];
	
	
	// Make sure that no update signals are triggered
	disconnect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),0,0);
	
	PopulateTable();
	
	// Reconnect the update signals
	connect(mCurrentTable->GetTable(),SIGNAL(cellChanged(int,int)),this,SLOT(UpdateDatabase(int,int)));
}
