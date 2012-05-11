 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasAnalysisCalc.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class allows for the importing of CW measurements 
 *                  in a *.val format
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

#ifndef cMeasAnalysisCalc_h
#define cMeasAnalysisCalc_h 1


// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "PredStructs.h"
#include "cPthlssp.h"
#include "cProfile.h"
#include "cAntpatn.h"
#include "cRasterFileHandler.h"

#include "../Interface/ui_MeasurementAnalysis.h"
#include "cGeoP.h"
#include <QMessageBox>

using namespace std;
using namespace Qrap;

struct tMeasPoint
{
	unsigned	sID;
	unsigned	sCell;
	int		sInstKeyMobile;
	int 		sInstKeyFixed;
	cGeoP 		sPoint;
	double 		sMeasValue;
	double		sPathLoss;
	double 		sPredValue;
	float		sTilt;
	double		sAzimuth;
	double		sDistance;
	unsigned 	sClutter;
};


//## Class cMeasAnalysisCalc

namespace Qrap
{
class cMeasAnalysisCalc 
{
   public:
	cMeasAnalysisCalc(); // default constructor
	~cMeasAnalysisCalc(); // destructor

	int LoadMeasurements(unsigned MeasType, unsigned PosSource, 
				unsigned MeasSource, unsigned Cell=0);


	int PerformAnalysis(double &Mean, double &StDev, double &CorrC, 
				unsigned Clutterfilter=0);


   private:

	tMeasPoint *mMeasPoints;	/// an array with all the measurements
	unsigned mNumMeas;
	double mkFactor;
	bool mUseClutter;
	bool mPlotResolution;
	eOutputUnits mUnits;
	short int mDEMsource;
	short int mClutterSource;
	cRasterFileHandler 	mDEM;
	cRasterFileHandler 	mClutter;
	vector<tFixed>	mFixedInsts;	///< Inforamtion on the fixed installations
	vector<tMobile>	mMobiles;	/// Information on all the mobile instruments used during the measurements

};
}
#endif
