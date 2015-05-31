 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cTrainPosNetDistAngle.cpp
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


#include "cTrainPosNetDistAngle.h"
#include <random>


//*********************************************************************
cTrainPosNetDistAngle::cTrainPosNetDistAngle() // default constructor
{
	
	mLTEsim = false;
	mOriginal = false; 
	mUMTS = true;

}

//*********************************************************************
cTrainPosNetDistAngle::~cTrainPosNetDistAngle() // destructor
{
	unsigned i,j;
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle() " << endl;
/*	for (i=0;i<mNumTrain;i++)
	{
		mPosSetsTrain[i].sTestPoints.clear();
		mPosSetsTrain[i].sMeasurements.clear();
	}
*/	mPosSetsTrain.clear();

/*	for (i=0;i<mNumTest;i++)
	{
		mPosSetsTest[i].sTestPoints.clear();
		mPosSetsTest[i].sMeasurements.clear();
	}
*/	mPosSetsTest.clear();
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle(): mPosSets cleared " << endl;

	for (i=0; i<mNumSites; i++)
	{
		cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle():  Site=" << i << endl;
		for (j=0; j<mSites[i].sNumInputs ;j++)
		{
			delete [] mSites[i].sInputTrain[j];
			delete [] mSites[i].sInputTest[j];
		}
		for (j=0; j<mSites[i].sNumOutputsA ;j++)
		{
			delete [] mSites[i].sOutputAngleTrain[j];
			delete [] mSites[i].sOutputAngleTest[j];	
		}
		for (j=0; j<mSites[i].sNumOutputsD ;j++)
		{
			delete [] mSites[i].sOutputDistTrain[j];
			delete [] mSites[i].sOutputDistTest[j];	
		}
		delete [] mSites[i].sInputTrain;
		delete [] mSites[i].sOutputAngleTrain;	
		delete [] mSites[i].sOutputDistTrain;
		delete [] mSites[i].sInputTest;
		delete [] mSites[i].sOutputAngleTest;	
		delete [] mSites[i].sOutputDistTest;
		mSites[i].sCellSet.clear();
		cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle():  Site=" << i << "	done" << endl;
	}
	mSites.clear();
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle(): mSites cleared " << endl;
}

//*********************************************************************
bool cTrainPosNetDistAngle::LoadSites(vPoints Points,
					unsigned MeasType, 
					unsigned MeasSource,
					unsigned PosSource,
					unsigned Technology)
{

	if (Points.size() < 2)
	{
		cout << "cTrainPosNetDistAngle::LoadMeasurements   Not enough points to define area. " << endl;
		return false;
	}

	pqxx::result r;
	double Lat, Lon, mNorth, mSouth, mEast, mWest;
	char *text= new char[33];
	cGeoP NorthWestCorner,SouthEastCorner; 
	Points[0].Get(mNorth,mEast);
	mSouth = mNorth;
	mWest = mEast;
	string query, areaQuery;
	pqxx::result MeasSelect;
	unsigned i;

	double longitude, latitude;
	string PointString;
	unsigned spacePos;
	unsigned NumInPosSet = 0; 
	unsigned siteid;
	
	areaQuery += " @ ST_GeomFromText('POLYGON((";
	for (i = 0 ; i < Points.size();i++)
  	{
		Points[i].Get(Lat, Lon);
     	mNorth = max(mNorth,Lat);
     	mSouth = min(mSouth,Lat);
     	mEast = max(mEast,Lon);
     	mWest = min(mWest,Lon);
		gcvt(Lon,12,text);
     	areaQuery += text;
     	areaQuery += " ";
		gcvt(Lat,12,text);
     	areaQuery += text;
     	areaQuery += ",";
  	}

  	NorthWestCorner.Set(mNorth,mWest,DEG);
  	SouthEastCorner.Set(mSouth,mEast,DEG);
	cout << "North West corner: " << endl;
	NorthWestCorner.Display();
	cout << "South East corner: " << endl;
	SouthEastCorner.Display();
	Points[0].Get(Lat,Lon);
	gcvt(Lon,12,text);
  	areaQuery += text;
  	areaQuery += " ";
	gcvt(Lat,12,text);
  	areaQuery += text;
  	areaQuery += "))',4326) ";

	query = "select siteid, sitelocation, ci from ";
	query += "(select siteid, ST_AsText(site.location) as siteLocation, ";
	query += "ci, count(measurement.tp) as numPoints ";
	query += "from measurement cross join testpointauxGSM cross join cell ";
	query += "cross join radioinstallation cross join site ";
	query += "where servci=cell.id and cell.risector = radioinstallation.id ";
	query += "and measurement.tp=testpointauxGSM.tp and siteid =site.id ";
	query += "and site.location";
	query += areaQuery;
	query += "group by siteid, ci, site.location ";
	query += "order by siteid, numPoints desc) as lys ";
	query += "order by siteid, numPoints desc;";
	if (!gDb.PerformRawSql(query))
	{
		string err = "cTrainPosNetDistAngle::LoadSites: ";
		err +="Problem with database query to get sites from selected area! Problem with query: ";
		err += query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		delete [] text;
		return false;
	}

	cout << query << endl;
	unsigned Counter =0;
	tCell NewCell;
	tSiteInfoNN NewSite;
	NewCell.sCI=0;
	NewSite.sSiteID=0;
	NewSite.sMaxDist = 0;
	NewSite.sMedianDist = 900;

	gDb.GetLastResult(r);
	cout << "cTrainPosNetDistAngle::LoadSites: Na query." << endl;

	cout << "cTrainPosNetDistAngle::LoadSites:r.size() ." << r.size()  << endl;

	if (r.size() >0)
	{
		for (i=0; i<r.size(); i++)
		{
			siteid = atoi(r[i]["siteid"].c_str());
//			cout << "cTrainPosNetDistAngle::LoadSite: siteid = " << siteid << endl;
			if (siteid != NewSite.sSiteID)
			{
				if (NewSite.sSiteID>0)
				{
//					cout << "cTrainPosNetDistAngle::Sites: Site in list. SiteID = " 
//						<< NewSite.sSiteID << endl;
					NewSite.sNumInputs = 3*NewSite.sCellSet.size() + 5;
					mSites.push_back(NewSite);
					NewSite.sCellSet.clear();
					NewSite.sMaxDist = 0;
					NewSite.sMedianDist = 900;
					Counter = 0;
				}
				NewSite.sSiteID = siteid;
				PointString = r[i]["siteLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				NewSite.sPosition.Set(latitude,longitude,DEG);
				NewSite.sNumOutputsA = 2;
				NewSite.sNumOutputsD = 1;
				NewSite.sNumDataRowsTrain = 0;
				NewSite.sNumDataRowsTest = 0;
			}
			NewCell.sI =Counter;
			NewCell.sCI= atoi(r[i]["ci"].c_str());
			NewSite.sCellSet.push_back(NewCell);
			Counter++;
		}
		if (NewSite.sSiteID>0)
		{
			cout << "cTrainPosNetDistAngle::LoadMeasurements: Site in list. SiteID = " << NewSite.sSiteID << endl;
			NewSite.sNumInputs = 3*NewSite.sCellSet.size() + 5;
			mSites.push_back(NewSite);
			NewSite.sCellSet.clear();
		}
	}
	else
	{
		string err = "cTrainPosNetDistAngle::LoadMeasurements: ";
		err +="Query to get site info from selected area is empty! Problem with query: ";
		err += query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		delete [] text;
		return false;
	}
	mNumSites = mSites.size();

/*	cout << "cTrainPosNetDistAngle::LoadMeasurements  Done with Site List. mNumSites = " << mNumSites << endl;
	for (i=0; i<mNumSites; i++)
	{
		cout << "i=" << i << "	mSites[i].sSiteID = " << mSites[i].sSiteID;
		cout << "		mSites[i].sNumInputs = " << mSites[i].sNumInputs;
		cout << "		mSites[i].sNumOutputsA = " << mSites[i].sNumOutputsA;
		cout << "		mSites[i].sNumDataRowsTrain = " << mSites[i].sNumDataRowsTrain;
		cout << "		mSites[i].sNumDataRowsTest = " << mSites[i].sNumDataRowsTest << endl;
		mSites[i].sPosition.Display();
	}
*/
	return true;
}


//*********************************************************************
bool cTrainPosNetDistAngle::LoadMeasurements(vPoints Points,
					unsigned MeasType, 
					unsigned MeasSource,
					unsigned PosSource,
					unsigned Technology,
					string list,
					bool Train /* Traindata: true; Testdata: false */)
{

    	std::random_device Dist;
    	std::mt19937_64 LTEdistance(Dist());
//  	std::exponential_distribution<double> distError(1.5);
		std::normal_distribution<double> distError(0,1);
		double addDist;

	if (Points.size() < 2)
	{
		cout << "cTrainPosNetDistAngle::LoadMeasurements   Not enough points to define area. " << endl;
		return false;
	}

	pqxx::result r;
	double Lat, Lon, mNorth, mSouth, mEast, mWest;
	double tempdist;
	char *text= new char[33];
	cGeoP NorthWestCorner,SouthEastCorner; 
	Points[0].Get(mNorth,mEast);
	mSouth = mNorth;
	mWest = mEast;
	string query, areaQuery;
	pqxx::result MeasSelect;
	unsigned i;

	unsigned Counter =0;
	tCell NewCell;
	tSiteInfoNN NewSite;
	NewCell.sCI=0;
	NewSite.sSiteID=0;

	double longitude, latitude, Distance;
	string PointString;
	unsigned spacePos;
	unsigned siteIndex=0;
	unsigned NumInPosSet = 0; 
	unsigned siteid, tp;
	
	areaQuery += " @ ST_GeomFromText('POLYGON((";
	for (i = 0 ; i < Points.size();i++)
  	{
		Points[i].Get(Lat, Lon);
     	mNorth = max(mNorth,Lat);
     	mSouth = min(mSouth,Lat);
     	mEast = max(mEast,Lon);
     	mWest = min(mWest,Lon);
		gcvt(Lon,12,text);
     	areaQuery += text;
     	areaQuery += " ";
		gcvt(Lat,12,text);
     	areaQuery += text;
     	areaQuery += ",";
  	}
  	NorthWestCorner.Set(mNorth,mWest,DEG);
  	SouthEastCorner.Set(mSouth,mEast,DEG);
//	cout << "North West corner: " << endl;
//	NorthWestCorner.Display();
//	cout << "South East corner: " << endl;
//	SouthEastCorner.Display();
	Points[0].Get(Lat,Lon);
	gcvt(Lon,12,text);
  	areaQuery += text;
  	areaQuery += " ";
	gcvt(Lat,12,text);
  	areaQuery += text;
  	areaQuery += "))',4326) ";

	query = "select distinct siteid, min(txbearing) as antbearing, ci, ";
	query += "testpoint.id as tp, ST_AsText(testpoint.location) as origLocation, ";
	query += "measurement.id as mid, measvalue, frequency, EIRP, tp1.TA as TA, ";
	query += "technology.DistRes as DistRes ";
	query += "from measurement left outer join ";
	query += list;
	query += " as tp1 on ( measurement.tp =tp1.tp and measurement.ci = tp1.servci) ";
	query += "cross join testpoint cross join ";
	query += list;
	query += " as tp2 cross join cell cross join radioinstallation cross join technology ";
	query += "where measurement.tp=testpoint.id and tp2.servci = cell.id ";
	query += "and testpoint.id = tp2.tp and cell.risector = radioinstallation.id ";
	query += "and radioinstallation.techkey = technology.id ";
	query += "and testpoint.positionsource < 2 ";
	query += "and testpoint.location";
	query += areaQuery;
	
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
	query += " group by siteid, testpoint.id, tp1.TA, mid, ci, measvalue, frequency, EIRP, DistRes ";
	query += "order by siteid, tp, TA, measvalue desc;";
	cout << query << endl;

	if (!gDb.PerformRawSql(query))
	{
		string err = "cTrainPosNetDistAngle::LoadMeasurements: ";
		err +="Problem with database query to get measurements from selected area! Problem with query: ";
		err += query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		delete [] text;
		return false;
	}

	gDb.GetLastResult(r);
		
	cout << "cTrainPosNetDistAngle::LoadMeasurements:  r.size() = " << r.size() << endl;

	bool found=true;
	if (r.size() >0)
	{
		tTestPointNN NewTestPoint;
		tPosSetNN NewPosSet;
		tMeasNN NewMeasurement; 
		NewTestPoint.sOriginalTP=0;
		double onder = 0;
		double bo = 120000000;
				
		siteIndex = 0;
		cout << "cTrainPosNetDistAngle::LoadMeasurements:  r.size() = " << r.size() << endl;
		cout << "cTrainPosNetDistAngle::LoadMeasurements:  mNumSites = " <<mNumSites << endl;
		for (i=0; i<r.size(); i++)
		{
			siteid = atoi(r[i]["siteid"].c_str());
			if (siteid != mSites[siteIndex].sSiteID)
			{
				mSites[siteIndex].sMaxDist = mSites[siteIndex].sMaxDist+60;
/*				cout << "Max = " << mSites[siteIndex].sMaxDist << "	Median = " << mSites[siteIndex].sMedianDist;
				mSites[siteIndex].sMaxDist = min (mSites[siteIndex].sMaxDist+60, 2*mSites[siteIndex].sMedianDist);
				cout << "	mSites[siteIndex].sMaxDist = " << mSites[siteIndex].sMaxDist << endl;
*/				if (Train) 	mSites[siteIndex].sNumDataRowsTrain = Counter;
				else		mSites[siteIndex].sNumDataRowsTest = Counter;

				onder = 0;
				bo = 120000000;
				Counter = 0;
				while ((siteid != mSites[siteIndex].sSiteID)&&(siteIndex<mNumSites))
					siteIndex++;
				if (siteid != mSites[siteIndex].sSiteID)
				{
					siteIndex = 0;
					while ((siteid != mSites[siteIndex].sSiteID)&&(siteIndex<mNumSites))
						siteIndex++;
					if (siteid != mSites[siteIndex].sSiteID)
					{
//						cout << "cTrainPosNetDistAngle::LoadMeasurements: ";
//						cout << " Site in measurements not in sitelist:   ";
//						cout << " SiteID = " << siteid << endl;
						found =false;
					}
					else found=true;
				}
			}
//				cout << "siteIndex = " << siteIndex << endl;
			if (found)
			{
				tp = atoi(r[i]["tp"].c_str());
				if ((tp != NewTestPoint.sOriginalTP))
				{
					Counter++;
					if (NewTestPoint.sOriginalTP>0)
					{
						
						NewPosSet.sNumMeas = NewPosSet.sMeasurements.size();
						if (Train) mPosSetsTrain.push_back(NewPosSet);
						else mPosSetsTest.push_back(NewPosSet);
						NewPosSet.sTestPoints.clear();
						NewPosSet.sMeasurements.clear();
					}
					NumInPosSet = 0;
					NewTestPoint.sOriginalTP = tp;
					PointString = r[i]["origLocation"].c_str();
					spacePos = PointString.find_first_of(' ');
					longitude = atof((PointString.substr(6,spacePos).c_str())); 
					latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
					NewTestPoint.sOriginalLocation.Set(latitude,longitude,DEG);
					NewTestPoint.sServCellAzimuth = atof(r[i]["antbearing"].c_str());
		
//					NewTestPoint.sOriginalLocation.Display();
//					mSites[siteIndex].sPosition.Display();
					Distance =mSites[siteIndex].sPosition.Distance(NewTestPoint.sOriginalLocation);
					NewTestPoint.sResDist = atof(r[i]["DistRes"].c_str());
					NewTestPoint.sResDist = 553.5;
						
					if (mLTEsim)
					{
						NewTestPoint.sResDist = 4.88;
						addDist =  50.0*distError(LTEdistance);
						while ((Distance + addDist)<=0)
							addDist = 50.0*distError(LTEdistance);
						Distance = Distance + addDist;
					}

					if (mUMTS)
					{
						NewTestPoint.sResDist = 38;
						addDist =  50.0*distError(LTEdistance);
						while ((Distance + addDist)<=0)
							addDist = 50.0*distError(LTEdistance);
						Distance = Distance + addDist;
					}
//					cout << " Distance = " << Distance << endl;
				
					if (mOriginal) 
						NewTestPoint.sTA = atoi(r[i]["TA"].c_str());
					else
						NewTestPoint.sTA = floor(Distance/NewTestPoint.sResDist);


					if (Distance > mSites[siteIndex].sMaxDist)
						mSites[siteIndex].sMaxDist = Distance;
					if (!(Distance<onder)&&!(Distance>bo))
					{
						if (Distance<mSites[siteIndex].sMedianDist)
							bo = mSites[siteIndex].sMedianDist;
						else
							onder  = mSites[siteIndex].sMedianDist;
						mSites[siteIndex].sMedianDist = Distance;
					}

					tempdist = (NewTestPoint.sTA+0.5)*NewTestPoint.sResDist;
					if (tempdist > mSites[siteIndex].sMaxDist)
						mSites[siteIndex].sMaxDist = tempdist;
					if (!(tempdist<onder)&&!(tempdist>bo))
					{
						if (tempdist<mSites[siteIndex].sMedianDist)
							bo = mSites[siteIndex].sMedianDist;
						else
							onder  = mSites[siteIndex].sMedianDist;
						mSites[siteIndex].sMedianDist = tempdist;
					}
					
					NewTestPoint.sDistance = NewTestPoint.sOriginalLocation.
											Distance(mSites[siteIndex].sPosition);
					if (NewTestPoint.sDistance>mSites[siteIndex].sMaxDist)
								 mSites[siteIndex].sMaxDist = NewTestPoint.sDistance;
					NewTestPoint.sBearing = mSites[siteIndex].sPosition.
														Bearing(NewTestPoint.sOriginalLocation);
					NewPosSet.sTestPoints.push_back(NewTestPoint);
				}
				NewMeasurement.sOriginalTP = tp;
				NewMeasurement.sID = i;

				NumInPosSet++; 
				NewMeasurement.sCellID = atoi(r[i]["ci"].c_str());
				NewMeasurement.sFrequency = atof(r[i]["frequency"].c_str());
				NewMeasurement.sMeasValue = atof(r[i]["measvalue"].c_str());
				NewMeasurement.sEIRP = atof(r[i]["EIRP"].c_str());
				NewMeasurement.sRFDistEstimate = - NewMeasurement.sMeasValue + NewMeasurement.sEIRP
										-20 *log10(NewMeasurement.sFrequency);					
				NewPosSet.sMeasurements.push_back(NewMeasurement);
			}	
		}//	 end for number of entries
	

		NewPosSet.sNumMeas = NewPosSet.sMeasurements.size();

		if (Train)
		{
			mPosSetsTrain.push_back(NewPosSet);
			mNumTrain = mPosSetsTrain.size();
			mSites[siteIndex].sNumDataRowsTrain = Counter;
		}
		else
		{
			mPosSetsTest.push_back(NewPosSet);
			mNumTest = mPosSetsTest.size();
			mSites[siteIndex].sNumDataRowsTest = Counter;
		}
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

/*	cout << "cTrainPosNetDistAngle::LoadMeasurements  Done with Site List. mNumSites = " << mNumSites << endl;
	for (i=0; i<mNumSites; i++)
	{
		cout << "i=" << i << "	mSites[i].sSiteID = " << mSites[i].sSiteID;
		cout << "		mSites[i].sNumInputs = " << mSites[i].sNumInputs;
		cout << "		mSites[i].sNumOutputsA = " << mSites[i].sNumOutputsA;
		cout << "		mSites[i].sNumDataRowsTrain = " << mSites[i].sNumDataRowsTrain;
		cout << "		mSites[i].sNumDataRowsTest = " << mSites[i].sNumDataRowsTest << endl;
		mSites[i].sPosition.Display();
	}
*/
	delete [] text;
	cout << "cTrainPosNetDistAngle::LoadMeasurement: leaving ... happy ... " << endl << endl;
	return true;
}


//**************************************************************************************************************************
bool cTrainPosNetDistAngle::TrainANDSaveANDTest()
{
	FANN::neural_net	ANN;
	FANN::training_data	TrainDataDist;
	FANN::training_data	TrainDataAngle;
	FANN::training_data	TestDataDist;
	FANN::training_data	TestDataAngle;
	double TrainError=9999999, TestError=9999999;
	double minTrainError = MAXDOUBLE;
	double minTestError = MAXDOUBLE;

	unsigned i,j,k, p, q, TrainIndex, TestIndex, type;
	bool stop = false;
	string query, queryM, queryC, outdir, filename, queryD;
	char * temp;
	temp = new char[33];
	char * site;
	site = new char[33];
	char * annid;
	annid = new char[33];
	char * machineid;
	machineid = new char[33];
	gcvt(gDb.globalMachineID,8,machineid);

//	time_t now;
	unsigned newANNid, newCIlistID;

	outdir = gDb.GetSetting("OutputDir");
	if(outdir=="") outdir = "Data/Output/";

	query = "SELECT (MAX(id)+1) AS id FROM NeuralNet";
	pqxx::result r;
	gDb.PerformRawSql(query);
	gDb.GetLastResult(r);
	newANNid = atoi(r[0]["id"].c_str());

	query = "SELECT (MAX(id)+1) AS id FROM ANNInputlist";
	gDb.PerformRawSql(query);
	gDb.GetLastResult(r);
	newCIlistID = atoi(r[0]["id"].c_str());
 
	queryM = "INSERT into NeuralNet (id, Lastmodified, machineid, type, siteid, ";
	queryM += "MaxDist, numInputs, numOutputs, filename) Values (";
	queryC = "INSERT into ANNInputlist (id, siteid, annid, index, cellid) Values (";

	TrainIndex = 0;
	TestIndex = 0;

	for (i=0; i<mNumSites; i++)
//	for (i=29; i<mNumSites; i++)
	{
		cout << "i=" << i << "	mSites[i].sSiteID = " << mSites[i].sSiteID;
		cout << "		mSites[i].sNumOutputsA = " << mSites[i].sNumOutputsA;
		cout << "		mSites[i].sNumDataRowsTrain = " << mSites[i].sNumDataRowsTrain;
		cout << "		mSites[i].sNumDataRowsTest = " << mSites[i].sNumDataRowsTest;
		cout << "		mSites[i].sMaxDist = " << mSites[i].sMaxDist << endl;
		if (mSites[i].sNumDataRowsTrain > 130)
		{

			mSites[i].sNumOutputsA = 2;
			mSites[i].sNumOutputsD = 1;
	
			mSites[i].sInputTrain = new double*[mSites[i].sNumDataRowsTrain];
			mSites[i].sOutputAngleTrain = new double*[mSites[i].sNumDataRowsTrain];
			mSites[i].sOutputDistTrain = new double*[mSites[i].sNumDataRowsTrain];
			for (j=0; j < mSites[i].sNumDataRowsTrain; j++)
			{
				mSites[i].sInputTrain[j] = new double[mSites[i].sNumInputs];
				mSites[i].sOutputAngleTrain[j] = new double[mSites[i].sNumOutputsA];
				mSites[i].sOutputDistTrain[j] = new double[mSites[i].sNumOutputsD];
			}

//			cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before train loop " << endl;
			for (j=0; j<mSites[i].sNumDataRowsTrain; j++)
			{
				mSites[i].sInputTrain[j][1] = cos(mPosSetsTrain[TrainIndex].sTestPoints[0].sServCellAzimuth*PI/180);
				mSites[i].sInputTrain[j][2] = sin(mPosSetsTrain[TrainIndex].sTestPoints[0].sServCellAzimuth*PI/180);
				mSites[i].sInputTrain[j][3] = 2*(((double)mPosSetsTrain[TrainIndex].sTestPoints[0].sTA+0.5)
																	*mPosSetsTrain[TrainIndex].sTestPoints[0].sResDist 
																	- mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
				mSites[i].sInputTrain[j][4] = (mPosSetsTrain[TrainIndex].sTestPoints[0].sResDist)/600;
				mSites[i].sInputTrain[j][0] = 1;
	
				for (q=0; q<mSites[i].sCellSet.size(); q++)
				{
					mSites[i].sInputTrain[j][3*q+5] = -1; // scaled
					mSites[i].sInputTrain[j][3*q+6] = -1; // scaled
					mSites[i].sInputTrain[j][3*q+7] = (945+FREQ_OFFSET)*FREQ_SCALE; 	
				}

				mSites[i].sOutputDistTrain[j][0] = 2*(mPosSetsTrain[TrainIndex].sTestPoints[0].sDistance 
																				- mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
				mSites[i].sOutputAngleTrain[j][0] = cos(mPosSetsTrain[TrainIndex].sTestPoints[0].sBearing*PI/180);
				mSites[i].sOutputAngleTrain[j][1] = sin(mPosSetsTrain[TrainIndex].sTestPoints[0].sBearing*PI/180);
	
//				cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before train loop on Meas " << endl;

				for (p=0; p < mPosSetsTrain[TrainIndex].sNumMeas; p++)
				{
					q=0;
					while ((mSites[i].sCellSet[q].sCI!=mPosSetsTrain[TrainIndex].sMeasurements[p].sCellID)
						&&(q+1<mSites[i].sCellSet.size()))
						q++;
					if (q<mSites[i].sCellSet.size())
					if (mSites[i].sCellSet[q].sCI==mPosSetsTrain[TrainIndex].sMeasurements[p].sCellID)
					{
						mSites[i].sInputTrain[j][3*q+5] = (-mPosSetsTrain[TrainIndex].sMeasurements[p].sRFDistEstimate
										+RFDist_OFFSET)*MEAS_SCALE;
						if (fabs(mSites[i].sInputTrain[j][3*q+5])>1)
							mSites[i].sInputTrain[j][3*q+5]/=fabs(mSites[i].sInputTrain[j][3*q+5]);
						mSites[i].sInputTrain[j][3*q+6] = (mPosSetsTrain[TrainIndex].sMeasurements[p].sMeasValue 
										+MEAS_OFFSET)*MEAS_SCALE;
						if (fabs(mSites[i].sInputTrain[j][3*q+6])>1)
							mSites[i].sInputTrain[j][3*q+6]/=fabs(mSites[i].sInputTrain[j][3*q+6]);
						mSites[i].sInputTrain[j][3*q+7] = (mPosSetsTrain[TrainIndex].sMeasurements[p].sFrequency
										+FREQ_OFFSET)*FREQ_SCALE;
						if (fabs(mSites[i].sInputTrain[j][3*q+7])>1)
							mSites[i].sInputTrain[j][3*q+7]/=fabs(mSites[i].sInputTrain[j][3*q+7]);
					} 
				}
				TrainIndex++;
			}

			mSites[i].sInputTest = new double*[mSites[i].sNumDataRowsTest];
			mSites[i].sOutputAngleTest = new double*[mSites[i].sNumDataRowsTest];
			mSites[i].sOutputDistTest = new double*[mSites[i].sNumDataRowsTest];
			for (j=0; j < mSites[i].sNumDataRowsTest; j++)
			{
				mSites[i].sInputTest[j] = new double[mSites[i].sNumInputs];
				mSites[i].sOutputAngleTest[j] = new double[mSites[i].sNumOutputsA];
				mSites[i].sOutputDistTest[j] = new double[mSites[i].sNumOutputsD];
			}
	
//			cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before test loop " << endl;
			for (j=0; j<mSites[i].sNumDataRowsTest; j++)
			{
				mSites[i].sInputTest[j][1] = cos(mPosSetsTest[TestIndex].sTestPoints[0].sServCellAzimuth*PI/180);
				mSites[i].sInputTest[j][2] = sin(mPosSetsTest[TestIndex].sTestPoints[0].sServCellAzimuth*PI/180);
				mSites[i].sInputTest[j][3] = 2*(((double)mPosSetsTest[TestIndex].sTestPoints[0].sTA+0.5)
							*mPosSetsTest[TestIndex].sTestPoints[0].sResDist 
							- mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
				mSites[i].sInputTest[j][4] = (mPosSetsTrain[TrainIndex].sTestPoints[0].sResDist)/600;
				mSites[i].sInputTest[j][0] = 1;
	
				for (q=0; q<mSites[i].sCellSet.size(); q++)
				{
					mSites[i].sInputTest[j][3*q+5] = -1; // scaled
					mSites[i].sInputTest[j][3*q+6] = -1; // scaled
					mSites[i].sInputTest[j][3*q+7] = (945+FREQ_OFFSET)*FREQ_SCALE; 	
				}

				mSites[i].sOutputDistTest[j][0] = 2*(mPosSetsTest[TestIndex].sTestPoints[0].sDistance 
																				- mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
				mSites[i].sOutputAngleTest[j][0] = cos(mPosSetsTest[TestIndex].sTestPoints[0].sBearing*PI/180);
				mSites[i].sOutputAngleTest[j][1] = sin(mPosSetsTest[TestIndex].sTestPoints[0].sBearing*PI/180);

//					cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before test loop on Meas" << endl;

				for (p=0; p < mPosSetsTest[TestIndex].sNumMeas; p++)
				{
					q=0;
					while ((mSites[i].sCellSet[q].sCI!=mPosSetsTest[TestIndex].sMeasurements[p].sCellID)
						&&(q+1<mSites[i].sCellSet.size()))
						q++;
					if (q<mSites[i].sCellSet.size())
					if (mSites[i].sCellSet[q].sCI==mPosSetsTest[TestIndex].sMeasurements[p].sCellID)
					{
						mSites[i].sInputTest[j][3*q+5] = (-mPosSetsTest[TestIndex].sMeasurements[p].sRFDistEstimate
										+RFDist_OFFSET)*MEAS_SCALE;
						if (fabs(mSites[i].sInputTest[j][3*q+5])>1)
							mSites[i].sInputTest[j][3*q+5]/=fabs(mSites[i].sInputTest[j][3*q+5]);
						mSites[i].sInputTest[j][3*q+6] = (mPosSetsTest[TestIndex].sMeasurements[p].sMeasValue 
										+MEAS_OFFSET)*MEAS_SCALE;
						if (fabs(mSites[i].sInputTest[j][3*q+6])>1)
							mSites[i].sInputTest[j][3*q+6]/=fabs(mSites[i].sInputTest[j][3*q+6]);
						mSites[i].sInputTest[j][3*q+7] = (mPosSetsTest[TestIndex].sMeasurements[p].sFrequency
										+FREQ_OFFSET)*FREQ_SCALE;
						if (fabs(mSites[i].sInputTest[j][3*q+7])>1)
							mSites[i].sInputTest[j][3*q+7]/=fabs(mSites[i].sInputTest[j][3*q+7]);
					} 
				}
				TestIndex++;
			}
			cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before set ANN " << endl;
			
			TrainDataAngle.set_train_data(mSites[i].sNumDataRowsTrain, 
						mSites[i].sNumInputs, mSites[i].sInputTrain,
						mSites[i].sNumOutputsA, mSites[i].sOutputAngleTrain);

			TestDataAngle.set_train_data(mSites[i].sNumDataRowsTest, 
						mSites[i].sNumInputs, mSites[i].sInputTest,
						mSites[i].sNumOutputsA, mSites[i].sOutputAngleTest);

			unsigned HiddenN1 = ceil(sqrt(mSites[i].sNumOutputsA*(5 + 2.8*max(0,(int)mSites[i].sCellSet.size()) )));
			unsigned HiddenN2 = 7;
//			double FanIn=sqrt(3);
			double FanIn = 2.0;
			FanIn = pow(mSites[i].sNumInputs/HiddenN1,0.5);
			cout << "FanIn = " << FanIn << endl;
			cout << "HiddenN1 = " << 	HiddenN1 << endl;
			cout << "mSites[i].sNumInputs = " << 	mSites[i].sNumInputs << endl;
	

			ANN.create_standard(4, mSites[i].sNumInputs, 
													HiddenN1 ,HiddenN2, mSites[i].sNumOutputsA);
 			ANN.set_train_error_function(FANN::ERRORFUNC_LINEAR);
//			ANN.set_learning_rate(0.7);
			ANN.set_train_stop_function(FANN::STOPFUNC_MSE);
			ANN.set_training_algorithm(FANN::TRAIN_QUICKPROP);
			ANN.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
			ANN.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
			ANN.randomize_weights(-1/FanIn,1/FanIn);

			cout << "saving ANN: site = " << mSites[i].sSiteID << "	i=" << i << endl;
			gcvt(mSites[i].sSiteID,9,site);
			filename = outdir;
			filename += "/";
			filename += site;
			filename += "_";
			type = 1;		
			gcvt(type,9,temp);
			filename += temp;		
//			time (&now);
//			filename += ctime(&now);
			filename += ".ann";

//			ANN.train_on_data(TrainDataAngle, MAXepoch,REPORTInt,ERROR);

			cout << "In cTrainPosNetDistAngle::TrainANDSaveANDTest(): before ANN training " << endl;

			minTestError = 99999999;
			minTrainError = 99999999;
			stop = false;	
			k=0;
			while ((k<MAXepoch+1)&&(!stop))
			{
				TrainError = ANN.train_epoch(TrainDataAngle);
				TestError = ANN.test_data(TestDataAngle);
				if (0==k%REPORTInt)
				{
//					TestError = ANN.test_data(TestDataAngle);
//					TestError = ANN.get_MSE();
					cout << "siteid = " << mSites[i].sSiteID << "	k=" << k 
							<< "	TrainErr = " << TrainError 
							<< "	TestErr = " << TestError << endl;
				}
				if (((TrainError <= minTrainError)&&(TestError<=minTestError))
					||((TrainError<= minTrainError*0.6)&&(TestError<=(minTestError*1.1))&&(k>REPORTInt)))
				{
					ANN.save(filename);
					stop = (((minTestError-TestError)/TestError) < ERROR)
								&&(((minTrainError-TrainError)/TrainError) < ERROR)&&(k>(MAXepoch/3));
					minTrainError = TrainError;
					minTestError = TestError; 
				}
				stop = stop||((1.1*minTestError<TestError)&&(1.1*minTrainError<TrainError)&&(k>(MAXepoch/3)));
				if (stop)
					cout << "siteid = " << mSites[i].sSiteID << "	k =" << k 
						<< "	TrainErr = " << TrainError 
						<< "	TestErr = " << TestError << endl;
				k++;
//				if (k==ceil(MAXepoch/2))
//				    ANN.set_train_error_function(FANN::ERRORFUNC_TANH);

			} 	

			ANN.destroy();

			query = queryM;
			gcvt(newANNid,9,annid);
			query +=annid;
			query +=",now(),";
			query += machineid;
			query += ",";
			type = 1;		
			gcvt(type,9,temp);
			query += temp;
			query += ",";
			query +=site;
			query +=",";
			gcvt(mSites[i].sMaxDist,9,temp);
			query +=temp;
			query +=",";
			gcvt(mSites[i].sNumInputs,9,temp);
			query +=temp;
			query += ",";
			gcvt(mSites[i].sNumOutputsA,9,temp);
			query +=temp;
			query += ",'";
			query += filename;
			query += "');";
	

			if (!gDb.PerformRawSql(query))
			{
				string err = "Error inserting NeuralNet by running query: ";
				err += query;
				cout << err <<endl; 
				QRAP_WARN(err.c_str());
				return false;
			}

			TrainDataDist.set_train_data(mSites[i].sNumDataRowsTrain, 
						mSites[i].sNumInputs, mSites[i].sInputTrain,
						mSites[i].sNumOutputsD, mSites[i].sOutputDistTrain);
	
			TestDataDist.set_train_data(mSites[i].sNumDataRowsTest, 
						mSites[i].sNumInputs, mSites[i].sInputTest,
						mSites[i].sNumOutputsD, mSites[i].sOutputDistTest);
	
//			HiddenN1 = ceil(sqrt(mSites[i].sNumOutputsD*(5 +2.6*max(0,(int)mSites[i].sCellSet.size()) )));
//			HiddenN2 = 7;
	
			ANN.create_standard(4, mSites[i].sNumInputs, 
			HiddenN1 ,HiddenN2, mSites[i].sNumOutputsD);
 			ANN.set_train_error_function(FANN::ERRORFUNC_LINEAR);
			ANN.set_train_stop_function(FANN::STOPFUNC_MSE);
			ANN.set_training_algorithm(FANN::TRAIN_QUICKPROP);
			ANN.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
			ANN.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
			ANN.randomize_weights(-1/FanIn,1/FanIn);
//			ANN.set_learning_rate(0.7);

			cout << "saving ANN: site = " << mSites[i].sSiteID << "	i=" << i << endl;
			gcvt(mSites[i].sSiteID,9,site);
			filename = outdir;
			filename += "/";
			filename += site;
			filename += "_";
			type = 2;		
			gcvt(type,9,temp);
			filename += temp;		
//			time (&now);
//			filename += ctime(&now);
			filename += ".ann";

//			ANN.train_on_data(TrainDataDist, MAXepoch,REPORTInt,ERROR/3);
			cout << "i=" << i << "	mSites[i].sSiteID = " << mSites[i].sSiteID;
			cout << "		mSites[i].sNumOutputsD = " << mSites[i].sNumOutputsD;
			cout << "		mSites[i].sNumDataRowsTrain = " << mSites[i].sNumDataRowsTrain;
			cout << "		mSites[i].sNumDataRowsTest = " << mSites[i].sNumDataRowsTest;
			cout << "		mSites[i].sMaxDist = " << mSites[i].sMaxDist << endl;

			minTrainError = 99999999;
			minTestError = 99999999;
			stop = false;	
			k=0;
			while ((k<MAXepoch+1)&&(!stop))
			{
				TrainError = ANN.train_epoch(TrainDataDist);
				TestError  = ANN.test_data(TestDataDist);
				if (0==k%REPORTInt)
				{
//					TestError = ANN.get_MSE();
					cout << "siteid = " << mSites[i].sSiteID << "	k=" << k 
								<< "	TrainErr = " << TrainError 
								<< "	TestErr = " << TestError << endl;
				}
				if (((TrainError <= minTrainError)&&(TestError<=minTestError))
					||((TrainError<= minTrainError*0.6)&&(TestError<=(minTestError*1.1))&&(k>REPORTInt)))
				{
					ANN.save(filename);
					stop = (((minTestError-TestError)/TestError) < ERROR*ERROR)
								&&(((minTrainError-TrainError)/TrainError) < ERROR*ERROR)&&(k>(MAXepoch/3));
					minTrainError = TrainError;
					minTestError = TestError;
				}
				stop = stop||((1.1*minTestError<TestError)&&(1.1*minTrainError<TrainError)&&(k>(MAXepoch/3)));
				if (stop)
					cout << "siteid = " << mSites[i].sSiteID << "	k =" << k 
						<< "	TrainErr = " << TrainError 
						<< "	TestErr = " << TestError << endl;
				k++;
//				if (k==MAXepoch)			
//					ANN.set_train_error_function(FANN::ERRORFUNC_TANH);
			} 

			ANN.save(filename);
			ANN.destroy();
	
			query = queryM;
			gcvt(newANNid,9,annid);
			query +=annid;
			query +=",now(),";
			query += machineid;
			query += ",";
			type = 2;		
			gcvt(type,9,temp);
			query += temp;
			query += ",";
			query +=site;
			query +=",";
			gcvt(mSites[i].sMaxDist,9,temp);
			query +=temp;
			query +=",";
			gcvt(mSites[i].sNumInputs,9,temp);
			query +=temp;
			query += ",";
			gcvt(mSites[i].sNumOutputsD,9,temp);
			query +=temp;
			query += ",'";
			query += filename;
			query += "');";
	
			if (!gDb.PerformRawSql(query))
			{
				string err = "Error inserting NeuralNet by running query: ";
				err += query;
				cout << err <<endl; 
				QRAP_WARN(err.c_str());
				return false;
			}
	
			for (j=0; j < mSites[i].sCellSet.size(); j++)
			{
				query = queryC;
				gcvt(newCIlistID,9,temp);
				query +=temp;
				query += ",";
				query += site;
				query += ",";
				query += annid;
				query += ",";
				gcvt((j+5),9,temp);
				query +=temp;
				query += ",";
				gcvt(mSites[i].sCellSet[j].sCI,9,temp);
				query +=temp;
				query += ");";
	
				if (!gDb.PerformRawSql(query))
				{
					string err = "Error inserting cell in ANNInputlist by running query: ";
					err += query;
					cout << err <<endl; 
					QRAP_WARN(err.c_str());
					return false;
				}
				newCIlistID++;
			}
		
			newANNid++;
			cout << "saved ANN: site = " << mSites[i].sSiteID << "	i=" << i << endl;
		} //end if enough training points;
	}
	delete [] temp;
	delete [] site;
	delete [] annid;
	delete [] machineid;
	cout << "tata" << endl;
	return true;
}



