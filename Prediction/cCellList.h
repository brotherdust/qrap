#ifndef CELLLIST_H_
#define CELLLIST_H_

// Container for Cell data that changes.
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"

namespace Qrap
{
class CellList
{
public:
	CellList();
	virtual ~CellList();
	const CellList &operator=(const CellList &right); 
	void clear();
	void Display();
	int InsertInDB();
	cGeoP m_Ligging;
	long m_CIpKey;
	int m_CI;				//
	int m_Site;
	int m_AntKey;
	double m_EIRP;
	double m_Azimuth;
	double m_MechTilt;
	double m_AziBeam;
	time_t m_StartTime;		// Date from which this data was valid
	int m_ARFCN;			// Serving cell ARFCN
	bool m_bBCCH;			// Are you sure ARFCN = BCCH?
	int m_BSIC;				// Serving cell BSIC	
	int m_LAC;				// Location Area Code
	double m_TempDist;
};
}

#endif /*CELLLIST_H_*/
