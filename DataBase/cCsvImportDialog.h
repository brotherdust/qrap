/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCsvImportDialog.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Thane Thomson (roeland.frans@gmail.com)
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

#ifndef __cCsvImportDialog_h__
#define __cCsvImportDialog_h__

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QRadioButton>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QTextEdit>

#include "Config.h"

namespace Qrap
{
	/**
	 * The CSV file importing dialog.
	 */
	class cCsvImportDialog: public QWizard
	{
		Q_OBJECT
	
	public:
		static cCsvImportDialog* msMainImportDialog;
		enum { Page_Intro, Page_InputFilename, Page_InputFolder, Page_Progress, Page_Finished };
		
		/**
		 * Constructor.
		 */
		cCsvImportDialog (QWidget* parent = 0, const QString& curTable = "");
		
		/**
		 * Destructor.
		 */
		~cCsvImportDialog () { };
		
		/**
		 * @name Member Retrieval
		 */
		 
		QString GetCurTable () const { return mCurTable; };
		
		int GetType () const { return mImportType; };
		void SetType (int type) { mImportType = type; };
		
		int GetDegFormat () const { return mDegFormat; };
		void SetDegFormat (int format) { mDegFormat = format; };
		
		void SetInputFilename (const QString& filename) { mInputFilename = filename; };
		QString GetInputFilename () const { return mInputFilename; };
		
		void SetInputFolder (const QString& folder) { mInputFolder = folder; };
		QString GetInputFolder () const { return mInputFolder; };
		
		void SetImportLog (const QString& log) { mImportLog = log; };
		QString GetImportLog () const { return mImportLog; };
		
	private:
		QString mCurTable;
		int     mImportType;             ///< Radio selection indicating the type of import to take place.
		int     mDegFormat;              ///< Radio selection indicating the degrees format to use when importing.
		QString mInputFilename;          ///< Input filename for single CSV file import.
		QString mInputFolder;            ///< Input folder for multiple CSV file import.
		QString mImportLog;              ///< The full import log.
	};
	
	/**
	 * The CSV importing wizard's introduction page.
	 */
	class cCsvImportIntroPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvImportIntroPage (QWidget* parent = 0);
		~cCsvImportIntroPage () {};
		
		int nextId () const;
	
	private:
		QLabel       *mTopLabel;
		QLabel       *mImportLabel;
		QRadioButton *mCurTableRadio;
		QRadioButton *mMultipleTablesRadio;
		QLabel       *mDegreesLabel;
		QRadioButton *mDMSRadio;
		QRadioButton *mDMRadio;
		QRadioButton *mDRadio;
	};
	
	/**
	 * The CSV importing wizard's filename selection page.
	 */
	class cCsvImportFilenamePage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvImportFilenamePage (QWidget* parent = 0);
		~cCsvImportFilenamePage () {};
		
		int nextId () const;
		bool isComplete () const;
		bool validatePage ();
		
	public slots:
		void TextChanged (const QString& text);
		void BrowseClicked ();
	
	private:
		QLabel      *mTopLabel;
		QLineEdit   *mFilenameEdit;
		QPushButton *mBrowseButton;
		QLabel      *mNoteLabel;
	};
	
	/**
	 * The CSV importing wizard's folder page.
	 */
	class cCsvImportFolderPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvImportFolderPage (QWidget* parent = 0);
		~cCsvImportFolderPage () {};
		
		int nextId () const;
		bool isComplete () const;
		bool validatePage ();
	
	public slots:
		void TextChanged (const QString& text);
		void BrowseClicked ();
	
	private:
		QLabel      *mTopLabel;
		QLineEdit   *mFolderEdit;
		QPushButton *mBrowseButton;
		QLabel      *mNoteLabel;
	};
	
	/**
	 * The CSV importing wizard's progress page.
	 */
	class cCsvImportProgressPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvImportProgressPage (QWidget* parent = 0);
		~cCsvImportProgressPage () {};
		
		int nextId () const;
		bool validatePage ();
	
	private:
		QLabel       *mTopLabel;
		QProgressBar *mProgressBar;
		QLabel       *mProgressLabel;
	};
	
	/**
	 * The CSV importing wizard's finished page.
	 */
	class cCsvImportFinishedPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvImportFinishedPage (QWidget* parent = 0);
		~cCsvImportFinishedPage () {};
		
		void initializePage ();
	
	private:
		QLabel    *mTopLabel;
		QTextEdit *mImportLog;
	};
}

#endif
