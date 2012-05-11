 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasAnalysisCalc.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class allows for the impoting of CW measurements 
 *                  in a space delimited format
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
	mNumMeas = 0;	
	mUnits = dBm;
	string setting;
	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		mkFactor=atof(setting.c_str());
	else mkFactor = 1.25;
	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
		mPlotResolution = atof(setting.c_str());
	else mPlotResolution = 90;
	setting = gDb.GetSetting("UseClutter");
	if (setting=="true")
		mUseClutter = true;
	else mUseClutter = false;
	mClutterSource = atoi(gDb.GetSetting("ClutterSource").c_str());
	if (mUseClutter)
		mClutter.SetRasterFileRules(mClutterSource);	
	mDEMsource = atoi(gDb.GetSetting("DEMsource").c_str());
	mDEM.SetRasterFileRules(mDEMsource);
}

//*********************************************************************
cMeasAnalysisCalc::~cMeasAnalysisCalc() // destructor
{
	delete [] mMeasPoints;		
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
int cMeasAnalysisCalc::PerformAnalysis(double &Mean, double &StDev, double &CorrC, unsigned Clutterfilter)
{
	int i=0, MobileNum=0, FixedNum=0 ,NumUsed = 0;
	double TotalError=0, TotalSError=0, TotalMeas=0, TotalPred=0, TotalSMeas=0, TotalSPred=0, TotalMeasPred=0;
	int currentInst=0;
	int currentMobile=0;
	cAntennaPattern FixedAnt, MobileAnt;
	
	int Length;
	cProfile Clutter;
	cProfile DEM;
	cPathLossPredictor PathLoss;
	double bearing=0, AntValue=0, EIRP=0;
	
	for (i=0;i<mNumMeas;i++)
	{
		if ((0==Clutterfilter)||(0==mMeasPoints[i].sClutter)||(Clutterfilter==mMeasPoints[i].sClutter))
		{
			NumUsed++;
			
			if (mMeasPoints[i].sInstKeyMobile!=currentMobile)
			{
				currentMobile = mMeasPoints[i].sInstKeyMobile;
				while ((mMobiles[MobileNum].sInstKey!=currentMobile)&&(MobileNum < mMobiles.size()))
					MobileNum++;
				if (MobileNum == mMobiles.size())
					return 0;
				MobileAnt.SetAntennaPattern(mMobiles[MobileNum].sPatternKey, 0, 0);
				PathLoss.setParameters(mkFactor,mFixedInsts[FixedNum].sFrequency,
								mFixedInsts[FixedNum].sTxHeight,
								mMobiles[MobileNum].sMobileHeight);
			}

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
				PathLoss.setParameters(mkFactor,mFixedInsts[FixedNum].sFrequency,
								mFixedInsts[FixedNum].sTxHeight,
								mMobiles[MobileNum].sMobileHeight);
			}
			

			if (mUseClutter)
			{
				Clutter=mClutter.GetForLink(mFixedInsts[FixedNum].sSitePos,mMeasPoints[i].sPoint,mPlotResolution);
//				mMeasPoints[i].sClutter = Clutter[Clutter.GetSize()-1];
			}
			DEM = mDEM.GetForLink(mFixedInsts[FixedNum].sSitePos,mMeasPoints[i].sPoint,mPlotResolution);
			mMeasPoints[i].sDistance = mFixedInsts[FixedNum].sSitePos.Distance(mMeasPoints[i].sPoint);
			Length = DEM.GetSize();

	
			if (Length > 1)
			{
				mMeasPoints[i].sPathLoss = PathLoss.TotPathLoss(DEM,mMeasPoints[i].sTilt,mUseClutter,Clutter);
				mMeasPoints[i].sAzimuth = mFixedInsts[FixedNum].sSitePos.Bearing(mMeasPoints[i].sPoint);
				AntValue = FixedAnt.GetPatternValue(mMeasPoints[i].sAzimuth, mMeasPoints[i].sTilt)
						+ MobileAnt.GetPatternValue(0, -mMeasPoints[i].sTilt);
				mMeasPoints[i].sPredValue = -mMeasPoints[i].sPathLoss + EIRP - AntValue;
				cout << mMeasPoints[i].sMeasValue << "	" << mMeasPoints[i].sPredValue << endl;
//				if (mMeasPoints[i].sPredValue > -115)
//				{
					TotalError +=  - mMeasPoints[i].sMeasValue + mMeasPoints[i].sPredValue; 
					TotalSError += (mMeasPoints[i].sMeasValue - mMeasPoints[i].sPredValue)*
							(mMeasPoints[i].sMeasValue - mMeasPoints[i].sPredValue);
					TotalMeas += mMeasPoints[i].sMeasValue; 
					TotalSMeas += mMeasPoints[i].sMeasValue*mMeasPoints[i].sMeasValue, 
					TotalPred += mMeasPoints[i].sPredValue;
					TotalSPred += mMeasPoints[i].sPredValue*mMeasPoints[i].sPredValue;				
					TotalMeasPred+= mMeasPoints[i].sPredValue*mMeasPoints[i].sMeasValue;
//				}
//				else NumUsed--;
			}
			else NumUsed--;

		}// if included in calculations
	}//for all measurements

	Mean = TotalError/NumUsed;
	StDev = sqrt(TotalSError/NumUsed-Mean*Mean);
	double StDevMeas = sqrt(TotalSMeas/NumUsed-TotalMeas*TotalMeas/(NumUsed*NumUsed));
	double StDevPred = sqrt(TotalSPred/NumUsed-TotalPred*TotalPred/(NumUsed*NumUsed));
	CorrC = (TotalMeasPred - TotalMeas*TotalPred/NumUsed) / ((NumUsed-1)*StDevMeas*StDevPred);

	return NumUsed;
}

