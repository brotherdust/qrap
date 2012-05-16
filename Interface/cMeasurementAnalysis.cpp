 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/16
 *    License     : GNU GPLv3
 *    File        : cMeasurementAnalysis.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for importing measurements
 *                : into the database.
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


#include "cMeasurementAnalysis.h"
using namespace Qrap;

//**********************************************************
cMeasurementAnalysis::cMeasurementAnalysis():QDialog()
{
	Setup();
}

//**********************************************************
/*cMeasurementAnalysis::cMeasurementAnalysis(QWidget* parent, Qt::WFlags fl)
:QDialog()
{
	Setup();
}
*/
//**********************************************************
cMeasurementAnalysis::~cMeasurementAnalysis()
{
}

//****************************************************************
void cMeasurementAnalysis::on_pushCancel_clicked()
{
	close();
}


//****************************************************************************
void cMeasurementAnalysis::on_pushOk_clicked()
{
	cBoxPosSource->setEnabled(false);
	cBoxMeasSource->setEnabled(false);
	cBoxMeasType->setEnabled(false);
	pushOk->setEnabled(false);
	pushCancel->setEnabled(false);

	QStringList parts;
	cout << " In cMeasurementAnalysis::on_pButtonLoad_clicked()" << endl;

	parts = cBoxMeasType->currentText().split(":");
	unsigned MeasType = (unsigned) parts[0].toInt();
	
	parts = cBoxMeasSource->currentText().split(":");
	unsigned MeasSource = (unsigned) parts[0].toInt();

	parts = cBoxPosSource->currentText().split(":");
	unsigned PosSource = (unsigned) parts[0].toInt();

	cMeasAnalysisCalc MeasSet;
	double Mean,StDev,CorrC;

	if (MeasSet.LoadMeasurements(MeasType,PosSource,MeasSource))
	{
		if (MeasSet.PerformAnalysis(Mean, StDev, CorrC))
		{
			ValueMeanError->setText(QString("%1").arg(Mean));
               		ValueStdDev->setText(QString("%1").arg(StDev));
            		ValueCorrCoef->setText(QString("%1").arg(CorrC));
			MeasSet.SaveResults();
			ValueMeanError->setEnabled(true);
               		ValueStdDev->setEnabled(true);
            		ValueCorrCoef->setEnabled(true);
			lblMeanError->setEnabled(true);
			lblStdDev->setEnabled(true);
			llblCorrCoef->setEnabled(true);
		}
		else
		{
			cout << "cMeasurementAnalysis::on_pushOK_clicked(); error performing measurement analysis" << endl;	
		}
	}
	else
	{
		cout << "cMeasurementAnalysis::on_pushOK_clicked(); error loading measurements" << endl;
	}
	cBoxPosSource->setEnabled(true);
	cBoxMeasSource->setEnabled(true);
	cBoxMeasType->setEnabled(true);
	pushOk->setEnabled(true);
	pushCancel->setEnabled(true);
}

//******************************************************
void cMeasurementAnalysis::Finished()
{

}

//******************************************************
void cMeasurementAnalysis::Setup()
{
	setupUi(this);
	LoadData();
}

//*****************************************************
void cMeasurementAnalysis::LoadData()
{
	StringIntArray vals;
	StringIntArrayIterator iterator;
	int j=1;
	
	cBoxMeasType->insertItem(0,"0:All");
	gDb.GetFieldUiParams("testpoint","meastype",vals);
	
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasType->insertItem(j,temp);
		j++;
	} // for

	cBoxMeasSource->insertItem(0,"0:All");
	gDb.GetFieldUiParams("testpoint","measdatasource",vals);
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for


	cBoxPosSource->insertItem(0,"0:All");
	gDb.GetFieldUiParams("testpoint","positionsource",vals);
//	cout << vals.begin()->second << endl;
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxPosSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for

}//end cMeasurementAnalysis::LoadData()




