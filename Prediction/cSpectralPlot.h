 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralPlot.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow 
 *                : (Magdaleen Ballot) (magdaleen.ballot@up.ac.za)
 *    Description : Combine results of many links and set-up frequency plot 
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

#ifndef CSPECTRALPLOT_H_
#define CSPECTRALPLOT_H_

#include "cSpectralLink.h"

#include <vector>
#include <qwt_plot_curve.h>
#include <QtCore/QList>
#include "cRasterFileHandler.h"

using std::vector;

namespace Qrap
{

class cSpectralPlot
{
public:
	cSpectralPlot();
	virtual ~cSpectralPlot();
	
	bool ReadPlotRequest (const char *filename);
	bool SetPlotTask(unsigned Rx, QList<unsigned> Tx, double FrequencySpacing, double kFactor);
	
	bool DoPlot();
	double *mPFDFreq;
	double *mPFDValue;
	int mNumFreqs;
	bool SavePlot(string filename);
	
private:
        bool CombinePlots();
	vector<cSpectralLink *> mLinks;
	
	double mFrequencySpacing;	//Gaps between discreet frequencies which we examine
	double mMinFreq;
	double mMaxFreq;
	double mkFactor;

	eOutputUnits	mUnits;
	double	mPlotResolution;///< Hz
	int	mNumLinks;
	
	cRasterFileHandler mDem;
};

}

#endif /*CSPECTRALPLOT_H_*/
