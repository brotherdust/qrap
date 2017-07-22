 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : PredStruct.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Defines classes and enums use in Predictions
 *                : Particularly radio instalations
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

#include "cGeoP.h"
#include "cMemmngr.h"
#include <vector>

#ifndef Qrap_PredStructs_h
#define Qrap_PredStructs_h
using namespace std; 

namespace Qrap
{
	/**
	 * Structure tFixed:
	 * This structure is used to present the chatracteristics of the 
	 * Fixed radio-installations in the during the calculation of 
	 * coverage and interference plots.  
	 */
	struct tFixed
	{
		int	sInstKey;	///< Primary key of Radio installation
		int 	sSiteID;	///< Primary key of site where it is installed
		int	sTechKey;	///< Primary key of the technology
		string  sSiteName;	///< Name of site
		cGeoP	sSitePos;	///< Position of installation
		double	sFrequency;	///< Frequency at which prediction will be done
		double 	sEIRP;		///< Effective Isotropic power in dBm
		double 	sLayerThres;	///< Layer Threshold in dBm
		double 	sCStraffic;	///< Circuit switched traffic in Erlang
		double 	sPStraffic;	///< Packet switched traffic in Megabytes/s
		double	sTxPower;	///< Transmitter power in ???
		double	sTxSysLoss;	///< Transmitter system losses in dB
		double	sRxSysLoss;	///< Receiver system losses in dB
		double	sRxSens;	///< Receiver sensitivity in dBm
		int	sTxPatternKey;	///< Primary key of transmitter antenna pattern
		double	sTxAzimuth;	///< Horizontal direction of main beam of transmitter
		double	sTxMechTilt;	///< Mechanical (down+) tilt of main transmitting antenna
		int	sRxPatternKey;	///< Primary key of receiver antenna pattern
		double	sRxAzimuth;	///< Horizontal direction of main beam of receiving antenna
		double	sRxMechTilt;	///< Mechanical (down+) tilt of main receiving antenna
		double	sDiversityGain;	///< The amount of diversity gain in dB
		double	sTxHeight;	///< Transmiter antenna height above local ground
		double	sRxHeight;	///< Receiver antenna height above local ground
		double  sRange;		///< The range for which the prediction must be carried out
		double	sFEdge;		///< The "front-edge" of the forseen coverage plot in degrees North (or West)
		double	sBEdge; 	///< The "back-edge" of the forseen coverage plot in degrees North (or West)
		double 	sCentroidX;	///< Location of centroid with respect to Left edge of plot in pixels
		double 	sCentroidY;	///< Location of centroid with respect to Top edge of plot in pixels
		int	sPixelCount;	///< Number of pixels where installation is primary server
		cGeoP	sCentroid;	///< Centroid of the coverage area of the installation
		vector<double> sFreqList;	///< List of actual center frequencies in MHz		
		double 	sBandWidth;	///< Bandwidth of the channels in MHz
	};

	/**
	* Description of CopyFixed:
	* This procedure copy the Right tFixed Installation info into the Left
	*/
	//************************************************************************
	inline void CopyFixed(tFixed &Left, tFixed Right)
	{
		Left.sInstKey		= Right.sInstKey;
		Left.sSiteID		= Right.sSiteID;
		Left.sSitePos		= Right.sSitePos;
		Left.sSiteName		= Right.sSiteName;
		Left.sFrequency		= Right.sFrequency;
		Left.sEIRP		= Right.sEIRP;
		Left.sLayerThres	= Right.sLayerThres;
		Left.sCStraffic		= Right.sCStraffic;
		Left.sPStraffic		= Left.sPStraffic;
		Left.sTxPower		= Right.sTxPower;
		Left.sTxSysLoss		= Right.sTxSysLoss;
		Left.sRxSysLoss		= Right.sRxSysLoss;
		Left.sRxSens		= Right.sRxSens;
		Left.sTxPatternKey	= Right.sTxPatternKey;
		Left.sTxAzimuth		= Right.sTxAzimuth;
		Left.sTxMechTilt	= Right.sTxMechTilt;
		Left.sRxPatternKey	= Right.sRxPatternKey;
		Left.sRxAzimuth		= Right.sRxAzimuth;
		Left.sRxMechTilt	= Right.sRxMechTilt;
		Left.sDiversityGain	= Right.sDiversityGain;
		Left.sTxHeight		= Right.sTxHeight;
		Left.sRxHeight		= Right.sRxHeight;
		Left.sRange		= Right.sRange;
		Left.sFEdge		= Right.sFEdge;
		Left.sBEdge		= Right.sBEdge;
		Left.sCentroid		= Right.sCentroid;
		Left.sCentroidX		= Right.sCentroidX;
		Left.sCentroidY		= Right.sCentroidY;
		Left.sPixelCount	= Right.sPixelCount;
		Left.sBandWidth 	= Right.sBandWidth;
		Left.sFreqList.clear();
		for (unsigned i=0; i<Right.sFreqList.size(); i++)
			Left.sFreqList.push_back(Right.sFreqList[i]);
		
	}
	
	/**
	 * Structure tMobile:
	 * This is used to present the mobile units characteristics during the calculation 
	 * of Coverage and Interference plots 
	 */
	struct tMobile	
	{
		int	sInstKey;		///< Primary k=Key of Mobile unit
		double	sEIRP;			///< Effective Isotropic Radiated Power in dBm
		double	sTxPower;		///< Transmitted Power in ???
		double	sTxSysLoss;		///< Transmitter system losses in dB
		double	sRxSysLoss;		///< Reciever system losses in dB
		double	sRxSens;		///< Receiver sensitivity in dBm
		int	sPatternKey;		///< Primary key to Antenna Pattern on mobile
		double	sMobileHeight;		///< Assumed height of mobile unit
		double  sFrequency;		///< Assumed transmitter frequency of mobile.
	};	
	
	/**
	 * Structure tSignalRaster:
	 * This structure stores information relavant to 
	 * This is seperate from the rest of the installation installation information
	 * because one needs to discard of it as soon as the raster is no longer required 
	 */
	struct tSignalRaster	
	{
		int	sInstKey;	///< Primary key of the radio installation
		int	sLayerThres;	///< Threshold level of installation
		int	sTop;		///< northern most boundary of plot in decimal degrees
		int 	sLeft;		///< western most boundary of plot in decimal degrees
		int	sNSsize;	///< Number of rows in the raster
		int	sEWsize;	///< Number of columns in the raster
		double  sBandWidth; ///< Bandwidth of channels in MHz
		vector<double> sFreqList; /// < Frequencies allocated to radio installation
		Float2DArray	sRaster;	///< stores Signal strength in rectagular coordinates 
	};
	
	/**
	* enum eOutputUnits:
	* The possible units in which the output could be displayed.
	*/
	enum eOutputUnits
	{
		dB,		///< Description
		dBW,		///< Description
		dBm,		///< Description
		dBuV,		///< Description
		dBuVm,		///< Description
		dBWm2Hz,	///< Description	
		dBWm2		///< Description
	};

	
}
#endif
