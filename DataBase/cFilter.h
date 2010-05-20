 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cFilter.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : Implemetation of Filter Functionality
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

#ifndef CFILTER_H_
#define CFILTER_H_

#include "ui_Filter.h"
#include "Config.h"
#include <stdio.h>
#include <QMessageBox>

using namespace Qrap;
using namespace std;

class cFilter : public QWidget, private Ui::Filter
{Q_OBJECT
public:
	cFilter();
	cFilter(QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cFilter();
	void LoadDefaults();
	void ChangeAlterButton();
	
public slots:
	void on_comboBoxTechType_currentIndexChanged(int index);
	void on_comboBoxStatus_currentIndexChanged(int index);
	void on_comboBoxProject_currentIndexChanged(int index);
	void on_comboBoxFlagX_currentIndexChanged(int index);
	void on_comboBoxFlagZ_currentIndexChanged(int index);
	void on_comboBoxArea_currentIndexChanged(int index);
	void on_comboBoxAreaType_currentIndexChanged(int index);
	void on_comboBoxField_currentIndexChanged(int index);
	void on_pushButtonCreateArea_clicked();
	void on_pushButtonApplyFilter_clicked();
	
signals:
	void FilterChanged();
	void Area();
	
private:
	void Setup();
	void LoadCombos();
	void CreateViews();
	
	bool Initialise;
	bool mChangeSite;
	bool mChangeInsts;
};

#endif /*CFILTER_H_*/
