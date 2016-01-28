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

#include "cPthlssp.h"

using namespace Qrap;

double cPathLossPredictor::m_c = 2.997924562e8;  // Speed of Light in m/s
double cPathLossPredictor::m_reR = 6370000.0;    // real earth Radius in m

// Class CPathLossPredictor
//## Constructors (implementation)
// CPathLossPredictor:: Default Constructor
//************************************************************************
cPathLossPredictor::cPathLossPredictor(	double k, double f, 
					double txh, double rxh,
					bool UseClutter, unsigned ClutterClassGroup)
{
	int i;
	m_kFactor = k;
	m_freq = f;
	m_htx = txh;
	m_hrx = rxh;
	m_Loss = 0;
	m_size = 2;
	mCalcMarker = 0;
	mTuning=false;
	mClutterIndex = 0;

	delete [] m_profile;
	m_profile = new float[m_size];
	m_TempProfile = new float[m_size];
	m_CurvedProfile = new float[m_size];
	mClutterProfile = new int[m_size];
	m_markers = new int[MAXPEAK];
	m_peakwidth = new int[MAXPEAK];
	m_aboveEarth = new double[MAXPEAK];
	mCterms = new double[NUMTERMS];

	m_tempIPD = 30;
	m_slope = 0.0;
	m_SmoothWidth=1;
	m_SeekWidth=22;
	mUseClutter = UseClutter;

	m_markers[0] = 0;
	m_peakwidth[0] = 2;
	m_aboveEarth[0] = m_htx;
	m_markers[1] = m_size - 1;
	m_peakwidth[1] = 2;
	m_aboveEarth[1] = m_hrx;

	for (i=2; i<MAXPEAK; i++)
	{
		m_markers[i] = -1;
		m_peakwidth[i] =2;
		m_aboveEarth[i] = 0.0;
	};

	for (i=0; i<NUMTERMS; i++)
		mCterms[i]=0;
		
	if (NUMTERMS>0) mCterms[0] = TERM0;
	if (NUMTERMS>2) mCterms[2] = TERM2;
	if (NUMTERMS>4) mCterms[4] = TERM4;
	if (NUMTERMS>5) mCterms[5] = TERM5;

	mUseClutter = true;
	if (mUseClutter) mUseClutter = mClutter.Reset(ClutterClassGroup);

}/* end CPathLossPredictor:: Default Constructor */


//************************************************************************
// CPathLossPredictor :: Copy Constructor
cPathLossPredictor::cPathLossPredictor(const cPathLossPredictor &right)
{
	int i;

	mClutterIndex = right.mClutterIndex;
	mUseClutter = right.mUseClutter ;
	if (mUseClutter) mClutter = right.mClutter;

	m_kFactor = right.m_kFactor;
	m_freq = right.m_freq;
	m_htx = right.m_htx;
	m_hrx = right.m_hrx;
	m_Loss = right.m_Loss;

	m_tempIPD = right.m_tempIPD;
	m_slope = right.m_slope;
	m_size = right.m_size;
	m_startLoc = right.m_startLoc;
	m_endLoc = right.m_endLoc;
	m_interPixelDist = right.m_interPixelDist;
	m_SmoothWidth = right.m_SmoothWidth;
	m_SeekWidth = right.m_SeekWidth;

	delete [] m_profile;
	m_CurvedProfile = new float[m_size];
	m_TempProfile = new float[m_size];
	m_profile = new float[m_size];
	mClutterProfile = new int[m_size];

	for (i=0; i<m_size; i++)
    {
		m_CurvedProfile[i] = right.m_CurvedProfile[i];
                m_TempProfile[i]=  right.m_TempProfile[i];
		m_profile[i] = right.m_profile[i];
		mClutterProfile[i] = right.mClutterProfile[i];	
    }

	m_markers = new int[MAXPEAK];
	m_peakwidth = new int[MAXPEAK];
	m_aboveEarth = new double[MAXPEAK];
	for (i=0; i<MAXPEAK; i++)
	{
		*(m_markers+i) = *(right.m_markers+i);
		*(m_peakwidth+i) = *(right.m_peakwidth+i);
		*(m_aboveEarth+i) = *(right.m_aboveEarth+i);
	}

	mCterms = new double[NUMTERMS];
	for (i=0;i<NUMTERMS;i++)
		mCterms[i] = right.mCterms[i];

}/* end CPathLossPredictor Copy Constructor */


//************************************************************************
// CPathLossPredictor:: Destructor
cPathLossPredictor::~cPathLossPredictor()
{
	delete [] m_markers;
	delete [] m_peakwidth;
	delete [] m_aboveEarth;
	delete [] m_TempProfile;
	delete [] m_CurvedProfile;
	delete [] mClutterProfile;
	delete [] mCterms;
}/* end CPathLossPredictor:: Destructor */


//************************************************************************
// CPathLossPredictor: assignment operator
const cPathLossPredictor & cPathLossPredictor::operator=
					(const cPathLossPredictor &right)
{
	int i;

	if (this == &right)
		return (*this);

	mUseClutter = right.mUseClutter ;
	if (mUseClutter) mClutter = right.mClutter;
	mClutterIndex = right.mClutterIndex;

	m_kFactor = right.m_kFactor;
	m_freq = right.m_freq;
	m_htx = right.m_htx;
	m_hrx = right.m_hrx;
	m_Loss = right.m_Loss;

	m_tempIPD = right.m_tempIPD;
	m_slope = right.m_slope;
	m_interPixelDist = right.m_interPixelDist;
	m_size = right.m_size;
	m_startLoc = right.m_startLoc;
	m_endLoc = right.m_endLoc;
	m_SmoothWidth = right.m_SmoothWidth;
	m_SeekWidth = right.m_SeekWidth;

	delete [] m_CurvedProfile;
	delete [] m_TempProfile;
	delete [] m_profile;
	delete [] mClutterProfile;
	m_CurvedProfile = new float [m_size];
	m_TempProfile = new float[m_size];
	m_profile = new float[m_size];
	mClutterProfile = new int[m_size];
	for (i=0; i<m_size; i++)
	{
		*(m_CurvedProfile+i) = *(right.m_CurvedProfile+i);
		*(m_TempProfile+i) = *(right.m_TempProfile+i);
		*(m_profile+i) = *(right.m_profile+i);
		*(mClutterProfile+i) = *(right.mClutterProfile+i);
	}

	for (i=0; i<MAXPEAK; i++)
	{
		*(m_markers+i) = *(right.m_markers+i);
		*(m_peakwidth+i) = *(right.m_peakwidth+i);
		*(m_aboveEarth+i) = *(right.m_aboveEarth+i);
	}

	for (i=0;i<NUMTERMS;i++)
		mCterms[i] = right.mCterms[i];

return (*this);
}/* end CPathLossPredictor assignment */


//************************************************************************
int cPathLossPredictor::setParameters(double k, double f,
					double TxHeight, double RxHeight, 
					bool &UseClutter, unsigned ClutterClassGroup)
{
#ifndef NO_DEBUG
//	cout << " Entering setParameters" << endl;
#endif
	m_kFactor = k;
	m_freq = f;
	m_htx = TxHeight;
	m_hrx = RxHeight;
	
	mClutterIndex = 0;

	mUseClutter = UseClutter;
	if ((ClutterClassGroup!=mClutter.mClassificationGroup)&&(mUseClutter))
	{
		if (mUseClutter) mUseClutter = mClutter.Reset(ClutterClassGroup);
		UseClutter = mUseClutter;
		mClutter.mClassificationGroup = ClutterClassGroup;
	}

	for (unsigned i=0; i<NUMTERMS; i++)
		mCterms[i]=0;

	if (mUseClutter)
	{
		if (NUMTERMS>0) mCterms[0] = TERM0;
		if (NUMTERMS>2) mCterms[2] = TERM2;
		if (NUMTERMS>4) mCterms[4] = TERM4;
		if (NUMTERMS>5) mCterms[5] = TERM5;
	}

return 1;

}/* end CPathLossPredictor:: SetParameters */


//************************************************************************
void cPathLossPredictor::set_Clutter(bool &UseClutter, unsigned ClutterClassGroup)
{
	mUseClutter = UseClutter;
	if (mUseClutter) mUseClutter=mClutter.Reset(ClutterClassGroup);
	UseClutter = mUseClutter;
}

//************************************************************************
// Calculates the Total Path Loss
float cPathLossPredictor::TotPathLoss(cProfile &InputProfile, 
					float &ElevAngleTX,  
					cProfile &ClutterProfile, double &DiffLoss)
{
	double MinClearance=DBL_MAX;
	double OldMinClear=DBL_MAX;
	float ElevAngleRX=0.0;
	ElevAngleTX=0.0;
	double FreeSpace=0.0;
	double PlaneEarth=0.0;
	double KnifeEdge=0.0;
	double RoundHill=0.0;
	double ReffHeight=0.0;
	double sqrtD1D2 = 0.0;
	double radius = 0.0;
	double alpha = 0.1;
	int PeakIndex = 0;
	int OldPeakIndex;
	int IfTooManyPeaks = 0;
	int i;

	mCalcMarker++;
//	cout << endl << mCalcMarker << "	";
//	InputProfile.Display();
	
	mLinkLength = CalcDist(InputProfile);
	if (mLinkLength<10)
		return 0.0;
	
	FreeSpace = CalcFreeSpaceLoss(mLinkLength);
	PlaneEarth = CalcPlaneEarthLoss(mLinkLength);
	m_Loss = FreeSpace ;


	DiffLoss = 0;
	if (InputProfile.GetSize()>3)
	{
		InitEffectEarth(InputProfile, ClutterProfile);
		FindElevAngles(ElevAngleTX,ElevAngleRX);
		
		ReffHeight = Horizontalize(0,1);
		PeakIndex = FindMainPeak(0,1,ReffHeight, MinClearance,sqrtD1D2);
		if (mUseClutter) 
		{
			if ((MinClearance<1.0)&&(PeakIndex!=0)) 
					mClutterIndex = mClutterProfile[PeakIndex];
			else
			    mClutterIndex = mClutterProfile[m_size-1];
		}

		if ((MinClearance<1.0)&&(PeakIndex!=0))
		{
			radius = SetPeakRadius(PeakIndex,alpha);
			DiffLoss += CalcDiffLoss(m_markers[0],m_markers[1],PeakIndex,
							ReffHeight,sqrtD1D2,radius,alpha,
							true,IfTooManyPeaks,
							KnifeEdge, RoundHill);
			if (m_peakwidth[0]<2) m_peakwidth[0]=2;
			if (m_peakwidth[2]<2) m_peakwidth[2]=2;
			i=0;
			while (m_markers[i+1]!=-1)
			{
				ReffHeight = Horizontalize(i,i+1);
				OldPeakIndex=PeakIndex;
				OldMinClear = MinClearance;
				PeakIndex = FindMainPeak(i,i+1,ReffHeight,
								MinClearance,sqrtD1D2);
				if ((MinClearance<0.6)&&(PeakIndex!=0))
				{     
					radius= SetPeakRadius(PeakIndex,alpha);
					DiffLoss += CalcDiffLoss(m_markers[i],m_markers[i+1],
								PeakIndex,ReffHeight,
								sqrtD1D2,radius,alpha,
								false,IfTooManyPeaks,
								KnifeEdge, RoundHill);
					if ((MinClearance<=0)&&(OldMinClear<=-1)&&(i<MAXPEAK-3)&&(m_markers[i+3]>-1))
					{
//						cout << endl << " i=" << i << " oldpeak: " << OldPeakIndex 
//							<< " peak: " << PeakIndex << endl;;
//						for (int k=0;k<MAXPEAK; k++)
//						{
//							cout << k << " Peak: " << m_markers[k] << endl; 
//						}

						double d2 = abs(m_markers[i+2]-m_markers[i+1]);
						double d1 = abs(m_markers[i+1]-m_markers[i]);
						double d3 = abs(m_markers[i+3]-m_markers[i+2]);
//						cout << " d1=" << d1 << " d2=" << d2 << " d3=" << d3;
						double cotA = sqrt((d1+d2)*(d2+d3)/(d2*abs(m_markers[i+3]-m_markers[i])));
						double cosecA = 1/sin(atan(1/cotA));
//						cout << " cotA=" << cotA << " cosecA=" << cosecA 
//							<< " MC=" << MinClearance << " OMC=" << OldMinClear;
						double param = cosecA*cosecA-MinClearance*cotA/OldMinClear-cotA*cotA;
						double L=0;
						if (param>0) L=-20*log10(param);
//						cout << endl << mCalcMarker << " L=" << L ;
						DiffLoss += L;
					}
					if (IfTooManyPeaks) i++;
				}
				else i++;
				if ((i+1)>=MAXPEAK)
				{
//            				cout << "  MAXPEAK limit reached	";
					break;
				}
			}
		}
	}

//	cout << "FLoss=" << m_Loss << "	DiffL=" << DiffLoss << endl;
// The following 2 commands should mostly be commented out. It is when a emperical model is tuned:
//	m_Loss = 0;
//	DiffLoss = 0;


	if ((NUMTERMS>6)&&(mUseClutter)&&(mClutterIndex>=0)&&(mClutterIndex<100))
	{
		if (( mClutter.mClutterTypes[mClutterIndex].sCoefficients[6]>0)||(mTuning))	mCterms[6] = DiffLoss;
		else m_Loss+=DiffLoss;
	}
	else m_Loss+=DiffLoss;
	
	if (mUseClutter)
	{
//		cout << mClutterProfile[m_size-1] <<"."; 
//		double Cheight = mClutter.mClutterTypes[mClutterProfile[m_size-1]].sHeight;
		
//		double Cwidth = mClutter.mClutterTypes[mClutterProfile[m_size-1]].sWidth;
//		cout << "	" << mLinkLength << ".";
		if (NUMTERMS>1) 
		{
			if (mLinkLength>0.0) mCterms[1] = TERM1;
			else mCterms[1] = 0.0;
		}
		if (NUMTERMS>3) 
		{
			if (mLinkLength>0.0) mCterms[3] = TERM3;
			else mCterms[3] = 0.0;
		}		
//		if (Cheight < (m_htx+0.1))
//			mCterms[8] = TERM8;
//		else mCterms[8] = 100;

		if ((mClutterIndex>=0)&&(mClutterIndex<100))
		{
			for (i=0; i<NUMTERMS; i++)
				m_Loss += mClutter.mClutterTypes[mClutterIndex].sCoefficients[i]*mCterms[i];
		}

	}

//cout << "Na clutter in TotPathloss" << endl;

	m_Loss = max(m_Loss,FreeSpace);
//	m_Loss = max(m_Loss,max(FreeSpace,PlaneEarth));
/*
#ifndef NO_DEBUG
	m_counter++;
	cout << m_counter << "   m_size="<< m_size <<"      m_Loss=" << m_Loss << endl<<  endl;
#endif
*/
	return (float)m_Loss;

}/* end TotPathLoss */


//**************************************************************************
inline double cPathLossPredictor::CalcDist(cProfile& InputProfile)
{
	double Length;
	double HeightDifference;
	int SizeOfProfile;
	float* Profile;
	cGeoP start, end;

	Profile = new float[2];
	m_interPixelDist = InputProfile.GetInterPixelDist();
	InputProfile.GetProfile(SizeOfProfile,Profile);
//	start = InputProfile.GetStartLoc();
//	end = InputProfile.GetEndLoc();
//	Length = start.Distance(end);
	m_size = SizeOfProfile;
	Length = (double)m_size*m_interPixelDist;
	HeightDifference = ((double)Profile[0]+m_htx-
					(double)Profile[SizeOfProfile-1]-m_hrx);
	Length = sqrt(Length*Length + HeightDifference*HeightDifference);

	delete [] Profile;
return Length;
}/* end CPathLossPredictor::CalcDist */


//************************************************************************
inline double cPathLossPredictor::CalcFreeSpaceLoss(const double pathLength)
{
	double FreeLoss;
	if (pathLength >0)
		FreeLoss = 32.44778 + 20.0*log10(m_freq)
						+ 20.0*log10(pathLength/1000.0);
	else	FreeLoss = 0;
return  FreeLoss;
}/* end CalcFreeSpaceLoss */

//************************************************************************
inline double cPathLossPredictor::CalcPlaneEarthLoss(const double pathLength)
{
	double PlaneEarth;
	if (pathLength >0)
		PlaneEarth = 40.0*log10(pathLength) 
				- 20.0*log10(m_htx) - 20.0*log10(m_hrx);
	else	PlaneEarth = 0;
return  PlaneEarth;
}/* end CalcFreeSpaceLoss */


//**************************************************************************
// Initialize and calculate the effective earth profile, taking the m_kFactor
// into account.
void cPathLossPredictor::InitEffectEarth(const cProfile &InputProfile, 
					const cProfile &ClutterProfile)
{
	int i;
	double centre;
	double offset;
	double Reff_2;

//	cout << "In cPathLossPredictor::InitEffectEarth InputProfile.size()=" <<  InputProfile.GetSize() << endl;
//	cout << "In cPathLossPredictor::InitEffectEarth ClutterProfile.size()=" <<  ClutterProfile.GetSize() << endl;
	m_size = InputProfile.GetSize();

	m_interPixelDist = InputProfile.GetInterPixelDist();
	m_tempIPD = m_interPixelDist;
	m_slope = 0.0;
	m_markers[0] = 0;
	m_peakwidth[0] = 2;
	m_aboveEarth[0] = m_htx;
	m_markers[1] = m_size - 1;
	m_peakwidth[1] = 2;
	m_aboveEarth[1] = m_hrx;

	for (i=2; i<MAXPEAK; i++)
	{
		m_markers[i] = -1;
		m_peakwidth[i] =0;
		m_aboveEarth[i] = 0.0;
	}

//	m_startLoc = InputProfile.GetStartLoc();
//	m_endLoc = InputProfile.GetEndLoc();
	delete [] m_CurvedProfile;
	m_CurvedProfile = new float[m_size];
	delete [] m_TempProfile;
	m_TempProfile = new float[m_size];
	delete [] m_profile;
	m_profile = new float[m_size];
	InputProfile.GetProfile(m_size,m_profile);

	if (mUseClutter)
	{
		ClutterProfile.GetProfile(m_size,m_TempProfile);
		delete [] mClutterProfile;
		mClutterProfile = new int[m_size];
		for (i=0; i<m_size; i++)
			mClutterProfile[i] = (int)m_TempProfile[i];
	}

	m_SeekWidth = (int) 36.9*pow((m_interPixelDist/18.4),(m_freq/1900));
//	cout << " m_SeekWidth=" << m_SeekWidth;
//      m_SmoothWidth = (int)(m_c/m_freq/m_interPixelDist/18000);
//	cout << " m_SmoothWidth=" << m_SmoothWidth  << endl;
	if (m_SeekWidth<3) m_SeekWidth = 3;

	if (m_kFactor!=1.0)
	{
		centre = ((double)m_size)/2.0;
		Reff_2 = 2.0 * m_kFactor * m_reR;	// Reference:
		for(i=0;i<m_size;i++)			// Radio Wave Propagation
		{                                 	// A.Picquenard, p.17
		   offset = (centre-(double)i-1) * m_interPixelDist;
		   *(m_CurvedProfile+i) = *(m_profile+i)-(float)(offset*offset/(Reff_2));
		   *(m_TempProfile+i) = *(m_CurvedProfile+i);
		}
	}// end if k
	else
	{
		for (i=0; i<m_size;i++)
		{
			*(m_CurvedProfile+i) = *(m_profile+i);
			*(m_TempProfile+i) = *(m_CurvedProfile+i);
		}
	}/*endif else*/

	if (mUseClutter)
	{
		for (i=1; i<m_size-1;i++)
		{
			*(m_CurvedProfile+i) += mClutter.mClutterTypes[mClutterProfile[i]].sHeight;
			*(m_TempProfile+i) += mClutter.mClutterTypes[mClutterProfile[i]].sHeight;
		}
	}
	
/*	cout << "PathProfile: ";  
	for (i=0; i<m_size;i++)
		cout << m_profile[i] << " ";
	cout << endl;
*/
}/* end InitEffectEarth */



//**************************************************************************
// Find the elevation angle that should be used for determining the antenna
// gain in the specific direction. // angles in degrees
void cPathLossPredictor::FindElevAngles(float &ElevAngleTX, float &ElevAngleRX)
{
	int i;
	double ElevSlopeTX, ElevSlopeRX;
	double slope;
	int Index;
	double xDist;
	double yHeight;
	double yReff;
	double R;
	double x0, y0;
	double offset;
	double temp;

	ElevSlopeTX = ((double)*(m_CurvedProfile+m_size-1)+m_hrx -
				(double)*(m_CurvedProfile) - m_htx)/((m_size-1)*m_interPixelDist);
	ElevSlopeRX = -ElevSlopeTX;
	
	Index = m_size-1;
	for(i=1;i<m_size;i++)
	{
		slope = ((double)*(m_CurvedProfile+i) - (double)*(m_CurvedProfile) - m_htx)
				/((double)i*m_interPixelDist);
		if (slope>ElevSlopeTX)
		{
			ElevSlopeTX = slope;
			Index = i;
		}
	}

	xDist = m_interPixelDist * (double)(Index);
	offset = ((double)m_size/2.0-Index) * m_interPixelDist;
	yReff = (double)*(m_profile) - m_interPixelDist*m_interPixelDist*
		  (double)(m_size*m_size)/(8.0*m_reR) + m_htx;
	yHeight = (double)*(m_profile+Index)-offset*offset/(2.0*m_reR) - yReff;
	if (Index == (m_size-1)) yHeight += m_hrx;

	if (m_kFactor!=1.0)
	{
		R = m_reR/((1.0-1.0/m_kFactor)*cos(atan(yHeight/xDist)));
		temp = yHeight*yHeight + xDist*xDist;
		y0 = yHeight/2.0 - xDist * sqrt(4.0*R*R/temp -1.0) /2.0;
		x0 = (temp - 2.0*y0*yHeight) / (2.0*xDist);
		yHeight = sqrt(R*R-(xDist-x0)*(xDist-x0)) + y0;
	}
   // else R is infinite and yHeight does not change. (earth is already flat)

	ElevSlopeTX = yHeight/xDist;
	ElevAngleTX = (180.0*atan(ElevSlopeTX)/PI);

	Index = 0;
	for(i=m_size-2; i>0 ;i--)
	{
		slope=((double)*(m_CurvedProfile+i)-(double)*(m_CurvedProfile+m_size-1) - m_hrx)
				/((double)(m_size-1-i)*m_interPixelDist);
		if (slope>ElevSlopeRX)
		{
			ElevSlopeRX = slope;
			Index = i;
		}
	}

	xDist = m_interPixelDist * (double)(m_size-1-Index);
	offset=((double)m_size/2.0-Index) * m_interPixelDist;
	yReff=(double)*(m_profile+m_size-1) + m_hrx -
		pow(((double)(m_size-1)-(double)m_size/2.0)*m_interPixelDist,2.0)
		/(2.0*m_reR);
	yHeight=(double)*(m_profile+Index)-offset*offset/(2.0*m_reR) - yReff;
	if (Index == 0) yHeight += m_htx;

   	if (m_kFactor!=1.0)
   	{
			R = m_reR/((1.0-1.0/m_kFactor)*cos(atan(yHeight/xDist)));
	   	temp = yHeight*yHeight + xDist*xDist;
	   	y0 = yHeight/2.0 - xDist * sqrt(4.0*R*R/temp -1.0) /2.0;
	   	x0 = (temp - 2.0*y0*yHeight) / (2.0*xDist);
	   	yHeight = sqrt(R*R-(xDist-x0)*(xDist-x0)) + y0;
   	}
   	//else R is infinite and yHeight does not change. (earth is already flat)

	ElevSlopeRX = yHeight/xDist;
	ElevAngleRX = 180.0*atan(ElevSlopeRX)/PI;
//	cout << "Rxh: " << m_hrx << "	Txh: " << m_htx << endl;
   
}/* end CPathLossPredictor::FindElevAngle */

//***************************************************************************
double cPathLossPredictor::Horizontalize(	const int BeginMarkerIndex,
							const int EndMarkerIndex)
{
	int i,start,stop;
	double angle;	// The angle of the line of sight of the
				// originals profile with respect to the horizon
	double sinA;
	double BeginHeight, EndHeight;
	int Begin,End;
	Begin = m_markers[BeginMarkerIndex];
	End = m_markers[EndMarkerIndex];

	BeginHeight = m_CurvedProfile[Begin] + m_aboveEarth[BeginMarkerIndex];
	EndHeight = m_CurvedProfile[End] + m_aboveEarth[EndMarkerIndex];

	m_slope=(EndHeight-BeginHeight)/((double)(End-Begin)*m_interPixelDist);
	angle = atan(m_slope);
	sinA = sin(angle);
	m_tempIPD = m_interPixelDist/cos(angle);
	start = max(0,Begin-m_SeekWidth-m_SmoothWidth-1);
	stop = min(m_size-1,End+m_SeekWidth+m_SmoothWidth+1);

	for (i=start; i<=stop; i++)
		m_TempProfile[i]=m_CurvedProfile[i]-(float)((double)(i-Begin)*m_tempIPD*sinA);

return BeginHeight;
}/* end CPeakProfile::Horizontalize */


//**************************************************************************
int cPathLossPredictor::FindMainPeak(const int BeginMarkerIndex,
						 const int EndMarkerIndex,
						 const double ReffHeight,
						 double &MinClear, double &SQd1d2)
{
	int MainPeakIndex=0;
	int i;
	int begin, end;
	double sqrtd1d2;
	double rootLambda;
	double Clear;		// Clearance
	MinClear=DBL_MAX;

	begin = m_markers[BeginMarkerIndex];
	end = m_markers[EndMarkerIndex];
	rootLambda = sqrt(m_c/(1000000.0*m_freq));
	if ((begin+m_peakwidth[BeginMarkerIndex])<=(end-m_peakwidth[EndMarkerIndex]))
	{
		for (i=max(0,begin+m_peakwidth[BeginMarkerIndex]);
			i<=min(m_size-1,(end-m_peakwidth[EndMarkerIndex]));i++)
		{
			sqrtd1d2 = sqrt((double)((i-begin)*(end-i))*m_tempIPD
						/ (double)(end-begin));
			Clear = (ReffHeight-(double)m_TempProfile[i])/(rootLambda*sqrtd1d2);
			if (Clear<MinClear)
			{
				MinClear = Clear;
				SQd1d2 = sqrtd1d2;
				MainPeakIndex = i;
			}

		}/* end for */

	 }/* end if */

return MainPeakIndex;
}/* end CPeakProfile::FindMainPeak */


//**************************************************************************
// Identify the peaks on a profile.
// Calculate and set the radii of the peaks in object of class CPeakProfile
double cPathLossPredictor::SetPeakRadius(int PeakIndex, double &alpha)
{
	double radius;
	float* SmoothProfile;
	int sizeSP;			// size of SmoothProfile
	int SPeakIndex;
	int leftInfl,rightInfl;	// Left and right inflection points of peak
	int i,j;               	// Loop m_counters
	double xL,xR;		// The "x" and "y"-coordinates of the left-
	double yL,yR; 		// and right inflection points relative to the
	double x0,y0;		// Peak
	double tempL, tempR;
	int start, stop;

	if (m_size<2)
	{
		alpha=PI;
	 	return 0.0;		
	}
//	cout << "In cPathLossPredictor::SetPeakRadius:m_size = " << m_size;
	sizeSP = m_size;
	SmoothProfile = new float[sizeSP];
	start = max(0,PeakIndex-m_SeekWidth-m_SmoothWidth);
	stop = min(PeakIndex+m_SeekWidth+m_SmoothWidth,sizeSP-1);

	for (i=start; i<=stop; i++)
	{
		*(SmoothProfile+i) = 0.0;
		for (j=max(start,i-m_SmoothWidth);j<=min(stop,i+m_SmoothWidth);j++)
		{
			*(SmoothProfile+i) += *(m_TempProfile+j);
		}
		*(SmoothProfile+i) /=
      			((float)(min(sizeSP-1,i+m_SmoothWidth)-max(0,i-m_SmoothWidth)+1));
	}

  	start = max(0,PeakIndex-m_SeekWidth);
	stop = min(PeakIndex+m_SeekWidth,sizeSP-1);
	SPeakIndex = PeakIndex;
	// Shift peak to the left if SmoothProfile has a peak to the left.
	if (SPeakIndex > 0)
		if (*(SmoothProfile+SPeakIndex-1)
			>*(SmoothProfile+SPeakIndex))
			while ((SPeakIndex>1)&&(SPeakIndex>(PeakIndex-m_SeekWidth)) &&
				(*(SmoothProfile+SPeakIndex-1)
				>*(SmoothProfile+SPeakIndex)))
				SPeakIndex--;
	// Shift peak right if SmoothProfile has a peak to the right.
	if (SPeakIndex < sizeSP-1)
		if (*(SmoothProfile+SPeakIndex+1)
			> *(SmoothProfile+SPeakIndex))
			while ((SPeakIndex<sizeSP-1)&&(SPeakIndex<(PeakIndex+m_SeekWidth))
				&&(*(SmoothProfile+SPeakIndex+1)
				>*(SmoothProfile+SPeakIndex)))
				SPeakIndex++;
	// Determine the left inflection point.
	leftInfl = SPeakIndex;
	if ((leftInfl==(m_size-1))&&
		(*(SmoothProfile+leftInfl)>*(SmoothProfile+leftInfl-1)))
		leftInfl--;
	if (leftInfl<m_size-1)
	while ((leftInfl>start+1)&&(leftInfl>(PeakIndex-m_SeekWidth))&&
		((*(SmoothProfile+leftInfl)-*(SmoothProfile+leftInfl-1))
		> (*(SmoothProfile+leftInfl+1)-*(SmoothProfile+leftInfl))))
		leftInfl--;
	if ((leftInfl==start+1)&&((*(SmoothProfile+start+2)-*(SmoothProfile+start+1))<
			(*(SmoothProfile+start+1)-*(SmoothProfile+start))))
		leftInfl=start;
	// Determine the right inflection point
	rightInfl = SPeakIndex;
	if ((rightInfl==0)&&
		(*(SmoothProfile+rightInfl)>*(SmoothProfile+rightInfl+1)))
		rightInfl++;
	if (rightInfl>0)
	while ((rightInfl<stop-1)&&(rightInfl<(PeakIndex+m_SeekWidth))&&
		((*(SmoothProfile+rightInfl)-*(SmoothProfile+rightInfl+1))
		> (*(SmoothProfile+rightInfl-1)-*(SmoothProfile+rightInfl))))
		rightInfl++;
	if ((rightInfl==stop-1)&&((*(SmoothProfile+stop)-*(SmoothProfile+stop-1))
			<(*(SmoothProfile+stop-1)-*(SmoothProfile+stop-2))))
		rightInfl = stop;
	if ((SPeakIndex==leftInfl)||(SPeakIndex==rightInfl))
	{
		SPeakIndex = PeakIndex;
		if (SPeakIndex==leftInfl) SPeakIndex++;
		if (SPeakIndex==rightInfl) SPeakIndex--;
		if ((SPeakIndex==leftInfl)&&
		    (*(SmoothProfile+rightInfl)>*(SmoothProfile+leftInfl)))
			SPeakIndex++;
	}
	xL = (double)(-leftInfl+SPeakIndex)*m_tempIPD;
	xR = (double)(rightInfl-SPeakIndex)*m_tempIPD;
	yL = (double)(-*(SmoothProfile+leftInfl)+*(SmoothProfile+SPeakIndex));
	yR = (double)(-*(SmoothProfile+rightInfl)+*(SmoothProfile+SPeakIndex));

	alpha = atan2(yL,xL)+atan2(yR,xR);
	tempL = xL*xL + yL*yL;
	tempR = xR*xR + yR*yR;

	if ((yR==0.0)&&(yL==0.0))
		{radius=3*m_reR;}
	else if ((yR==0.0)&&(xL==0.0))
		{radius = 3*m_reR;}
	else if ((yL==0.0)&&(xR==0.0))
		{radius = 3*m_reR;}
	else if (xL==0.0)
		{radius = tempR/(2.0*yR);}
	else if (xR==0.0)
		{radius = tempL/(2.0*yL);}
	else if ((xR==-xL)&&(yL==-yR))
		{radius = tempL/(2.0*yL);}
	else	{x0 = (-yR*tempL+yL*tempR);
		 y0 = (-xL*tempR-xR*tempL);
		 radius=(sqrt(x0*x0+y0*y0)/(2.0*(xL*yR+xR*yL)));}

	if (radius>MAXDOUBLE/1.2) radius = (2.0*(xL+xR)*xR*xL)/(alpha*(xL*xL+xR*xR));
	else if (radius<-(MAXDOUBLE/1.2)) radius = (2.0*(xL+xR)*xR*xL)/(alpha*(xL*xL+xR*xR));

	if (radius>MAXDOUBLE/1.2) radius = 3*m_reR;
	else if (radius<-(MAXDOUBLE/1.2)) radius = 3*m_reR;

/*	
	cout << endl<< "	r2:"<< radius;

	double  r1 = (2.0*(xL+xR)*xR*xL)/(alpha*(xL*xL+xR*xR)); 
	cout << "	r1:"<< r1;
        cout << "	xL=" << xL << "  xR=" << xR << "  yL=" << yL << "  yR=" << yR << endl;
*/
	delete [] SmoothProfile;
	if (leftInfl>=rightInfl)
	{
		alpha = PI;
 		return 0.0;
	}
	return radius;
}/*end SetPeakRadii*/


//************************************************************************
// Reference: David Parsons, "The Mobile Radio Propagation Channel", 1992
// 		  paragraph 3.3.2 	p.44-46
double cPathLossPredictor::CalcDiffLoss(const int BeginIndex,
					const int EndIndex, int PeakIndex,
					const double ReffHeight,
					const double SQd1d2, double radius, double alpha,
					bool MainEdge, 
					int &TooManyPeaks, double &KnifeEdge, double &RoundHill, 
					const double Er, const double Sigma,
					const int Vertical)
{
	double DiffLoss;
	double mhu;
	double rho;
	double MhuRho;
	KnifeEdge=0.0;
	RoundHill=0.0;
	double Surface=0.0;
	double temp;
	double lambda;
	double Y1, Y2;
	double K, Betha, X;

	TooManyPeaks = 0;
	lambda = (m_c/(m_freq*1000000.0));
	rho = pow(radius*radius*lambda/M_PI,1.0/6.0) / SQd1d2;
	mhu = ((double)m_TempProfile[PeakIndex]-ReffHeight)
			* sqrt(2.0/lambda)/SQd1d2;
	MhuRho = mhu*rho;

#ifndef NO_DEBUG
//	cout	<< " radius=" << radius << "  rho=" << rho
//		<< "   mhu=" << mhu << "   MhuRho=" << MhuRho << endl;
#endif

	if (mhu<-0.8) KnifeEdge = 1.0;	// equivalent to 0
	else if (mhu < 0.0) KnifeEdge = 0.5 - 0.62*mhu;
	else if (mhu < 1.0)  KnifeEdge = 0.5*exp(-0.95*mhu);
	else if (mhu < 2.4) { temp = 0.38 - 0.1*mhu;
				   KnifeEdge = 0.4 - sqrt(0.1184 - temp*temp);}
	else KnifeEdge = 0.225/mhu;
	KnifeEdge = -20.0*log10(KnifeEdge);

	temp = (double)(EndIndex-BeginIndex)*m_interPixelDist/2.0;

	if ((!Vertical)&&(radius>0)&&(rho<=1.4)&&(radius<0.5*m_reR)&&(mhu>-0.6))
	{
		// Parsons  //if (rho<=1.4)   //For horizontal polirization 
		// Dougherty & Maloney 1964 Radio Science Journal of Research
//		cout << " D&M	";
		RoundHill = 6.0 + 7.19*rho - 2.02*rho*rho
				+ 3.63*rho*rho*rho - 0.75*rho*rho*rho*rho;
		if (MhuRho<-0.95) Surface = -30.0;
		else if (MhuRho<2.0)
			Surface=(43.6+23.5*MhuRho)*log10(1.0+MhuRho) -
					6.0 - 6.7*MhuRho;
		else 	Surface=22.0*MhuRho-20.0*log10(MhuRho)-14.13;
	}
	else if ((radius>0)&&(rho<=1.2)&&(radius<=0.5*m_reR)&&(mhu>-0.6))
	{
		//  Old RAP 	// if (rho<=1.0) 
//		cout << " OldRap		";
		RoundHill = 6.02 + 5.556*rho + 3.418*rho*rho +
				0.256*rho*rho*rho;
		if (MhuRho<-2.0) Surface =-20.0;
		else if (MhuRho<=3.0)
			Surface = 11.45*MhuRho + 2.19*MhuRho*MhuRho -
					0.206*MhuRho*MhuRho*MhuRho - 6.02;
		else if (MhuRho<=5.0)
			Surface = 13.47*MhuRho + 1.058*MhuRho*MhuRho -
					0.048*MhuRho*MhuRho*MhuRho - 6.02;
		else Surface = 20.0*MhuRho -18.2;
	}
	else if ((temp<radius)&&(radius>0.5*m_reR)&&(MainEdge)&&(mhu>0.0))
	{
		//  Rec. ITU-R P.526-12 Feb 2012 $3.1.1 
		// Spherical earth
//		temp = (double)(EndIndex-BeginIndex)*m_interPixelDist/2.0;
		Y1=Y2=ReffHeight-m_TempProfile[PeakIndex]+radius -
			sqrt(radius*radius-temp*temp);
		temp = 60.0*60.0*lambda*lambda*Sigma*Sigma;
		K = pow(2.0*M_PI*radius/lambda,-1.0/3.0)
			* pow((Er-1.0)*(Er-1.0) + temp, -0.25);
		if (Vertical) K *= sqrt(Er*Er +temp);
		temp = K*K*K*K;
		Betha = ( 1.0+1.6*K*K+0.67*temp)/(1.0+4.5*K*K+1.53*temp);
		X = Betha*pow(M_PI/(lambda*radius*radius),1.0/3.0)*
		    (m_interPixelDist*(double)(EndIndex-BeginIndex));
		temp = 2.0*Betha*pow(M_PI*M_PI/(lambda*lambda*radius),1.0/3.0);
		Y1 *= temp;
		Y2 *= temp;
		if (X>=6) Surface =  -(11.0 + 10.0*log10(X) -17.6*X);
		else Surface = 20*log10(X) + 5.6488*pow(X,1.425);
		Surface -=  (HeightGain(Y1,K) + HeightGain(Y2,K));
		KnifeEdge = 0;
//		Surface *= mClutter.mClutterTypes[mClutterProfile[PeakIndex]].sRho;
	}
	else if (radius>0)
	{
		// according to Hacking BBC report Parsons p45	
//		cout << " Hacking		";
//		RoundHill = mClutter.mClutterTypes[mClutterProfile[PeakIndex]].sRho
		RoundHill = 11.7*pow((PI*radius*m_freq*1000*1000/m_c),1/3)*alpha;

//		double R2 = RoundHill;
//		RoundHill = max(R1,R2);
	}
/*
	else if ((radius>0)&&(radius<3*m_reR)&&(rho<1.3)&&(mhu>-0.6))
	{
		//  Rec. ITU-R P.526-12 Feb 2012 $4.2 
//		if (mCalcMarker==142423) 
		cout << " RoundHill	";
		RoundHill = 7.2*rho - 2.0*rho*rho + 3.6*rho*rho*rho
						- 0.8*rho*rho*rho*rho;
		double xhi;
		xhi = MhuRho*sqrt(M_PI/2.0);
		Surface = 0.0;
//		if ((xhi>=-rho)&&(xhi<0)) Surface = RoundHill*xhi/rho;
		if (xhi<=4.0) Surface = 12.5*xhi;
		else Surface = 17.0*xhi - 6.0 - 20.0*log10(xhi);
		
//		double R1 = Surface + RoundHill;
//		Surface = 0.0;
	}

*/
/*	else if (MainEdge)
	{
		// Adjustment to Flat earth model
		cout << " FlatEarth	";
		Y1=Y2=ReffHeight-m_TempProfile[PeakIndex];
		if (Y1<0.1) Y1 =Y2 = 0.1;
		Surface = -mClutter.mClutterTypes[mClutterProfile[PeakIndex]].sRho
				*20.0*log10(Y1*Y2*4.0*M_PI/(lambda*mLinkLength));
	}
*/	
/*	{
#ifndef NO_DEBUG
	cout << " mhu=" << mhu;
	cout << " radius=" << radius;
	cout << " rho=" << rho;
	cout << " KnifeEdge=" << KnifeEdge;
	cout << " RoundHill=" << RoundHill;
	cout << " Surface=" << Surface;
#endif
	}
*/
	if ((RoundHill+Surface)<0.0)
	{
		RoundHill=0;
		Surface = 0;
	}

	DiffLoss = KnifeEdge + RoundHill + Surface;

	int i,j;
	for(i=0;(i<MAXPEAK)&&(m_markers[i]<PeakIndex)&&(m_markers[i]!=-1);i++);
	if (m_markers[MAXPEAK-1]==-1)
		for(j=MAXPEAK-1; (j>i)&&(j>0) ; j--)
		{
			m_markers[j] = m_markers[j-1];
			m_peakwidth[j] = m_peakwidth[j-1];
			m_aboveEarth[j] = m_aboveEarth[j-1];
		}
	else TooManyPeaks=1;

	if (!TooManyPeaks)
	{
		m_markers[i] = PeakIndex;
		temp = (double)m_TempProfile[PeakIndex]-
			 (ReffHeight-sqrt(lambda)*SQd1d2);
		if (radius<0.0) m_peakwidth[i] = m_size;
		else if ((temp<0)||(temp>2.0*radius)) m_peakwidth[i] = 2;
		else 	m_peakwidth[i] = (int)(sqrt(temp*(2.0*radius-temp))
							/m_interPixelDist+0.5);
		if (m_peakwidth[i]<2) m_peakwidth[i]=2;

		if (mhu<0) m_aboveEarth[i]=(ReffHeight-m_TempProfile[PeakIndex]);
		else m_aboveEarth[i] = 0.0;
	}
#ifndef NO_DEBUG
//	for (i=0;i<MAXPEAK;i++)
//		cout << m_markers[i] << " ";
//	cout << endl;
#endif

	return DiffLoss;
} /* end CPathLossPrdictor::CalcDiffLoss */


//**************************************************************************
inline double cPathLossPredictor::HeightGain(const double Y, const double K)
{
	double Gain,temp;
	temp = 2.0 + 20.0*log10(K);
	if (Y<(K/10.0))
		Gain = temp;
	else if (Y<(10.0*K))
		Gain = 2.0 + 20.0*log10(K) +9.0*log10(Y/K)*(log10(Y/K)+1.0);
	else if (Y<2.0)
		Gain = 20.0*log10(Y+0.1*Y*Y*Y);
	else	Gain = 17.6*sqrt(Y-1.1) -5.0*log10(Y-1.1) -8.0;
	if (Gain < temp) Gain = temp;  
return Gain;
}/* end CPathLossPredictor::HeightGain */





