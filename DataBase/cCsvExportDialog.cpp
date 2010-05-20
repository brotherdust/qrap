 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCsvExportDialog.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson (roeland.frans@gmail.com)
 *    Description : Export of tables or Query results to CSV
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

#include <iostream>
#include <string>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

#include "Config.h"
#include "cCsvExportDialog.h"

using namespace std;
using namespace Qrap;

cCsvExportDialog* cCsvExportDialog::msMainExportDialog;

//***************************************************************************
cCsvExportDialog::cCsvExportDialog (QWidget* parent, const QString& curTable):
	QWizard(parent), mCurTable(curTable), mRadioSelection(0)
{
	msMainExportDialog = this;
	setWindowIcon(QIcon(":images/logo_icon.png"));
	
	if (mCurTable.length() == 0) mCurTable = "site";
	
	// set up the pages
	setPage(Page_Intro, new cCsvExportIntroPage);
	setPage(Page_SelectTables, new cCsvExportSelectTablesPage);
	setPage(Page_EnterQuery, new cCsvExportEnterQueryPage);
	setPage(Page_OutputFile, new cCsvExportOutputFilePage);
	setPage(Page_OutputFolder, new cCsvExportOutputFolderPage);
	setPage(Page_Progress, new cCsvExportProgressPage);
	setPage(Page_Finished, new cCsvExportFinishedPage);
	
	// set the first page
	setStartId(Page_Intro);
	
	// set the style if we're not using a Mac
	#ifndef Q_WS_MAC
		setWizardStyle(ModernStyle);
	#endif
	
	setWindowTitle(tr("QRAP CSV Export Wizard"));
}

//*****************************************************************************************
cCsvExportIntroPage::cCsvExportIntroPage (QWidget* parent): QWizardPage(parent)
{
	string      curTable = cCsvExportDialog::msMainExportDialog->GetCurTable().toStdString();
	DbStructure dbs;
	string      curTableName;
	
	gDb.GetDbStructure(dbs);
	
	curTableName = dbs[curTable].mLabel;
	
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Export type selection"));

	QVBoxLayout* vboxLayout;
	QGridLayout* gridLayout;
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("This wizard will guide you through the process of exporting data to one or more CSV files. Please select the appropriate export type to perform."));
	mTopLabel->setTextFormat(Qt::PlainText);
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	gridLayout = new QGridLayout();
	mTypeLabel = new QLabel(tr("Export type"));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mTypeLabel->sizePolicy().hasHeightForWidth());
	mTypeLabel->setSizePolicy(sizePolicy);
	
	gridLayout->addWidget(mTypeLabel, 0, 0, 1, 1);
	
	mCurrentTableRadio = new QRadioButton(tr(string("Current table ("+curTableName+")").c_str()));
	QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
	sizePolicy1.setHorizontalStretch(2);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(mCurrentTableRadio->sizePolicy().hasHeightForWidth());
	mCurrentTableRadio->setSizePolicy(sizePolicy1);
	mCurrentTableRadio->setChecked(true);
	
	gridLayout->addWidget(mCurrentTableRadio, 0, 1, 1, 1);
	
	mMultipleTablesRadio = new QRadioButton(tr("Multiple tables"));
	
	gridLayout->addWidget(mMultipleTablesRadio, 1, 1, 1, 1);
	
	mQueryResultRadio = new QRadioButton(tr("Query result"));
	
	gridLayout->addWidget(mQueryResultRadio, 2, 1, 1, 1);
	
	
	vboxLayout->addLayout(gridLayout);
	setLayout(vboxLayout);
}

//**********************************************************************************************
int cCsvExportIntroPage::nextId () const
{
	if (mCurrentTableRadio->isChecked())
	{
		cCsvExportDialog::msMainExportDialog->SetCurRadioSelection(0);
		return cCsvExportDialog::Page_OutputFile;
	} else
	if (mMultipleTablesRadio->isChecked())
	{
		cCsvExportDialog::msMainExportDialog->SetCurRadioSelection(1);
		return cCsvExportDialog::Page_SelectTables;
	}
	
	// assume it's a query
	cCsvExportDialog::msMainExportDialog->SetCurRadioSelection(2);
	return cCsvExportDialog::Page_EnterQuery;
}

//**************************************************************************************************

cCsvExportSelectTablesPage::cCsvExportSelectTablesPage (QWidget* parent):
	QWizardPage(parent)
{
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Select the tables to export"));

	QVBoxLayout* vboxLayout;

	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Please select the tables you wish to export. Each table will be saved in a separate CSV file."));;
	mTopLabel->setTextFormat(Qt::PlainText);
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	mTableList = new QListWidget;
	
	vboxLayout->addWidget(mTableList);
	
	setLayout(vboxLayout);
	
	connect(mTableList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(SelectionChanged(QListWidgetItem*)));
}

//*************************************************************************************************************
void cCsvExportSelectTablesPage::SelectionChanged (QListWidgetItem* item)
{
	emit completeChanged();
}

//**************************************************************************************************************
void cCsvExportSelectTablesPage::initializePage ()
{
	DbStructure      dbs;
	QListWidgetItem* item;
	
	// clear the table list
	mTableList->clear();
	
	// get the database structure
	gDb.GetDbStructure(dbs);
	// run through the tables
	for (DbStructureIterator table=dbs.begin(); table != dbs.end(); table++)
	{
		// if the table's visible
		if (table->second.mVisible)
		{
			// add an item with the table's name
			item = new QListWidgetItem(tr(table->second.mLabel.c_str()));
			// set the user role data
			item->setData(Qt::UserRole, QVariant(table->first.c_str()));
			// set the check box usage
			item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			// set initial checked state
			item->setCheckState(Qt::Unchecked);
			// add the item to the table list
			mTableList->addItem(item);
		}
	}
}

//******************************************************************************************88
int cCsvExportSelectTablesPage::nextId () const
{
	return cCsvExportDialog::Page_OutputFolder;
}

//**************************************************************************************************
bool cCsvExportSelectTablesPage::isComplete () const
{
	// check that there are any checked entries in the table list
	int               i, count = mTableList->count(), exportCount = 0;
	QListWidgetItem*  item;
	cCsvExportDialog* exportDlg = cCsvExportDialog::msMainExportDialog;
	
	exportDlg->ClearExportTableList();
	
	for (i=0;i<count;i++)
	{
		item = mTableList->item(i);
		// if this item is checked
		if (item->checkState() == Qt::Checked)
		{
			exportDlg->AddExportTable(item->data(Qt::UserRole).toString().toStdString());
			exportCount++;
		}
	}
	
	return (exportCount > 0) ? true : false;
}

/**************************************************************************************************/

cCsvExportEnterQueryPage::cCsvExportEnterQueryPage (QWidget* parent): QWizardPage(parent)
{
	QSpacerItem* spacerItem;
	QVBoxLayout* vboxLayout;
	
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Save SQL query results to a CSV file"));

	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Please enter the SQL query to perform in the box below. The results of the query will be stored in the selected CSV file on the next page."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	mQueryEdit = new QLineEdit;
	
	vboxLayout->addWidget(mQueryEdit);
	
	mExampleLabel = new QLabel(tr("Example: select * from site where status='Experimental';"));
	
	vboxLayout->addWidget(mExampleLabel);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem);
	
	// set the main layout
	setLayout(vboxLayout);
	
	connect(mQueryEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChanged(const QString&)));
}

//*******************************************************************************88
void cCsvExportEnterQueryPage::TextChanged (const QString& text)
{
	emit completeChanged();
}

//**********************************************************************************
bool cCsvExportEnterQueryPage::isComplete () const
{
	// check that there is some text in the query box
	return (mQueryEdit->text().length() > 0) ? true : false;
}

//**********************************************************************************8
int cCsvExportEnterQueryPage::nextId () const
{
	// we now want to save the output file
	return cCsvExportDialog::Page_OutputFile;
}

//***********************************************************************************
bool cCsvExportEnterQueryPage::validatePage ()
{
	cCsvExportDialog* exportDlg = cCsvExportDialog::msMainExportDialog;
	
	// save the query
	exportDlg->SetExportQuery(mQueryEdit->text());
	return true;
}

//**************************************************************************************************
cCsvExportOutputFilePage::cCsvExportOutputFilePage (QWidget* parent): QWizardPage(parent)
{
	QSpacerItem* spacerItem;
	QVBoxLayout* vboxLayout;
	QHBoxLayout* hboxLayout;
	
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Select or enter output file name"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Enter the name of a file in which to store the CSV data."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	hboxLayout = new QHBoxLayout;
	mFilenameEdit = new QLineEdit;
	
	hboxLayout->addWidget(mFilenameEdit);
	
	mBrowseButton = new QPushButton(tr("Browse..."));
	
	hboxLayout->addWidget(mBrowseButton);
	vboxLayout->addLayout(hboxLayout);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem);
	
	setLayout(vboxLayout);
	
	connect(mFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChanged(const QString&)));
	connect(mBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseClicked()));
}

//*********************************************************************************************************
void cCsvExportOutputFilePage::TextChanged (const QString& text)
{
	emit completeChanged();
}

//****************************************************************************************
void cCsvExportOutputFilePage::BrowseClicked ()
{
	mFilenameEdit->setText(QFileDialog::getSaveFileName(this, tr("Save CSV File"), tr(""), tr("CSV files (*.csv)")));
}

//*****************************************************************************************
bool cCsvExportOutputFilePage::isComplete () const
{
	// check that there's text in the filename line edit
	return (mFilenameEdit->text().length() > 0) ? true : false;
}

//*********************************************************************************************
int cCsvExportOutputFilePage::nextId () const
{
	return cCsvExportDialog::Page_Progress;
}

//**********************************************************************************************
bool cCsvExportOutputFilePage::validatePage ()
{
	cCsvExportDialog* exportDlg = cCsvExportDialog::msMainExportDialog;
	
	// save the output file name
	exportDlg->SetOutputFile(mFilenameEdit->text());
	return true;
}

//**********************************************************************************************
cCsvExportOutputFolderPage::cCsvExportOutputFolderPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout* vboxLayout;
	QHBoxLayout* hboxLayout;
	QSpacerItem* spacerItem;
	
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Select or enter output folder"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Select or enter an output folder into which the tables' CSV files will be stored."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	hboxLayout = new QHBoxLayout();
	mFolderEdit = new QLineEdit;
	
	hboxLayout->addWidget(mFolderEdit);
	
	mBrowseButton = new QPushButton(tr("Browse..."));
	
	hboxLayout->addWidget(mBrowseButton);
	
	
	vboxLayout->addLayout(hboxLayout);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem);
	
	setLayout(vboxLayout);
	
	connect(mFolderEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChanged(const QString&)));
	connect(mBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseClicked()));
}

//************************************************************************************
void cCsvExportOutputFolderPage::TextChanged (const QString& text)
{
	emit completeChanged();
}

//***************************************************************************************
void cCsvExportOutputFolderPage::BrowseClicked ()
{
	mFolderEdit->setText(QFileDialog::getExistingDirectory(this, tr("Select Output Folder"), tr("")));
}

//**************************************************************************************
bool cCsvExportOutputFolderPage::isComplete () const
{
	return (mFolderEdit->text().length() > 0) ? true : false;
}

//****************************************************************************************
int cCsvExportOutputFolderPage::nextId () const
{
	cCsvExportDialog::msMainExportDialog->SetOutputFolder(mFolderEdit->text());
	return cCsvExportDialog::Page_Progress;
}

//**************************************************************************************************
cCsvExportProgressPage::cCsvExportProgressPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout* vboxLayout;
	QHBoxLayout* hboxLayout;
	QSpacerItem* spacerItem, *spacerItem1;
	
	setTitle(tr("QRAP CSV Export"));
	setSubTitle(tr("Exporting data to CSV"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Click the Next button to begin the export."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	mProgressBar = new QProgressBar;
	mProgressBar->setValue(0);
	mProgressBar->setTextVisible(false);
	
	vboxLayout->addWidget(mProgressBar);
	
	hboxLayout = new QHBoxLayout();
	spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	hboxLayout->addItem(spacerItem);
	
	mProgressLabel = new QLabel(tr("Ready."));
	
	hboxLayout->addWidget(mProgressLabel);
	vboxLayout->addLayout(hboxLayout);
	
	spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem1);
	
	// set the main layout
	setLayout(vboxLayout);
}

//**************************************************************************************
bool cCsvExportProgressPage::isComplete () const
{
	// this page is only complete when the progress bar reaches 100
	//return (mProgressBar->value() == 100) ? true : false;
	return true;
}

//*****************************************************************************************
bool cCsvExportProgressPage::validatePage ()
{
	// now time to actually export...
	cCsvExportDialog* exportDlg = cCsvExportDialog::msMainExportDialog;
	string            curTable, curFile, query;
	StringArray       tableList;
	int               i, count;
	
	
	exportDlg->SetExportSuccess(false);
	mTopLabel->setText(tr("Exporting to CSV..."));
	
	// check which type of export is required
	switch (exportDlg->GetCurRadioSelection())
	{
	// exporting the current table
	case 0:
		curTable = exportDlg->GetCurTable().toStdString();
		mProgressLabel->setText(tr(curTable.c_str()));
		repaint();
		// export the current table
		if (!gDb.ExportToCsv(curTable, exportDlg->GetOutputFile().toStdString()))
		{
			mTopLabel->setText(tr("Error while exporting to file."));
			return false;
		}
		break;
	
	// exporting multiple tables
	case 1:
		// get the export table list
		exportDlg->GetExportTableList(tableList);
		count = tableList.size();
		for (i=0;i<count;i++)
		{
			// set the progress bar's label to the current table's name
			mProgressLabel->setText(tr(tableList[i].c_str()));
			// set the progress
			mProgressBar->setValue((int)(((float)i/(float)count)*100.0));
			repaint();
			curFile = exportDlg->GetOutputFolder().toStdString()+"/"+tableList[i]+".csv";
			if (!gDb.ExportToCsv(tableList[i], curFile))
			{
				mTopLabel->setText(tr(string("Error while exporting to file "+curFile).c_str()));
				return false;
			}
		}
		break;
	
	// exporting a query
	case 2:
		// get the query
		query = exportDlg->GetExportQuery().toStdString();
		curFile = exportDlg->GetOutputFile().toStdString();
		// export it to the file
		if (!gDb.ExportCmdToCsv(query, curFile))
		{
			mTopLabel->setText(tr(string("Error while exporting command \""+query+"\" to file \""+curFile+"\".").c_str()));
			return false;
		}
		break;
	
	default:
		return false;
	}
	
	mProgressBar->setValue(100);
	mProgressLabel->setText(tr("Done."));
	repaint();

	exportDlg->SetExportSuccess(true);
	
	return true;
}

//**************************************************************************************************
cCsvExportFinishedPage::cCsvExportFinishedPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout* vboxLayout;
	
	setTitle(tr("QRAP CSV Export"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel;
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	setLayout(vboxLayout);
}

//****************************************************************************************************
void cCsvExportFinishedPage::initializePage ()
{
	if (cCsvExportDialog::msMainExportDialog->GetExportSuccess())
	{
		mTopLabel->setText(tr("CSV file export successful. Click Finish to close the dialog."));
		setSubTitle(tr("CSV export successful"));
	} else
	{
		mTopLabel->setText(tr("CSV export unsuccessful. Click Finish to close the dialog."));
		setSubTitle(tr("CSV export unsuccessful"));
	}
}
