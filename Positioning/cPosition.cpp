#include "cPosition.h"
const double TA = 553.537;
//********************************************************************
cPosition::cPosition()
{
	m_lastTP=0;
	m_NumOriginalMMR=0;
}

//********************************************************************
cPosition::~cPosition()
{
	m_NBMeas.clear();
	m_coreMMR.clear();
	m_TempList.clear();
}

//********************************************************************
int cPosition::GetCurrentData(PGconn* DBase)
{
	CellList TempCell;
	double TempCellLat, TempCellLon;
	double TempPointLat, TempPointLon;
	struct tm tmTime;
	cMeas TempMeas;
	cTPoint TempPoint;
	char* QueryResult;
	int CellLastKey=0;
	int Year, ss, pp;
	const char* Query;
	PGresult* res;
	unsigned NumBytes,i,ii,j,rows,TempI;
	char Temp[33];
	char	   *pghost,
			   *pgport,
			   *pgoptions,
			   *pgtty;
 
    pghost = NULL;				// host name of the backend server 
	pgport = NULL;				// port of the backend server
	pgoptions = NULL;			// special options to start up the backend server
	pgtty = NULL;				// debugging tty for the backend server
	CellLastKey = 0;
	double sign = 1.0;
	
	string LACList = "(0,";
	for (i=0; ((i<m_LACList.size()-1)&&(m_LACList[i]<65536)&&(m_LACList[i]>=0)); i++)
	{
		gcvt(m_LACList[i], 8, Temp);
		LACList += Temp;
		LACList += ",";	
	}
	gcvt(m_LACList[i], 8, Temp);
	LACList += Temp;
	LACList += ") ";
//	cout << LACList << endl;
	
	string CmdStr = "SELECT MeasKey, max(Measurements.CI), max(TP), max(Azimuth), max(Tilt), max(Distance)," ;
	CmdStr += "max(BTL), max(MeasValue), max(Predict) ";
	CmdStr += "FROM Measurements cross join TestPoint cross join CellData "; 
	CmdStr += "where Measurements.TP=TestPoint.TPrimKey ";
	CmdStr += "and TestPoint.MeasServCI = CellData.CI and LAC in ";
	CmdStr += LACList;
	CmdStr += "group by CellData.CI, TP, MeasKey ";
	CmdStr += "order by CellData.CI, TP, MeasKey;";
	
	Query = CmdStr.c_str();
	res = PQexec(DBase, Query);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			rows = PQntuples(res);
			if (rows)
			{
				for (j=0; j<rows ;j++)
				{
					// Measurement primary key
					QueryResult = PQgetvalue(res,j,0);
					NumBytes = PQgetlength(res,j,0);
					TempMeas.m_MeasKey =0;
					for (i=0;i<NumBytes;i++)
					 	TempMeas.m_MeasKey =  TempMeas.m_MeasKey*10 + (QueryResult[i]-'0');
					// Measured Cell (foreign key)
					QueryResult = PQgetvalue(res,j,1);
					NumBytes = PQgetlength(res,j,1);
					TempMeas.m_CI = 0;
					for (i=0;i<NumBytes;i++)
					 	TempMeas.m_CI =  TempMeas.m_CI*10 + (QueryResult[i]-'0');
					// Testpoint foreign key
					QueryResult = PQgetvalue(res,j,2);
					NumBytes = PQgetlength(res,j,2);
					TempMeas.m_TestPKey = 0;
					for (i=0;i<NumBytes;i++)
					 	TempMeas.m_TestPKey =  TempMeas.m_TestPKey*10 + (QueryResult[i]-'0'); 
					// Azimuth from measured cell
					QueryResult = PQgetvalue(res,j,3);
					NumBytes = PQgetlength(res,j,3);
					TempMeas.m_Azimuth = CharP_to_Double(QueryResult,NumBytes);
					// Tilt from measured cell
					QueryResult = PQgetvalue(res,j,4);
					NumBytes = PQgetlength(res,j,4);
					TempMeas.m_Tilt = CharP_to_Double(QueryResult,NumBytes);
					// Distance from measured cell
					QueryResult = PQgetvalue(res,j,5);
					NumBytes = PQgetlength(res,j,5);
					TempMeas.m_Distance = CharP_to_Double(QueryResult,NumBytes);
    				// Basic Transmission Loss from measured cell
					QueryResult = PQgetvalue(res,j,6);
					NumBytes = PQgetlength(res,j,6);
					TempMeas.m_BTL = CharP_to_Double(QueryResult,NumBytes);
					// Received signal value
					QueryResult = PQgetvalue(res,j,7);
					NumBytes = PQgetlength(res,j,7);
					TempMeas.m_MeasValue = CharP_to_Double(QueryResult,NumBytes);
					// Predicted signal strength
					QueryResult = PQgetvalue(res,j,8);
					NumBytes = PQgetlength(res,j,8);
					TempMeas.m_Predict = CharP_to_Double(QueryResult,NumBytes);
					if ((TempMeas.m_MeasValue<-46)&&(TempMeas.m_MeasValue>-111))
							m_NBMeas.push_back(TempMeas);
//    				cout <<"j:" <<  j<< "Key: "<<TempMeas.m_MeasKey << " CI: "<< TempMeas.m_CI << endl;
					TempMeas.clear();
				}	
			}
			PQclear(res);
		}
		else
		{
			cout << "Problem with command: " << Query << endl;
			PQclear(res);
			PQreset(DBase);
			return 0;
		}
	}
	else
	{
		PQclear(res);
		PQreset(DBase);
		return 0;
	}
	
	CmdStr = "SELECT distinct TPrimKey, TPDerived, MeasServCI, m_NBS, pos[0], pos[1], PosSource, height, high, ";
	CmdStr += "TA, TApred, MSTxPower, BSTxpower, measBCCH, ";
	CmdStr += "RxLevFullDn, RxLevSubDn, RxLevFullUp, RxLevSubUp, DateTime "; 
	CmdStr += "FROM TestPoint cross join CellData "; 
	CmdStr += "where TestPoint.MeasServCI = CellData.CI and LAC in ";
	CmdStr += LACList;
	CmdStr += "order by MeasServCI, TPrimKey;";
	Query = CmdStr.c_str();
	res = PQexec(DBase, Query);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			rows = PQntuples(res);
			if (rows)
			{
				for (j=0; j<rows ;j++)
				{
					// Testpoint / MMR primary key
					QueryResult = PQgetvalue(res,j,0);
					NumBytes = PQgetlength(res,j,0);
					TempPoint.m_TPKey =0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_TPKey =  TempPoint.m_TPKey*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,1);
					NumBytes = PQgetlength(res,j,1);
					TempPoint.m_DerivedTPKey =0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_DerivedTPKey = TempPoint.m_DerivedTPKey*10 
					 									+ (QueryResult[i]-'0');
					// Serving Cell (foreign Key)
					QueryResult = PQgetvalue(res,j,2);
					NumBytes = PQgetlength(res,j,2);
					TempPoint.m_ServCI = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_ServCI = TempPoint.m_ServCI*10 + (QueryResult[i]-'0');
					// Number of measurements on point
					QueryResult = PQgetvalue(res,j,3);
					NumBytes = PQgetlength(res,j,3);
					TempPoint.m_NBS = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_NBS  =  TempPoint.m_NBS*10 + (QueryResult[i]-'0'); 
					// Position of Testpoint
					QueryResult = PQgetvalue(res,j,4);
					NumBytes = PQgetlength(res,j,4);
    				TempPointLat = 0.0;
					pp = 0;
					ss = 0;
					ii = 0;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign = 1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=',');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempPointLat = TempPointLat*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempPointLat/=sign*exp10(ss-pp);
					ii = i+1;
					QueryResult = PQgetvalue(res,j,5);
					NumBytes = PQgetlength(res,j,5);
					TempPointLon = 0.0;
    				pp = 0;
					ss = 0;
					ii = 0;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign =1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=')');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempPointLon = TempPointLon*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempPointLon/=sign*exp10(ss-pp);
					TempPoint.m_Position.Set(TempPointLat, TempPointLon);
					// Source of position .. enum
					QueryResult = PQgetvalue(res,j,6);
					NumBytes = PQgetlength(res,j,6);
					TempI = 0;
					for (i=0;i<NumBytes;i++)
					 	TempI = TempI*10 + (QueryResult[i]-'0');
					TempPoint.m_PosSource=PosMethods(TempI);
					// Height
					QueryResult = PQgetvalue(res,j,7);
					NumBytes = PQgetlength(res,j,7);
					TempPoint.m_Height = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_Height = TempPoint.m_Height*10 + (QueryResult[i]-'0');
					// Height above local terrain
					QueryResult = PQgetvalue(res,j,8);
					NumBytes = PQgetlength(res,j,8);
					TempPoint.m_High  = CharP_to_Double(QueryResult,NumBytes);
					// Timing Advance
					QueryResult = PQgetvalue(res,j,9);
					NumBytes = PQgetlength(res,j,9);
					TempPoint.m_TA = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_TA = TempPoint.m_TA*10 + (QueryResult[i]-'0');
					// Predicted signal strength
					QueryResult = PQgetvalue(res,j,10);
					NumBytes = PQgetlength(res,j,10);
					TempPoint.m_PredTA = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_PredTA = TempPoint.m_PredTA*10 + (QueryResult[i]-'0');
					// Phone power control parameter
					QueryResult = PQgetvalue(res,j,11);
					NumBytes = PQgetlength(res,j,11);
					TempPoint.m_MSTxPower = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_MSTxPower= TempPoint.m_MSTxPower*10 + (QueryResult[i]-'0');
					// Base station power control parameter
					QueryResult = PQgetvalue(res,j,12);
					NumBytes = PQgetlength(res,j,12);
					TempPoint.m_BSTxPower = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_BSTxPower= TempPoint.m_BSTxPower*10 + (QueryResult[i]-'0');
					// Measurement on BCCH ?
					QueryResult = PQgetvalue(res,j,13);
					NumBytes = PQgetlength(res,j,13);
					TempPoint.m_BCCH = (QueryResult[0]=='t')||(QueryResult[0]=='T');
					// Serving Cell Measurments
					QueryResult = PQgetvalue(res,j,14);
					NumBytes = PQgetlength(res,j,14);
					TempPoint.m_RxLevUpFull = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_RxLevUpFull = TempPoint.m_RxLevUpFull*10 + (QueryResult[i]-'0');
    				QueryResult = PQgetvalue(res,j,15);
					NumBytes = PQgetlength(res,j,15);
					TempPoint.m_RxLevUpFull = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_RxLevUpSub = TempPoint.m_RxLevUpSub*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,16);
					NumBytes = PQgetlength(res,j,16);
					TempPoint.m_RxLevDnFull = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_RxLevDnFull = TempPoint.m_RxLevDnFull*10 + (QueryResult[i]-'0');
    				QueryResult = PQgetvalue(res,j,17);
					NumBytes = PQgetlength(res,j,17);
					TempPoint.m_RxLevUpFull = 0;
					for (i=0;i<NumBytes;i++)
					 	TempPoint.m_RxLevDnSub = TempPoint.m_RxLevDnSub*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,18);
					NumBytes = PQgetlength(res,j,18);
					Year = 0;
					for (i=0;i<4;i++)
					 	Year =  Year*10 + (QueryResult[i]-'0'); 
					tmTime.tm_year = Year -1900;
					tmTime.tm_mon = 0;
					for (i=5;i<7;i++)
					 	tmTime.tm_mon =  tmTime.tm_mon*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_mday = 0;
					for (i=8;i<10;i++)
					 	tmTime.tm_mday =  tmTime.tm_mday*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_hour = 0;
					for (i=11;i<13;i++)
					 	tmTime.tm_hour =  tmTime.tm_hour*10 + (QueryResult[i]-'0'); 
					tmTime.tm_min = 0;
					for (i=14;i<16;i++)
					 	tmTime.tm_min =  tmTime.tm_min*10 + (QueryResult[i]-'0'); 
    				tmTime.tm_sec = 0;
    				for (i=17;i<19;i++)
					 	tmTime.tm_sec =  tmTime.tm_sec*10 + (QueryResult[i]-'0'); 
    				TempPoint.m_DateTime=mktime(&tmTime);
    				m_coreMMR.push_back(TempPoint);
					TempPoint.clear();
				}	
			}
			PQclear(res);
		}
		else
		{
			cout << "Problem with command: " << Query << endl;
			PQclear(res);
			PQreset(DBase);
			return 0;
		}
	}
	else
	{
		PQclear(res);
		PQreset(DBase);
		return 0;
	}
	m_NumOriginalMMR = m_coreMMR.size();
	
	CmdStr = "SELECT max(TPrimKey) from TestPoint";
	Query = CmdStr.c_str();
	res = PQexec(DBase, Query);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			QueryResult = PQgetvalue(res,0,0);
			NumBytes = PQgetlength(res,0,0);
			m_lastTP =0;
			for (i=0;i<NumBytes;i++)
				m_lastTP = m_lastTP*10 + (QueryResult[i]-'0');
		}	
		PQclear(res);
	}
	else
	{
		cout << "Problem with command: " << Query << endl;
		PQclear(res);
		PQreset(DBase);
		return 0;
	}
	

	CmdStr = "SELECT max(CDKey), CellData.CI, Sector.Site, max(BCCH), max(SitePos[0]), max(SitePos[1]), ";
	CmdStr += "max(AntennaKey), max(Bearing), max(AntMechTilt), max(AziBeamWidth), max(EIRP) ";
	CmdStr += "FROM CellData cross join Cell cross join Sector ";
	CmdStr += "cross join AntennaPattern cross join Site ";
	CmdStr += "where CellData.CI=Cell.CI and Cell.SectorID=Sector.SI ";
	CmdStr += "and Sector.AntennaKey=AntennaPattern.AntPatternKey ";
	CmdStr += "and Sector.Site = Site.SitePK and LAC in ";
	CmdStr += LACList;
	CmdStr += "group by CellData.CI, Sector.SI, Sector.Site  ";
	CmdStr += "order by CellData.CI;";
	
	Query = CmdStr.c_str();
	res = PQexec(DBase, Query);
	if (res)
	{
		if ((PQresultStatus(res) == PGRES_TUPLES_OK)||(PQresultStatus(res) == PGRES_COMMAND_OK))
		{
			rows = PQntuples(res);
			if (rows)
			{
				for (j=0; j<rows ;j++)
				{
					QueryResult = PQgetvalue(res,j,0);
					NumBytes = PQgetlength(res,j,0);
					TempCell.m_CIpKey =0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_CIpKey =  TempCell.m_CIpKey*10 + (QueryResult[i]-'0');
					if (CellLastKey < TempCell.m_CIpKey)
						CellLastKey = TempCell.m_CIpKey; 
					QueryResult = PQgetvalue(res,j,1);
					NumBytes = PQgetlength(res,j,1);
					TempCell.m_CI = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_CI =  TempCell.m_CI*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,2);
					NumBytes = PQgetlength(res,j,2);
					TempCell.m_Site = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_Site =  TempCell.m_Site*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,3);
					NumBytes = PQgetlength(res,j,3);
					TempCell.m_ARFCN = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_ARFCN =  TempCell.m_ARFCN*10 + (QueryResult[i]-'0'); 
					QueryResult = PQgetvalue(res,j,4);
					NumBytes = PQgetlength(res,j,4);
    				TempCellLat = 0.0;
					pp = 0;
					ss = 0;
					ii = 0;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign = 1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=',');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempCellLat = TempCellLat*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempCellLat/=sign*exp10(ss-pp);
					ii = 0;
					QueryResult = PQgetvalue(res,j,5);
					NumBytes = PQgetlength(res,j,5);
					TempCellLon = 0.0;
    				pp = 0;
					ss = 0;
					if (QueryResult[ii]=='-')
					{
						sign = -1.0;
						ii++;
					}
					else sign =1.0;
					for (i=ii;(i<NumBytes)&&(QueryResult[i]!=')');i++)
					{
						if (QueryResult[i]!='.')
						{
							TempCellLon = TempCellLon*10 + (QueryResult[i]-'0');
							ss++;
						}
						else pp = ss;	
					}
					if (pp) TempCellLon/=sign*exp10(ss-pp);
//					cout << "Lat: " << TempCellLat << "   Lon: " << TempCellLon << endl;
					TempCell.m_Ligging.Set(TempCellLat,TempCellLon);
					QueryResult = PQgetvalue(res,j,6);
					NumBytes = PQgetlength(res,j,6);
					TempCell.m_AntKey = 0;
					for (i=0;i<NumBytes;i++)
					 	TempCell.m_AntKey =  TempCell.m_AntKey*10 + (QueryResult[i]-'0');
					QueryResult = PQgetvalue(res,j,7);
					NumBytes = PQgetlength(res,j,7);
					TempCell.m_Azimuth = CharP_to_Double(QueryResult,NumBytes);
					QueryResult = PQgetvalue(res,j,8);
					NumBytes = PQgetlength(res,j,8);
					TempCell.m_MechTilt = CharP_to_Double(QueryResult,NumBytes);
    				QueryResult = PQgetvalue(res,j,9);
					NumBytes = PQgetlength(res,j,9);
					TempCell.m_AziBeam = CharP_to_Double(QueryResult,NumBytes);
					QueryResult = PQgetvalue(res,j,10);
					NumBytes = PQgetlength(res,j,10);
					TempCell.m_EIRP = CharP_to_Double(QueryResult,NumBytes);
    				m_TempList.push_back(TempCell);
					TempCell.clear();
				}	
			}
			PQclear(res);
		}
		else
		{
			cout << "Problem with command: " << Query << endl;
			PQclear(res);
			PQreset(DBase);
			return 0;
		}
	}
	else
	{
		cout << "Problem with command: " << Query << endl;
		PQclear(res);
		PQreset(DBase);
		return 0;
	}
	return 1;
}

//********************************************************************
double cPosition::EstimatePositions(PGconn* DBase)
{
	cPosSet ToBeEstimated;
	cMeas TempNBMeas;
	CellList TempCell;
	VecDouble Error;
	double fout, distance, bearing, Hf;;
	unsigned i, j, k, kN;
	int currentTP;
	unsigned ll, kk;
	unsigned NBstop, MMRstop, CellStop;
	
	NBstop = m_NBMeas.size();
	MMRstop = m_coreMMR.size();
	CellStop = m_TempList.size();
	i=0; 	j=0;	k=0;  kN=0;
	
/*	cout << CellStop << endl;
	for (int kkk=0; kkk<CellStop; kkk++)
		m_TempList[kkk].Display();
	cout << endl; 
	cout << endl;
*/	
	
	while ((i<NBstop)&&(j<MMRstop))
	{
		currentTP = m_NBMeas[i].m_TestPKey;
		while ((i<NBstop)&&(m_NBMeas[i].m_TestPKey==currentTP))
		{
			TempNBMeas=m_NBMeas[i];
			ToBeEstimated.m_NB_Meas.push_back(TempNBMeas);
//			cout <<"i:" <<  i<< "Key: "<<TempNBMeas.m_MeasKey << " CI: "<< TempNBMeas.m_CI << endl;
			i++;
			TempNBMeas.clear();
		}
//		cout <<"IIIII "<< i << endl;

		while ((j<MMRstop)&&(m_coreMMR[j].m_TPKey!=currentTP))	
			j++;
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
//		ToBeEstimated.m_TestPoint[0].Display();
//		cout << " j: " << j << endl;
		while ((k<CellStop)
				&&(ToBeEstimated.m_TestPoint[0].m_ServCI!=m_TempList[k].m_CI))
			k++;
		ToBeEstimated.m_ServCell = m_TempList[k];
//		ToBeEstimated.m_ServCell.Display();
		for(ll=0; ll<ToBeEstimated.m_NB_Meas.size(); ll++)
		{
			kN=k; 
			while ((ToBeEstimated.m_NB_Meas[ll].m_CI!=m_TempList[kN].m_CI)
					&&(kN<CellStop))
				kN++;
			if (kN==CellStop)
			{
				kN = 0;
				while ((kN<CellStop)&&(kN<k)&&
						(ToBeEstimated.m_NB_Meas[ll].m_CI!=m_TempList[kN].m_CI))
					kN++;
			}
			if (ToBeEstimated.m_NB_Meas[ll].m_CI==m_TempList[kN].m_CI)
			{
				ToBeEstimated.m_NB_Cells.push_back(m_TempList[kN]);	
			}
			else
			{
				ToBeEstimated.m_NB_Meas.erase(ToBeEstimated.m_NB_Meas.begin()+ll);
				ll--;
			}
		}
		
		bool Swap=FALSE;
		double S2SDist = 73000, Rkk;
		for(ll=0; ll<ToBeEstimated.m_NB_Meas.size(); ll++)
		{
			for (kk=ll+1; kk<ToBeEstimated.m_NB_Meas.size(); kk++)
			{
				Swap= (ToBeEstimated.m_NB_Cells[kk].m_CI==ToBeEstimated.m_ServCell.m_CI);
				Swap=Swap||((ToBeEstimated.m_NB_Cells[ll].m_CI!=ToBeEstimated.m_ServCell.m_CI)
							&&(ToBeEstimated.m_NB_Cells[kk].m_Site==ToBeEstimated.m_ServCell.m_Site)
							&&(ToBeEstimated.m_NB_Cells[ll].m_Site!=ToBeEstimated.m_ServCell.m_Site));
				if (!Swap)
				{
//					Rll = ToBeEstimated.m_NB_Cells[ll].m_Ligging.Distance(ToBeEstimated.m_ServCell.m_Ligging);
					Rkk = ToBeEstimated.m_NB_Cells[kk].m_Ligging.Distance(ToBeEstimated.m_ServCell.m_Ligging);
					if (Rkk>10)	S2SDist = MIN(S2SDist,Rkk);
//					Swap = (Rkk<Rll);
					Swap = (ToBeEstimated.m_NB_Cells[ll].m_Site!=ToBeEstimated.m_ServCell.m_Site)
							&&((ToBeEstimated.m_NB_Meas[kk].m_MeasValue-ToBeEstimated.m_NB_Cells[kk].m_EIRP) 
							> (ToBeEstimated.m_NB_Meas[ll].m_MeasValue-ToBeEstimated.m_NB_Cells[kk].m_EIRP));
				}
				else Swap = Swap&&(ToBeEstimated.m_NB_Cells[ll].m_CI!=ToBeEstimated.m_ServCell.m_CI)
							&&((ToBeEstimated.m_NB_Meas[kk].m_MeasValue-ToBeEstimated.m_NB_Cells[kk].m_EIRP) 
								> (ToBeEstimated.m_NB_Meas[ll].m_MeasValue-ToBeEstimated.m_NB_Cells[kk].m_EIRP));
				if (Swap)
				{
					TempCell = ToBeEstimated.m_NB_Cells[ll];
					TempNBMeas = ToBeEstimated.m_NB_Meas[ll];
					ToBeEstimated.m_NB_Cells[ll]=ToBeEstimated.m_NB_Cells[kk];
					ToBeEstimated.m_NB_Meas[ll]=ToBeEstimated.m_NB_Meas[kk];
					ToBeEstimated.m_NB_Meas[kk]=TempNBMeas;
					ToBeEstimated.m_NB_Cells[kk]=TempCell;
				}	
			}
		}
	
		int Best = 1;
		if (ToBeEstimated.m_TestPoint[0].m_PosSource==GPS)
		{
			Error.push_back(0.0);
			ToBeEstimated.m_TestPoint[0].m_PredTA = 
				(short int)trunc(ToBeEstimated.m_TestPoint[0].m_Position.
						Distance(ToBeEstimated.m_ServCell.m_Ligging)/TA);
		}
		else Error.push_back(72000.0);
//		if (fabs(ToBeEstimated.m_TestPoint[0].m_PredTA-ToBeEstimated.m_TestPoint[0].m_TA)>2)
//		{
//			cout << "Wat gaan aan??" << endl;
//			ToBeEstimated.m_TestPoint[0].m_TA=ToBeEstimated.m_TestPoint[0].m_PredTA;
//			m_coreMMR[j].m_TA = ToBeEstimated.m_TestPoint[0].m_PredTA;
//		}

		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.SI(ToBeEstimated.m_TestPoint.size()-1);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
			Best = ToBeEstimated.m_TestPoint.size()-1;
		}
		
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		if(ToBeEstimated.m_ServCell.m_AziBeam>180.0)	
			fout = ToBeEstimated.SecondSite(ToBeEstimated.m_TestPoint.size()-1);
		else fout = ToBeEstimated.CI_TA(ToBeEstimated.m_TestPoint.size()-1);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
			Best = ToBeEstimated.m_TestPoint.size()-1;
		}
		
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.CoSecAzi(DBase,ToBeEstimated.m_TestPoint.size()-1);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
			if (S2SDist>650) Best = ToBeEstimated.m_TestPoint.size()-1;
		}
		
		ToBeEstimated.Initialise(DBase,ToBeEstimated.m_TestPoint[Best].m_Position);
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.CoSinRule
					(DBase,ToBeEstimated.m_TestPoint.size()-1);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
//			if ((ToBeEstimated.m_TestPoint[0].m_TA>3)&&(Best<3)) 
//				Best = ToBeEstimated.m_TestPoint.size()-1;
		}
	
/*		ToBeEstimated.Initialise(DBase,ToBeEstimated.m_TestPoint[Best].m_Position);
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.DoubleDelta
					(DBase,ToBeEstimated.m_TestPoint.size()-1);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
//			if ((ToBeEstimated.m_TestPoint[0].m_TA<2)&&(S2SDist>620)&&(Best<3))
//				Best = ToBeEstimated.m_TestPoint.size()-1;
		}
*/		
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.BruteForceSearch
					(DBase,ToBeEstimated.m_TestPoint.size()-1,
					ToBeEstimated.m_TestPoint[Best].m_Position);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
//			Best = ToBeEstimated.m_TestPoint.size()-1;
		}

/*		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.CorrCoefOpt
				(DBase,ToBeEstimated.m_TestPoint.size()-1,
				ToBeEstimated.m_TestPoint[Best].m_Position);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
			Error.push_back(fout);
*/	
		ToBeEstimated.m_TestPoint.push_back(m_coreMMR[j]);
		fout = ToBeEstimated.DistanceDiff
					(DBase,ToBeEstimated.m_TestPoint.size()-1,
					ToBeEstimated.m_TestPoint[Best].m_Position);
		if (fout==70000.0)
			ToBeEstimated.m_TestPoint.erase(ToBeEstimated.m_TestPoint.end()-1);
		else
		{
			Error.push_back(fout);
//			if ((ToBeEstimated.m_TestPoint[0].m_TA>2)&&(S2SDist>820)&&(Best<3))
//				Best = ToBeEstimated.m_TestPoint.size()-1;
		}
		
		// if no position estimate existed replace with latest 
		if (m_coreMMR[j].m_PosSource==NOTD)
			m_coreMMR[j]=ToBeEstimated.m_TestPoint[ToBeEstimated.m_TestPoint.size()-1];
		
		cout << "ServCI:," << ToBeEstimated.m_ServCell.m_CI	
				<< ", TP:,"<< ToBeEstimated.m_TestPoint[0].m_TPKey
				<< ", TA:," << ToBeEstimated.m_TestPoint[0].m_TA << ",  "
				<< ", PredTA:," << ToBeEstimated.m_TestPoint[0].m_PredTA << ", ";
//		cout << endl; 
		for(ll=1; ll<ToBeEstimated.m_NB_Meas.size(); ll++)
		{
			distance = ToBeEstimated.m_NB_Cells[ll].m_Ligging.
							Distance(ToBeEstimated.m_ServCell.m_Ligging);
			bearing = ToBeEstimated.m_ServCell.m_Ligging.
							Bearing(ToBeEstimated.m_NB_Cells[ll].m_Ligging);
			Hf = ToBeEstimated.m_NB_Cells[ll].m_Azimuth - bearing;
			ToBeEstimated.m_NB_Cells[ll].m_Ligging.Display();
			cout 	<< " BCCH:,"<< ToBeEstimated.m_NB_Cells[ll].m_ARFCN
					<< ", CI:," << ToBeEstimated.m_NB_Cells[ll].m_CI
					<< ", Meas:," << ToBeEstimated.m_NB_Meas[ll].m_MeasValue 
					<< ", Azi:," << ToBeEstimated.m_NB_Cells[ll].m_Azimuth
					<< ", Theta:," << bearing 
					<< ", Hf:," << Hf - 180*Hf/fabs(Hf)
					<< ", Dist:," << distance << ",   ";
//			cout << endl;
		}
		cout << ", ,";
		for (ll=0; ll<ToBeEstimated.m_TestPoint.size(); ll++)
		{
			distance = ToBeEstimated.m_TestPoint[ll].m_Position.
						Distance(ToBeEstimated.m_ServCell.m_Ligging);
			bearing = ToBeEstimated.m_ServCell.m_Ligging.Bearing(
						ToBeEstimated.m_TestPoint[ll].m_Position);
//			cout << "  SC CI: " << ToBeEstimated.m_ServCell.m_CI 
//				<< "   CI: " << ToBeEstimated.m_TestPoint[ll].m_ServCI << endl; 
//			if (ll>ToBeEstimated.m_TestPoint[ll].m_PosSource-1)
//				cout << "Huh?" << endl;
			cout<< "M:," << ToBeEstimated.m_TestPoint[ll].m_PosSource <<",";
			ToBeEstimated.m_TestPoint[ll].m_Position.Display();
			fout = ToBeEstimated.m_TestPoint[0].m_Position.Distance(ToBeEstimated.m_TestPoint[ll].m_Position);
			cout << " Err:, "	<< fout << " ,Dist:," << distance << " ,Theta:," << bearing << ",   ";
//			cout << endl; 
		}
		distance = ToBeEstimated.m_TestPoint[Best].m_Position.
								Distance(ToBeEstimated.m_ServCell.m_Ligging);
		bearing = ToBeEstimated.m_ServCell.m_Ligging.Bearing(
								ToBeEstimated.m_TestPoint[Best].m_Position);
		//			cout << "  SC CI: " << ToBeEstimated.m_ServCell.m_CI 
		//				<< "   CI: " << ToBeEstimated.m_TestPoint[ll].m_ServCI << endl; 
		cout<< ",M:," << ToBeEstimated.m_TestPoint[Best].m_PosSource <<",";
		ToBeEstimated.m_TestPoint[Best].m_Position.Display();
		fout = ToBeEstimated.m_TestPoint[0].m_Position.Distance(ToBeEstimated.m_TestPoint[Best].m_Position);
		cout << " Err:, "	<< fout << " ,Dist:," << distance << " ,Theta:," << bearing << ",   ";
//		cout << endl; 
		cout <<'\n';
//		if (ToBeEstimated.m_TestPoint[0].m_TA>2)
//			cout << "TA groot" << endl;
		for (ll=1; ll<ToBeEstimated.m_TestPoint.size(); ll++)
		{
			m_lastTP++;
//			cout << "lastTP: " << m_lastTP << endl;
			ToBeEstimated.m_TestPoint[ll].m_TPKey = m_lastTP;
			m_coreMMR.push_back(ToBeEstimated.m_TestPoint[ll]);
		}
		ToBeEstimated.clearOld();
	}
	return 1.0;
}

//********************************************************************
int cPosition::WriteToDatabase(PGconn* DBase)
{
	PGresult* res; 
	unsigned j;
	
	res = PQexec(DBase, "BEGIN;");
    PQclear(res);
    cout << "MMRs  " << m_coreMMR.size() << endl;
    for (j=0; j<(unsigned)m_NumOriginalMMR; j++)
    	m_coreMMR[j].UpdateDB(DBase);
   	for (j=(unsigned)m_NumOriginalMMR; j<m_coreMMR.size(); j++)
		m_coreMMR[j].InsertInDB(DBase);
	res = PQexec(DBase, "END;");
	PQclear(res);
	return 1;
		
}//End WriteToDatabase

