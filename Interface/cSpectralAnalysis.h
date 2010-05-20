 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralAnalysis.cpp
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

#ifndef CSPECTRALANALYSIS_H
#define CSPECTRALANALYSIS_H

#include "../DataBase/Config.h"
#include "ui_SpectralAnalysis.h"
#include "../Prediction/cSpectralPlot.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_text.h>
#include <string>
#include <iostream>

using std::string;

namespace Qrap
{
class cSpectralAnalysis : public QDialog, public Ui::SpectralAnalysis
{Q_OBJECT
public:
	cSpectralAnalysis();
	cSpectralAnalysis( QWidget* parent, Qt::WFlags fl = 0 );
	~cSpectralAnalysis();
	
	void DoAndSetUpDisplay(unsigned mRx,QList<unsigned> TxList,double FrequencyRes);
	
	QwtPlotCurve *mSpectrum;	
	cSpectralPlot mSpectralPlot;
	string mFileName;
private:
	
public slots:
	void on_pushClose_clicked();
	void on_pushSave_clicked();
};
}

#endif
