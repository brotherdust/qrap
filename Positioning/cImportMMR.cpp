//**********************************************************************/

#include "cImportMMR.h"

// Class cImportMMR
//## Constructors (implementation)
// cImportMMR:: Default Constructor
//*********************************************************************
cImportMMR::cImportMMR()
{	
}/* end CAntennaPattern:: Default Constructor */


//*********************************************************************
// CAntennaPattern:: Destructor
cImportMMR::~cImportMMR()
{
	m_coreMMR.clear();
	m_NBMeas.clear();
	m_TempList.clear();
	
}/* end CAntennaPattern:: Destructor */


//********************************************************************
int cImportMMR::ReadTEMSfile(const char* DriveTestFile, time_t FileDate, 
					int &MMRLastKey, int &MeasLastKey, int &CellLastKey)
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
	char temp[TEMPSIZE];
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
		cout << "Drive-test file not in the right format: " << DriveTestFile << endl; 
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
	if (!(Records.size() < 0))
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
				if ((oldPos.Distance(TempPoint.m_Position)>100.0)
	 					&&(TempPoint.m_Position.GetGeoType()!=NDEF))
	 			{
	 				MMRLastKey++;
					MeasLastKey++;
					CellLastKey++;
	 				TempPoint.m_TPKey = MMRLastKey;
					TempMeas.m_TestPKey = MMRLastKey;
					TempMeas.m_MeasKey = MeasLastKey;
					TempCell.m_CIpKey = CellLastKey;
		 			// Various Cell Parameters
		 			record = Records[j][9];
	 				cout << record << "   ";
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
					record = Records[j][16];
					charecord = record.c_str();
					tmpInt = atoi(charecord);
					TempCell.m_ARFCN = tmpInt;	
					TempMeas.m_BCCH = tmpInt;	
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
								NBMeas.m_TestPKey = MMRLastKey;
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
									MeasLastKey++;
									NBMeas.m_MeasKey = MeasLastKey;
									TempPoint.m_NBS++;
								}
								NBMeas.m_ServCI=TempPoint.m_ServCI;
								m_NBMeas.push_back(NBMeas);	
							}
						}
						k = k + 5;
					}
					TempPoint.m_DataSource=1;
					TempPoint.m_PosSource=GPS;
					TempPoint.m_MMRType=1;
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
	return 1;
}//End ReadTEMSfile


//********************************************************************
int cImportMMR::SortReduceCellList()
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
int cImportMMR::GetCurrentCellList(PGconn* DBase, int &CellLastKey)
{
	CellList TempCell;
	char* QueryResult;
	double TempCellLat, TempCellLon;
	const char* Query;
	PGresult* res;
	unsigned NumBytes,i,j,ii,rows;
//	char Temp[33];
	char	   *pghost,
			   *pgport,
			   *pgoptions,
			   *pgtty;
 
    pghost = NULL;				// host name of the backend server 
	pgport = NULL;				// port of the backend server
	pgoptions = NULL;			// special options to start up the backend server
	pgtty = NULL;				// debugging tty for the backend server
	CellLastKey = 0;
	struct tm tmTime;
	int Year, ss, pp;
	double sign=1.0;
	
	string CmdStr = "SELECT CDKey, CellData.CI, BCCH, BSIC, bBCCH, StartTime, SitePos ";
	CmdStr+= "FROM CellData cross join Cell cross join Sector cross join Site ";
	CmdStr+= "where CellData.CI=Cell.CI and Cell.SectorID=Sector.SI ";
	CmdStr+= "and Sector.Site = Site.SitePK;";
/*	for (i=0; i<m_LACList.size()-1; i++)
	{
		gcvt(m_LACList[i], 8, Temp);
		CmdStr += Temp;
		CmdStr += ",";	
	}
	gcvt(m_LACList[i], 8, Temp);
	CmdStr += Temp;
	CmdStr += ");";
	cout << CmdStr << endl;
*/	Query = CmdStr.c_str();
	res = PQexec(DBase, Query);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			rows = PQntuples(res);
			if (rows)
			{
				for (j=0; j<rows ;j++)
				{
					QueryResult = PQgetvalue(res,j,0);
					NumBytes = PQgetlength(res,j,0);
					TempCell.m_CIpKey =0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_CIpKey =  TempCell.m_CIpKey*10 + (QueryResult[i]-'0');
					if (CellLastKey < TempCell.m_CIpKey)
						CellLastKey = TempCell.m_CIpKey; 
					QueryResult = PQgetvalue(res,j,1);
					NumBytes = PQgetlength(res,j,1);
					TempCell.m_CI = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_CI =  TempCell.m_CI*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,2);
					NumBytes = PQgetlength(res,j,2);
					TempCell.m_ARFCN = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_ARFCN =  TempCell.m_ARFCN*10 + (QueryResult[i]-'0'); 
					QueryResult = PQgetvalue(res,j,3);
					NumBytes = PQgetlength(res,j,3);
					TempCell.m_BSIC = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_BSIC =  TempCell.m_BSIC*10 + (QueryResult[i]-'0'); 
					QueryResult = PQgetvalue(res,j,4);
					NumBytes = PQgetlength(res,j,4);
					TempCell.m_bBCCH = (QueryResult[0]=='t')||(QueryResult[0]=='T');
					QueryResult = PQgetvalue(res,j,5);
					NumBytes = PQgetlength(res,j,5);
					Year = 0;
					for (i=0;i<4;i++)
					 	Year =  Year*10 + (QueryResult[i]-'0'); 
					tmTime.tm_year = Year -1900;
					tmTime.tm_mon = 0;
					for (i=5;i<7;i++)
					 	tmTime.tm_mon =  tmTime.tm_mon*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_mday = 0;
					for (i=8;i<10;i++)
					 	tmTime.tm_mday =  tmTime.tm_mday*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_hour = 0;
					for (i=11;i<13;i++)
					 	tmTime.tm_hour =  tmTime.tm_hour*10 + (QueryResult[i]-'0'); 
					tmTime.tm_min = 0;
					for (i=14;i<16;i++)
					 	tmTime.tm_min =  tmTime.tm_min*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_sec = 0;
    				for (i=17;i<19;i++)
					 	tmTime.tm_sec =  tmTime.tm_sec*10 + (QueryResult[i]-'0'); 
/*    				cout << tmTime.tm_year << "-" 
    						<< tmTime.tm_mon << "-" 
    						<< tmTime.tm_mday << "  "
    						<< tmTime.tm_hour << ":"
    						<< tmTime.tm_min << ":"
    						<< tmTime.tm_sec << endl;
*/    						
    				TempCell.m_StartTime=mktime(&tmTime);
    				QueryResult = PQgetvalue(res,j,6);
					NumBytes = PQgetlength(res,j,6);
    				TempCellLat = 0.0;
					pp = 0;
					ss = 0;
					ii = 1;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign = 1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=',');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempCellLat = TempCellLat*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempCellLat/=sign*exp10(ss-pp);
					ii = i+1;
					TempCellLon = 0.0;
    				pp = 0;
					ss = 0;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign =1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=')');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempCellLon = TempCellLon*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempCellLon/=sign*exp10(ss-pp);
					TempCell.m_Ligging.Set(TempCellLat, TempCellLon);

    				m_TempList.push_back(TempCell);
					TempCell.clear();
				}	
			}
			PQclear(res);
		}
		else
		{
			cout << "Problem with command: " << Query << endl;
			PQclear(res);
			PQreset(DBase);
			return 0;
		}
	}
	else
	{
		PQclear(res);
		PQreset(DBase);
		return 0;
	}
	return 1;
}


//********************************************************************
// This code assumes that the measurements imported in one bunch are 
// are more or less from the same time ... i.e. using the same 
// Cell parameters for BSIC/BCCH
int cImportMMR::GetCIfromBSICBCCH()
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
	// Sort Measurement in order of Serving cell and BSIC, BCCH to en sure 
	// quicker identification
	// That way the distance between sites will have to be calculated less often.
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	cout << "m_NBMeas.size(): "  << m_NBMeas.size() << endl;
	
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
	double MinDist=5000000.0;
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
int cImportMMR::WriteCellListToDB(PGconn* DBase)
{
	PGresult* res; 
	unsigned j;
	res = PQexec(DBase, "BEGIN;");
    PQclear(res);
    for (j=0; j<m_TempList.size(); j++)
		m_TempList[j].InsertInDB(DBase);
	res = PQexec(DBase, "END;");
	PQclear(res);
	return 1;
}//End WriteCellDatatoDB

//********************************************************************
int cImportMMR::WriteToDatabase(PGconn* DBase)
{
	PGresult* res; 
	unsigned j;
	
	res = PQexec(DBase, "BEGIN;");
    PQclear(res);
    cout << "MMRs  " << m_coreMMR.size() << endl;
   	for (j=0; j<m_coreMMR.size(); j++)
		m_coreMMR[j].InsertInDB(DBase);
	res = PQexec(DBase, "END;");
	PQclear(res);
	res = PQexec(DBase, "BEGIN;");
    PQclear(res);
	cout << "Meass  " << m_NBMeas.size() << endl;
	for (j=0; j<m_NBMeas.size(); j++)
		m_NBMeas[j].InsertInDB(DBase);
  	res = PQexec(DBase, "END;");
	PQclear(res);
	return 1;	
}//End WriteToDatabase
