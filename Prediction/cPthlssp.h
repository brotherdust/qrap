 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cPthlssp.h
 *    Copyright   : (c) Meraka Institute CSIR (1997) & University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Core propagation prediction algorithm
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

#ifndef Qrap_cPthlssp_h
#define Qrap_cPthlssp_h

// Include global headers
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <float.h>

// Include local headers
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cProfile.h"
#include "cClutter.h"

// local defines
#define MAXPEAK 4

namespace Qrap
{
	/**
	 * This class encapsulates the core line loss prediction routines. It uses the profile and 
	 * some additional calculation parameters to determine the path loss at the end point of the profile.
	 * Assumes the trasmitter is at the start point and the receiver is at the end point.
	 * The profile is passed as a parameter and the other parameters is stored in a object of this class.
	 * For the refractive index of  the lower atmosphere alinear distribution is assumed allowing the use of the
	 * effective earth radius approach. Deygout's method is used for the multiple edge diffraction.
	 */
	class cPathLossPredictor : public cProfile
	{
	
	
		public:
			/**
			* Constructor
			* 
			* @param k k-factor that determines the effective earth radius
			* @param f is the frequency in MHz
			* @param txh is the transmitter height above local ground
			* @param rxh is the receiver height above local ground
			* @param UseClutter is a boolean variable indicating whether Clutter is 
			* to be used in the path loss predictions
			* @param ClutterClassGroup indicates the Clutter Classification Group 
			* of the Clutter files that is used in the predictions
			*/
			cPathLossPredictor (	double k=1.33,
					    	double f=400.0,
					    	double txh=18.0,
					    	double rxh=1.5,
						bool UseClutter=false,
						unsigned ClutterClassGroup=9999);
			
			
			/**
			 * Overloaded constructor
			 * 
			 * @param right Description
			 */
			cPathLossPredictor (const cPathLossPredictor &right);
			
			/**
			 * Destructor
			 */
			~cPathLossPredictor ();
			
			/**
			 * Main routine to calculate path loss
			 * 
			 * @param DTMProfile Description
			 * @param Tilt Description
			 * @param UseClutter Description
			 * @param ClutterProfile Description
			 *
			 * @return The total pathloss.
			 */
			float TotPathLoss (cProfile &DTMProfile, 
						float &Tilt, 
						cProfile &ClutterProfile,
						double &DiffLoss);
			
			/**
			 * Find the elevation angle in which the main ray is directed.
			 * 
			 * @param TXElevAngle Description
			 * @param RXElevAngle Description
			 */
			void FindElevAngles (float &TXElevAngle, 
					     float &RXElevAngle);
			
			/**
			 * Setting the parameters
			 * 
			* @param k k-factor that determines the effective earth radius
			* @param f is the frequency in MHz
			* @param TxHeight is the transmitter height above local ground
			* @param RxHeight is the receiver height above local ground
			* @param UseClutter is a boolean variable indicating whether Clutter is 
			* to be used in the path loss predictions
			* @param ClutterClassGroup indicates the Clutter Classification Group 
			* of the Clutter files that is used in the predictions
			* 
			* @return An integer
			*/
			int setParameters (double k, double f,
					  	double TxHeight, double RxHeight,
						bool &UseClutter,unsigned ClutterClassGroup);

			/**
			 * Description of set_kFactor
			 * 
			 * @param value Description
			 */

			void set_kFactor(const double value) {m_kFactor = value;}
			
			/**
			 * Description of set_freq
			 * 
			 * @param value Description
			 */
			void set_freq(const double value) {m_freq = value;}
			
			/**
			 * Description of set_htx
			 * 
			 * @param value Description
			 */
			void set_htx(const double value) {m_htx = value;}
			
			/**
			 * Description set_hrx
			 * 
			 * @param value Description
			 */
			void set_hrx(const double value) {m_hrx = value;}

			/**
			 * Description set_Seekwidth
			 * 
			 * @param value is the width in pixels that will be search for the 
			 * inflection point of a hill.
			 */
			void setSeekWidth(const int value) { m_SeekWidth = value;}

			/**
			 * Description set_Smoothwidth
			 * 
			 * @param value is the number of points left and right that will be included  
			 * in local smooting of the terrain profile. 
			 */
			void setSmoothWidth(const int value) { m_SmoothWidth = value;}
			
			/**
			 * Description get_Clutter
			 * 
			* The function returns the ClutterType of the MainPeak
			* it is to be called directly after TotPathLoss if it is required
			 */
			int get_Clutter() { return mClutterIndex; } 

			/**
			 * Description set_Clutter
			 * 
			* @param UseClutter is a boolean variable indicating whether Clutter is 
			* to be used in the path loss predictions
			* @param ClutterClassGroup indicates the Clutter Classification Group 
			* of the Clutter files that is used in the predictions
			 */
			void set_Clutter(bool &UseClutter, unsigned ClutterClassGroup=9999); 

			void set_Tuning( bool Tuning) { mTuning=Tuning;}

			/**
			 * Overloaded operator
			 */
			const cPathLossPredictor & operator=(const cPathLossPredictor &right);
		
		private:
			/**
			 * Calculates the difference between the begin and end points, taking the height difference into account. Result is in metres.
			 * 
			 * @param InputProfile Description
			 *
			 * @return A double
			 */
			inline double CalcDist (cProfile &InputProfile);
			
			/**
			 * Calculate the free space loss, which is dependent of the profile length and the frequency but independent of the actual profileheights. Takes pathLength in metres.
			 * 
			 * @param pathLength Description
			 *
			 * @return A double.
			 */
			inline double CalcFreeSpaceLoss (const double pathLength);
			

			inline double CalcPlaneEarthLoss(const double pathLength);


			/**
			 * Initializes the object of type CPathLossPredictor, setting the effective profile, and the other members.
			 */
			void InitEffectEarth (const cProfile &InputProfile, const cProfile &ClutterProfile);
			

			/**
			 * "Horizontilize" the original profile between Begin and End and store it in TempProfile. Return reference height.
			 * 
			 * @param BeginMarkerIndex Description
			 * @param EndMarkerIndex Description
			 */
			double Horizontalize(const int BeginMarkerIndex, const int EndMarkerIndex);
			
			/**
			 * Description of FindMainPeak
			 * 
			 * @param BeginMarkerIndex Description
			 * @param EndMarkerIndex Description
			 * @param ReffHeigh The reference height (the height at Begin).
			 * @param Clearance Description
			 * @param SQd1d2 SQd1d2 is sqrt( d1*d2/(d1+d2)) Reference: Parsons p.39 .
			 *
			 * @return Return the index of the main peak between markers[BeginMarkerIndex] and markers[EndMarkerIndex]
			 */
			int FindMainPeak(const int BeginMarkerIndex,
					 const int EndMarkerIndex,
					 const double ReffHeight,
					 double &Clearance,
					 double &SQd1d2);
			
			/**
			 * Indentify the peaks of a profile and calculate and set the radii of the peaks. These radii is needed to account for the additional diffraction losses due the roundness of the hills.
			 * 
			 * @param PeakIndex Description
			 * 
			 * @return A double
			 */
			double SetPeakRadius(int PeakIndex, double &alpha);
			
			/**
			 *  Calculates the diffraction losses.
			 * 
			 * @param BeginIndex Description
			 * @param EndIndex Description
			 * @param PeakIndex Description
			 * @param ReffHeight Description
			 * @param SQd1d2 Description
			 * @param radius Description
			 * @param TooManyPeaks Description
			 * @param KnifeEdge Description
			 * @param RoundHill Description
			 * @param GroundPermittivity Description
			 * @param GroundConductivity Description
			 * @param Vertical Description
			 *
			 * @return A double
			 */
			double CalcDiffLoss(const int BeginIndex, const int EndIndex,
						int PeakIndex, const double ReffHeight,
						const double SQd1d2, double radius, double alpha,
						bool MainEdge,
						int &TooManyPeaks, double &KnifeEdge, double & RoundHill, 
						const double GroundPermittivity = 12.0,
						const double GroundConductivity = 0.0005,
						const int Vertical = 1);
			
			/**
			 * Used by CalcDiffLoss
			 * 
			 * @param Y Description
			 * @param K Description
			 *
			 * @return A double
			 */
			inline double HeightGain(const double Y, const double K);
		
		private:
		
			static double m_c;		///< Speed of Light in m/s.
			static double m_reR;		///< real earth Radius in meter.
			double m_Loss;			///< The path loss in dB.
			double m_kFactor;		///< The k_factor used to calculate the effect earth curviture.
			double mLinkLength;		///< Distance between transmitter and receiver.
			double m_freq;			///< The operating frequency in MHz.
			double m_htx;			///< The height of the transmitter antenna in meter.
			double m_hrx;			///< The height of the receiver antenna in meter.
			float *m_TempProfile;		///< Temporary (horizontilized profile).
			float *m_CurvedProfile;		///< Storage for original profile.
			int *mClutterProfile;
			double m_tempIPD;		///< Temporary InterPixelDist used to "flatten" the profile in meter
			double m_slope;			///< (temporary) slope of the profile.
			double mTuning;
			int *m_markers;			///< Indexes of markers
			int *m_peakwidth;		///< number of pixel from peak not to seek the next peak.
			double *m_aboveEarth;		///< array to store the mast heights and the height of the line of sigth above the earth at each marker.
//			int m_counter;			///< for debuging purpses
			int m_SmoothWidth;		///< Used to Smooth the profile when calculating the radii of the peaks
			int m_SeekWidth;		///< Use to seek the inflection points when calculating the peak radii
			bool mUseClutter;		///< indicates whether clutter dependancies should be included. 
			double *mCterms;		///< values of the clutter terms
			int mClutterIndex;		///< index to the clutter type where the profile has least clearance
			unsigned mCalcMarker;
		public:
			cClutter mClutter;		///< 'container' class that has the coefficients and heights to be used in the pathloss calculations. It must be public for the optimiser to see it. 
	};
}

#endif

