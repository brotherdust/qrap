 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cColourManager.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : (Magdaleen Ballot) 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This is to edit the colurs. Probably based on QGIS code
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

#ifndef CCOLOURMANAGER_H_
#define CCOLOURMANAGER_H_
#include "ui_ColourManager.h"
#include <QFileDialog>
#include <QColorDialog>
#include "../DataBase/Config.h"
#include <stdio.h>
#include <math.h>
#include <gdal.h>
#include <ogr_srs_api.h>
#include <gdal_priv.h>
#include <cpl_string.h>
#include <ogr_spatialref.h>

using namespace Qrap;
using namespace std;

struct Band
{
	float val;
	int r;
	int g;
	int b;
	int a;
};

typedef vector<Band> Colour;

class cColourManager:public QWidget, private Ui::ColourManager
{Q_OBJECT
public:
	cColourManager();
	virtual ~cColourManager();
	cColourManager( QWidget* parent = 0, Qt::WFlags fl = 0 );


public slots:
	void on_btnCancel_clicked();
	void on_btnOk_clicked();
	void on_addFilesButton_clicked();
	void on_addButton_clicked();
	void on_removeButton_clicked();
	void on_plotTypeCombo_currentIndexChanged();
	void on_mClassifyButton_clicked();
	/**Callback for double clicks on the colormap entry widget*/
	void handleColormapTreeWidgetDoubleClick(QTreeWidgetItem* item, int column);
	void SetColour(QString Dir, QString File,QString PlotType = "");
		
private:
	void GetDefaults();
	QStringList mFiles;
	QString mSourceDir;
	int mCount;
	int mCol;
	Colour mColours;
	Colour mBands;
};

#endif /*CCOLOURMANAGER_H_*/
