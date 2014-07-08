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
	

}

//*********************************************************************
cTrainPosNetDistAngle::~cTrainPosNetDistAngle() // destructor
{
	unsigned i,j;
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle() " << endl;
	for (i=0;i<mNumPoints;i++)
	{
		mPosSets[i].sTestPoints.clear();
		mPosSets[i].sMeasurements.clear();
	}
	mPosSets.clear();
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle(): mPosSets cleared " << endl;

	for (i=0; i<mNumSites; i++)
	{
		cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle():  Site=" << i << endl;
		for (j=0; j<mSites[i].sNumInputs ;j++)
			delete [] mSites[i].sInput[j];
		for (j=0; j<mSites[i].sNumOutputsA ;j++)
			delete [] mSites[i].sOutputAngle[j];	
		for (j=0; j<mSites[i].sNumOutputsD ;j++)
			delete [] mSites[i].sOutputDist[j];	
		delete [] mSites[i].sInput;
		delete [] mSites[i].sOutputAngle;	
		delete [] mSites[i].sOutputDist;
		mSites[i].sCellSet.clear();
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle():  Site=" << i << "	done" << endl;
	}
	mSites.clear();
	cout << "In cTrainPosNetDistAngle::~cTrainPosNetDistAngle(): mSites cleared " << endl;
}

//*********************************************************************
bool cTrainPosNetDistAngle::LoadMeasurements(vPoints Points,
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
	double tempdist;
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
	unsigned siteIndex = 0;
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
		string err = "cTrainPosNetDistAngle::LoadMeasurements: ";
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

	gDb.GetLastResult(r);
	if (r.size() >0)
	{
		for (i=0; i<r.size(); i++)
		{
			siteid = atoi(r[i]["siteid"].c_str());
			if (siteid != NewSite.sSiteID)
			{
				if (NewSite.sSiteID>0)
				{
					cout << "cTrainPosNetDistAngle::LoadMeasurements: Site in list. SiteID = " << NewSite.sSiteID << endl;
					Counter = 0;
					NewSite.sNumInputs = 3*NewSite.sCellSet.size() + 5;
					NewSite.sNumOutputsA = 2;
					NewSite.sNumOutputsD = 1;
					mSites.push_back(NewSite);
					NewSite.sCellSet.clear();
					NewSite.sMaxDist = 0;
				}
				NewSite.sSiteID = siteid;
				PointString = r[i]["siteLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				NewSite.sPosition.Set(latitude,longitude,DEG);
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
			NewSite.sNumOutputsA = 2;
			NewSite.sNumOutputsD = 1;
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

	cout << "cTrainPosNetDistAngle::LoadMeasurements  Done with Site List. mNumSites = " << mNumSites << endl;

	query = "select distinct siteid, min(txbearing) as antbearing, ci, ";
	query += "testpoint.id as tp, ST_AsText(testpoint.location) as origLocation, ";
	query += "measurement.id as mid, measvalue, frequency, EIRP, tp1.TA as TA, ";
	query += "technology.DistRes as DistRes ";
	query += "from measurement left outer join tplist as tp1 ";
	query += "on ( measurement.tp =tp1.tp and measurement.ci = tp1.servci) ";
	query += "cross join testpoint cross join tplist as tp2 ";
	query += "cross join cell cross join radioinstallation cross join technology ";
	query += "where measurement.tp=testpoint.id and tp2.servci = cell.id ";
	query += "and testpoint.id = tp2.tp and cell.risector = radioinstallation.id ";
	query += "and radioinstallation.techkey = technology.id ";
	query += "and testpoint.positionsource<2 ";
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
	if (r.size() >0)
	{
		tTestPointNN NewTestPoint;
		tPosSetNN NewPosSet;
		tMeasNN NewMeasurement; 

		NewTestPoint.sOriginalTP=0;

		for (i=0; i<r.size(); i++)
		{
			siteid = atoi(r[i]["siteid"].c_str());
			if (siteid != mSites[siteIndex].sSiteID)
			{
				mSites[siteIndex].sNumDataRows = Counter;
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
						cout << "cTrainPosNetDistAngle::LoadMeasurements: ";
						cout << " Site in measurements not in sitelist:   ";
						cout << " SiteID = " << siteid << endl;
						return false;
					}
				}
			}
			tp = atoi(r[i]["tp"].c_str());
			if (tp != NewTestPoint.sOriginalTP)
			{
				Counter++;
				if (NewTestPoint.sOriginalTP>0)
				{
					NewPosSet.sNumMeas = NewPosSet.sMeasurements.size();
					mPosSets.push_back(NewPosSet);
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
				NewTestPoint.sResDist = atof(r[i]["DistRes"].c_str());
				NewTestPoint.sTA = atoi(r[i]["TA"].c_str());
				
				tempdist = (NewTestPoint.sTA+0.5)*NewTestPoint.sResDist;
				if (tempdist > mSites[siteIndex].sMaxDist)
					mSites[siteIndex].sMaxDist = tempdist;
				
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
		}// end for number of entries
		NewPosSet.sNumMeas = NewPosSet.sMeasurements.size();
		mPosSets.push_back(NewPosSet);
		mNumPoints = mPosSets.size();
		mSites[siteIndex].sNumDataRows = Counter;
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

/*
	for (i=0; i<mPosSets.size(); i++)
		if (mPosSets[i].sTestPoints.size()>0)
			cout << mPosSets[i].sTestPoints[0].sOriginalTP << "n" 
				<< mPosSets[i].sMeasurements.size() << "	";
	cout << endl; 
*/
	delete [] text;
	cout << "cTrainPosNetDistAngle::LoadMeasurement: leaving ... happy ... " << endl << endl;
	return true;
}

//**************************************************************************************************************************
bool cTrainPosNetDistAngle::TrainANDSave()
{
	FANN::neural_net	ANN;
	FANN::training_data	TrainDataDist;
	FANN::training_data	TrainDataAngle;
	unsigned i,j, p, q, tpIndex, type;
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

	tpIndex = 0;

	for (i=0; i<mNumSites; i++)
	{
		cout << "mSites[i].sSiteID = " << mSites[i].sSiteID;
		cout << "		mSites[i].sNumOutputsA = " << mSites[i].sNumOutputsA << endl;
		cout << "		mSites[i].sNumDataRows = " << mSites[i].sNumDataRows << endl;
		mSites[i].sNumOutputsA = 2;
		mSites[i].sNumOutputsD = 1;

		mSites[i].sInput = new double*[mSites[i].sNumDataRows];
		mSites[i].sOutputAngle = new double*[mSites[i].sNumDataRows];
		mSites[i].sOutputDist = new double*[mSites[i].sNumDataRows];
		for (j=0; j < mSites[i].sNumDataRows; j++)
		{
			mSites[i].sInput[j] = new double[mSites[i].sNumInputs];
			mSites[i].sOutputAngle[j] = new double[mSites[i].sNumOutputsA];
			mSites[i].sOutputDist[j] = new double[mSites[i].sNumOutputsD];
		}

		for (j=0; j<mSites[i].sNumDataRows; j++)
		{
			mSites[i].sInput[j][1] = cos(mPosSets[tpIndex].sTestPoints[0].sServCellAzimuth*PI/180);
			mSites[i].sInput[j][2] = sin(mPosSets[tpIndex].sTestPoints[0].sServCellAzimuth*PI/180);
			mSites[i].sInput[j][3] = (((double)mPosSets[tpIndex].sTestPoints[0].sTA+0.5)*mPosSets[tpIndex].sTestPoints[0].sResDist 
															- mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
			mSites[i].sInput[j][4] = mPosSets[tpIndex].sTestPoints[0].sResDist/1000;
			mSites[i].sInput[j][0] = 1;

			for (q=0; q<mSites[i].sCellSet.size(); q++)
			{
				mSites[i].sInput[j][3*q+5] = -1; // scaled
				mSites[i].sInput[j][3*q+6] = -1; // scaled
				mSites[i].sInput[j][3*q+7] = (945+FREQ_OFFSET)*FREQ_SCALE; 	
			}

			mSites[i].sOutputDist[j][0] = (mPosSets[tpIndex].sTestPoints[0].sDistance - mSites[i].sMaxDist/2)/mSites[i].sMaxDist;
			mSites[i].sOutputAngle[j][0] = cos(mPosSets[tpIndex].sTestPoints[0].sBearing*PI/180);
			mSites[i].sOutputAngle[j][1] = sin(mPosSets[tpIndex].sTestPoints[0].sBearing*PI/180);

			for (p=0; p<mPosSets[tpIndex].sNumMeas; p++)
			{
				q=0;
				while ((mSites[i].sCellSet[q].sCI!=mPosSets[tpIndex].sMeasurements[p].sCellID)
					&&(q+1<mSites[i].sCellSet.size()))
					q++;
				if (q<mSites[i].sCellSet.size())
				if (mSites[i].sCellSet[q].sCI==mPosSets[tpIndex].sMeasurements[p].sCellID)
				{
					mSites[i].sInput[j][3*q+5] = (-mPosSets[tpIndex].sMeasurements[p].sRFDistEstimate
																		+RFDist_OFFSET)*MEAS_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+5])>1)
						mSites[i].sInput[j][3*q+5]/=fabs(mSites[i].sInput[j][3*q+5]);
					mSites[i].sInput[j][3*q+6] = (mPosSets[tpIndex].sMeasurements[p].sMeasValue 
																		+MEAS_OFFSET)*MEAS_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+6])>1)
						mSites[i].sInput[j][3*q+6]/=fabs(mSites[i].sInput[j][3*q+6]);
					mSites[i].sInput[j][3*q+7] = (mPosSets[tpIndex].sMeasurements[p].sFrequency
									+FREQ_OFFSET)*FREQ_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+7])>1)
						mSites[i].sInput[j][3*q+7]/=fabs(mSites[i].sInput[j][3*q+7]);
/*					cout << mPosSets[tpIndex].sMeasurements[p].sRFDistEstimate << "	"
						<< mPosSets[tpIndex].sMeasurements[p].sMeasValue << "	"
						<< mPosSets[tpIndex].sMeasurements[p].sFrequency << endl;
					cout << mSites[i].sInput[j][3*q+5] << "		"
						<< mSites[i].sInput[j][3*q+6] << "		" 
						<< mSites[i].sInput[j][3*q+7] << endl;
*/
				} 
			}
			tpIndex++;
		}
		
		TrainDataAngle.set_train_data(mSites[i].sNumDataRows, 
					mSites[i].sNumInputs, mSites[i].sInput,
					mSites[i].sNumOutputsA, mSites[i].sOutputAngle);

		unsigned HiddenN1 = ceil(sqrt(mSites[i].sNumOutputsA*(5 + 9 + max(0,(int)mSites[i].sCellSet.size() -3) )));
		unsigned HiddenN2 = 6;

		ANN.create_standard(4, mSites[i].sNumInputs, 
										HiddenN1 ,HiddenN2, mSites[i].sNumOutputsA);
 		ANN.set_train_error_function(FANN::ERRORFUNC_LINEAR);
		ANN.set_train_stop_function(FANN::STOPFUNC_MSE);
		ANN.set_training_algorithm(FANN::TRAIN_QUICKPROP);
		ANN.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		ANN.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		ANN.randomize_weights(-0.58,0.58);

		ANN.train_on_data(TrainDataAngle, MAXepoch,REPORTInt,ERROR);

		cout << "saving ANN: site = " << mSites[i].sSiteID << "	i=" << i << endl;
		gcvt(mSites[i].sSiteID,9,site);
		filename = outdir;
		filename += "/";
		filename += site;
		filename += "_";
		type = 1;		
		gcvt(type,9,temp);
		filename += temp;		
//		time (&now);
//		filename += ctime(&now);
		filename += ".ann";

		ANN.save(filename);
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

		TrainDataDist.set_train_data(mSites[i].sNumDataRows, 
					mSites[i].sNumInputs, mSites[i].sInput,
					mSites[i].sNumOutputsD, mSites[i].sOutputDist);

		HiddenN2 = 4;

		ANN.create_standard(4, mSites[i].sNumInputs, 
										HiddenN1 ,HiddenN2, mSites[i].sNumOutputsD);
 		ANN.set_train_error_function(FANN::ERRORFUNC_LINEAR);
		ANN.set_train_stop_function(FANN::STOPFUNC_MSE);
		ANN.set_training_algorithm(FANN::TRAIN_QUICKPROP);
		ANN.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		ANN.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		ANN.randomize_weights(-0.58,0.58);

		ANN.train_on_data(TrainDataDist, 2*MAXepoch,REPORTInt,ERROR/5);

		cout << "saving ANN: site = " << mSites[i].sSiteID << "	i=" << i << endl;
		gcvt(mSites[i].sSiteID,9,site);
		filename = outdir;
		filename += "/";
		filename += site;
		filename += "_";
		type = 2;		
		gcvt(type,9,temp);
		filename += temp;		
//		time (&now);
//		filename += ctime(&now);
		filename += ".ann";

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
	}
	delete [] temp;
	delete [] site;
	delete [] annid;
	delete [] machineid;
	cout << "tata" << endl;
	return true;
}


