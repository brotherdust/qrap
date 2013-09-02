 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCutSet.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for cutting an existing
 *                : raster file set into smaller Guass Conform files.
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

#ifndef CCUTSET_H_
#define CCUTSET_H_

#include "ui_CutSet.h"
#include <QFileDialog>
#include "cLoadThread.h"

#include <stdio.h>

using namespace std;

class cCutSet: public QWidget, private Ui::CutSet
{Q_OBJECT
public:
	cCutSet();
	cCutSet( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cCutSet();
	
public slots:
	void on_pushButtonBrowse1_clicked();
	void on_pushButtonCut1_clicked();
	void on_comboBoxFileFormat1_currentIndexChanged();
	void on_comboBoxProjection1_currentIndexChanged();
	void on_comboBoxFileType1_currentIndexChanged();
	void on_tableWidgetFileSets1_currentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn );
	void Set(int,QString);
	void Finished();
	void LoadData();
	
signals:
	void GiveFeedback(int Counter,QString msgs);
	
private:
	void Setup();

	QStringList mFiles;
	QString mBinDir;
	int mCount;
	short int mSource;
	short int mBin;
	GeoType mType;
	int mCentMer;
	QString mFileType;
	short int mIFileType;
	cLoadThread *CLoadThread;
};

#endif /*CCUTSET_H_*/
