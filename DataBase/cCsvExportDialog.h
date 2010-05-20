/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCsvExportDialog.h
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

#ifndef __cCsvExportDialog_h__
#define __cCsvExportDialog_h__

#include <QWizard>
#include <QWizardPage>
#include <QLabel>
#include <QRadioButton>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>

#include "Config.h"


namespace Qrap
{
	/**
	 * The CSV exporting dialog.
	 */
	class cCsvExportDialog: public QWizard
	{
		Q_OBJECT
		
	public:
		enum { Page_Intro, Page_SelectTables, Page_EnterQuery,
			Page_OutputFile, Page_OutputFolder, Page_Progress, Page_Finished };
		
		static cCsvExportDialog* msMainExportDialog;
		
		/**
		 * Constructor.
		 * 
		 * @param parent The parent widget.
		 * @param curTable The name of the currently selected table in the user interface.
		 * If left blank, the option to export the current table's contents will be hidden.
		 */
		cCsvExportDialog (QWidget* parent = 0, const QString& curTable = "");
		
		/**
		 * Destructor.
		 */
		~cCsvExportDialog () {};
		
		/**
		 * Retrieves the name of the current table as passed to the constructor.
		 */
		QString GetCurTable () const { return mCurTable; };
		
		/**
		 * Retrieves the index of the selected radio button on the intro page.
		 */
		int GetCurRadioSelection () const { return mRadioSelection; };
		
		/**
		 * Sets the index of the selected radio button.
		 */
		void SetCurRadioSelection (int val) { mRadioSelection = val; };
		
		/**
		 * Retrieves the list of tables to export.
		 */
		void GetExportTableList (StringArray& list) const { list = mExportTableList; };
		
		/**
		 * Clears the list of exported tables.
		 */
		void ClearExportTableList () { mExportTableList.clear(); };
		
		/**
		 * Adds a table to be exported.
		 */
		void SetExportTableList (const StringArray& list) { mExportTableList = list; };
		
		void AddExportTable (const std::string& table) { mExportTableList.push_back(table); };
		
		/**
		 * Sets the export query.
		 */
		void SetExportQuery (const QString& query) { mExportQuery = query; };
		
		/**
		 * Retrieves the export query.
		 */
		QString GetExportQuery () const { return mExportQuery; };
		
		void SetOutputFile (const QString& outputFile) { mOutputFile = outputFile; };
		QString GetOutputFile () const { return mOutputFile; };
		
		void SetOutputFolder (const QString& outputFolder) { mOutputFolder = outputFolder; };
		QString GetOutputFolder () const { return mOutputFolder; };
		
		void SetExportSuccess (bool success) { mExportSuccess = success; };
		bool GetExportSuccess () const { return mExportSuccess; };
	
	private:
		QString     mCurTable;            ///< The currently selected table in the user interface.
		int         mRadioSelection;      ///< The which radio button was selected on the intro page.
		StringArray mExportTableList;     ///< The list of tables to be exported.
		QString     mExportQuery;         ///< The query to be exported to CSV file.
		QString     mOutputFile;          ///< The output file's name.
		QString     mOutputFolder;        ///< The output folder's name.
		bool        mExportSuccess;       ///< Was the file export successful?
	};
	
	/**
	 * The CSV exporting dialog's introduction page.
	 */
	class cCsvExportIntroPage: public QWizardPage
	{
		Q_OBJECT
		
	public:
		/**
		 * Constructor.
		 */
		cCsvExportIntroPage (QWidget* parent = 0);
		
		/**
		 * Destructor.
		 */
		~cCsvExportIntroPage () {};
		
		int nextId () const;
	
	private:
		QLabel*       mTopLabel;
		QLabel*       mTypeLabel;
		QRadioButton* mCurrentTableRadio;
		QRadioButton* mMultipleTablesRadio;
		QRadioButton* mQueryResultRadio;
	};
	
	/**
	 * The CSV exporting dialog's table selection page.
	 */
	class cCsvExportSelectTablesPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportSelectTablesPage (QWidget* parent = 0);
		~cCsvExportSelectTablesPage () { };
		
		bool isComplete () const;
		int nextId () const;
		void initializePage ();
	
	public slots:
		void SelectionChanged (QListWidgetItem* item);
	
	private:
		QLabel*      mTopLabel;
		QListWidget* mTableList;
		StringArray  mSelectedTables;
	};
	
	/**
	 * The CSV exporting dialog's query entry page.
	 */
	class cCsvExportEnterQueryPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportEnterQueryPage (QWidget* parent = 0);
		~cCsvExportEnterQueryPage () { };
		
		bool isComplete () const;
		int nextId () const;
		bool validatePage ();
	
	public slots:
		void TextChanged (const QString& text);
	
	private:
		QLabel*    mTopLabel;
		QLineEdit* mQueryEdit;
		QLabel*    mExampleLabel;
	};
	
	/**
	 * The CSV exporting dialog's output file page.
	 */
	class cCsvExportOutputFilePage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportOutputFilePage (QWidget* parent = 0);
		~cCsvExportOutputFilePage () { };
		
		bool isComplete () const;
		int nextId () const;
		bool validatePage ();
	
	public slots:
		void TextChanged (const QString& text);
		void BrowseClicked ();
	
	private:
		QLabel *mTopLabel;
		QLineEdit *mFilenameEdit;
		QPushButton *mBrowseButton;
	};
	
	/**
	 * The CSV exporting dialog's output folder page.
	 */
	class cCsvExportOutputFolderPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportOutputFolderPage (QWidget* parent = 0);
		~cCsvExportOutputFolderPage () { };
		
		bool isComplete () const;
		int nextId () const;
	
	public slots:
		void TextChanged (const QString& text);
		void BrowseClicked ();
	
	private:
		QLabel*      mTopLabel;
		QLineEdit*   mFolderEdit;
		QPushButton* mBrowseButton;
	};
	
	/**
	 * The CSV exporting dialog's exporting progress page.
	 */
	class cCsvExportProgressPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportProgressPage (QWidget* parent = 0);
		~cCsvExportProgressPage () { };
		
		bool isComplete () const;
		bool validatePage ();
	
	private:
		QLabel*       mTopLabel;
		QProgressBar* mProgressBar;
		QLabel*       mProgressLabel;
	};
	
	/**
	 * The CSV exporting dialog's final page.
	 */
	class cCsvExportFinishedPage: public QWizardPage
	{
		Q_OBJECT
	
	public:
		cCsvExportFinishedPage (QWidget* parent = 0);
		~cCsvExportFinishedPage () { };
		
		void initializePage ();
		
	private:
		QLabel* mTopLabel;
	};
}

#endif
