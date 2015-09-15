 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLink.h
 *    Copyright   : (c) University of Pretoria & Meraka Institute CSIR
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : (Dirk van der Merwe)
 *    Description : A rather clumsy implemetation of a link analysis. 
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

#ifndef Qrap_cLink_h
#define Qrap_cLink_h

 
// Include local headers
#include "PredStructs.h"
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cProfile.h"
#include "cPthlssp.h"
#include "cClutter.h"
#include "cAntpatn.h"
#include "cRasterFileHandler.h"


namespace Qrap
{
	/**
	 * Class cLink
	 */
	class cLink
	{
		public:
			/**
			 * Class constructor.
			 */
			cLink ();
			
			/**
			* Class destructor.
			*/
			const cLink & operator=(const cLink &right);
			
			/**
			 * Class destructor.
			 */
			virtual ~cLink ();
			
			/**
			 * Description of ReadPlotRequest
			 * 
			 * @param filename The file name.
			 * 
			 * @return A boolean.
			 */
			bool ReadPlotRequest (const char *filename);
			
			/**
			 * Description of WriteOutput
			 */
			void WriteASCIIMatrix(FILE *fp);
			
			/**
			 * Description of CombinCov
			 * 
			 * @return A boolean.
			 */
			bool DoLink (bool Trial=false, double MaxDist=MAXDOUBLE);

			void SaveLink(string &linkname, int &LinkID);

			void SetPropLoss(const float* InPropLoss);

			int 	GetSize() { return mLength; }
			double 	GetInterDist() { return mPlotResolution; }
		    	double 	GetMinClearance() { return mMinClearance; }
			double 	GetPathLoss() { return mPathLossEnd; }
			double 	GetTxElevation() { return mTxElevation; }
			double 	GetRxElevation() { return mRxElevation; }
			double 	GetTxTilt() { return mTxTilt; }
			double 	GetRxTilt() { return mRxTilt; }
			double 	GetTxBearing() {return mTxBearing; }
			double 	GetRxBearing() {return mRxBearing; }
			double 	GetFreq() {return mFrequency;}
                        double 	GetEIRP() {return EIRP;}
                        double 	GetRxLev() {return mRxLevEnd;}
                        double 	GetDistance() {return mDist;}

			bool SetTxSite(int TxSite, tFixed Inst);
			bool SetRxSite(int RxSite, tFixed Inst);
			void SetTxInst(int TxInst){mTxInst.sInstKey=TxInst;};
			void SetRxInst(int RxInst){mRxInst.sInstKey=RxInst;};
			void ReSetPlotRes(int PlotRes){mPlotResolution=PlotRes;};
			
			void GetLinkInfo(int &Size, double &InterPixDist, 
					double &MinClearance, double &Freq,
					double &TxElevation, double &RxElevation,
					double &TxTilt, double &RxTilt,
		                	double &TxBearing, double &RxBearing,
					double &EndLoss, double &RxLev,
					double &TxAntHeight, double &RxAntHeight,
					double &TxRealTilt, double &RxRealTilt,
					double &TxRealBearing, double &RxRealBearing,
					double &TxLat, double &TxLon,
					double &RxLat, double &RxLon,
					double &maxH, double &minH);

						
						
			void SetLink(eOutputUnits	Units,
				bool	Downlink,
				double	Frequency,
				double	kFactor,
				double	PlotResolution,
				short int DEMsource,
				short int ClutterSource,
				bool &UseClutter,
				int TxInst, //This will be site IDs in the case of trial Links
				int RxInst, //This will be site IDs in the case of trial Links
				bool Trail = false);

			float 	*mEffProfile;	///< The effective earth profile after taking the k-Factor into account
			float	*mLineOfSight;	///< Line from Transmitter to Receiver
			float	*mFresnelH;	///< Higher first fresnel zone line
			float	*mFresnelL;	///< Lower first fresnel zone line
			float	*mFresnelSH;	///< Higher 60% of first fresnel zone line
			float	*mFresnelSL;	///< Lower line of 60% of first fresnel zone	
			float	*mPropLoss;	///< Array storing the propagation loss at each point along the profile
			float	*mRxLev;	///< Array storing the signal strength at each point along the profile
			float	*mTrueHeightGrid1;
			float	*mTrueHeightGrid2;
			float	*mTrueHeightGrid3; ///< Gridlines against which the true heights can be measured
		
		private:
			
			/**
			 * Description of GetDBinfo
			 */
			bool GetDBinfo(tFixed &Inst);
			bool GetDBinfoS(tFixed &Inst);
			
			void Initialize(const cProfile& DEMProfile, const cProfile& ClutterProfile);

			void SetEffProfile();

			void SetLineOfSight();

			void SetHeightGridLines();

			void SetFresnelClear();
			// Set Higher and lower (100% and 60%) first fresnel zone arrays
			// and the array with the Clearance at each point

			static double reR;			///< real earth Radius in m
			static double c;			///< Speed of Light in m/s
			cPathLossPredictor 	PathLoss;			
			eOutputUnits		mUnits;		///< Requested output units of link
			bool			mDownlink;	///< Is the TxInst transmitting? 
			double			mFrequency;	///< Operating frequency of Link
			double			mkFactor;	///< k Factor used to calculate Effective earth radio(vs. real earth)
			double			mPlotResolution;///< Requested plot resolution in meter
			cRasterFileHandler 	mDEM;		///< Rasterfile handler to get height data
			short int		mDEMsource;	///< Key to file-set-order array for height data
			short int		mClutterSource;	///< Key to file-set-order array for clutter
			bool			mUseClutter;	///< Do we use clutter information?
			unsigned		mClutterClassGroup;
			cRasterFileHandler 	mClutter;	///< Rasterfile handler to get Clutter
			cClutter		mClutterset;
			bool			mTrial;		///< Is this only a trail link (true) or actual (false)?
			tFixed			mTxInst;	///< Transmitting Radio Installation
			tFixed			mRxInst;	///< Receiving Radio Installation
			cAntennaPattern 	mTxAnt;		///< AntennaPattern of Transmitting Radio installation
			cAntennaPattern 	mRxAnt;		///< AntennaPattern of Receiving Radio installation
			int			mLength;	///< Size of the arrays/profiles
			double 			mSlope;		///< Slope of LineOfSight
			double 			mEffRadius;	///< Effective earth radius
			double 			mMinClearance;	///< Minimum fresnel clearance
			double			mPathLossEnd;	///< PathLoss over path
                        double			mRxLevEnd;	///< Signal level at receiver
                        double                  mDist;          ///< Distance between transmitter and receiver
			float			mTxTilt;	///< Recommended tilt on transmitting antenna
			float 			mRxTilt;	///< Recommended tilt on receiving antenna
			double 			mTxBearing;	///< Recommended bearing of transmitting antenna
			double 			mRxBearing;	///< Recommended bearing of receiving antenna
			double 			mTxElevation;	///< True height of Tx site
			double 			mRxElevation;	///< True height of Rx site
			float 			*mFlatProfile;	///< The flat earth profile not taking the true earth radius into account
			float 			*mClearance;	///< Clearance at each point
			int 			*mClutterProfile;
                        double                  EIRP;
			double 			mMinimum;
			double 			mMaximum;
			bool mUseAntennaANN;
	};
}
#endif 
