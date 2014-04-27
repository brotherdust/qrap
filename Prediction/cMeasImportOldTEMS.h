 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasImportOldTEMS.h
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

#ifndef cMeasImportOldTEMS_h
#define cMeasImportOldTEMS_h 1


// include local headers
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"
#include "cGeoP.h"
#include "cMeas.h"
#include "cTPoint.h"
#include "cCellList.h"
#include <QMessageBox>

using namespace std;
using namespace Qrap;

typedef vector<CellList> VecCells;
typedef vector<cTPoint>	 VecMMRs;
typedef	vector<cMeas> 	 VecMeas;
typedef vector<int>	 VecInt;


//## Class cMeasImportOldTEMS

namespace Qrap
{
class cMeasImportOldTEMS 
{
   public:
	cMeasImportOldTEMS(); // default constructor
	~cMeasImportOldTEMS(); // destructor

	cMeasImportOldTEMS(unsigned MeasType, 
			unsigned MeasSource, 
			unsigned PosSource);

	void cSetMeas(	unsigned MeasType, 
			unsigned MeasSource, 
			unsigned PosSource,
			unsigned long LastTestPoint,
			unsigned long LastMeas,
			unsigned LastCellListID);

	void cGetMeas(	unsigned &MeasType, 
			unsigned &MeasSource, 
			unsigned &PosSource,
			unsigned long &LastTestPoint,
			unsigned long &LastMeas,
			unsigned &LastCellListID);

	int LoadMeasurement(char *filename);

	int GetCurrentCellList();
	int ReadTEMSfile(const char* DriveTestFile, time_t FileDate);
	int SortReduceCellList();
	int GetCIfromBSICBCCH();
	int WriteToDatabase();
	int WriteCellListToDB();


   private:

	unsigned mMeasType;	/// The primary key to the Measurement Type (meastype) that descripes the measurement type
	unsigned mMeasSource;	/// The primary key to the Data Source (measdatasource) of the measurements
	unsigned mPosSource;	/// The primary key to the Position Source (positionsource) of the location e.g. 0 for GPS 
	unsigned long mLastTestPoint;	/// The id of the last testpoint stored
	unsigned long mLastMeas;	/// The id of the last measurement stored
	unsigned mLastCellListID;
	char * mMeasTypeS;
	char * mMeasSourceS;
	char * mPosSourceS;

	VecMMRs		m_coreMMR;
	VecMeas		m_NBMeas;
	VecCells	m_TempList;
	VecInt		m_LACList;
};
}

#endif
