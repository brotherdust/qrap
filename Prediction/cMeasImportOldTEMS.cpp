 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/24
 *    License     : GNU GPLv3
 *    File        : cMeasImportOldTEMS.cpp
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



#include "cMeasImportOldTEMS.h"
//*********************************************************************
cMeasImportOldTEMS::cMeasImportOldTEMS() // default constructor
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

	q = "SELECT (MAX(id)) AS id FROM cellList";
	gDb.PerformRawSql(q);
	gDb.GetLastResult(r);
	MaxId = r[0]["id"].c_str();
	mLastCellListID = atoi(r[0]["id"].c_str());

	mPosSource = 0;
	mMeasType = 0;
	mMeasSource = 0;
	
	mMeasTypeS = new char[10];
	mMeasSourceS = new char[10];
	mPosSourceS = new char[10];

	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
}

//*********************************************************************
cMeasImportOldTEMS::~cMeasImportOldTEMS() // destructor
{
	
	delete [] mMeasTypeS;
	delete [] mMeasSourceS;
	delete [] mPosSourceS;
}

//*********************************************************************
// preferred constructor
cMeasImportOldTEMS::cMeasImportOldTEMS(	unsigned MeasType, 
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

	q = "SELECT (MAX(id)) AS id FROM cellList";
	gDb.PerformRawSql(q);
	gDb.GetLastResult(r);
	MaxId = r[0]["id"].c_str();
	mLastCellListID = atoi(r[0]["id"].c_str());

	mMeasType = MeasType;
	mMeasSource = MeasSource;
	mPosSource = PosSource;

	mMeasTypeS = new char[10];
	mMeasSourceS = new char[10];
	mPosSourceS = new char[10];

	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
}


//*********************************************************************
// Setting / changing the member varaibles. 
void cMeasImportOldTEMS::cSetMeas(	unsigned MeasType, 
					unsigned MeasSource, 
					unsigned PosSource,
					unsigned long LastTestPoint,
					unsigned long LastMeasurement,
					unsigned LastCellListID)
{
	mMeasType = MeasType;
	mMeasSource = MeasSource;
	mPosSource = PosSource; 
	mLastMeas = LastMeasurement;
	mLastTestPoint = LastTestPoint;
	mLastCellListID = LastCellListID;

	gcvt(mMeasType,9,mMeasTypeS);
	gcvt(mMeasSource,9,mMeasSourceS);
	gcvt(mPosSource,9,mPosSourceS);
}

//*********************************************************************
// Getting the member varaibles. 
void cMeasImportOldTEMS::cGetMeas(	unsigned &MeasType, 
					unsigned &MeasSource, 
					unsigned &PosSource,
					unsigned long &LastTestPoint,
					unsigned long &LastMeasurement,
					unsigned &LastCellListID)
{
	MeasType = mMeasType;
	MeasSource = mMeasSource;
	PosSource = mPosSource; 
	LastMeasurement = mLastMeas;
	LastTestPoint = mLastTestPoint;
	LastCellListID = mLastCellListID;
}

//********************************************************************
int cMeasImportOldTEMS::ReadTEMSfile(const char* DriveTestFile, time_t FileDate)
{
	map<size_t, vector<string> > Records;
	size_t PrimaryKey( 0 ); 
	
	cMeas TempMeas;   	
	cMeas NBMeas;
	cTPoint TempPoint; 
	CellList TempCell;
	
	int k;
	unsigned j,i;
	bool skip, servMeas, LAC_in;
	
	const int TEMPSIZE=1000;
	char *temp;
	temp = new char[TEMPSIZE];
	string line, field, strResult, strTemp;
	
	double dblLatDeg, dblLatMin, dblLonDeg, dblLonMin;
	string strLatDeg, strLatMin, strLonDeg, strLonMin;
	string strLonS, strLatS, strLongN, strLatN;	
	double TempPointLat,TempPointLon;
	double hours, minutes, seconds;
	double oldTime=0.0;
	cGeoP oldPos(-23,23,DEG,23);
	
	string record, file;
	const char* charecord;
	char north = 'N';
	char south = 'S';
	char east = 'E';
	char west = 'W';
	
	int spcNB, tmpInt;
	long int tmpLng;
	
	// Find id of last fingerprint

	for (k=0;k<TEMPSIZE;k++)
		temp[k] ='\0';
	
	// open drive-test text file for reading
	ifstream in_file(DriveTestFile);
	
	if (!in_file)
	{
		cout << "error opening " << DriveTestFile << endl;
		return 0;
	}
	in_file >> temp;
	temp[4] = '\0';
	if (in_file.eof())
	{
		cout << "Drive-test file is empty: " << DriveTestFile << endl;
		return 0;
	}
	else if (strcmp(temp,"TIME"))
	{
		cout << "Drive-test file not in the right format for this importer: " << DriveTestFile << endl; 
		return 0; 
	}
	
	getline(in_file, line);	// first 3 rows are column headers	
	getline(in_file, line);
	getline(in_file, line);

	// Read all records for current file into map object
	while(getline(in_file, line))
	{	
		++PrimaryKey;
	    	istringstream Iss( line );
	
    		while( getline( Iss, field, '\t' ) )
        	   	Records[ PrimaryKey ].push_back( field );
   	}
	
	in_file.close();	// finished with current drive-test file
	
	j = 1;
	k = 0;
	skip = 0;
	if (0!=Records.size())
	{
		while (j < Records.size())
		{
			skip = 0;
//			cout << Records[j][4] << endl;
			if (Records[j].size() < 13)
			{ 	
				skip = 1; 
				cout << "size < 13" << endl; 
			}
			else  
			{// OK, analyse data further
				
				servMeas = TRUE;
								
				TempMeas.clear();   	
				TempPoint.clear(); 	
				TempCell.clear();
								
				skip = 0;
				charecord = NULL;
				
				// Time
				record = Records[j][0];   
				strTemp = record.substr(0,2);
				charecord = strTemp.c_str();
				hours = strtod(charecord, NULL);
				strTemp = record.substr(3,2);
				charecord = strTemp.c_str();
				minutes = hours*60 + strtod(charecord, NULL);
				strTemp = record.substr(6,5);
				charecord = strTemp.c_str();
				seconds = minutes*60 + floor(strtod(charecord, NULL)+0.5);
				TempCell.m_StartTime = FileDate + (long)seconds;
				TempPoint.m_DateTime = FileDate + (long)seconds;
		
				// Location
				charecord = NULL;
				record = Records[j][2];		// Latitude	
				if (record[0] == south) 
				{	
//					cout << "erase south" << endl;
					record.erase (0, 3);
					record = "-" + record;				
					strLatDeg = record.substr(0,3);
					charecord = strLatDeg.c_str();
					dblLatDeg = strtod(charecord, NULL);
					strLatMin = record.substr(3,6);
					charecord = strLatMin.c_str();
					dblLatMin = strtod(charecord, NULL);
					dblLatMin = dblLatMin/60;
					TempPointLat = dblLatDeg - dblLatMin; 
				}
				else if (record[0] == north)
				{
					record.erase (0, 3);
					strLatDeg = record.substr(0,2);
					charecord = strLatDeg.c_str();
					dblLatDeg = strtod(charecord, NULL);
					strLatMin = record.substr(2,6);
					charecord = strLatMin.c_str();
					dblLatMin = strtod(charecord, NULL);
					dblLatMin = dblLatMin/60;
					TempPointLat = dblLatDeg + dblLatMin; 
				}
				else TempPointLat = 0;
				
				record = Records[j][3];		// Longitude
				if (record[0] == west) 
				{
//					cout << "erase west" << endl;
					record.erase (0, 2); 		
					record = "-" + record;
					strLonDeg = record.substr(0,4);
					charecord = strLonDeg.c_str();
					dblLonDeg = strtod(charecord, NULL);
					strLonMin = record.substr(4,6);
					charecord = strLonMin.c_str();
					dblLonMin = strtod(charecord, NULL);
					dblLonMin = dblLonMin/60;
					TempPointLon = dblLonDeg - dblLonMin; 
				}
				else if (record[0] == east)
				{
//					cout << "erase east" << endl;
					record.erase (0, 2);
					strLonDeg = record.substr(0,3);
					charecord = strLonDeg.c_str();
					dblLonDeg = strtod(charecord, NULL);
					strLonMin = record.substr(3,6);
					charecord = strLonMin.c_str();
					dblLonMin = strtod(charecord, NULL);
					dblLonMin = dblLonMin/60;
					TempPointLon = dblLonDeg + dblLonMin; 
				}
				else TempPointLon = 0.0;
				if (!((TempPointLat==0.0)||(TempPointLon==0.0)))
	 				TempPoint.m_Position.Set(TempPointLat,TempPointLon);
				if ((oldPos.Distance(TempPoint.m_Position)>30.0)
	 					&&(TempPoint.m_Position.GetGeoType()!=NDEF))
	 			{
					TempPoint.m_PosSource = mPosSource;
					TempPoint.m_MeasType = mMeasType;
					TempPoint.m_DataSource = mMeasSource;

	 				mLastTestPoint++;
					mLastMeas++;
					mLastCellListID++;
	 				TempPoint.m_TPKey = mLastTestPoint;
					TempPoint.m_DerivedTPKey = mLastTestPoint;
					TempMeas.m_TestPKey = mLastTestPoint;
					TempMeas.m_MeasKey = mLastMeas;
					TempCell.m_CIpKey = mLastCellListID;
		 			// Various Cell Parameters
		 			record = Records[j][9];
//	 				cout << record << "   ";
	 				if (record =="YES"||record =="Yes"||record =="Y"
	 					||record =="yes"||record =="y"||record =="1") 
		 				TempPoint.m_Hopping=2; //assume Synth hopping
		 			else TempPoint.m_Hopping=0;
/*	 				record = Records[j][11];
					charecord = record.c_str();
					TempCell.m_MAIO = atoi(charecord);
					record = Records[j][12];
					charecord = record.c_str();
					TempCell.m_HSN = atoi(charecord);
*/					
					record = Records[j][14];
					charecord = record.c_str();
					tmpLng = strtol ( charecord, NULL, 16 );
					tmpInt = (int) tmpLng;
					TempPoint.m_ServCI = tmpInt;
					TempCell.m_CI = tmpInt;	
					TempMeas.m_CI = tmpInt;
					TempMeas.m_ServCI = tmpInt;
					record = Records[j][15];
					charecord = record.c_str();
					tmpInt = atoi(charecord);
					TempCell.m_BSIC = tmpInt;
					TempMeas.m_BSIC = tmpInt;
					TempPoint.m_BSIC = tmpInt;	
					record = Records[j][16];
					charecord = record.c_str();
					tmpInt = atoi(charecord);
					TempCell.m_ARFCN = tmpInt;	
					TempMeas.m_BCCH = tmpInt;
					TempPoint.m_ARFCH = tmpInt;	
					record = Records[j][19];
					charecord = record.c_str();
					tmpLng = strtol ( charecord, NULL, 16 );
					TempCell.m_LAC = (int) tmpLng;
					LAC_in = FALSE;
					for (i=0; i<m_LACList.size(); i++)
						LAC_in = LAC_in||(m_LACList[i]==TempCell.m_LAC);
					if (!LAC_in) m_LACList.push_back(TempCell.m_LAC);	
					
					//Serving Cell measurements
					record = Records[j][20];		
					charecord = record.c_str();
					TempPoint.m_RxLevDnFull = atoi(charecord);
					record = Records[j][21];		
					charecord = record.c_str();
					TempPoint.m_RxQualDnFull = atoi(charecord);
					record = Records[j][22];		
					charecord = record.c_str();
					TempPoint.m_RxLevDnSub = atoi(charecord);
					record = Records[j][23];		
					charecord = record.c_str();
					TempPoint.m_RxQualDnSub = atoi(charecord);
					
					record = Records[j][24];
					charecord = record.c_str();
					TempPoint.m_TA = atoi(charecord);				
					
					record = Records[j][25];
					charecord = record.c_str();
					TempPoint.m_MSTxPower = atoi(charecord);
					
					record = Records[j][29];
					charecord = record.c_str();
					TempMeas.m_C1 = atoi(charecord);
					
					record = Records[j][30];
					charecord = record.c_str();
					TempMeas.m_C2 = atoi(charecord);
				
					record = Records[j][7];
					strTemp = record.substr(0,3);
					if ((TempPoint.m_Hopping==0)&& 		// Assume no power control
						(TempPoint.m_MSTxPower<6)&&		// and use measurement
						(strTemp!="TCH"))
					{
						TempMeas.m_MeasValue = MAX(TempPoint.m_RxLevDnFull,TempPoint.m_RxLevDnSub) -110;
					}
					else 
					{
						TempMeas.m_MeasValue=-110; 
						servMeas = FALSE;
					}
								
//					cout << j << " lat " << TempPoint.m_lat << " long " << TempPoint.m_long 
//						<< "  RxLev  " << TempPoint.m_RxLevDnFull<< endl;
					//Neighbour measurements
					spcNB = Records[j].size() - 39;	// Neighbour space
					TempPoint.m_NBS = 1; // assuming TempMeas
					k = 38;
			
					while (k < (38+spcNB))
					{
						if (Records[j][k] != "" && Records[j][k+1] != "" && Records[j][k+2] != "") 
						{											
							if (Records[j][k] != "     **" && Records[j][k+1] != "     **" 
								&& Records[j][k+2] != "     **")
							{
								NBMeas.clear(); 
								NBMeas.m_TestPKey = mLastTestPoint;
								record = Records[j][k];
								charecord = record.c_str();
								NBMeas.m_BCCH = atoi(charecord);
								record = Records[j][k+1];
								charecord = record.c_str();
								NBMeas.m_MeasValue = atoi(charecord)-110;
								record = Records[j][k+2];
								charecord = record.c_str();
								NBMeas.m_BSIC = atoi(charecord);
								record = Records[j][k+3];
								charecord = record.c_str();
								NBMeas.m_C1 = atoi(charecord);
								record = Records[j][k+4];
								charecord = record.c_str();
								NBMeas.m_C2 = atoi(charecord);
								if ((NBMeas.m_BCCH==TempMeas.m_BCCH)
									&&(NBMeas.m_BSIC==TempMeas.m_BSIC)
									&&(abs((int)(NBMeas.m_MeasValue-TempMeas.m_MeasValue))<10.0))
								{
									servMeas = FALSE;
									NBMeas.m_MeasKey = TempMeas.m_MeasKey;
									NBMeas.m_CI = TempPoint.m_ServCI;
									TempCell.m_bBCCH =TRUE;
								}
								else
								{
									mLastMeas++;
									NBMeas.m_MeasKey = mLastMeas;
									TempPoint.m_NBS++;
								}
								NBMeas.m_ServCI=TempPoint.m_ServCI;
								m_NBMeas.push_back(NBMeas);	
							}
						}
						k = k + 5;
					}
					TempPoint.m_DataSource=mMeasSource;
					TempPoint.m_PosSource=mPosSource;
					TempPoint.m_MMRType=mMeasType;

					if (servMeas)
						m_NBMeas.push_back(TempMeas);
					m_coreMMR.push_back(TempPoint);
					oldPos=TempPoint.m_Position;
		 			oldTime=seconds;
	 			}
	 			m_TempList.push_back(TempCell);
				TempCell.clear();
				TempPoint.clear();
				TempMeas.clear();
				j++;
			}
					
			if(skip)
			{
				++j;
				continue;
			}
		}
	}
	delete [] temp;
	return 1;
}//End ReadTEMSfile


//********************************************************************
int cMeasImportOldTEMS::SortReduceCellList()
{
	unsigned i,j;
	CellList Swap;
//	cout << "In Sort Reduce" << endl;
//	for (k=0; k < m_TempList.size(); k++)
//		m_TempList[k].display();
//	cout << "In Sort Reduce" << endl;
	// reduce list
	for (i=0 ; i<m_TempList.size();  i++)
	{
		if (m_TempList[i].m_CI==0)
		{
			m_TempList.erase(m_TempList.begin()+i);
			i--;
		}
		else
		for (j=i+1; j<m_TempList.size(); j++)
		{
			if (m_TempList[j].m_CI==0)
			{
				m_TempList.erase(m_TempList.begin()+j);
				j--;
			}
			else if (m_TempList[i].m_CI>m_TempList[j].m_CI)
			{
				Swap = m_TempList[i];
				m_TempList[i] = m_TempList[j];
				m_TempList[j] = Swap;
			}
			else if (m_TempList[i].m_CI==m_TempList[j].m_CI)
			{
				if (m_TempList[j].m_Ligging.GetGeoType()!=NDEF)
					m_TempList[i].m_Ligging=m_TempList[j].m_Ligging;
				else if (m_TempList[i].m_Ligging.GetGeoType()!=NDEF)
					m_TempList[j].m_Ligging=m_TempList[i].m_Ligging;
				if (m_TempList[i].m_BSIC>m_TempList[j].m_BSIC)
				{
					Swap = m_TempList[i];
					m_TempList[i] = m_TempList[j];
					m_TempList[j] = Swap;
				}
				else if (m_TempList[i].m_BSIC==m_TempList[j].m_BSIC)
				{
					if (m_TempList[i].m_ARFCN>m_TempList[j].m_ARFCN)
					{
						Swap = m_TempList[i];
						m_TempList[i] = m_TempList[j];
						m_TempList[j] = Swap;
					}
					else if (m_TempList[i].m_ARFCN==m_TempList[j].m_ARFCN)
					{
						if (m_TempList[i].m_StartTime>m_TempList[j].m_StartTime)
							m_TempList[i].m_StartTime=m_TempList[j].m_StartTime;
						else if (m_TempList[j].m_LAC>0)
							m_TempList[i].m_LAC = m_TempList[j].m_LAC;
						if (m_TempList[i].m_LAC==0)
							m_TempList[i].m_LAC = m_TempList[j].m_LAC;
						
						m_TempList[i].m_bBCCH = m_TempList[i].m_bBCCH || m_TempList[j].m_bBCCH;
						m_TempList.erase(m_TempList.begin()+j);
						j--;
					}
				}
			}			
		}
	}// end reduce list
	
	
	// sort list by CI and Date
	for (i=0; i<m_TempList.size(); i++)
	{
		for (j=i+1; j<m_TempList.size(); j++)
		{
			if (m_TempList[i].m_CI>m_TempList[j].m_CI)
			{
				Swap = m_TempList[i];
				m_TempList[i] = m_TempList[j];
				m_TempList[j] = Swap;
			}
			else if (m_TempList[i].m_CI==m_TempList[j].m_CI)
			{
				if (m_TempList[i].m_StartTime>m_TempList[j].m_StartTime)
				{
					Swap = m_TempList[i];
					m_TempList[i] = m_TempList[j];
					m_TempList[j] = Swap;
				}
			}
		}
		m_TempList[i].m_CIpKey=i+1;
	}// end sort CI and Date
	
//	cout << "Uit Sort Reduce" << endl;
//	for (k=0; k < m_TempList.size(); k++)
//		m_TempList[k].display();
//	cout << "Uit Sort Reduce" << endl;
		
	return m_TempList.size();
}// end SortReduceCellList()


//********************************************************************
int cMeasImportOldTEMS::GetCurrentCellList()
{
	CellList TempCell;
	pqxx::result CellList;
 
	string PointString;
	double longitude, latitude; 
	int spacePos;
	
	string query = "SELECT celllist.ID as id, ci, Carrier as BCCH, Code as BSIC, ";
	query += "celllist.lastmodified as StartTime, ST_AsText(site.location) AS SitePos ";
	query += "FROM CellList cross join Cell cross join radioinstallation cross join Site ";
	query += "where CI=Cell.id and Cell.risector=radioinstallation.ID ";
	query += "and radioinstallation.SiteID = Site.ID;";

	if (!gDb.PerformRawSql(query))
	{
		cout << "Problem with database query to get cellList";
		return 0;
	}
	gDb.GetLastResult(CellList);

	if (CellList.size() >0)
	{
		for (int i = 0; i < CellList.size();i++)
		{
			TempCell.m_CIpKey = atoi(CellList[i]["ID"].c_str());
			if (mLastCellListID < TempCell.m_CIpKey)
				mLastCellListID = TempCell.m_CIpKey; 
		 	TempCell.m_CI = atoi(CellList[i]["ci"].c_str());
		 	TempCell.m_ARFCN = atoi(CellList[i]["BCCH"].c_str());
			TempCell.m_bBCCH = true;
			TempCell.m_BSIC = atoi(CellList[i]["BSIC"].c_str());
			TempCell.m_StartTime = atoi(CellList[i]["StartTime"].c_str());
			PointString = CellList[i]["SitePos"].c_str();
			spacePos = PointString.find_first_of(' ');
			longitude = atof((PointString.substr(6,spacePos).c_str())); 
			latitude = atof((PointString.substr(spacePos,PointString.length()-1)).c_str());
			TempCell.m_Ligging.Set(latitude,longitude,DEG);
			m_TempList.push_back(TempCell);
			TempCell.clear();
		}	
	}
	else
	{
		cout << "Database query to get cellList empty";
		return 0;
	}
	return CellList.size();
}


//********************************************************************
// This code assumes that the measurements imported in one bunch are 
// are more or less from the same time ... i.e. using the same 
// Cell parameters for BSIC/BCCH
int cMeasImportOldTEMS::GetCIfromBSICBCCH()
{
	unsigned i,j,k;
	cMeas Swap;
	double MinDiff, Diff;
	time_t Compare;
		
//	cout << " In GetCI" << endl;
//	for (k=0; k < m_TempList.size(); k++)
//		m_TempList[k].display();
//	cout << " In GetCI" << endl;
//	Reduce the cell-list to contain only the cell parameters that we assume was valid at the time.
	Compare = m_coreMMR[0].m_DateTime;
	
	struct tm tmTime;
	tmTime.tm_year=106; 
	tmTime.tm_mon=8;
	tmTime.tm_mday=25;
	tmTime.tm_hour=1;
	tmTime.tm_min=1;
	tmTime.tm_sec=1;
	Compare=mktime(&tmTime);
	
	j=0;
	while (j<m_TempList.size())
	{
		MinDiff=20*365*24*60*60; //10 years
		Diff= Compare - m_TempList[j].m_StartTime;
		if ((Diff>0.0)&&(Diff<MinDiff))
			MinDiff=Diff;
		j++;

 		while ((j<m_TempList.size())&&(m_TempList[j].m_CI==m_TempList[j-1].m_CI))
		{
			Diff = Compare - m_TempList[j].m_StartTime;
/*			cout 	<< "  CItime: "<< m_TempList[j].m_StartTime 
					<< "  TPtime: "<< Compare
					<< "  Diff: " << Diff
					<< "  MinDiff: " << MinDiff << endl;
*/			if ((Diff>0.0)&&(Diff<MinDiff)/*&&(m_TempList[j].m_bBCCH)*/)
			{
				MinDiff=Diff;
				if (m_TempList[j-1].m_Ligging.GetGeoType()!=NDEF)
					m_TempList[j].m_Ligging=m_TempList[j-1].m_Ligging;
				m_TempList.erase(m_TempList.begin()+j-1);
			}
			else
			{	
				if (m_TempList[j].m_Ligging.GetGeoType()!=NDEF)
					m_TempList[j-1].m_Ligging=m_TempList[j].m_Ligging;
				m_TempList.erase(m_TempList.begin()+j);
			}
		}
	}
//	cout << " Uit GetCI" << endl;
//	for (k=0; k < m_TempList.size(); k++)
//		m_TempList[k].display();
//	cout << " Uit GetCI" << endl;
	// Sort Measurement in order of Serving cell and BSIC, BCCH to ensure 
	// quicker identification
	// That way the distance between sites will have to be calculated less often.
/*	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
*/	
	for (i=0; i<m_NBMeas.size(); i++)		
	{
		for (j=i+1; j<m_NBMeas.size(); j++)
		{
			if (m_NBMeas[i].m_ServCI>m_NBMeas[j].m_ServCI)
			{
				Swap = m_NBMeas[i];
				m_NBMeas[i] = m_NBMeas[j];
				m_NBMeas[j] = Swap;
			}
			else if (m_NBMeas[i].m_ServCI==m_NBMeas[j].m_ServCI)
			{
				if (m_NBMeas[i].m_BSIC>m_NBMeas[j].m_BSIC)
				{
					Swap = m_NBMeas[i];
					m_NBMeas[i] = m_NBMeas[j];
					m_NBMeas[j] = Swap;
				}
				else if (m_NBMeas[i].m_BSIC==m_NBMeas[j].m_BSIC)
				{
					if (m_NBMeas[i].m_BCCH>m_NBMeas[j].m_BCCH)
					{
						Swap = m_NBMeas[i];
						m_NBMeas[i] = m_NBMeas[j];
						m_NBMeas[j] = Swap;
					}
					else if (m_NBMeas[i].m_BCCH==m_NBMeas[j].m_BCCH)
					{ 	// This step assumes that the two measurements where taken about the same time
						// i.e. using the same cell data. 
						if (m_NBMeas[i].m_CI>0) 
							m_NBMeas[j].m_CI=m_NBMeas[i].m_CI;
						else if (m_NBMeas[j].m_CI>0) 
							m_NBMeas[i].m_CI=m_NBMeas[j].m_CI;
					}
				}
			}			
		}
	}// sort Measlist by ServCell

	bool endOnce =FALSE;
	double FAR = 500000.0;
	double MinDist=FAR;
	i=0; j=0; k=0;
	while (i<m_NBMeas.size())
	{
		if ((m_NBMeas[i].m_ServCI)&&(!m_NBMeas[i].m_CI))
		{
			MinDist=FAR;	
			while ((j<m_TempList.size())&&(m_TempList[j].m_CI!=m_NBMeas[i].m_ServCI))
				j++;
//			cout << i << " " << j << "  Cell " << m_TempList[j].m_CI 
//					<< "  NBServ " << m_NBMeas[i].m_ServCI 
//					<< "  NBServ_p " << m_NBMeas[i-1].m_ServCI << endl;
			if ((j==m_TempList.size())&&(i<m_NBMeas.size()))
			{	
				j=0;
				if (endOnce)
				{
					i++;
					MinDist = FAR;
					endOnce = FALSE;
					cout << "Cell " << m_NBMeas[i].m_ServCI << " not found in CellList." << endl;
				}
				else endOnce = TRUE;
			}
			else if (i<m_NBMeas.size())
			{
				MinDist = FAR;
				for (k=0; k<m_TempList.size(); k++)
				{
					if (m_TempList[k].m_Ligging.GetGeoType()!=NDEF)
					{
						m_TempList[k].m_TempDist = 
								m_TempList[k].m_Ligging.Distance(m_TempList[j].m_Ligging);
						if ((m_NBMeas[i].m_BSIC==m_TempList[k].m_BSIC)&&
							(m_NBMeas[i].m_BCCH==m_TempList[k].m_ARFCN)&&
							(m_TempList[k].m_TempDist<MinDist))
						{
							MinDist=m_TempList[k].m_TempDist;
							m_NBMeas[i].m_CI=m_TempList[k].m_CI;
						}
					}
					else 
					{
						m_TempList[k].m_TempDist = FAR-1.0;
						cout << "position not known: " << m_TempList[k].m_CI << endl;
					}
				}
//				for (k=0; k < m_TempList.size(); k++)
//					m_TempList[k].display();
//				cout << "j" << endl;
//				m_TempList[j].display();
				i++;
				MinDist = FAR;
				while ((i<m_NBMeas.size())&&(m_TempList[j].m_CI==m_NBMeas[i].m_ServCI))
				{
//					cout << i << " " << j << "  Cell " << m_TempList[j].m_CI 
//							<< "__NBServ " << m_NBMeas[i].m_ServCI << endl;
					for (k=0; k<m_TempList.size(); k++)
					{
						if ((m_NBMeas[i].m_BSIC==m_TempList[k].m_BSIC)&&
							(m_NBMeas[i].m_BCCH==m_TempList[k].m_ARFCN)&&
							(m_TempList[k].m_TempDist<MinDist))
						{
							MinDist = m_TempList[k].m_TempDist;
							m_NBMeas[i].m_CI=m_TempList[k].m_CI;
						}
					}
					i++;
					MinDist = FAR;
				}
			}
		}
		else 
		{
//			cout << m_NBMeas[i].m_ServCI << "Serv    NB: " << m_NBMeas[i].m_CI << endl;
			i++;
		}
	}
	return 1;
}

//********************************************************************
int cMeasImportOldTEMS::WriteCellListToDB()
{
	unsigned j;

	string query = "Truncate table CellList; ";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Problem with query: Truncate table CellList;";
		return 0;
	}

    	for (j=0; j<m_TempList.size(); j++)
		if (m_TempList[j].m_bBCCH)
			m_TempList[j].InsertInDB();

	return 1;
}//End WriteCellDatatoDB

//********************************************************************
int cMeasImportOldTEMS::WriteToDatabase()
{
	unsigned j;
	
    	cout << "MMRs  " << m_coreMMR.size() << endl;
   	for (j=0; j<m_coreMMR.size(); j++)
		m_coreMMR[j].InsertInDB();

	cout << "Meas  " << m_NBMeas.size() << endl;
	for (j=0; j<m_NBMeas.size(); j++)
		if (m_NBMeas[j].m_CI>0) m_NBMeas[j].InsertInDB();
	return 1;	
}//End WriteToDatabase


