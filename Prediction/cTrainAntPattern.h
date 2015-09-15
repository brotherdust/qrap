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

#ifndef cTrainAntPattern_h
#define cTrainAntPattern_h 1

#define MOBILEHEIGHT 1
#define AntFileInt	0.5 // the interval in degrees of samples that are included for the training from the supplied antenna file.
#define SUB 10 // every SUB sample will be used as a test point
// ANTSCALE is #defined in DataBase/Config.h
#define antMAXepoch 100000
#define antREPORTInt 10000
#define antERROR 1e-6

// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "cPthlssp.h"
#include "cAntpatn.h"
#include "cProfile.h"
#include "cAntpatn.h"
#include "cMemmngr.h"
#include "PredStructs.h"
#include "cRasterFileHandler.h"
#include "doublefann.h"
#include "fann_cpp.h"
#include <iostream>
#include <ctime>

using namespace std;
using namespace Qrap;


struct tMeasNNAnt
{
	unsigned	sID;
	unsigned	sOriginalTP;
	cGeoP		sLocation;
	unsigned	sCellID;
	float			sFrequency;
	float 			sMeasValue;
};

typedef	vector<tMeasNNAnt> vMeasNNAnt;

struct tCellAnt
{
	unsigned sCI;
	unsigned sRI;
	double			sTxPwr;
	double			sTxSysLoss;
	cGeoP			sPosition;
	float				sHeight;
	int					sAntPatternKey;
	double			sBearing;
	double			sTilt;
	unsigned		sNumInputs;
	unsigned		sNumOutputs;
	unsigned   	sNumTrain;
	unsigned		sNumTest;
	double			sMean;
	double			sMin;			// Actually the value at maximum gain
	double**		sInputTrain;
	double**		sOutputTrain;
	double**		sInputTest;
	double**		sOutputTest;
	vMeasNNAnt		sMeasTrain;
	vMeasNNAnt		sMeasTest;
};

typedef	vector<tCellAnt> vCellSetAnt;

typedef vector<cGeoP> vPoints;


//## Class cTrainAntPattern

namespace Qrap
{
class cTrainAntPattern
{
   public:
	cTrainAntPattern(); // default constructor
	~cTrainAntPattern(); // destructor


	bool LoadMeasurements(vPoints Points,
				unsigned MeasType=0, 
				unsigned MeasSource=0,
				unsigned PosSource=0,
				unsigned Technology=0);

	bool TrainANDSaveANDTest();

	bool Output(string Outputfile, unsigned currentCell);


   private:

	vCellSetAnt mCells;
	unsigned mNumCells;
	unsigned mNumTrain;
	unsigned mNumTest;
	bool mUseClutter;
	short int mDEMsource;
	short int mClutterSource;
	cRasterFileHandler mDEM;
	cRasterFileHandler mClutter;
	unsigned mClutterClassGroup;
	unsigned *mClutterCount;
	cPathLossPredictor mPathLoss;
	cProfile mClutterProfile;
	cProfile mDEMProfile;
	double mPlotResolution;
	double mkFactor;
	FANN::neural_net mANN;
	double mMAXANNOutput;
	unsigned mNumAzifFile, mNumElevfFile;
};
}
#endif
