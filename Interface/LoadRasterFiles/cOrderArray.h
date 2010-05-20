 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cOrderArray.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used setting the order in which 
 *                : rasterfiles should be used
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

#ifndef CORDERARRAY_H_
#define CORDERARRAY_H_

#include "ui_OrderArray.h"
#include <stdio.h>
#include "../../DataBase/Config.h"

using namespace std;
using namespace Qrap;


class cOrderArray: public QWidget, private Ui::OrderArray
{Q_OBJECT
public:
	cOrderArray();
	cOrderArray( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cOrderArray();
	
public slots:
	void on_pushButtonAdd_clicked();
	void on_pushButtonRemove_clicked();
	void on_comboBoxFileType4_currentIndexChanged();
	void on_pushButtonUp_clicked();
	void on_pushButtonDown_clicked();
	void on_tableWidgetFileSets4_itemSelectionChanged();
	
	
private:
	void Setup();
	void LoadTables();
	void CastRow(QTableWidget* from,QTableWidget* to,int fromRow, int toRow);
	void EnableButtons();
	void WriteDB();
};

#endif /*CORDERARRAY_H_*/
