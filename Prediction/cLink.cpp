 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLink.cpp
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

#include "cLink.h"

using namespace Qrap;
using namespace std;

double cLink::reR = 6367444;    	///< real earth Radius in m
double cLink::c = 2.997924562e8;  	///< Speed of Light in m/s

//************************************************************************
cLink::cLink()
{
	mLength	= 2;
	mSlope 	= 0.0;
	mEffRadius 	= reR;
	mMinClearance  = 3.402823e+38;
	mClutterClassGroup = 0;
	mRxLev 		= new float[2];
    	mFlatProfile 	= new float[2];
	mEffProfile  	= new float[2];
	mLineOfSight 	= new float[2];
	mFresnelH    	= new float[2];
	mFresnelL    	= new float[2];
	mFresnelSH   	= new float[2];
	mFresnelSL   	= new float[2];
	mClearance   	= new float[2];
	mPropLoss    	= new float[2];
	mTrueHeightGrid1 = new float[2];
	mTrueHeightGrid2 = new float[2];
	mTrueHeightGrid3 = new float[2];
	mClutterProfile = new int[2];
	mUnits = dBm;
	mFrequency = 2400;
	mkFactor = 1;
	mDEMsource = 1;
	mClutterSource = 1;
	mUseClutter = false;
	mPlotResolution = 90;
	mTxInst.sInstKey=1;
	mTxInst.sSiteID=1;
	mTxInst.sSitePos.Set(-25,25,DEG);
	mTxInst.sFrequency=2400;
	mTxInst.sEIRP=33;
	mTxInst.sTxPower=2;	
	mTxInst.sTxSysLoss=0;
	mTxInst.sRxSysLoss=0;
	mTxInst.sRxSens=-105;
	mTxInst.sTxPatternKey=1;	
	mTxInst.sTxAzimuth=0;		
	mTxInst.sTxMechTilt=0;	
	mTxInst.sRxPatternKey=1;
	mTxInst.sRxAzimuth=0;
	mTxInst.sRxMechTilt=0;
	mTxInst.sDiversityGain=0;
	mTxInst.sTxHeight=10;
	mTxInst.sRxHeight=10;
	mTxInst.sRange=0;
	mTxInst.sFEdge=0;
	mRxInst.sBEdge=0;
	mRxInst.sInstKey=1;
	mRxInst.sSiteID=1;
	mRxInst.sSitePos.Set(-25,25.5,DEG);
	mRxInst.sFrequency=2400;
	mRxInst.sEIRP=33;
	mRxInst.sTxPower=2;	
	mRxInst.sTxSysLoss=0;
	mRxInst.sRxSysLoss=0;
	mRxInst.sRxSens=-105;
	mRxInst.sTxPatternKey=1;	
	mRxInst.sTxAzimuth=0;		
	mRxInst.sTxMechTilt=0;	
	mRxInst.sRxPatternKey=1;
	mRxInst.sRxAzimuth=0;
	mRxInst.sRxMechTilt=0;
	mRxInst.sDiversityGain=0;
	mRxInst.sTxHeight=10;
	mRxInst.sRxHeight=10;
	mRxInst.sRange=0;
	mRxInst.sFEdge=0;
	mRxInst.sBEdge=0;
}

//**************************************************************************
cLink::~cLink()
{
	delete [] mRxLev;
	delete [] mFlatProfile;
	delete [] mEffProfile;
	delete [] mLineOfSight;
	delete [] mFresnelH;
	delete [] mFresnelL;
	delete [] mFresnelSH;
	delete [] mFresnelSL;
	delete [] mClearance;
	delete [] mPropLoss;
	delete [] mTrueHeightGrid1;
	delete [] mTrueHeightGrid2;
	delete [] mTrueHeightGrid3;
	delete [] mClutterProfile;
}

//***************************************************************************
// Assignment Operator
const cLink & cLink::operator=(const cLink &right)
{
	int i;

	if (this == &right)
		return (*this);

	delete [] mRxLev;
    	delete [] mFlatProfile;
	delete [] mEffProfile;
	delete [] mLineOfSight;
	delete [] mFresnelH;
	delete [] mFresnelL;
	delete [] mFresnelSH;
	delete [] mFresnelSL;
	delete [] mClearance;
	delete [] mPropLoss;
	delete [] mTrueHeightGrid1;
	delete [] mTrueHeightGrid2;
	delete [] mTrueHeightGrid3;
	delete [] mClutterProfile;

	mClutterset		= right.mClutterset;
	mLength			= right.mLength;
	mSlope 			= right.mSlope;
	mPlotResolution	= right.mPlotResolution;
	mEffRadius 		= right.mEffRadius;
    	mMinClearance  	= right.mMinClearance;
	mFrequency		= right.mFrequency;
	mTxTilt			= right.mTxTilt;
	mRxTilt			= right.mRxTilt;
	mTxBearing		= right.mTxBearing;
	mRxBearing		= right.mRxBearing;
	mTxElevation	= right.mTxElevation;
	mRxElevation	= right.mRxElevation;
	mUnits			= right.mUnits;				///< Description
	mDownlink		= right.mDownlink;			///< Description
	mkFactor		= right.mkFactor;			///< Description
	mPlotResolution	= right.mPlotResolution;	///< m
	mDEMsource		= right.mDEMsource;			///< Description
	mClutterSource	= right.mClutterSource;		///< Description
	// \TODO: Does mRasterfileHandler have an assignment overload ... implement
	mDEM			= right.mDEM;				///< Description
	mUseClutter		= right.mUseClutter;		///< Description
	mClutter		= right.mClutter;			///< Description
	mClutterClassGroup 	= right.mClutterClassGroup;
	CopyFixed(mTxInst,right.mTxInst);
	CopyFixed(mRxInst,right.mRxInst);		///< Description
    	mRxLev	 = new float[mLength];
	mFlatProfile = new float[mLength];
	mEffProfile  = new float[mLength];
	mLineOfSight = new float[mLength];
	mFresnelH    = new float[mLength];
	mFresnelL    = new float[mLength];
	mFresnelSH   = new float[mLength];
	mFresnelSL   = new float[mLength];
	mClearance   = new float[mLength];
	mPropLoss    = new float[mLength];
	mTrueHeightGrid1 = new float[mLength];
	mTrueHeightGrid2 = new float[mLength];
	mTrueHeightGrid3 = new float[mLength];
	mClutterProfile = new int[mLength];

	for(i=0;i<mLength;i++)
	{
		*(mRxLev+i) 		= *(right.mRxLev+i);
       		*(mFlatProfile+i) 	= *(right.mFlatProfile+i);
		*(mEffProfile+i)  	= *(right.mEffProfile+i);
		*(mLineOfSight+i) 	= *(right.mLineOfSight+i);
		*(mFresnelH+i)    	= *(right.mFresnelH+i);
		*(mFresnelL+i)    	= *(right.mFresnelL+i);
		*(mFresnelSH+i)   	= *(right.mFresnelSH+i);
		*(mFresnelSL+i)   	= *(right.mFresnelSL+i);
		*(mClearance+i)   	= *(right.mClearance+i);
		*(mPropLoss+i)    	= *(right.mPropLoss+i);
		*(mTrueHeightGrid1) = *(right.mTrueHeightGrid1+i);
		*(mTrueHeightGrid2) = *(right.mTrueHeightGrid2+i);
		*(mTrueHeightGrid3) = *(right.mTrueHeightGrid3+i);
		*(mClutterProfile) = *(right.mClutterProfile+i);
	}
	return (*this);
}/* cLink Assignment */

//******************************************************/
void cLink::SetLink(eOutputUnits	Units,
			bool		Downlink,
			double		Frequency,
			double		kFactor,
			double		PlotResolution,
			short int 	DEMsource,
			short int	ClutterSource,
			bool 		&UseClutter,
			int		TxInst, //This will be site IDs in the case of trial Links
			int 		RxInst, //This will be site IDs in the case of trial Links
			bool		Trial)
{
	mTrial = Trial;
	mUnits = Units;
	mDownlink =Downlink;
	mFrequency = Frequency;
	mkFactor = kFactor;
	mPlotResolution = PlotResolution;
	mDEMsource = DEMsource;
	mClutterSource = ClutterSource;
	mDEM.SetRasterFileRules(mDEMsource);
	mUseClutter = UseClutter;
	if (mUseClutter)
		mUseClutter = mClutter.SetRasterFileRules(mClutterSource);
	else
		mClutterSource = 1;
	if (mUseClutter) 
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	if (mUseClutter) mClutterset.Reset(mClutterClassGroup);
	UseClutter = mUseClutter;

	if (mTrial)
	{
		mTxInst.sSiteID = TxInst;
		mRxInst.sSiteID = RxInst;
	}
	else
	{
		mTxInst.sInstKey = TxInst;
		mRxInst.sInstKey = RxInst; 
	}
	
	//DEBUGGING!!!!!!!!!!!!!!
	bool WriteData = false;
	if (WriteData)
	{
		FILE *fp = fopen("/home/maggie/ExampleLinkRequest","w");
		if (fp != NULL)
		{
			string temp = "dBm";
//			switch (Units)
//			{
//				
//			}
			fprintf(fp,"%s\n",temp.c_str());
			fprintf(fp,"Freq %f\n",Frequency);
			fprintf(fp,"kFactor %f\n",kFactor);
			fprintf(fp,"Resolution %f\n",PlotResolution);
			fprintf(fp,"Tx %d\n",TxInst);
			fprintf(fp,"Rx %d\n",RxInst);
			fprintf(fp,"DEM %d\n",DEMsource);
			fclose(fp);
		}
	}
}

//*******************************************************************************
bool cLink::SetTxSite(int TxInst, tFixed Inst)
{ 
	mTxInst = Inst;
	mTxInst.sSiteID = TxInst;
	return GetDBinfoS(mTxInst);
}

//*********************************************************************************
bool cLink::SetRxSite(int RxInst, tFixed Inst)
{ 
	mRxInst = Inst;
	mRxInst.sSiteID = RxInst;
	return GetDBinfoS(mRxInst);
}

//***************************************************************************
bool cLink::DoLink(bool Trial, double MaxDist)
{
	bool DBworked = true;
	if (!Trial) // else the information should be already loaded via SetTxSite, StRxSite and GetDBinfoS
	{
		DBworked = GetDBinfo(mTxInst);
		DBworked = (DBworked)&&(GetDBinfo(mRxInst));
		if (!DBworked)
		{
			cout << "cLink::DoLink():  Database query for radioinstallations in Link failed." << endl;
			return DBworked;
		}
	}

	double Dist = mTxInst.sSitePos.Distance(mRxInst.sSitePos);
	mDist = Dist;
	if (Dist>MaxDist)
		return false;
	else if ((int)(Dist/mPlotResolution)<2)
		return false;

	cProfile DEM;
	unsigned Length;
	mDEM.GetForLink(mTxInst.sSitePos,mRxInst.sSitePos,mPlotResolution,DEM);
	Length = DEM.GetSize();
	mLength = Length;
	
	if (Length < 2)
		return false;

	unsigned j;
	
	double LinkOtherGain, TxAntValue=0.0;
//	double RxAntValue=0.0;

	bool AfterReceiver = (mUnits!=dBWm2Hz) && (mUnits!=dBWm2);	

	eAnt Which;
	if (mDownlink) Which =Tx;
	else Which = Rx;
	mTxAnt.SetAntennaPattern(mTxInst.sInstKey, Which, mTxInst.sTxAzimuth,  mTxInst.sTxMechTilt);
	if (mDownlink) Which =Rx;
	else Which = Tx;
	mRxAnt.SetAntennaPattern(mRxInst.sInstKey, Which, mRxInst.sRxAzimuth,  mRxInst.sRxMechTilt);
	if (mDownlink) EIRP = mTxInst.sTxPower - mTxInst.sTxSysLoss + mTxAnt.mGain;
	else mRxInst.sTxPower - mRxInst.sTxSysLoss + mRxAnt.mGain;

	AfterReceiver = (mUnits==dBW)||(mUnits==dBm)||(mUnits==dBuV);
	if (AfterReceiver)
	{
		if (mDownlink)
			LinkOtherGain = EIRP -  mRxInst.sRxSysLoss + mRxAnt.mGain;
		else
			LinkOtherGain = EIRP - mTxInst.sRxSysLoss + mTxAnt.mGain;
	}
	else LinkOtherGain = EIRP;
	
	float *Tilt;
	Tilt = new float[Length];
	delete [] mRxLev;
	mRxLev = new float[Length];
	delete [] mPropLoss;
	mPropLoss = new float[Length];
	double DiffLoss=0;
	for (j=0; j<Length; j++)
	{
		Tilt[j]=0;
		mRxLev[j]=0;
		mPropLoss[j]=0;
	}

	cProfile Clutter;
	if (mUseClutter)
		mClutter.GetForLink(mTxInst.sSitePos,mRxInst.sSitePos,mPlotResolution, Clutter);

	cout << "cLink::DoLink( ... ) before Initialize" << endl; 
	Initialize(DEM,Clutter);
	SetEffProfile();	//Calculates the effective profile
	SetLineOfSight();
	SetFresnelClear();
	
	cout << "cLink::DoLink( ... ) before PathLoss.setParameters" << endl; 	
	PathLoss.setParameters(mkFactor,mFrequency,mTxInst.sTxHeight,mRxInst.sRxHeight,
				mUseClutter,mClutterClassGroup);
	cout << "cLink::DoLink( ... ) After PathLoss.setParameters" << endl;
	if (Length>0)	
		j=(Length-1);
	else
	{
		cout << "In cLink::DoLink Link has 0 lenght" << endl;
		return false;
	}
	mPropLoss[j] = PathLoss.TotPathLoss(DEM,Tilt[j],Clutter,DiffLoss);
	mTxBearing = mTxInst.sSitePos.Bearing(mRxInst.sSitePos);
	if (mTxBearing < 180.0)
		mRxBearing = mTxBearing + 180.0;
	else	
		mRxBearing = mTxBearing - 180.0;
	PathLoss.FindElevAngles(mTxTilt,mRxTilt);
	cout << "cLink::DoLink( ... ) before mPropLoss loop" << endl;
	for (j=(Length-1); j>0 ; j--)
	{
		mPropLoss[j] = PathLoss.TotPathLoss(DEM,Tilt[j],Clutter,DiffLoss);
		TxAntValue = mTxAnt.GetPatternValue(mTxBearing, Tilt[j])
					+ mRxAnt.GetPatternValue(mRxBearing, -Tilt[j]);
		mRxLev[j] = -mPropLoss[j] + LinkOtherGain - TxAntValue;
		if (mUseClutter) Clutter.ReduceSize();
		DEM.ReduceSize();
	}
	cout << endl;
	mRxLev[0]=mRxLev[1];

	mPathLossEnd = mPropLoss[mLength-1];
	mRxLevEnd = mRxLev[mLength-1];
//	cout << "cLink::DoLink( ... ) before delete [] Tilt" << endl;
	delete [] Tilt;
//	cout << "cLink::DoLink( ... ) exiting" << endl;
	return true;
}

//***************************************************************************
// Initializes all the arrays: Put real profile in EffProfile and zero the
// rest.
void cLink::Initialize(const cProfile &DEMProfile,const cProfile &ClutterProfile)
{
	unsigned i;
	DEMProfile.GetProfile(mLength,mFlatProfile);
/*	for (int j=0;j<mLength;j++)
		cout << mFlatProfile[j] << " ";
	cout << endl;
*/	
	mPlotResolution = DEMProfile.GetInterPixelDist();
	mTxElevation = mFlatProfile[0];
	mRxElevation = mFlatProfile[mLength-1];

//	cout << "In cLink::Initialize(...) before if mUseClutter" << endl;
	if (mUseClutter)
	{
		delete [] mClearance;		//Just temporary (mis)using mClearance
		mClearance = new float[mLength];
		delete [] mClutterProfile;
		mClutterProfile = new int[mLength];
		ClutterProfile.GetProfile(mLength,mClearance);
		for (i=0; i<mLength; i++)
			mClutterProfile[i] = (int)mClearance[i];
		for (i=0; i<mLength; i++)
			mFlatProfile[i] += mClutterset.mClutterTypes[mClutterProfile[i]].sHeight;
	}

//	cout << "In cLink::Initialize(...) before delete []'s" << endl;
	delete [] mRxLev;
	delete [] mPropLoss;
    	delete [] mEffProfile;
	delete [] mLineOfSight;
	delete [] mFresnelH;
	delete [] mFresnelL;
	delete [] mFresnelSH;
	delete [] mFresnelSL;
	delete [] mClearance;
	delete [] mTrueHeightGrid1;
	delete [] mTrueHeightGrid2;
	delete [] mTrueHeightGrid3;

//	cout << "In cLink::Initialize(...) before delete []'s" << endl;
	mPropLoss    	= new float[mLength];
	mRxLev    	= new float[mLength];
	mEffProfile  	= new float[mLength];
	mLineOfSight 	= new float[mLength];
	mFresnelH    	= new float[mLength];
	mFresnelL    	= new float[mLength];
	mFresnelSH   	= new float[mLength];
	mFresnelSL   	= new float[mLength];
	mClearance   	= new float[mLength];
	mTrueHeightGrid1	= new float[mLength];
	mTrueHeightGrid2	= new float[mLength];
	mTrueHeightGrid3	= new float[mLength];

	for(i=0;i<mLength;i++)
	{
		mPropLoss[i]   = 0.0;
		mRxLev[i]   	= 0.0;
        	mEffProfile[i] = mFlatProfile[i];
		mLineOfSight[i]= 0.0;
		mFresnelH[i]   = 0.0;
		mFresnelL[i]   = 0.0;
		mFresnelSH[i]  = 0.0;
		mFresnelSL[i]  = 0.0;
		mClearance[i]  = 0.0;
		mTrueHeightGrid1[i]	= 0.0;
		mTrueHeightGrid2[i]	= 0.0;
		mTrueHeightGrid3[i]	= 0.0;
	}
//	cout << "In cLink::Initialize(...) exiting" << endl;
}/* end CLink::Initialize */


//**************************************************************************
// Initialize and calculate the effective earth profile, taking the kFactor
// into account.
void cLink::SetEffProfile()
{
	int i;
	double centre;
	double offset;
	double Reff2;

	mEffRadius = mkFactor * reR;

	centre = ((double)mLength)/2.0;
	Reff2 = 2.0 * mEffRadius;		// Reference:
	for(i=0;i<mLength;i++)			// Radio Wave Propagation
	{                                      	// A.Picquenard, p.17
		offset = (centre-i-1) * mPlotResolution;
		*(mEffProfile+i) -= (float)(offset*offset/(Reff2));
	}
}/* end cLink::SetEffProfile */

//***************************************************************************
void cLink::SetLineOfSight()
{
	int i;
	double TXheight;

	TXheight = (double)*(mEffProfile) + mTxInst.sTxHeight;
	mSlope = (((double)*(mEffProfile+mLength-1)+mRxInst.sRxHeight)-TXheight)
               /(double)(mLength-1);
	for (i=0; i < mLength; i++)
		*(mLineOfSight+i) = (float)(TXheight + mSlope*(double)i);

	mSlope /= (double)mPlotResolution;

}/* end SetLineOfSight */


//***************************************************************************
// Set all the Fresnel-Zones: 100% and 60%; top and bottom
// This procedure take account of the possibility that the link might be at
// a steep verticle angle
// Reference: D.Parsons, The Mobile Radio Propagation Channel, Section 3.3.1
void cLink::SetFresnelClear()
{
	int i;
	double wavelength;	// in meter
	double angle;		// angle of LineOfSight relative to the horizon
					// positive upwards
	double sinA;		// sinus of 'angle'
	double cosA;		// cosinus of 'angle'
	double FRadius=1;		// Fresnel radius
	double delta;		// temporary value
	double temp=0;		// temporary value

	wavelength = c/(mFrequency*1000000.0);
	angle = atan(mSlope);
	sinA = sin(angle);
	cosA = cos(angle);

	mMinClearance = MAXDOUBLE;
	for (i=0; i<mLength; i++)
	{
		temp = wavelength*i*(mLength-1-i)*mPlotResolution/(cosA*(mLength-1));
		if (temp<0.0) temp = 0.0;
		FRadius = sqrt(temp);
		temp = FRadius*sinA/mPlotResolution;

		mFresnelH[i+(int)( temp+0.5)]=mLineOfSight[i]+(float)(FRadius*cosA);
		mFresnelL[i+(int)(-temp+0.5)]=mLineOfSight[i]-(float)(FRadius*cosA);
		mFresnelSH[i+(int)(0.6*temp+0.5)]=mLineOfSight[i]+(float)(0.6*FRadius*cosA);
		mFresnelSL[i+(int)(-0.6*temp+0.5)]=mLineOfSight[i]-(float)(0.6*FRadius*cosA);

		if (mFresnelH[i]==.0) mFresnelH[i]=mLineOfSight[i]+(float)(FRadius*cosA);
		if (mFresnelL[i]==.0) mFresnelL[i] = mLineOfSight[i]-(float)(FRadius*cosA);
		if (mFresnelSH[i]==.0) mFresnelSH[i]=mLineOfSight[i]+(float)(0.6*FRadius*cosA);
		if (mFresnelSL[i]==.0) mFresnelSL[i]=mLineOfSight[i]-(float)(0.6*FRadius*cosA);

		// To calculate the clearance
		delta = (double)(mLineOfSight[i] - mEffProfile[i])*cosA;
		if (FRadius>0.0)
			mClearance[i] = (float)(delta/FRadius);
		else
			mClearance[i] = MAXDOUBLE;
		
		if (mMinClearance > mClearance[i])
			mMinClearance= mClearance[i];

	}/* end for i*/
}/* end CLink::SetFresnel */

//**************************************************************************
//
void cLink::GetLinkInfo(int &Size, double &InterPixDist,
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
			double &maxH, double &minH)
{

	int i;
	Size 		= mLength;
	InterPixDist	= mPlotResolution;
	Freq		= mFrequency;
	TxElevation 	= mTxElevation;
	RxElevation 	= mRxElevation;
	TxTilt		= mTxTilt;
	RxTilt		= mRxTilt;
	TxBearing	= mTxBearing;
    	RxBearing	= mRxBearing;
	if (mDownlink)
	{
	    	TxRealTilt	= mTxInst.sTxMechTilt;
		RxRealTilt	= mRxInst.sRxMechTilt;
		TxRealBearing	= mTxInst.sTxAzimuth;
    		RxRealBearing	= mRxInst.sRxAzimuth;
		TxAntHeight	= mTxInst.sTxHeight;
    		RxAntHeight	= mRxInst.sRxHeight;
	}
	else
	{
	    	TxRealTilt	= mTxInst.sRxMechTilt;
		RxRealTilt	= mRxInst.sTxMechTilt;
		TxRealBearing	= mTxInst.sRxAzimuth;
    		RxRealBearing	= mRxInst.sTxAzimuth;
		TxAntHeight	= mTxInst.sRxHeight;
    		RxAntHeight	= mRxInst.sTxHeight;
	}
	mTxInst.sSitePos.Get(TxLat,TxLon);
	mRxInst.sSitePos.Get(RxLat,RxLon);
	
	EndLoss = mPropLoss[mLength-1];
	MinClearance = mMinClearance;
	RxLev = mRxLev[mLength-1];
	mMinimum = 8880;
	mMaximum = -440;

	for (i=0;i<mLength;i++)
		if ((mEffProfile[i] < mMinimum)&&(mEffProfile[i]!=0))	mMinimum = mEffProfile[i];
		else if (mEffProfile[i] > mMaximum) mMaximum = mEffProfile[i];
	for (i=1;i<mLength-1;i++)
		if (mFresnelH[i] > mMaximum) mMaximum = mFresnelH[i];
	for (i=1;i<mLength-1;i++)
    		if (mFresnelL[i] < mMinimum) 	mMinimum = mFresnelL[i];

	int bottom;
	int top;
	double centre;
	double offset;
	double R2;
	int grid1;
	int grid2;
	int grid3;
	double EffRadius=mEffRadius;
  
	centre = ((double)mLength)/2.0;
	bottom = (int)(floor(mMinimum/50.0)*50.0);
	top 	= (int)(ceil(mMaximum/50.0)*50.0);

	R2 = EffRadius*EffRadius;
	grid1  = (int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid1[i] = bottom + grid1 - EffRadius+ sqrt(R2 - offset*offset);
	}

	EffRadius += grid1;
	R2 = EffRadius*EffRadius;
	grid2  = 2* (int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid2[i] = bottom + grid2 - EffRadius+ sqrt(R2 - offset*offset);
	}

	EffRadius += grid2 - grid1;
	R2 = EffRadius*EffRadius;
	grid3  = 3*(int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid3[i] = bottom + grid3 - EffRadius+ sqrt(R2 - offset*offset);
	}
	maxH = mMaximum;
	minH = mMinimum;

}/* end GetLinkInfo */


//***************************************************************************
bool cLink::GetDBinfo(tFixed &Inst)
{
	pqxx::result r;
	string query, PointString;
	char *temp;
	temp = new char[33];
	double longitude, latitude;
	int spacePos;
	// Populate the rest of the fixed installation information from the database.
		
	gcvt(Inst.sInstKey,8,temp);
	if (mFrequency>59999)
	{ // Get frequency from database
		query = "SELECT siteid,ST_AsText(location) as location, eirp, txpower, txlosses, rxlosses, "; 			query += "rxsensitivity, frequency, ";
		query += "txantpatternkey, txbearing, txmechtilt, txantennaheight, ";
		query += "rxantpatternkey, rxbearing, rxmechtilt, rxantennaheight ";
		query += "FROM radioinstallation cross join site cross join cell ";
		query += "cross join frequencyallocationlist ";
		query += "WHERE cell.sector=radioinstallation.id and ci=cell.id and siteid=site.id ";
		query += "and carrier=1 and radioinstallation.id ="; 
		query += temp; 
		query += ";";
	}
	else
	{
		query = "SELECT siteid,ST_AsText(location) as location, eirp, txpower, txlosses, rxlosses, ";
		query += "rxsensitivity, txantpatternkey,txbearing,txmechtilt,txantennaheight,";
		query += "rxantpatternkey,rxbearing,rxmechtilt,rxantennaheight ";
		query += "FROM radioinstallation cross join site WHERE siteid=site.id AND radioinstallation.id =";
		query += temp; 
		query += ";";
		// Perform a Raw SQL query
	}
	if(!gDb.PerformRawSql(query))
	{
		string err = "In cLink: Database Select on tables radioinstallation and frequencyallocation failed: Query:";
		err+=query; 
		QRAP_ERROR(err.c_str());
		delete [] temp;
		return false;
	} // if
	else
	{
		gDb.GetLastResult(r);
		
		int size = r.size();
		if(size!=0)
		{
			Inst.sSiteID = atoi(r[0]["siteid"].c_str());
			PointString = r[0]["location"].c_str();
			spacePos = PointString.find_first_of(' ');
			longitude = atof((PointString.substr(6,spacePos).c_str())); 
			latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
			Inst.sSitePos.Set(latitude,longitude,DEG);
			Inst.sEIRP = atof(r[0]["eirp"].c_str());
			Inst.sTxPower = atof(r[0]["txpower"].c_str());
			Inst.sTxSysLoss = atof(r[0]["txlosses"].c_str());
			Inst.sRxSysLoss = atof(r[0]["rxlosses"].c_str());
			Inst.sRxSens = atof(r[0]["rxsensitivity"].c_str());
			Inst.sTxPatternKey = atoi(r[0]["txantpatternkey"].c_str());
			Inst.sTxAzimuth = atof(r[0]["txbearing"].c_str());
			Inst.sTxMechTilt = atof(r[0]["txmechtilt"].c_str());
			Inst.sRxPatternKey = atoi(r[0]["rxantpatternkey"].c_str());
			Inst.sRxAzimuth = atof(r[0]["rxbearing"].c_str());
			Inst.sRxMechTilt = atof(r[0]["rxmechtilt"].c_str());
			Inst.sTxHeight = atof(r[0]["txantennaheight"].c_str());
			Inst.sRxHeight = atof(r[0]["rxantennaheight"].c_str());
			if (mFrequency>59999)
				Inst.sFrequency = atof(r[0]["frequency"].c_str());
			else Inst.sFrequency = mFrequency;
		}
		else
		{
			// Log and display a warning.
			#ifdef QRAP_DEBUG
//			Base::Log("Warning, radio installation " + IntToString(mFixedInsts[i].sInstKey) + " does not exist in the database.");
			#endif
			// \TODO: exceptions and messages
			gcvt(mTxInst.sInstKey,8,temp);
			cout << query << endl;
			delete [] temp;
			cout << "Warning, radio installation " << mTxInst.sInstKey<< + " does not exist in the database." << endl;
			return false;
		} // else
	} // else
	delete [] temp;
	return true;
}

//***************************************************************************
bool cLink::GetDBinfoS(tFixed &Inst)
{
	pqxx::result r;
	string query, PointString;
	char *temp;
	temp = new char[33];
	double longitude, latitude;
	int spacePos;
	// Populate the site information from the database. The rest of the info is already in Inst
		
	gcvt(Inst.sSiteID,8,temp);
	query = "SELECT id,sitename,ST_AsText(location) as location from site ";
	query += "WHERE id=";
	query += temp; 
	query += ";";
	if(!gDb.PerformRawSql(query))
	{
		string err = "In cLink: Database Select on tables radioinstallation and frequencyallocation failed: Query:";
		err+=query; 
		QRAP_ERROR(err.c_str());
		delete [] temp;
		return false;
	} // if
	else
	{
		gDb.GetLastResult(r);
		
		int size = r.size();
		if(size!=0)
		{
			Inst.sSiteID = atoi(r[0]["id"].c_str());
			Inst.sSiteName = r[0]["sitename"].c_str();
			PointString = r[0]["location"].c_str();
			spacePos = PointString.find_first_of(' ');
			longitude = atof((PointString.substr(6,spacePos).c_str())); 
			latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
			Inst.sSitePos.Set(latitude,longitude,DEG);
		}
		else
		{
			gcvt(mTxInst.sSiteID,8,temp);
			cout << query << endl;
			cout << "Warning, radio installation " << mTxInst.sInstKey<< + " does not exist in the database." << endl;
			delete [] temp;
			return false;
		} // else
	} // else
	delete [] temp;
	return true;
}

//****************************************************************************************
void cLink::SaveLink(string &LinkName, int &LinkID)
{
	char *temp = new char[33];
	string query = "";

	if (LinkName=="Link")
	{
		LinkName = mTxInst.sSiteName;
		LinkName+=" to ";
		LinkName+= mRxInst.sSiteName;
	}

	if (!LinkID)
		query = "INSERT INTO links ";
	else
		query = "UPDATE links set ";
	query+= "(lastmodified,linkname,txinst,rxinst, minclearance,frequency, pathloss, kfactor, status, line) ";
	if (!LinkID)
		query += " VALUES";
	else
		query += " = ";
	query += "(now(),'";
	query += LinkName;
	query += "', ";
	gcvt(mTxInst.sInstKey,8,temp);
	query += temp;
	query += ", ";
	gcvt(mRxInst.sInstKey,8,temp);
	query += temp;
	query += ", ";
	gcvt(mMinClearance,8,temp);
	query += temp;
	query += ", ";	
	gcvt(mFrequency,8,temp);
	query += temp;
	query += ", ";	
	gcvt(mPathLossEnd,8,temp);
	query += temp;
	query += ", ";	
	gcvt(mkFactor,8,temp);
	query += temp;
        query += ", 'Experimental'";
	query += ", ST_GeomFromText('LINESTRING(";
	double lat,lon;
	mTxInst.sSitePos.SetGeoType(DEG);
	mTxInst.sSitePos.Get(lat,lon);
	gcvt(lon,10,temp);
	query += temp;
	query += " ";
	gcvt(lat,10,temp);
	query += temp;
	query += ", ";
	mRxInst.sSitePos.SetGeoType(DEG);
	mRxInst.sSitePos.Get(lat,lon);
	gcvt(lon,10,temp);
	query += temp;
    	query += " ";
	gcvt(lat,10,temp);
    	query +=temp;
    	query +=")',4326))";
	if (LinkID)
	{
		query += " where id = ";
		gcvt(LinkID,8,temp);
		query +=temp;
	}
	query+=";";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database link insert failed"<< endl;
	}
	if (!LinkID)
	{
		query ="select max(ID) as linkid from links;";
		pqxx::result r;
		if(!gDb.PerformRawSql(query))
		{
			string err = "In cLinkAnalysis::on_btnSave_clicked(): Failed to get last ID of Links table.";
			err+=query; 
			QRAP_WARN(err.c_str());
			LinkID=0;
		}
		else
		{
			gDb.GetLastResult(r);
			LinkID=atoi(r[0]["linkid"].c_str());
		}
	}
	delete [] temp;
}


//**************************************************************************
//
void cLink::WriteASCIIMatrix(FILE *fp)
{
	int i;
	mMinimum = 8880;
	mMaximum = -440;

	for (i=0;i<mLength;i++)
	{
		if ((mEffProfile[i] < mMinimum)&&(mEffProfile[i]!=0))	mMinimum = mEffProfile[i];
		else if (mEffProfile[i] > mMaximum) mMaximum = mEffProfile[i];
		fprintf(fp,"1 %d %f\n",i+1,mEffProfile[i]);
	}
	for (i=0;i<mLength;i++)
	{
		fprintf(fp,"2 %d %f\n",i+1,mLineOfSight[i]);
	}
	fprintf(fp,"3 %d %f\n",i+1,mFresnelH[0]);
	for (i=1;i<mLength-1;i++)
	{
		if (mFresnelH[i] > mMaximum) mMaximum = mFresnelH[i];
    		fprintf(fp,"3 %d %f\n",i+1,mFresnelH[i]);
	}
	fprintf(fp,"3 %d %f\n",i+1,mFresnelH[mLength-1]);
	fprintf(fp,"4 %d %f\n",i+1,mFresnelL[0]);
	for (i=1;i<mLength-1;i++)
	{
    		if (mFresnelL[i] < mMinimum) 	mMinimum = mFresnelL[i];
    		fprintf(fp,"4 %d %f\n",i+1,mFresnelL[i]);
	}
	fprintf(fp,"4 %d %f\n",i+1,mFresnelL[mLength-1]);
	for (i=0;i<mLength;i++)
	{
	    	fprintf(fp,"5 %d %f\n",i+1,mFresnelSH[i]);
	}
	for (i=0;i<mLength;i++)
	{
	    	fprintf(fp,"6 %d %f\n",i+1,mFresnelSL[i]);
	}
	for (i=0;i<mLength;i++)
     		fprintf(fp,"7 %d %f\n",i+1,mClearance[i]);
	for (i=0;i<mLength;i++)
		fprintf(fp,"8 %d %f\n",i+1,mPropLoss[i]);
	for (i=0;i<mLength;i++)
		fprintf(fp,"8 %d %f\n",i+1,mRxLev[i]);

	int bottom;
	int top;
	double centre;
	double offset;
	double R2;
	int grid1;
	int grid2;
	int grid3;
	double EffRadius=mEffRadius;
  
	centre = ((double)mLength)/2.0;
	bottom = (int)(floor(mMinimum/50.0)*50.0);
	top 	= (int)(ceil(mMaximum/50.0)*50.0);

	R2 = EffRadius*EffRadius;
	grid1  = (int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid1[i] = bottom + grid1 - EffRadius+ sqrt(R2 - offset*offset);
		fprintf(fp,"9 %d %f\n",i+1,mTrueHeightGrid1[i]);
	}

	EffRadius += grid1;
	R2 = EffRadius*EffRadius;
	grid2  = 2* (int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid2[i] = bottom + grid2 - EffRadius+ sqrt(R2 - offset*offset);
		fprintf(fp,"10 %d %f\n",i+1,mTrueHeightGrid2[i]);
	}

	EffRadius += grid2 - grid1;
	R2 = EffRadius*EffRadius;
	grid3  = 3*(int)((double)(top - bottom)/4.0 + 0.5);
	for (i=0; i<mLength; i++)
	{
		offset = (centre - i-1)*mPlotResolution;
		mTrueHeightGrid3[i] = bottom + grid3 - EffRadius+ sqrt(R2 - offset*offset);
		fprintf(fp,"11 %d %f\n",i+1,mTrueHeightGrid3[i]);
	}
} /* end WriteASCIIMatrix */


//**************************************************************************
bool cLink::ReadPlotRequest(const char *filename)
{
	
	const unsigned TEMPSIZE=1000;
	char temp[TEMPSIZE];
	bool FoundRasterSet = TRUE;
	
//	for (i=0;i<TEMPSIZE;i++)
//		temp[i] ='\0';
	
	ifstream infile(filename);
	if (!infile)
	{
		string err = "In cLink:ReadPlotRequest Error opening file: ";
		err+=filename;
		QRAP_ERROR(err.c_str());
		return false;
	}
		
	infile >> temp;
	if (infile.eof())
	{
		string err = "Error reading file: ";
		err+=+filename;
		err+=" at Units. End of file reached.";
		QRAP_WARN(err.c_str());
		return false;
	}
	else if (!strcasecmp(temp,"dB"))
		mUnits = dB;
	else if (!strcasecmp(temp,"dBW"))
		mUnits = dBW;
	else if (!strcasecmp(temp,"dBm"))
		mUnits = dBm;	
	else if (!strcasecmp(temp,"dBuV"))
		mUnits = dBuV;
	else if (!strcasecmp(temp,"dBuVm"))
		mUnits = dBuVm;
	else if (!strcasecmp(temp,"dBWm2Hz"))
		mUnits = dBWm2Hz;
	else if (!strcasecmp(temp,"dBWm2"))
		mUnits = dBWm2;
	else
	{
		string err = "Error reading file: ";
		err+=+filename;
		err+=" at Units. Using default.";
		QRAP_WARN(err.c_str());
	}
	infile >> temp;
	if (!strcasecmp(temp, "Freq"))
	{
		infile >> mFrequency;
		infile >> temp;
	}
	//infile >> temp;
	if (!strcasecmp(temp, "kFactor"))
	{
		infile >> mkFactor;
		infile >> temp;
	}
	if (!strcasecmp(temp, "Resolution"))
	{
		infile >> mPlotResolution;
		infile >> temp;
	}
	if (!strcasecmp(temp, "Tx"))
	{
		infile >> mTxInst.sInstKey;
		infile >> temp;
	}
	if (!strcasecmp(temp, "Rx"))
	{
		infile >>  mRxInst.sInstKey;
		infile >> temp;
	}
	if (!strcasecmp(temp, "DEM"))
	{
		infile >> mDEMsource;
		FoundRasterSet = mDEM.SetRasterFileRules(mDEMsource);
		if (!FoundRasterSet)
		{
			string err = "Error reading file: ";
			err+=filename;
			err+=" at DEM: Trouble getting DEM list. Using default";
			QRAP_WARN(err.c_str());
		}
		infile >> temp;
	}
	if (!strcasecmp(temp, "Clutter"))
	{
		infile >> mClutterSource;
		FoundRasterSet = mClutter.SetRasterFileRules(mClutterSource);
		mUseClutter = FoundRasterSet;
		if (!FoundRasterSet)
		{
			string err = "Error reading file: ";
			err += filename;
			err+=" at Clutter: Trouble getting Clutter list. Using default.";
			QRAP_WARN(err.c_str());
			return false;
		}
	}
	mDownlink=true;
//	bool ReturnValue = GetDBinfo(mTxInst);
//	ReturnValue = (ReturnValue)&&(GetDBinfo(mRxInst));
	return true;
}


