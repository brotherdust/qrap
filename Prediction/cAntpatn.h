 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAntpatn.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents an antenna in a radio installation
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

#ifndef Qrap_cAntpatn_h
#define Qrap_cAntpatn_h

// Include local headers
#include "cMemmngr.h"
#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <math.h>
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"

// local defines
#define DB_DESCRIB_SIZE 150
#define DB_NAME_SIZE 40
#define DB_MAKE_SIZE 40
#define DB_FILE_SIZE 40

using namespace std;

namespace Qrap
{
	/**
	 * This is to read the antenna file structures and import it into the database.
	 */
	class cAntennaPattern
	{
	
		public:
			/**
			 * Constructor
			 */
			cAntennaPattern();
			
			/**
			 * Destructor
			 */
			~cAntennaPattern();
									
			/**
			 * Description of SetAntPattern
			 * 
			 * @param DBase Description
			 * @param Ant_PatternKey Description
			 * @param Bearing Description
			 * @param MechTilt Description
			 *
			 * @return An integer.
			 */
			bool SetAntennaPattern(int Ant_PatternKey, double Bearing, double MechTilt);
			
			/**
			 * Description of GetPatternValue
			 * 
			 * @param Azimuth Description
			 * @param Elevation Description
			 *
			 * @return A double
			 */
			double GetPatternValue(double Azimuth, double Elevation);
			
			
			char *mPatternFile;		///< Filename containing antenna pattern
			char *mName; 			///< Antenna Pattern unique name
			char *mMake; 			///< Manufacturer
			char *mDescription;		///< Describtion of antenna pattern 
			char *mPol;			///< Polarisation of transmitted/received E-field
			double mFreq;			///< Frequency where antenna was measured
			double mGain;			///< Maximum Gain of antenna pattern
			double mBeamW;			///< Azimuth beamwidth
			double mVBeamW;			///< Elevation beamwidth
			double mFtoB;			///< Front to Back ratio			
			double mPrice;			///< Price of antenna
			float *mAziAngles;		///< Description
			float *mAziValues;		///< Description
			float *mElevAngles;		///< Description
			float *mElevValues;		///< Description
			unsigned mNAA;			///< Number of Elevation Values
			unsigned mNEA;			///< Number of Azimuth Values
		
		private:	
			double mBearing;			///< Description
			double mMechTilt;			///< Description
			unsigned mRef180az;				///< Description
			unsigned mRef0el;				///< Description
			unsigned mRefAgterEl;
			Float2DArray mAntPattern;	///< Description
	};
}

#endif
