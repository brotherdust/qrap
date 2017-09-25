 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cTrainPosNetDistAngle.h
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

#ifndef cTrainPosNetDistAngle_h
#define cTrainPosNetDistAngle_h 1

#define GAMMA 2.4
#define MOBILEHEIGHT 1
#define MARGIN 15
#define SENSITIVITY (-120)
#define FREQ_SCALE (2/(2170-1553))
#define FREQ_OFFSET (-1553)
#define MEAS_SCALE (1/35)
#define MEAS_OFFSET (-80)
#define RFDist_SCALE (1.5/45)
#define RFDist_OFFSET (75)

#define MAXepoch 300000
#define REPORTInt 10000
#define TERROR (0.0015/5000)

// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "cMemmngr.h"
#include "doublefann.h"
#include "fann_cpp.h"
#include <iostream>
#include <ctime>

using namespace std;
using namespace Qrap;


struct tTestPointNN
{
	unsigned	sOriginalTP;
	cGeoP		sOriginalLocation;
	unsigned	sServSite;
	double		sServCellAzimuth;
	unsigned	sTA; 		// this could be GSM TA or UMTS timeDiff
	double 		sResDist; 	// resolution of the above (554m or 36m)
	double		sBearing; 	// testpoint direction from serving site
	double		sDistance; 	// from testpoint to serving site
};

typedef	vector<tTestPointNN> vTestPointNN;

struct tMeasNN
{
	unsigned	sID;
	unsigned	sOriginalTP;
	unsigned	sCellID;
	double		sFrequency;
	double 		sMeasValue;
	double		sRFDistEstimate; // "pathloss" corrected for frequency and EIRP 
	double 		sEIRP;
};

typedef	vector<tMeasNN> vMeasNN;

struct tPosSetNN
{
	vTestPointNN sTestPoints;
	vMeasNN sMeasurements;
	unsigned sNumMeas;
};

typedef	vector<tPosSetNN> vPosSetNN;

struct tCell
{
	unsigned sI;
	unsigned sCI;
};

typedef	vector<tCell> vCellSet;

struct tSiteInfoNN
{
	unsigned 		sSiteID;
	cGeoP			sPosition;
	vCellSet 		sCellSet;
	double			sMaxDist;
	double			sMedianDist;
	unsigned		sNumInputs;
	unsigned		sNumOutputsA;
	unsigned		sNumOutputsD;
	unsigned		sNumDataRowsTrain;
	unsigned		sNumDataRowsTest;
	double**		sInputTrain;
	double**		sOutputAngleTrain;
	double**		sOutputDistTrain;
	double**		sInputTest;
	double**		sOutputAngleTest;
	double**		sOutputDistTest;
};

typedef	vector<tSiteInfoNN> vSiteInfoNN;

typedef vector<cGeoP> vPoints;


//## Class cTrainPosNetDistAngle

namespace Qrap
{
class cTrainPosNetDistAngle 
{
   public:
	cTrainPosNetDistAngle(); // default constructor
	~cTrainPosNetDistAngle(); // destructor

	bool LoadSites(vPoints Points,
				unsigned MeasType=0, 
				unsigned MeasSource=0,
				unsigned PosSource=0,
				unsigned Technology=0);

	bool LoadMeasurements(vPoints Points,
				unsigned MeasType=0, 
				unsigned MeasSource=0,
				unsigned PosSource=0,
				unsigned Technology=0,
				string list="Train",
				bool Train=true /* Train: true, Test: False*/);

	bool TrainANDSaveANDTest();


   private:

	vPosSetNN mPosSetsTrain;	/// an array with all the training points
	vPosSetNN mPosSetsTest;		/// an array with all the training points
	vSiteInfoNN mSites;
	unsigned mNumSites;
	unsigned mNumTrain;
	unsigned mNumTest;
	bool mLTEsim;
	bool mUMTS;
	bool mOriginal;
};
}
#endif
