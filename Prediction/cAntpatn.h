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

	enum eAnt
	{
		Tx,
		Rx,
		Mobile
	};

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
			 * @param Key: The CellID
			 * @param UseANN: Use result of the antenna neural net for the cell to get the antenna values
			 * @param Bearing: Azimuth direction of the antenna. In the case of a neural net, it is assumed that the antenna is still directed in direction it was when it was trained then Bearing = 0
			 * @param MechTilt: The mechanical tilt of the antenna. In the case of a neural net, it is assumed that the antenna is still directed in direction it was when it was trained then MechTilt = 0
			 *
			 * @return An integer.
			 */
			bool SetAntennaPattern(int RadKey, eAnt Type = Tx, 
															double Bearing=0, double MechTilt=0);

			void SetUseAntANN( bool UseAntANN) { mUseANN = UseAntANN;};

			bool ResetUseAntANN();
		
			
			/**
			 * Description of GetPatternValue
			 * 
			 * @param Azimuth: The direction of the mobile (relative to north)  with respect to the site
			 * @param Elevation: The angle from the horizon that the mobile is with respect to the fixed installation antenna. Down is possitive.
			 *
			 * @return A double: the value of the antenna pattern in that direction. 
			 */
			double GetPatternValue(double Azimuth, double Elevation);
			
			
			char *mFile;		///< Filename containing antenna pattern or neural net
			char *mPol;		///< Polarisation of transmitted/received E-field
			double mFreq;		///< Frequency where antenna was measured
			double mGain;		///< Maximum Gain of antenna pattern
			double mBeamW;		///< Azimuth beamwidth
			double mVBeamW;		///< Elevation beamwidth
			double mFtoB;		///< Front to Back ratio			
			double mPrice;		///< Price of antenna
			float *mAziAngles;	///< Array linked to mAziValues that indicate the angle[i] at which the azimuth pattern is mAziValues[i]
			float *mAziValues;	///< The relative value[i] of the antenna pattern at azimuth mAziAngles[i]. (Antenna Radiation Level = Gain - mAziValues[i])
			float *mElevAngles;	///< Array linked to mElevValues that indicate the angle[i] at which the elevation pattern is mElevValues[i]
			float *mElevValues;	///< The relative value[i] of the antenna pattern at elevation mElevAngles[i]. (Antenna Radiation Level = Gain - mElevValues[i])
			unsigned mNAA;		///< Number of Elevation Values
			unsigned mNEA;		///< Number of Azimuth Values
		
		private:	
			double mBearing;	///< The bearing /azimuth direction the antenna is pointing at
			double mMechTilt;	///< The mechanical downtilt of the antanna
			unsigned mRef180az;	///< Reference index of the "back" of the antenna
			unsigned mRef0el;	///< Reference index to the "horizon" of the antanna
			Float2DArray mAntPattern;	///< Matrix containing the values of the antenna pattern at all mAziAngles and mElevAngles
			FANN::neural_net *mAntennasANN;
			bool mUseANN;
			int mElecTiltRef;
			double mElecTiltValue;
			double mMinValueElev;
			double mMinValueAzi;
			double mScale;
	};
}

#endif
