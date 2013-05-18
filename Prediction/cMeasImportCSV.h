 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasImportCSV.h
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

#ifndef cMeasImportCSV_h
#define cMeasImportCSV_h 1


// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include <QMessageBox>

using namespace std;
using namespace Qrap;

//## Class cMeasImportCSV

namespace Qrap
{
class cMeasImportCSV 
{
   public:
	cMeasImportCSV(); // default constructor
	~cMeasImportCSV(); // destructor

	cMeasImportCSV(double Sensitivity,
			double Frequency,
			unsigned CI, 
			unsigned MeasType, 
			unsigned MeasSource, 
			unsigned PosSource);

	void cSetMeas(	double Sensitivity,
			double Frequency,
			unsigned CI, 
			unsigned MeasType, 
			unsigned MeasSource, 
			unsigned PosSource,
			unsigned long LastTestPoint,
			unsigned long LastMeas);

	void cSetCI (unsigned CI) {mCI =CI;};

	void cGetMeas(	double &Sensitivity,
			double &Frequency,
			unsigned &CI, 
			unsigned &MeasType, 
			unsigned &MeasSource, 
			unsigned &PosSource,
			unsigned long &LastTestPoint,
			unsigned long &LastMeas);

	int LoadMeasurement(char *filename);

   private:

	double mSensitivity;	/// The sensitivity of the reciever or the noise floor when the measurements were taken. Values equal and below this value will be discarded. 
	double mFrequency;	/// The frequency in MHz at which the measurement was taken
	unsigned mCI;		/// The primary key to the Cell from which the transmission took place.
	unsigned mMeasType;	/// The primary key to the Measurement Type (meastype) that descripes the measurement type
	unsigned mMeasSource;	/// The primary key to the Data Source (measdatasource) of the measurements
	unsigned mPosSource;	/// The primary key to the Position Source (positionsource) of the location e.g. 0 for GPS 
	unsigned long mLastTestPoint;	/// The id of the last testpoint stored
	unsigned long mLastMeas;	/// The id of the last measurement stored
	char * mCIS;		
	char * mMeasTypeS;
	char * mMeasSourceS;
	char * mPosSourceS;
	char * mFrequencyS;
};
}

#endif
