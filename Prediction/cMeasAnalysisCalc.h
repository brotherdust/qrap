 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasAnalysisCalc.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class does the analysis of the measurements in the 
 *			the database
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
#include "cMemmngr.h"
#include "cRasterFileHandler.h"
#include "../Interface/ui_MeasurementAnalysis.h"
#include "cGeoP.h"

#include <iostream>
#include <Eigen/Dense>


using namespace std;
using namespace Eigen;
using namespace Qrap;

struct tMeasPoint
{
	unsigned	sID;
	unsigned	sCell;
	unsigned	sInstKeyMobile;
	unsigned	sInstKeyFixed;
	cGeoP 		sPoint;
	double 		sMeasValue;
	double		sPathLoss;
	double 		sPredValue;
	float		sTilt;
	double		sAzimuth;
	double		sDistance;
	unsigned 	sClutter;
};

typedef vector<cGeoP> vPoints;

//## Class cMeasAnalysisCalc

namespace Qrap
{
class cMeasAnalysisCalc 
{
   public:
	cMeasAnalysisCalc(); // default constructor
	~cMeasAnalysisCalc(); // destructor

bool LoadMeasurements(vPoints Points, 
			unsigned MeasType=0, 
			unsigned PosSource=0, 
			unsigned MeasSource=0, 
			unsigned Cell=0);

	bool LoadMeasurements(	unsigned MeasType=0, 
				unsigned PosSource=0, 
				unsigned MeasSource=0, 
				unsigned Cell=0);

	int PerformAnalysis(double &Mean, double &MeanSquareError,
				double &StDev, double &CorrC, unsigned Clutterfilter=0);

	void SetUseAntANN( bool UseAntANN) { mUseAntANN = UseAntANN;};
	void SetPlotResolution( double InPlotRes) {mPlotResolution = InPlotRes;};
	void SetSeekWidthBest( double INS) {mSeekWidthBest=INS;};
	void SetSmoothWidthBest( double INS) {mSmoothWidthBest=INS;};

	int SaveResults();

	bool OptimiseModelCoefD(unsigned MeasSource=0);
	bool OptimiseOffsets(unsigned MeasSource=0);
	bool OptimiseModelCoefAllTotal(unsigned MeasSource=0);
	bool OptimiseModelCoefAllTogether();
	bool OptimiseSeekWidth();
	bool OptimiseHeights(unsigned MeasSource=0);	
	cPathLossPredictor mPathLoss;	// we need to access the clutter coefficients of mClutter of cPathLoss 
					// ... hence mPathLoss needs to be a member/class global variable

   private:

	tMeasPoint *mMeasPoints;	/// an array with all the measurements
	unsigned mNumMeas;
	double mkFactor;
	bool mUseClutter;
	bool mUseAntANN;
	unsigned mClutterClassGroup;
	unsigned mClutterFilter;
	unsigned *mClutterCount;
	double mPlotResolution;
	eOutputUnits mUnits;
	short int mDEMsource;
	short int mClutterSource;
	cRasterFileHandler mDEM;
	cRasterFileHandler mClutter;
	vector<tFixed>	mFixedInsts;	///< Information on the fixed installations
	vector<tMobile>	mMobiles;	/// Information on all the mobile instruments used during the measurements
	MatrixXd mSolveCoefMatrix;	// matrix representing the set of linear equations to be solved
	VectorXd mLeftSide;
	RowVectorXd mDeltaCoeff;
	double *mMinTerm;		// The minimum and maximum values of a specific term is used to identify 
	double *mMaxTerm;		// whether or not the coefficients of these terms are relavant or whether 
					// the term will be constant.
	double *mMidTerm;
	double mSeekWidthBest;
	double mSmoothWidthBest;
};
}
#endif
