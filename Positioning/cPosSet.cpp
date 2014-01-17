#include "cPosSet.h"

const double TA = 553.537;
//********************************************************************
cPosSet::cPosSet()
{
	m_NumUsed = 1;
	m_Scale=new double[1];
	m_IndexUsed = new short int[1];
	m_AntUsed = new CAntennaPattern[1]; 
	m_PosUsed = new cGeoP[1];
	m_predUsed = new double[1];
	m_EIRPused = new double[1];
	m_MeasUsed = new double[1];
}

//********************************************************************
cPosSet::~cPosSet()
{
	m_NB_Meas.clear();
	m_NB_Cells.clear();
	m_TestPoint.clear();
	delete [] m_IndexUsed;
	delete [] m_PosUsed;
	delete [] m_predUsed;
	delete [] m_EIRPused;
	delete [] m_MeasUsed;
	delete [] m_Scale;
	delete [] m_AntUsed; 
}

//********************************************************************
void cPosSet::clearOld()
{
	m_NB_Meas.clear();
	m_NB_Cells.clear();
	m_TestPoint.clear();
}

//*******************************************************************
double cPosSet::SI(int I)
{
	double errorEstimate=m_TestPoint[I].m_TA*TA;
	m_TestPoint[I].m_PosSource = CI;
	unsigned i;
	int StrongestIndex=-1;
	double Strongest = -155;
	for (i=0; i < m_NB_Cells.size(); i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
		errorEstimate = m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4;
	else
		errorEstimate = 35000;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	return errorEstimate;	
}
//*******************************************************************
double cPosSet::CI_TA(int I)
{
	double errorEstimate;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	m_TestPoint[I].m_PosSource = CITA;
	unsigned i;
	int StrongestIndex=-1;
	double Strongest = -155;
	for (i=0; i < m_NB_Cells.size(); i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
	{	
		m_Distance = m_TestPoint[I].m_TA*TA+ 
					MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
	}
	m_Azimuth = m_ServCell.m_Azimuth;
	errorEstimate = MAX(TA,m_Distance*m_ServCell.m_AziBeam*PI/180.0);
	m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
	return errorEstimate;	
}

//***************************************************************************************
double cPosSet::SecondSite(int I)
{
	double errorEstimate;
	int NumNCells=m_NB_Cells.size();
	errorEstimate = 70000;
	m_TestPoint[I].m_PosSource = SiteDir;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	int i, StrongestIndex=-1;
	double Strongest = -155;
	for (i=0; i < NumNCells; i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
	{	
		m_Distance = m_TestPoint[I].m_TA*TA+ 
			MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
		m_Azimuth = m_ServCell.m_Ligging.Bearing(m_NB_Cells[StrongestIndex].m_Ligging);
		errorEstimate = MAX(TA,m_Distance);
		m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
	}
	return errorEstimate;	
	
}

//**************************************************************************************
double cPosSet::CoSecAzi(PGconn* DBase, int I)
{
	double errorEstimate, RxLevDiff;
	unsigned i, NumNCells;
	short int SecA=-1, SecB=-1;
	CAntennaPattern AntSecA, AntSecB;
	unsigned NumCo=0, C900=0, C1800=0, CUMTS=0, C50=0;
	bool *CoSitedI;
	NumNCells=m_NB_Cells.size();
	CoSitedI = new bool[NumNCells];
	
	errorEstimate = 70000.0;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	m_TestPoint[I].m_PosSource  = AZI;
	double Strongest = -155;
	int StrongestIndex=-1;
	for (i=0; i < NumNCells; i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
	{	
		m_Distance = m_TestPoint[I].m_TA*TA+ 
				MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
	}
	else m_Distance = (m_TestPoint[I].m_TA+0.5)*TA;
	
	//Look for co-sites, same tech/band
	NumCo=0;
	for (i=0; i< NumNCells; i++)
	{
//		cout << "SC: " << m_ServCell.m_Site << "  NB: " << m_NB_Cells[i].m_Site << endl;
		if ((m_ServCell.m_Site == m_NB_Cells[i].m_Site)&&
				(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111))
		{
			CoSitedI[i]=TRUE;
			NumCo++;
			if (m_NB_Cells[i].m_ARFCN < 252)
				C900++;
			else if (m_NB_Cells[i].m_ARFCN < 512)
				C50++;
			else if (m_NB_Cells[i].m_ARFCN < 886)
				C1800++;
			else if (m_NB_Cells[i].m_ARFCN < 1024)
				C900++;
			else CUMTS++;
		}
		else CoSitedI[i]=FALSE;
	}
	if ((C900>C1800)&&(C900>C50)&&(C900>CUMTS))
	{
		for (i=0; i< NumNCells; i++)
		{
			if ((m_NB_Cells[i].m_ARFCN>1023)||
				((m_NB_Cells[i].m_ARFCN>251)&&(m_NB_Cells[i].m_ARFCN<886)))
			CoSitedI[i]=FALSE;
		}
	}
	else if ((C1800>C900)&&(C1800>C50)&&(C1800>CUMTS))
	{
		for (i=0; i< NumNCells; i++)
		{
			if (!((m_NB_Cells[i].m_ARFCN>511)&&(m_NB_Cells[i].m_ARFCN<886)))
				CoSitedI[i]=FALSE;
		}
	}
	else 
	{
		for (i=0; i< NumNCells; i++)
		{
			if (m_NB_Cells[i].m_ARFCN<1024)
				CoSitedI[i]=FALSE;
		}
	}
	for (i=0; i< NumNCells; i++)
	{
		if (CoSitedI[i])
		{
			if (SecA<0)
				SecA=i;
			else if ((m_ServCell.m_CI == m_NB_Cells[i].m_CI)&&(SecB<0))
			{
				SecB=SecA;
				SecA=i;
			}		
			else if (SecB<0)
				SecB=i;
			else if (i<m_NB_Meas.size())
				if ((m_NB_Meas[i].m_MeasValue>m_NB_Meas[SecB].m_MeasValue)
						&&(m_NB_Meas[SecA].m_MeasValue>m_NB_Meas[SecB].m_MeasValue))
					SecB=i;
				else if (m_NB_Meas[i].m_MeasValue>m_NB_Meas[SecA].m_MeasValue)
					SecA=i;
		}
	}
	if ((SecA<0)||(NumCo<2)||(SecB<0))
	{
//		cout << "Bummer ... Relative antenna azimuth method won't work" << endl;
		delete CoSitedI;
		return 70000;
	}
	
/*	will only work if one actually have the RxLevDn measurments
 * 	if ((unsigned)SecA > m_NB_Meas.size())
		RxLevDiff = -110.0+m_TestPoint[I].m_RxLevDnSub+2.0*m_TestPoint[I].m_BSTxPower 
						- m_NB_Meas[SecB].m_MeasValue;
	else */
	RxLevDiff = m_NB_Meas[SecA].m_MeasValue - m_NB_Meas[SecB].m_MeasValue;
	if (fabs(RxLevDiff)>16)
	{
//		cout << " Azi not good idea ? "<< endl;
		delete CoSitedI;
		return 70000;
	}
/*	
	cout << "Delta: " << RxLevDiff 	<< "   AMeas: " << m_NB_Meas[SecA].m_MeasValue 
									<< "   BMeas: " << m_NB_Meas[SecB].m_MeasValue << endl; 
	cout << "A: " << m_NB_Cells[SecA].m_CI << " Key: "<< m_NB_Cells[SecA].m_AntKey << " Az: " << m_NB_Cells[SecA].m_Azimuth << endl;
	cout << "B: " << m_NB_Cells[SecB].m_CI << " Key: "<< m_NB_Cells[SecB].m_AntKey << " Az: " << m_NB_Cells[SecB].m_Azimuth << endl;
*/
	AntSecA.SetAntennaPattern(DBase, m_NB_Cells[SecA].m_AntKey, 
								m_NB_Cells[SecA].m_Azimuth, m_NB_Cells[SecA].m_MechTilt);
	AntSecB.SetAntennaPattern(DBase, m_NB_Cells[SecB].m_AntKey, 
								m_NB_Cells[SecB].m_Azimuth, m_NB_Cells[SecB].m_MechTilt);	
	double DegRes = MIN(10.0, max(90*TA/PI/m_Distance,1.0));
	double DegBegin=0, DegEnd=360; 
	if (RxLevDiff>=0)
	{
		DegBegin = m_NB_Cells[SecA].m_Azimuth-m_NB_Cells[SecA].m_AziBeam; 
		DegEnd = m_NB_Cells[SecA].m_Azimuth +m_NB_Cells[SecA].m_AziBeam;
	}
	else
	{
		DegBegin = MIN(m_Azimuth,m_NB_Cells[SecB].m_Azimuth)-m_NB_Cells[SecB].m_AziBeam; 
		DegEnd = MAX(m_Azimuth,m_NB_Cells[SecB].m_Azimuth) +m_NB_Cells[SecB].m_AziBeam;
	}
	if ((DegEnd-DegBegin)>360.0)
	{
		DegBegin = 0.0;
		DegEnd = 360.0;
	}
	
	double MinnDelta;
	double Delta;
	double antValueA, antValueB;
	m_Elev = 180*atan2(20.0,m_Distance)/PI;
//	m_Elev = 0.0;
	antValueA = AntSecA.GetPatternValue(m_Azimuth, m_Elev);
	antValueB = AntSecB.GetPatternValue(m_Azimuth, m_Elev);
	MinnDelta = fabs ( RxLevDiff - m_NB_Cells[SecA].m_EIRP + antValueA
			 							+ m_NB_Cells[SecB].m_EIRP - antValueB);
	
	unsigned steps = (int)((DegEnd - DegBegin +0.5)/DegRes); 
	
	for (i=0; i<steps ; i++)
	{
		antValueA = AntSecA.GetPatternValue(DegBegin+DegRes*i, m_Elev);
		antValueB = AntSecB.GetPatternValue(DegBegin+DegRes*i, m_Elev);
	 	Delta = fabs ( RxLevDiff - m_NB_Cells[SecA].m_EIRP + antValueA
	 							+ m_NB_Cells[SecB].m_EIRP - antValueB);
/*	 	cout << "Azi: "<< DegBegin+DegRes*i  << "  Delta: " << Delta << "   Diff: " << RxLevDiff << endl;
	 	cout 	<< "SecA: " <<  m_NB_Cells[SecA].m_CI << "  EIRP: " << m_NB_Cells[SecA].m_EIRP 
	 			<< "   AntA: " << antValueA 
	 			<< "  AziA: "<< m_NB_Cells[SecA].m_Azimuth - DegBegin+DegRes*i << endl;
	 	cout 	<< "SecB: " <<  m_NB_Cells[SecB].m_CI << "  EIRP: " << m_NB_Cells[SecB].m_EIRP 
	 		 			<< "   AntB: " << antValueB 
	 		 			<< "  AziB: "<< m_NB_Cells[SecB].m_Azimuth - DegBegin+DegRes*i  << endl;
*/
	 	if ((Delta<MinnDelta)&&(antValueA<17)&&(antValueB<17))
	 	{
	 		MinnDelta = Delta;
	 		m_Azimuth = DegBegin+DegRes*i;
	 	}
	}
//	cout << "AZI: " << m_Azimuth << endl; 
	m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
//	m_TestPoint[I].m_Position.Display();
//	cout << "  mD: " <<m_Distance << "  mB: "<< m_Azimuth << endl;
	errorEstimate = max(TA/2,DegRes*PI*m_Distance/180);
	delete CoSitedI;
	return errorEstimate;	
}

//**************************************************************************************
double cPosSet::CoSinRule(PGconn* DBase, int I)
{
	double errorEstimate, RxLevDiff;
	unsigned i=0,j=0, NumNCells;
	short int Sec1A=-1, Sec1B=-1, Sec2A=-1, Sec2B=-1,TechA, TechB;
	CAntennaPattern AntSec1A, AntSec1B, AntSec2A, AntSec2B;;
	double Azimuth1, Azimuth2;
	double Distance; // Distance between 2 sites
	bool *CoSitedI;
	bool found1 = FALSE, found2=FALSE;
	NumNCells=m_NB_Cells.size();
	CoSitedI = new bool[NumNCells];
	
	errorEstimate = 70000.0;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	m_TestPoint[I].m_PosSource  = CosRule;

	for (i=0;i<NumNCells;i++)
		CoSitedI[i]=FALSE;
	//Look for co-sites, same tech/band
	i=0;
	
	while ((!found1)&&(i<NumNCells))
	{
		j=i+1;
		if ((m_ServCell.m_Site != m_NB_Cells[i].m_Site)
			&&(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111))
		{
			while ((!found1)&&(j<NumNCells))
			{
//			cout << "SC: " << m_ServCell.m_Site << "  NB: " << m_NB_Cells[i].m_Site << endl;
				if ((m_NB_Cells[j].m_Site == m_NB_Cells[i].m_Site)&&
					(m_NB_Meas[j].m_MeasValue<-46)&&(m_NB_Meas[j].m_MeasValue>-111))
				{
					if (m_NB_Cells[i].m_ARFCN < 252)
						TechA=0;
					else if (m_NB_Cells[i].m_ARFCN < 512)
						TechA=1;
					else if (m_NB_Cells[i].m_ARFCN < 886)
						TechA=2;
					else if (m_NB_Cells[i].m_ARFCN < 1024)
						TechA=0;
					else TechA=3;
					if (m_NB_Cells[j].m_ARFCN < 252)
						TechB=0;
					else if (m_NB_Cells[j].m_ARFCN < 512)
						TechB=1;
					else if (m_NB_Cells[j].m_ARFCN < 886)
						TechB=2;
					else if (m_NB_Cells[j].m_ARFCN < 1024)
						TechB=0;
					else TechB=3;
					found1 = (TechA==TechB);
					CoSitedI[i]=found1;
					CoSitedI[j]=found1;
					Sec1A=i;
					Sec1B=j;
				}
				j++;
			}
		}
		i++;
	}

	
	if ((Sec1A<0)||(!found1)||(Sec1B<0))
	{
//		cout << "Bummer ... Cosine rule method won't work" << endl;
		delete CoSitedI;
		return 70000;
	}
	RxLevDiff = m_NB_Meas[Sec1A].m_MeasValue - m_NB_Meas[Sec1B].m_MeasValue;
/*	if (fabs(RxLevDiff)>20)
	{
		found1 = false;
		delete CoSitedI;
		return 70000;
	}
*/	
	Distance = m_NB_Cells[Sec1A].m_Ligging.Distance(m_ServCell.m_Ligging);

	AntSec1A.SetAntennaPattern(DBase, m_NB_Cells[Sec1A].m_AntKey, 
								m_NB_Cells[Sec1A].m_Azimuth, m_NB_Cells[Sec1A].m_MechTilt);
	AntSec1B.SetAntennaPattern(DBase, m_NB_Cells[Sec1B].m_AntKey, 
								m_NB_Cells[Sec1B].m_Azimuth, m_NB_Cells[Sec1B].m_MechTilt);	
	double DegRes = MIN(10.0, max(90*TA/PI/Distance,1.0));
	double DegBegin=0, DegEnd=360; 
	if (RxLevDiff>=0)
	{
		DegBegin = m_NB_Cells[Sec1A].m_Azimuth-m_NB_Cells[Sec1A].m_AziBeam; 
		DegEnd = m_NB_Cells[Sec1A].m_Azimuth +m_NB_Cells[Sec1A].m_AziBeam;
	}
	else
	{
		DegBegin = m_NB_Cells[Sec1B].m_Azimuth-m_NB_Cells[Sec1B].m_AziBeam; 
		DegEnd = m_NB_Cells[Sec1B].m_Azimuth +m_NB_Cells[Sec1B].m_AziBeam;
	}
	if ((DegEnd-DegBegin)>360.0)
	{
		DegBegin = 0.0;
		DegEnd = 360.0;
	}
	
	double MinnDelta;
	double Delta;
	double antValueA, antValueB;
	double Elev = 180*atan2(20,Distance);
	Azimuth1 = m_NB_Cells[Sec1A].m_Azimuth;
	antValueA = AntSec1A.GetPatternValue(Azimuth1, Elev);
	antValueB = AntSec1B.GetPatternValue(Azimuth1, Elev);
	MinnDelta = fabs ( RxLevDiff - m_NB_Cells[Sec1A].m_EIRP + antValueA
			+ m_NB_Cells[Sec1B].m_EIRP - antValueB);
	
	unsigned steps = (int)((DegEnd - DegBegin +0.5)/DegRes); 
	
	for (i=0; i<steps ; i++)
	{
		antValueA = AntSec1A.GetPatternValue(DegBegin+DegRes*i, Elev);
		antValueB = AntSec1B.GetPatternValue(DegBegin+DegRes*i, Elev);
	 	Delta = fabs ( RxLevDiff - m_NB_Cells[Sec1A].m_EIRP + antValueA
	 				+ m_NB_Cells[Sec1B].m_EIRP - antValueB);
/*	 	cout << "Azi: "<< DegBegin+DegRes*i  << "  Delta: " << Delta << "   Diff: " << RxLevDiff << endl;
	 	cout 	<< "SecA: " <<  m_NB_Cells[Sec1A].m_CI << "  EIRP: " << m_NB_Cells[Sec1A].m_EIRP 
	 			<< "   AntA: " << antValueA 
	 			<< "  AziA: "<< m_NB_Cells[Sec1A].m_Azimuth - DegBegin+DegRes*i << endl;
	 	cout 	<< "SecB: " <<  m_NB_Cells[Sec1B].m_CI << "  EIRP: " << m_NB_Cells[Sec1B].m_EIRP 
	 		 			<< "   AntB: " << antValueB 
	 		 			<< "  AziB: "<< m_NB_Cells[Sec1B].m_Azimuth - DegBegin+DegRes*i  << endl;
*/
	 	if ((Delta<MinnDelta)&&(antValueA<16)&&(antValueB<16))
	 	{
	 		MinnDelta = Delta;
	 		Azimuth1 = DegBegin+DegRes*i;
	 	}
	}
		
	i=0;
	while ((!found2)&&(i<NumNCells))
	{
		j=i+1;
		if ((!CoSitedI[i])&&(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111))
		{
			while ((!found2)&&(j<NumNCells))
			{
//			cout << "SC: " << m_ServCell.m_Site << "  NB: " << m_NB_Cells[i].m_Site << endl;
				if ((m_NB_Cells[j].m_Site == m_NB_Cells[i].m_Site)&&
					(m_NB_Meas[j].m_MeasValue<-46)&&(m_NB_Meas[j].m_MeasValue>-111))
				{
					if (m_NB_Cells[i].m_ARFCN < 252)
						TechA=0;
					else if (m_NB_Cells[i].m_ARFCN < 512)
						TechA=1;
					else if (m_NB_Cells[i].m_ARFCN < 886)
						TechA=2;
					else if (m_NB_Cells[i].m_ARFCN < 1024)
						TechA=0;
					else TechA=3;
					if (m_NB_Cells[j].m_ARFCN < 252)
						TechB=0;
					else if (m_NB_Cells[j].m_ARFCN < 512)
						TechB=1;
					else if (m_NB_Cells[j].m_ARFCN < 886)
						TechB=2;
					else if (m_NB_Cells[j].m_ARFCN < 1024)
						TechB=0;
					else TechB=3;
					found2 = (TechA==TechB);
					CoSitedI[i]=found2;
					CoSitedI[j]=found2;
					Sec2A=i;
					Sec2B=j;
				}
				j++;
			}
		}
		i++;
	}	
	
	
	if (found2)
	{
		RxLevDiff = m_NB_Meas[Sec2A].m_MeasValue - m_NB_Meas[Sec2B].m_MeasValue;
//		if (fabs(RxLevDiff)>16)
//			found2 = false;
//		else
		{
			Distance = m_NB_Cells[Sec2A].m_Ligging.Distance(m_NB_Cells[Sec1A].m_Ligging);
			AntSec2A.SetAntennaPattern(DBase, m_NB_Cells[Sec2A].m_AntKey, 
								m_NB_Cells[Sec2A].m_Azimuth, m_NB_Cells[Sec2A].m_MechTilt);
			AntSec2B.SetAntennaPattern(DBase, m_NB_Cells[Sec2B].m_AntKey, 
								m_NB_Cells[Sec2B].m_Azimuth, m_NB_Cells[Sec2B].m_MechTilt);	
			DegRes = MIN(10.0, max(90*TA/PI/Distance,1.0));
			DegBegin=0, DegEnd=360; 
			if (RxLevDiff>=0)
			{
				DegBegin = m_NB_Cells[Sec2A].m_Azimuth-m_NB_Cells[Sec2A].m_AziBeam; 
				DegEnd = m_NB_Cells[Sec2A].m_Azimuth +m_NB_Cells[Sec2A].m_AziBeam;
			}
			else
			{
				DegBegin = m_NB_Cells[Sec2B].m_Azimuth-m_NB_Cells[Sec2B].m_AziBeam; 
				DegEnd = m_NB_Cells[Sec2B].m_Azimuth +m_NB_Cells[Sec2B].m_AziBeam;
			}
			if ((DegEnd-DegBegin)>360.0)
			{
				DegBegin = 0.0;
				DegEnd = 360.0;
			}
	
			Azimuth2 = m_NB_Cells[Sec2A].m_Azimuth;
			antValueA = AntSec2A.GetPatternValue(Azimuth2, Elev);
			antValueB = AntSec2B.GetPatternValue(Azimuth2, Elev);
			MinnDelta = fabs ( RxLevDiff - m_NB_Cells[Sec2A].m_EIRP + antValueA
						+ m_NB_Cells[Sec2B].m_EIRP - antValueB);
	
			steps = (int)((DegEnd - DegBegin +0.5)/DegRes); 
	
			for (i=0; i<steps ; i++)
			{
				antValueA = AntSec2A.GetPatternValue(DegBegin+DegRes*i, Elev);
				antValueB = AntSec2B.GetPatternValue(DegBegin+DegRes*i, Elev);
				Delta = fabs ( RxLevDiff - m_NB_Cells[Sec2A].m_EIRP + antValueA
	 							+ m_NB_Cells[Sec2B].m_EIRP - antValueB);
/*	 			cout << "Azi: "<< DegBegin+DegRes*i  << "  Delta: " << Delta << "   Diff: " << RxLevDiff << endl;
	 			cout 	<< "SecA: " <<  m_NB_Cells[Sec2A].m_CI << "  EIRP: " << m_NB_Cells[Sec2A].m_EIRP 
	 				<< "   AntA: " << antValueA 
	 				<< "  AziA: "<< m_NB_Cells[Sec2A].m_Azimuth - DegBegin+DegRes*i << endl;
	 			cout << "SecB: " <<  m_NB_Cells[Sec2B].m_CI << "  EIRP: " << m_NB_Cells[Sec2B].m_EIRP 
	 				<< "   AntB: " << antValueB 
	 				<< "  AziB: "<< m_NB_Cells[Sec2B].m_Azimuth - DegBegin+DegRes*i  << endl;
*/
				if ((Delta<MinnDelta)&&(antValueA<16)&&(antValueB<16))
				{
					MinnDelta = Delta;
					Azimuth2 = DegBegin+DegRes*i;
				}
			}
		
			double A, B, C, alpha, beta, gamma, onderwortel, QB, QC, QA;
			alpha = (m_NB_Cells[Sec1A].m_Ligging.Bearing(m_NB_Cells[Sec2A].m_Ligging)-Azimuth1);
			if (alpha > 180)
				alpha -=360;
			beta = (Azimuth2-m_NB_Cells[Sec2A].m_Ligging.Bearing(m_NB_Cells[Sec1A].m_Ligging));
			if (beta >180)
				beta -=360;
			if (alpha*beta<0)
				found2=FALSE;
			else
			{
				if (alpha<0)
				{
					alpha*=-1;
					beta*=-1;
				}
				gamma = 180 - alpha -beta;
				alpha*=PI/180;
				beta*=PI/180;
				gamma*=PI/180;
				C = m_NB_Cells[Sec2A].m_Ligging.Distance(m_NB_Cells[Sec1A].m_Ligging);
				QB = 2*C*(cos(gamma)*cos(alpha)+cos(beta));
				QA = sin(gamma)*sin(gamma);
				QC = C*C*sin(alpha)*sin(alpha);
				onderwortel = QB*QB-4*QA*QC;
				if (onderwortel<0)
					found2 =FALSE;
				else
				{
					A = (-2.0*QB-sqrt(onderwortel))/(2.0*QA);
					B = -C*cos(alpha)+A*cos(gamma);
					if (A<0) 
						found2 = FALSE;
					else
					{
						if ((m_NB_Cells[Sec2A].m_Site == m_ServCell.m_Site)
							&&(A>((m_TestPoint[I].m_TA+1)*TA)))
							A = (m_TestPoint[I].m_TA+1)*TA;
						m_TestPoint[I].m_Position.FromHere(m_NB_Cells[Sec2A].m_Ligging, A, Azimuth2);
						m_Distance = m_TestPoint[I].m_Position.Distance(m_ServCell.m_Ligging);
						m_Azimuth = m_ServCell.m_Ligging.Bearing(m_TestPoint[I].m_Position);
					}
				}
			}
		}
	}
	
	if (!found2)
	{
		m_TestPoint[I].m_PosSource = CosRule2;
		double A,B, C, alpha, onderwortel, beta, QB,QC;
		C = Distance;
		unsigned i;
		int StrongestIndex=-1;
		double Strongest = -165;
		for (i=0; i < m_NB_Cells.size(); i++)
		{
			if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
					&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
			{
				Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
				StrongestIndex = i;
			}
		}
		if (StrongestIndex!=-1)
		{	
			B = m_TestPoint[I].m_TA*TA+ 
				MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
		}
		else B=((m_TestPoint[I].m_TA+0.5)*TA);
		alpha = ((m_NB_Cells[Sec1A].m_Ligging.Bearing(m_ServCell.m_Ligging)-Azimuth1)*PI/180 );
		if (alpha>180)
			alpha-=360;
		alpha=fabs(alpha);
		QB = -2.0*C*cos(alpha);
		QC = C*C - B*B;
		onderwortel = QB*QB-4.0*QC;
		if (onderwortel<0)
		{
			A = MIN(fabs(C*sin(alpha)),(1+m_TestPoint[I].m_TA)*TA);
			beta = acos(A/C);
			m_Azimuth = m_ServCell.m_Ligging.Bearing(m_NB_Cells[Sec1A].m_Ligging)-beta*alpha/fabs(alpha)*180/PI;
			m_Distance = A;
			m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
		}
		else
		{
			B = 2*C*cos(alpha)-sqrt(onderwortel)/2;
			if (B<0)
				B = 2*C*cos(alpha)+sqrt(onderwortel)/2;
			m_TestPoint[I].m_Position.FromHere(m_NB_Cells[Sec1A].m_Ligging, B, Azimuth1);
			m_Distance = m_TestPoint[I].m_Position.Distance(m_ServCell.m_Ligging);
			m_Azimuth = m_ServCell.m_Ligging.Bearing(m_TestPoint[I].m_Position);
		}
	}
	
	errorEstimate = max(TA/2,DegRes*PI*m_Distance/180);
	delete CoSitedI;
	return errorEstimate;	
}


//**************************************************************************************
double cPosSet::DoubleDelta(PGconn* DBase, int I)
{
	double errorEstimate, RxLevDiff1, RxLevDiff2;
	unsigned i=0,j=0, NumNCells;
	short int Sec1A=-1, Sec1B=-1, Sec2A=-1, Sec2B=-1,TechA, TechB;
	CAntennaPattern AntSec1A, AntSec1B, AntSec2A, AntSec2B;
	double Azimuth1;
	double Distance; // Distance between 2 sites
	bool *CoSitedI;
	bool found1 = FALSE, found2=FALSE;
	NumNCells=m_NB_Cells.size();
	CoSitedI = new bool[NumNCells];
	
	errorEstimate = 70000.0;
	m_TestPoint[I].m_PosSource  = DDelta;
	m_TestPoint[I].m_Position=m_ServCell.m_Ligging;
	int StrongestIndex=-1;
	double Strongest = -155;
	for (i=0; i < m_NB_Cells.size(); i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
	{	
		m_Distance = m_TestPoint[I].m_TA*TA+ 
					MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
	}

	for (i=0;i<NumNCells;i++)
		CoSitedI[i]=FALSE;
	//Look for co-sites, same tech/band
	i=0;
	
	while ((!found1)&&(i<NumNCells))
	{
		j=i+1;
		if ((m_ServCell.m_Site != m_NB_Cells[i].m_Site)
			&&(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111))
		{
			while ((!found1)&&(j<NumNCells))
			{
//			cout << "SC: " << m_ServCell.m_Site << "  NB: " << m_NB_Cells[i].m_Site << endl;
				if ((m_NB_Cells[j].m_Site == m_NB_Cells[i].m_Site)&&
					(m_NB_Meas[j].m_MeasValue<-46)&&(m_NB_Meas[j].m_MeasValue>-111))
				{
					if (m_NB_Cells[i].m_ARFCN < 252)
						TechA=0;
					else if (m_NB_Cells[i].m_ARFCN < 512)
						TechA=1;
					else if (m_NB_Cells[i].m_ARFCN < 886)
						TechA=2;
					else if (m_NB_Cells[i].m_ARFCN < 1024)
						TechA=0;
					else TechA=3;
					if (m_NB_Cells[j].m_ARFCN < 252)
						TechB=0;
					else if (m_NB_Cells[j].m_ARFCN < 512)
						TechB=1;
					else if (m_NB_Cells[j].m_ARFCN < 886)
						TechB=2;
					else if (m_NB_Cells[j].m_ARFCN < 1024)
						TechB=0;
					else TechB=3;
					found1 = (TechA==TechB);
					CoSitedI[i]=found1;
					CoSitedI[j]=found1;
					Sec1A=i;
					Sec1B=j;
				}
				j++;
			}
		}
		i++;
	}
	
	if ((Sec1A<0)||(!found1)||(Sec1B<0))
	{
//		cout << "Bummer ... Cosine rule method won't work" << endl;
		m_TestPoint[I].m_PosSource = DDelta;
		return 70000;
	}
	
	RxLevDiff1 = m_NB_Meas[Sec1A].m_MeasValue - m_NB_Meas[Sec1B].m_MeasValue;
	if (fabs(RxLevDiff1)>20)
	{
		found1=FALSE;
		m_TestPoint[I].m_PosSource = DDelta;
		delete CoSitedI;
		return 70000;
	}
	i=0;
	while ((!found2)&&(i<NumNCells))
	{
		j=i+1;
		if ((!CoSitedI[i])&&(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111))
		{
			while ((!found2)&&(j<NumNCells))
			{
//			cout << "SC: " << m_ServCell.m_Site << "  NB: " << m_NB_Cells[i].m_Site << endl;
				if ((m_NB_Cells[j].m_Site == m_NB_Cells[i].m_Site)&&
					(m_NB_Meas[j].m_MeasValue<-46)&&(m_NB_Meas[j].m_MeasValue>-111))
				{
					if (m_NB_Cells[i].m_ARFCN < 252)
						TechA=0;
					else if (m_NB_Cells[i].m_ARFCN < 512)
						TechA=1;
					else if (m_NB_Cells[i].m_ARFCN < 886)
						TechA=2;
					else if (m_NB_Cells[i].m_ARFCN < 1024)
						TechA=0;
					else TechA=3;
					if (m_NB_Cells[j].m_ARFCN < 252)
						TechB=0;
					else if (m_NB_Cells[j].m_ARFCN < 512)
						TechB=1;
					else if (m_NB_Cells[j].m_ARFCN < 886)
						TechB=2;
					else if (m_NB_Cells[j].m_ARFCN < 1024)
						TechB=0;
					else TechB=3;
					found2 = (TechA==TechB);
					CoSitedI[i]=found2;
					CoSitedI[j]=found2;
					Sec2A=i;
					Sec2B=j;
				}
				j++;
			}
		}
		i++;
	}	
	if ((Sec2A<0)||(Sec2B<0)||(!found2))
	{
		m_TestPoint[I].m_PosSource = DDelta;
		delete CoSitedI;
		return 70000;
	}
	RxLevDiff2 = m_NB_Meas[Sec2A].m_MeasValue - m_NB_Meas[Sec2B].m_MeasValue;
	if (fabs(RxLevDiff2)>16)
	{
		found2=FALSE;
		m_TestPoint[I].m_PosSource = DDelta;
		delete CoSitedI;
		return 70000;
	}
	
	
	AntSec1A.SetAntennaPattern(DBase, m_NB_Cells[Sec1A].m_AntKey, 
								m_NB_Cells[Sec1A].m_Azimuth, m_NB_Cells[Sec1A].m_MechTilt);
	AntSec1B.SetAntennaPattern(DBase, m_NB_Cells[Sec1B].m_AntKey, 
								m_NB_Cells[Sec1B].m_Azimuth, m_NB_Cells[Sec1B].m_MechTilt);	
	double DegRes = MIN(10.0, max(90*TA/PI/Distance,1.0));
	double DegBegin=0, DegEnd=360; 
	if (RxLevDiff1>=0)
	{
		DegBegin = m_NB_Cells[Sec1A].m_Azimuth-m_NB_Cells[Sec1A].m_AziBeam; 
		DegEnd = m_NB_Cells[Sec1A].m_Azimuth +m_NB_Cells[Sec1A].m_AziBeam;
	}
	else
	{
		DegBegin = m_NB_Cells[Sec1B].m_Azimuth-m_NB_Cells[Sec1B].m_AziBeam; 
		DegEnd = m_NB_Cells[Sec1B].m_Azimuth +m_NB_Cells[Sec1B].m_AziBeam;
	}
	if ((DegEnd-DegBegin)>360.0)
	{
		DegBegin = 0.0;
		DegEnd = 360.0;
	}
	
	double MinnDelta;
	double Delta;
	double antValueA, antValueB;
	double Elev = 180*atan2(20,Distance);
	cGeoP TempPos;
	double Bearing2;
	Azimuth1 = m_NB_Cells[Sec1A].m_Azimuth;
	antValueA = AntSec1A.GetPatternValue(Azimuth1, Elev);
	antValueB = AntSec1B.GetPatternValue(Azimuth1, Elev);
	MinnDelta =-110;
//	MinnDelta = fabs ( RxLevDiff - m_NB_Cells[Sec1A].m_EIRP + antValueA
//			 							+ m_NB_Cells[Sec1B].m_EIRP - antValueB);

	AntSec2A.SetAntennaPattern(DBase, m_NB_Cells[Sec2A].m_AntKey, 
									m_NB_Cells[Sec2A].m_Azimuth, m_NB_Cells[Sec2A].m_MechTilt);
	AntSec2B.SetAntennaPattern(DBase, m_NB_Cells[Sec2B].m_AntKey, 
									m_NB_Cells[Sec2B].m_Azimuth, m_NB_Cells[Sec2B].m_MechTilt);	
	
	unsigned steps = (int)((DegEnd - DegBegin +0.5)/DegRes); 
	
	for (i=0; i<steps ; i++)
	{
		antValueA = AntSec1A.GetPatternValue(DegBegin+DegRes*i, Elev);
		antValueB = AntSec1B.GetPatternValue(DegBegin+DegRes*i, Elev);
	 	Delta = fabs ( RxLevDiff1 - m_NB_Cells[Sec1A].m_EIRP + antValueA
	 							+ m_NB_Cells[Sec1B].m_EIRP - antValueB);
	 	TempPos.FromHere(m_NB_Cells[Sec1A].m_Ligging,m_Distance,DegBegin+DegRes*i);
	 	Bearing2 = m_NB_Cells[Sec2A].m_Ligging.Bearing(TempPos);
	 	antValueA = AntSec1A.GetPatternValue(Bearing2, Elev);
	 	antValueB = AntSec1B.GetPatternValue(Bearing2, Elev);
	 	Delta += fabs ( RxLevDiff2 - m_NB_Cells[Sec2A].m_EIRP + antValueA
					+ m_NB_Cells[Sec2B].m_EIRP - antValueB);
/*	 	cout << "Azi: "<< DegBegin+DegRes*i  << "  Delta: " << Delta << "   Diff: " << RxLevDiff << endl;
	 	cout 	<< "SecA: " <<  m_NB_Cells[Sec1A].m_CI << "  EIRP: " << m_NB_Cells[Sec1A].m_EIRP 
	 			<< "   AntA: " << antValueA 
	 			<< "  AziA: "<< m_NB_Cells[Sec1A].m_Azimuth - DegBegin+DegRes*i << endl;
	 	cout 	<< "SecB: " <<  m_NB_Cells[Sec1B].m_CI << "  EIRP: " << m_NB_Cells[Sec1B].m_EIRP 
	 		 			<< "   AntB: " << antValueB 
	 		 			<< "  AziB: "<< m_NB_Cells[Sec1B].m_Azimuth - DegBegin+DegRes*i  << endl;
*/
	 	if (Delta<MinnDelta)
	 	{
	 		MinnDelta = Delta;
	 		Azimuth1 = DegBegin+DegRes*i;
	 	}
	}

	m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, Azimuth1);	
	m_TestPoint[I].m_PosSource = DDelta;
	errorEstimate = max(TA/2,DegRes*PI*m_Distance/180);
	delete CoSitedI;
	return errorEstimate;	
}


//*******************************************************************
double cPosSet::BruteForceSearch(PGconn* DBase, int I,cGeoP Begin)
{
	double errorEstimate=70000.0;
	double K = 32, G = 3.2;

	double Bearing, Dist;
	double func, funcMin;
	m_TestPoint[I].m_PosSource = Brutal;
	int j;
	unsigned i, k;
	int Fchoice = 0;
	cGeoP TempPos;
	double TotWeight = 0.0;
	for (i=0; i<m_NumUsed; i++)
		TotWeight+=m_Scale[i];
	if (TotWeight <1.05)
	{
//		cout << "too little to work on. " << endl;
		m_TestPoint[I].m_PosSource = Brutal;
		return 70000;	
	}
	int StrongestIndex=-1;
	double Strongest = -155;
	for (i=0; i < m_NB_Cells.size(); i++)
	{
		if (((m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP)>Strongest)
				&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			Strongest = m_NB_Meas[i].m_MeasValue-m_NB_Cells[i].m_EIRP;
			StrongestIndex = i;
		}
	}
	if (StrongestIndex!=-1)
	{	
		m_Distance = m_TestPoint[I].m_TA*TA+ 
					MAX(0,MIN(m_ServCell.m_Ligging.Distance(m_NB_Cells[StrongestIndex].m_Ligging)/4-m_TestPoint[I].m_TA*TA,TA));
	}
	else
		m_Distance = (0.5+m_TestPoint[I].m_TA)*TA;
	
	funcMin = 2;
	m_Elev = 180*atan2(20,m_Distance);
	
	m_Azimuth = m_ServCell.m_Ligging.Bearing(Begin);
	double DegRes = MIN(10.0, max(90*TA/PI/m_Distance,1.0));
	double DegBegin = m_ServCell.m_Azimuth - m_ServCell.m_AziBeam; 
	double DegEnd = m_ServCell.m_Azimuth + m_ServCell.m_AziBeam;
	unsigned NumPoints = (int)trunc((double)(DegEnd-DegBegin)/DegRes+0.5);
	if ((DegEnd-DegBegin) >360)	
	{ 
		DegBegin=0; 
		DegEnd=360;
	}
/*	Theta = m_PosUsed[0].Bearing(Begin);
	R = Begin.Distance(m_PosUsed[0]);
	Elev = 180*atan2(20.0,R)/PI;
	antValue=m_AntUsed[0].GetPatternValue(Theta, Elev);
	m_predUsed[0] = m_EIRPused[0]-antValue- K -G*10.0*log10(R);
	double minErr=(m_MeasUsed[0]-m_predUsed[0])*(m_MeasUsed[0]-m_predUsed[0]);
	double squareErr;
	double Gseek;
	
	for (j=0;j< 17;j++)
	{
		Gseek=2.0+j*0.1;
		Theta = m_PosUsed[0].Bearing(Begin);
		R = Begin.Distance(m_PosUsed[0]);
		Elev = 180*atan2(20.0,R)/PI;
		antValue=m_AntUsed[0].GetPatternValue(Theta, Elev);
		m_predUsed[0] = m_EIRPused[0]-antValue- K -G*10.0*log10(R);
		squareErr=(m_MeasUsed[0]-m_predUsed[0])*(m_MeasUsed[0]-m_predUsed[0]);
//		cout << "D: " << Dist << "   B: " << Bearing << "   G: "<< G <<"   F: "<< func << endl;
		if (squareErr<minErr)
		{
			minErr=squareErr;
			G=Gseek;
		}
	}
*/

/* Toets antenna patrone
 * 	cout << "sc_CI: " << m_ServCell.m_CI << "  Elev: "<< m_Elev << endl ;
	for (j=0;j< NumNCells; j++)
	{
		for (i=0;i<360;i++)
		{
			Bearing = i;
			cout << "CI:  " <<m_NB_Cells[j].m_CI << "  antAZI: " << m_NB_Cells[j].m_Azimuth
					<< "   i: "<< i << "  AntValue: " 
					<< m_AntUsed[j].GetPatternValue(Bearing, m_Elev) << endl;
		}
	}
*/
	double Gmin = 2.0;
	funcMin = 70000;
	for (k=0; k<12; k++)
	{
		Dist = (m_TestPoint[I].m_TA+k/11)*TA;
		for (i=0; i<NumPoints; i++)
		{
			Bearing = DegBegin+i*DegRes;
			TempPos.FromHere(m_ServCell.m_Ligging,Dist,Bearing);
			for (j=0;j< 16;j++)
			{
				G=2.4+j*0.1;
				func = funcSwitch(Fchoice,TempPos,K,G);
//				cout << "D: " << Dist << "   B: " << Bearing << "   G: "<< G <<"   F: "<< func << endl;
				if (func<funcMin)
				{
					funcMin=func;
					m_Azimuth = Bearing;
					m_Distance = Dist;
					Gmin = G;
				}
			}
		}
	}
		
	
//	cout << "Brute  "<< "F: " << funcMin << "  Gmin: " << Gmin 
//					<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//					TempPos.Display();
//					cout << endl;
	m_TestPoint[I].m_Position.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
	m_TestPoint[I].m_PosSource = Brutal;
	errorEstimate = max(TA/2,DegRes*PI*m_Distance/180);
	return errorEstimate;	
}

//*******************************************************************
double cPosSet::CorrCoefOpt(PGconn* DBase, int I, cGeoP Begin)
{
	// Implement Steepest -Decent to get positions and 
	// Lee-type propagation loss constants
	// Minimise correlation coefficient
	unsigned i=0;
	double K=32, G=3.2;
	double errorEstimate = 70000;
	double Al3;
	int Fchoice = 0;
	int countA =0;
	double minA, DTheta, dPos;
	double taper = 2*TA;
	unsigned STOP = 5000;
	double funcDT1, funcDR1, funcDG1, funcDK1, funcDT2, funcDR2, funcDG2, funcDK2;
	double func, funcA3, funcA2, funcA0, funcMin; 
	func=70000;
	double dK, dG, dT=1, dR=TA/4, sizeZ, Al2, h1, h2, h3, Al0;
	double previousDelta;
	cGeoP TempPos = Begin;
	cGeoP Previous = m_ServCell.m_Ligging;
	cGeoP PosD = Begin;
	m_TestPoint[I].m_PosSource = CorrCoef;

	double TotWeight = 0.0;
	for (i=0; i<m_NumUsed; i++)
		TotWeight+=m_Scale[i];
	if (TotWeight <1.05)
	{
//		cout << "too little to work on. " << endl;
		m_TestPoint[I].m_PosSource = CorrCoef;
		return 70000;	
	}
	while ((func>0.0001)&&((errorEstimate>0.0)||(i<50))&&(i<STOP))
	{
		if (G<2) 	
			G=2;
		else if (G>6.5)	
			G=6.5; 
		if (K<30) 
			K=30;
		else if (K>300) 
			K=300;
		if (m_Distance > (m_TestPoint[I].m_TA+2.0)*TA)
			TempPos.FromHere(m_ServCell.m_Ligging,(m_TestPoint[I].m_TA+2.0)*TA, m_Azimuth);
		else if (m_Distance < (m_TestPoint[I].m_TA-1.0)*TA)
			TempPos.FromHere(m_ServCell.m_Ligging,(m_TestPoint[I].m_TA-1.0)*TA, m_Azimuth);
		
		m_Elev = 180*atan2(20.0,m_Distance)/PI;
		func = funcSwitch(Fchoice,TempPos,K,G);
		funcDK1 = funcSwitch(Fchoice,TempPos,K+1,G);
		funcDG1 = funcSwitch(Fchoice,TempPos,K,G+0.05);
		funcDK2 = funcSwitch(Fchoice,TempPos,K-1,G);
		funcDG2 = funcSwitch(Fchoice,TempPos,K,G-0.05);
		dK = (funcDK1-funcDK2)/2;
		dG = (funcDG1-funcDG2)/0.1;
		
		DTheta=MIN(5,MAX(180*TA/10/m_Distance,0.5));
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth-DTheta);
		funcDT1 = funcSwitch(Fchoice,PosD,K,G);
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth+DTheta);
		funcDT2 = funcSwitch(Fchoice,PosD,K,G);
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance-TA/15, m_Azimuth);
		funcDR1 = funcSwitch(Fchoice,PosD,K,G);
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance+TA/15, m_Azimuth);
		funcDR2 = funcSwitch(Fchoice,PosD,K,G);
		dT = (funcDT2-funcDT1)/2/DTheta;
		dR = (funcDR2-funcDR1)*15/TA/2;
		
		sizeZ = sqrt(dK*dK+dG*dG+dT*dT+dR*dR);
		dK = dK/sizeZ;
		dG = dG/sizeZ;
		dT = dT/sizeZ;
		dR = dR/sizeZ;
		dPos= sqrt(dR*dR+dT*dT/m_Distance/m_Distance);

		Al3 = max(fabs(taper),max(fabs(func/sizeZ),TA/dPos));
		if (fabs(Al3*dR)>TA/2)
			Al3 = TA/dR/2;
		double D=MIN(30,MAX(180*TA/m_Distance,5));
		if (fabs(Al3*dT)>D);
			Al3=D/fabs(dT);
		D = min(fabs(G-2),fabs(7-G));
		if ((Al3*fabs(dG)>D)&&(D>0))
			Al3 = D/fabs(dG);
		D=min(fabs(K-10),fabs(300-K));
		if ((Al3*fabs(dK)>D)&&(D>0))
			Al3 = D/fabs(dK);
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al3*dR, m_Azimuth-Al3*dT);
		funcA3 = funcSwitch(Fchoice,PosD,K-Al3*dK,G-Al3*dG);
		Al2 = Al3/2.0;
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al2*dR, m_Azimuth-Al2*dT);
		funcA2 = funcSwitch(Fchoice,PosD,K-Al2*dK,G-Al2*dG);
		if (funcA2>funcA3)
		{
			minA=Al3;
			funcMin= funcA3;
		}
		else 
		{
			minA=Al2;
			funcMin= funcA2;
		}
		countA=0;
		while ((((funcA3>func)&&(Al3>1e-4))||(funcA2>funcA3))&&(countA<100))
		{		
			countA++;
			if (funcA3>func)
				Al3/=2.0;
			else if (funcA2>funcA3)
			{
				if (funcA3<funcMin)
				{
					funcMin = funcA3;
					minA = Al3;
				}
				Al3=Al3*2.1;
				if (fabs(Al3*dR)>TA/2)
					Al3 = TA/dR/2;
				D=MIN(30,MAX(180*TA/m_Distance,5));
				if (fabs(Al3*dT)>D);
					Al3=D/fabs(dT);
				D = min(fabs(G-2),fabs(7-G));
				if ((Al3*fabs(dG)>D)&&(D>0))
					Al3 = D/fabs(dG);
				D=min(fabs(K-10),fabs(300-K));
				if ((Al3*fabs(dK)>D)&&(D>0))
					Al3 = D/fabs(dK);
			}
			else if (funcA2<funcMin)
			{
				funcMin = funcA2;
				minA = Al2;
			}
			PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al3*dR, m_Azimuth-Al3*dT);
			funcA3 = funcSwitch(Fchoice,PosD,K-Al3*dK,G-Al3*dG);
			Al2 = Al3/2.0;
			PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al2*dR, m_Azimuth-Al2*dT);
			funcA2 = funcSwitch(Fchoice,PosD,K-Al2*dK,G-Al2*dG);
		}
		if (countA==100)
		{
			if (funcMin<func)
			{
				Al2=minA;
				funcA2=funcMin;
				Al3=2*minA;
				PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al3*dR, m_Azimuth-Al3*dT);
				funcA3 = funcSwitch(Fchoice,PosD,K-Al3*dK,G-Al3*dG);
			}
			else
			{
//				cout << " Al0 not found at " << i <<endl;
				m_TestPoint[I].m_PosSource = CorrCoef;
				m_TestPoint[I].m_Position = TempPos;
//				cout << "F: " << func << "  G: " << G << "  K: "<< K 
//								<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//								TempPos.Display();
//								cout << endl;
				return errorEstimate;
			}
		}
		h1 = (funcA2-func)/Al2;
		h2 = (funcA3-funcA2)/(Al3-Al2);
		h3 = (h2-h1)/Al3;
		Al0 = (Al2-h1/h3)/2.0;
		PosD.FromHere(m_ServCell.m_Ligging, m_Distance-Al0*dR, m_Azimuth-Al0*dT);
		funcA3 = funcSwitch(Fchoice,PosD,K-Al0*dK,G-Al0*dG);		
		if ((funcA0<funcA3)&&(funcA0<func))
			taper = Al0;
		else if (funcA3<func)
			taper = Al3;
		else if (funcMin<func)
			taper=minA;
		else if (funcA2<func)
			taper = Al2;
		else 
		{					
			if (funcA0<funcA3)
				taper = Al0;
			else taper=Al3;
//			cout << " Diverging at " << i <<endl;
			m_TestPoint[I].m_PosSource = CorrCoef;
			m_TestPoint[I].m_Position = TempPos;
//			cout << "F: " << func << "  G: " << G << "  K: "<< K 
//					<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//					TempPos.Display();
//					cout << endl;
			return errorEstimate;
		}
		K-=taper*dK;
		G-=taper*dG;
		m_Distance-=taper*dR;
		m_Azimuth-=taper*dT;
		TempPos.FromHere(m_ServCell.m_Ligging, m_Distance, m_Azimuth);
		
/*		cout << "F: " << func << "  G: " << G << "  K: "<< K 
				<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
				TempPos.Display();
				cout << endl;
*/
		errorEstimate = fabs(TempPos.Distance(Previous));
		Previous = TempPos;
		previousDelta = errorEstimate;
		i++;
	}
//	if ((G<2)||(G>10)||(K<20)||(K>250)) errorEstimate=70000;
//	cout << "F: " << func << "  G: " << G << "  K: "<< K 
//					<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//					TempPos.Display();
//					cout << endl;
//	cout << " is STOP?: " << i << endl;
	m_TestPoint[I].m_PosSource = CorrCoef;
	m_TestPoint[I].m_Position = TempPos;
	return errorEstimate;	
}


//*******************************************************************************
double cPosSet::funcSwitch(int Switch, cGeoP Pos, double K, double G)
{
	switch (Switch)
	{
		case 0:
			return OneMinCorr(Pos, K, G);
		case 1:
			return squareErr(Pos, K, G);
		case 2:
			return CosMeasPred(Pos, K, G);
		case 3:
		{
			double dEdLat,dEdLon;
			return EdEdXdY(Pos, K, G, dEdLat, dEdLon);
		}
		case 4:
		{
			double dEdLat,dEdLon;
			EdEdXdY(Pos, K, G, dEdLat, dEdLon);
			return 100000*sqrt(dEdLat*dEdLat+dEdLon*dEdLon);
		}
		default:
			return OneMinCorr(Pos, K, G);
	}
}

//*******************************************************************************
double cPosSet::OneMinCorr(cGeoP Pos, double K, double G)
{
	double R, Theta, func=2,  Elev, antValue;
	int i, NumUsed=0;
	double avePred=0, varPred=0, varCo=0, aveMeas=0, varMeas=0;
	for (i=0;i<m_NumUsed;i++)
	{
		if (m_Scale[i]>0)
		{
			Theta = m_PosUsed[i].Bearing(Pos);
			R = Pos.Distance(m_PosUsed[i]);
			Elev = 180*atan2(20.0,R)/PI;
			antValue = m_AntUsed[i].GetPatternValue(Theta, Elev);
			if (R<0.01)
			{
//				Pos.Display();
//				cout << "now 10m"<< endl;
				R=10;
			}
			m_predUsed[i] = m_EIRPused[i]-antValue- K -G*10.0*log10(R);
			avePred+= m_predUsed[i];
			aveMeas+= m_MeasUsed[i];
			NumUsed++;
		}
	}
//		cout << "R: " << R<< "   pred: "<< m_predUsed[i] << "   meas: " << m_MeasUsed[i] << " Theta: " << Theta << endl;
	
	avePred/= double(NumUsed);
	aveMeas/= double(NumUsed);
	for (i=0;i<m_NumUsed;i++)
	{
		if (m_Scale[i]>0)
		{
			varMeas+=(m_MeasUsed[i]-aveMeas)*(m_MeasUsed[i]-aveMeas);
			varPred+=(m_predUsed[i]-avePred)*(m_predUsed[i]-avePred);
			varCo+=m_Scale[i]*(m_predUsed[i]-avePred)*(m_MeasUsed[i]-aveMeas);
		}
	}
	if (varPred>0.0)
		func = 1.0 - varCo/sqrt(varMeas*varPred);
	else func = 2.0;
//	cout << "F: " << func << endl;
	return func;
}

//*******************************************************************************
double cPosSet::CosMeasPred(cGeoP Pos, double K, double G)
{
	double R, Theta, func, prod=0, meas2=0, pred2=0, Elev, antValue;
	int i;
	for (i=0;i<m_NumUsed;i++)
	{
		Theta = m_PosUsed[i].Bearing(Pos);
		R = Pos.Distance(m_PosUsed[i]);
		Elev = 180*atan2(20.0,R)/PI;
		antValue = m_AntUsed[i].GetPatternValue(Theta, Elev);
		m_predUsed[i] = m_EIRPused[i]-antValue- K -G*10.0*log10(R);
		prod += m_Scale[i]*m_predUsed[i]*m_MeasUsed[i];
		meas2 += m_Scale[i]*m_MeasUsed[i]*m_MeasUsed[i];
		pred2 += m_Scale[i]*m_predUsed[i]*m_predUsed[i];

//		cout << "R: " << R<< "   pred: "<< m_predUsed[i] << "   meas: " << m_MeasUsed[i] << endl;
	}
	func = 1.0-prod/sqrt(meas2*pred2);
	return func;
}

//*******************************************************************************
double cPosSet::squareErr(cGeoP Pos, double K, double G)
{
	double R, Theta, func, squareErr,Elev,antValue;
	int i, NumUsed=0;
	squareErr = 0.0;
	for (i=0;i<m_NumUsed;i++)
	{
		Theta = m_PosUsed[i].Bearing(Pos);
		R = Pos.Distance(m_PosUsed[i]);
		Elev = 180*atan2(20.0,R)/PI;
		antValue=m_AntUsed[i].GetPatternValue(Theta, Elev);
		NumUsed++;
		m_predUsed[i] = m_EIRPused[i]-antValue- K -G*10.0*log10(R);
		squareErr+=m_Scale[i]*(m_MeasUsed[i]-m_predUsed[i])*(m_MeasUsed[i]-m_predUsed[i]);
//		cout << "R: " << R<< "   pred: "<< m_predUsed[i] << "   meas: " << m_MeasUsed[i] << endl;
	}
	squareErr/=NumUsed;
	func = squareErr;
	return func; 
}

//*******************************************************************
double cPosSet::DistanceDiff(PGconn* DBase, int I, cGeoP Begin)
{
	double errorEstimate=70000.0;
	double G=3.2,K=32;
	unsigned i=0;
	double taper = 1;
	int countA=0;
	unsigned STOP = 5000;
	double funcDG1, funcDK1, funcDG2, funcDK2;
	double func, funcA3, funcA2, funcA0, funcMin; 
	double dEdLat=70000, dEdLon=70000, minA; 
	double Lon, Lat, dK, dG, sizeZ, Al2, h1, h2, h3, Al0, Al3, dPos;
	double dumLat, dumLon;
	double previousDelta;
	double TotWeight = 0.0;
	m_TestPoint[I].m_PosSource = DistErr;
	for (i=0; i<m_NumUsed; i++)
		TotWeight+=m_Scale[i];
	if (TotWeight <1.05)
	{
//		cout << "too little to work on. " << endl;
		m_TestPoint[I].m_PosSource = DistErr;
		return 70000;	
	}
	cGeoP TempPos = Begin;
	cGeoP Previous = m_ServCell.m_Ligging;
	cGeoP PosD = Begin;
	Al3 = TA;
	while ((((dPos>0.00001)&&(errorEstimate>0.0))||(i<50))&&(i<STOP))
	{
		if (K<10) 
			K=10;
		else if (K>300) 
			K=300;
		if (G<2) 	
			G=2;
		else if (G>6.5)	
			G=6.5; 
		if (m_Distance > (m_TestPoint[I].m_TA+2.0)*TA)
			TempPos.FromHere(m_ServCell.m_Ligging,(m_TestPoint[I].m_TA+2.0)*TA, m_Azimuth);
		else if (m_Distance < (m_TestPoint[I].m_TA-1.0)*TA)
			TempPos.FromHere(m_ServCell.m_Ligging,(m_TestPoint[I].m_TA-1.0)*TA, m_Azimuth);
		m_Elev = 180*atan2(20.0,m_Distance)/PI;
		funcDK1 = EdEdXdY(TempPos,K+1,G, dEdLat, dEdLon);
		funcDG1 = EdEdXdY(TempPos,K,G +0.05, dEdLat, dEdLon);
		funcDK2 = EdEdXdY(TempPos,K-1,G, dEdLat, dEdLon);
		funcDG2 = EdEdXdY(TempPos,K,G -0.05, dEdLat, dEdLon);
		func = EdEdXdY(TempPos, K, G, dEdLat, dEdLon);
		dK = (funcDK1-funcDK2)/2.0;
		dG = (funcDG1-funcDG2)/0.1;

		sizeZ = sqrt(dK*dK+dG*dG+dEdLat*dEdLat+dEdLon*dEdLon);
		dK = dK/sizeZ;
		dG = dG/sizeZ;
		dEdLat = dEdLat/sizeZ;
		dEdLon = dEdLon/sizeZ;
		TempPos.Get(Lat,Lon);
		PosD.Set(Lat-dEdLat,Lon-dEdLon);
		dPos = PosD.Distance(TempPos);

		Al3 = max(fabs(taper),max(fabs(func/sizeZ),TA/dPos/2));
		if (fabs(Al3*dPos)>TA/2)
			Al3 = TA/dPos/2;
		double D = min(fabs(G-2),fabs(7-G));
		if ((Al3*fabs(dG)>D)&&(D>0))
			Al3 = D/fabs(dG);
		D=min(fabs(K-10),fabs(300-K));
		if ((Al3*fabs(dK)>D)&&(D>0))
			Al3 = D/fabs(dK);
		PosD.Set(Lat-Al3*dEdLat,Lon-Al3*dEdLon);
		funcA3 = EdEdXdY(PosD, K-Al3*dK, G-Al3*dG, dumLat, dumLon);
		Al2 = Al3/2.0;
		PosD.Set(Lat-Al2*dEdLat,Lon-Al2*dEdLon);
		funcA2 = EdEdXdY(PosD, K-Al2*dK, G-Al2*dG, dumLat, dumLon);
		if (funcA2>funcA3)
		{
			minA=Al3;
			funcMin= funcA3;
		}
		else 
		{
			minA=Al2;
			funcMin= funcA2;
		}
		countA=0;
		while ((((funcA3>=func)&&(Al3>1e-10))||(funcA2>funcA3))&&(countA<200))
		{		
			countA++;
			if (funcA3>=func)
				Al3/=2.0;
			else if ((funcA2>funcA3)&&(funcA2<func))
			{
				if (funcA3<funcMin)
				{
					funcMin = funcA3;
					minA = Al3;
				}
				Al3=Al3*2.1;
				if (fabs(Al3*dPos)>TA)
					Al3 = TA/dPos;
				D = min(fabs(G-2),fabs(7-G));
				if ((Al3*fabs(dG)>D)&&(D>0))
					Al3 = D/fabs(dG);
				D=min(fabs(K-10),fabs(300-K));
				if ((Al3*fabs(dK)>D)&&(D>0))
					Al3 = D/fabs(dK);
			}
			else if (funcA2<funcMin)
			{
				funcMin = funcA2;
				minA = Al2;
			}
			PosD.Set(Lat-Al3*dEdLat,Lon-Al3*dEdLon);
			funcA3 = EdEdXdY(PosD, K-Al3*dK, G-Al3*dG, dumLat, dumLon);
			Al2 = Al3/2.0;
			PosD.Set(Lat-Al2*dEdLat,Lon-Al2*dEdLon);
			funcA2 = EdEdXdY(PosD, K-Al2*dK, G-Al2*dG, dumLat, dumLon);
		}
		if (countA==200)
		{
			if (funcMin<func)
			{
				Al2=minA;
				funcA2=funcMin;
				Al3=2*minA;
				PosD.Set(Lat-Al3*dEdLat,Lon-Al3*dEdLon);
				funcA3 = EdEdXdY(PosD, K-Al3*dK, G-Al3*dG, dumLat, dumLon);	
			}
			else 
			{
//				cout << " Al0 not found at " << i <<endl;
				m_TestPoint[I].m_PosSource = DistErr;
				m_TestPoint[I].m_Position = TempPos;
//				cout << "F: " << func << "  G: " << G << "  K: "<< K 
//					<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//					TempPos.Display();
//					cout << endl;
				return errorEstimate;
			}
		}
				
		h1 = (funcA2-func)/Al2;
		h2 = (funcA3-funcA2)/(Al3-Al2);
		h3 = (h2-h1)/Al3;
		Al0 = (Al2-h1/h3)/2.0;
		PosD.Set(Lat-Al0*dEdLat,Lon-Al0*dEdLon);
		funcA0 = EdEdXdY(PosD, K-Al0*dK, G-Al0*dG, dumLat, dumLon);	
		
		if ((funcA0<funcA3)&&(funcA0<func))
			taper = Al0;
		else if (funcA3<func)
			taper = Al3;
		else if (funcMin<func)
			taper=minA;
		else if (funcA2<func)
			taper = Al2;
		else 
		{	
			if (funcA0<funcA3)
				taper = Al0;
			else taper=Al3;
//			cout << " Diverging at " << i <<endl;
			m_TestPoint[I].m_PosSource = DistErr;
			m_TestPoint[I].m_Position = TempPos;
//			cout << "F: " << func << "  G: " << G << "  K: "<< K 
//				<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "  ";
//					TempPos.Display();
//					cout << endl;
			return errorEstimate;
		}
		K-=taper*dK;
		G-=taper*dG;
		TempPos.Set(Lat-taper*dEdLat,Lon-taper*dEdLon);
	
		m_Distance = TempPos.Distance(m_ServCell.m_Ligging);
		m_Azimuth = m_ServCell.m_Ligging.Bearing(TempPos);
//		cout << "F: " << func << "  G: " << G << "  K: "<< K 
//				<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "   ";
//		TempPos.Display();
//		cout << endl;
	
		errorEstimate = fabs(TempPos.Distance(Previous));
		Previous = TempPos;
		previousDelta = errorEstimate;
		i++;
	}
//	cout << " is STOP?: " << i << endl;
//	cout << "F: " << func << "  G: " << G << "  K: "<< K 
//				<< "  Dist: " << m_Distance << "  Theta: " << m_Azimuth << "   ";
//	TempPos.Display();
//	cout << endl;
//	if ((G<2)||(G>7)||(K<20)||(K>250)) errorEstimate=70000;
	
	m_TestPoint[I].m_PosSource = DistErr;
	m_TestPoint[I].m_Position = TempPos;
//	if (i==STOP) return 70000;
	return errorEstimate;	
}

//*******************************************************************************
double cPosSet::EdEdXdY(cGeoP Pos, double K, double G, double &dEdLat, double &dEdLon)
{
	double R, Theta, func=0.0, d, alpha, Lat, Lon, SLat, SLon, Elev,antValue;
	double TotWeight=0;
	dEdLat=0.0;
	dEdLon=0.0;
	int i;
	for (i=0;i<m_NumUsed;i++)
	{
		Theta = m_PosUsed[i].Bearing(Pos);
		R = Pos.Distance(m_PosUsed[i]);
		Elev = 180*atan2(20.0,R)/PI;
		antValue=m_AntUsed[i].GetPatternValue(Theta, Elev);
		if (m_Scale[i]>0.0)
		{
			TotWeight+=m_Scale[i];
			Pos.Get(Lat,Lon);
			m_PosUsed[i].Get(SLat, SLon);
			alpha = (m_EIRPused[i]-antValue	- m_MeasUsed[i] - K)/(10.0*G);
			d = pow(10.0,alpha);
			dEdLat+=m_Scale[i]*2.0*(Lat-SLat)*(1.0-d/R);
			dEdLon+=m_Scale[i]*2.0*(Lon-SLon)*(1.0-d/R);
			func += m_Scale[i]*(R-d)*(R-d);
		}
/*		cout << "   G: "<< G << "   K: "<< K << "    R: " << R<< "   d: "<< d 
			<< "  Meas: " << m_MeasUsed[i] 
			<< "   Ptx: " << m_EIRPused[i]-m_AntUsed[i].GetPatternValue(Theta, m_Elev)<< endl;
*/	}
//	cout << endl;
	dEdLat/=TotWeight;
	dEdLon/=TotWeight;
	return sqrt(func)/TotWeight;
}

//*****************************************************************************
int cPosSet::Initialise(PGconn* DBase, cGeoP Begin)
{
	unsigned i, k, NumNCells;
	short int *Tech;
	short int TechChoice = 0;
	unsigned C900=0, C1800=0, CUMTS=0, C50=0;
	bool *UsedCell;
	NumNCells=m_NB_Cells.size();
	if (NumNCells==0)
		return 0;
	UsedCell = new bool[NumNCells];
	Tech = new short int[NumNCells];
	double f=880;
	for (i=0; i< NumNCells; i++)
	{
		UsedCell[i]=FALSE;
		Tech[i]=0;
	}
	m_NumUsed = 0;
	for (i=0; i< NumNCells; i++)
	{
		UsedCell[i]=(m_NB_Meas[i].m_MeasValue<-46)&&(m_NB_Meas[i].m_MeasValue>-111);
		if (UsedCell[i])
		{
			m_NumUsed++;
			if (m_NB_Cells[i].m_ARFCN < 252)
			{
				C900++;
				Tech[i]=0;
			}
			else if (m_NB_Cells[i].m_ARFCN < 512)
			{
				C50++;
				Tech[i]=2;
			}
			else if (m_NB_Cells[i].m_ARFCN < 886)
			{
				C1800++;
				Tech[i]=1;
			}
			else if (m_NB_Cells[i].m_ARFCN < 1024)
			{
				C900++;
				Tech[i]=0;
			}
			else
			{
				CUMTS++;
				Tech[i]=3;
			}
		}
	}
	if ((C900>C1800)&&(C900>C50)&&(C900>CUMTS))
	{
		TechChoice=0;
		f=880;
	}
	else if ((C1800>C900)&&(C1800>C50)&&(C1800>CUMTS))
	{
		TechChoice=1;
		f=1750;
	}
	else
	{
		TechChoice =3;
		f=1900;
	}
	for (i=0; i< NumNCells; i++)
	{
		if (Tech[i]!=TechChoice)
		{
			UsedCell[i]=FALSE;
//			cout << "CI: " << m_NB_Cells[i].m_CI 
//				<< "   BCCH: " <<m_NB_Cells[i].m_ARFCN 
//				<< "  Tech: "<< Tech[i] << " vs. " << TechChoice<< endl;
			m_NumUsed--;
		}
	}

	for (i=0; (i<NumNCells)&&(m_NumUsed<4); i++ )
	{
		if ((UsedCell[i]==FALSE)&&(Tech[i]==TechChoice))
		{
			UsedCell[i]=TRUE;
			m_NumUsed++;
		}
	}
	
	
	double R, RxMax, Delta;
	for (i=0; i<NumNCells; i++ )	
	{
		if ((UsedCell[i])&&(m_NB_Cells[i].m_Site!=m_ServCell.m_Site))
		{
			R = (m_NB_Cells[i].m_Ligging.Distance(m_ServCell.m_Ligging)-(m_TestPoint[0].m_TA+1.0)*TA)/1000;
			RxMax = m_NB_Cells[i].m_EIRP
					- min(32.44+20.0*log10(f)+20.0*log10(R), 120.0-20.0*log10(20.0*1.5)+40.0*log10(R));
			Delta =  RxMax-m_NB_Meas[i].m_MeasValue;
			if (Delta<0.0)
			{
				m_NumUsed--;
				UsedCell[i]=FALSE;
			}
		}
	}
	m_aveMeas/=m_NumUsed;
	
	delete [] m_IndexUsed;
	m_IndexUsed = new short int[m_NumUsed];
	delete [] m_AntUsed;
	m_AntUsed = new CAntennaPattern[m_NumUsed]; 
	delete [] m_PosUsed;
	m_PosUsed = new cGeoP[m_NumUsed];
	delete [] m_predUsed; 
	m_predUsed = new double[m_NumUsed];
	delete [] m_EIRPused;
	m_EIRPused = new double[m_NumUsed];
	delete [] m_MeasUsed;
	m_MeasUsed = new double[m_NumUsed];

	i=0;
	m_aveMeas = 0.0;
	for (k=0;k<NumNCells;k++)
	{
		if (UsedCell[k])
		{	
//			cout << " CI: "<< m_NB_Cells[k].m_CI
//								<< " Key: " << m_NB_Cells[k].m_AntKey 
//								<< " Azi: " << m_NB_Cells[k].m_Azimuth
//								<< " Tilt: "<< m_NB_Cells[k].m_MechTilt << endl;
			m_IndexUsed[i]=k;
			m_MeasUsed[i] =m_NB_Meas[k].m_MeasValue;
			m_aveMeas+=m_MeasUsed[i];
			m_PosUsed[i] = m_NB_Cells[k].m_Ligging;
			m_EIRPused[i] = m_NB_Cells[k].m_EIRP;
			m_AntUsed[i].SetAntennaPattern(DBase, m_NB_Cells[k].m_AntKey, 
												m_NB_Cells[k].m_Azimuth, 
												m_NB_Cells[k].m_MechTilt);

			i++;
		}
	}	
	
	m_Distance = (0.75+m_TestPoint[0].m_TA)*TA;
	m_Azimuth = m_ServCell.m_Ligging.Bearing(Begin);
	double DegBegin = m_ServCell.m_Azimuth - m_ServCell.m_AziBeam; 
	double DegEnd = m_ServCell.m_Azimuth + m_ServCell.m_AziBeam;
	if ((DegEnd-DegBegin) >360)	
	{ 
		DegBegin=0; 
		DegEnd=360;
	}
	
	cGeoP BeginPos, EndPos;
	BeginPos.FromHere(m_ServCell.m_Ligging,m_Distance,DegBegin);
	EndPos.FromHere(m_ServCell.m_Ligging,m_Distance,DegEnd);
	double antValue, antValueB, antValueE, Theta, Elev=0;
	double TotWeight =0;
	double LimitU = MAX(m_MeasUsed[0]-m_EIRPused[0]-20,-150);
	double LimitL = MAX(m_MeasUsed[0]-m_EIRPused[0]-30,-158);
	delete [] m_Scale;
	m_Scale=new double[m_NumUsed];
	for (i=0;i<m_NumUsed; i++)
	{
		Theta = m_PosUsed[i].Bearing(Begin);
		antValue = m_AntUsed[i].GetPatternValue(Theta, Elev);
		Theta = m_PosUsed[i].Bearing(BeginPos);
		antValueB = m_AntUsed[i].GetPatternValue(Theta, Elev);
		Theta = m_PosUsed[i].Bearing(EndPos);
		antValueE = m_AntUsed[i].GetPatternValue(Theta, Elev);
		if (((antValue<16)&&(antValueB<14)&&(antValueE<14)
				&&((m_MeasUsed[i]-m_EIRPused[i]) >=LimitU))/*||
				(m_NB_Cells[i].m_Site==m_ServCell.m_Site)*/)
			m_Scale[i]=1;
		else
			m_Scale[i]=1-MAX(0,MAX(MAX(antValue,MAX(antValueB,antValueE))-14/8,
							(LimitU-m_MeasUsed[i]+m_EIRPused[i])/(LimitU-LimitL))); 
		if (m_Scale[i]<0) m_Scale[i] = 0;
		TotWeight+=m_Scale[i];
	}
	m_Scale[0]=1;

//	for (i=0;i<m_NumUsed;i++)	
//		cout << "  Meas: " << m_MeasUsed[i] << "   Scale: " << m_Scale[i] << endl;
	delete [] UsedCell;
	delete [] Tech;
	return m_NumUsed;
}
