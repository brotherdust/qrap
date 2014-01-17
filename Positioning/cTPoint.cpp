#include "cTPoint.h"

//*******************************************************************
cTPoint::cTPoint()
{
	clear();
}

//*******************************************************************
cTPoint::~cTPoint()
{
}

//*******************************************************************
void cTPoint::clear()
{
	m_TPKey=0;			// Primary Key as in database
	m_DerivedTPKey=0;	// Testpoint from which it was originally derived.
	m_DateTime=0; 		// Time stamp of measurement
	m_DataSource=0;		// Key to the kind of origin of the data point
	m_Position.Set(-26.0,28.0);
	m_PosSource=NOTD;		// Key to the kind of source for the position
	m_Height=0;			// Height above see level in meters
	m_High=0;			// Height above local ground (i.e. m_Height)
	m_HiSource=0;		// Source of height and high
	m_CStraffic=0;		// Circuit Switch traffic demand represented by point
	m_PStraffic=0;		// Packet Switch traffic demand represented by point
	m_SimServCI=0;		// CI of serving Cell in current simulation
	m_SimQual=0;		// the quality expected in current simulation
	m_ServCI=0;			// CI of serving cell during the actual measurement
	m_MMRType=0;		// Type of MMR ? Idle / Active ??
	m_TA=0;				// Timing Advance
	m_PredTA=0;			// Predicted Timing advance based on distance calculation
	m_FER=0;			// Frame Erasure Rate
	m_MSTxPower=0;		// MS tx power at time of measurement (dBm)
	m_BSTxPower=0;		// BTS tx power at time of measurement (dBm)
	m_Hopping=0;		// Was the Cell Hopping No = 0; Baseband =1, Synth=2;
	m_BCCH=0;			// Was the measurement on the BCCH
	m_RxLevUpFull=0;	
	m_RxLevUpSub=0;		
	m_RxLevDnFull=0;	
	m_RxLevDnSub=0;		
	m_RxQualUpFull=0;	
	m_RxQualUpSub=0;	
	m_RxQualDnFull=0;	
	m_RxQualDnSub=0;	
	m_NBS=0;			// number of neighbours in original list
}

//******************************************************************
const cTPoint & cTPoint::operator=(const cTPoint &right)
{
	if (this==&right)
     	return(*this);
 	m_TPKey			= right.m_TPKey;	
 	m_DerivedTPKey	= right.m_DerivedTPKey;
	m_DateTime		= right.m_DateTime; 		
	m_DataSource 	= right.m_DataSource;	
	m_Position		= right.m_Position;		
	m_PosSource		= right.m_PosSource;	
	m_Height		= right.m_Height;		
	m_High			= right.m_High;	
	m_HiSource		= right.m_HiSource;	
	m_CStraffic		= right.m_CStraffic;
	m_PStraffic		= right.m_PStraffic;
	m_SimServCI		= right.m_SimServCI;
	m_SimQual		= right.m_SimQual;
	m_ServCI		= right.m_ServCI;
	m_MMRType		= right.m_MMRType;	
	m_TA			= right.m_TA;
	m_PredTA		= right.m_PredTA;
	m_FER			= right.m_FER;	
	m_MSTxPower		= right.m_MSTxPower;
	m_BSTxPower		= right.m_BSTxPower;
	m_Hopping		= right.m_Hopping;
	m_BCCH			= right.m_BCCH;			
	m_RxLevUpFull	= right.m_RxLevUpFull;	
	m_RxLevUpSub	= right.m_RxLevUpSub;		
	m_RxLevDnFull	= right.m_RxLevDnFull;	
	m_RxLevDnSub	= right.m_RxLevDnSub;		
	m_RxQualUpFull	= right.m_RxQualUpFull;	
	m_RxQualUpSub	= right.m_RxQualUpSub;	
	m_RxQualDnFull	= right.m_RxQualDnFull;	
	m_RxQualDnSub	= right.m_RxQualDnSub;	
	m_NBS			= right.m_NBS;
  	return(*this);
}

//*********************************************************************
void cTPoint::Display()
{
 	cout << "  m_TPKey: " << m_TPKey;			
	m_Position.Display();
	cout << "  m_PosSource: " <<  m_PosSource;	
	cout << "  m_ServCI: " << m_ServCI;
	cout << "  m_TA: " << m_TA;
	cout << "  m_PredTA: " << m_PredTA;
	cout << "  m_NBS: " << m_NBS << endl;
}

//*********************************************************************
int cTPoint::InsertInDB(PGconn* DBase)
{
	const char* QueryString;
	//char* QueryResult;
	PGresult* res; 
	char Temp[33];
	double templon, templat;
	string CmdStr = "INSERT INTO TestPoint VALUES (";
	gcvt(m_TPKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", ";
	gcvt(m_DerivedTPKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", '";
	strftime(Temp, 33, "%Y-%m-%dT%H:%M:%S", localtime(&m_DateTime));
	CmdStr += Temp;
	CmdStr += "', ";
	gcvt(m_DataSource,10,Temp);
	CmdStr += Temp;
	m_Position.Get(templat,templon);
	CmdStr += ", point (";
	gcvt(templat,10,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(templon,10,Temp);
	CmdStr += Temp;
	CmdStr += "), ";
	gcvt(m_PosSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Height,7,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_High,4,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_HiSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_CStraffic,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_PStraffic,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_ServCI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_SimQual,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_ServCI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_MMRType,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_TA,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_PredTA,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_FER,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_MSTxPower,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_BSTxPower,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Hopping,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	if (m_BCCH)
		CmdStr += "'TRUE', ";
	else	
		CmdStr += "'FALSE', ";
	gcvt(m_RxLevUpFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxLevUpSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxLevDnFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxLevDnSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxQualUpFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxQualUpSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxQualDnFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_RxQualDnSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_NBS,3,Temp);
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
			PQclear(res);
			res = PQexec(DBase, "END;");
    		PQclear(res);
			PQreset(DBase);
    		res = PQexec(DBase, "BEGIN;");
    		PQclear(res);
			return 0;
		}
	}
	return 1;
}
//*****************************************************************************

int cTPoint::UpdateDB(PGconn* DBase)
{
	const char* QueryString;
	//char* QueryResult;
	PGresult* res; 
	char Temp[33];
	double templon, templat;
	string CmdStr = "UPDATE TestPoint SET datetime='";
	
	strftime(Temp, 33, "%Y-%m-%dT%H:%M:%S", localtime(&m_DateTime));
	CmdStr += Temp;
	CmdStr += "', tpsource=";
	gcvt(m_DataSource,10,Temp);
	CmdStr += Temp;
	m_Position.Get(templat,templon);
	CmdStr += ", pos = point (";
	gcvt(templat,10,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(templon,10,Temp);
	CmdStr += Temp;
	CmdStr += "), possource=";
	gcvt(m_PosSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", height=";
	gcvt(m_Height,7,Temp);
	CmdStr += Temp;
	CmdStr += ", high=";
	gcvt(m_High,4,Temp);
	CmdStr += Temp;
	CmdStr += ", hisource=";
	gcvt(m_HiSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", CStraffic=";
	gcvt(m_CStraffic,6,Temp);
	CmdStr += Temp;
	CmdStr += ", PStraffic=";
	gcvt(m_PStraffic,6,Temp);
	CmdStr += Temp;
	CmdStr += ", SimServCI=";
	gcvt(m_ServCI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", SimQual=";
	gcvt(m_SimQual,6,Temp);
	CmdStr += Temp;
	CmdStr += ", MeasServCI=";
	gcvt(m_ServCI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", TypeMMR=";
	gcvt(m_MMRType,2,Temp);
	CmdStr += Temp;
	CmdStr += ", TA=";
	gcvt(m_TA,3,Temp);
	CmdStr += Temp;
	CmdStr += ", TApred=";
	gcvt(m_PredTA,3,Temp);
	CmdStr += Temp;
	CmdStr += ", FER=";
	gcvt(m_FER,3,Temp);
	CmdStr += Temp;
	CmdStr += ", MSTxPower=";
	gcvt(m_MSTxPower,3,Temp);
	CmdStr += Temp;
	CmdStr += ", BSTxPower=";
	gcvt(m_BSTxPower,3,Temp);
	CmdStr += Temp;
	CmdStr += ", hopping=";
	gcvt(m_Hopping,2,Temp);
	CmdStr += Temp;
	CmdStr += ", measbcch=";
	if (m_BCCH)
		CmdStr += "'TRUE', RxLevFullUp=";
	else	
		CmdStr += "'FALSE', RxLevFullUp=";
	gcvt(m_RxLevUpFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxLevSubUp=";
	gcvt(m_RxLevUpSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxLevFullDn=";
	gcvt(m_RxLevDnFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxLevSubDn=";
	gcvt(m_RxLevDnSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxQualFullUp=";
	gcvt(m_RxQualUpFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxQualSubUp=";
	gcvt(m_RxQualUpSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxQualFullDn=";
	gcvt(m_RxQualDnFull,3,Temp);
	CmdStr += Temp;
	CmdStr += ", RxQualSubDn=";
	gcvt(m_RxQualDnSub,3,Temp);
	CmdStr += Temp;
	CmdStr += ", m_NBS=";
	gcvt(m_NBS,3,Temp);
	CmdStr += Temp;
	CmdStr += " where TPrimKey=";
	gcvt(m_TPKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=";";

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
			PQclear(res);
			res = PQexec(DBase, "END;");
    		PQclear(res);
			PQreset(DBase);
    		res = PQexec(DBase, "BEGIN;");
    		PQclear(res);
			return 0;
		}
	}
	return 1;
}
//*****************************************************************************

