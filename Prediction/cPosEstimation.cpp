 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cPosEstimation.cpp
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


#include "cPosEstimation.h"
#include <random>


//*********************************************************************
cPosEstimation::cPosEstimation() // default constructor
{
	
	mNumPoints = 0;
	mNewTP = 0;
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
	else mPlotResolution = 30;

	mPlotResolution = 30;

	setting = gDb.GetSetting("UseClutter");
	if (setting=="true")
		mUseClutter = true;
	else mUseClutter = false;

	mUseClutter = true;

	mClutterSource = atoi(gDb.GetSetting("ClutterSource").c_str());
	cout << "mClutterSource = " << mClutterSource << endl;
	if (mUseClutter)
		cout << "cPosEstimation constructor1:  Using Clutter " <<  endl;
	else cout << "cPosEstimation constructor1:  NOT Using Clutter " <<  endl;
	if (mUseClutter)
		mUseClutter = mClutter.SetRasterFileRules(mClutterSource);
	if (mUseClutter)
		cout << "cPosEstimation constructor2:  Using Clutter " <<  endl;
	else cout << "cPosEstimation constructor2:  NOT Using Clutter " <<  endl;
	if (mUseClutter)
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	if (mUseClutter)
		cout << "cPosEstimation constructor3:  Using Clutter " <<  endl;
	else cout << "cPosEstimation constructor3:  NOT Using Clutter " <<  endl;
	mUseClutter = (mUseClutter)&&(mClutterClassGroup>0);
	if (mUseClutter) mClutterCount = new unsigned[mPathLoss.mClutter.mNumber];
	else mClutterCount = new unsigned[2];
	if (mUseClutter) mClutter.SetSampleMethod(1);
	if (mUseClutter)
		cout << "cPosEstimation constructor4:  Using Clutter " <<  endl;
	else cout << "cPosEstimation constructor4:  NOT Using Clutter " <<  endl;

	mDEMsource = atoi(gDb.GetSetting("DEMsource").c_str());
	cout << "mDEMsource = " << mDEMsource << endl;
	mDEM.SetRasterFileRules(mDEMsource);
	mDEM.SetSampleMethod(2);

	mFixedAnts = new cAntennaPattern[2];
	mCellPathLoss = new double[2];
	mNumInsts = 2;
}

//*********************************************************************
cPosEstimation::~cPosEstimation() // destructor
{
	unsigned i;
	for (i=0;i<mNumPoints;i++)
	{
		mPosSets[i].sTestPoints.clear();
		mPosSets[i].sMeasurements.clear();
	}
	mPosSets.clear();
	delete [] mFixedAnts;
	delete [] mCellPathLoss;
}

//*********************************************************************
bool cPosEstimation::LoadMeasurements(vPoints Points,
					unsigned MeasType, 
					unsigned MeasSource,
					unsigned PosSource,
					unsigned Technology)
{
	pqxx::result r;
	unsigned tp = 0;
	unsigned NumInPosSet = 0;
	double Lat, Lon, mNorth, mSouth, mEast, mWest;
	char *text= new char[33];
	cGeoP NorthWestCorner,SouthEastCorner; 
	if (Points.size() > 1)
	{
		Points[0].Get(mNorth,mEast);
		mSouth = mNorth;
		mWest = mEast;
		string query;
		pqxx::result MeasSelect;

		query ="SELECT distinct measurement.tp as tp, ST_AsText(testpoint.location) as origLocation,";
		query += "siteid, ST_AsText(site.location) as siteLocation, radioinstallation.id as InstKeyFixed, ";
		query += "txantennaheight, EIRP, txbearing, txmechtilt, txantpatternkey, azibeamwidth, ";
		query += "ci, ST_AsText(centriod) as centriod, measurement.frequency as frequency, ";
		query += "max(measvalue) as measvalue, min(pathloss) as pathloss, ";
		query += "min(testpointauxUTRAN.RxTxDiff) as timeDiff1, min(testpointauxUTRAN.RxTxDiff2) as timeDiff2, ";
		query += "min(testpointauxGSM.TA) as TA, technology.DistRes as DistRes ";
		query += "from measurement left join testpointauxUTRAN ";
		query += "on (measurement.tp=testpointauxUTRAN.tp and measurement.ci=testpointauxUTRAN.servci)"; 
		query += "left join testpointauxGSM ";
		query += "on (measurement.tp=testpointauxGSM.tp and measurement.ci=testpointauxGSM.servci) ";
		query += "cross join testpoint cross join cell cross join radioinstallation  ";
		query += "cross join antennapattern cross join site ";
		query += " LEFT OUTER JOIN technology ON (radioinstallation.techkey=technology.id) ";
		query += "where measurement.tp=testpoint.id and measurement.ci=cell.id and siteid=site.id ";
		query += "and cell.risector=radioinstallation.id and radioinstallation.txantpatternkey = antennapattern.id ";
		query += "and testpoint.positionsource<2 ";
		query += " and site.Location @ ST_GeomFromText('POLYGON((";
		for (int i = 0 ; i < Points.size();i++)
	        {
			Points[i].Get(Lat, Lon);
	        	mNorth = max(mNorth,Lat);
	        	mSouth = min(mSouth,Lat);
	        	mEast = max(mEast,Lon);
	        	mWest = min(mWest,Lon);
			gcvt(Lon,12,text);
	        	query += text;
	        	query += " ";
			gcvt(Lat,12,text);
	        	query += text;
	        	query += ",";
	        }
	        NorthWestCorner.Set(mNorth,mWest,DEG);
	        SouthEastCorner.Set(mSouth,mEast,DEG);
		cout << "North West corner: " << endl;
		NorthWestCorner.Display();
		cout << "South East corner: " << endl;
		SouthEastCorner.Display();
		Points[0].Get(Lat,Lon);
		gcvt(Lon,12,text);
	        query += text;
	        query += " ";
		gcvt(Lat,12,text);
	        query += text;
	        query += "))',4326) ";

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
		if (Technology>0)
		{
			query += " and Technology.id=";
			gcvt(Technology,9,text);
			query += text;
		}
		query+=" group by siteid, InstKeyFixed, azibeamwidth, origLocation, "; 
		query+=" measurement.frequency, measurement.tp, ci, siteLocation, centriod, DistRes ";
		query+=" order by measurement.tp, timeDiff1, timeDiff2, TA, measvalue desc;";

		cout << query << endl;


		if (!gDb.PerformRawSql(query))
		{
			string err ="Problem with database query to get measurements from selected area! Problem with query: ";
			err += query;
			cout << err << endl;
			QRAP_ERROR(err.c_str());
			delete [] text;
			return false;
		}

		gDb.GetLastResult(r);
		if (r.size() >0)
		{
			tTestPoint NewTestPoint;
			tPosSet NewPosSet;
			tMeas NewMeasurement; 
			double longitude, latitude;
			double Distance;
			string PointString;
			unsigned spacePos;

			NewTestPoint.sOriginalTP=0;
			NewTestPoint.sNewTP=0;

			unsigned i;
			for (i = 0; i < r.size();i++)
			{
				tp = atoi(r[i]["tp"].c_str());
				if (tp != NewTestPoint.sOriginalTP)
				{
					NewPosSet.sNumMeas = NumInPosSet;
					mPosSets.push_back(NewPosSet);
					NewPosSet.sTestPoints.clear();
					NewPosSet.sMeasurements.clear();
					NumInPosSet = 1;
					NewTestPoint.sOriginalTP = tp;
					NewTestPoint.sNewTP = tp;
					PointString = r[i]["origLocation"].c_str();
					spacePos = PointString.find_first_of(' ');
					longitude = atof((PointString.substr(6,spacePos).c_str())); 
					latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
					NewTestPoint.sOriginalLocation.Set(latitude,longitude,DEG);
					NewTestPoint.sEstimatedLocation.Set(latitude,longitude,DEG);
					NewTestPoint.sMethodUsed = GPS;
					NewTestPoint.sErrorEstimate = 0.0;
					NewPosSet.sTestPoints.push_back(NewTestPoint);
				}
				NewMeasurement.sOriginalTP = tp;
				NewMeasurement.sID = i;
				NewMeasurement.sSiteID = atoi(r[i]["siteid"].c_str());
				PointString = r[i]["siteLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				if ((longitude<181)&&(longitude>-181)&&(latitude<91)&&(latitude>-91))
				{
					NumInPosSet++; 
					NewMeasurement.sSiteLocation.Set(latitude,longitude,DEG);
					NewMeasurement.sCellID = atoi(r[i]["ci"].c_str());
					PointString = r[i]["centriod"].c_str();
					spacePos = PointString.find_first_of(' ');
					longitude = atof((PointString.substr(6,spacePos).c_str())); 
					latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
					NewMeasurement.sCentroid.Set(latitude,longitude,DEG);
					NewMeasurement.sInstKeyFixed = atoi(r[i]["InstKeyFixed"].c_str());
					NewMeasurement.sAntPatternKey = atoi(r[i]["txantpatternkey"].c_str());
					NewMeasurement.sFrequency = atof(r[i]["frequency"].c_str());
					NewMeasurement.sMeasValue = atof(r[i]["measvalue"].c_str());
					NewMeasurement.sPathLoss = atof(r[i]["pathloss"].c_str());
					NewMeasurement.sResDist = atof(r[i]["DistRes"].c_str());
					NewMeasurement.sEIRP = atof(r[i]["EIRP"].c_str());
					NewMeasurement.sPredValue = 0;
					NewMeasurement.sAzimuth = atof(r[i]["txbearing"].c_str());
					NewMeasurement.sBeamWidth = atof(r[i]["azibeamwidth"].c_str());
					NewMeasurement.sTilt = atof(r[i]["txmechtilt"].c_str());
					NewMeasurement.sHeight = atof(r[i]["txantennaheight"].c_str());
					NewMeasurement.sDistance = 999999;
					NewMeasurement.sServingCell = false;
					if (strlen(r[i]["timeDiff1"].c_str())>0)
					{
						NewMeasurement.sServingCell = true;
						Distance = (atoi(r[i]["timeDiff1"].c_str())+0.5)*NewMeasurement.sResDist;
						if (Distance<NewMeasurement.sDistance)
							NewMeasurement.sDistance = Distance;
					}
					if (strlen(r[i]["timeDiff2"].c_str())>0)
					{
						NewMeasurement.sServingCell = true;
						NewMeasurement.sResDist /=16;
						Distance = (atoi(r[i]["timeDiff2"].c_str())+0.5)*NewMeasurement.sResDist;
						if (Distance<NewMeasurement.sDistance)
							NewMeasurement.sDistance = Distance;
					}
					if (strlen(r[i]["TA"].c_str())>0)
					{
						NewMeasurement.sServingCell = true;
						Distance = (atoi(r[i]["TA"].c_str())+0.5)*NewMeasurement.sResDist;
						if (Distance<NewMeasurement.sDistance)
							NewMeasurement.sDistance = Distance;
					}
	
					NewMeasurement.sPathLoss = - NewMeasurement.sMeasValue 
																					+ NewMeasurement.sEIRP;
						
					NewPosSet.sMeasurements.push_back(NewMeasurement);
				}
			}// end for number of entries

			NewPosSet.sNumMeas = NumInPosSet;
			mPosSets.push_back(NewPosSet);
			mNumPoints = mPosSets.size();
			string q = "SELECT (MAX(id)+1) AS id FROM TestPoint";
			pqxx::result r;
			gDb.PerformRawSql(q);
			gDb.GetLastResult(r);
			mNewTP = atoi(r[0]["id"].c_str());
		} // end if query is NOT empty
		else 
		{
			string err ="Empty query: ";
			err += query;
			cout << err << endl;
			QRAP_ERROR(err.c_str());
			delete [] text;
			return false;
		}
		delete [] text;
/*
		for (unsigned i=0; i<mPosSets.size(); i++)
			if (mPosSets[i].sTestPoints.size()>0)
				cout << mPosSets[i].sTestPoints[0].sOriginalTP << "n" 
					<< mPosSets[i].sMeasurements.size() << "	";
		cout << endl; 
*/
		cout << "cPosEstimation::LoadMeasurement: leaving ... happy ... " << endl << endl;
		return true;
	}
	delete [] text;
	cout << "cPosEstimation::LoadMeasurement: leaving ... not enough points in selecting area." << endl << endl;
	return false;
}

//********************************************************************************
void cPosEstimation::EstimatePositions()
{
	unsigned i,j;
	double distance;
	mNumPoints = mPosSets.size();
	cout << "cPosEstimation::EstimatePositions(): mNumPoints = " << mNumPoints << endl;
	if (mUseClutter)
		cout << "cPosEstimation::EstimatePositions(): Using Clutter " <<  endl;
	else cout << "cPosEstimation::EstimatePositions(): NOT Using Clutter " <<  endl;

	for (i=0; i< mNumPoints; i++)
	{
		mCurrentPosSetIndex = i;
		if ((mPosSets[mCurrentPosSetIndex].sMeasurements.size()>0)
			&&(mPosSets[mCurrentPosSetIndex].sTestPoints.size()>0))
		{
			mNumInsts = mPosSets[mCurrentPosSetIndex].sMeasurements.size();
			delete [] mFixedAnts;
			mNumInsts = mPosSets[mCurrentPosSetIndex].sMeasurements.size();
			mFixedAnts = new cAntennaPattern[mNumInsts];
			for (j=0; j < mNumInsts; j++)
			{
				mFixedAnts[j].SetAntennaPattern(mPosSets[mCurrentPosSetIndex].sMeasurements[j].sAntPatternKey, 
					mPosSets[mCurrentPosSetIndex].sMeasurements[j].sAzimuth, 
					mPosSets[mCurrentPosSetIndex].sMeasurements[j].sTilt);
			}
			mPathLoss.setParameters(mkFactor,mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency,
					mPosSets[mCurrentPosSetIndex].sMeasurements[0].sHeight, MOBILEHEIGHT,
					mUseClutter, mClutterClassGroup);
	
			if (	mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance<120001)		
			{
				CoSinRule();
				CI_TA();
			}
			else SecondSite();
			CI();
			DCM_ParticleSwarm();
			for (j=0; j<mPosSets[mCurrentPosSetIndex].sTestPoints.size(); j++)
			{

				distance = mPosSets[mCurrentPosSetIndex].sTestPoints[j].sOriginalLocation.
					Distance(mPosSets[mCurrentPosSetIndex].sTestPoints[j].sEstimatedLocation);
				cout << distance << "	";
				switch (mPosSets[mCurrentPosSetIndex].sTestPoints[j].sMethodUsed)
				{
					case GPS: 			cout << "GPS" << endl;		break;  
					case CellID: 			cout << "CellID" << endl;	break; 
					case CellID_TA: 		cout << "CellID_TA" << endl;	break; 
					case SSiteDir:			cout << "SSiteDir" << endl;	break; 
					case CoSiteSecDir:		cout << "CoSiteSecDir" << endl;	break; 
					case CosRuleAngleDistRatio:	cout << "CosRuleAngleDistRatio" << endl; break; 
					case CosRuleDistDist:		cout << "CosRuleDistDist" << endl; break; 
					case CosRuleDistAngle:		cout << "CosRuleDistAngle" << endl; break; 
					case CosRuleAngleAngle:		cout << "CosRuleAngleAngle" << endl; break; 
					case DCM_PSO:			cout << "DCM_PSO" << endl;	break; 
					default: cout << "None" << endl;
				}
				
				mPosSets[mCurrentPosSetIndex].sTestPoints[j].sOriginalLocation.Display();
				mPosSets[mCurrentPosSetIndex].sTestPoints[j].sEstimatedLocation.Display();
				cout << endl;

			} 
			cout << endl;
		}
	}
}

//*******************************************************************
bool cPosEstimation::CI()
{
	unsigned i;
	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;

	newTestPoint.sMethodUsed = CellID;

	double Distance=0.0;
	for(i=1;i<mPosSets[mCurrentPosSetIndex].sMeasurements.size(); i++)
	{
		Distance+= mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
			Distance(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteLocation);
	}
	newTestPoint.sErrorEstimate = Distance /mPosSets[mCurrentPosSetIndex].sMeasurements.size()/4;

	newTestPoint.sEstimatedLocation =  mPosSets[mCurrentPosSetIndex].sMeasurements[0].sCentroid;

	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;
	return true;

}
//*******************************************************************
// Can only be used if a sectorised site is used
bool cPosEstimation::CI_TA()
{
	unsigned i;
	double pathloss;
	if (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth > 270)
		return false;

	double Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance; 

	int ClosestIndex =-1;
	double ClosestValue = MAXDOUBLE;

	if (Distance>120000)
	{
		for (i=1; i < mPosSets[mCurrentPosSetIndex].sMeasurements.size(); i++)
		{
			if (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID !=
				mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteID)
			{
				pathloss = - mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue
						+ mPosSets[mCurrentPosSetIndex].sMeasurements[i].sEIRP
						- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency);
				if (pathloss < ClosestValue)
				{
					ClosestValue = pathloss;
					ClosestIndex = i;
				}
			}
		}
		if (ClosestIndex > 0)
		{
			Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
				Distance(mPosSets[mCurrentPosSetIndex].sMeasurements[ClosestIndex].sSiteLocation)/2;
		}
		else Distance = 17500;
	}

	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;

	newTestPoint.sMethodUsed = CellID_TA;

	//Error Estimate is the arc at the Distance 
	newTestPoint.sErrorEstimate = max(Distance * mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth * PI /180,
					mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist);
	
	newTestPoint.sEstimatedLocation.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, 
						Distance, mPosSets[mCurrentPosSetIndex].sMeasurements[0].sAzimuth);

	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;
	return true;	
}

//***************************************************************************************
// The direction is based on the 'closest' other site. This could be useful for omni-directional sites
bool cPosEstimation::SecondSite()
{
	unsigned i;
	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;

	newTestPoint.sMethodUsed = SSiteDir;

	int ClosestIndex=-1;
	double ClosestValue = 155;
	double pathloss;

	for (i=1; i < mPosSets[mCurrentPosSetIndex].sMeasurements.size(); i++)
	{
		if (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID !=
			mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteID)
		{
			pathloss = - mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue
					+ mPosSets[mCurrentPosSetIndex].sMeasurements[i].sEIRP
					- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency);
			if (pathloss < ClosestValue)
			{
				ClosestValue = pathloss;
				ClosestIndex = i;
			}
		}
	}

	if (-1==ClosestIndex)
	return false;

	double Azimuth = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
			Bearing(mPosSets[mCurrentPosSetIndex].sMeasurements[ClosestIndex].sSiteLocation);

	double SiteToSite = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
			Distance(mPosSets[mCurrentPosSetIndex].sMeasurements[ClosestIndex].sSiteLocation);

	pathloss = - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sMeasValue
					+ mPosSets[mCurrentPosSetIndex].sMeasurements[0].sEIRP
					- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency);

	double k = pow(10, (ClosestValue - pathloss)/(10*GAMMA));
	cout << "k = " << k << endl;
	double Distance = SiteToSite / (k+1);

//	Distance = SiteToSite / 2;

	if ((fabs((Distance-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
		mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
		&&(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance<120000))
	{
		if ((Distance - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance) < 0)
			Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					- 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
		else 
			Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					+ 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
	}

	//Assuming a more or less regular cellplan
	newTestPoint.sErrorEstimate = max(Distance * PI/6,
					mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist);

	newTestPoint.sEstimatedLocation.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, Distance, Azimuth);

	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;

	return true;	
	
}

//**************************************************************************************
// This method base the direction on the antenna patterns of two sectors of the serving site
bool cPosEstimation::CoSecAzi(double &minAzi)
{

	vBand Bands;
	tBand Band;

	Band.sFrequency = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency;
	Band.sAIndex = 0;
	Band.sBIndex = -1;
	Band.sMaxMeasValue = -120; 
	Bands.push_back(Band);

	unsigned i,j;

	//Find Co-sited cell in same band
	for (i=1; i < mPosSets[mCurrentPosSetIndex].sMeasurements.size(); i++)
	{
		for (j=0; j < Bands.size(); j++)
		{	// If Co-Sited
			if (mPosSets[mCurrentPosSetIndex].sMeasurements[Bands[j].sAIndex].sSiteID ==
				mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteID) 
			{
				// Working in same band ... obstruction will affect different f-band differently
				if (fabs( mPosSets[mCurrentPosSetIndex].sMeasurements[Bands[j].sAIndex].sFrequency -
					mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency) < 200 )
				{
					if (mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue > Bands[j].sMaxMeasValue)
						Bands[j].sBIndex=i; 

				}
				else 
				{	
					Band.sFrequency = mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency;
					Band.sAIndex = i;
					Band.sBIndex = -1;
					Band.sMaxMeasValue = -120;
					Bands.push_back(Band); 
				}
			}
		}
	}

	j=0;
	if (Bands.size()>0)
	{
		while ((-1==Bands[j].sBIndex)&&(j<Bands.size()-1))
			j++;

		if ((j>=Bands.size()-1)&&(-1==Bands[j].sBIndex))
		{
			Bands.clear();
			SecondSite();
		 	return false;
		}
	}
	else return false;

	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
	newTestPoint.sMethodUsed = CoSiteSecDir;

	unsigned AIndex = Bands[j].sAIndex;
	unsigned BIndex = Bands[j].sBIndex;
	Bands.clear();

	minAzi = FindAzi(BIndex, AIndex);

	double Distance= mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;

	double minDist = 2.0*mPlotResolution;
	double maxDist = 35000;
	if (Distance>120000)
	{	
		if (mPosSets[mCurrentPosSetIndex].sMeasurements.size()>1)
		{
			j=0;
			while ((j<mPosSets[mCurrentPosSetIndex].sMeasurements.size()-1)
						&&(	mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID==
								mPosSets[mCurrentPosSetIndex].sMeasurements[j].sSiteID))
				j++;
		
			if ((mPosSets[mCurrentPosSetIndex].sMeasurements.size()<j)
				||(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID!=
								mPosSets[mCurrentPosSetIndex].sMeasurements[j].sSiteID))
			{		
				maxDist = 
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Distance
								(mPosSets[mCurrentPosSetIndex].sMeasurements[j].sSiteLocation);
			}
		}
	}
	else
	{
		maxDist = (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance
				/mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist+0.5)*
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist+50.0;
		minDist = max(2.0*mPlotResolution,(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance
				/mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist-0.5)
				*mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist-50.0);
	}

	if (maxDist>120000) maxDist=35000;
	if (maxDist<4.0*mPlotResolution) maxDist=4.0*mPlotResolution;;
	Distance = SearchDistance(minAzi,minDist, maxDist); 
	
	cout << "Distance = " << Distance << endl;

	newTestPoint.sEstimatedLocation.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, Distance, minAzi);

	double DegRes = min(5.0,max(90.0*mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sResDist/Distance/PI,1.0));
	newTestPoint.sErrorEstimate = max(((double)mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sResDist)/2.0,DegRes*PI*Distance/180.0);

	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;
	return true;
}

//************************************************************************************************
//
double cPosEstimation::SearchDistance(double Azimuth, double min, double max)
{
	unsigned i;
	unsigned StopNum = ceil((max-min)/mPlotResolution);
	cout << "StopNum = " << StopNum << "	mPlotResolution = " << mPlotResolution 
				<< "	max = " << max << "	min = " << min << endl;
	double Distance = (max-min)/2.0;
	double BestCost = 999999;
	double Cost = 999999;

	for (i=0; i<StopNum; i++)
	{
		Cost = CostFunction(min + (double)i*mPlotResolution, Azimuth);
		if (Cost < BestCost)
		{
			BestCost = Cost;
			Distance = min + i*mPlotResolution;
		}
	}	
	return Distance;
}


//*************************************************************************************************
//
double cPosEstimation::FindAzi(unsigned BIndex, unsigned AIndex)
{
	unsigned i;
	double LeftAngle, RightAngle;
	unsigned LeftIndex, RightIndex;
	
	double DeltaAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth
			- mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sAzimuth;

	if (fabs(DeltaAngle)>180)
	{
		if (DeltaAngle<0)
			mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sAzimuth -= 360;
		else
			mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth -= 360;
		DeltaAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth
				- mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sAzimuth;		
	}

	if (DeltaAngle < 0)
	{
		LeftAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth
				- mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sBeamWidth;
		LeftIndex = AIndex;
		RightAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sAzimuth
						+ mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sBeamWidth;
		RightIndex = BIndex;
	}
	else
	{
		LeftAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sAzimuth
						- mPosSets[mCurrentPosSetIndex].sMeasurements[BIndex].sBeamWidth;
		LeftIndex = BIndex;
		RightAngle = mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth
				+ mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sBeamWidth;
		RightIndex = AIndex;
	} 


	cAntennaPattern LeftAnt, RightAnt;
	LeftAnt.SetAntennaPattern(mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sAntPatternKey, 
				mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sAzimuth, 
				mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sTilt);
	RightAnt.SetAntennaPattern(mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sAntPatternKey, 
				mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sAzimuth, 
				mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sTilt);

	double RxLevDiff = mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sMeasValue 
				- mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sMeasValue;
	double Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
	double Elev = 180*atan2(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sHeight,Distance)/PI;

	double Delta;
	double minAzi = mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sAzimuth;
	double Azimuth = minAzi;
	double LeftAntValue = LeftAnt.GetPatternValue(Azimuth, Elev);
	double RightAntValue = RightAnt.GetPatternValue(Azimuth, Elev);
	double minDelta = fabs (RxLevDiff - mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sEIRP + LeftAntValue
				+ mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sEIRP - RightAntValue);

	//Exaustive search through angles
	double DegRes = min(5.0,max(90*mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sResDist/Distance/PI,1.0));
	unsigned NumSteps = (int)((RightAngle - LeftAngle)/DegRes)+1; 

	for (i=0; i<NumSteps ; i++)
	{
		Azimuth = LeftAngle + DegRes*i;
		LeftAntValue = LeftAnt.GetPatternValue(Azimuth, Elev);
		RightAntValue = RightAnt.GetPatternValue(Azimuth, Elev);
		Delta = fabs (RxLevDiff - mPosSets[mCurrentPosSetIndex].sMeasurements[LeftIndex].sEIRP + LeftAntValue
				+ mPosSets[mCurrentPosSetIndex].sMeasurements[RightIndex].sEIRP - RightAntValue);

	 	if ((Delta<minDelta)&&(LeftAntValue<14)&&(RightAntValue<14)&&((LeftAntValue>6)||(RightAntValue>6)))
	 	{
	 		minDelta = Delta;
	 		minAzi = Azimuth;
	 	}
	}
	return minAzi;
}
//**************************************************************************************
bool cPosEstimation::CoSinRule()
{

	double servSiteAzi=0;
	
	bool haveOwnAngle = CoSecAzi(servSiteAzi);

	vBand Bands;

	tBand Band;

	unsigned i,j, k, AIndex, BIndex;

	k=1;
	while (mPosSets[mCurrentPosSetIndex].sMeasurements[k].sSiteID 
		== mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID)
		k++;
	Band.sFrequency = mPosSets[mCurrentPosSetIndex].sMeasurements[k].sFrequency;
	Band.sAIndex = k;
	Band.sBIndex = -1;
	Band.sMaxMeasValue = -120; 
	Bands.push_back(Band);

	bool stop = false;
	unsigned FirstOtherSite = k;

	double A, B, C, alpha, beta, gamma, cosB, cosC, cosA, sinA, sinB;
	cGeoP oldEst(-25.7, 28.2,DEG);
	bool worked = false;
	unsigned OtherSiteIndex;

	if (haveOwnAngle)
	{
		OtherSiteIndex = FirstOtherSite;
		tTestPoint newTestPoint;
		newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
		newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
		newTestPoint.sMethodUsed = CosRuleAngleDistRatio;

		double Site2SiteDist = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Distance
				(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation);
		C = Site2SiteDist;
		double OwnPathL = - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sMeasValue
			+ mPosSets[mCurrentPosSetIndex].sMeasurements[0].sEIRP
			- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency);
		double OtherPathL = - mPosSets[mCurrentPosSetIndex].sMeasurements[FirstOtherSite].sMeasValue
			+ mPosSets[mCurrentPosSetIndex].sMeasurements[FirstOtherSite].sEIRP
			- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[FirstOtherSite].sFrequency);

		double k = pow(10, (OtherPathL - OwnPathL)/(10*GAMMA));

		cout << "k = " << k << endl;

		beta = (servSiteAzi-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Bearing
				(mPosSets[mCurrentPosSetIndex].sMeasurements[FirstOtherSite].sSiteLocation));
		if (beta >180) beta -=360;
		if (beta <-180) beta += 360;
		if (beta <0) beta *= -1;
		
		cosB = cos(beta*PI/180);
		sinB = sin(beta*PI/180);
		
		if (k==1)
			A = C / (2*cosB); 
		else if (k<fabs(sinB)) // non-real solution not possible change k for solution to be real
		{
			k = fabs(sinB);
			A = C *cosB/(1-k*k);
		}
		else
		{
			double A1 = C*(cosB + sqrt(k*k-sinB*sinB))/(1-k*k);
			double A2 = C*(cosB - sqrt(k*k-sinB*sinB))/(1-k*k);
			cout << "beta=" << beta << "	A1=" << A1 << "	A2=" << A2 << endl;
			if (A1<0) A1=A2; // negative answer is non-sensicle
			else if (A2<0) A2=A1; // negative answer is non-sensicle
			else if (fabs((A1-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
				A1 = A2; // choose solution that fit range
			else if (fabs((A2-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
				A2 = A1;
			A=min(A1,A2);
			if (A<=0) A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
		}
		if ((fabs((A-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
			mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
			&&(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance<120000))
		{
			if ((A - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance) < 0)
				A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					- 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
			else 
				A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					+ 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
		}

		newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, A, servSiteAzi);
		newTestPoint.sErrorEstimate = oldEst.Distance(newTestPoint.sEstimatedLocation);

		newTestPoint.sNewTP = mNewTP;	
		mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
		mNewTP++;
	}

	for (k=FirstOtherSite+1; (k<mPosSets[mCurrentPosSetIndex].sMeasurements.size())&&(!stop); k++)
	{
		if (mPosSets[mCurrentPosSetIndex].sMeasurements[k].sSiteID 
			!= mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteID)
		{
			for (i=k+1; i < mPosSets[mCurrentPosSetIndex].sMeasurements.size(); i++)
			{
				for (j=0; j < Bands.size(); j++)
				{	// If Co-Sited
					if (mPosSets[mCurrentPosSetIndex].sMeasurements[Bands[j].sAIndex].sSiteID ==
						mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteID)
					{
						// Working in same band ... obstruction will affect different f-band differently
						if (fabs( mPosSets[mCurrentPosSetIndex].sMeasurements[Bands[j].sAIndex].sFrequency -
							mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency) < 200 )
						{ //updating old band
							if (mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue 
								> Bands[j].sMaxMeasValue)
								Bands[j].sBIndex=i; 
						}
						else //initialise new band
						{	
							Band.sFrequency = mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency;
							Band.sAIndex = i;
							Band.sBIndex = -1;
							Band.sMaxMeasValue = -120;
							Bands.push_back(Band); 
						}
					}
				}
			}
		}
	


		if (Bands.size()>0)
		{
			j=0;
			while ((-1==Bands[j].sBIndex)&&(j<Bands.size()-1))
				j++;

			if ((Bands.size()-1==j)&&(-1==Bands[j].sBIndex))
				Bands.clear();
			else if (Bands.size()-1<j)
				Bands.clear();
			else
			{ 
				AIndex = Bands[j].sAIndex;
				BIndex = Bands[j].sBIndex;
				Bands.clear();
				stop = true;
			}
		}
	}

	int OtherAziIndex=-1;
	if (stop) OtherAziIndex = AIndex;

	int OtherDistanceIndex=-1;
	double OtherSiteDistance = 999999;

	for (k=1; (k<mPosSets[mCurrentPosSetIndex].sMeasurements.size())&&(-1==OtherDistanceIndex); k++)
		if (mPosSets[mCurrentPosSetIndex].sMeasurements[k].sDistance < 120000)
		{
			OtherDistanceIndex = k;
			OtherSiteDistance = mPosSets[mCurrentPosSetIndex].sMeasurements[k].sDistance;
		}


	if ((-1==OtherAziIndex)&&(-1==OtherDistanceIndex))
		return false;


// If one has the Rx-Tx from active sets from different sites
	if (-1!=OtherDistanceIndex)
	{
		tTestPoint newTestPoint;
		newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
		newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
		newTestPoint.sMethodUsed = CosRuleDistDist;
		worked = true;
		OtherSiteIndex = OtherDistanceIndex;
		double Site2SiteDist = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Distance
				(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation);
		C = Site2SiteDist;

		A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
		B = OtherSiteDistance;
		
		beta = 180*acos((A*A+C*C-B*B)/(2*A*C))/PI;

		double SiteAzi = beta+mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Bearing
					(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation);
		
		newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, A, SiteAzi);
		newTestPoint.sErrorEstimate = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
		oldEst = newTestPoint.sEstimatedLocation;
		newTestPoint.sNewTP = mNewTP;	
		mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
		mNewTP++;
/*
		if (haveOwnAngle)
		{
			beta = (servSiteAzi-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Bearing
					(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation));
			if (beta >180) beta -=360;
			if (beta <-180) beta += 360;
			
			C = Site2SiteDist;
			B = OtherSiteDistance;

			cosB = cos(beta*PI/180);
			sinB = sin(beta*PI/180);
			A = C*cosB - sqrt(B*B-C*C*sinB*sinB);

			newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, A, servSiteAzi);
			newTestPoint.sErrorEstimate = oldEst.Distance(newTestPoint.sEstimatedLocation);
		}
*/
	}

// Have other site angle
	if ((-1!=OtherAziIndex)&&(-1!=BIndex))
	{
		tTestPoint newTestPoint;
		newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
		newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
		newTestPoint.sMethodUsed = CosRuleDistAngle;
		worked = true;
		double OtherSiteAzi = FindAzi(BIndex, AIndex);
		OtherSiteIndex = OtherAziIndex;
		double Site2SiteDist = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Distance
					(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation);

		C = Site2SiteDist;
		alpha = (OtherSiteAzi-mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation.Bearing
					(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation));
		if (alpha>180) alpha-=360;
		if (alpha<-180) alpha+=360;
		if (alpha<0) alpha*=-1;

		cosA = cos(alpha*PI/180);
		sinA = sin(alpha*PI/180);

		double OwnPathL = - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sMeasValue
			+ mPosSets[mCurrentPosSetIndex].sMeasurements[0].sEIRP
			- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency);
		double OtherPathL = - mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sMeasValue
			+ mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sEIRP
			- 20*log10(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sFrequency);

		double k = pow(10, (OtherPathL - OwnPathL)/(10*GAMMA));

		cout << "k = " << k << endl;
		
		if (k==1)
			A = C / (2*k*cosA); 
		else if (1<fabs(k*sinA)) /// non-real solution not possible change k such that solution is real
		{
			k= 1.0/fabs(sinA);
			A = C*k*cosA/(k*k-1); 
		}
		else
		{
			double A1 = C*(k*cosA + sqrt(1-k*k*sinA*sinA))/(k*k-1);
			double A2 = C*(k*cosA - sqrt(1-k*k*sinA*sinA))/(k*k-1);
			cout << "alpha=" << alpha << "	A1=" << A1 << "	A2=" << A2 << endl;
			if (A1<0) A1=A2; // negative distance is non-sensicle
			else if (A2<0) A2=A1; // negative distance is non-sensicle
			else if (fabs((A1-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
				A1 = A2;
			else if (fabs((A2-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
				A2 = A1;
			A=min(A1,A2); 
			if (A<=0) A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
		}
		if ((fabs((A-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance)/	
			mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)>1.0)
			&&(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance<120000))
		{
			if ((A - mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance) <0)
				A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					- 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
			else 
				A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance 
					+ 0.5* mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
		}
		
		if (1.0<fabs(C*sinA/A))
			worked = false;
		else
		{
			double B1 = C*cosA - sqrt(A*A - C*C*sinA*sinA);
			double B2 = C*cosA + sqrt(A*A - C*C*sinA*sinA);

			cout <<  "	B1=" << B1 << "	B2=" << B2 << endl;

			if (B1<0) B1=B2;
				else if (B2<0) B2=B1;
			B = min(B1,B2);
			if (B<=0) worked=false;
			else
			{
				newTestPoint.sEstimatedLocation.FromHere(
					mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation, B, OtherSiteAzi);
				oldEst = newTestPoint.sEstimatedLocation;
				newTestPoint.sErrorEstimate = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;
			
				newTestPoint.sNewTP = mNewTP;	
				mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
				mNewTP++;
			}
	}
		if (haveOwnAngle)
		{
			worked =true;
			tTestPoint newTestPoint;
			newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
			newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
			newTestPoint.sMethodUsed = CosRuleAngleAngle;
			beta = (servSiteAzi-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Bearing
					(mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation));
			if (beta >180) beta -=360;
			if (beta <-180) beta += 360;

			gamma = 180 - alpha -beta;
			cosB = cos(beta*PI/180);
			cosC = cos(gamma*PI/180);

			A = (C*cosC*cosA+C*cosB)/(1-cosC*cosC);
			B = (C*cosC*cosB+C*cosA)/(1-cosC*cosC);
			newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, A, servSiteAzi);
			newTestPoint.sErrorEstimate = oldEst.Distance(newTestPoint.sEstimatedLocation);
			newTestPoint.sNewTP = mNewTP;	
			mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
			mNewTP++;
		}
	}

	return worked;	
}


//******************************************************************************************************************************
bool cPosEstimation::DCM_ParticleSwarm()
{
	unsigned i,j;

	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
	newTestPoint.sMethodUsed = DCM_PSO;

	// Initialise swarm
	// make position coordinates rho (in meters) and phi (cylindrical coordinates)

	double* rho;
	rho = new double[NUMPARTICLES];
	double* phi;
	phi = new double[NUMPARTICLES];
	double* value;
	value = new double[NUMPARTICLES];
	double* pbestValue;
	pbestValue = new double[NUMPARTICLES];
	double* pbestRho;
	pbestRho = new double[NUMPARTICLES];
	double* pbestPhi;
	pbestPhi = new double[NUMPARTICLES];
	double* rho_snelheid;
	rho_snelheid = new double[NUMPARTICLES];
	double* phi_snelheid;
	phi_snelheid = new double[NUMPARTICLES];
	double gbestValue, gbestRho, gbestPhi;
	double tempvalue = 0.5;

	double rho_max = 120001.0;
	double rho_min = 0.0;
	if (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance > 120000)
	{
		if (mPosSets[mCurrentPosSetIndex].sMeasurements.size()>1)
		{
			rho_max = 
			mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.Distance
			(mPosSets[mCurrentPosSetIndex].sMeasurements[1].sSiteLocation);
		}
		else rho_max = 35000;
		rho_min = 0;
	}
	else
	{
		rho_max = (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance
				/mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist+0.5)*
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist+50.0;
		rho_min = max(0.0,(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance
				/mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist-0.5)
				*mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist-50.0);
	}	
	if (rho_max<6*mPlotResolution) rho_max = 6*mPlotResolution;
	cout << "rho_min=" << rho_min << "	rho_max=" << rho_max << endl; 

    	std::random_device RhoRD;
    	std::mt19937_64 Rho_engine(RhoRD());
  	   std::exponential_distribution<double> rho_distE(1);
    	std::random_device RhoRDU;
    	std::mt19937_64 Rho_engineU(RhoRDU());
	   uniform_real_distribution<double> rho_distU(0,1);

	double phi_min = -180;
	double phi_max = 180;
	phi_min = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sAzimuth
		- (ceil)(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth/2.0);
	phi_max = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sAzimuth
		+ (ceil)(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth/2.0);	

    	std::random_device PhiRD;
    	std::mt19937_64 Phi_engine(PhiRD());
	uniform_real_distribution<double> phi_dist(phi_min,phi_max);

	gbestValue = MAXDOUBLE;

//	cout << " cPosEstimation::DCM_ParticleSwarm() VOOR initialisation loop" << endl; 
	for (i=0; i<NUMPARTICLES; i++)
	{
		tempvalue = 2;
		if (i>NUMPARTICLES/2)
			tempvalue = rho_distU(Rho_engineU);
		else
		{
			while (tempvalue>1)
				tempvalue = rho_distE(Rho_engine);
		}
		rho[i] = rho_min + (1.0 - tempvalue) * (rho_max - rho_min);
		phi[i] = phi_dist(Phi_engine);
		if (rho[i]<0)
		{
			rho[i]*=-1;
			if (phi[i]>=180.0)
				phi[i]-=180.0;
			else phi[i]+=180.0;
		}
		if (rho[i]<2.0*mPlotResolution) rho[i] = 2.0*mPlotResolution;
		if (phi[i]>180)
			phi[i]-=360;
		else if (phi[i]<-180)
			phi[i]+=360;

		value[i] = CostFunction(rho[i], phi[i]);
		
//		cout << "i=" << i << "	tempvalue=" << tempvalue <<
//			"	rho[i]=" << rho[i] << "	phi[i]=" << phi[i] << "	Cost=" << value[i]<< endl;

		phi_snelheid[i] = 0.0;
		rho_snelheid[i] = -0.001;
		pbestRho[i] = rho[i];
		pbestPhi[i] = phi[i];
		pbestValue[i] = value[i];
//		cout << 0 <<"	i=" <<i << "	rho=" << rho[i] << "	phi=" << phi[i] << "	value=" << value[i] << endl;
		if (pbestValue[i] < gbestValue)
		{
			gbestValue = pbestValue[i];					
			gbestRho = pbestRho[i];
			gbestPhi = pbestPhi[i];
		}
	} 

/*	newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, gbestRho, gbestPhi);
	newTestPoint.sEstimatedLocation.Display();
	cout << gbestRho << endl;
*/	

	//Searching .... using Global Best

	double oldBestValue = gbestValue;
	unsigned iterationN = 0;
	unsigned LastChangedN = 0;
	double oldBestRho = gbestRho;
	double oldBestPhi = gbestPhi;

    	std::random_device RpRhoRD;
    	std::mt19937_64 RpRho_engine(RpRhoRD());
	std::uniform_real_distribution<double> RpRho_dist(0.0,1.0);

    	std::random_device RgRhoRD;
    	std::mt19937_64 RgRho_engine(RgRhoRD());
	std::uniform_real_distribution<double> RgRho_dist(0.0,1.0);

    	std::random_device RpPhiRD;
    	std::mt19937_64 RpPhi_engine(RpPhiRD());
	std::uniform_real_distribution<double> RpPhi_dist(0.0,1.0);

    	std::random_device RgPhiRD;
    	std::mt19937_64 RgPhi_engine(RgPhiRD());
	std::uniform_real_distribution<double> RgPhi_dist(0.0,1.0);

	bool stop = false;
	bool change = false;
	LastChangedN = 0;
	double rhoR=0, phiR=0;

	iterationN = 0;
	while (!stop)
	{
		iterationN++;
//		cout << "iterationN=" << iterationN << endl;
		for (i=0; i< NUMPARTICLES; i++)
		{
			change = false;
			if ((value[i]<pbestValue[i])&&(rho[i]>rho_min)&&(rho[i]<rho_max))
			{
				change = true;
/*				if ((phi[i]>phi_min-mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth/2.0)
					&&(phi[i]<phi_max+mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth/2.0))
				{
					change = true;
				for (j=1; j<mPosSets[mCurrentPosSetIndex].sMeasurements.size(); j++)
					{
						change = change && ((mCellPathLoss[0]+MARGIN) > mCellPathLoss[j]);
					}
//					if (change) cout << "changing" << endl;
//					else cout << "not changing" << endl;
				}
*/
			}
			if (change)
			{
				pbestValue[i] = value[i];
				pbestRho[i] = rho[i];
				pbestPhi[i] = phi[i]; 
				if (pbestValue[i] < gbestValue)
				{
					cout << "Found better" << endl;
					stop = (( ( (pbestValue[i]-oldBestValue) < DELTA) &&(iterationN>STOPN/3))|| ((iterationN-LastChangedN)>STOPN));
					oldBestValue = gbestValue;
					oldBestRho = gbestRho;
					oldBestPhi = gbestPhi;
					LastChangedN = iterationN;
					gbestValue = pbestValue[i];					
					gbestRho = pbestRho[i];
					gbestPhi = pbestPhi[i];
				}
			}

			stop = stop||((iterationN-LastChangedN) > STOPN); 

			rho_snelheid[i] = INERTIA*rho_snelheid[i] 
					+ Cp*RpRho_dist(RpRho_engine)*(pbestValue[i] - value[i])
					+ Cg*RgRho_dist(RgRho_engine)*(gbestValue - value[i]);
			phi_snelheid[i] = INERTIA*phi_snelheid[i] 
					+ Cp*RpPhi_dist(RpPhi_engine)*(pbestValue[i] - value[i])
					+ Cg*RgPhi_dist(RgPhi_engine)*(gbestValue - value[i]);			
			rho[i] = rho[i] + rho_snelheid[i];
			phi[i] = phi[i] + phi_snelheid[i];

			phi[i] = fmod(phi[i],360);		
			if (phi[i]>180)
				phi[i]-=360;
			else if (phi[i]<-180)
				phi[i]+=360;

			if (rho[i]<0)
			{
				rho[i]*=-1;
				if (phi[i]>=180.0)
					phi[i]-=180.0;
				else phi[i]+=180.0;
			}
			if ((rho[i]>rho_max*4)||(rho[i]>120000)||(rho[i]<3*mPlotResolution))
			{
				tempvalue = 2;
				while (tempvalue>1)
					tempvalue = rho_distE(Rho_engine);
				rho[i] = rho_min + (1.0 - tempvalue) * (rho_max - rho_min);	
				rho_snelheid[i] = 0;
				phi[i] = phi_dist(Phi_engine);
				phi_snelheid[i] = 0;		
			}
			if (rho[i]<3.0*mPlotResolution) rho[i] = 3.0*mPlotResolution;
			value[i] = CostFunction(rho[i], phi[i]);
			cout << iterationN <<"		i=" <<i << "		rho=" << rho[i] << "		phi=" << phi[i] << "		value=" << value[i] << endl;

		}// end for
	}// end while

	cGeoP BeforeLast(-25.7, 28.2, DEG);
	newTestPoint.sEstimatedLocation.FromHere(
				mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, gbestRho, gbestPhi);
	BeforeLast.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, oldBestRho, oldBestPhi);
	newTestPoint.sErrorEstimate = newTestPoint.sEstimatedLocation.Distance(BeforeLast);	
	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;

	delete [] rho;
	delete [] phi;
	delete [] pbestRho;
	delete [] pbestPhi;
	delete [] value;
	delete [] pbestValue;
	delete [] rho_snelheid;
	delete [] phi_snelheid;

	return (!stop);
}


//******************************************************************************************
double cPosEstimation::CostFunction(double rho, double phi)
{
	unsigned i;
	unsigned NumUsed = mNumInsts;
	double* Delta;
	Delta = new double[mNumInsts];
	double* Prediction;
	Prediction = new double[mNumInsts];
	delete [] mCellPathLoss;
	mCellPathLoss = new double[mNumInsts];
	double DiffLoss=0, Azimuth, AntValue;
	float Tilt=0;
	cGeoP ParticlePosition(-25.7, 28.2, DEG);

	ParticlePosition.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, rho, phi);
//	cout << "rho = " << rho << "	phi = " << phi << endl;
	for (i=0; i<mNumInsts; i++)
	{
//			mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteLocation.Display();
//			cout << "i = " << i << "	mCurrentPosSetIndex =" << mCurrentPosSetIndex << endl;
			mDEM.GetForLink(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteLocation,
													ParticlePosition, mPlotResolution, mDEMProfile);

			if (mUseClutter)
			{
				mClutter.GetForLink(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteLocation,
															ParticlePosition, mPlotResolution, mClutterProfile);
			}

			mPathLoss.setParameters(mkFactor,mPosSets[mCurrentPosSetIndex].sMeasurements[i].sFrequency,
						mPosSets[mCurrentPosSetIndex].sMeasurements[i].sHeight, MOBILEHEIGHT,
						mUseClutter, mClutterClassGroup);
			mCellPathLoss[i] = mPathLoss.TotPathLoss(mDEMProfile, Tilt, mClutterProfile, DiffLoss);
			Azimuth = mPosSets[mCurrentPosSetIndex].sMeasurements[i].sSiteLocation.Bearing(ParticlePosition);
//			cout << "Tilt = " << Tilt << "	Azimuth = " << Azimuth << endl;
			AntValue = mFixedAnts[i].GetPatternValue(Azimuth, Tilt);
			Prediction[i] =  mPosSets[mCurrentPosSetIndex].sMeasurements[i].sEIRP 
										- mCellPathLoss[i] - AntValue;
//			if (Prediction[i] > SENSITIVITY)
				Delta[i] = (mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue - Prediction[i]) 
								* (mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue - Prediction[i]);
/*			else
			{
				Delta[i] = 0;
				NumUsed --;
			}
*/
//			cout << i << "	meas=" << mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue
//						<< "	prediction =" << prediction << "	Delta[i]=" << Delta[i] << endl;
	}


	double Cost=0.0, Sexp=1.0, Pexp=1.0, Aexp=1.0;
	for (i=0; i<mNumInsts; i++)
	{
/*		if (i>0) Cost += Delta[i-1] + Delta[i] 
			     -2*((mPosSets[mCurrentPosSetIndex].sMeasurements[i-1].sMeasValue-Prediction[i-1])
				*(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue-Prediction[i]));
		else Cost += Delta[i];
*/
		Cost += Delta[i];

/*		if (i>0) Aexp = Aexp*exp(Delta[i-1] + Delta[i] 
			     -2*((mPosSets[mCurrentPosSetIndex].sMeasurements[i-1].sMeasValue-Prediction[i-1])
			*(mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue-Prediction[i]))/SIGMA2);
		cout << SIGMA2 << "	"	<<exp(-Delta[i]/(SIGMA2)) << endl;
*/
				Sexp = Sexp*exp(-Delta[i]/(SIGMA2));
	}

	Pexp = pow(Sexp, (double)(1.0/(double)NumUsed));

	cout <<"	Cost=" << Cost << "	Sexp=" << Sexp << "	mNumInsts=" << mNumInsts << "		Pexp=" << Pexp << endl;
	double Pcost = 1.0 - Pexp;

	delete [] Delta;
	delete [] Prediction;
//	return Cost;
	return Pcost;
}

//*******************************************************************************************************************************
int cPosEstimation::SaveResults()
{
	unsigned i,j;
	string query;
	char * temp;
	temp = new char[33];
	double Lat, Lon;
	QString PosString;

	string queryM = "INSERT into testpoint (id, originaltp, positionsource, location) Values (";

	for (i=0; i<mNumPoints; i++)
	{
		for (j=1; j < mPosSets[i].sTestPoints.size(); j++)
		{
			query = queryM;
			gcvt(mPosSets[i].sTestPoints[j].sNewTP,9,temp);
			query += temp;
	
			query += ", ";
			gcvt(mPosSets[i].sTestPoints[j].sOriginalTP,9,temp);
			query += temp;
	
			query += ", ";
			gcvt(mPosSets[i].sTestPoints[j].sMethodUsed+1,9,temp);
			query += temp;
	
			mPosSets[i].sTestPoints[j].sEstimatedLocation.Get(Lat,Lon);
			PosString=QString(",ST_GeomFromText('POINT(%1 %2)',4326)").arg(Lon).arg(Lat);
			query += PosString.toStdString();
			query +=");";

			if (!gDb.PerformRawSql(query))
			{
				string err = "Error inserting TestPoint by running query: ";
				err += query;
				cout << err <<endl; 
				QRAP_WARN(err.c_str());
				return false;
			}
		}

	}

	delete [] temp;
	return true;
}


