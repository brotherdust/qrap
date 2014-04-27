#ifndef MEASPOINTS_H_
#define MEASPOINTS_H_ 1

#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"

using namespace std;

namespace Qrap
{
class cMeas
{
public:
	cMeas();
	virtual ~cMeas();
	void clear();
	int InsertInDB();
	const cMeas &operator=(const cMeas &right); 
	
	long int 	m_MeasKey;		// Primary Key of measpoint
	int 		m_CI;			// Foreign key to the cell the measurement was from
	short int	m_BCCH;			// Only used temporary to find CI 
	short int	m_BSIC;			// Only used temporary to find CI 
	long int 	m_TestPKey;		// Key to original testpoint/MMR
	double 		m_Azimuth; 		// Direction w.r.t Cell/Site; 
	double 		m_Tilt;			// vertical direction w.r.t. Cell/Site; populated when BTL is calculated 
	double 		m_Distance; 	// Distance between Cell/Site and TP
	double 		m_BTL; 			// Basic Transmission loss between Tx and Rx
	double		m_MeasValue;	// in dBm
	short int	m_C1;			// C1 idle mode criteria value
	short int 	m_C2;			// C2 idle mode criteria value
	double		m_Predict;		// Signal strength predicted	
	double		m_SimValue; 	// Estimated Signal Strength after simulated changes
	double 		m_Frequency;
	int		m_ServCI;		// Serving CI used to identify measured CI using distance and BSIC-BCCH.
};
}

#endif /*MEASPOINTS_H_*/
