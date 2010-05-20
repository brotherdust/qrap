 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmLink.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : This is the interface class to initiate a link analysis
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

#ifndef CCONFIRMLINK_H_
#define CCONFIRMLINK_H_

#include "ui_ConfirmLink.h"
#include "../DataBase/Config.h"
#include "../Prediction/cGeoP.h"
#include "../Prediction/cLink.h"
#include <qgspoint.h>
#include <stdio.h>

using namespace std;
using namespace Qrap;

class cConfirmLink: public QDialog, private Ui::ConfirmLink
{Q_OBJECT
public:
	cConfirmLink();
	cConfirmLink( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cConfirmLink();
	
	bool SetPoints(QList<QgsPoint> Points,double scale);
	bool SetOldLink(int ID, int RxID,int TxID,string Name, double frequency, double kfactor);
	
	eOutputUnits	Units;
	bool				Downlink;
	double			Frequency;
	double			kFactor;
	double			PlotResolution;
	short int			DEMsource;
	short int			ClutterSource;
	int 		TxID;
	int 		RxID;
	int		mLinkID;
	bool				UseClutter;
	QString 			TxName;
	QString 			RxName;
	QString mLinkName;
	
public slots:
	//void on_txtkFactor_textChanged();
	void on_btnCancel_clicked();
	void on_btnOk_clicked();
	void on_btnSave_clicked();
	
private:
	
};

#endif /*CCONFIRMLINK_H_*/
