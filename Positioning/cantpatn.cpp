#include "cantpatn.h"

// Class CAntennaPattern
//## Constructors (implementation)
// CAntennaPattern:: Default Constructor
//*********************************************************************
CAntennaPattern::CAntennaPattern()
{
	m_NAA = 1;
	m_NEA = 1;
	m_Freq = 900.0;
	m_Gain = 0.0;
	m_Bearing = 0.0;
	m_MechTilt = 0.0;
	m_FtoB = 0.0;
	m_BeamW = 360.0;
	m_VBeamW = 90.0;
	m_Tilt = FALSE;
	m_Price = 0.0;
	m_ref_180az = 0;
	m_ref_0el = 0;
	m_AziAngles = new TempType[m_NAA];
	m_AziValues = new TempType[m_NAA];
	m_ElevAngles = new TempType[m_NEA];
	m_ElevValues = new TempType[m_NEA];
	m_AntPattern = new_Double2DArray(m_NAA,m_NEA);
	m_PatternFile = new char[DB_FILE_SIZE];
	m_Name = new char[DB_NAME_SIZE];
	m_Make = new char[DB_MAKE_SIZE];
	m_Description = new char[DB_DESCRIB_SIZE];
	m_Pol = new char[2];
	m_Pol[0] ='V';
	m_Pol[1] ='\0'; 
}/* end CAntennaPattern:: Default Constructor */


//*********************************************************************
// CAntennaPattern:: Destructor
CAntennaPattern::~CAntennaPattern()
{
	destruct();
}/* end CAntennaPattern:: Destructor */

//*****************************************************************************
void CAntennaPattern::destruct()
{
	delete [] m_AziAngles;
	delete [] m_AziValues;
	delete [] m_ElevAngles;
	delete [] m_ElevValues;
	delete_Double2DArray(m_AntPattern);
	delete [] m_PatternFile;
	delete [] m_Name;
	delete [] m_Make;
	delete [] m_Description;
	delete [] m_Pol; 
}
//*********************************************************************
int CAntennaPattern::ReadPlanetFormat(const char* AntFileName)
{
	const int TEMPSIZE=1000;
	char temp[TEMPSIZE];
	int i;
	int size;
		
	for (i=0;i<TEMPSIZE;i++)
		temp[i] ='\0';
	
	ifstream in_file(AntFileName);
	
	if (!in_file)
	{
		cout << "error opening " << AntFileName << endl;
		return 0;
	}
	in_file >> temp;
	temp[4] = '\0';
	if (in_file.eof())
	{
		cout << "Antenna file is empty: " << AntFileName << endl;
		return 0;
	}
	else if (strcmp(temp,"NAME"))
	{
		cout << "Antenna file not in the right format: " << AntFileName << endl; 
		return 0; 
	}
	
	size = strlen(AntFileName)+1;
	delete [] m_PatternFile;
	m_PatternFile = new char[size];
	for (i=0;((i<size)&&(AntFileName[i]!='\0')&&(i<DB_FILE_SIZE)); i++)
		m_PatternFile[i]=AntFileName[i];
	m_PatternFile[i]='\0';
	in_file >> temp;
	size = strlen(temp)+1;
	delete [] m_Name;
	m_Name = new char[size];
	for (i=0;((i<size)&&(temp[i]!='\0')&&(i<DB_NAME_SIZE)); i++)
	{
		m_Name[i]=temp[i];
		temp[i]='\0';
	}
	m_Name[i]='\0';
	temp[i]='\0';
	cout << "    " << m_Name << endl;
	in_file >> temp;
	if (!strcasecmp(temp, "MAKE"))
	{
		in_file.getline(temp,TEMPSIZE,'\n');
		size = strlen(temp)+1;
		delete [] m_Make;
		m_Make = new char[size];
		for (i=0;((i<size)&&(temp[i]!='\0')&&(i<DB_MAKE_SIZE)); i++)
		{
			m_Make[i]=temp[i];
			temp[i]='\0';
		}
		m_Make[i]='\0';
		temp[i]='\0';
		in_file >> temp;
	}
	else
	{
		strcpy(m_Make,"KATREIN");
		cout << "No manufacturer name found in " << AntFileName << ". Assuming Katrein" << endl;
	}
//	cout << "Make " << m_Make << endl;
	if (!strcasecmp(temp, "FREQUENCY"))
	{
		in_file >> m_Freq;
//		cout << "Freq: " << m_Freq << endl;
		in_file >> temp;
	}
		
	if (!strcasecmp(temp, "H_WIDTH"))
	{
		in_file >> m_BeamW;
//		cout << "Horizontal BeamWidth: " << m_BeamW << endl;
		in_file >> temp;
	}
	
	if (!strcasecmp(temp, "V_WIDTH"))
	{
		in_file >> m_VBeamW;
//		cout << "Vertical BeamWidth: " << m_VBeamW << endl;
		in_file >> temp;
	}
	
	if (!strcasecmp(temp, "FRONT_TO_BACK"))
	{
		in_file >> m_FtoB;
//		cout << "Front to Back Ratio: " << m_FtoB << endl;
		in_file >> temp;
	}
	
	if (!strcasecmp(temp, "GAIN"))
	{
		in_file >> m_Gain;
		in_file.getline(temp,TEMPSIZE,'\n');
		if (!(strcasecmp(temp,"dBd"))||(strstr(temp,"dBd")!=NULL))
			m_Gain += 2.15;
//		cout << "Gain "<< m_Gain << endl;
		in_file >> temp;
	}
	else
	{
		cout << "Antenna file not in the right format: " << AntFileName << endl; 
		return 0; 
	}
	
	if (!strcasecmp(temp, "TILT"))
	{
		in_file >> temp;
		m_Tilt = strcasecmp(temp, "MECHANICAL");
		in_file >> temp;
	}
	
	if (!strcasecmp(temp, "POLARIZATION"))
	{
		in_file >> temp;
		size = strlen(temp)+1;
		delete [] m_Pol;
		m_Pol = new char[size];
		for (i=0;((i<size)&&(temp[i]!='\0')&&(i<10)); i++)
		{
			m_Pol[i]=temp[i];
			temp[i]='\0';
		}
		m_Pol[i]='\0';
		temp[i]='\0';
		in_file >> temp;
	}
	
	if (!strcasecmp(temp,"MASKING"))
	{
		in_file >> temp;
		in_file >> temp;
	}
		
	if ((!strcasecmp(temp,"COMMENT"))||(!strcasecmp(temp,"COMMENTS")))
	{
		in_file.getline(temp,TEMPSIZE,'\n');
		string Tydelik("");
		Tydelik += temp;
		const char *Description; 
		Description = new char[DB_DESCRIB_SIZE];
		Description = Tydelik.c_str();
		for (i=0; (i<DB_DESCRIB_SIZE) && (Description[i]!='\0');i++)
		{ 
			if (Description[i]==39)
			{	
				m_Description[i]='`';
				cout << Description[i] << Description[i] << Description[i] << endl;
			}
			m_Description[i]=Description[i];
		}
		if (i>=DB_DESCRIB_SIZE)
		{ 
			m_Description[i]='\0';
			m_Description[i-1]=39;
		}
		in_file >> temp;
	}
		
	if (temp[0] == '\0')
	{
		cout << "Antenna file not in the right format: " << AntFileName << endl; 
		return 0;	
	}

	i = 0;
	while ((strcasecmp(temp,"HORIZONTAL"))&&(i<10))
	{
		in_file >> temp;
		i++;
		if (temp[0] == '\0')
		{
			cout << "Antenna file not in the right format: " << AntFileName << endl; 
			return 0;	
		}
	}
	if (i>=10)
	{
		cout << "Antenna file not in the right format: " << AntFileName << endl; 
		return 0;	
	}
	in_file >> m_NAA;
	delete [] m_AziAngles;
	delete [] m_AziValues;
	m_AziAngles = new TempType[m_NAA];
	m_AziValues = new TempType[m_NAA];
	for (i=0; i < m_NAA; i++)
	{
		in_file >> m_AziAngles[i];
		in_file >> m_AziValues[i];
	}
	in_file >> temp;
	if (strcasecmp(temp,"VERTICAL")) 
	{
		cout << "error in antenna file format: " << AntFileName << endl;
		cout << "Last read: " << temp << endl;
		return 0;
	}
	in_file >> m_NEA;
	delete [] m_ElevAngles;
	delete [] m_ElevValues;
	m_ElevAngles = new TempType [m_NEA];
	m_ElevValues = new TempType [m_NEA];
	for (i=0; i < m_NEA; i++)
	{
		in_file >> m_ElevAngles[i];
		in_file >> m_ElevValues[i];
	}
	return 1;
}

//********************************************************************
int CAntennaPattern::WriteToDatabase(PGconn* DBase, int &DeviceLastKey, int &PatternLastKey)
{
	char* QueryResult;
	const char* QueryString;
	PGresult* res; 
	int NumBytes, DeviceKey;
	int i;
	char DeviceKeyS[33], Temp[33];
		
	string CmdStr("SELECT AntDevKey FROM AntennaDevice WHERE AntennaName ='");
	CmdStr += m_Name;
	CmdStr += "';";
	QueryString = CmdStr.c_str();
	res = PQexec(DBase, "BEGIN;");
        PQclear(res);
	res = PQexec(DBase, QueryString);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			if (PQntuples(res)) //Device already exist in database
			{
				QueryResult = PQgetvalue(res,0,0);
				NumBytes = PQgetlength(res,0,0);
				DeviceKey = 0;
				for (i=0;i<NumBytes;i++)
					DeviceKey = DeviceKey*10 + (QueryResult[i]-'0');
				gcvt((double)DeviceKey,16,DeviceKeyS);
				PQclear(res);
			}
			else // Add device to database
			{
				DeviceLastKey++;
				DeviceKey = DeviceLastKey;
				gcvt((double)DeviceKey,16,DeviceKeyS);
				PQclear(res);
				CmdStr = "INSERT INTO AntennaDevice VALUES (";
				CmdStr += DeviceKeyS;
				CmdStr += ", '";
				CmdStr += m_Name; 
				CmdStr +="', '";
				CmdStr += m_Make;
				CmdStr +="', '";
				CmdStr += m_Name; 
				CmdStr +="', '";
				if (m_Tilt) CmdStr +="electrical','";
				else CmdStr +="mechanical','";
				CmdStr +=m_Description;
//				cout << m_Description << endl;
				CmdStr +="',0.0);";
				QueryString = CmdStr.c_str();
				res = PQexec(DBase, QueryString);
				if (res)
					if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
						cout << "The command: " << QueryString << " worked." << endl;
					else cout << "ERROR in: " << QueryString << endl;
				PQclear(res);
			}
		}
		else
		{
			cout << "Problem with command: " << QueryString << endl;
			PQclear(res);
			return 0;
		}
	}
	else 
	{
		PQclear(res);
		return 0;
	}
	
	PatternLastKey++;
	gcvt((double)PatternLastKey,16,Temp);
	CmdStr= "INSERT INTO AntennaPattern VALUES ("; 
	CmdStr += Temp;
	CmdStr += ", ";
	CmdStr += DeviceKeyS;
	CmdStr += ", '";
	CmdStr += m_PatternFile;
	CmdStr += "', '";
	CmdStr += m_Description;
	CmdStr += "', ";
	gcvt(m_Freq,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", ";
	gcvt(m_Gain,8,Temp);
	CmdStr += Temp; 
	if (m_Tilt) CmdStr +=", TRUE, ";
	else CmdStr +=", FALSE, ";
	gcvt(m_BeamW,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", ";
	gcvt(m_VBeamW,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", ";
	gcvt(m_FtoB,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", '";
	CmdStr += m_Pol;
	CmdStr += "', ";
	gcvt((double)m_NAA,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", ";
	gcvt((double)m_NEA,8,Temp);
	CmdStr += Temp; 
	CmdStr +=", '{";
	
	gcvt(m_AziAngles[0], 8, Temp);	
	CmdStr += Temp; 
	for (i=1; i<m_NAA; i++)
	{
		CmdStr +=", ";
		gcvt(m_AziAngles[i], 8, Temp);	
		CmdStr += Temp; 
	}
	CmdStr += "}', '{";
	gcvt(m_AziValues[0], 8, Temp);
	CmdStr += Temp; 
	for (i=1; i<m_NAA; i++)
	{
		CmdStr +=", ";
		gcvt(m_AziValues[i], 8, Temp);
		CmdStr += Temp; 
	}
	CmdStr += "}', '{";
	gcvt(m_ElevAngles[0], 8, Temp);
	CmdStr += Temp;
	for (i=1; i<m_NEA; i++)
	{
		CmdStr += ", ";
		gcvt(m_ElevAngles[i], 8, Temp);
		CmdStr += Temp; 
	}
	CmdStr += "}', '{";
	gcvt(m_ElevValues[0], 8, Temp);
	CmdStr += Temp; 
	for (i=1; i<m_NEA; i++)
	{
		CmdStr +=", ";
		gcvt(m_ElevValues[i], 8, Temp);
		CmdStr += Temp; 
	}
	CmdStr += "}');";
	
    QueryString = CmdStr.c_str();
	res = PQexec(DBase, QueryString);
	if (res)
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
			cout << "The command: " << QueryString << " worked." << endl; 
		else 
		{
			cout << "ERROR in: " << QueryString << endl << " Check file format."<< endl;
			PQclear(res);
			PQfinish(DBase);
			return 0;
		}
	PQclear(res);
	res = PQexec(DBase, "COMMIT;");
	PQclear(res);
	res = PQexec(DBase, "END;");
	PQclear(res);
	return 1;
}//End WriteToDatabase

	
// ********************************************************************
// Set antenna pattern file
int CAntennaPattern::SetAntennaPattern(PGconn* DBase, int Ant_PatternKey, double Bearing, double MechTilt)
{							
	char* QueryResult;
	const char* QueryString;
	PGresult* res; 
	int NumBytes;
	int i,k,l;
	int DeviceKey;
	char Temp[33];
	int pp, ss; 	//point position
	TempType *TempAziAngles;
	TempType *TempElevAngles;
	TempType *TempAgterAngles;
	TempType *TempAgterValues;
	int *TempAgterRef;
	int *TempRef_m_Azi;
	int *TempRef_m_Elev;
	int I_swap;
	TempType A_swap, V_swap; 
	TempType Value;
	
	m_Bearing = Bearing;
	m_MechTilt = MechTilt;
	
	string CmdStr1("SELECT AntDevKey, Frequency, Gain, AziBeamwidth, patternFilename, ElecTilt");
	CmdStr1+=" FROM AntennaPattern WHERE AntPatternKey =";
	gcvt(Ant_PatternKey, 8, Temp);
	CmdStr1 += Temp;
	CmdStr1 += ";";
	QueryString = CmdStr1.c_str();
//	res = PQexec(DBase, "BEGIN;");
//   PQclear(res);
	res = PQexec(DBase, QueryString);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			if (PQntuples(res)) // Query successful
			{
				QueryResult = PQgetvalue(res,0,0);
				NumBytes = PQgetlength(res,0,0);
				DeviceKey = 0;
				for (i=0;i<NumBytes;i++)
					DeviceKey = DeviceKey*10 + (QueryResult[i]-'0');
				
				QueryResult = PQgetvalue(res,0,1);
				NumBytes = PQgetlength(res,0,1);
				m_Freq = 0;
				pp = 0;
				ss = 0;
				for (i=0;i<NumBytes;i++)
				{
					if (QueryResult[i]!='.')
					{
						m_Freq = m_Freq*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;	
				}
				if (pp) m_Freq/=exp10(ss-pp);
				
				QueryResult = PQgetvalue(res,0,2);
				NumBytes = PQgetlength(res,0,2);
				m_Gain = 0;
				pp = 0;
				ss = 0;
				for (i=0;i<NumBytes;i++)
				{
					if (QueryResult[i]!='.')
					{
						m_Gain = m_Gain*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;	
				}
				if (pp) m_Gain/=exp10(ss-pp);
				
				QueryResult = PQgetvalue(res,0,3);
				NumBytes = PQgetlength(res,0,3);
				m_BeamW = 0;
				pp = 0;
				ss = 0;
				for (i=0;i<NumBytes;i++)
				{
					if (QueryResult[i]!='.')
					{
						m_BeamW = m_BeamW*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;	
				}
				if (pp) m_BeamW/=exp10(ss-pp);
				
				QueryResult = PQgetvalue(res,0,4);
				NumBytes = PQgetlength(res,0,4);
				delete [] m_PatternFile;
				m_PatternFile = new char[NumBytes];
				for (i=0;i<NumBytes;i++)
					m_PatternFile[i] = QueryResult[i];
					
				QueryResult = PQgetvalue(res,0,5);
				m_Tilt = (QueryResult[0]=='T')||(QueryResult[0]=='t')||(QueryResult[0]=='0');
			}
			else
			{
				cout << "Empty Query: " << QueryString << endl;
				PQclear(res);
				res = PQexec(DBase, "END;");
    			PQclear(res);
				return 0;	
			}
		}
	}
	
	string CmdStr("SELECT numazipoints, aziangles, azivalues,");
	CmdStr+=" numelevpoints, elevangles, elevvalues FROM AntennaPattern WHERE AntPatternKey =";
	gcvt(Ant_PatternKey, 8, Temp);
	CmdStr += Temp;
	CmdStr += ";";
//	cout << CmdStr << endl;
	QueryString = CmdStr.c_str();
	res = PQexec(DBase, QueryString);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			if (PQntuples(res)) // Query successful
			{
				// Get Azimuth
				QueryResult = PQgetvalue(res,0,0);
				NumBytes = PQgetlength(res,0,0);
				m_NAA = 0;
				for (i=0;i<NumBytes;i++)
					m_NAA = m_NAA*10 + (QueryResult[i]-'0');
				delete [] m_AziAngles;
				delete [] m_AziValues;
				m_AziAngles = new TempType[m_NAA];
				m_AziValues = new TempType[m_NAA];
				QueryResult = PQgetvalue(res,0,1);
				NumBytes = PQgetlength(res,0,1);
				i=1;
				k=0;
				while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<m_NAA))
				{
					m_AziAngles[k] = 0;
					pp = 0;
					ss = 0;
					while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
					{
						if (QueryResult[i]!='.')
						{
							m_AziAngles[k] = m_AziAngles[k]*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;
						i++;	
					}
					if (pp) m_AziAngles[k]/=exp10(ss-pp);
					i++;
					k++;
				}	
				QueryResult = PQgetvalue(res,0,2);
				NumBytes = PQgetlength(res,0,2);
				i=1;
				k=0;
				while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<m_NAA))
				{
					m_AziValues[k] = 0;
					pp = 0;
					ss = 0;
					while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
					{
						if (QueryResult[i]!='.')
						{
							ss++;
							m_AziValues[k] = m_AziValues[k]*10 + (QueryResult[i]-'0');
						}
						else pp = ss;
						i++;	
					}
					if (pp) m_AziValues[k]/=exp10(ss-pp);
					i++;
					k++;
				}	
				
				//Get Elevation
				QueryResult = PQgetvalue(res,0,3);
				NumBytes = PQgetlength(res,0,3);
				m_NEA = 0;
				for (i=0;i<NumBytes;i++)
					m_NEA = m_NEA*10 + (QueryResult[i]-'0');
				delete [] m_ElevAngles;
				delete [] m_ElevValues;
				m_ElevAngles = new TempType[m_NEA];
				m_ElevValues = new TempType[m_NEA];
				
				QueryResult = PQgetvalue(res,0,4);
				NumBytes = PQgetlength(res,0,4);
				i=1;
				k=0;
				while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<m_NEA))
				{
					m_ElevAngles[k] = 0;
					pp = 0;
					ss = 0;
					while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
					{
						if (QueryResult[i]!='.')
						{
							m_ElevAngles[k] = m_ElevAngles[k]*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;
						i++;	
					}
					if (pp) m_ElevAngles[k]/=exp10(ss-pp);
					i++;
					k++;
				}	
				QueryResult = PQgetvalue(res,0,5);
				NumBytes = PQgetlength(res,0,5);
				i=1;
				k=0;
				while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<m_NEA))
				{
					m_ElevValues[k] = 0;
					pp = 0;
					ss = 0;
					while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
					{
						if (QueryResult[i]!='.')
						{
							m_ElevValues[k] = m_ElevValues[k]*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;
						i++;	
					}
					if (pp) m_ElevValues[k]/=exp10(ss-pp);
					i++;
					k++;
				}	
				
				PQclear(res);
			}
			else
			{
				cout << "Empty Query: " << QueryString << endl;
				PQclear(res);
				res = PQexec(DBase, "END;");
    			PQclear(res);
				return 0;	
			}
		}
		else
		{
			cout << "Problem with Query: " << QueryString << endl;
			PQclear(res);
			res = PQexec(DBase, "END;");
    		PQclear(res);
			return 0;
		}
	}
	else
	{
		cout << "Problem with Query: " << QueryString << endl;
		PQclear(res);
		res = PQexec(DBase, "END;");
    	PQclear(res);
		return 0;
	}

	// Prepare angular values for mask
	TempAziAngles = new TempType[m_NAA+3];
	TempRef_m_Azi = new int[m_NAA+3];
	for (k=0;k<m_NAA+3;k++)
	{
		TempRef_m_Azi[k]=9999;
		TempAziAngles[k]=9999.0;
	}
	TempAziAngles[0] = 0.0;
	TempAziAngles[1] = 180.0;
	TempAziAngles[2] = 360.0;
	for (k=0; k<m_NAA; k++)
	{
		if (m_AziAngles[k]<0) m_AziAngles[k]+=360.0;
		else if (m_AziAngles[k]>360) m_AziAngles[k]-=360.0;
		if (fabs(m_AziAngles[k]-0.0)<0.2) 
			TempRef_m_Azi[0]=k;
		else if (fabs(m_AziAngles[k]-180.0)<0.2)
			TempRef_m_Azi[1]=k;
		else if (fabs(m_AziAngles[k]-360.0)<0.2)
			TempRef_m_Azi[2]=k;
		else
		{
			 TempAziAngles[k+3]=m_AziAngles[k];
			 TempRef_m_Azi[k+3]=k;
		}
	}
	
	if (TempRef_m_Azi[2]>=m_NAA) TempRef_m_Azi[2]=TempRef_m_Azi[0];
//  if no entry was found for 360 degrees make it equal to that of 0 degrees
	
	for (k=0;k<m_NAA+3;k++)
	{
		for (l=k+1;l<m_NAA+3;l++)
		{
			if ((fabs(TempAziAngles[k]-TempAziAngles[l])<0.2)&&(	TempRef_m_Azi[k]<m_NAA))//merge the two points
			{
//				cout << "Merge Azi " << k << "  " << TempAziAngles[k] << "    " << l << "  " << TempAziAngles[l] << endl;
				m_AziValues[TempRef_m_Azi[k]]= (m_AziValues[TempRef_m_Azi[k]]+m_AziValues[TempRef_m_Azi[l]])/2.0;
				// given that we are dealing with negative log10 values this is probably WRONG!!
				TempRef_m_Azi[l]=9999;
				TempAziAngles[l]=9999.0;
			}
			else if (TempAziAngles[l]<TempAziAngles[k])	//swap
			{
				A_swap = TempAziAngles[l];
				I_swap = TempRef_m_Azi[l];
				TempAziAngles[l]=TempAziAngles[k];
				TempRef_m_Azi[l]=TempRef_m_Azi[k];
				TempAziAngles[k] = A_swap;
				TempRef_m_Azi[k] = I_swap;
			}
		}
	}
	
/*	for (k=0; k<m_NAA+3; k++)
	{
		cout << k << ":  " << TempRef_m_Azi[k] << "  h: " 
		<< TempAziAngles[k] << "  w: " << m_AziValues[TempRef_m_Azi[k]] << endl;
	}
	cout << endl;
*/

	TempElevAngles = new TempType[m_NEA+3];
	TempRef_m_Elev = new int[m_NEA+3];
	TempAgterAngles = new TempType[m_NEA+3]; 
	TempAgterValues = new TempType[m_NEA+3];
	TempAgterRef = new int[m_NEA+3];
	for (k=0;k<m_NEA+3;k++)
	{
		TempRef_m_Elev[k]=9999;
		TempElevAngles[k]=9999.0;
		TempAgterAngles[k]=9999.0;
		TempAgterValues[k]=9999.0;
		TempAgterRef[k]=9999;
	}
	TempElevAngles[0] = -90.0;
	TempElevAngles[1] = 0.0;
	TempElevAngles[2] = 90.0;
	TempAgterAngles[0] = -90.0;
	TempAgterAngles[1] = 0.0;
	TempAgterAngles[2] = 90.0;
	for (k=0; k<m_NEA; k++)
	{
		if (m_ElevAngles[k]<-90.0) m_ElevAngles[k]+=360.0;
		else if (m_ElevAngles[k]>270.0) m_ElevAngles[k]-=360.0;
		if (fabs(m_ElevAngles[k]-0.0)<0.2) 
		{
			TempRef_m_Elev[1] = k;
//			cout << m_ElevAngles[k] << " ?= "<< m_ElevAngles[TempRef_m_Elev[1]] << endl;
		}
		else if (fabs(m_ElevAngles[k]-180.0)<0.2)
		{
			TempAgterRef[1] = k;
			TempAgterValues[1] = m_ElevValues[k];
		}
		else if (fabs(m_ElevAngles[k]-90.0)<0.2)
		{
			TempRef_m_Elev[2] = k;
			TempAgterRef[2] = k;
			TempAgterValues[2] = m_ElevValues[k]	;
		}
		else if ((fabs(m_ElevAngles[k]-270.0)<0.2)||(fabs(m_ElevAngles[k]+90.0)<0.2))
		{
			TempRef_m_Elev[0] = k;
			TempAgterRef[0] = k;
			TempAgterValues[0] = m_ElevValues[k]	;
		}
		else if ((m_ElevAngles[k]>90.0)&&(m_ElevAngles[k]<270.0))
		{
			TempElevAngles[k+3] = 180 - m_ElevAngles[k];
			TempRef_m_Elev[k+3] = 7777;
			TempAgterAngles[k+3]= 180 - m_ElevAngles[k]; 
			TempAgterRef[k+3] = k;
			TempAgterValues[k+3] = m_ElevValues[k]; 
		}
		else
		{
			 TempElevAngles[k+3] = m_ElevAngles[k];
			 TempRef_m_Elev[k+3] = k;
			 TempAgterAngles[k+3] = m_ElevAngles[k];
			 TempAgterRef[k+3]= 7777;
		}
	}
	
	for (k=0;k<m_NEA+3;k++)
	{
		for (l=k+1;l<m_NEA+3;l++)
		{
			if ((fabs(TempAgterAngles[k]-TempAgterAngles[l])<0.2)&&(TempAgterRef[k]<8888))
			{	//merge the two points
				if (TempAgterRef[k]==7777)
				{
					TempAgterRef[k]=TempAgterRef[l];
					TempAgterValues[k] = TempAgterValues[l];
					TempAgterRef[l]=9999;
					TempAgterAngles[l]=9999.0;
				}
				else if (TempAgterRef[l]==7777)
				{
					TempAgterRef[l]=9999;
					TempAgterAngles[l]=9999.0;
				}
				else
				{
					TempAgterValues[k]= (TempAgterValues[l]+TempAgterValues[l])/2.0;
					// given that we are dealing with negative log10 values this is probably WRONG!!
					TempAgterRef[l] = 9999;
					TempAgterAngles[l] = 9999.0;
				}
			}
			else if (TempAgterAngles[l]<TempAgterAngles[k])	//swap
			{
				A_swap = TempAgterAngles[l];
				I_swap = TempAgterRef[l];
				V_swap = TempAgterValues[l];
				TempAgterAngles[l] = TempAgterAngles[k];
				TempAgterRef[l] = TempAgterRef[k];
				TempAgterValues[l] = TempAgterValues[k];
				TempAgterAngles[k] = A_swap;
				TempAgterRef[k] = I_swap;
				TempAgterValues[k] = V_swap;
			}
		}
	}
	
	for (k=0;k<m_NEA+3;k++)
	{
		for (l=k+1;l<m_NEA+3;l++)
		{
			if ((fabs(TempElevAngles[k]-TempElevAngles[l])<0.2)&&(TempRef_m_Elev[k]<8888))	
			{	//merge the two points
				if (TempRef_m_Elev[k]==7777)
				{
					TempRef_m_Elev[k]=TempRef_m_Elev[l];
					TempRef_m_Elev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
				else if (TempRef_m_Elev[l]==7777)
				{
					TempRef_m_Elev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
				else
				{
					m_ElevValues[TempRef_m_Elev[k]]= (m_ElevValues[TempRef_m_Elev[k]]+m_ElevValues[TempRef_m_Elev[l]])/2.0;
					// given that we are dealing with negative log10 values this is probably WRONG!!
					TempRef_m_Elev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
			}
			else if (TempElevAngles[l]<TempElevAngles[k])	//swap
			{
				A_swap = TempElevAngles[l];
				I_swap = TempRef_m_Elev[l];
				TempElevAngles[l]=TempElevAngles[k];
				TempRef_m_Elev[l]=TempRef_m_Elev[k];
				TempElevAngles[k]=A_swap;
				TempRef_m_Elev[k]=I_swap;
			}
		}
	}

/*	
	for (k=0; k<m_NEA+3; k++)
	{
		cout << k << ":  " << TempRef_m_Elev[k];
		if (TempRef_m_Elev[k]<m_NEA+1)
		{
			cout << "  " << TempElevAngles[k] << "  ";
			cout << m_ElevValues[TempRef_m_Elev[k]];
		}
		cout << "  a: " << TempAgterRef[k] << "  ";
		if (TempAgterRef[k]< m_NEA+1)
	    {
	    	cout << TempAgterAngles[k] << "  " << TempAgterValues[k]; 
	    }
	    cout << endl; 
	}
	cout << endl;
*/	
	k=0;
	while ((k<m_NAA+3)&&(TempRef_m_Azi[k]!=9999))
		k++;
	m_NAA = k;
	
	k=0;
	while ((k<m_NEA+3)&&(TempRef_m_Elev[k]!=9999))
		k++;
	m_NEA = k;
	
	//	cout << "Azi " << m_NAA << "  Elev " << m_NEA << endl;
	
	delete [] m_ElevAngles;
	delete [] m_AziAngles;
	m_ElevAngles = new TempType[m_NEA];
	m_AziAngles = new TempType[m_NAA];
	m_AntPattern = new_Double2DArray(m_NAA,m_NEA);
	for (k=0;k<m_NAA;k++)
		for (l=0;l<m_NEA; l++)
			m_AntPattern[k][l]=9999.9;
	
	for (k=0; k<m_NAA; k++)
	{
		m_AziAngles[k]=TempAziAngles[k];
		if (fabs(m_AziAngles[k]-180.0)<0.2) m_ref_180az = k;
	}
	for (k=0; k<m_NEA; k++)
	{
		m_ElevAngles[k]=TempElevAngles[k];
		if (fabs(m_ElevAngles[k]-0.0)<0.2) m_ref_0el = k;
	}
	
	for (k=0; k<m_NEA; k++)	//populate pattern at azimuth 0, 360 and 180
	{
		m_AntPattern[0][k]= m_ElevValues[TempRef_m_Elev[k]];
		m_AntPattern[m_NAA-1][k]= m_ElevValues[TempRef_m_Elev[k]];
		m_AntPattern[m_ref_180az][k]= TempAgterValues[k];
	}
	for (k=0; k<m_NAA; k++) //populate pattern at elevation -90, 0 and 90 
	{
		m_AntPattern[k][m_ref_0el] = m_AziValues[TempRef_m_Azi[k]];
		m_AntPattern[k][0] = m_ElevValues[TempRef_m_Elev[0]];
		m_AntPattern[k][m_NEA-1] = m_ElevValues[TempRef_m_Elev[m_NEA-1]];
	}

/*	for (k=0; k<m_NAA; k+=10)
	{
		cout << k << " k- "  ;
		for (l=0; l<m_NEA; l+=10)
			cout << "  " << m_AntPattern[k][l];
		cout << endl;
	}
	cout << endl;
	cout << "180az  "<< m_ref_180az << " 0el  " << m_ref_0el << endl;
*/	
	delete [] m_ElevValues;
	delete [] m_AziValues;
	delete [] TempAgterValues;
	delete [] TempAgterAngles;
	delete [] TempRef_m_Elev;
	delete [] TempRef_m_Azi;
	delete [] TempAgterRef;
	
	m_ElevValues = new TempType[m_NEA];
	m_AziValues = new TempType[m_NAA];
	
	
	TempType tydelik;
//  populate remaining values
	for (l=0; l<m_NEA; l++)
		m_ElevValues[l] = m_AntPattern[0][l];
	for (k=1; k<m_NAA-1; k++)
	{
		m_AziValues[k] = m_AntPattern[k][m_ref_0el];
		for (l=1; l<m_NEA-1; l++)
		{
			if (m_AntPattern[k][l]>9990.0)
			{
				Value = 0.0;
				if (k<m_ref_180az)
				{
//					cout << m_AziAngles[k] << "   0_l  " <<m_AntPattern[0][l] << "   180_1 " << m_AntPattern[m_ref_180az][l] << endl;
					Value = exp10(-m_AntPattern[0][l]/10.0);
					tydelik = Value + m_AziAngles[k]*(exp10(-m_AntPattern[m_ref_180az][l]/10.0)-Value)/180.0;
				//	Value = m_AntPattern[0][l];
				//	tydelik = Value + m_AziAngles[k]*(m_AntPattern[m_ref_180az][l]-Value)/180.0;
				}
				else if (k>m_ref_180az)
				{
//					cout << m_AziAngles[k] << "  180l  " <<m_AntPattern[m_ref_180az][l] << "   3601 " << m_AntPattern[m_NAA-1][l] << endl;
					Value = exp10(-m_AntPattern[m_ref_180az][l]/10.0);
					tydelik = Value + (m_AziAngles[k]-180.0)*(exp10(-m_AntPattern[m_NAA-1][l]/10.0)-Value)/180.0;
				//	Value = m_AntPattern[m_ref_180az][l];
				//	tydelik = Value + (m_AziAngles[k]-180.0)*(m_AntPattern[m_NAA-1][l]-Value)/180.0;
				}
				else
					tydelik = exp10(-m_AntPattern[m_ref_180az][l]/10.0);

				if (l<m_ref_0el)
				{
//					cout << m_ElevAngles[l] << "   k-90  " <<m_AntPattern[k][0] << "   kref0 " << m_AntPattern[k][m_ref_0el] << endl;
					Value = exp10(-m_AntPattern[k][0]/10.0);
					Value = Value + (m_ElevAngles[l]+90)*(exp10(-m_AntPattern[k][m_ref_0el]/10.0)-Value)/90.0;
				//	Value = m_AntPattern[k][0];
				//	Value = Value + (m_ElevAngles[l]+90)*(m_AntPattern[k][m_ref_0el]-Value)/90.0;
				}
				else if (l>m_ref_0el)
				{
//					cout << m_ElevAngles[l] << "   kref0  " <<m_AntPattern[k][m_ref_0el] << "   k90 " << m_AntPattern[k][m_NEA-1] << endl;
					Value = exp10(-m_AntPattern[k][m_ref_0el]/10.0);
					Value = Value + m_ElevAngles[l]*(exp10(-m_AntPattern[k][m_NEA-1]/10.0)-Value)/90.0;
				//	Value = m_AntPattern[k][m_ref_0el];
				//	Value = Value + m_ElevAngles[l]*(m_AntPattern[k][m_NEA-1]-Value)/90.0;
				}
				else
					Value = exp10(-m_AntPattern[k][m_ref_0el]/10.0); 
					
				m_AntPattern[k][l] = min(Value,tydelik);
				m_AntPattern[k][l]= -10.0*log10(m_AntPattern[k][l]);
//				cout << -10.0*log10(tydelik) << "  " << -10.0*log10(Value) << "   " << m_AntPattern[k][l] <<"dB" << endl;
			}
		}
	}
/*	for (k=0; k< int(m_NAA/5); k++)
	{
		cout << k << "k:: ";
		for (l=int(2*m_NEA/5); l<int(3*m_NEA/5); l++)
			cout << "   " << m_AntPattern[k][l];
		cout << endl;
	}
*/		
	return 1;
} // End SetAntennaPattern


//***********************************************************************
double CAntennaPattern::GetPatternValue(double Azimuth, double Elevation)
{
	if ((m_NAA<0)||(m_NAA>9990)||(m_NEA<0)||(m_NEA>9990))
	{
		cout << "There seems to be a problem with the antenna pattern "
			<< m_Name << endl;
		return 0;
	}
	TempType ValueAz1, ValueAz2, Value;
	TempType Az, Azz, El, Ell;
	TempType minDelta;
	int ref_Azi, ref_El, kk, ll;
	
	Az = Azimuth - m_Bearing;
	El = Elevation - m_MechTilt;
	if (El<-90.0) El+=360.0;
	else if (El>270.0) El-=360.0;
	if (El>90.0)
	{ 
		Azz = 180.0 - Az;
		Ell = 180.0 - El;
		Az = Azz;
		El = Ell;
	}
	else if (El<-90.0)
	{
		Azz = 180.0 - Az;
		Ell = -180.0 - El;
		Az = Azz;
		El = Ell;
	}
	if (Az>360.0) Az-=360.0;
	else if (Az<0.0) Az+=360.0;
	ref_Azi=0;
	minDelta = 360.0;
	for (kk=0;(kk<m_NAA-1); kk++)
		if (fabs(Az-m_AziAngles[kk])<minDelta)
		{
			ref_Azi=kk;
			minDelta = fabs(Az-m_AziAngles[kk]);
		}
	if ((Az-m_AziAngles[ref_Azi])<0)
		if (ref_Azi>0)  ref_Azi--;
		else ref_Azi=m_NAA-1;
	// What happens if value lies just before ref0??
	ref_El=0;
	minDelta = 360.0;
	for (ll=0;(ll<m_NEA-1); ll++)
		if (fabs(El-m_ElevAngles[ll])<minDelta)
		{
			ref_El=ll;
			minDelta = fabs(El-m_ElevAngles[ll]);
		}
	if ((El-m_ElevAngles[ref_El])<0)
		if (ref_El>0) ref_El--;
		else ref_El=m_NEA-1;
	
//	cout << "Azi_ref: " << ref_Azi << "  El_ref: " << ref_El << endl; 
	
	ValueAz1 = m_AntPattern[ref_Azi][ref_El];
	ValueAz1 = ValueAz1 + (El-m_ElevAngles[ref_El])
				*(m_AntPattern[ref_Azi][ref_El+1]-ValueAz1)
				/(m_ElevAngles[ref_El+1]-m_ElevAngles[ref_El]);
	ValueAz2 = m_AntPattern[ref_Azi+1][ref_El];
	ValueAz2 = ValueAz2 + (El-m_ElevAngles[ref_El])
				*(m_AntPattern[ref_Azi+1][ref_El+1]-ValueAz2)
				/(m_ElevAngles[ref_El+1]-m_ElevAngles[ref_El]);
	Value = ValueAz1 + (Az-m_AziAngles[ref_Azi])*(ValueAz2-ValueAz1)
						/(m_AziAngles[ref_Azi+1]-m_AziAngles[ref_Azi]);
	return Value;
}
