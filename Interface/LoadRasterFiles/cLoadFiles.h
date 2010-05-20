 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadFiles.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for importing files
 *                : into a raster file set.
 *                :  Slots into cLoadRasterFiles
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
//\TODO: This whole LoadRasterFile bunch need to be cleaned up.

#ifndef CLOADFILES_H_
#define CLOADFILES_H_

#include "ui_LoadFiles.h"
#include "cLoadThread.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
 #include "../../Prediction/cLoadFile.h"

#include <stdio.h>

using namespace std;


class cLoadFiles: public QWidget, private Ui::LoadFiles
{Q_OBJECT
public:
	cLoadFiles();
	cLoadFiles( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cLoadFiles();
	void LoadData();
	
public slots:
	void on_pushButtonBrowse_clicked();
	void on_pushButtonORTBrowse_clicked();
	void on_pushButtonImport_clicked();
	void on_comboBoxFileFormat_currentIndexChanged();
	void on_comboBoxProjection_currentIndexChanged();
	void on_comboBoxFileType_currentIndexChanged();
	void Set(int,QString);
	void Finished();
	void on_tableWidgetFileSets_currentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn );
	
signals:
	void GiveFeedback(int Counter,QString msgs);
	
	
private:
	void Setup();
	QStringList mFiles;
	QString mSourceDir;
	int mCount;
	short int mSource;
	short int mBin;
	GeoType mType;
	int mCentMer;
	QString mFileType;
	cLoadThread *CLoadThread;
};

#endif /*CLOADFILES_H_*/
