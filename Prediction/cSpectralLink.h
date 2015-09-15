 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralLink.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow (magdaleen.ballot@up.ac.za)
 *    Description : Does the link for each potential interferer
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

#ifndef CSPECTRALLINK_H_
#define CSPECTRALLINK_H_

// Include local headers
#include "PredStructs.h"
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cProfile.h"
#include "cPthlssp.h"
#include "cAntpatn.h"
#include "cRasterFileHandler.h"

namespace Qrap
{

bool MakeArrayFromDB(vector<float> &result, string input);
string MakeDBFromArray(vector<float> &input);

class cSpectralLink
{
public:
	cSpectralLink(cRasterFileHandler *);
	cSpectralLink(cRasterFileHandler *, unsigned int &, unsigned int &, double &, double &);
	virtual ~cSpectralLink();
	
	//const cSpectralLink & operator=(const cSpectralLink &right);
	
	bool DoLink();
	
	double GetMinFreq() {return mEnvelopeFreq[0];}
	double GetMaxFreq() {return mEnvelopeFreq[mEnvelopeSize-1];}
	
	float GetEnvelopeValue(int i) {return mEnvelopeValue[i];}
	int GetEnvelopeSize() {return mEnvelopeSize;}
	bool CalcDistribution();
private:
	bool GetDBinfo(tFixed &Inst);
	eOutputUnits	mUnits;
	double mFrequencySpacing;	//Gaps between discreet frequencies which we examine
	float *mEnvelopeFreq;	//Frequency offsets for the envelope
	float *mEnvelopeValue;	//Relative gain at offsets
	int mEnvelopeSize;
	float mEnvelopeTotal;
	
	int mDescSize;
	vector<float> mDescOffset;
	vector<float> mDescValue;
		
	//These were copied from cLink on 25/03/2008 as a base
	bool			mDownlink;		///< Description
	double			mFrequency;		///<
	double			mkFactor;		///< Description
	double			mPlotResolution;///< Hz
	short int		mDEMsource;		///< Description
	short int		mClutterSource;	///< Description
	unsigned 		mClutterClassGroup;
	cClutter		mClutterset;
	cRasterFileHandler 	*mDEM;		///< Description
	bool			mUseClutter;	///< Description
	bool mUseAntennaANN; ///< 
	cRasterFileHandler 	mClutter;	///< Description
	tFixed			mTxInst;		///< Description
	tFixed			mRxInst;		///< Description
	cAntennaPattern 	mTxAnt;			///<
	cAntennaPattern 	mRxAnt;			///<
	int 			mLength;		///< Size of the arrays
	double 			mSlope;			///< Slope of LineOfSight
	double 			mEffRadius;		///< Effective earth radius
	double 			mMinClearance;	///< Minimum fresnel clearance
	double 			mTxTilt;		///< Recommended tilt on transmitting antenna
	double 			mRxTilt;		///< Recommended tilt on receiving antenna
	double 			mTxBearing;		///< Recommended bearing of transmitting antenna
	double 			mRxBearing;		///< Recommended bearing of receiving antenna
	double 			mTxElevation;	///< True height of Tx site
	double 			mRxElevation;	///< True height of Rx site
	float 			mPropLoss;		///< The propagation loss at the end point
	float 			mRxLev;		///< The propagation loss at each point along the profile
};

}

#endif /*CSPECTRALLINK_H_*/
