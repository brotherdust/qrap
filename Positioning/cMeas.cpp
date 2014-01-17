#include "cMeas.h"

//********************************************************************
cMeas::cMeas()
{
	clear();
}

//********************************************************************
cMeas::~cMeas()
{
}

//*********************************************************************
void cMeas::clear()
{
	m_MeasKey=0;	// Primary Key of measpoint
	m_CI=0;			// Foreign key to the cell the measurement was from
	m_BCCH=0;		// Only used temporary to find CI 
	m_BSIC=0;		// Only used temporary to find CI 
	m_TestPKey=0;	// Key to original testpoint/MMR
	m_Azimuth=0; 	// Direction w.r.t Cell/Site; 
	m_Tilt=0;		// vertical direction w.r.t. Cell/Site; 
					// populated when BTL is calculated 
	m_Distance=0; 	// Distance between Cell/Site and TP
	m_BTL=0; 		// Basic Transmission loss between Tx and Rx
	m_MeasValue=0; 	// in dBm
	m_C1=0;			// C1 idle mode criteria value
	m_C2=0;			// C2 idle mode criteria value
	m_Predict=0;	// Signal strength predicted	
	m_SimValue=0; 	// Estimated Signal Strength after simulated changes
	m_ServCI=0;		// Serving CI used to identify measured CI using 
					// distance and BSIC-BCCH.
}


//************************************************************************
const cMeas & cMeas::operator=(const cMeas &right)
{
	if (this==&right)
     	return(*this);
 	m_MeasKey 	= right.m_MeasKey;
	m_CI 		= right.m_CI;
	m_BCCH		= right.m_BCCH;
	m_BSIC		= right.m_BSIC;
	m_TestPKey	= right.m_TestPKey;
	m_Azimuth	= right.m_Azimuth; 
	m_Tilt		= right.m_Tilt;
	m_Distance	= right.m_Distance;
	m_BTL		= right.m_BTL;
	m_MeasValue	= right.m_MeasValue;
	m_C1		= right.m_C1;
	m_C2		= right.m_C2;
	m_Predict	= right.m_Predict;
	m_SimValue	= right.m_SimValue;
	m_ServCI	= right.m_ServCI;
  	return(*this);
}

//*********************************************************************
int cMeas::InsertInDB(PGconn* DBase)
{
	const char* QueryString;
	PGresult* res; 
	char Temp[33];
	
	string CmdStr = "INSERT INTO Measurements VALUES (";
	gcvt(m_MeasKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", ";
	gcvt(m_CI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_TestPKey,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Azimuth,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Tilt,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Distance,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_BTL,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_MeasValue,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_C1,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_C2,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Predict,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_SimValue,8,Temp);
	CmdStr += Temp;
	CmdStr += ");";
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
