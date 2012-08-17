 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasAnalysisCalc.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class does the analysis of the measurements in the 
 *			the database
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



#include "cMeasAnalysisCalc.h"
//*********************************************************************
cMeasAnalysisCalc::cMeasAnalysisCalc() // default constructor
{
	mMeasPoints = new tMeasPoint[2];
	mMinTerm = new double[NUMTERMS];
	mMaxTerm = new double[NUMTERMS];
	mMidTerm = new double[NUMTERMS];

	unsigned i,j;
	for(i=0; i<NUMTERMS; i++)
	{
		mMinTerm[i] =   MAXDOUBLE;
		mMaxTerm[i] = - MAXDOUBLE;
		mMidTerm[i] =   MAXDOUBLE;		
		mLeftSide(i) = 0.0;
		mDeltaCoeff(i) = 0.0;
		for (j=0; j<NUMTERMS; j++)
			mSolveCoefMatrix(i,j) = 0.0;
	}

	mNumMeas = 0;	
	double kS, kI;
	mUnits = dBm;
	string setting;
	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		kS=atof(setting.c_str());
	else kS = 1.25;
	setting = gDb.GetSetting("kFactorInt");
	if(setting!="")
		kI=atof(setting.c_str());
	else kI = 1.25;
	mkFactor = min((kI+2*kS)/3.0,1.33);

	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
	{
		mPlotResolution = atof(setting.c_str());
	}
	else mPlotResolution = 90;
	setting = gDb.GetSetting("UseClutter");
	if (setting=="true")
		mUseClutter = true;
	else mUseClutter = false;
	mClutterSource = atoi(gDb.GetSetting("ClutterSource").c_str());
	if (mUseClutter)
		mUseClutter = mClutter.SetRasterFileRules(mClutterSource);
	if (mUseClutter)
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	mUseClutter = (mUseClutter)&&(mClutterClassGroup>0);

	mDEMsource = atoi(gDb.GetSetting("DEMsource").c_str());
	mDEM.SetRasterFileRules(mDEMsource);
}

//*********************************************************************
cMeasAnalysisCalc::~cMeasAnalysisCalc() // destructor
{
	delete [] mMeasPoints;		
	delete [] mMaxTerm;
	delete [] mMinTerm;
	delete [] mMidTerm;
}


//*********************************************************************
int cMeasAnalysisCalc::LoadMeasurements(unsigned MeasType, unsigned PosSource, 
					unsigned MeasSource, unsigned CI)
{

	char * text = new char[10];		
	unsigned MeasCount=0;	
	pqxx::result r, rMobile, rFixed;
	double Lat, Lon, prevLat, prevLon, Meas, dist;

	string query ="SELECT mobile, ci, frequency, AsText(location) as location, ";
	query += "measurement.id as id, measvalue, predictvalue, pathloss, distance, tilt, azimuth ";
	query += "from measurement cross join testpoint cross join measdatasource ";
	query += "where tp=testpoint.id and measdatasource=measdatasource.id";
	if (MeasType>0)
	{
		query += " and meastype=";
		gcvt(MeasType,9,text);
		query += text;
	}
	if (MeasSource>0)
	{
		query += " and measdatasource=";
		gcvt(MeasSource,9,text);
		query += text;
	}
	if (PosSource>0)
	{
		query += " and positionsource=";
		gcvt(PosSource,9,text);
		query += text;
	}
	if (CI>0)
	{
		query += " and ci=";
		gcvt(CI,9,text);
		query += text;
	}
	query+=" order by mobile, ci;";

	string PointString;
	double longitude, latitude; 
	unsigned spacePos;
	unsigned currentCI=0;
	int currentFixedInst=0;
	int currentMobile=0;
	tFixed tempInst;
	tMobile tempMobile;
	// Perform a Raw SQL query
	if(!gDb.PerformRawSql(query))
	{
		string err = "Error in Database Select on Measurement Table. Query:";
		err+=query; 
		QRAP_ERROR(err.c_str());
		return false;
	} // if
	else
	{
		gDb.GetLastResult(r);
			
		mNumMeas = r.size();
	
		if(mNumMeas!=0)
		{
			delete [] mMeasPoints;
			mMeasPoints = new tMeasPoint[mNumMeas];
			int i;
			for (i=0;i<mNumMeas;i++)
			{		
				mMeasPoints[i].sCell = atoi(r[i]["ci"].c_str());	
				mMeasPoints[i].sID = atoi(r[i]["id"].c_str());
				mMeasPoints[i].sInstKeyMobile = atoi(r[i]["mobile"].c_str());
				PointString = r[i]["location"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				mMeasPoints[i].sPoint.Set(latitude,longitude,DEG);
				mMeasPoints[i].sMeasValue = atof(r[i]["measvalue"].c_str());
				mMeasPoints[i].sPredValue = atof(r[i]["predictvalue"].c_str());
				mMeasPoints[i].sPathLoss = atof(r[i]["pathloss"].c_str());
				mMeasPoints[i].sDistance = atof(r[i]["distance"].c_str());
				mMeasPoints[i].sAzimuth = atof(r[i]["azimuth"].c_str());
				mMeasPoints[i].sTilt = atof(r[i]["tilt"].c_str());
				mMeasPoints[i].sClutter = 0;

				
				if (mMeasPoints[i].sInstKeyMobile != currentMobile)
				{
					tempMobile.sInstKey =mMeasPoints[i].sInstKeyMobile;
					currentMobile = mMeasPoints[i].sInstKeyMobile;
					query = "SELECT eirp,txpower,txlosses,rxlosses,rxsensitivity,";
					query += " antpatternkey,mobileheight ";
					query += "FROM mobile CROSS JOIN technology ";
					query += "WHERE mobile.techkey=technology.id AND mobile.id=";
					gcvt(mMeasPoints[i].sInstKeyMobile,8,text);
					query += text;
					query += ";";
	
					if(!gDb.PerformRawSql(query))
					{
						string err = "Error in Database Select on tables mobile and technology. Query:";
						err+=query; 
						QRAP_ERROR(err.c_str());
						return false;
					}
					else
					{
						gDb.GetLastResult(rMobile);
		
						if(rMobile.size()!=0)
						{
							tempMobile.sEIRP = atof(rMobile[0]["eirp"].c_str());
							tempMobile.sTxPower = atof(rMobile[0]["txpower"].c_str());
							tempMobile.sTxSysLoss = atof(rMobile[0]["txlosses"].c_str());
							tempMobile.sRxSysLoss = atof(rMobile[0]["rxlosses"].c_str());
							tempMobile.sRxSens = atof(rMobile[0]["rxsensitivity"].c_str());
							tempMobile.sPatternKey = atoi(rMobile[0]["antpatternkey"].c_str());
							tempMobile.sMobileHeight = atof(rMobile[0]["mobileheight"].c_str());
						} // if rMobile.size()
					} // else !gDb->PerformRawSq
					mMobiles.push_back(tempMobile);	
				}
				
				if (mMeasPoints[i].sCell==currentCI)
					mMeasPoints[i].sInstKeyFixed = currentFixedInst;
				else
				{
					gcvt(mMeasPoints[i].sCell,8,text);
					query = "SELECT risector, siteid, AsText(location) as location, eirp,";
					query += "radioinstallation.txpower as txpower,txlosses,rxlosses";
					query +=",rxsensitivity,txantpatternkey,txbearing,txmechtilt,rxantpatternkey,";
					query += "rxbearing,rxmechtilt,txantennaheight,rxantennaheight ";
					query += "FROM cell CROSS JOIN radioinstallation CROSS JOIN site ";
					query += "WHERE risector=radioinstallation.id and siteid=site.id ";
					query += " and cell.id="; 
					query += text;
					query += ";";
				
					// Perform a Raw SQL query
					if(!gDb.PerformRawSql(query))
					{
						string err = "Error in Database Select on tables radioinstallation ";
						err += "and technology failed. Query:";
						err+=query; 
						QRAP_ERROR(err.c_str());
						return false;
					} // if
					else
					{
						gDb.GetLastResult(rFixed);
						if(rFixed.size()!=0)
						{
							
							currentFixedInst = atoi(rFixed[0]["risector"].c_str());
							mMeasPoints[i].sInstKeyFixed = currentFixedInst;
							tempInst.sFrequency = atof(r[i]["frequency"].c_str());

							tempInst.sInstKey = currentFixedInst;
							tempInst.sSiteID = atoi(rFixed[0]["siteid"].c_str());
							PointString = rFixed[0]["location"].c_str();
							spacePos = PointString.find_first_of(' ');
							longitude = atof((PointString.substr(6,spacePos).c_str())); 
							latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
							tempInst.sSitePos.Set(latitude,longitude,DEG);
							tempInst.sEIRP = atof(rFixed[0]["eirp"].c_str());
							tempInst.sTxPower = atof(rFixed[0]["txpower"].c_str());
							tempInst.sTxSysLoss = atof(rFixed[0]["txlosses"].c_str());
							tempInst.sRxSysLoss = atof(rFixed[0]["rxlosses"].c_str());
							tempInst.sRxSens = atof(rFixed[0]["rxsensitivity"].c_str());
							tempInst.sTxPatternKey = atoi(rFixed[0]["txantpatternkey"].c_str());
							tempInst.sTxAzimuth = atof(rFixed[0]["txbearing"].c_str());
							tempInst.sTxMechTilt = atof(rFixed[0]["txmechtilt"].c_str());
							tempInst.sRxPatternKey = atoi(rFixed[0]["rxantpatternkey"].c_str());
							tempInst.sRxAzimuth = atof(rFixed[0]["rxbearing"].c_str());
							tempInst.sRxMechTilt = atof(rFixed[0]["rxmechtilt"].c_str());
							tempInst.sTxHeight = atof(rFixed[0]["txantennaheight"].c_str());
							tempInst.sRxHeight = atof(rFixed[0]["rxantennaheight"].c_str());

							mFixedInsts.push_back(tempInst);
						} // if the query was not empty
							
					}// else ... hence the query was successful
				}// else ... hence new installation that must be loaded.
			}//for all measurements
		}// if there are measurements
	} // else ... hence the query was successful
	
	cout << "cMeasAnalysisCalc::LoadMeasurement: leaving ";
}


//******************************************************************************
int cMeasAnalysisCalc::PerformAnalysis(double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, unsigned Clutterfilter)
{

	mClutterFilter = Clutterfilter;
	unsigned i=0, j=0, MobileNum=0, FixedNum=0 ,NumUsed = 0;
	double Error=0, TotalError=0, TotalSError=0, TotalMeas=0;
	double TotalPred=0, TotalSMeas=0, TotalSPred=0, TotalMeasPred=0;
	unsigned currentInst=0;
	unsigned currentMobile=0;
	cAntennaPattern FixedAnt, MobileAnt;
	
	unsigned Length;
	cProfile Clutter;
	cProfile DEM;
	double bearing=0, AntValue=0, EIRP=0;

	//These varaibles are local and are such that they can be used with the TERMs defined in cClutter.h
	double mLinkLength, m_freq, m_htx, Cheight;

	mSolveCoefMatrix.resize(NUMTERMS,NUMTERMS);
	mLeftSide.resize(NUMTERMS);
	mDeltaCoeff.resize(NUMTERMS);
	if ((Clutterfilter>0)&&(mUseClutter))
	{
		for (i=0;i<NUMTERMS;i++)
		{
			mMinTerm[i] =   MAXDOUBLE;
			mMaxTerm[i] = - MAXDOUBLE;
			mMaxTerm[i] =   MAXDOUBLE;
			mLeftSide(i) = 0.0;
			mDeltaCoeff(i) = 0.0;
			for (j=0; j<NUMTERMS; j++)
				mSolveCoefMatrix(i,j)=0.0;
		}
		
	}

	for (i=0;i<mNumMeas;i++)
	{
		if ((0==Clutterfilter)||(0==mMeasPoints[i].sClutter)||(Clutterfilter==mMeasPoints[i].sClutter))
		{
			NumUsed++;

			//Change settings if the mobile installation changed
			if (mMeasPoints[i].sInstKeyMobile!=currentMobile)
			{
				currentMobile = mMeasPoints[i].sInstKeyMobile;
				while ((mMobiles[MobileNum].sInstKey!=currentMobile)&&(MobileNum < mMobiles.size()))
					MobileNum++;
				if (MobileNum == mMobiles.size())
					return 0;
				MobileAnt.SetAntennaPattern(mMobiles[MobileNum].sPatternKey, 0, 0);
				mPathLoss.setParameters(mkFactor,mFixedInsts[FixedNum].sFrequency,
								mFixedInsts[FixedNum].sTxHeight,
								mMobiles[MobileNum].sMobileHeight,
								mUseClutter, mClutterClassGroup);
			}

			//Change settings if the Fixed Installation changed
			if (mMeasPoints[i].sInstKeyFixed!=currentInst)
			{
				currentInst = mMeasPoints[i].sInstKeyFixed;
				while ((mFixedInsts[FixedNum].sInstKey!=currentInst)&&(FixedNum < mFixedInsts.size()))
					FixedNum++;
				if (FixedNum == mFixedInsts.size())
					return 0;
				FixedAnt.SetAntennaPattern(mFixedInsts[FixedNum].sTxPatternKey, 
								mFixedInsts[FixedNum].sTxAzimuth,  
								mFixedInsts[FixedNum].sTxMechTilt);
				EIRP = mFixedInsts[FixedNum].sTxPower 
					- mFixedInsts[FixedNum].sTxSysLoss + FixedAnt.mGain + MobileAnt.mGain;
				mPathLoss.setParameters(mkFactor,mFixedInsts[FixedNum].sFrequency,
								mFixedInsts[FixedNum].sTxHeight,
								mMobiles[MobileNum].sMobileHeight,
								mUseClutter, mClutterClassGroup);
				if ((Clutterfilter>0)&&(mUseClutter))
				{
					m_freq = mFixedInsts[FixedNum].sFrequency;
					m_htx = mFixedInsts[FixedNum].sTxHeight;

					if (TERM2 < mMinTerm[2])
					{
						if ((mMidTerm[2]-TERM2) > (mMaxTerm[2]-mMidTerm[2]))
							mMidTerm[2] = mMinTerm[2];
						mMinTerm[2] = TERM2;
					}
					else if (TERM2 > mMaxTerm[2])
					{
						if ((mMidTerm[2]-TERM2) < (mMaxTerm[2]-mMidTerm[2]))
							mMidTerm[2] = mMaxTerm[2];
						mMaxTerm[2] = TERM2;
					}

					if (TERM3 < mMinTerm[3])
					{
						if ((mMidTerm[3]-TERM3) > (mMaxTerm[3]-mMidTerm[3]))
							mMidTerm[3] = mMinTerm[3];
						mMinTerm[3] = TERM3;
					}
					else if (TERM3 > mMaxTerm[3])
					{
						if ((mMidTerm[3]-TERM3) < (mMaxTerm[3]-mMidTerm[3]))
							mMidTerm[3] = mMaxTerm[3];
						mMaxTerm[3] = TERM3;
					}

					if (TERM5 < mMinTerm[5])
					{
						if ((mMidTerm[5]-TERM5) > (mMaxTerm[5]-mMidTerm[5]))
							mMidTerm[5] = mMinTerm[5];
						mMinTerm[5] = TERM5;
					}
					else if (TERM5 > mMaxTerm[5])
					{
						if ((mMidTerm[5]-TERM5) < (mMaxTerm[5]-mMidTerm[5]))
							mMidTerm[5] = mMaxTerm[5];
						mMaxTerm[5] = TERM5;
					}

					if (TERM6 < mMinTerm[6])
					{
						if ((mMidTerm[6]-TERM6) > (mMaxTerm[6]-mMidTerm[6]))
							mMidTerm[6] = mMinTerm[6];
						mMinTerm[6] = TERM6;
					}
					else if (TERM6 > mMaxTerm[6])
					{
						if ((mMidTerm[6]-TERM6) < (mMaxTerm[6]-mMidTerm[6]))
							mMidTerm[6] = mMaxTerm[6];
						mMaxTerm[6] = TERM6;
					}

					if (TERM7 < mMinTerm[7])
					{
						if ((mMidTerm[7]-TERM7) > (mMaxTerm[7]-mMidTerm[7]))
							mMidTerm[7] = mMinTerm[7];
						mMinTerm[7] = TERM7;
					}
					else if (TERM7 > mMaxTerm[7])
					{
						if ((mMidTerm[7]-TERM7) < (mMaxTerm[7]-mMidTerm[7]))
							mMidTerm[7] = mMaxTerm[7];
						mMaxTerm[7] = TERM7;
					}
				}
			}
			

			DEM = mDEM.GetForLink(mFixedInsts[FixedNum].sSitePos,mMeasPoints[i].sPoint,mPlotResolution);
			mMeasPoints[i].sDistance = mFixedInsts[FixedNum].sSitePos.Distance(mMeasPoints[i].sPoint);
			Length = DEM.GetSize();
	
			if (Length > 2)
			{
				if (mUseClutter)
				{
					Clutter=mClutter.GetForLink(mFixedInsts[FixedNum].sSitePos,mMeasPoints[i].sPoint,mPlotResolution);
					mMeasPoints[i].sClutter = (int)Clutter.GetLastValue();
				}
				mMeasPoints[i].sPathLoss = mPathLoss.TotPathLoss(DEM,mMeasPoints[i].sTilt,Clutter);
				mMeasPoints[i].sAzimuth = mFixedInsts[FixedNum].sSitePos.Bearing(mMeasPoints[i].sPoint);
				AntValue = FixedAnt.GetPatternValue(mMeasPoints[i].sAzimuth, mMeasPoints[i].sTilt)
						+ MobileAnt.GetPatternValue(0, -mMeasPoints[i].sTilt);
				mMeasPoints[i].sPredValue = -mMeasPoints[i].sPathLoss + EIRP - AntValue;
//				cout << endl << mMeasPoints[i].sMeasValue << "	" << mMeasPoints[i].sPredValue << endl << endl;

				Error = - mMeasPoints[i].sMeasValue + mMeasPoints[i].sPredValue;
				TotalError += Error;  
				TotalSError += (mMeasPoints[i].sMeasValue - mMeasPoints[i].sPredValue)*
							(mMeasPoints[i].sMeasValue - mMeasPoints[i].sPredValue);
				TotalMeas += mMeasPoints[i].sMeasValue; 
				TotalSMeas += mMeasPoints[i].sMeasValue*mMeasPoints[i].sMeasValue, 
				TotalPred += mMeasPoints[i].sPredValue;
				TotalSPred += mMeasPoints[i].sPredValue*mMeasPoints[i].sPredValue;				
				TotalMeasPred+= mMeasPoints[i].sPredValue*mMeasPoints[i].sMeasValue;
				
				if ((Clutterfilter > 0)&&(mUseClutter))
				{
					mLinkLength = mMeasPoints[i].sDistance;
					Cheight = mPathLoss.mClutter.mClutterTypes[mMeasPoints[i].sClutter].sHeight;
					
					if (TERM1 < mMinTerm[1])
					{
						if ((mMidTerm[1]-TERM1) > (mMaxTerm[1]-mMidTerm[1]))
							mMidTerm[1] = mMinTerm[1];
						mMinTerm[1] = TERM1;
					}
					else if (TERM1 > mMaxTerm[1])
					{
						if ((mMidTerm[1]-TERM1) < (mMaxTerm[1]-mMidTerm[1]))
							mMidTerm[1] = mMaxTerm[1];
						mMaxTerm[1] = TERM1;
					}

					if (TERM4 < mMinTerm[4])
					{
						if ((mMidTerm[4]-TERM4) > (mMaxTerm[4]-mMidTerm[4]))
							mMidTerm[4] = mMinTerm[4];
						mMinTerm[4] = TERM4;
					}
					else if (TERM4 > mMaxTerm[4])
					{
						if ((mMidTerm[4]-TERM4) < (mMaxTerm[4]-mMidTerm[4]))
							mMidTerm[4] = mMaxTerm[4];
						mMaxTerm[4] = TERM4;
					}

					if (TERM8 < mMinTerm[8])
					{
						if ((mMidTerm[8]-TERM8) > (mMaxTerm[8]-mMidTerm[8]))
							mMidTerm[8] = mMinTerm[8];
						mMinTerm[8] = TERM8;
					}
					else if (TERM8 > mMaxTerm[8])
					{
						if ((mMidTerm[8]-TERM8) < (mMaxTerm[8]-mMidTerm[8]))
							mMidTerm[8] = mMaxTerm[8];
						mMaxTerm[8] = TERM8;
					}

					mLeftSide(0) += Error;
					mSolveCoefMatrix(0,0)++;
					mSolveCoefMatrix(0,1)+=TERM1;
					mSolveCoefMatrix(1,0)+=TERM1;
					mSolveCoefMatrix(0,2)+=TERM2;
					mSolveCoefMatrix(2,0)+=TERM2;
					mSolveCoefMatrix(0,3)+=TERM3;
					mSolveCoefMatrix(3,0)+=TERM3;
					mSolveCoefMatrix(0,4)+=TERM4;
					mSolveCoefMatrix(4,0)+=TERM4;
					mSolveCoefMatrix(0,5)+=TERM5;
					mSolveCoefMatrix(5,0)+=TERM5;
					mSolveCoefMatrix(0,6)+=TERM6;
					mSolveCoefMatrix(6,0)+=TERM6;
					mSolveCoefMatrix(0,7)+=TERM7;
					mSolveCoefMatrix(7,0)+=TERM7;
					mSolveCoefMatrix(0,8)+=TERM8;
					mSolveCoefMatrix(8,0)+=TERM8;

					mLeftSide(1) += Error*TERM1;
					mSolveCoefMatrix(1,1)+=TERM1*TERM1;
					mSolveCoefMatrix(1,2)+=TERM1*TERM2;
					mSolveCoefMatrix(2,1)+=TERM1*TERM2;
					mSolveCoefMatrix(1,3)+=TERM1*TERM3;
					mSolveCoefMatrix(3,1)+=TERM1*TERM3;
					mSolveCoefMatrix(1,4)+=TERM1*TERM4;
					mSolveCoefMatrix(4,1)+=TERM1*TERM4;
					mSolveCoefMatrix(1,5)+=TERM1*TERM5;
					mSolveCoefMatrix(5,1)+=TERM1*TERM5;
					mSolveCoefMatrix(1,6)+=TERM1*TERM6;
					mSolveCoefMatrix(6,1)+=TERM1*TERM6;
					mSolveCoefMatrix(1,7)+=TERM1*TERM7;
					mSolveCoefMatrix(7,1)+=TERM1*TERM7;
					mSolveCoefMatrix(1,8)+=TERM1*TERM8;
					mSolveCoefMatrix(8,1)+=TERM1*TERM8;

					mLeftSide(2) += Error*TERM2;
					mSolveCoefMatrix(2,2)+=TERM2*TERM2;
					mSolveCoefMatrix(2,3)+=TERM2*TERM3;
					mSolveCoefMatrix(3,2)+=TERM2*TERM3;
					mSolveCoefMatrix(2,4)+=TERM2*TERM4;
					mSolveCoefMatrix(4,2)+=TERM2*TERM4;
					mSolveCoefMatrix(2,5)+=TERM2*TERM5;
					mSolveCoefMatrix(5,2)+=TERM2*TERM5;
					mSolveCoefMatrix(2,6)+=TERM2*TERM6;
					mSolveCoefMatrix(6,2)+=TERM2*TERM6;
					mSolveCoefMatrix(2,7)+=TERM2*TERM7;
					mSolveCoefMatrix(7,2)+=TERM2*TERM7;
					mSolveCoefMatrix(2,8)+=TERM2*TERM8;
					mSolveCoefMatrix(8,2)+=TERM2*TERM8;

					mLeftSide(3) += Error*TERM3;
					mSolveCoefMatrix(3,3)+=TERM3*TERM3;
					mSolveCoefMatrix(3,4)+=TERM3*TERM4;
					mSolveCoefMatrix(4,3)+=TERM3*TERM4;
					mSolveCoefMatrix(3,5)+=TERM3*TERM5;
					mSolveCoefMatrix(5,3)+=TERM3*TERM5;
					mSolveCoefMatrix(3,6)+=TERM3*TERM6;
					mSolveCoefMatrix(6,3)+=TERM3*TERM6;
					mSolveCoefMatrix(3,7)+=TERM3*TERM7;
					mSolveCoefMatrix(7,3)+=TERM3*TERM7;
					mSolveCoefMatrix(3,8)+=TERM3*TERM8;
					mSolveCoefMatrix(8,3)+=TERM3*TERM8;

					mLeftSide(4) += Error*TERM4;
					mSolveCoefMatrix(4,4)+=TERM4*TERM4;
					mSolveCoefMatrix(4,5)+=TERM4*TERM5;
					mSolveCoefMatrix(5,4)+=TERM4*TERM5;
					mSolveCoefMatrix(4,6)+=TERM4*TERM6;
					mSolveCoefMatrix(6,4)+=TERM4*TERM6;
					mSolveCoefMatrix(4,7)+=TERM4*TERM7;
					mSolveCoefMatrix(7,4)+=TERM4*TERM7;
					mSolveCoefMatrix(4,8)+=TERM4*TERM8;
					mSolveCoefMatrix(8,4)+=TERM4*TERM8;

					mLeftSide(5) += Error*TERM5;
					mSolveCoefMatrix(5,5)+=TERM5*TERM5;
					mSolveCoefMatrix(5,6)+=TERM5*TERM6;
					mSolveCoefMatrix(6,5)+=TERM5*TERM6;
					mSolveCoefMatrix(5,7)+=TERM5*TERM7;
					mSolveCoefMatrix(7,5)+=TERM5*TERM7;
					mSolveCoefMatrix(5,8)+=TERM5*TERM8;
					mSolveCoefMatrix(8,5)+=TERM5*TERM8;

					mLeftSide(6) += Error*TERM6;
					mSolveCoefMatrix(6,6)+=TERM6*TERM6;
					mSolveCoefMatrix(6,7)+=TERM6*TERM7;
					mSolveCoefMatrix(7,6)+=TERM6*TERM7;
					mSolveCoefMatrix(6,8)+=TERM6*TERM8;
					mSolveCoefMatrix(8,6)+=TERM6*TERM8;

					mLeftSide(7) += Error*TERM7;
					mSolveCoefMatrix(7,7)+=TERM7*TERM7;
					mSolveCoefMatrix(7,8)+=TERM7*TERM8;
					mSolveCoefMatrix(8,7)+=TERM7*TERM8;
					
					mLeftSide(8) += Error*TERM8;
					mSolveCoefMatrix(8,8)+=TERM8*TERM8;
				}
				

			}
			else NumUsed--;

		}// if included in calculations
	}//for all measurements

	Mean = TotalError/NumUsed;
	MeanSquareError = TotalSError/NumUsed;
	StDev = sqrt(MeanSquareError-Mean*Mean);
	
	double StDevMeas = sqrt(TotalSMeas/NumUsed-TotalMeas*TotalMeas/(NumUsed*NumUsed));
	double StDevPred = sqrt(TotalSPred/NumUsed-TotalPred*TotalPred/(NumUsed*NumUsed));
	CorrC = (TotalMeasPred - TotalMeas*TotalPred/NumUsed) / ((NumUsed-1)*StDevMeas*StDevPred);

	return NumUsed;
}


//************************************************************************************************************************************
int cMeasAnalysisCalc::SaveResults()
{
	int i;
	string query;
	string queryM = "UPDATE measurement SET predictvalue=";
	char * temp;
	temp = new char[33];

    	string line, TimeS, LonS, LatS, RSSIS, MERS;
	QString PosString;

	for (i=0; i<mNumMeas; i++)
	{
		query = queryM;
		gcvt(mMeasPoints[i].sPredValue,9,temp);
		query += temp;
		query += ", tilt=";
		gcvt(mMeasPoints[i].sTilt,9,temp);
		query += temp;
		query += ", azimuth=";
		gcvt(mMeasPoints[i].sAzimuth,9,temp);
		query += temp;
		query += ", pathloss=";
		gcvt(mMeasPoints[i].sPathLoss,9,temp);
		query += temp;
		query += ", distance=";
		gcvt(mMeasPoints[i].sDistance,9,temp);
		query += temp;
		query += " where id=";
		gcvt(mMeasPoints[i].sID,9,temp);
		query += temp;
		query += ";";
		if (!gDb.PerformRawSql(query))
		{
			cout << "cMeasImportCSV::LoadMeasurement: Error inserting the Measurement!: " << endl;
			cout << query << endl;
			string err = "Error inserting Measurement by running query: ";
			err += query;
			QRAP_WARN(err.c_str());
			return 0;
		}
	}
	delete [] temp;

	query = "create view meas_view as select measurement.id, location, measvalue, predictvalue, ";
	query += "(measvalue-predictvalue) as error, pathloss, ci, distance, azimuth, tilt, frequency ";
	query += "from measurement cross join testpoint where tp=testpoint.id;";
		
	if (gDb.ViewExists("meas_view"))
	{
		string drop = "drop view meas_view cascade";
		if (!gDb.PerformRawSql(drop))
		{
			cout << "Error dropping meas_view in cMeasAnalysisCalc::SaveResults()" << endl;
			QRAP_ERROR("Error dropping meas_view in cMeasAnalysisCalc::SaveResults()");
		}
	}
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error creating meas_view in cMeasAnalysisCalc::SaveResults()" << endl;
		QRAP_ERROR("Error creating meas_view in cMeasAnalysisCalc::SaveResults()");
	}
	cout << " leaving cMeasAnalysisCalc::SaveResults()" << endl;
}


//*************************************************************************************************
//*
bool cMeasAnalysisCalc::Optimiser(bool ChangeHeights)
{
	unsigned i;
	for (i=0; i<NUMTERMS; i++)
		mPathLoss.mClutter.mClutterTypes[mClutterFilter].sAllowCchange[i] = ((mMaxTerm[i]-mMinTerm[i]) > 0.1*fabs(mMidTerm[i]));

	mPathLoss.mClutter.mClutterTypes[mClutterFilter].sAllowCchange[5] = ((mMaxTerm[5] - mMidTerm[5]) > 0.1*fabs(mMidTerm[5])) &&
								((mMidTerm[5] - mMinTerm[5]) > 0.1*fabs(mMidTerm[5]));
	mPathLoss.mClutter.mClutterTypes[mClutterFilter].sAllowCchange[7] = ((mMaxTerm[7] - mMidTerm[7]) > 0.1*fabs(mMidTerm[7])) &&
								((mMidTerm[7] - mMinTerm[7]) > 0.1*fabs(mMidTerm[7]));

	for (i=0; i<NUMTERMS; i++)
	{
		if (!mPathLoss.mClutter.mClutterTypes[mClutterFilter].sAllowCchange[i])
		{
// skuif ry en kolom een op
		}
	} 
}


