 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadRasterFiles.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This base interface class is used managing the rasterfiles
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

#ifndef CLOADRASTERFILES_H_
#define CLOADRASTERFILES_H_

#include "ui_LoadRasterFiles.h"
//#include "../DataBase/Config.h"
#include "../../Prediction/cLoadFile.h"
#include "cLoadFiles.h"
#include "cCutSet.h"
#include "cCreateDataSet.h"
#include "cOrderArray.h"
#include <stdio.h>

using namespace Qrap;
using namespace std;

class cLoadRasterFiles:   public QDialog, private Ui::LoadRasterFiles
{Q_OBJECT
public:
	cLoadRasterFiles();
	cLoadRasterFiles( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cLoadRasterFiles();
	
	void Setup();

public slots:
	void on_treeWidget_currentItemChanged();
	void GiveFeedback(int,QString);
	
private:
	cLoadFiles *LoadFiles;
	cCutSet *CutSet;
	cCreateDataSet *CreateDataSet;
	cOrderArray *OrderArray;
};

#endif /*CLOADRASTERFILES_H_*/
