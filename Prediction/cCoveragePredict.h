 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCoveragePredict.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Adds installations effects to cBTLPredict.
 *                : several slot into cPlotTask. 
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

#ifndef Qrap_cCoveragePredict_h
#define Qrap_cCoveragePredict_h

// Include local headers
#include "cAntpatn.h"
#include "cMemmngr.h"
#include "cBTLPredict.h"
#include "../DataBase/cBase.h"
#include "cGeoP.h"


namespace Qrap
{
	/**
	 * Description of class cCoveragePredict
	 */
	class cCoveragePredict 
	{
		public:
			/**
			 * Constructor
			 */
			cCoveragePredict ();
			
			/**
			 * Destructor
			 */
			virtual ~cCoveragePredict ();
			
			/**
			 * Description of SetCommunicationLink
			 * 
			 * @param SiteID Description
			 * @param DownLink Description
			 * @param TxPower Description
			 * @param TxSysLoss Description
			 * @param RxSysLoss Description
			 * @param RxSens Description
			 * @param FixedPatternKey Description
			 * @param FixedAzimuth Description
			 * @param FixedMechTilt Description
			 * @param MobilePatternKey Description
			 * @param FixedHeight In Meters.
			 * @param MobileHeight In Meters.
			 * @param Frequency In MHz.
			 * @param kFactor Description
			 * @param Radius In Meters.
			 * @param DistanceRes In Meters.
			 * @param AngleRes Input in degrees.
			 * @param DTMsource Key to source.
			 * @param ClutterSource Description.
			 *
			 * @return It returns 1 if it does exist and mBTL and mTilt is populated.
			 *	   It return 2 if it needs to be collected from the file . ReadBTL, GetBTL
			 *	   It return 3 if the height (and clutter) data needs to be prepared and then the PredictBTL must be called.
			 */
			int SetCommunicationLink (int	SiteID,
						bool 	DownLink,
						double 	&EIRP,
						double	TxPower,
						double	TxSysLoss,
						double	RxSysLoss,
						double	RxSens,
						int	FixedInst,
						double	FixedAzimuth,
						double	FixedMechTilt,
						int	MobileInst,
						double 	FixedHeight,
						double 	MobileHeight,
						double 	&Frequency,
						double 	kFactor,
						double 	&Radius,
						double 	DistanceRes,
						unsigned 	NumAngles,
						int 	DTMsource,
						int 	ClutterSource);
			
			
			/**
			 * This implementation assumes that the Rx antenna is with the direction of maximum gain towards the Tx antenna.
			 * 
			 * @param AfterReceiver Description
			 *
			 * @return A boolean.
			 */
			bool CalculateRadialCoverage (bool AfterReceiver);
			
			/**
			 * Description of InterpolateToSquare
			 * 
			 * @param res Resolution in meters.
			 * @param NW The center of the top-left corner.
			 * @param Result Description
			 * @param size Description
			 *
			 * @return A boolean.
			 */
			bool InterpolateToSquare ( cGeoP SitePos, cGeoP NW,
										Float2DArray &Result, 
										double res, int &rowa, int &cols); 
			
			cBTLPredict mBTLPredict;	///< Description
			
		private:
			// Note that the receive (Rx) and transmit (Tx) refer here to the T
			// Tx and Rx on either side of the link and not the Tx and Rx of the same radio. 
		
			int			mSiteID;		///< Description
			bool 		mDownLink;		///< Are we calculating the downlink (TRUE) or uplink (False)
			double 		mEIRP;			///< In dBm
			double		mTxPower;		///< In Watt
			double		mTxSysLoss;		///< In dB
			double		mRxSysLoss;		///< In dB
			double		mRxSens;		///< In dBm
			int			mFixedInst;	///< Description
			double		mFixedAzimuth;		///< In degrees 0 North, positive clockwise
			double		mFixedMechTilt;		///< Description
			int			mMobileInst; 	///< Description
			cAntennaPattern mFixedAntenna;		///< Description
			cAntennaPattern mMobileAntenna;		///< Description
			bool		mBTLloaded;		///< Description
			double  	mAngleRes;		///< In degrees
			double		mDistRes;		///< In meters
			unsigned 		mNumAngles;		///< Description
			unsigned			mNumRadialPoints;	///< Description
			Float2DArray	mBTL;			///< Description
			Float2DArray	mTilt;			///< Description
			Float2DArray	mRxLev;			///< Description
	};
}
#endif
