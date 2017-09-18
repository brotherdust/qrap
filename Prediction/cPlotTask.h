 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cPlotTask.cpp
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

#ifndef Qrap_cPlotTask_h
#define Qrap_cPlotTask_h

// Include local headers
#include "PredStructs.h"
#include "cCoveragePredict.h"
#include "cBTLPredict.h"
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cRasterFileHandler.h"
#include <string>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;

namespace Qrap
{
	/**
	 * enum ePlotType
	 */
enum ePlotType
{
	DEM,		///< Simply display the heights in the given area. Implemented in cRasterFileHandler::GetForDEM
	CellCentroid,	///< Determine and store the cell centroids and display the Primary Server plot.
	TrafficDist,	///< Estimate traffic density per clutter type.  
	Cov,		///< Combined Coverage, providing the strongest receivable signal strength at each point. Implemented in CombineCov
	PrimServer,	///< Primary Server providing the installation key of the strongest server. Implemented in CombineCov 
	SecondServer,	///< Secondary Server, proving the key to second strongest server. Implemented in CombineCov
	NumServers,	///< The number of server above Minimum required signal strength. Implemented in CombineCov
	IntRatioCo,	///< Co-channel Interference Ratio in dB. Implemented in InterferencePlot.
	IntRatioAd,	///< Adjacent channel Interference Ratio in dB. Implemented in InterferencePlot.
	IntAreas,	///< Interfered areas where co- or adjancent channel interference above the threshold occur. Implemented in InterferencePlot. 
	NumInt,		///< Number of interferers above the threshold. Implemented in InterferencePlot.	
	PrimIntCo,	///< The dominant co-channel interferer. Implemented in InterferencePlot.	
	PrimIntAd,	///< The dominant adjacent channel interferer.Implemented in InterferencePlot.
	SN,		///< Signal to Noise Ratio. Implemented in CombineCov
	EbNo,		///< Energy per Bit to Noise energy ratio -- not yet implemented
	ServiceLimits	///< -- not yet implemented
};	
	/**
	 * typedef vPred:
	 */
	typedef vector<tSignalRaster>	vPred;
	
	
	/**
	 * Class cPlotTask:
	 * 
	 * This class is the main entry point
	 */
	class cPlotTask
	{
		public:
			/**
			 * Class default constructor.
			 */
			cPlotTask ();
			
			/**
			 * Class destructor.
			 */
			virtual ~cPlotTask ();
			
			/**
			 * Description of ReadPlotRequest:
			 * This procedure is build in for debugging purposes.
			 * It read the information that would otherwise be coming from
			 * the interface and initialises the plot request.
			 * It assumes the file is in the current directory
			 * 
			 * @param filename: The file name containing the information for the plot.
			 * 
			 * @return A boolean - true if it works.
			 */
			bool ReadPlotRequest (const char *filename);
			
			/**
			 * Description of WriteOutput:
			 */
			bool WriteOutput (GeoType OutputProj);
			
			/**
			 * Description of CombinCov
			 * 
			 * @return A boolean.
			 */
			bool CombineCov ();
			
			/**
			 * Description of InterferencePlot
			 * 
			 * @return A boolean.
			 */
			bool InterferencePlot();

			/**
			 * CellCentroids calculate the cell centroids and updates them in the database. 
			 * after a primary server plot has been performed by calling CombineCov (). 
			 * @return A boolean indicating success of calculation.
			 */
			bool CellCentriods();

			/**
			 * Estimatess and stores the traffic density in each cluttertype for th
			 * given technology (e.g. GSM, UMTS ot LTE) and 
			 * traffictype (e.g. circuit switched or packet switched) 
			 * after a primary server plot has been performed by calling CombineCov (). 
			 * @return A boolean indicating success of calculation.
			 */
			bool DetermineTrafficDist(bool Packet=true);
			
			/*
			 * 
			 */
			bool SetPlotTask(	ePlotType PlotType,
						eOutputUnits DisplayUnits,
						bool Downlink,
						double RequiredSignalToNoise,
						double RequiredMinimumReceiverLevel,
						double FadeMargin,
						double RequiredCoChannelCarrierToInterference,
						double RequiredAdjacentCarrierToInterference,
						double RequiredEnergyPerBitToNoiseRatio,
						double NoiseLevel,
						double kFactorForServers,
						double kFactorForInterferers,
						short int DEMsourceList,
						short int ClutterSourceList,
						bool UseClutterDataInPathLossCalculations,
						cGeoP NorthWestCorner,
						cGeoP SouthWestCorner,
						double PlotResolution,
						double MinimumAngularResolution,
						unsigned MobileInstallationKey,
						unsigned NumberOfFixedInstallations,
						unsigned *FixedInstallationKeys,
						double *CoverangeRanges, // In Kilometer
						string DirectoryToStoreResult,
						string OutputFileForResult);
		
		private:
			
			/**
			 * Description of GetDBinfo
			 */
			bool GetDBinfo();
			
			/**
			 * Description of GetDBIntInfo
			 */			
			bool GetDBIntInfo();
			
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
			
			ePlotType		mPlotType;		///< The type of plot requested
			eOutputUnits	mUnits;			///< The units in which the result is requested ... in case of Coverage
			bool			mDownlink;		///< Is it a downlink(true) or uplink(false) calculation 
			double			mRqSN;			///< Minimum required signal to Noise ratio in dB
			double			mRxMin;			///< Minimum required Received Signal Strength in dBm
			double			mFadeMargin;	///< The fademargin required dB
			double			mRqCIco;		///< The minimum required co-channel Carrier-to-interference ratio in dB
			double			mRqCIad;		///< The minimum required adjacent-channel Carrier-to-interference ratio in dB
			double			mRqEbNo;		///< The minimum required energy per bit to noise ratio 
			double			mNoiseLevel; 	///< The noise floor ??? dBm
			double			mkFactorServ;	///< kFactor used in effective earth model for coverage calculations
			double			mkFactorInt;	///< kFactor used in effective earth model for interference calculations
			short int		mDEMsource;		///< the primary key to the FILESETSUSED table for selection of the DEM data 
			short int		mClutterSource;	///< the primary key to the FILESETSUSED table for selection of the Clutter data
			cRasterFileHandler 	mDEM;		///< RasterfileHandler to get the rasters containing the DEM for the upcoming prediction Radials at different angels 
			bool			mUseClutter;	///< Is clutter data used in the calculations?
			unsigned		mClutterClassGroup; ///< The classification used for the clutter ... it depends on the file-set. 
			cRasterFileHandler 	mClutter;	///< RasterFileHandler to get the rasters containing the Clutter for the upcoming prediction Radials at different angels 
			cGeoP			mNorthWest;	///< North-Western corner of plot area
			cGeoP			mSouthWest;	///< South-Western corner of plot area
			cGeoP			mNorthEast;	///< North-Eastern corner of plot area
			cGeoP			mSouthEast;	///< South-Eastern corner of plot area
			double			mCentMer;	///< CentralMeridian of Output
			double			mPlotResolution;///< Resolution of the plot in meters
			double			mNSres;		///< North-South resolution in degrees
			double			mEWres;		///< West-East resolution in degrees
			double			mMinAngleRes;	///< Minimum angle resolution for individual coverage plots in degrees
			unsigned		mNumAngles;	///< Number of angles in 360 used individual coverage plots
			tMobile			mMobile;	///< Information on the mobile units
			unsigned		mNumFixed;	///< Number of fixed radion installations.
			vector<tFixed>		mFixedInsts;	///< Inforamtion on the fixed installations
			Float2DArray		mPlot;		///< Result of the plot
			Float2DArray		mSupportPlot;	///< secodary/supporting information e.g. signal strength incase of Prim Server
			unsigned		mRows;		///< Number of rows of the plot
			unsigned		mCols;		///< Number of colunms of the plot
			unsigned		mInstCounter; 	///< Used to display progres of big calculations ... 'installation M of N'
			vPred 			mActiveRasters;	///< Rasters containing individual plots (in rectangular coordinates) ... normally the signal strenth in dBm
			double			mCurrentEdge;	///< indicate how the plot progress through the area sourthern (or eastern) edge of the area already calculated  
			double			mMaxPathLoss;	///< Maximum pathloss that needs to be considers 
			double			mMaxRange;	///< Maximum range that will be considered
			string			mDir;		///< Directory of the output file
			string			mOutputFile;	///< Name of the output file
			MatrixXd mTheMatrix;	// matrix representing the set of linear equations to be solved
			MatrixXd mRight;
			MatrixXd mTrafficDens;
	};
}
#endif /*CPLOTTASK_H_*/
