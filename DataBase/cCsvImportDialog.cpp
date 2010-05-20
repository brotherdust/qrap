/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCsvImportDialog.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson ((roeland.frans@gmail.com)
 *    Description : Simple importing CSV files in specific formats to QRap tables 
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
#include <dirent.h>
#include <sys/types.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "Config.h"
#include "cCsvImportDialog.h"

using namespace std;
using namespace Qrap;

cCsvImportDialog* cCsvImportDialog::msMainImportDialog;

/**************************************************************************************************/

cCsvImportDialog::cCsvImportDialog (QWidget* parent, const QString& curTable): QWizard(parent),
	mCurTable(curTable), mImportType(0), mDegFormat(0)
{
	msMainImportDialog = this;
	setWindowIcon(QIcon(":images/logo_icon.png"));
	
	if (mCurTable.length() == 0) mCurTable = "site";
	
	// set up the pages
	setPage(Page_Intro, new cCsvImportIntroPage);
	setPage(Page_InputFilename, new cCsvImportFilenamePage);
	setPage(Page_InputFolder, new cCsvImportFolderPage);
	setPage(Page_Progress, new cCsvImportProgressPage);
	setPage(Page_Finished, new cCsvImportFinishedPage);
	
	// set the first page
	setStartId(Page_Intro);
	
	// set the style if we're not using a Mac
	#ifndef Q_WS_MAC
		setWizardStyle(ModernStyle);
	#endif
	
	setWindowTitle(tr("QRAP CSV Import Wizard"));
}

/**************************************************************************************************/

cCsvImportIntroPage::cCsvImportIntroPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout, *vboxLayout1, *vboxLayout2;
	QHBoxLayout *hboxLayout, *hboxLayout1;;
	QWidget     *widget, *verticalLayout_2, *widget_2, *verticalLayout_3;
//	QSpacerItem *spacerItem;
	
	string      curTable = cCsvImportDialog::msMainImportDialog->GetCurTable().toStdString();
	DbStructure dbs;
	string      curTableName;
	
	gDb.GetDbStructure(dbs);
	curTableName = dbs[curTable].mLabel;
	
	setTitle(tr("QRAP CSV Import"));
	setSubTitle(tr("Import type selection"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("This wizard will guide you through the process of importing data from CSV files into the database. You may either import data from a single CSV file into the currently active table in the user interface, or you may import an entire folder of CSV files."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	hboxLayout = new QHBoxLayout;
	mImportLabel = new QLabel(tr("Import type"));
	QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(mImportLabel->sizePolicy().hasHeightForWidth());
	mImportLabel->setSizePolicy(sizePolicy);
	mImportLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	
	hboxLayout->addWidget(mImportLabel);
	
	widget = new QWidget;
	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy1.setHorizontalStretch(2);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
	widget->setSizePolicy(sizePolicy1);
	verticalLayout_2 = new QWidget(widget);
	verticalLayout_2->setGeometry(QRect(0, 0, 300, 54));
	vboxLayout1 = new QVBoxLayout(verticalLayout_2);
	vboxLayout1->setContentsMargins(0, 0, 0, 0);
	mCurTableRadio = new QRadioButton(tr(string("Single CSV file into the current table ("+curTableName+")").c_str()));
	mCurTableRadio->setChecked(true);
	QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(mCurTableRadio->sizePolicy().hasHeightForWidth());
	mCurTableRadio->setSizePolicy(sizePolicy2);
	
	vboxLayout1->addWidget(mCurTableRadio);
	
	mMultipleTablesRadio = new QRadioButton(tr("Multiple CSV files into the database"));
	
	vboxLayout1->addWidget(mMultipleTablesRadio);
	hboxLayout->addWidget(widget);
	vboxLayout->addLayout(hboxLayout);
	
	hboxLayout1 = new QHBoxLayout();
	mDegreesLabel = new QLabel(tr("Degrees format"));
	sizePolicy.setHeightForWidth(mDegreesLabel->sizePolicy().hasHeightForWidth());
	mDegreesLabel->setSizePolicy(sizePolicy);
	mDegreesLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	
	hboxLayout1->addWidget(mDegreesLabel);
	
	widget_2 = new QWidget;
	QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Expanding);
	sizePolicy3.setHorizontalStretch(2);
	sizePolicy3.setVerticalStretch(0);
	sizePolicy3.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
	widget_2->setSizePolicy(sizePolicy3);
	
	verticalLayout_3 = new QWidget(widget_2);
	verticalLayout_3->setGeometry(QRect(0, 0, 281, 83));
	vboxLayout2 = new QVBoxLayout(verticalLayout_3);
	vboxLayout2->setContentsMargins(0, 0, 0, 0);
	mDMSRadio = new QRadioButton(tr("Degrees, minutes, seconds"));
	mDMSRadio->setChecked(true);
	
	vboxLayout2->addWidget(mDMSRadio);
	
	mDMRadio = new QRadioButton(tr("Degrees, decimal minutes"));
	
	vboxLayout2->addWidget(mDMRadio);
	
	mDRadio = new QRadioButton(tr("Decimal degrees"));
	
	vboxLayout2->addWidget(mDRadio);
	hboxLayout1->addWidget(widget_2);
	vboxLayout->addLayout(hboxLayout1);
	
	/*spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	vboxLayout->addItem(spacerItem);*/
	
	setLayout(vboxLayout);
}

int cCsvImportIntroPage::nextId () const
{
	cCsvImportDialog* importDlg = cCsvImportDialog::msMainImportDialog;
	
	// check the degree format
	if (mDMSRadio->isChecked())
		importDlg->SetDegFormat(0);
	else if (mDMRadio->isChecked())
		importDlg->SetDegFormat(1);
	else if (mDRadio->isChecked())
		importDlg->SetDegFormat(2);

	if (mCurTableRadio->isChecked())
	{
		importDlg->SetType(0);
		return cCsvImportDialog::Page_InputFilename;
	}
	
	importDlg->SetType(1);
	return cCsvImportDialog::Page_InputFolder;
}

/**************************************************************************************************/

cCsvImportFilenamePage::cCsvImportFilenamePage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout;
	QHBoxLayout *hboxLayout;
	QSpacerItem *spacerItem;
	
	setTitle(tr("QRAP CSV Import"));
	setSubTitle(tr("Enter input filename"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Please enter the name of the CSV file to import."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	hboxLayout = new QHBoxLayout();
	mFilenameEdit = new QLineEdit;
	
	hboxLayout->addWidget(mFilenameEdit);
	
	mBrowseButton = new QPushButton(tr("Browse..."));
	
	hboxLayout->addWidget(mBrowseButton);
	vboxLayout->addLayout(hboxLayout);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem);
	
	mNoteLabel = new QLabel(tr("Note that the first line of the CSV file must contain the names of the fields of the table."));
	mNoteLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mNoteLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mNoteLabel);
	
	setLayout(vboxLayout);
	
	connect(mFilenameEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChanged(const QString&)));
	connect(mBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseClicked()));
}

void cCsvImportFilenamePage::TextChanged (const QString& text)
{
	emit completeChanged();
}

void cCsvImportFilenamePage::BrowseClicked ()
{
	mFilenameEdit->setText(QFileDialog::getOpenFileName(this, tr("Select CSV File"), tr(""), tr("CSV files (*.csv)")));
}

int cCsvImportFilenamePage::nextId () const
{
	return cCsvImportDialog::Page_Progress;
}

bool cCsvImportFilenamePage::isComplete () const
{
	return (mFilenameEdit->text().length() > 0) ? true : false;
}

bool cCsvImportFilenamePage::validatePage ()
{
	cCsvImportDialog::msMainImportDialog->SetInputFilename(mFilenameEdit->text());
	return true;
}

/**************************************************************************************************/

cCsvImportFolderPage::cCsvImportFolderPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout;
	QHBoxLayout *hboxLayout;
	QSpacerItem *spacerItem;
	
	setTitle(tr("QRAP CSV Import"));
	setSubTitle(tr("Enter input folder name"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("Please enter the folder from which to import CSV files."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	hboxLayout = new QHBoxLayout;
	mFolderEdit = new QLineEdit;
	
	hboxLayout->addWidget(mFolderEdit);
	
	mBrowseButton = new QPushButton(tr("Browse..."));
	hboxLayout->addWidget(mBrowseButton);
	vboxLayout->addLayout(hboxLayout);
	
	spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem);
	
	mNoteLabel = new QLabel(tr("Note that the name of each CSV file in the folder must correspond to the name of a table in the database, and the first line of the CSV file must contain the field names for that specific table."));
	mNoteLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mNoteLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mNoteLabel);
	
	setLayout(vboxLayout);
	
	connect(mFolderEdit, SIGNAL(textChanged(const QString&)), this, SLOT(TextChanged(const QString&)));
	connect(mBrowseButton, SIGNAL(clicked()), this, SLOT(BrowseClicked()));
}

void cCsvImportFolderPage::TextChanged (const QString& text)
{
	emit completeChanged();
}

void cCsvImportFolderPage::BrowseClicked ()
{
	mFolderEdit->setText(QFileDialog::getExistingDirectory(this, tr("Select Output Folder"), tr("")));
}

int cCsvImportFolderPage::nextId () const
{
	return cCsvImportDialog::Page_Progress;
}

bool cCsvImportFolderPage::isComplete () const
{
	return (mFolderEdit->text().length() > 0) ? true : false;
}

bool cCsvImportFolderPage::validatePage ()
{
	cCsvImportDialog::msMainImportDialog->SetInputFolder(mFolderEdit->text());
	return true;
}

/**************************************************************************************************/

cCsvImportProgressPage::cCsvImportProgressPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout;
	QHBoxLayout *hboxLayout;
	QSpacerItem *spacerItem, *spacerItem1;
	
	setTitle(tr("QRAP CSV Import"));
	setSubTitle(tr("CSV import progress"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("The wizard is now ready to import. Click the Next button to begin the import process."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	mProgressBar = new QProgressBar;
	mProgressBar->setValue(0);
	mProgressBar->setTextVisible(false);
	
	vboxLayout->addWidget(mProgressBar);
	
	hboxLayout = new QHBoxLayout;
	spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	hboxLayout->addItem(spacerItem);
	
	mProgressLabel = new QLabel(tr("Ready."));
	hboxLayout->addWidget(mProgressLabel);
	vboxLayout->addLayout(hboxLayout);
	spacerItem1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	
	vboxLayout->addItem(spacerItem1);
	
	setLayout(vboxLayout);
}

int cCsvImportProgressPage::nextId () const
{
	return cCsvImportDialog::Page_Finished;
}

// Helper function for filtering out CSV files in a directory
int csvFilter (const struct dirent* e)
{
	string name = e->d_name, ext;
	unsigned    dotPos = name.rfind('.');
	
	// skip "." and ".."
	if ((name == ".") || (name == ".."))
		return 0;
	
	if (dotPos < name.length())
	{
		ext = StrToLower(name.substr(dotPos+1));
		if (ext == "csv")
			return 1;
	}
	
	return 0;
}

bool cCsvImportProgressPage::validatePage ()
{
	cCsvImportDialog* importDlg = cCsvImportDialog::msMainImportDialog;
	string            curTable, inFolder, inFile;
	StringMap         inFiles;
	string            log;
	IntArray          failed;
	DegreeFormat      fmt = dfDeg;
	int               n, i, j, k, count, tableCount;
	DbStructure       dbs;
	DbStructure::const_iterator table;
	struct dirent**   nameList;
	char text[33];
	
	switch (importDlg->GetDegFormat())
	{
	case 0: fmt = dfDegMinSec; break;
	case 1: fmt = dfDegMin; break;
	}
	
	gDb.GetDbStructure(dbs);
	
	// check what type of import it is
	switch (importDlg->GetType())
	{
	// if we're importing from a single file
	case 0:
		curTable = importDlg->GetCurTable().toStdString();
		mProgressLabel->setText(tr(curTable.c_str()));
		repaint();
		// get the input file's name
		inFile = importDlg->GetInputFilename().toStdString();
		log = "Importing from file \""+inFile+"\" into table \""+curTable+"\"...\n";
		if (!gDb.ImportFromCsv(inFile, curTable, failed, fmt))
		{
			log += "Unable to import from file at all.";
		} else
		{
			count = failed.size();
			// check if any entries failed
			if (count > 0)
			{
				log += "The entries on the following line(s) could not be imported:\n";
				for (j=0;j<count;j++)
				{
					gcvt(failed[j],10,text);
					log += text;
					// delimit the output
					if ((j+1) < count)
						log += ", ";
				}
				log += "\n";
			} else
			{
				log += "File successfully imported.\n";
			}
		}
		mProgressBar->setValue(100);
		repaint();
		break;
	
	// if we're importing from a folder
	case 1:
		// get the input folder's name
		inFolder = importDlg->GetInputFolder().toStdString();
		// make sure there's a trailing slash
		if (inFolder[inFolder.length()-1] != '/')
			inFolder += '/';
		
		log = "Importing from folder \""+inFolder+"\"...\n";
		mTopLabel->setText(tr(string("Importing from folder \""+inFolder+"\"...").c_str()));
		// scan the folder for CSV files
		n = scandir(inFolder.c_str(), &nameList, csvFilter, alphasort);
		if (n < 0)
		{
			log += "Unable to scan the given folder for CSV files.";
			QMessageBox::critical(this, tr("Error"), tr("Unable to scan the given folder for CSV files"));
			break;
		}
		// if there are no CSV files
		if (n == 0)
		{
			log += "No CSV files were found in the given folder.";
			break;
		}
		// run through the CSV files to get all the files' names
		for (i=0;i<n;i++)
		{
			// get the current input file's name
			inFile = inFolder+nameList[i]->d_name;
			// extract the table's name from the file name
			curTable = nameList[i]->d_name;
			curTable = StrToLower(curTable.substr(0, curTable.rfind('.')));
			inFiles[curTable] = inFile;
		}
		
		tableCount = dbs.size();
		k = 0;
		// run through all the tables in the database in the correct order
		for (i=0;i<tableCount;i++)
		{
			table = gDb.GetTableWithOrder(i);
			// if data is to be imported into this table
			if (inFiles.count(table->first) > 0)
			{
				k++;
				
				inFile = inFiles[table->first];
				curTable = table->first;
				mProgressLabel->setText(tr(curTable.c_str()));
				// update the progress
				mProgressBar->setValue((int)(((float)k/(float)n)*100.0));
				// repaint the window
				repaint();
				log += "\nImporting file \""+inFile+"\" into table \""+curTable+"\"...\n";
				
				failed.clear();
				// import the CSV file
				if (!gDb.ImportFromCsv(inFile, curTable, failed, fmt))
				{
					log += "Unable to import from file at all.\n";
				} else
				{
					count = failed.size();
					// check if any entries failed
					if (count > 0)
					{
						log += "The entries on the following line(s) could not be imported:\n";
						for (j=0;j<count;j++)
						{
							gcvt(failed[j],8,text);
							log += text;
							// delimit the output
							if ((j+1) < count)
								log += ", ";
						}
						log += "\n";
					} else
						log += "File successfully imported.\n";
				}
			}
		}
		
		// free the memory allocated to the name list by the scandir function
		free(nameList);
		break;
	}
	
	importDlg->SetImportLog(log.c_str());
	
	return true;
}

/**************************************************************************************************/

cCsvImportFinishedPage::cCsvImportFinishedPage (QWidget* parent): QWizardPage(parent)
{
	QVBoxLayout *vboxLayout;
	
	setTitle(tr("QRAP CSV Import"));
	setSubTitle(tr("CSV import complete"));
	
	vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	mTopLabel = new QLabel(tr("The wizard is now complete. The log window below shows more detailed information about what happened during the import process."));
	mTopLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
	mTopLabel->setWordWrap(true);
	
	vboxLayout->addWidget(mTopLabel);
	
	mImportLog = new QTextEdit;
	mImportLog->setReadOnly(true);
	
	vboxLayout->addWidget(mImportLog);
	setLayout(vboxLayout);
}

void cCsvImportFinishedPage::initializePage ()
{
	mImportLog->setText(cCsvImportDialog::msMainImportDialog->GetImportLog());
}
