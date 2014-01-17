#include "CellList.h"

//**********************************************************************
CellList::CellList()
{
	clear();
}

//**********************************************************************
CellList::~CellList()
{
}

//***********************************************************************
void CellList::clear()
{
	m_CIpKey = 0;
	m_CI = 0;				//
	m_Site = 0;
	m_AntKey = 0;
	m_EIRP = 0.0;
	m_Azimuth = 0.0;
	m_MechTilt = 0.0;
	m_AziBeam = 360.0;
	m_StartTime = 0;		// Date from which this data was valid
	m_ARFCN = 0;			// Serving cell ARFCN
	m_bBCCH = FALSE;
	m_BSIC = 0;				// Serving cell BSIC	
	m_LAC = 0;	
	m_Ligging.Set(-26.0,28.0,NDEF);
	m_TempDist = 5000.0;
}

//************************************************************* 
void CellList::Display()
{
	cout << "Key: " << m_CIpKey ;
	cout << "	CI: " << m_CI;
	cout << "	site: " << m_Site;
	cout << "	Pos: ";
	m_Ligging.Display();
	cout << " 	EIRP: " << m_EIRP;
	cout << "   AntKey: " << m_AntKey;
	cout << "   Azimuth: " << m_Azimuth;
	cout << "   MechTilt: " << m_MechTilt;
	cout << "   AziBeam: " << m_AziBeam;
	cout << "	BCCH: " <<m_ARFCN;	
	cout << "	BSIC: " <<m_BSIC;	
	cout << "	?: " <<m_bBCCH ;
	cout << "	LAC: " <<m_LAC;	
	cout << "	dist: " <<m_TempDist;
	cout << "	time: " << m_StartTime << endl;
}
//*********************************************************************
// Assignment overlay
const CellList & CellList::operator=(const CellList &right)
{
	if (this==&right)
     	return(*this);
 	m_CIpKey = right.m_CIpKey;
	m_CI = right.m_CI;
	m_Site = right.m_Site;
	m_StartTime = right.m_StartTime;
	m_EIRP = right.m_EIRP;
	m_AntKey = 	right.m_AntKey;
	m_Azimuth = right.m_Azimuth;
	m_MechTilt = right.m_MechTilt;
	m_AziBeam = right.m_AziBeam;
	m_ARFCN = right.m_ARFCN;
	m_bBCCH = right.m_bBCCH;
	m_BSIC = right.m_BSIC;	
	m_LAC = right.m_LAC;	
	m_Ligging = right.m_Ligging;
	m_TempDist = right.m_TempDist;
  	return(*this);
}

//*********************************************************************
int CellList::InsertInDB(PGconn* DBase)
{
	const char* QueryString;
	//char* QueryResult;
	PGresult* res; 
	char Temp[33];
	string CmdStr = "INSERT INTO CellData VALUES (";
	gcvt(m_CIpKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", ";
	gcvt(m_CI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_ARFCN,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_BSIC,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_LAC,5,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	if (m_bBCCH)
		CmdStr += "'TRUE', '";
	else	
		CmdStr += "'FALSE', '";
	strftime(Temp, 33, "%Y-%m-%dT%H:%M:%S", localtime(&m_StartTime));
	CmdStr += Temp;
	CmdStr += "');";
	QueryString = CmdStr.c_str();
	res = PQexec(DBase, QueryString);
		
	if (res)
	{ 
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
//			cout << "The command: " << QueryString << " worked." << endl;
//			PQclear(res);
			res = PQexec(DBase, "COMMIT;");
//			PQclear(res);
		}
		else
		{
			cout << "ERROR in: " << QueryString << endl;
			PQclear(res);
			res = PQexec(DBase, "COMMIT;");
//			PQclear(res);
			res = PQexec(DBase, "END;");
//    		PQclear(res);
			PQreset(DBase);
    		res = PQexec(DBase, "BEGIN;");
    		PQclear(res);
			return 0;
		}
	}
	return 1;
} 
