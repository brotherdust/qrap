//## Class: cImportMMR
//	This is to read the drive-tests and import it into the database.

#ifndef cImportMMR_H_
#define cImportMMR_H_ 1

#include "cgeneric.h"
#include "memmngr.h"
#include "cMeas.h"
#include "cTPoint.h"
#include "CellList.h"

#define DB_FILE_SIZE 40
#define FAR 5000000

using namespace std;
using std::cout;

typedef vector<CellList> VecCells;
typedef vector<cTPoint>	 VecMMRs;
typedef	vector<cMeas> 	 VecMeas;
typedef vector<int>		 VecInt;

class cImportMMR
{

public:
	cImportMMR();
	~cImportMMR();
	
	int GetCurrentCellList(PGconn* DBase, int &CellLastKey);
	int ReadTEMSfile(const char* DriveTestFile, time_t FileDate,
					int &MMRLastKey, int &MeasLastKey, int &CellLastKey);
	int SortReduceCellList();
	int GetCIfromBSICBCCH();
	int WriteToDatabase(PGconn* DBase);
	int WriteCellListToDB(PGconn* DBase);

private:
	VecMMRs		m_coreMMR;
	VecMeas		m_NBMeas;
	VecCells	m_TempList;
	VecInt		m_LACList;
};


#endif
