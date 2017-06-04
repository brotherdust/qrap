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

#define GAMMA 2.0
#define MOBILEHEIGHT 1
#define SIGMA2 (8*8)
#define MARGIN 15
#define SENSITIVITY -110
#define NUMPARTICLES 20
#define MINSTEPSIZE 30  
#define INERTIA 0.7
#define Cp 1.4
#define Cg 1.4
#define STOPN 10
#define MAXITERpos 20
#define DELTA 5e-8

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
#include "doublefann.h"
#include "fann_cpp.h"
#include "cTrainPosNetDistAngle.h"
//#include "/home/maggie/libcmaes-master/src/cmaes.h"

//#include "cTrainAntPattern.h"

using namespace std;
using namespace Qrap;
//using namespace libcmaes;

enum eMethod
{
	GPS, //1
	CellID, //2
	CellID_TA, //3
	SSiteDir, // 4 The direction is based on the direction of the second strongest cell
	CoSiteSecDir, //5
	CosRuleAngleDistRatio, //6
	CosRuleDistDist, //7
	CosRuleDistAngle, //8
	CosRuleAngleAngle, //9
	DCM_PSO, //10
	DCM_PSObestN, //11
	DCM_CMA_ESmean, //12
	DCM_CMA_ESbest, //13
	DCM_CMA_ESbestSeen, //14
	ANN, //15
	ANNangleLineSearch, //16
	Exhaustive, //17
	None
};

struct tTestPoint
{
	unsigned	sNewTP;
	unsigned	sOriginalTP;
	cGeoP		sEstimatedLocation;
	cGeoP		sOriginalLocation;
	eMethod	sMethodUsed;
	double		sErrorEstimate;
	double		sErrorActual;
	double		sAzimuth; //with respect to serving cell/site
	double		sDistance; //with respect to serving cell/site
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
	bool			sServingCell;
	unsigned	sInstKeyFixed;
	double 	sEIRP;
	unsigned	sAntPatternKey;
	double		sFrequency;
	double 	sMeasValue;
	double		sPathLoss;
	double		sRFDistEstimate; // "pathloss" corrected for frequency and EIRP 
	double 	sPredValue;
	float			sTilt;
	double		sHeight;
	double		sAzimuth;
	double 	sBeamWidth;
	double		sDistance;
	int				sTA; 		// this could be GSM TA or UMTS timeDiff
	double 	sResDist;
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
	int sAIndex;
	int sBIndex;
	double sMaxMeasValue;
};
typedef	vector<tBand> vBand;
typedef vector<cGeoP> vPoints;

struct tSiteInfo
{
	unsigned	sSiteID;
	cGeoP		sPosition;
	vCellSet		sCellSet;
	double		sMaxDist;
	unsigned	sNumInputs;
	unsigned	sNumOutputsA;
	unsigned	sNumOutputsD;
	string			sANNfileA;	
	string			sANNfileD;		
};

typedef	vector<tSiteInfo> vSiteInfo;

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

	void SetUseAntANN( bool UseAntANN) { mUseAntANN = UseAntANN;};

	void EstimatePositions();
	int SaveResults();

//	static FitFunc CostCMA_ES;
	static vPosSet mPosSets;		/// an array with all the testpoints
	static unsigned mNumInsts;	
	static unsigned mCurPosI;
	static double* mCellPathLoss;		/// Pathloss to each cell in a measurement set.
	static	cRasterFileHandler *mDEM;
	static cRasterFileHandler *mClutter;
	static cProfile mClutterProfile;
	static cProfile mDEMProfile;
	static cAntennaPattern* mFixedAnts;
	static cPathLossPredictor *mPathLoss;
	static unsigned mClutterClassGroup;
	static bool mUseClutter;
	static	double mkFactor;
	static double mPlotResolution;


//	vPosSet mPosSets;		/// an array with all the testpoints
//	unsigned mNumInsts;	
//	unsigned mCurPosI;
//	double* mCellPathLoss;		/// Pathloss to each cell in a measurement set.
//	cRasterFileHandler *mDEM;
//	cRasterFileHandler *mClutter;
//	cProfile mClutterProfile;
//	cProfile mDEMProfile;
//	cAntennaPattern* mFixedAnts;
//	cPathLossPredictor *mPathLoss;
//	unsigned mClutterClassGroup;
//	bool mUseClutter;
//	double mkFactor;
//	double mPlotResolution;


   private:

	bool CI();
	bool CI_TA();
	bool SecondSite();
	bool CoSecAzi(double &minAzi);
	double FindAzi(unsigned BIndex, unsigned AIndex=0);
	double SearchDistance(double Azimuth, double min, double max);
	bool CoSinRule();
	bool DCM_ParticleSwarm();
	bool ExhaustiveSearch();
//	int DCM_CMA_ES();
	bool ANNrun();

	// In this function the default mobile installation 
	// with height of MOBILEHEIGHT (#defined) and an isotropic antenna. 
	double CostFunction(double rho, double phi);
	bool SetSearchBoundaries();

	FANN::neural_net *mCurANNa;
	FANN::neural_net *mCurANNd;
	unsigned mCurSiteI;
	unsigned mNumPoints;
	unsigned mNewTP;
	unsigned mNumSites;
	unsigned mNum;
	eOutputUnits mUnits;
	short int mDEMsource;
	short int mClutterSource;
	unsigned *mClutterCount;
	vector<tFixed>	mFixedInsts;	///< Information on the fixed installations
	vector<tMobile>	mMobiles;	/// Information on all the mobile instruments used during the measurements
	vSiteInfo mSites;
	bool mLTEsim;
	bool mUMTS;	
	bool mOriginal;
	bool mUseAntANN;
	double mRho_max;
	double mRho_min;
	double mPhi_max;
	double mPhi_min;
	double mPhi_max_back;
	double mPhi_min_back;
};
}
#endif
