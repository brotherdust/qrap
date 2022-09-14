 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmSpectral.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This is the interface part that initiates spectral analysis
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

#ifndef CCONFIRMSPECTRAL_H_
#define CCONFIRMSPECTRAL_H_

//#include <QDialog>
#include "../DataBase/Config.h"
#include "ui_ConfirmSpectral.h"
#include <qgspoint.h>
#include "../Prediction/cGeoP.h"
#include <string>
#include <QtWidgets/QCheckBox>
#include <QtCore/QList>

using namespace std;
using namespace Qrap;

class cConfirmSpectral: public QDialog, private Ui::ConfirmSpectral
{Q_OBJECT
public:
	cConfirmSpectral();
	cConfirmSpectral( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cConfirmSpectral();
	
	bool SetPoints(QList<QgsPoint> Points);
	void LoadDefaults();
	
	cGeoP NorthWestCorner;
	cGeoP SouthEastCorner;
	
	QList<unsigned> mRadInst;
	unsigned mRx;
	double mFrequencySpacing;
	double mkFactor;
private:
	
public slots:
	void on_pushCancel_clicked();
	void on_pushOk_clicked();
private slots:
};
#endif /*CCONFIRMSPECTRAL_H_*/
