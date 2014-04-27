#include "cMeas.h"
using namespace Qrap;

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
	m_CI=0;		// Foreign key to the cell the measurement was from
	m_BCCH=0;	// Only used temporary to find CI 
	m_BSIC=0;	// Only used temporary to find CI 
	m_TestPKey=0;	// Key to original testpoint/MMR
	m_Azimuth=0; 	// Direction w.r.t Cell/Site; 
	m_Tilt=0;	// vertical direction w.r.t. Cell/Site; 
			// populated when BTL is calculated 
	m_Distance=500000; // Distance between Cell/Site and TP
	m_BTL=170; 	// Basic Transmission loss between Tx and Rx
	m_MeasValue=-110; // in dBm
	m_Frequency = 900;
	m_C1=0;		// C1 idle mode criteria value
	m_C2=0;		// C2 idle mode criteria value
	m_Predict=-110;	// Signal strength predicted	
	m_SimValue=-110;// Estimated Signal Strength after simulated changes
	m_ServCI=0;	// Serving CI used to identify measured CI using 
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
	if (m_BCCH < 125)
		m_Frequency = 890+45+0.2*m_BCCH;
	else if ((m_BCCH>511)&&(m_BCCH<886))
		m_Frequency = 1710.2+95+0.2*(m_BCCH-512);
	else if ((m_BCCH>954)&&(m_BCCH<1024))
		m_Frequency = 890+45+0.2*(m_BCCH-1024);
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
int cMeas::InsertInDB()
{
	char *Temp;
	Temp = new char[33];
	
	string CmdStr = "INSERT INTO Measurement VALUES (";
	gcvt(m_MeasKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", ";
	gcvt(m_CI,6,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_TestPKey,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	if (m_BCCH < 125)
		m_Frequency = 890+45+0.2*m_BCCH;
	else if ((m_BCCH>511)&&(m_BCCH<886))
		m_Frequency = 1710.2+95+0.2*(m_BCCH-512);
	else if ((m_BCCH>954)&&(m_BCCH<1024))
		m_Frequency = 890+45+0.2*(m_BCCH-1024);
	gcvt(m_Frequency,8,Temp);
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
	gcvt(m_Predict,8,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_SimValue,8,Temp);
	CmdStr += Temp;
	CmdStr += ");";

	if (!gDb.PerformRawSql(CmdStr))
	{
		cout << "Problem inserting CellList values";
		delete [] Temp;
		return 0;
	}
	delete [] Temp;
	return 1;
} 
