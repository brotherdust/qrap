 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cTrainPosNet.h
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

#ifndef cTrainPosNet_h
#define cTrainPosNet_h 1

#define GAMMA 2.5
#define MOBILEHEIGHT 1
#define SIGMA2 (14*14)
#define MARGIN 15
#define SENSITIVITY -110
#define TA_SCALE 0.07
#define DIST_SCALE 0.00012
#define FREQ_SCALE (0.3/450)
#define FREQ_OFFSET (-945-450)
#define MEAS_SCALE (0.5/(85-65))
#define MEAS_OFFSET 85
#define RFDist_OFFSET -80

#define MAXepoch 200000
#define REPORTInt 10000
#define ERROR 1e-4

// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "cMemmngr.h"
#include "doublefann.h"
#include "fann_cpp.h"
#include <iostream>
#include <random>
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
	unsigned		sNumInputs;
	unsigned		sNumOutputs;
	unsigned		sNumDataRows;
	double**		sInput;
	double**		sOutput;
};
typedef	vector<tSiteInfoNN> vSiteInfoNN;

typedef vector<cGeoP> vPoints;


//## Class cTrainPosNet

namespace Qrap
{
class cTrainPosNet 
{
   public:
	cTrainPosNet(); // default constructor
	~cTrainPosNet(); // destructor

	bool LoadMeasurements(vPoints Points,
				unsigned MeasType=0, 
				unsigned MeasSource=0,
				unsigned PosSource=0,
				unsigned Technology=0);

	bool TrainANDSave();


   private:

	vPosSetNN mPosSets;		/// an array with all the testpoints
	vSiteInfoNN mSites;
	unsigned mNumSites;
	unsigned mNumPoints;
};
}
#endif
