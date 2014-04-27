#include "cTPoint.h"
using namespace Qrap;

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
	m_DerivedTPKey=0;		// Testpoint from which it was originally derived.
	m_DateTime=0; 			// Time stamp of measurement
	m_DataSource=0;			// Key to the kind of origin of the data point
	m_MeasType=0;			// 
	m_Position.Set(-26.0,28.0);
	m_PosSource=0;			// Key to the kind of source for the position
	m_Height=0;			// Height above see level in meters
	m_High=0;			// Height above local ground (i.e. m_Height)
	m_HiSource=0;			// Source of height and high
	m_CStraffic=0;			// Circuit Switch traffic demand represented by point
	m_PStraffic=0;			// Packet Switch traffic demand represented by point
	m_SimServCI=0;			// CI of serving Cell in current simulation
	m_SimQual=0;			// the quality expected in current simulation
	m_ServCI=0;			// CI of serving cell during the actual measurement
	m_MMRType=0;			// Type of MMR ? Idle / Active ??
	m_TA=0;				// Timing Advance
	m_PredTA=0;			// Predicted Timing advance based on distance calculation
	m_FER=0;			// Frame Erasure Rate
	m_MSTxPower=0;			// MS tx power at time of measurement (dBm)
	m_BSTxPower=0;			// BTS tx power at time of measurement (dBm)
	m_Hopping=0;			// Was the Cell Hopping No = 0; Baseband =1, Synth=2;
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
 	m_TPKey		= right.m_TPKey;	
 	m_DerivedTPKey	= right.m_DerivedTPKey;
	m_DateTime	= right.m_DateTime; 		
	m_DataSource 	= right.m_DataSource;	
	m_MeasType 	= right.m_MeasType;
	m_Position	= right.m_Position;		
	m_PosSource	= right.m_PosSource;	
	m_Height	= right.m_Height;		
	m_High		= right.m_High;	
	m_HiSource	= right.m_HiSource;	
	m_CStraffic	= right.m_CStraffic;
	m_PStraffic	= right.m_PStraffic;
	m_SimServCI	= right.m_SimServCI;
	m_SimQual	= right.m_SimQual;
	m_ServCI	= right.m_ServCI;
	m_MMRType	= right.m_MMRType;	
	m_TA		= right.m_TA;
	m_PredTA	= right.m_PredTA;
	m_FER		= right.m_FER;	
	m_MSTxPower	= right.m_MSTxPower;
	m_BSTxPower	= right.m_BSTxPower;
	m_Hopping	= right.m_Hopping;
	m_BCCH		= right.m_BCCH;			
	m_RxLevUpFull	= right.m_RxLevUpFull;	
	m_RxLevUpSub	= right.m_RxLevUpSub;		
	m_RxLevDnFull	= right.m_RxLevDnFull;	
	m_RxLevDnSub	= right.m_RxLevDnSub;		
	m_RxQualUpFull	= right.m_RxQualUpFull;	
	m_RxQualUpSub	= right.m_RxQualUpSub;	
	m_RxQualDnFull	= right.m_RxQualDnFull;	
	m_RxQualDnSub	= right.m_RxQualDnSub;	
	m_NBS		= right.m_NBS;
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
int cTPoint::InsertInDB()
{

 	char *Temp;
	Temp = new char[33];
	double lon, lat;
	QString PosString;

	string CmdStr = "INSERT INTO TestPoint VALUES (";
	gcvt(m_TPKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", '";
	strftime(Temp, 33, "%Y-%m-%dT%H:%M:%S", localtime(&m_DateTime));
	CmdStr += Temp;
	CmdStr +="', ";
	gcvt(m_DerivedTPKey,8,Temp);
	CmdStr += Temp;
	CmdStr +=", ";
	gcvt(m_DataSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_PosSource,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_MeasType,2,Temp);
	CmdStr += Temp;
	CmdStr += ", ";
	gcvt(m_Height,7,Temp);
	CmdStr += Temp;
	m_Position.Get(lat,lon);
	PosString=QString(", ST_GeomFromText('POINT(%1 %2)',4326));").arg(lon).arg(lat);
	CmdStr += PosString.toStdString();

	if (!gDb.PerformRawSql(CmdStr))
	{
		cout << "Problem inserting TestPoint values";
		delete [] Temp;
		return 0;
	}

	if (m_ServCI>0)
	{
		CmdStr = "INSERT INTO testpointauxGSM VALUES (";
		gcvt(m_TPKey,8,Temp);
		CmdStr += Temp;
		CmdStr +=", ";
		gcvt(m_TPKey,8,Temp);
		CmdStr += Temp;
		CmdStr +=", ";
		gcvt(m_ServCI,6,Temp);
		CmdStr += Temp;
		CmdStr += ", ";
		gcvt(m_TA,3,Temp);
		CmdStr += Temp;
		CmdStr += ", 0, "; // Prop delay taken as zero 

		gcvt(m_BSTxPower,3,Temp);
		CmdStr += Temp;
		CmdStr += ", ";

		gcvt(m_MSTxPower,3,Temp);
		CmdStr += Temp;
		CmdStr += ", ";	

		int Rx = max( m_RxLevUpFull, m_RxLevUpSub); 

		gcvt(Rx,3,Temp);
		CmdStr += Temp;
		CmdStr += ", ";
	
		Rx = min( m_RxQualUpFull, m_RxQualUpSub); 	
		gcvt(Rx,3,Temp);
		CmdStr += Temp;
		CmdStr += ", ";

		Rx = min( m_RxQualDnFull, m_RxQualDnSub); 
		gcvt(Rx,3,Temp);
		CmdStr += Temp;
		CmdStr += "); ";

		if (!gDb.PerformRawSql(CmdStr))
		{
			cout << "Problem inserting testpointauxGSM values";
			delete [] Temp;
			return 0;
		}
	}
	delete [] Temp;
	return 1;
}
//*****************************************************************************


