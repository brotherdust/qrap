 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cTrafficDist.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : The entry class for all propagation prediction plots
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

#ifndef Qrap_cTrafficDist_h
#define Qrap_cTrafficDist_h

// Include local headers
#include "PredStructs.h"
#include "cCoveragePredict.h"
#include "cBTLPredict.h"
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cRasterFileHandler.h"
#include <string>
#include <Eigen/Dense>

using namespace Eigen;

namespace Qrap
{
	
	/**
	 * typedef vPredict:
	 */
	typedef vector<tSignalRaster>	vPredict;
	
	/**
	 * Class cTrafficDist:
	 * 
	 * This class is the main entry point
	 */
	class cTrafficDist
	{
		public:
			/**
			 * Class default constructor.
			 */
			cTrafficDist ();
			
			/**
			 * Class destructor.
			 */
			virtual ~cTrafficDist ();
			
				
			/**
			 * Description of WriteOutput:
			 */
			bool WriteOutput (GeoType OutputProj);
			
			/**
			 * Perform a primary server plot for all the radioinstallations
			 * 
			 * @return A boolean.
			 */
			bool PrimServPlot ();

			/**
			 * Determine a matrix containing the area of each clutter type within a radioinstallations coverage area.
			 * 
			 * @return A boolean.
			 */
			bool DetermineClutterDist ();
			
			/*
			 * 
			 */
			bool SetTrafficDist(	eOutputUnits DisplayUnits,
						bool Downlink,
						double RequiredSignalToNoise,
						double RequiredMinimumReceiverLevel,
						double FadeMargin,
						double RequiredEnergyPerBitToNoiseRatio,
						double NoiseLevel,
						double kFactorForServers,
						short int DEMsourceList,
						short int ClutterSourceList,
						bool UseClutterDataInPathLossCalculations,
						cGeoP NorthWestCorner,
						cGeoP SouthEestCorner,
						double PlotResolution,
						double MinimumAngularResolution,
						unsigned MobileInstallationKey,
						double CoverangeRanges, // In Kilometer
						string DirectoryToStoreResult,
						string OutputFileForResult);
		
		private:
			
			/**
			 * Description of GetDBinfo
			 */
			bool GetDBinfo();
			

			/**
			 * Description of OrderAllPred
			 */
			int OrderAllPred();
			
			/**
			 * Description of UpdateActiveRasters
			 * 
			 * @param Now Description
			 * @param Advance Description
			 */
			unsigned UpdateActiveRasters(int Now, int Advance);
			
			eOutputUnits	mUnits;		///< The units in which the result is requested ... in case of Coverage
			bool		mDownlink;	///< Is it a downlink(true) or uplink(false) calculation 
			double		mRqSN;		///< Minimum required signal to Noise ratio in dB
			double		mRxMin;		///< Minimum required Received Signal Strength in dBm
			double		mFadeMargin;	///< The fademargin required dB
			double		mRqEbNo;	///< The minimum required energy per bit to noise ratio 
			double		mNoiseLevel; 	///< The noise floor ??? dBm
			double		mkFactorServ;	///< kFactor used in effective earth model for coverage calculations
			short int	mDEMsource;	///< the primary key to the FILESETSUSED table for selection of the DEM data 
			short int	mClutterSource;	///< the primary key to the FILESETSUSED table for selection of the Clutter data
			cRasterFileHandler 	mDEM;	///< RasterfileHandler to get the rasters containing the DEM for the upcoming prediction Radials at different angels 
			bool		mUseClutter;	///< Is clutter data used in the calculations?
			unsigned	mClutterClassGroup; ///< The classification used for the clutter ... it depends on the file-set. 
			cRasterFileHandler 	mClutter;	///< RasterFileHandler to get the rasters containing the Clutter for the upcoming prediction Radials at different angels 
			cGeoP		mNorthWest;	///< North-Western corner of plot area
			cGeoP		mSouthWest;	///< South-Western corner of plot area
			cGeoP		mNorthEast;	///< North-Eastern corner of plot area
			cGeoP		mSouthEast;	///< South-Eastern corner of plot area
			double		mCentMer;	///< CentralMeridian of Output
			double		mPlotResolution;///< Resolution of the plot in meters
			double		mNSres;		///< North-South resolution in degrees
			double		mEWres;		///< West-East resolution in degrees
			double		mMinAngleRes;	///< Minimum angle resolution for individual coverage plots in degrees
			double 		mMaxCoverageRange;
			unsigned	mNumAngles;	///< Number of angles in 360 used individual coverage plots
			tMobile		mMobile;	///< Information on the mobile units
			unsigned	mNumFixed;	///< Number of fixed radion installations.
			vector<tFixed>	mFixedInsts;	///< Inforamtion on the fixed installations
			Float2DArray	mPlot;		///< Result of the plot
			Float2DArray	mSupportPlot;	///< secodary/supporting information e.g. signal strength incase of Prim Server
			unsigned	mRows;		///< Number of rows of the plot
			unsigned	mCols;		///< Number of colunms of the plot
			unsigned	mInstCounter; 	///< Ag ek kan nie onthou nie .. ek is nie eens seker of dit gebruik word nie ...???
			vPredict	mActiveRasters;	///< Rasters containing individual plots (in rectangular coordinates) ... normally the signal strenth in dBm
			double		mCurrentEdge;	///< indicate how the plot progress through the area sourthern (or eastern) edge of the area already calculated  
			double		mMaxPathLoss;	///< Maximum pathloss that needs to be considers 
			double		mMaxRange;	///< Maximum range that will be considered
			string		mDir;		///< Directory of the output file
			string		mOutputFile;	///< Name of the output file

			MatrixXd mTheMatrix;	// matrix representing the set of linear equations to be solved
			MatrixXd mTraffic;
			MatrixXd mWeights;

	public:
			double *mPSweights;
			double *mCSweights;
			
	};
}
#endif /*CTrafficDist_H_*/
