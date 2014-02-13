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
		mUseClutter = mClutter.SetRasterFileRules(mClutterSource);
	if (mUseClutter)
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	mUseClutter = (mUseClutter)&&(mClutterClassGroup>0);
	if (mUseClutter) mClutterCount = new unsigned[mPathLoss.mClutter.mNumber];
	else mClutterCount = new unsigned[2];
	if (mUseClutter) mClutter.SetSampleMethod(1);

	mDEMsource = atoi(gDb.GetSetting("DEMsource").c_str());
	cout << "mDEMsource = " << mDEMsource << endl;
	mDEM.SetRasterFileRules(mDEMsource);
	mDEM.SetSampleMethod(2);
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
}

//*********************************************************************
bool cPosEstimation::LoadMeasurements(QList<QgsPoint> Points,
					unsigned MeasType, 
					unsigned MeasSource,
					unsigned PosSource,
					unsigned Technology)
{
	pqxx::result r;
	unsigned tp = 0;
	unsigned NumInPosSet = 0;
	char *text= new char[33];
	cGeoP NorthWestCorner,SouthEastCorner; 
	if (Points.size() > 1)
	{
		double North = Points[0].y();
		double South = Points[0].y();
		double East = Points[0].x();
		double West = Points[0].x();
		string query;
		pqxx::result MeasSelect;

		query ="SELECT tp, ST_AsText(testpoint.location) as origLocation, measurement.id as id, ";
		query += "site.id as siteid, ST_AsText(site.location) as siteLocation, radioinstallation.id as InstKeyFixed, ";
		query += "txantennaheight, EIRP, txbearing, txtilt, txantpatternkey, azibeamwidth, ";
		query += "ci, centriod, frequency, measvalue, pathloss, min(testpointauxUTRAN.RxTxDiff) as timeDiff1, ";
		query += "min(testpointauxUTRAN.RxTxDiff2) as timeDiff2, testpointauxGSM.TA as TA, technology.DistRes as DistRes ";
		query += "from measurement left join testpointauxUTRAN ";
		query += "on (measurement.tp=testpointauxUTRAN.tp and measurement.ci=testpointauxUTRAN.servci) ";
		query += "left join testpointauxGSM ";
		query += "on (measurement.tp=testpointauxGSM.tp and measurement.ci=testpointauxGSM.servci) ";
		query += "cross join testpoint on (measurement.tp=testpoint.id) ";
		query += "cross join cell on (measurement.ci=cell.id) ";
		query += "cross join radioinstallation on (cell.risector=radioinstallation.id) ";
		query += "cross join antennapattern on (radioinstallation.txantpatternkey = antennapattern.id) ";
		query += "cross join site on (radioinstallation.siteid=site.id) ";
		query += " LEFT OUTER JOIN technology ON (radioinstallation_view.techkey=technology.id) ";
		query += " where  siteLocation @ ST_GeomFromText('POLYGON((";
		for (int i = 0 ; i < Points.size();i++)
	        {
	        	North = max(North,Points[i].y());
	        	South = min(South,Points[i].y());
	        	East = max(East,Points[i].x());
	        	West = min(West,Points[i].x());
			gcvt(Points[i].x(),12,text);
	        	query += text;
	        	query += " ";
			gcvt(Points[i].y(),12,text);
	        	query += text;
	        	query += ",";
	        }
	        NorthWestCorner.Set(North,West);
	        SouthEastCorner.Set(South,East);
		cout << "North West corner: " << endl;
		NorthWestCorner.Display();
		cout << "South East corner: " << endl;
		SouthEastCorner.Display();
		gcvt(Points[0].x(),12,text);
	        query += text;
	        query += " ";
		gcvt(Points[0].y(),12,text);
	        query += text;
	        query += "))',4326) order by radioinstallation_view.id;";

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
		query+=" group by measurement.tp, measurement ci "; 
		query+=" order by measurement.tp, timeDiff1, timeDiff2, TA, measvalue;";


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
			for (int i = 0; i < r.size();i++)
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
				else NumInPosSet++; 
				NewMeasurement.sOriginalTP = tp;
				NewMeasurement.sID = atoi(r[i]["id"].c_str());
				NewMeasurement.sSiteID = atoi(r[i]["siteid"].c_str());
				PointString = r[i]["siteLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				NewMeasurement.sSiteLocation.Set(latitude,longitude,DEG);
				NewMeasurement.sCellID = atoi(r[i]["ci"].c_str());
				PointString = r[i]["centroid"].c_str();
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
				NewMeasurement.sTilt = atof(r[i]["txtilt"].c_str());
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
				NewPosSet.sMeasurements.push_back(NewMeasurement);
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
		cout << "cPosEstimation::LoadMeasurement: leaving ... happy ... " << endl << endl;
		return true;
	}
	delete [] text;
	cout << "cPosEstimation::LoadMeasurement: leaving ... not enough points in selecting area." << endl << endl;
	return false;
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
	if (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sBeamWidth > 270)
		return false;

	unsigned i;
	double Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance; 
	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;

	newTestPoint.sMethodUsed = CellID_RxTx;

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
	
	double Distance = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
	if (Distance < mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist)
		Distance = (mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
				Distance(mPosSets[mCurrentPosSetIndex].sMeasurements[ClosestIndex].sSiteLocation))/4;
	double Azimuth = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation.
				Bearing(mPosSets[mCurrentPosSetIndex].sMeasurements[ClosestIndex].sSiteLocation);

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

	double Distance= mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;

	Band.sFrequency = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sFrequency;
	Band.sAIndex = 0;
	Band.sBIndex = -1;
	Band.sMaxMeasValue = -120; 
	Bands.push_back(Band);

	unsigned i,j;

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
	while ((-1==Bands[j].sBIndex)&&(j<Bands.size()))
		j++;

	if (j>=Bands.size())
	{
		Bands.clear();
	 	return false;
	}

	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
	newTestPoint.sMethodUsed = CoSiteSecDir;

	unsigned AIndex = Bands[j].sAIndex;
	unsigned BIndex = Bands[j].sBIndex;
	Bands.clear();
	
	minAzi = FindAzi(BIndex, AIndex);
	
	newTestPoint.sEstimatedLocation.FromHere(mPosSets[mCurrentPosSetIndex].sMeasurements[0].sSiteLocation, Distance, minAzi);

	double DegRes = min(5.0,max(90*mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sResDist/Distance/PI,1.0));
	newTestPoint.sErrorEstimate = max(((double)mPosSets[mCurrentPosSetIndex].sMeasurements[AIndex].sResDist)/2.0,DegRes*PI*Distance/180);

	newTestPoint.sNewTP = mNewTP;	
	mPosSets[mCurrentPosSetIndex].sTestPoints.push_back(newTestPoint);
	mNewTP++;
	return true;
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

	 	if ((Delta<minDelta)&&(LeftAntValue<20)&&(RightAntValue<20))
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
						{
							if (mPosSets[mCurrentPosSetIndex].sMeasurements[i].sMeasValue 
								> Bands[j].sMaxMeasValue)
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
		}
	

		j=0;
		while ((-1==Bands[j].sBIndex)&&(j<Bands.size()))
			j++;

		if (j>=Bands.size())
			Bands.clear();
		else
		{ 
			AIndex = Bands[j].sAIndex;
			BIndex = Bands[j].sBIndex;
			Bands.clear();
			stop = true;
		}
	}

	int OtherAziIndex=-1;
	if (stop) OtherAziIndex = AIndex;

	int OtherDistanceIndex=-1;
	double OtherSiteDistance = 999999;

	for (k=1; (k<mPosSets[mCurrentPosSetIndex].sMeasurements.size())&&(-1==OtherDistanceIndex); k++)
		if (mPosSets[mCurrentPosSetIndex].sMeasurements[k].sDistance < 300000)
		{
			OtherDistanceIndex = k;
			OtherSiteDistance = mPosSets[mCurrentPosSetIndex].sMeasurements[k].sDistance;
		}


	if ((-1==OtherAziIndex)&&(-1==OtherDistanceIndex))
		return false;


	tTestPoint newTestPoint;
	newTestPoint.sOriginalTP = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalTP;
	newTestPoint.sOriginalLocation = mPosSets[mCurrentPosSetIndex].sTestPoints[0].sOriginalLocation;
	newTestPoint.sMethodUsed = CosineRule;


	double A, B, C, alpha, beta, gamma, onderwortel, cosB, cosC, cosA, sinA, sinB;
	cGeoP oldEst;
	bool worked = false;
	unsigned OtherSiteIndex;

// If one has the Rx-Tx from active sets from different sites
	if (-1!=OtherDistanceIndex)
	{
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
	}

// Have other site angle
	if (-1!=OtherAziIndex)
	{
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
	
		cosA = cos(alpha*PI/180);
		sinA = sin(alpha*PI/180);
		
		A = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sDistance;
		B = C*cosA - sqrt(A*A-C*C*sinA*sinA);

		newTestPoint.sEstimatedLocation.FromHere(
			mPosSets[mCurrentPosSetIndex].sMeasurements[OtherSiteIndex].sSiteLocation, B, OtherSiteAzi);
		oldEst = newTestPoint.sEstimatedLocation;
		newTestPoint.sErrorEstimate = mPosSets[mCurrentPosSetIndex].sMeasurements[0].sResDist;

		if (haveOwnAngle)
		{
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
		}
	}

	return worked;	
}


//************************************************************************************************************************************
int cPosEstimation::SaveResults()
{
	unsigned i,j;
	string query;
	char * temp;
	temp = new char[33];
	double Lat, Lon;
	QString PosString;

	string queryM = "INSERT into testpoint (id, originaltp, positionsource, location) Values";

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
			gcvt(mPosSets[i].sTestPoints[j].sMethodUsed,9,temp);
			query += temp;
	
			mPosSets[i].sTestPoints[j].sEstimatedLocation.Get(Lat,Lon);
			PosString=QString(",ST_GeomFromText('POINT(%1 %2)',4326),").arg(Lon).arg(Lat);
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


