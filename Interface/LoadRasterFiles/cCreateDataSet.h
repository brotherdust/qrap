 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCreateDataSet.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for creating a new 
 *                : raster file set. Slots into cLoadRasterFiles
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

#ifndef CCREATEDATASET_H_
#define CCREATEDATASET_H_

#include "ui_CreateDataSet.h"
#include "../../DataBase/Config.h"
#include <stdio.h>
#include <QMessageBox>

using namespace std;
using namespace Qrap;

class cCreateDataSet: public QWidget, private Ui::CreateDataSet
{Q_OBJECT
public:
	cCreateDataSet();
	cCreateDataSet( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cCreateDataSet();
	void LoadData();
	
public slots:
	void on_checkBoxDerivedBinary_stateChanged(int state);
	void on_lineEditDescription_textChanged(QString txt);
	void on_pushButtonCreate_clicked();
	
	
private:
	void Setup();
	void LoadBin();
	
};

#endif /*CCREATEDATASET_H_*/
