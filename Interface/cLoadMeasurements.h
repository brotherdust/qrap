 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/18
 *    License     : GNU GPLv3
 *    File        : cLoadMeasurements.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This base interface class is used importing measurements
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

#ifndef CLOADMEASUREMENTS_H_
#define CLOADMEASUREMENTS_H_

#include "../DataBase/Config.h"
#include "ui_LoadMeasurements.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <stdio.h>
#include "../Prediction/cMeasImportSpace.h"
#include <limits.h>
#define MAX_PATH 250

using namespace std;
using namespace Qrap;

enum eFileFormat
{
	eCSV,
	eSpaceDelimited,
	eVal,
	eDriveTest	
};


class cLoadMeasurements:   public QDialog, private Ui::LoadMeasurements
{Q_OBJECT
public:
	cLoadMeasurements();
	cLoadMeasurements( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cLoadMeasurements();


public slots:
	void on_pButtonBrowse_clicked();
	void on_pButtonLoad_clicked();
	void on_pButtonClose_clicked();
	void on_cBoxFileType_currentIndexChanged();
	void Finished();
		
private:

	void Setup();
	void LoadData();

	unsigned mFileCount;
	QStringList mFiles;
	QString mSourceDir;
	eFileFormat mFileFormat;
	unsigned mPosSource;
	unsigned mMeasSource;
	unsigned mCI_CellID;
};

#endif /*CLOADRASTERFILES_H_*/
