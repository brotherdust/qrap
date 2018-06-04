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

#include "cTrafficDist.h"

using namespace Qrap;
using namespace std;

//************************************************************************
cTrafficDist::cTrafficDist()
{
	mUnits = dBm;
	mInstCounter =0;
	mRqSN = 8;				//dB
	mRxMin = -110;			//dBm
	mFadeMargin = 3;		//dB
	mRqEbNo = 8;			//dB
	mNoiseLevel = 6;		//dBm
	mkFactorServ = 1.33;
	mDEMsource = 1;
	mDownlink = true;
	mClutterSource = 1;
	mUseClutter = false;
	mPlotResolution = 90;
	mMinAngleRes = 1;
	mNumAngles = 360;
	mMobile.sInstKey = 0;
	mMobile.sEIRP = 33;
	mMobile.sTxPower = 2;
	mMobile.sTxSysLoss = 0;
	mMobile.sRxSysLoss = 0;
	mMobile.sRxSens = -104;
	mMobile.sPatternKey = 0;
	mMobile.sMobileHeight = 1;
	mPlot = new_Float2DArray(2,2);
	mSupportPlot = new_Float2DArray(2,2);
	mCSweights = new double[2];
	mPSweights = new double[2];
	mDir = "/home/maggie/Data/qrap/Predictions/";
	mOutputFile = "Output";	
	mClutterDist = new_Float2DArray(2,2); 
}

//**************************************************************************
cTrafficDist::~cTrafficDist()
{
	delete [] mCSweights;
	delete [] mPSweights;
	delete_Float2DArray(mClutterDist);
	delete_Float2DArray(mPlot);
	delete_Float2DArray(mSupportPlot);
	mFixedInsts.clear();
	for (unsigned i=0; i<mActiveRasters.size(); i++)
		delete_Float2DArray(mActiveRasters[i].sRaster);
	mActiveRasters.clear();
}

//**************************************************************************
bool cTrafficDist::SetTrafficDist(eOutputUnits DisplayUnits,
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
								cGeoP SouthEastCorner,
								double PlotResolution,
								double MinimumAngularResolution,
								unsigned MobileInstallationKey,
								double CoverangeRange, // In Kilometer
								string DirectoryToStoreResult,
								string OutputFileForResult)
{
	string err = "Preparing for Requested Prediction.";
//	QRAP_INFO(err.c_str());
	cout << err << endl;
	mUnits 		= DisplayUnits;							
	mDownlink 	= Downlink;							
	mRqSN 		= RequiredSignalToNoise;							
	mRxMin 		= RequiredMinimumReceiverLevel;							
	mFadeMargin	= FadeMargin;							
	mRqEbNo 	= RequiredEnergyPerBitToNoiseRatio;							
	mNoiseLevel	= NoiseLevel;		
	mkFactorServ= kFactorForServers;						
	mDEMsource	= DEMsourceList;				
	mClutterSource  = ClutterSourceList;					
	mUseClutter	= UseClutterDataInPathLossCalculations;							
	mNorthWest	= NorthWestCorner;				
	mSouthEast	= SouthEastCorner;
	double N, W, S, E;
	cout << "North West corner: " << endl;
	NorthWestCorner.Display();
	cout << "South East corner: " << endl;
	SouthEastCorner.Display();
	mNorthWest.Get(N,W);
	mSouthEast.Get(S,E);
	mSouthWest.Set(S,W);
	mNorthEast.Set(N,E);
	mPlotResolution=PlotResolution;					
	mMinAngleRes=MinimumAngularResolution;			
	mNumAngles = (unsigned)(360.0/mMinAngleRes);
	mMobile.sInstKey=MobileInstallationKey;				
	mMaxCoverageRange = CoverangeRange;
	
	mMaxRange=0;

	if (mMaxRange>0)
	{
		if (mMinAngleRes<(270*mPlotResolution/mMaxRange/PI))
		{
			if (mPlotResolution<0.5) mPlotResolution=20; 
			int mNumAngles = (int)ceil(1.5*PI*mMaxRange/mPlotResolution);
			mMinAngleRes=360.0/(double)mNumAngles;
		}
	}
	if (mMinAngleRes<0.0001)
	{
		mMinAngleRes=1;
		mNumAngles = 360;
	}
	mNumAngles = (unsigned)(360.0/mMinAngleRes);
	if (mNumAngles<4)
	{
		mMinAngleRes=20;
		mNumAngles = 18;		
	}
	
	mDir=DirectoryToStoreResult;			
	mOutputFile=OutputFileForResult;	
//	double WE = max(mNorthWest.Distance(mNorthEast), 
//					mSouthWest.Distance(mSouthEast));
//	mRows = (int)ceil(mNorthWest.Distance(mSouthWest)/mPlotResolution);
//	mCols = (int)ceil(WE/mPlotResolution);
	mNSres = mPlotResolution*cDegResT;
	mEWres = mPlotResolution*cDegResT;
	mRows = (unsigned)((N-S)/(mNSres));
	mCols = (unsigned)((E-W)/(mEWres));
	delete_Float2DArray(mPlot);
	delete_Float2DArray(mSupportPlot);
	mPlot = new_Float2DArray(mRows,mCols);
	mSupportPlot = new_Float2DArray(mRows,mCols);
	cGeoP MidNorth(N,(W+E)/2, DEG);
	cGeoP Mid((N+S)/2,(E+W)/2,DEG);
	mCentMer = Mid.DefaultCentMer(WGS84GC);
//	mNorthWest.FromHere(MidNorth,mCols/2.0*mPlotResolution,270.0);
	double dummy;
	if (mCols<mRows)
		mNorthWest.Get(mCurrentEdge, dummy);
	else 
		mNorthWest.Get(dummy, mCurrentEdge);
	bool FoundRasterSet = mDEM.SetRasterFileRules(mDEMsource);
	if (!FoundRasterSet)
	{
		string err = "Trouble getting DEM list. Using default";
		QRAP_WARN(err.c_str());
	}
	if (mUseClutter)
	{
		FoundRasterSet = mClutter.SetRasterFileRules(mClutterSource);
		mUseClutter = (FoundRasterSet)&&(UseClutterDataInPathLossCalculations);
		if (!FoundRasterSet)
		{
			string err ="  Trouble getting Clutter list. Not using clutter.";
			QRAP_WARN(err.c_str());
			cout << "  Trouble getting Clutter list. Not using clutter."<< endl;
			cout << "  cTrafficDist::SetTrafficDist:  This makes no sense without clutter." << endl; 
			return false;
		}
	}
	if (mUseClutter)
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	mUseClutter = (mUseClutter) && (mClutterClassGroup>0);
	
	mClutterUsed.Reset(mClutterClassGroup);

	cout << "North West corner: " << endl;
	NorthWestCorner.Display();
	cout << "South East corner: " << endl;
	SouthEastCorner.Display();

	bool ReturnValue = GetDBinfo();
	return ReturnValue;
}// end cTrafficDist::SetTrafficDist



//***************************************************************************
bool cTrafficDist::GetDBinfo()
{
	pqxx::result r, rFixed;
	string query;
	char temp[33];
	unsigned i,j;
	double dloffset, upoffset, chansep; 
	query = "SELECT eirp,txpower,txlosses,rxlosses,rxsensitivity,antpatternkey,mobileheight,btlfreq,maxpathloss ";
	query += "FROM mobile CROSS JOIN technology ";
	query += "WHERE mobile.techkey=technology.id AND mobile.id=";
	gcvt(mMobile.sInstKey,8,temp);
	query += temp;
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
		gDb.GetLastResult(r);
		
		if(r.size()!=0)
		{
			mMobile.sEIRP = atof(r[0]["eirp"].c_str());
			mMobile.sTxPower = atof(r[0]["txpower"].c_str());
			mMobile.sTxSysLoss = atof(r[0]["txlosses"].c_str());
			mMobile.sRxSysLoss = atof(r[0]["rxlosses"].c_str());
			mMobile.sRxSens = atof(r[0]["rxsensitivity"].c_str());
			mMobile.sPatternKey = atoi(r[0]["antpatternkey"].c_str());
			mMobile.sMobileHeight = atof(r[0]["mobileheight"].c_str());
			mMobile.sFrequency = atof(r[0]["btlfreq"].c_str());
			mMaxPathLoss = atof(r[0]["maxpathloss"].c_str());
		} // if r.size()
	} // else !gDb->PerformRawSq
	
	unsigned n=0;
	double aveUpFreq=0.0;
	string PointString;
	double longitude, latitude; 
	unsigned spacePos;
	double channel;
	double N,S,W,E;
	mNorthWest.Get(N,W);
	mSouthEast.Get(S,E);
	char *text= new char[33];

	string areaQuery = "@ ST_GeomFromText('POLYGON((";
	gcvt(W,12,text);
   	areaQuery += text;
   	areaQuery += " ";
	gcvt(N,12,text);
   	areaQuery += text;
   	areaQuery += ",";
	gcvt(E,12,text);
   	areaQuery += text;
   	areaQuery += " ";
	gcvt(N,12,text);
   	areaQuery += text;
   	areaQuery += ",";
	gcvt(E,12,text);
   	areaQuery += text;
   	areaQuery += " ";
	gcvt(S,12,text);
   	areaQuery += text;
   	areaQuery += ",";
	gcvt(W,12,text);
   	areaQuery += text;
   	areaQuery += " ";
	gcvt(S,12,text);
   	areaQuery += text;
   	areaQuery += ",";
	gcvt(W,12,text);
   	areaQuery += text;
   	areaQuery += " ";
	gcvt(N,12,text);
   	areaQuery += text;
	areaQuery += "))',4326) ";

	delete [] text;

	query = "SELECT radioinstallation.id as radid, siteid, ST_AsText(location) as location, ";
	query += "eirp, max(cell.txpower) as txpower, txlosses, rxlosses, rxsensitivity, ";
	query += "txantpatternkey, txbearing, txmechtilt, ";
	query += "rxantpatternkey, rxbearing, rxmechtilt, txantennaheight, rxantennaheight, ";
	query += "btlfreq, spacing, bandwidth, downlink, uplink, ";
	query += "sum(cstraffic) as cstraffic, sum(pstraffic) as pstraffic ";
	query += "FROM radioinstallation CROSS JOIN cell CROSS JOIN technology CROSS JOIN site ";
	query += "WHERE techkey=technology.id and siteid=site.id and radioinstallation.id=risector ";
	query += "and location ";
	query += areaQuery;
	query += "group by radid, siteid, location, eirp, txlosses, rxlosses,";
	query += "txantpatternkey, txbearing, txmechtilt, ";
	query += "rxantpatternkey, rxbearing, rxmechtilt, txantennaheight, rxantennaheight, ";
	query += "btlfreq, spacing, bandwidth, downlink, uplink, rxsensitivity;";

	cout << query;
	// Perform a Raw SQL query
	if(!gDb.PerformRawSql(query))
	{
		string err = "Error in Database Select on tables radioinstallation and technology failed. Query:";
		err+=query; 
		QRAP_ERROR(err.c_str());
		return false;
	} // if
	else
	{
		gDb.GetLastResult(rFixed);
		
		int size = rFixed.size();
			
		if(size!=0)
		{
			mNumFixed=size;
			mFixedInsts.clear();
			
			for (i=0; i<mNumFixed; i++)
			{
				tFixed tempInst;
				tempInst.sInstKey = atoi(rFixed[i]["radid"].c_str());
				tempInst.sRange = mMaxCoverageRange;
				
				tempInst.sSiteID = atoi(rFixed[i]["siteid"].c_str());
				PointString = rFixed[i]["location"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				tempInst.sSitePos.Set(latitude,longitude,DEG);
				tempInst.sEIRP = atof(rFixed[i]["eirp"].c_str());
				tempInst.sTxPower = atof(rFixed[i]["txpower"].c_str());
				tempInst.sTxSysLoss = atof(rFixed[i]["txlosses"].c_str());
				tempInst.sRxSysLoss = atof(rFixed[i]["rxlosses"].c_str());
				tempInst.sRxSens = atof(rFixed[i]["rxsensitivity"].c_str());
				tempInst.sTxPatternKey = atoi(rFixed[i]["txantpatternkey"].c_str());
				tempInst.sTxAzimuth = atof(rFixed[i]["txbearing"].c_str());
				tempInst.sTxMechTilt = atof(rFixed[i]["txmechtilt"].c_str());
				tempInst.sRxPatternKey = atoi(rFixed[i]["rxantpatternkey"].c_str());
				tempInst.sRxAzimuth = atof(rFixed[i]["rxbearing"].c_str());
				tempInst.sRxMechTilt = atof(rFixed[i]["rxmechtilt"].c_str());
				tempInst.sTxHeight = atof(rFixed[i]["txantennaheight"].c_str());
				tempInst.sRxHeight = atof(rFixed[i]["rxantennaheight"].c_str());
				tempInst.sFrequency = atof(rFixed[i]["btlfreq"].c_str());
				tempInst.sBandWidth = atof(rFixed[i]["bandwidth"].c_str());
				tempInst.sCStraffic = atof(rFixed[i]["cstraffic"].c_str());
				tempInst.sPStraffic = atof(rFixed[i]["pstraffic"].c_str());
				dloffset = atof(rFixed[i]["downlink"].c_str());
				upoffset = atof(rFixed[i]["uplink"].c_str());
				chansep = atof(rFixed[i]["spacing"].c_str());
				
				query = "SELECT channel ";
				query += "FROM cell CROSS JOIN frequencyallocationlist ";
				query += "WHERE frequencyallocationlist.ci=cell.id and cell.risector ="; 
				gcvt(tempInst.sInstKey,8,temp);
				query += temp;
				query += " order by carrier;";
								
				// Perform a Raw SQL query
				if(!gDb.PerformRawSql(query))
				{
					string err = "Error in Database Select on tables radioinstallation and technology failed. Query:";
					err+=query; 
					QRAP_ERROR(err.c_str());
				} // if
				else
				{
					gDb.GetLastResult(r);
					unsigned size = r.size();
					if (size>0)
					{
						channel = atof(r[0]["channel"].c_str());
						mMobile.sFrequency = channel*chansep/1000.0+upoffset;
						tempInst.sFrequency = channel*chansep/1000.0+dloffset;
						n++;
						aveUpFreq +=channel*chansep/1000.0+upoffset;
						tempInst.sFreqList.push_back(channel*chansep/1000.0+dloffset);
						for (j=1; j<size; j++)
							tempInst.sFreqList.push_back(atof(r[j]["channel"].c_str())*chansep/1000.0+dloffset);
					} // if query not empty
				} // else query successfull
				mFixedInsts.push_back(tempInst);
			}// end for
		} // end if size>0
		else
		{
			string err = "Empty query. No radioinstallations in area ";
			QRAP_WARN(err.c_str());
		} // else query empty
	} // else query successful
	if (n>0) mMobile.sFrequency = aveUpFreq/n;

	return true;
}


//***************************************************************************
bool cTrafficDist::DetermineClutterDistMatrix()
{
	unsigned i, j, k;
	unsigned NumInsts = mFixedInsts.size();

	delete_Float2DArray(mClutterDist);
	mClutterDist = new_Float2DArray(NumInsts,mClutterUsed.mNumber+1); 
	for (i=0; i<NumInsts; i++)
		for (j=0; j<mClutterUsed.mNumber; j++)
				mClutterDist[i][j] = 0.0;

	cout << "In cTrafficDist::DetermineClutterDist(). NumInsts = " 
		<< NumInsts << "ClutterUsed.mNumber = " << ClutterUsed.mNumber << endl;	

	Float2DArray ClutterRaster;
	ClutterRaster = new_Float2DArray(2,2);
	cGeoP NW = mNorthWest;
	cGeoP SE = mSouthEast;
	double PlotRes=mPlotResolution;
	unsigned Rows = mRows;
	unsigned Cols = mCols;
	mClutter.GetForDEM(NW, SE, PlotRes, Rows, Cols, ClutterRaster);
	cout << "In cTrafficDist::DetermineClutterDist() " << endl;
	cout << "PlotRes:	Voor:	" << mPlotResolution << "	Na:	" <<  PlotRes << endl;
	cout << "Rows:	Voor:	" << mRows<< "	Na:	" <<  Rows << endl;
	cout << "Cols:	Voor:	" << mCols<< "	Na:	" <<  Cols << endl;
	cout << "North West:" << endl;
	mNorthWest.Display();
	NW.Display();
	cout << "South East: " << endl;
	mSouthEast.Display();
	SE.Display();

	unsigned CurrentRadInstID = 0;

	// Fill in Matrix
	for (i=0; i<mRows; i++)
	{
		for (j=0; j<mCols; j++)
		{
			if (mPlot[i][j] != mFixedInsts[CurrentRadInstID].sInstKey)
			{
				while ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID<NumInsts-1))
					CurrentRadInstID++;
				if ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID==NumInsts-1))
				{
					CurrentRadInstID=0;
					while ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID<NumInsts-1))
						CurrentRadInstID++;
				}
			}
			mClutterDist[CurrentRadInstID][(int)mClutterRaster[i][j]] += mPlotResolution*mPlotResolution/1000/1000;
	
		}
	}
	delete_Float2DArray(ClutterRaster);

	double *TotalsPerInst;
	TotalsPerInst = new double[NumInsts];
	for (i=0; i<NumInsts; i++)
	{
		TotalsPerInst[i] = 0.0;
		for (j=0; j<=mClutterUsed.mNumber; j++)
		{
			TotalsPerInst[i]+=mClutterDist[i][j];
//			cout << ClutterDist[i][j] << "	";
		}
//		cout << endl;
	}


	// Determine size of matrix ... i.e. How many Cluttertypes are represented
	double *TotalsPerClutter;
	TotalsPerClutter = new double[mClutterUsed.mNumber+1];
	for (j=0; j<=mClutterUsed.mNumber; j++)
	{
		TotalsPerClutter[j] = 0.0;
		for (i=0; i<NumInsts; i++)
		{
//			cout << ClutterDist[i][j] << "	";
			TotalsPerClutter[j]+=mClutterDist[i][j];
		}
//		cout << endl;
//		cout << "j=" <<j <<"	" <<TotalsPerClutter[j] << endl;
	}
	
	unsigned ClutterCount = 0;
	for (j=0; j<=ClutterUsed.mNumber; j++)
	{
		if (TotalsPerClutter[j]>0) 
			ClutterCount++;
	}


	if (NumInsts<ClutterCount)
	{
		cout << "Too few cells are included. Retry with more cells." << endl;
		delete [] TotalsPerClutter;
		delete [] TotalsPerInst;
		delete_Float2DArray(mClutterDist);
		return false;
	}

	mTheMatrix.resize(ClutterCount,ClutterCount);
	mTraffic.resize(ClutterCount,1);

	for( i=0; i<ClutterCount; i++)
	{
		mTraffic(i,0)=0.0;
		for(j=0; j<ClutterCount; j++)
		{
			mTheMatrix(i,j) = 0.0;
		}
	}

	// Fill in square matrix
	unsigned *RadInstOrder;
	RadInstOrder = new unsigned[NumInsts];
	for(i=0; i<NumInsts; i++)
	{
		RadInstOrder[i] = i;
	}
	
	
	unsigned swap;
	for (i=0;i<NumInsts;i++)
	{
		for (j=i+1; j<NumInsts; j++)
		{
			if (TotalsPerInst[RadInstOrder[j]]>TotalsPerInst[RadInstOrder[i]])
			{
				swap = RadInstOrder[i];
				RadInstOrder[i] = RadInstOrder[j];
				RadInstOrder[j] = swap; 
			}
		}
	}

//	cout << "In cTrafficDist::DetermineClutterDist(). ClutterCount = " << ClutterCount << endl; 
//	cout << "In cTrafficDist::DetermineClutterDist(). ClutterUsed.mNumber = " << ClutterUsed.mNumber << endl; 
	unsigned *ClutterIndex;
	ClutterIndex = new unsigned[ClutterCount];
	for (j=0; j<ClutterCount; j++)
	{
		ClutterIndex[j]=0;
	}
	unsigned Index = 0;
	j=0;
	for (j=0; j<=ClutterUsed.mNumber; j++)
	{
//		cout << "j=" <<j <<"	" <<TotalsPerClutter[j] << endl;
		if ((TotalsPerClutter[j]>0)&&(Index<ClutterCount))
		{
			ClutterIndex[Index] = j;
			Index++;
		} 
	}

//	cout << "Index = " << Index << endl;
//	cout << "In cTrafficDist::DetermineClutterDist() before Completing matrixes" << endl;

	for(i=0; i<ClutterCount; i++)
	{
//		cout << "i=" << i << "	RadInstOrder[i] = " << RadInstOrder[i] << endl;
		mTraffic(i,0) = mFixedInsts[RadInstOrder[i]].sCStraffic;
		for(j=0; j<ClutterCount; j++)
		{
//			cout << "j=" << j << ": " << ClutterIndex[j] << "	";
			mTheMatrix(i,j) = mClutterDist[RadInstOrder[i]][ClutterIndex[j]];
		}
//		cout << endl;
	}

/*	unsigned CountWrap = ceil(((double)NumInsts)/((double)ClutterCount));
	for (k=1;k<=CountWrap; k++)
	{
		for (i=ClutterCount*k+1;i<min(NumInsts,ClutterCount*(k+1));i++)
		{
			mTraffic(ClutterCount-(i-ClutterCount*k),0) += mFixedInsts[RadInstOrder[i]].sCStraffic;
			for(j=0; j<ClutterCount; j++)
				mTheMatrix(ClutterCount-(i-ClutterCount*k),j) += mClutterDist[RadInstOrder[i]][ClutterIndex[j]];
		}
 	}
*/
	cout << "In cTrafficDist::DetermineClutterDist() before Solving fo CStraffic" << endl;

	mWeights = mTheMatrix.fullPivLu().solve(mTraffic);

	cout << "In cTrafficDist::DetermineClutterDist() before writing back to mCSweights" << endl;
	delete [] mCSweights;
	mCSweights = new double[mClutterUsed.mNumber+1];
	for (i=0;i<ClutterCount;i++)
	{
		mCSweights[ClutterIndex[i]] = mWeights(i);
		cout << "ClutterIndex[i]=" << ClutterIndex[i] 
			<< "	mCSweights[ClutterIndex[i]] = " << mCSweights[ClutterIndex[i]] << endl;
	}

	cout << "In cTrafficDist::DetermineClutterDist() Doing PS traffice now" << endl;
	for(i=0; i<ClutterCount; i++)
	{
		mTraffic(i,0) = mFixedInsts[RadInstOrder[i]].sPStraffic;
		for(j=0; j<ClutterCount; j++)
			mTheMatrix(i,j) = mClutterDist[RadInstOrder[i]][ClutterIndex[j]];
	}

/*	for (k=1;k<=CountWrap; k++)
	{
		for (i=ClutterCount*k+1;i<min(NumInsts,ClutterCount*(k+1));i++)
		{
			mTraffic(ClutterCount-(i-ClutterCount*k),0) += mFixedInsts[RadInstOrder[i]].sPStraffic;
			for(j=0; j<ClutterCount; j++)
				mTheMatrix(ClutterCount-(i-ClutterCount*k),j) += ClutterDist[RadInstOrder[i]][ClutterIndex[j]];
		}
 	}
*/
	cout << "In cTrafficDist::DetermineClutterDist() before Solving fo CStraffic" << endl;
	mWeights = mTheMatrix.fullPivLu().solve(mTraffic);

	delete [] mPSweights;
	mPSweights = new double[mClutterUsed.mNumber+1];
	for (i=0;i<ClutterCount;i++)
		mPSweights[ClutterIndex[i]] = mWeights(i);
	
	delete [] RadInstOrder;
	delete [] TotalsPerClutter;
	delete [] TotalsPerInst;
	delete [] ClutterIndex;
	return true;

}


//***************************************************************************
bool cTrafficDist::DetermineClutterDistGradient()
{
	unsigned i, j, k;
	unsigned NumInsts = mFixedInsts.size();

	delete_Float2DArray(mClutterDist);
	mClutterDist = new_Float2DArray(NumInsts,mClutterUsed.mNumber+1); 
	for (i=0; i<NumInsts; i++)
		for (j=0; j<mClutterUsed.mNumber; j++)
				mClutterDist[i][j] = 0.0;

	cout << "In cTrafficDist::DetermineClutterDist(). NumInsts = " 
		<< NumInsts << "ClutterUsed.mNumber = " << ClutterUsed.mNumber << endl;	

	Float2DArray ClutterRaster;
	ClutterRaster = new_Float2DArray(2,2);
	cGeoP NW = mNorthWest;
	cGeoP SE = mSouthEast;
	double PlotRes=mPlotResolution;
	unsigned Rows = mRows;
	unsigned Cols = mCols;
	mClutter.GetForDEM(NW, SE, PlotRes, Rows, Cols, ClutterRaster);
	cout << "In cTrafficDist::DetermineClutterDist() " << endl;
	cout << "PlotRes:	Voor:	" << mPlotResolution << "	Na:	" <<  PlotRes << endl;
	cout << "Rows:	Voor:	" << mRows<< "	Na:	" <<  Rows << endl;
	cout << "Cols:	Voor:	" << mCols<< "	Na:	" <<  Cols << endl;
	cout << "North West:" << endl;
	mNorthWest.Display();
	NW.Display();
	cout << "South East: " << endl;
	mSouthEast.Display();
	SE.Display();

	unsigned CurrentRadInstID = 0;

	// Fill in Matrix
	for (i=0; i<mRows; i++)
	{
		for (j=0; j<mCols; j++)
		{
			if (mPlot[i][j] != mFixedInsts[CurrentRadInstID].sInstKey)
			{
				while ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID<NumInsts-1))
					CurrentRadInstID++;
				if ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID==NumInsts-1))
				{
					CurrentRadInstID=0;
					while ((mFixedInsts[CurrentRadInstID].sInstKey!=mPlot[i][j])&&(CurrentRadInstID<NumInsts-1))
						CurrentRadInstID++;
				}
			}
			mClutterDist[CurrentRadInstID][(int)mClutterRaster[i][j]] += mPlotResolution*mPlotResolution/1000/1000;
	
		}
	}
	delete_Float2DArray(ClutterRaster);


	return true;

}


//***************************************************************************
bool cTrafficDist::WriteOutput(GeoType OutputProj)
{
	string err = "Writing output for requested Prediction.";
	cout << err << endl;
//	QRAP_INFO(err.c_str());
	cRaster Output;
	bool returnValue;
	
	mNorthWest.SetGeoType(DEG);
	mSouthEast.SetGeoType(DEG);

	returnValue = Output.WriteFile(mPlot, mNorthWest, mSouthEast, mRows, mCols,
							mNSres,mEWres,mDir,mOutputFile,HFA,DEG,	OutputProj,mCentMer);
	err = "After WriteOutput in cTrafficDist.";
	cout << err << endl;
	return returnValue;
//	QRAP_INFO(err.c_str());
}

//***************************************************************************
double cTrafficDist::CostFunction(bool PacketData)
{
	unsigned i, j;
	double TotalCost = 0.0;
	double SumPerRow = 0.0;

	if (PacketData)
	{
		for (i=0; i<mNumFixed; i++)
		{
			SumPerRow=0.0;
			for (j=0; j<=mClutterUsed.mNumber; j++)
				SumPerRow += mPSweights[j]*mClutterDist[i][j];
			TotalCost += (SumPerRow-mFixedInsts[i].sPStraffic)*(SumPerRow-mFixedInsts[i].sPStraffic);
		}
	}
	else
	{
		for (i=0; i<mNumFixed; i++)
		{
			SumPerRow=0.0;
			for (j=0; j<=mClutterUsed.mNumber; j++)
				SumPerRow += mCSweights[j]*mClutterDist[i][j];
			TotalCost += (SumPerRow-mFixedInsts[i].sCStraffic)*(SumPerRow-mFixedInsts[i].sCStraffic);
		}
	}
	return TotalCost;
}

