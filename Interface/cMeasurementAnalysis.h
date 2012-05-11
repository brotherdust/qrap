 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/18
 *    License     : GNU GPLv3
 *    File        : cMeasurementAnalysis.h
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

#ifndef CMeasurementAnalysis_H_
#define CMeasurementAnalysis_H_

#include "../DataBase/Config.h"
#include "../Prediction/cMeasAnalysisCalc.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <stdio.h>
#include <limits.h>
#define MAX_PATH 250

using namespace std;
using namespace Qrap;


class cMeasurementAnalysis:   public QDialog, private Ui::MeasurementAnalysis
{Q_OBJECT
public:
	cMeasurementAnalysis();
//	cMeasurementAnalysis( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cMeasurementAnalysis();



public slots:
	void on_pushOk_clicked();
	void on_pushCancel_clicked();
	void Finished();
		
private:

	void Setup();
	void LoadData();
};

#endif /*CLOADRASTERFILES_H_*/
