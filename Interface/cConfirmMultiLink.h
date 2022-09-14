 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmMultiLink.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This is the interface class to initiate a search for
 *                : possible links among a set of sites (sans radio insts)
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

#ifndef CCONFIRMMULTILINK_H_
#define CCONFIRMMULTILINK_H_

//#include <QDialog>
#include "../DataBase/Config.h"
#include "ui_ConfirmMultiLink.h"
#include <qgspoint.h>
#include "../Prediction/cGeoP.h"
#include "../Prediction/cLink.h"
#include "../Prediction/PredStructs.h"
#include "cLinkAnalysis.h"
#include <string>
#include <QtWidgets/QCheckBox>
#include <QtCore/QList>

using namespace std;
using namespace Qrap;

class cConfirmMultiLink: public QDialog, private Ui::ConfirmMultiLink
{Q_OBJECT
public:
	cConfirmMultiLink();
	cConfirmMultiLink( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cConfirmMultiLink();
	
	bool SetPoints(QList<QgsPoint> Points);

private:
	void LoadDefaults();
	QList<unsigned> mSiteList;
	double mMinClear;
	double mMaxPath;
	double mMaxDist;
	double mkFactor;
	int mTechKey;
	int mProject;
	string mTechnology;
	tFixed mInst;
	QWidget* mParent;
	Qt::WFlags mFL;
	int InsertRadInst(int SiteID, int Sector=1, double Bearing=0.0, double Tilt=0.0);
	
public slots:
	void on_pushCancel_clicked();
	void on_pushOk_clicked();
	void checkBoxChanged();
private slots:

};
#endif /*CCONFIRMMULTILINK_H_*/
