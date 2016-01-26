 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasImportCSV.cpp
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



#include "cMeasImportCSV.h"
//*********************************************************************
cMeasImportCSV::cMeasImportCSV() // default constructor
{
	string q = "SELECT (MAX(id)) AS id FROM testpoint";
	gDb.PerformRawSql(q);
	pqxx::result r;
	gDb.GetLastResult(r);
	QString MaxId = r[0]["id"].c_str();
	mLastTestPoint = atoi(r[0]["id"].c_str());

	q = "SELECT (MAX(id)) AS id FROM measurement";
	gDb.PerformRawSql(q);
	gDb.GetLastResult(r);
	MaxId = r[0]["id"].c_str();
	mLastMeas = atoi(r[0]["id"].c_str());

	mSensitivity = -110;
	mFrequency = 900;
	mCI = 0;
	mPosSource = 0;
	mMeasType = 0;
	mMeasSource = 0;
	
	mCIS = new char[10];		
	mMeasTypeS = new char[10];
	mMeasSourceS = new char[10];
	mPosSourceS = new char[10];
	mFrequencyS = new char[10];

	gcvt(mCI,9,mCIS);
	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
	gcvt(mFrequency,9,mFrequencyS);
}

//*********************************************************************
cMeasImportCSV::~cMeasImportCSV() // destructor
{
	delete [] mCIS;		
	delete [] mMeasTypeS;
	delete [] mMeasSourceS;
	delete [] mPosSourceS;
	delete [] mFrequencyS;
}

//*********************************************************************
// preferred constructor
cMeasImportCSV::cMeasImportCSV(double Sensitivity,
					double Frequency,
					unsigned CI, 
					unsigned MeasType, 
					unsigned MeasSource, 
					unsigned PosSource)
{
	string q = "SELECT (MAX(id)) AS id FROM testpoint";
	gDb.PerformRawSql(q);
	pqxx::result r;
	gDb.GetLastResult(r);
	QString MaxId = r[0]["id"].c_str();
	mLastTestPoint = atoi(r[0]["id"].c_str());

	q = "SELECT (MAX(id)) AS id FROM measurement";
	gDb.PerformRawSql(q);
	gDb.GetLastResult(r);
	MaxId = r[0]["id"].c_str();
	mLastMeas = atoi(r[0]["id"].c_str());

	mSensitivity = Sensitivity;
	mFrequency = Frequency;
	mCI = CI;
	mMeasType = MeasType;
	mMeasSource = MeasSource;
	mPosSource = PosSource;

	mCIS = new char[10];		
	mMeasTypeS = new char[10];
	mMeasSourceS = new char[10];
	mPosSourceS = new char[10];
	mFrequencyS = new char[10];

	gcvt(mCI,9,mCIS);
	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
	gcvt(mFrequency,9,mFrequencyS);
}


//*********************************************************************
// Setting / changing the member varaibles. 
void cMeasImportCSV::cSetMeas(double Sensitivity,
				double Frequency,
				unsigned CI, 
				unsigned MeasType, 
				unsigned MeasSource, 
				unsigned PosSource,
				unsigned long LastTestPoint,
				unsigned long LastMeasurement)
{
	mSensitivity = Sensitivity;
	mFrequency = Frequency;
	mCI = CI;
	mMeasType = MeasType;
	mMeasSource = MeasSource;
	mPosSource = PosSource; 
	mLastMeas = LastMeasurement;
	mLastTestPoint = LastTestPoint;

	gcvt(mCI,9,mCIS);
	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
	gcvt(mFrequency,9,mFrequencyS);
}

//*********************************************************************
// Getting the member varaibles. 
void cMeasImportCSV::cGetMeas(double &Sensitivity,
				double &Frequency,
				unsigned &CI, 
				unsigned &MeasType, 
				unsigned &MeasSource, 
				unsigned &PosSource,
				unsigned long &LastTestPoint,
				unsigned long &LastMeasurement)
{
	Sensitivity = mSensitivity;
	Frequency = mFrequency;
	CI = mCI;
	MeasType = mMeasType;
	MeasSource = mMeasSource;
	PosSource = mPosSource; 
	LastMeasurement = mLastMeas;
	LastTestPoint = mLastTestPoint;
	
}


//*********************************************************************
int cMeasImportCSV::LoadMeasurement(char *filename)
{
	fstream DataFile;
	DataFile.open(filename, ios::in);
	if (!DataFile.rdbuf()->is_open())	// file open error
	{
		cout << "cMeasImportCSV::LoadMeasurement: Error in opening measurement data file: ";
		cout << filename << endl;
		string err = "Error opening measurement file: ";
		err += filename;
		QRAP_WARN(err.c_str());
		return 0;
	}

	unsigned LocalNum=0;	
	double Lat, Lon, prevLat, prevLon, Meas, dist, Ddist=1e-12, NprevLat, NprevLon, ndist=0;
	double LocalTotal=0.0, LocalAve=0.0;

    	// reposition to the start of measurement data
     	DataFile.seekg(0L, ios::beg);  // go to beginning of file

	// for each measurement
	prevLat = 0.0;
	prevLon = 0.0;
	char * TPID;
	TPID = new char[10];
	char *temp;
	temp = new char[10];
	char *temp2;
	temp2 = new char[10];

	double res;
	string query = "select min(resolution) as res from filesets;";
	if (!gDb.PerformRawSql(query))
	{
		string err = "Error getting minimum resolution in file sets: ";
		err += query;
		QRAP_WARN(err.c_str());
		cout << "cMeasImportSpace::LoadMeasurement: Error getting minimum resolution in file sets: " << endl;
		cout << query << endl;
		res = 90;
	}
	else
	{
		pqxx::result r;
		gDb.GetLastResult(r);
		res = atof(r[0]["res"].c_str());
	}

	query ="";
	string queryP = "INSERT INTO testpoint (measdatasource, positionsource, meastype, location, id) VALUES(";
	if (mMeasSource>0)
		queryP += mMeasSourceS;
	else queryP += "NULL";
	queryP += ",";
	if (mPosSource>0)
		queryP += mPosSourceS;
	else queryP += "NULL";
	queryP += ",";
	if (mMeasType>0)
		queryP += mMeasTypeS;
	else queryP += "NULL"; 

	string queryM = "INSERT INTO measurement (ci,frequency,id,tp,measvalue) VALUES(";
	queryM += mCIS;
	queryM += ",";
	queryM += mFrequencyS;
	queryM += ",";

    	string line, TimeS, LonS, LatS, RSSIS, MERS;
	QString PosString;
	if (!getline (DataFile, line))
	{
		cout << "cMeasImportCSV::LoadMeasurement: problem reading first line of ";
		cout << filename << endl;
		return 0;	
	}
	else cout << line << endl;     

	double DegInMeter,DLat,DLon;

     	while (getline(DataFile, line)&&(line.substr(1,1)=="#"))
	{
        	istringstream linestream(line);
       		getline (linestream, TimeS, ',');
		getline (linestream, LatS, ',');
		getline (linestream, LonS, ',');
		getline (linestream, RSSIS, ',');
		getline (linestream, MERS);
	}
	if (LatS.substr(0,1)=="\"")
	{
		Lat = atof((LatS.substr(1,LatS.size()-2)).c_str());
		Lon = atof((LonS.substr(1,LonS.size()-2)).c_str());
		Meas = atof((RSSIS.substr(1,RSSIS.size()-2)).c_str());
	}
	else
	{
		Lat = atof(LatS.c_str());
		Lon = atof(LonS.c_str());
		Meas = atof(RSSIS.c_str());
	}


	double TLat=0.0, TLon=0.0;
	cGeoP StartP(Lat,Lon);
	prevLat = Lat;
	prevLon = Lon;
	NprevLat = Lat;
	NprevLon = Lon;
	cGeoP DeltaP(Lat,Lon);
	DeltaP.FromHere(StartP,res,0);
	DeltaP.Get(DLat,DLon);
	DegInMeter = (DLat-Lat)*(DLat-Lat)/res/res;
	DeltaP.FromHere(StartP,res,90);
	DeltaP.Get(DLat,DLon);
	DegInMeter = min(DegInMeter,(DLon-Lon)*(DLon-Lon)/res/res);
	cout << "DegInMeter = " << DegInMeter << endl;

//	Averaging distance = min ( 25x the wavelength , 25meter);
	Ddist = min(25*25*DegInMeter*LIGSNEL*LIGSNEL/1.0e12/(mFrequency*mFrequency), 25*25*DegInMeter);             

//	New sample distance ... mainly to resuce the number of samples stored
//	1.41* resolution ... to make sure each measurement is at least on a different block.
	double Ndist = max(Ddist, 30*res*res*DegInMeter);

	mSensitivity = -108;
	double MaxVal = -33;

	cout << "Ddist = " << Ddist << "	Ndist = "<< Ndist << endl;
	cout << "cMeasImportCSV::LoadMeasurement: before while " << endl;	
     	while (getline (DataFile, line))
    	{
		if (line.substr(1,1)!="#")
		{
	        	istringstream linestream(line);
        		getline (linestream, TimeS, ',');
  			getline (linestream, LatS, ',');
			getline (linestream, LonS, ',');
			getline (linestream, RSSIS, ',');
			getline (linestream, MERS);
			if (LatS.substr(0,1)=="\"")
			{
				Lat = atof((LatS.substr(1,LatS.size()-2)).c_str());
				Lon = atof((LonS.substr(1,LonS.size()-2)).c_str());
				Meas = atof((RSSIS.substr(1,RSSIS.size()-2)).c_str());
			}
			else
			{
				Lat = atof(LatS.c_str());
				Lon = atof(LonS.c_str());
				Meas = atof(RSSIS.c_str());
			}
			
			dist = (prevLat-Lat)*(prevLat-Lat)+(prevLon-Lon)*(prevLon-Lon);
			ndist = (NprevLat-Lat)*(NprevLat-Lat)+(NprevLon-Lon)*(NprevLon-Lon);
			if ((Meas > mSensitivity)&&(Meas<MaxVal) && (fabs(Lat)>1.0e-12)&&(fabs(Lon)>1.0e-12)&&((dist<2)||(prevLat==0)))
			{

				if (dist>=Ddist)
				{
		
					prevLon = Lon;
					prevLat = Lat;

					if (LocalNum>0) 
					{
						LocalAve = (LocalTotal+Meas)/(LocalNum+1);
						Lon = (TLon+Lon)/(LocalNum+1);
						Lat = (TLat+Lat)/(LocalNum+1);
//						cout << "mLastMeas = " <<mLastMeas <<"	LocalNum = " << LocalNum << endl;
					}
					else LocalAve = Meas;
	
					LocalTotal = 0.0;
					LocalNum = 0;
					TLon = 0.0;
					TLat = 0.0;

					if (ndist>=Ndist)
					{
						NprevLon = Lon;
						NprevLat = Lat;
						mLastTestPoint++;
						mLastMeas++;
						gcvt(mLastTestPoint,9,TPID);
						query = queryP;
						PosString=QString(",ST_GeomFromText('POINT(%1 %2)',4326),").arg(Lon).arg(Lat);
						query += PosString.toStdString();
						query += TPID;
						query += ");";
	
						if (!gDb.PerformRawSql(query))
						{
							string err = "Error inserting Test Point by running query: ";
							err += query;
							QRAP_WARN(err.c_str());
							cout << "cMeasImportCSV::LoadMeasurement: Error inserting the Test Point!: " << endl;
							cout << query << endl;
							return 0;
						}
		
						query = queryM;
						gcvt(mLastMeas,9,temp);
						query += temp;
						query += ",";
						query += TPID;
						query += ",";
						gcvt(LocalAve,9,temp);
						query += temp;
						query += ");";
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
				}
				else
				{
					LocalNum++;
					LocalTotal += Meas;
					TLat+=Lat;
					TLon+=Lon;
				}
			}
			
		}
		else cout << line << endl;
	}
	cout << "cMeasImportCSV::LoadMeasurement: done " << endl;
	DataFile.close();
	delete [] temp;
	delete [] temp2;
	delete [] TPID;
	cout << "cMeasImportCSV::LoadMeasurement: leaving ";
	return 1;
}


