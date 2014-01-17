#include "libpq-fe.h"
#include "cPosition.h"
#include "cgeneric.h"

int main(int argc, char **argv)
// argv[1] -  list of location areas of interest
// argv[2] - database name
{
	char st[255];
	char* conninfo;
	const char* List;
	char* Dir;
	char* cLAC;
	int LAC, i;
	PGconn* DBase;
	PGresult* res;
	char	   *pghost,
			   *pgport,
			   *pgoptions,
			   *pgtty,
			   *dBName;
 
    pghost = NULL;				// host name of the backend server 
	pgport = NULL;				// port of the backend server
	pgoptions = NULL;			// special options to start up the backend server
	pgtty = NULL;				// debugging tty for the backend server
	cPosition KryPosisie;
		
/*	//Toets cGeoP
	cGeoP hier(-26,27);
	cGeoP daar(-25.5,27);
	double distance, bearing;
	distance= hier.Distance(daar);
	bearing=hier.Bearing(daar);
	

	for (i=0;i<360;i++)
	{
		bearing=i;
		daar.FromHere(hier,distance,bearing);
		distance= hier.Distance(daar);
		bearing=hier.Bearing(daar);
		cout << "i: " << i << "   ";
		hier.Display();
		cout << "   daar: ";
		daar.Display();
		cout << "   D: " << distance << "   B " << bearing << endl;
	}
*/	
		
	if (argc > 1)
	{
		List = argv[1];
		int size = strlen(List);
		for (i=size-1;(i>0)&&(List[i]!='/');i--);
		Dir = new char[size+1];
		strcpy(Dir,List);
		for(i=i+1;i<size;i++)
			Dir[i]='\0';
		cout << Dir << endl;	
		chdir(Dir);
		// Kry directory sodat die antenna leers in daai directory oopgemaak sal word.
		if (argc > 2) conninfo = argv[2];
        else 
        {
        	conninfo = "host=/tmp port=5432 dbname=ACP_MMR user=maggie";
        	dBName="ACP_MMR";
        }
	}
    else
	{
        List = "LAC.lst";
       	Dir = new char[2];
		conninfo = "host=/tmp port=5432 dbname=ACP_MMR user=maggie";
		dBName="ACP_MMR";
	}
  
	ifstream fp;
	string line, field;
	fp.open(List,ios::in);
	if (!fp.rdbuf()->is_open())
	{
		cout << "could not open " << List << endl;
		return 0;
	}
	
	while (!fp.eof())
	// for the entire lac
	{
		fp.getline(st,255);
		cLAC= new char[strlen(st)+2];
		sscanf(st,"%s",cLAC);
		LAC = 0;
		for (i=0;(i<7)&&(cLAC[i]!='\0');i++)
			LAC =  LAC*10 + (cLAC[i]-'0');
		if ((LAC<65536)&&(LAC>=0))
			KryPosisie.m_LACList.push_back(LAC);
		delete [] cLAC;
	}
	fp.close();
	delete [] Dir;
	
	DBase = PQconnectdb(conninfo);
	if (PQstatus(DBase) != CONNECTION_OK)
    {
    	fprintf(stderr, "Connection to database failed: %s",
                        PQerrorMessage(DBase));
		PQfinish(DBase);
		return 0;
    }
	KryPosisie.GetCurrentData(DBase);
	KryPosisie.EstimatePositions(DBase);
	KryPosisie.WriteToDatabase(DBase);	
	
	res = PQexec(DBase, "END;");
	PQclear(res);
}//end main
	
       
	
