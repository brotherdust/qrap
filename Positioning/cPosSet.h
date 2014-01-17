#ifndef CPOSSET_H_
#define CPOSSET_H_

#include "cMeas.h"
#include "cTPoint.h"
#include "CellList.h"
#include "cantpatn.h"
#include "cgeneric.h"

using namespace std;
using std::cout;

typedef	vector<cTPoint>  VecMMRs;
typedef vector<CellList> VecCells;
typedef	vector<cMeas> 	 VecMeas;

class cPosSet
{
public:
	cPosSet();
	virtual ~cPosSet();
	void clearOld();
	double SI(int I);
	double CI_TA(int I);
	double SecondSite(int I);
	double CoSecAzi(PGconn* DBase, int I);
	double CoSinRule(PGconn* DBase, int I);
	double DoubleDelta(PGconn* DBase, int I);
	double CorrCoefOpt(PGconn* DBase,int I, cGeoP Begin);
	double BruteForceSearch(PGconn* DBase,int I, cGeoP Begin);
	double DistanceDiff(PGconn* DBase, int I, cGeoP Begin);
	int Initialise(PGconn* DBase, cGeoP Begin);
	
	VecMMRs	m_TestPoint; 	// An array of copies of actually the same test point
							// with only a different method of position estimation.
	CellList			m_ServCell;
	CAntennaPattern		m_ServAntenna;
	VecMeas 			m_NB_Meas;
	VecCells			m_NB_Cells;
	double				m_Distance;
	double				m_Azimuth;
	double				m_Elev;
	
private:
	double funcSwitch(int Switch, cGeoP Pos, double K, double G);
	double OneMinCorr(cGeoP Pos, double K, double G);
	double squareErr(cGeoP Pos, double K, double G);
	double CosMeasPred(cGeoP Pos, double K, double G);
	double EdEdXdY(cGeoP Pos, double K, double G, double &dEdLat, double &dEdLon);
	
	CAntennaPattern		*m_AntUsed;
	cGeoP				*m_PosUsed;
	unsigned short int	m_NumUsed;
	short int			*m_IndexUsed;
	double 				*m_predUsed;
	double 				*m_EIRPused;
	double				*m_MeasUsed;
	double 				*m_Scale;
	double				m_aveMeas, m_varMeas;

};

#endif /*CPOSSET_H_*/
