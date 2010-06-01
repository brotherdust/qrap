 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmSpectral.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow
 *                : (Magdaleen Ballot) (magdaleen.ballot@up.ac.za)
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

#include "cSpectralAnalysis.h"
using namespace Qrap;
using namespace std;

#include <iostream>

//****************************************************************
cSpectralAnalysis::cSpectralAnalysis(): QDialog()
{
	cout << "In cSpectralAnalysis::cSpectralAnalysis(): QDialog() voor setupUi(this);" << endl;
	setupUi(this);
	cout << "In cSpectralAnalysis::cSpectralAnalysis(): QDialog() voor this->setModal(true);" << endl;
	this->setModal(true);
	cout << "In cSpectralAnalysis::cSpectralAnalysis(): QDialog() voor spectrum = new QwtPlotCurve;" << endl;
}

//***************************************************************/
cSpectralAnalysis::cSpectralAnalysis(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	cout << "In cSpectralAnalysis::cSpectralAnalysis(QWidget* parent, Qt::WFlags fl)";
	cout << ": QDialog(parent, fl) voor setupUi(this);" << endl;
	setupUi(this);
	cout << "In cSpectralAnalysis::cSpectralAnalysis(QWidget* parent, Qt::WFlags fl)";
	cout << ": QDialog(parent, fl) voor this->setModal(true);" << endl;
	this->setModal(true);
	cout << "In cSpectralAnalysis::cSpectralAnalysis(QWidget* parent, Qt::WFlags fl)";
	cout << ": QDialog(parent, fl) voor spectrum = new QwtPlotCurve;" << endl;
}


//***************************************************************/
cSpectralAnalysis::~cSpectralAnalysis()
{
//	delete mSpectrum;
}


//***********************************************************************************
void cSpectralAnalysis::on_pushClose_clicked()
{
	close();
}

//**********************************************************************************
void cSpectralAnalysis::on_pushSave_clicked()
{
	QByteArray ba = lineFilename->text().toLatin1();
	mFileName = ba.data();
	mSpectralPlot.SavePlot(mFileName);
}

//********************************************************************************************************
void cSpectralAnalysis::DoAndSetUpDisplay(unsigned mRx,QList<unsigned> TxList,double FrequencyRes, double kFactor)
{
	mSpectralPlot.SetPlotTask(mRx,TxList,FrequencyRes,kFactor);
	if (mSpectralPlot.DoPlot())
	{
		cout << "In cSpectralAnalysis::DoAndSetUpDisplay voor mSpectrum = new QwtPlotCurve" << endl;
		mSpectrum = new QwtPlotCurve("Spectral Power Flux");
		cout << "In cSpectralAnalysis::DoAndSetUpDisplay NA mSpectrum = new QwtPlotCurve" << endl;
		mSpectrum->setPen(QPen(Qt::darkRed));
		cout << "In cSpectralAnalysis::DoAndSetUpDisplay voor mSpectrum->attach(qwtPlot)" << endl;
    		mSpectrum->attach(qwtPlot);
		mSpectrum->setRenderHint(QwtPlotItem::RenderAntialiased);
		qwtPlot->setAxisTitle(qwtPlot->xBottom, "Frequency (MHz)");
		qwtPlot->setAxisTitle(qwtPlot->yLeft, QwtText("Spectral Power Flux (dB(W/m^2/hz))"));
		cout << "In cSpectralAnalysis::DoAndSetUpDisplay voor mSpectrum->setRawData(" << endl;
		mSpectrum->setRawData(mSpectralPlot.mPFDFreq,mSpectralPlot.mPFDValue,mSpectralPlot.mNumFreqs);
	}
	else 
	{
		QString Problem = "The Spectral plot did not succeed. ";
		Problem+= "Are you sure all technologies on the sites have signal envelopes defined?";
		Problem+= " See 'Signal Envelopes' under 'Supporting Tables' in the Database interface.";
		QMessageBox::information(this, "QRap",Problem );
	}
}
