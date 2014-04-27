#ifndef CTESTPOINT_H_
#define CTESTPOINT_H_

#include "cGeoP.h"
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"

namespace Qrap
{
class cTPoint
{
public:
	cTPoint();
	virtual ~cTPoint();
	void clear();
	void Display();
	int InsertInDB();
	const cTPoint &operator=(const cTPoint &right); 
	
	long int	m_TPKey;		// Primary Key as in database
	long int	m_DerivedTPKey;	// TestPoint from which it was originally derived.
	time_t 		m_DateTime; 	// Time stamp of measurement
	short int	m_DataSource;	// Key to the kind of origin of the data point
	short int	m_MeasType;	// Key to the kind of origin of the data point
	cGeoP		m_Position;
	short int	m_PosSource;	// Key to the kind of source for the position
	short int	m_Height;		// Height above see level in meters
	double		m_High;			// Height above local ground (i.e. m_Height)
	short int	m_HiSource;		// Source of height and high
	double		m_CStraffic;	// Circuit Switch traffic demand represented by point
	double		m_PStraffic;	// Packet Switch traffic demand represented by point
	int		m_SimServCI;	// CI of serving Cell in current simulation
	double		m_SimQual;		// the quality expected in current simulation
	int 		m_ServCI;		// CI of serving cell during the actual measurement
	short int	m_ARFCH;		// Only used temporary to find CI 
	short int	m_BSIC;			// Only used temporary to find CI 
	short int	m_MMRType;		// Type of MMR ? Idle / Active ??
	short int	m_TA;			// Timing Advance
	short int	m_PredTA;		// Predicted Timing advance based on distance calculation
	double		m_FER;			// Frame Erasure Rate
	short int 	m_MSTxPower;	// MS tx power at time of measurement (dBm)
	short int	m_BSTxPower;	// BTS tx power at time of measurement (dBm)
	short int	m_Hopping;		// Was the Cell Hopping No = 0; Baseband =1, Synth=2;
	bool		m_BCCH;			// Was the measurement on the BCCH
	short int	m_RxLevUpFull;	
	short int	m_RxLevUpSub;
	short int	m_RxLevDnFull;	
	short int	m_RxLevDnSub;
	short int	m_RxQualUpFull;	
	short int	m_RxQualUpSub;
	short int	m_RxQualDnFull;	
	short int	m_RxQualDnSub;
	short int 	m_NBS;			// number of neighbours in original list
};
}

#endif /*CTESTPOINT_H_*/
