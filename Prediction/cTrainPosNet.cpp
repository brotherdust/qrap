 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cTrainPosNet.cpp
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


#include "cTrainPosNet.h"
#include <random>


//*********************************************************************
cTrainPosNet::cTrainPosNet() // default constructor
{
	

}

//*********************************************************************
cTrainPosNet::~cTrainPosNet() // destructor
{
	unsigned i,j;
	cout << "In cTrainPosNet::~cTrainPosNet() " << endl;
	for (i=0;i<mNumPoints;i++)
	{
		mPosSets[i].sTestPoints.clear();
		mPosSets[i].sMeasurements.clear();
	}
	mPosSets.clear();

	for (i=0; i<mNumSites; i++)
	{
	cout << "In cTrainPosNet::~cTrainPosNet():  Site=" << i << endl;
		for (j=0; j<mSites[i].sNumDataRows ;j++)
		{
			delete [] mSites[i].sInput[j];
			delete [] mSites[i].sOutput[j];	
		}
		delete [] mSites[i].sInput;
		delete [] mSites[i].sOutput;	
		mSites[i].sCellSet.clear();
	cout << "In cTrainPosNet::~cTrainPosNet():  Site=" << i << "	done" << endl;
	}
	mSites.clear();
	cout << "In cTrainPosNet::~cTrainPosNet(): mSites cleared " << endl;
}

//*********************************************************************
bool cTrainPosNet::LoadMeasurements(vPoints Points,
					unsigned MeasType, 
					unsigned MeasSource,
					unsigned PosSource,
					unsigned Technology)
{

	if (Points.size() < 2)
	{
		cout << "cTrainPosNet::LoadMeasurements   Not enough points to define area. " << endl;
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
	unsigned i,j;

	double longitude, latitude;
	string PointString;
	unsigned spacePos;
	unsigned Counter=0;
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
	query += "where numPoints>190 ";

	query += "order by siteid, numPoints desc;";
	if (!gDb.PerformRawSql(query))
	{
		string err = "cTrainPosNet::LoadMeasurements: ";
		err +="Problem with database query to get sites from selected area! Problem with query: ";
		err += query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		delete [] text;
		return false;
	}

	tCell NewCell;
	tSiteInfoNN NewSite;
	NewCell.sCI=0;
	NewSite.sSiteID=0;
	Counter = 0;

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
					NewSite.sInput = new double*[2];
					NewSite.sOutput = new double*[2];
					for (j=0; j < 2; j++)
					{
						NewSite.sInput[j] = new double[2];
						NewSite.sOutput[j] = new double[2];
					}
					NewSite.sNumInputs = 3*NewSite.sCellSet.size() + 4;
					NewSite.sNumOutputs = 3;
					mSites.push_back(NewSite);
					NewSite.sCellSet.clear();
				}
				NewSite.sSiteID = siteid;
				PointString = r[i]["siteLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				NewSite.sPosition.Set(latitude,longitude,DEG);
				Counter=0;
			}
			NewCell.sI =Counter;
			NewCell.sCI= atoi(r[i]["ci"].c_str());
			Counter++;
			NewSite.sCellSet.push_back(NewCell);
		}
		if (NewSite.sSiteID>0)
		{
			NewSite.sInput = new double*[2];
			NewSite.sOutput = new double*[2];
			for (j=0; j < 2; j++)
			{
				NewSite.sInput[j] = new double[2];
				NewSite.sOutput[j] = new double[2];
			}
			NewSite.sNumInputs = 3*NewSite.sCellSet.size() + 4;
			NewSite.sNumOutputs = 3;
			mSites.push_back(NewSite);
			NewSite.sCellSet.clear();
		}
	}
	else
	{
		string err = "cTrainPosNet::LoadMeasurements: ";
		err +="Query to get site info from selected area is empty! Problem with query: ";
		err += query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		delete [] text;
		return false;
	}
	mNumSites = mSites.size();

	cout << "cTrainPosNet::LoadMeasurements  Done with Site List. mNumSites = " << mNumSites << endl;

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
		string err = "cTrainPosNet::LoadMeasurements: ";
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
		Counter = 0;

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
						cout << "cTrainPosNet::LoadMeasurements: ";
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
					NewPosSet.sNumMeas = NumInPosSet;
					mPosSets.push_back(NewPosSet);
					NewPosSet.sTestPoints.clear();
					NewPosSet.sMeasurements.clear();
				}
				NumInPosSet = 1;
				NewTestPoint.sOriginalTP = tp;
				PointString = r[i]["origLocation"].c_str();
				spacePos = PointString.find_first_of(' ');
				longitude = atof((PointString.substr(6,spacePos).c_str())); 
				latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
				NewTestPoint.sOriginalLocation.Set(latitude,longitude,DEG);
				NewTestPoint.sServCellAzimuth = atof(r[i]["antbearing"].c_str());
				NewTestPoint.sResDist = atof(r[i]["DistRes"].c_str());
				NewTestPoint.sTA = atoi(r[i]["TA"].c_str());
				
				NewTestPoint.sDistance = NewTestPoint.sOriginalLocation.
							Distance(mSites[siteIndex].sPosition);
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
		NewPosSet.sNumMeas = NumInPosSet;
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
	cout << "cTrainPosNet::LoadMeasurement: leaving ... happy ... " << endl << endl;
	return true;
}

//**************************************************************************************************************************
bool cTrainPosNet::TrainANDSave()
{
	FANN::neural_net	ANN;
	FANN::training_data	TrainData;
	unsigned i,j, p, q, tpIndex;
	string query, queryM, queryC, outdir, filename;
	char * temp;
	temp = new char[33];
	char * site;
	site = new char[33];
	char * annid;
	annid = new char[33];
	char * machineid;
	machineid = new char[33];
	gcvt(gDb.globalMachineID,8,machineid);

	time_t now;
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
 
	queryM = "INSERT into NeuralNet (id, Lastmodified, machineid, siteid, ";
	queryM += "numInputs, numOutputs, filename) Values (";
	queryC = "INSERT into ANNInputlist (id, siteid, annid, index, cellid) Values (";

	tpIndex = 0;

	for (i=0; i<mNumSites; i++)
	{
		for (j=0; j<2 ;j++)
		{
			delete [] mSites[i].sInput[j];
			delete [] mSites[i].sOutput[j];	
		}
		delete [] mSites[i].sInput;
		delete [] mSites[i].sOutput;
		mSites[i].sInput = new double*[mSites[i].sNumDataRows];
		mSites[i].sOutput = new double*[mSites[i].sNumDataRows];
		for (j=0; j < mSites[i].sNumDataRows; j++)
		{
			mSites[i].sInput[j] = new double[mSites[i].sNumInputs];
			mSites[i].sOutput[j] = new double[mSites[i].sNumOutputs];
		}

		for (j=0; j<mSites[i].sNumDataRows; j++)
		{
//	if (i>22) cout << i <<"	" << j << "	before input outpot" << endl;
			mSites[i].sOutput[j][0] = cos(mPosSets[tpIndex].sTestPoints[0].sBearing*PI/180);
			mSites[i].sOutput[j][1] = sin(mPosSets[tpIndex].sTestPoints[0].sBearing*PI/180);
			mSites[i].sOutput[j][2] = DIST_SCALE*mPosSets[tpIndex].sTestPoints[0].sDistance;

			mSites[i].sInput[j][1] = cos(mPosSets[tpIndex].sTestPoints[0].sServCellAzimuth*PI/180);
			mSites[i].sInput[j][2] = sin(mPosSets[tpIndex].sTestPoints[0].sServCellAzimuth*PI/180);
			mSites[i].sInput[j][3] = TA_SCALE*mPosSets[tpIndex].sTestPoints[0].sTA;
			mSites[i].sInput[j][4] = mPosSets[tpIndex].sTestPoints[0].sResDist/1000;
			mSites[i].sInput[j][0] = 1;

//	if (i>22) cout << i <<"	" << j << "	after firtst input outpot" << endl;
			for (q=0; q<mSites[i].sCellSet.size(); q++)
			{
				mSites[i].sInput[j][3*q+5] = -1; // scaled
				mSites[i].sInput[j][3*q+6] = -1; // scaled
				mSites[i].sInput[j][3*q+7] = (945+FREQ_OFFSET)*FREQ_SCALE; 	
			}

//	if (i>22) cout << i <<"	" << j << "	after input initialisation " << endl;
			for (p=0; p<mPosSets[tpIndex].sNumMeas; p++)
			{
				q=0;
				while ((mSites[i].sCellSet[q].sCI!=mPosSets[tpIndex].sMeasurements[p].sCellID)
					&&(q< mSites[i].sCellSet.size()))
					q++;
				if (mSites[i].sCellSet[q].sCI==mPosSets[tpIndex].sMeasurements[p].sCellID)
				{
					mSites[i].sInput[j][3*q+5] = (mPosSets[tpIndex].sMeasurements[p].sRFDistEstimate
									+RFDist_OFFSET)*MEAS_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+5])>1)
						mSites[i].sInput[j][3*q+5]/=fabs(mSites[i].sInput[j][3*q+5]);
					mSites[i].sInput[j][3*q+6] = (mPosSets[tpIndex].sMeasurements[p].sMeasValue 										+MEAS_OFFSET)*MEAS_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+6])>1)
						mSites[i].sInput[j][3*q+6]/=fabs(mSites[i].sInput[j][3*q+6]);
					mSites[i].sInput[j][3*q+7] = (mPosSets[tpIndex].sMeasurements[p].sFrequency
									+FREQ_OFFSET)*FREQ_SCALE;
					if (fabs(mSites[i].sInput[j][3*q+7])>1)
						mSites[i].sInput[j][3*q+7]/=fabs(mSites[i].sInput[j][3*q+7]);
//					cout << mPosSets[tpIndex].sMeasurements[p].sRFDistEstimate << "	"
//						<< mPosSets[tpIndex].sMeasurements[p].sMeasValue << "	"
//						<< mPosSets[tpIndex].sMeasurements[p].sFrequency << endl;
//					cout << mSites[i].sInput[j][3*q+4] << "	"
//						<< mSites[i].sInput[j][3*q+5] << "	" 
//						<< mSites[i].sInput[j][3*q+6] << endl;

				} 
			}
			tpIndex++;
/*			for (q=0; q<mSites[i].sNumOutputs; q++)
				cout << j<< " Output " << q <<":	" << mSites[i].sOutput[j][q] << endl; 
			for (q=0; q<mSites[i].sNumInputs; q++)
				cout << j << " Input " << q <<":	" << mSites[i].sInput[j][q] << endl; 
			cout << "mSites[i].sNumDataRows = " << mSites[i].sNumDataRows << endl;
*/
//			ANN.train(mSites[i].sInput[j], mSites[i].sOutput[j]);
		}
		
		TrainData.set_train_data(mSites[i].sNumDataRows, 
					mSites[i].sNumInputs, mSites[i].sInput,
					mSites[i].sNumOutputs, mSites[i].sOutput);

		unsigned HiddenN = ceil(sqrt(mSites[i].sNumOutputs*(5 + 9 + 2*15)));

//		unsigned HiddenN1 = 5+ceil((mSites[i].sNumInputs-4)/3);
//		unsigned HiddenN2 = ceil(sqrt(mSites[i].sNumOutputs*(5 + 9 + 2*15)));

//		cout << "HiddenN = " << HiddenN;

		ANN.create_standard(3, mSites[i].sNumInputs+1, 
										HiddenN, mSites[i].sNumOutputs);
 		ANN.set_train_error_function(FANN::ERRORFUNC_LINEAR);
		ANN.set_train_stop_function(FANN::STOPFUNC_MSE);
		ANN.set_training_algorithm(FANN::TRAIN_QUICKPROP);
		ANN.set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
		ANN.set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
		ANN.randomize_weights(-0.58,0.58);

		ANN.train_on_data(TrainData, MAXepoch,REPORTInt,ERROR);

		gcvt(mSites[i].sSiteID,9,site);
		filename = outdir;
		filename += "/";
		filename += site;
//		filename += "_";
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
		query +=site;
		query +=",'";
		gcvt(mSites[i].sNumInputs,9,temp);
		query +=temp;
		query += ",";
		gcvt(mSites[i].sNumOutputs,9,temp);
		query +=temp;
		query += ",";
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
		newANNid++;

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
	}
	delete [] temp;
	delete [] site;
	delete [] annid;
	delete [] machineid;
	cout << "tata" << endl;
	return true;
}


