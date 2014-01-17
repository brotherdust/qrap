#ifndef CPOSITION_H_
#define CPOSITION_H_

#include "cgeneric.h"
#include "cMeas.h"
#include "cTPoint.h"
#include "CellList.h"
#include "cPosSet.h"

using namespace std;
using std::cout;

typedef vector<CellList> VecCells;
typedef	vector<cTPoint>  VecMMRs;
typedef	vector<cMeas> 	 VecMeas;
typedef vector<int>		 VecInt;
typedef vector<double>	 VecDouble;

class cPosition
{
public:

	cPosition();
	virtual ~cPosition();
	
	int GetCurrentData(PGconn* DBase);
	double EstimatePositions(PGconn* DBase);
	int WriteToDatabase(PGconn* DBase);
//	int OutputEstimates();
	
	VecMeas		m_NBMeas;
	VecCells	m_TempList;
	VecMMRs		m_coreMMR;
	VecInt		m_LACList;
	int			m_lastTP;
	int			m_NumOriginalMMR;
	
	
private:

	inline double CharP_to_Double(char *Input, unsigned NumBytes)
	{
		double Val=0;
		int pp = 0;
		int ss = 0;
		unsigned i=0;
		short int sign;
		if (Input[0]=='-')
		{
				sign=-1;
				i++;
		}
		else sign=1;
		for (i;i<NumBytes;i++)
		{
			if (Input[i]!='.')
			{
				Val = Val*10 + (Input[i]-'0');
				ss++;
			}
			else pp = ss;	
		}
		if (pp) Val/=exp10(ss-pp);
		return sign*Val;
	};	
};
	

#endif /*CPOSITION_H_*/
