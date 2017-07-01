 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBTLPredict.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Respresents a Basic Transmission Loss Prediction.
 *                : contained by each cCoveragePredict. 
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

#ifndef Qrap_BTLPredict_h
#define Qrap_BTLPredict_h

//#include <math.h>
//#include <stdio.h>
//#include <fstream>
//#include <stdlib.h>
//#include <string>

// Incluce local headers
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cProfile.h"
#include "cPthlssp.h"
#include "../DataBase/Config.h"

// All 2D arrays are stored as Angles(first index) and Distances(second index)

namespace Qrap
{
	/**
	 * This class calls cPathLossPredictor to calculate the Basic Transmission Loss to each point. The result is stored in a 2 dimensional array, with each point representing an angle relative to north and distance from the site.
	 */
	class cBTLPredict
	{
		public:
			/**
			 * Constructor
			 */
			cBTLPredict();
			
			/**
			 * Destructor
			 */
			virtual ~cBTLPredict();
			
			/**
			 * This routines sets up all the variables for the prediction and check the Database for the existance.
			 * 
			 * @param SiteID Description
			 * @param Radius In meters.
			 * @param DistanceRes In meters.
			 * @param AngleRes Input in degrees.
			 * @param Frequency In MHz.
			 * @param FixedHeight In meteres.
			 * @param MobileHeight Description
			 * @param kFactor Description
			 * @param DTMSource Key to source.
			 * @param ClutterSource Description
			 *
			 * @return It returns 0 if it does exist and mBTL and mTilt is populated.
			 *	   It returns the ID of the BTL if it needs to be collected from the file . ReadBTL, GetBTL.
			 *	   It returns -1 if the height (and clutter) data needs to be prepared and then the PredictBTL must be called.
			 */
			int Check_and_SetBTL (	int SiteID, 
						double 		&Radius,
						double 		&DistanceRes,
						unsigned 	&NumAngles,
						double 		&Frequency,
						double 		&FixedHeight,
						double 		&MobileHeight,
						double 		&kFactor,
						int 		DTMsource = 1,
						int 		ClutterSource = 1);
			
			/**
			 * This routine runs a prediction.
			 * 
			 * @param NumbAngles Description
			 * @param NumDistance Description
			 * @param DistanceRes Description
			 * @param DTM Description
			 * @param UseClutter Description
			 * @param Clutter Description
			 */
			int PredictBTL (unsigned NumAngles,
							unsigned NumDistance,
							double DistanceRes,
							Float2DArray DTM,
							bool UseClutter,
							unsigned ClutterClassGroup,
							Float2DArray Clutter);
			
			/**
			 * Description of GetBTL
			 * 
			 * @param NumAngles Description
			 * @param NumRadialPoints Description
			 * @param mSiteLocation Description
			 * @param Radius In meters.
			 * @param DistanceRes In meters.
			 * @param AngleRes In radius.
			 *
			 * @return A boolean.
			 */
			bool GetBTL (	unsigned &NumAngles,
					unsigned &NumRadialPoints,
					double &Radius,
					double &DistanceRes);


			void SetMaxPathLoss(double MaxPathLoss) 
			{
				mMaxPathLoss = MaxPathLoss;
				mMaxPathLossReached = false;
			};
			
			double GetRange() 	{ return mRange; };
			double GetDistRes() 	{ return mDistanceRes; };
			double GetAnglesRes() 	{ return mAngleRes; };
	
			Float2DArray mBTL;	///< stores the elevation/heigh data
			Float2DArray mTilt;	///< stores the clutter data
			
			int	mSiteID;	///< Site ID of the centre of the prediction.
		
		private:

			/**
			 * Description of ReadBTL
			 */
			bool ReadBTL();
			
			/**
			 * Description of StoreBTL
			 */
			int StoreBTL();
		
			bool		mArraysPopulated;	///< Are the arrays populated
			bool		mMaxPathLossReached; ///<Indicates whether range was limited because the maximum pathloss was reached.
			double		mMaxPathLoss;
			int 		mBTLid;
			double		mAngleRes; 		///< In degrees
			unsigned 	mNumAngles;		///< Description
			double		mDistanceRes;		///< In meters
			unsigned	mNumRadialPoints;	///< Description
			double		mFrequency;		///< In MHz
			double		mkFactor;		///< Used in effective earth model to represent refractivity
			double		mRange;			///< In meters
			double		mFixedHeight;		///< In meters
			double		mMobileHeight;		///< In meters
			int 		mDTMsource;		///< "key" to indicate which DTM set/rule was used for prediction
			int 		mClutterSource;		///< "key" to indicate which Clutter set/rule was used for prediction
			// stores the tilt that the Tx antenna will have to a point.
			string		mBTLfile;  		///< Name of the BTL file
			string		mBTLdir;		///< Description
	};
}
#endif
