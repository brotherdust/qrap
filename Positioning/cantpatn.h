// Copyright University of Pretoria 2005
// Author	: Magdaleen Snyman
// Date	:

//## Class: CAntennaPattern
//	This is to read the antenna file structures and import it into the database.

#ifndef CantPatn_h
#define CantPatn_h 1
#include "cgeneric.h"
#include "memmngr.h"
#include "libpq-fe.h"
#define TempType double
#define DB_DESCRIB_SIZE 150
#define DB_NAME_SIZE 40
#define DB_MAKE_SIZE 40
#define DB_FILE_SIZE 40

using namespace std;
using std::cout;

class CAntennaPattern
{

public:

	CAntennaPattern();

	~CAntennaPattern();
	
	void destruct();
	
	int ReadPlanetFormat(const char* AntFileName);
	
	int WriteToDatabase(PGconn* DBase, int &DeviceLastKey, int &PatternLastKey);
	
	int SetAntennaPattern(PGconn* DBase, int Ant_PatternKey, double Bearing, double MechTilt);
	
	double GetPatternValue(double Azimuth, double Elevation);
	
	char *m_PatternFile;	// Filename containing antenna pattern
	char *m_Name; 			// Antenna Pattern unique name
	char *m_Make; 			// Manufacturer
	char *m_Description;	// Describtion of antenna pattern 
	char *m_Pol;			// Polarisation of transmitted/received E-field
	double m_Freq;			// Frequency where antenna was measured
	double m_Gain;			// Maximum Gain of antenna pattern
	double m_BeamW;			// Azimuth beamwidth
	double m_VBeamW;		// Elevation beamwidth
	double m_FtoB;			// Front to Back ratio			
	double m_Price;			// Price of antenna
	bool m_Tilt;			// Kind of Tilt supported 	0 - mechanical only
							//							1 - electrical
	TempType *m_AziAngles;
	TempType *m_AziValues;
	TempType *m_ElevAngles;
	TempType *m_ElevValues;
	int m_NAA;	//number of Elevation Values
	int m_NEA;	//number of Azimuth Values

private:	
	double m_Bearing;
	double m_MechTilt;
	int m_ref_180az;
	int m_ref_0el;
	Double2DArray m_AntPattern;
};


#endif
