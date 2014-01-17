#ifndef CGEOP_H_
#define CGEOP_H_

//#include <projects.h>
#include "cgeneric.h"

#define TOL 0.00002

enum GeoType 
{
	DEG,		// decimal degrees
	UTM,
	GAUSS,		// gaussian confrom
	DTM,			// DTM proprietary format (only for use internal to engine)
	NDEF			// not explicity set
};

class cGeoP
{
public:
	cGeoP();
	cGeoP(double lat, double lon, GeoType type=DEG, int centMer=29);
	virtual ~cGeoP();
	int Get(double &lat, double &lon);
	GeoType GetGeoType() {return m_type;}; 
	int Get(double &lat, double &lon, GeoType &type, int &m_centMer);
	void Set(double lat, double lon, GeoType type=DEG, int centMer=29); 
	const cGeoP &operator=(const cGeoP &right);
	int operator==(const cGeoP &right) const;
	int operator!=(const cGeoP &right) const;
	void Display();
	double Distance(const cGeoP &right);
	double Bearing(const cGeoP &right);
	void FromHere(cGeoP here, double distance, double direction);
	
private:
	double m_lat;
	double m_lon;
	GeoType m_type;
	int m_centMer;
};

#endif /*CGEOP_H_*/
