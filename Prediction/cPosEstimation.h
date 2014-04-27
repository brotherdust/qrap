 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cPosEstimation.h
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

#ifndef cPosEstimation_h
#define cPosEstimation_h 1

#define GAMMA 2.5
#define MOBILEHEIGHT 1.5
#define SIGMA2 (14*14)
#define MARGIN 15
#define SENSITIVITY -110
#define NUMPARTICLES 30
#define INERTIA 0.72
#define Cp 1.4
#define Cg 1.4
#define STOPN 20
#define DELTA 0.000005


// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "cPthlssp.h"
#include "cProfile.h"
#include "cAntpatn.h"
#include "cMemmngr.h"
#include "PredStructs.h"
#include "cRasterFileHandler.h"
#include <iostream>
#include <random>

using namespace std;
using namespace Qrap;

enum eMethod
{
	GPS,
	CellID,
	CellID_RxTx,
	SSiteDir, // The direction is based on the direction of the second strongest cell
	CoSiteSecDir,
	CosRuleAngleDistRatio,
	CosRuleDistDist,
	CosRuleDistAngle,
	CosRuleAngleAngle,
	DCM_PSO,
	None
};

struct tTestPoint
{
	unsigned	sNewTP;
	unsigned	sOriginalTP;
	cGeoP		sEstimatedLocation;
	cGeoP		sOriginalLocation;
	eMethod		sMethodUsed;
	double		sErrorEstimate;
};

typedef	vector<tTestPoint> vTestPoint;

struct tMeas
{
	unsigned	sID;
	unsigned	sOriginalTP;
	unsigned 	sSiteID;
	cGeoP		sSiteLocation;
	unsigned	sCellID;
	cGeoP		sCentroid;
	bool		sServingCell;
	unsigned	sInstKeyFixed;
	double 		sEIRP;
	unsigned	sAntPatternKey;
	double		sFrequency;
	double 		sMeasValue;
	double		sPathLoss;
	double 		sPredValue;
	float		sTilt;
	double		sHeight;
	double		sAzimuth;
	double 		sBeamWidth;
	double		sDistance;	// The estimated distance from the site in meters using TA or Rx-Tx chips
	double 		sResDist;
};

typedef	vector<tMeas> vMeas;

struct tPosSet
{
	vTestPoint sTestPoints;
	vMeas sMeasurements;
	unsigned sNumMeas;
};

typedef	vector<tPosSet> vPosSet;

struct tBand
{
	double sFrequency;
	unsigned sAIndex;
	unsigned sBIndex;
	double sMaxMeasValue;
};
typedef	vector<tBand> vBand;
typedef vector<cGeoP> vPoints;


//## Class cPosEstimation

namespace Qrap
{
class cPosEstimation 
{
   public:
	cPosEstimation(); // default constructor
	~cPosEstimation(); // destructor

	bool LoadMeasurements(vPoints Points,
				unsigned MeasType=0, 
				unsigned MeasSource=0,
				unsigned PosSource=0,
				unsigned Technology=0);

	void EstimatePositions();
	int SaveResults();


   private:

	bool CI();
	bool CI_TA();
	bool SecondSite();
	bool CoSecAzi(double &minAzi);
	double FindAzi(unsigned BIndex, unsigned AIndex=0);
	double SearchDistance(double Azimuth, double min, double max);
	bool CoSinRule();
	bool DCM_ParticleSwarm();

	// In this function the default mobile installation 
	// with height of MOBILEHEIGHT (#defined) and an isotropic antenna. 
	double CostFunction(double rho, double phi);

	vPosSet mPosSets;		/// an array with all the testpoints
	unsigned mCurrentPosSetIndex;
	unsigned mNumPoints;
	unsigned mNewTP;

	double mPlotResolution;
	double mkFactor;
	eOutputUnits mUnits;
	bool mUseClutter;
	short int mDEMsource;
	short int mClutterSource;
	cRasterFileHandler mDEM;
	cRasterFileHandler mClutter;
	cAntennaPattern* mFixedAnts;
	unsigned mNumInsts;
	unsigned mClutterClassGroup;
	unsigned *mClutterCount;
	cPathLossPredictor mPathLoss;
	double* mCellPathLoss;		/// Pathloss to each cell in a measurement set.
	cProfile mClutterProfile;
	cProfile mDEMProfile;
	vector<tFixed>	mFixedInsts;	///< Information on the fixed installations
	vector<tMobile>	mMobiles;	/// Information on all the mobile instruments used during the measurements

};
}
#endif