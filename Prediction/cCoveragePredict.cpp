 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCoveragePredict.cpp
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

#include "cCoveragePredict.h"

using namespace Qrap;

//************************************************************************
cCoveragePredict::cCoveragePredict()
{
	mSiteID = 1;
	mDownLink = true;
	mEIRP = 0.0;			// in dBm
	mTxPower = 0.001;	// in dBm
	mTxSysLoss = 0;	// in dB
	mRxSysLoss = 0;	// in dB
	mRxSens = -104;		// in dBm
	mFixedInst = 1;
	mFixedAzimuth = 0;	// in degrees 0 North, positive clockwise
	mFixedMechTilt = 0;	// in degrees with 0 on horizon and positive down
	mMobileInst = 1; 
	mBTLloaded = false;
	mAngleRes = 1; 	// in degees
	mNumAngles = 360;
	mNumRadialPoints = 2;
	mBTL = new_Float2DArray(mNumAngles,mNumRadialPoints);
	mTilt = new_Float2DArray(mNumAngles,mNumRadialPoints);
	mRxLev = new_Float2DArray(mNumAngles,mNumRadialPoints);
}


//*************************************************************************
cCoveragePredict::~cCoveragePredict()
{
	delete_Float2DArray(mBTL);
	delete_Float2DArray(mTilt);
	delete_Float2DArray(mRxLev);
}


//*************************************************************************
int cCoveragePredict::SetCommunicationLink(	int		SiteID,
						bool 		DownLink,
						double 		&EIRP,
						double		TxPower,
						double		TxSysLoss,
						double		RxSysLoss,
						double		RxSens,
						int		FixedInst,
						double		FixedAzimuth,
						double		FixedMechTilt,
						int		MobileInst,
						double 		FixedHeight, // in meters
						double 		MobileHeight, // 
						double 		&Frequency, // in MHz
						double 		kFactor,
						double 		&Radius, // in meters
						double 		DistanceRes, // in meters
						unsigned	NumAngles, // input in degrees
						int 		DTMsource, // key to source 
						int 		ClutterSource)
{
	mSiteID = SiteID;
	mDownLink = DownLink;
	mTxPower = TxPower;		// in dBm
	mTxSysLoss = TxSysLoss;		// in dB
	mRxSysLoss = RxSysLoss;		// in dB
	mRxSens = RxSens;		// in dBm
	mFixedAzimuth = FixedAzimuth;	// in degrees 0 North, positive clockwise
	mFixedMechTilt = FixedMechTilt;	// in degrees with 0 on horizon and positive down
	mNumAngles = NumAngles;
	mAngleRes = 360.0/(double)mNumAngles;
	mDistRes = DistanceRes;
	
	mFixedInst = FixedInst;
	mMobileInst = MobileInst;

	eAnt Which;

	if (DownLink) Which = Tx;
	else Which = Rx;
	mFixedAntenna.SetAntennaPattern(mFixedInst, Which, mFixedAzimuth, mFixedMechTilt);
	mMobileAntenna.SetAntennaPattern(mMobileInst, Mobile, 0, 0);
	
	// \TODO :
	if (EIRP<=0.00000001)
	{
		if (DownLink)
			EIRP = TxPower - TxSysLoss + mFixedAntenna.mGain; 
		else 
			EIRP = TxPower - TxSysLoss + mMobileAntenna.mGain;
	}
	mEIRP = EIRP;
	
	cout << "Freq: " << Frequency << endl;
	cout << "k: " << kFactor << endl;
	cout << "F_height: " << FixedHeight << endl;
	cout << "M_height: " << MobileHeight << endl;
	int rValue = mBTLPredict.Check_and_SetBTL
					(SiteID, Radius, mDistRes, mNumAngles,
					Frequency, FixedHeight, MobileHeight, 
					kFactor, DTMsource,	ClutterSource);
	return rValue;
}

//*************************************************************************
bool cCoveragePredict::CalculateRadialCoverage(bool AfterReceiver)
{
	unsigned 	i,j;
	double AntValue;
	double LinkOtherGain;
	cGeoP SitePos;
	double Radius;
	
	mBTLloaded = mBTLPredict.GetBTL(mNumAngles, mNumRadialPoints, 
									Radius, mDistRes);
	
	mAngleRes = 360.0/mNumAngles;
	if (!mBTLloaded)
		return false;
	delete_Float2DArray(mRxLev);
	mRxLev = new_Float2DArray(mNumAngles,mNumRadialPoints);

	if (AfterReceiver)
	{
		if (mDownLink)
			LinkOtherGain = mEIRP + mMobileAntenna.mGain - mRxSysLoss;
		else
			LinkOtherGain = mEIRP + mFixedAntenna.mGain - mRxSysLoss;
	}
	else LinkOtherGain = mEIRP;
	
	for (i=0; i<mNumAngles; i++)
	{		
		for (j=0; j<mNumRadialPoints; j++)
		{
			AntValue = mFixedAntenna.GetPatternValue(i*mAngleRes, mBTLPredict.mTilt[i][j]);
			mRxLev[i][j] = - mBTLPredict.mBTL[i][j] + LinkOtherGain - AntValue;
		}
//		cout << endl;
	}
	
	return true;
}

//*****************************************************************************
bool cCoveragePredict::InterpolateToSquare(cGeoP SitePos, cGeoP NW, // the centre of the top-left corner
					Float2DArray &Result, 
					double res, int &rows, int&cols)
{
	int i,j;
	cGeoP point;
//	double RfromNWtoSite, AfromNWtoSite, 
	double SiteN, SiteW, CornerN, CornerW;
	double R, rRes, aRes, nsRes, ewRes;
	double Rij, Aij;
//	double X, Y;
	int iR, iA;
	double V11,V12,V21,V22, V1, V2;
//	double HalfSide;
//	double midX, midY; // indicate site position, with NW at zero
	int SiteX, SiteY;
	NW.SetGeoType(DEG);
	SitePos.SetGeoType(DEG);
	NW.Get(CornerN,CornerW);
	SitePos.Get(SiteN,SiteW);

//	cout << rows << "   RxC   " << cols << endl;	
	mBTLloaded = mBTLPredict.GetBTL(mNumAngles, mNumRadialPoints, R, mDistRes);

	mAngleRes = 360.0/mNumAngles;
	if (!mBTLloaded) return false;
//	aRes=mAngleRes*PI/180.0;
	aRes = mAngleRes;
	rRes = mDistRes;
	nsRes = res*cDegResT;
	ewRes = res*cDegResT;
	SiteX = (int)((CornerN-SiteN)/nsRes+0.5);
	SiteY = (int)((SiteW-CornerW)/ewRes+0.5);
//	RfromNWtoSite = NW.Distance(SitePos);
//	AfromNWtoSite = NW.Bearing(SitePos);
//	double amper45 = (180.0-AfromNWtoSite)*PI/180.0;
//	midX = RfromNWtoSite*sin(amper45);
//	midY = RfromNWtoSite*cos(amper45);
//	HalfSide = RfromNWtoSite/sqrt(2.0);
//	size = 2*(int)ceil(HalfSide/res);
	
	delete_Float2DArray(Result);
	Result = new_Float2DArray(rows,cols);
	
	for (i=0; i<rows; i++)
	{
		for (j=0; j<cols; j++)
		{
//			X = j*res-midX;
//			Y = i*res-midY;
//			Aij = atan2(X,-Y);
//			if (Aij<0) Aij+=2*PI;
//			cout << "  " << Aij*180/PI;			
//			Rij = sqrt(X*X+Y*Y);
			point.Set(CornerN-(double)i*nsRes,CornerW+(double)j*ewRes,DEG);
			Rij = point.Distance(SitePos);
			Aij = SitePos.Bearing(point);
			if (Rij<R)
			{
//				Aij = atan2(X,-Y);
//				if (Aij<0) Aij+=2*PI;
				iR = (int)(Rij/rRes);
				iA = (int)(Aij/aRes);
//				cout << "iA: " << iA << "   Aij: " << Aij*180/PI << endl;
				if ((iA>mNumAngles-1)||(iA<0)||(iR<0)||(iR>mNumRadialPoints-1))
				// if out of range
				{
//					cout << "NumDist: " << mNumRadialPoints<<"   NumAngles=" << mNumAngles << endl;
//					cout << "    iR="<<iR << " iA=" << iA << endl;
					if ((iA==mNumAngles)&&(iR>-1)&&(iR<mNumRadialPoints))
						Result[i][j] = mRxLev[iA-1][iR];
					else if ((iR==mNumRadialPoints)&&(iA>-1)&&(iA<mNumAngles))
						Result[i][j] = mRxLev[iA][iR-1];
					else Result[i][j] =-120.0;
				}	
				else 
				// not out of range
				{
					V11 = mRxLev[iA][iR];
					if ((iA<(mNumAngles-1))&&(iR<(mNumRadialPoints-1)))
					{
						V21 = mRxLev[iA+1][iR];
						V12 = mRxLev[iA][iR+1];
						V22 = mRxLev[iA+1][iR+1];
						V1 = V11 + (Rij-iR*rRes)*(V12-V11)/rRes;
						V2 = V21 + (Rij-iR*rRes)*(V22-V21)/rRes;
						Result[i][j] = V1 + (Aij-iA*aRes)*(V2-V1)/aRes;
					}
					else if (iR<(mNumRadialPoints-1))
					{
						V21 = mRxLev[0][iR];
						V12 = mRxLev[iA][iR+1];
						V22 = mRxLev[0][iR+1];
						V1 = V11 + (Rij-iR*rRes)*(V12-V11)/rRes;
						V2 = V21 + (Rij-iR*rRes)*(V22-V21)/rRes;
						Result[i][j] = V1 + (Aij-iA*aRes)*(V2-V1)/aRes;
					}
					else if (iA<mNumAngles-1)
					{
						V21 = mRxLev[iA+1][iR];
						Result[i][j] = V11 + (Aij-iA*aRes)*(V21-V11)/aRes;
					}
					else
					{
						V21 = mRxLev[0][iR];
						Result[i][j] = V11 + (Aij-iA*aRes)*(V21-V11)/aRes;				
					}
				} // end else (not out of range)
			} //end if (Rij<R)
			else Result[i][j]= -9999;
		} // end for (j
		if (((double)i/100.0)==(i/100))
			cout << "Interpolate to Square: " << 100*i/rows << endl;
	} // end for (i
	return true;
}
