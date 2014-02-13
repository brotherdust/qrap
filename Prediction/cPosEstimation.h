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

enum eMethod
{
	GPS,
	CellID,
	CellID_RxTx,
	SSiteDir, // The direction is based on the direction of the second strongest cell
	CoSiteSecDir,
	CosineRule,
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

typedef tPosSet
{
	vTestpoint sTestPoints,
	vMeas sMeasurements,
	unsigned sNumMeas
};

typedef	vector<tPosSet> vPosSet;


//## Class cPosEstimation

namespace Qrap
{
class cPosEstimation 
{
   public:
	cPosEstimation(); // default constructor
	~cPosEstimation(); // destructor

	bool LoadMeasurements(QList<QgsPoint> Points,
					unsigned MeasType=0, 
					unsigned MeasSource=0,
					unsigned PosSource=0,
					unsigned Technology=0);


	int SaveResults();


   private:

	bool CI();
	bool CI_TA();
	bool SecondSite();
	bool CoSecAzi(double &minAzi);
	double FindAzi(unsigned BIndex, unsigned AIndex=0);
	bool CoSinRule();

	vPosSet mPosSets;		/// an array with all the testpoints
	unsigned mCurrentPosSetIndex;
	unsigned mNumPoints;
	unsigned mNewTP;
	
};
}
#endif
