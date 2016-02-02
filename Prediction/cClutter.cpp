 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/08/12
 *    License     : GNU GPLv3
 *    File        : cClutter.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : container class for clutter information. 
 *
 **************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

// Include local headers
#include "cClutter.h"


using namespace Qrap;


//*************************************************************************
// Default constructor, this assumes all ClutterTypes regardless of 
// Classification Group will be used. 
cClutter::cClutter()
{
//	cout << "In cClutter Default constructor" << endl;
	mNumber = 0;
	mClutterTypes = NULL;
	mClassificationGroup = 0;
	string setting = gDb.GetSetting("UseClutter");
	if (setting=="true")
		GetFromDatabase();
}/*end cClutter Default Constructor */


//*************************************************************************
// Overloaded constructor, where a specific Clutter Classification Group 
// is specified. 
cClutter::cClutter(unsigned ClassGroup)
{
//	cout << "In cClutter Overlaod constructor" << endl;
	mNumber = 0;
	mClutterTypes = NULL;
	mClassificationGroup = ClassGroup;
	string setting = gDb.GetSetting("UseClutter");
	if (setting=="true")
		GetFromDatabase(ClassGroup);
}/*end cClutter Default Constructor */


//*************************************************************************
//## Destructor 
cClutter::~cClutter()
{
		Destroy();
}


//*************************************************************************
//## Assignment Operation (implementation)
const cClutter & cClutter::operator=(const cClutter &right)
{
	unsigned i,j;
	if (this == &right)
		return (*this);
	
	Destroy();
	
	mNumber = right.mNumber;
	mClutterTypes = new sClutter[mNumber];

	for (i=0; i<mNumber; i++)
	{
		mClutterTypes[i].sLandCoverID 	= 	right.mClutterTypes[i].sLandCoverID;
		mClutterTypes[i].sRho 		= 	right.mClutterTypes[i].sRho;
		mClutterTypes[i].sHeight	= 	right.mClutterTypes[i].sHeight;
		mClutterTypes[i].sCoefficients 	= 	new double[NUMTERMS];
		mClutterTypes[i].sAllowCchange 	= 	new bool[NUMTERMS];
		for (j=0; j<NUMTERMS; j++)
		{
			mClutterTypes[i].sCoefficients[j] = right.mClutterTypes[i].sCoefficients[j];
			mClutterTypes[i].sAllowCchange[j] = right.mClutterTypes[i].sAllowCchange[j];
		}
	}
	

return (*this);
}/* end cClutter Assignment */

			
//*************************************************************************
void cClutter::Display()
{
	unsigned i,j;
	for (i=0; i<mNumber; i++)
	{
		cout << "ID: " << mClutterTypes[i].sLandCoverID;
		cout << "	rho: " << mClutterTypes[i].sRho;
		cout << "	H: " << mClutterTypes[i].sHeight << endl;
		cout << "TermCoeffs:";
		for (j=0; j<NUMTERMS; j++)
			cout << "	" << j<< "	" << mClutterTypes[i].sCoefficients[j];
		cout << endl;
	}
}


//*************************************************************************
// This routine is called from the constructors and hence does constructor work.
bool cClutter::GetFromDatabase(unsigned ClassGroup)
{
	bool Success=false;
	pqxx::result r, CC;
	string query, queryC, queryU, queryCC, queryI;
	char *temp, *type, *idnr;
	unsigned MaxLandCoverID;
	temp = new char[33];
	type = new char[33];
	idnr = new char[33];
	unsigned j=0,jj=0, landcoverid=0, CoefIndex=0;
	mNumber = 0;

	queryC = " UNION SELECT term, coefficient from coefficients ";
	queryC += " cross join cluttertype as this cross join cluttertype as standard ";
	queryC += " where this.standardtype = standard.id ";
	queryC += " and coefficients.cluttertype = standard.id ";
	queryC += " and this.landcoverid = ";

	queryU = "Select distinct term, coefficient from (";
	queryU += "SELECT term, coefficient from coefficients ";
	queryU += " cross join cluttertype";
	queryU += " where standardtype is NULL ";
	queryU += " and cluttertype = ";
	
	query = "Select distinct id, landcoverid, height, width from (";
	query += "SELECT this.id as id, this.landcoverid as landcoverid, standard.height as height, standard.width as width";
	query += " FROM cluttertype as this CROSS JOIN cluttertype standard ";
	query += " WHERE standard.id=this.standardtype ";
	if (ClassGroup<9000)
	{
		query += " AND this.classgroup=";
		gcvt(ClassGroup,9,temp);
		query += temp; 
	}
	query += " UNION ";
	query += " SELECT id, landcoverid, height, width ";
	query += " FROM cluttertype WHERE standardtype is NULL ";
	if (ClassGroup<9000)
	{
		query += " AND classgroup=";
		gcvt(ClassGroup,9,temp);
		query += temp; 
	}
	query += " ) as alles order by landcoverid;";
					
	if(!gDb.PerformRawSql(query))
	{
		string err = "Not Using Clutter because of failure of Database Select for basic Clutter information. Query: ";
		err += query;
		QRAP_WARN(err.c_str());
	 	Success=false;
	}
	else
	{
		gDb.GetLastResult(r);
		mNumber = r.size();
		if (mNumber)
		{
			Success = true;
			MaxLandCoverID =0;
			for (j=0; j<mNumber; j++)
			{
				landcoverid = atoi(r[j]["landcoverid"].c_str());
				if (landcoverid > MaxLandCoverID)
					MaxLandCoverID = landcoverid;
			}
			mClutterTypes = new sClutter[MaxLandCoverID+1];
			for (j=0; j<MaxLandCoverID+1; j++)
			{
				mClutterTypes[j].sCoefficients = new double[NUMTERMS];
				mClutterTypes[j].sAllowCchange = new bool[NUMTERMS];
				mClutterTypes[j].sLandCoverID = 0;
				mClutterTypes[j].sRho = 0.8;
				mClutterTypes[j].sHeight = 0;

				for (jj=0; jj<NUMTERMS; jj++)
				{
					mClutterTypes[j].sCoefficients[jj]=0.0;
					mClutterTypes[j].sAllowCchange[jj] = false;
				}
			}
			for (j=0; (j<mNumber)&&(Success); j++)
			{
				landcoverid = atoi(r[j]["landcoverid"].c_str());
				mClutterTypes[landcoverid].sLandCoverID = landcoverid;
				mClutterTypes[landcoverid].sRho = atof(r[j]["width"].c_str());
				mClutterTypes[landcoverid].sHeight = atof(r[j]["height"].c_str());
//				cout << landcoverid << "	" << mClutterTypes[landcoverid].sHeight << endl;
				gcvt(atoi(r[j]["id"].c_str()),9,idnr);
				gcvt(landcoverid,9,type);
				queryCC = queryU;
				queryCC += idnr;
				queryCC += queryC;
				queryCC += type;
				queryCC += ") as alles order by term;";
				if(!gDb.PerformRawSql(queryCC))
				{
					string err = "Database Select for Clutter Coefficients failed. Query: ";
					err += queryCC;
					QRAP_WARN(err.c_str());
					Success = false;
				}
				else
				{
					gDb.GetLastResult(CC);
					
//					cout << j << " " << landcoverid << endl;
					if (CC.size()>0)
					{
//						cout << queryCC << endl;
//						cout << CC.size() << endl;
//						if (CC.size()==NUMTERMS)
//						{
						for (jj=0; jj<CC.size(); jj++)
						{
							CoefIndex = atoi(CC[jj]["term"].c_str());
							if (CoefIndex<NUMTERMS)
								mClutterTypes[landcoverid].sCoefficients[CoefIndex]
										=atof(CC[jj]["coefficient"].c_str());
//							cout << "lid=" << landcoverid << "	c=" << CoefIndex << "	" 
//								<< mClutterTypes[landcoverid].sCoefficients[CoefIndex] 
//								<< endl;
						}
/*						}
						else
						{
							for (jj=0; jj<NUMTERMS; jj++)
							{
								queryI = "INSERT INTO coefficients"; 
								queryI += "(term, cluttertype, coefficient) values (";	
								gcvt(jj,9,temp);
								queryI += temp;
								queryI += ", ";
								queryI += idnr;
								queryI += ", 0);";
								if(!gDb.PerformRawSql(queryI))
								{
									string err = "Failure inserting the coefficients. Query: ";
									err += queryI;
									QRAP_WARN(err.c_str());
								}
							}
						}
*/
					}
					else if (landcoverid>0)
					{
//						cout << "Empty query: " << queryCC << endl;
						for (jj=0; (jj<NUMTERMS)&&(Success); jj++)
						{
							queryI = "INSERT INTO coefficients (term, cluttertype, coefficient) values (";
							gcvt(jj,9,temp);
							queryI += temp;
							queryI += ", ";
							queryI += idnr;
							queryI += ", 0);";
							if(!gDb.PerformRawSql(queryI))
							{
								string err = "Failure inserting the coefficients. Query: ";
								err += queryI;
								QRAP_WARN(err.c_str());
								Success= false;						
							}
						}
					}
				}
			}
		}
		else
		{
			string err = "Not Using Clutter because of void query for basic Clutter information. Query: ";
			err += query;
			QRAP_WARN(err.c_str());
			Success = false;
		}
			
	} 
	
	if (!Success)
	{
		MaxLandCoverID = 1;
		mClutterTypes = new sClutter[MaxLandCoverID+1];
		for (j=0; j<MaxLandCoverID+1; j++)
		{
			mClutterTypes[j].sCoefficients = new double[NUMTERMS];
			mClutterTypes[j].sAllowCchange = new bool[NUMTERMS];
			mClutterTypes[j].sLandCoverID = 0;
			mClutterTypes[j].sRho = 0.8;
			mClutterTypes[j].sHeight = 0;

			for (jj=0; jj<NUMTERMS; jj++)
			{
				mClutterTypes[j].sCoefficients[jj]=0.0;
				mClutterTypes[j].sAllowCchange[jj] = false;
			}
		}
	}
	
/*	query = "commit;";
	if(!gDb.PerformRawSql(query))
	{
		string err = "Failing to commit in cClutter::GetFromDatabase. Query: ";
		err += query;
		QRAP_WARN(err.c_str());
	}
*/
	mNumber = MaxLandCoverID+1;
//	cout << "mNumber=" << mNumber << endl;
	delete [] type;
	delete [] temp;
	delete [] idnr; 
	return Success;
}

//**************************************************************************************
//* Delete the content of the class
void cClutter::Destroy()
{
	unsigned i;
	for (i=0; i<mNumber; i++)
	{
		delete [] mClutterTypes[i].sCoefficients;
		delete [] mClutterTypes[i].sAllowCchange;
	}

	if (mClutterTypes!=NULL) delete [] mClutterTypes;
}

//**************************************************************************************
//* Reset the Clutter class to reflect the content of the specified Classification Group
bool cClutter::Reset (unsigned ClassGroup)
{
//	cout << " In cClutter::Reset " << endl;
	bool value;
	Destroy();
	mClassificationGroup=ClassGroup;
	value = GetFromDatabase(ClassGroup);
	return value;
}

//*************************************************************************************************
//*
bool cClutter::UpdateCoefficients(unsigned ClutterType)
{

//	cout << " In cClutter::UpdateCoefficients  type = " << ClutterType << end;

	bool Success=true;
	string query, queryI;
	char *term, *coef, *type, *group;
	term = new char[33];
	coef = new char[33];
	type = new char[33];
	group = new char[33];
	unsigned j=0;
	gcvt(mClutterTypes[ClutterType].sLandCoverID,9,type);
	gcvt(mClassificationGroup,9,group);

	string queryA;
	queryA = " AND cluttertype = (select id from cluttertype where ";
	queryA += " classgroup = ";
	queryA += group;
	queryA += " and landcoverid = ";
	queryA += type;
	queryA += ");";

	Success=true;
	for (j=0; (j<NUMTERMS)&&(Success); j++)
	{
//		cout << "	term = "<< j << "	coef = " << mClutterTypes[ClutterType].sCoefficients[j] << endl;
		query = "UPDATE coefficients set coefficient = ";
		gcvt(mClutterTypes[ClutterType].sCoefficients[j],9,coef);	
		query += coef;
		query += " WHERE term = ";
		gcvt(j,9,term);	
		query += term;
		query += queryA;

//		cout << query << endl;
		if(!gDb.PerformRawSql(query))
		{
			string err = "Failure updating the coefficients. Query: ";
			err += query;
			QRAP_WARN(err.c_str());	
			Success = false;
		}
	}
//	cout << endl;

	delete [] term;
	delete [] type;
	delete [] coef;
	delete [] group;
	return Success;
}

//*************************************************************************************************
//*
bool cClutter::UpdateHeightsWidths()
{
	bool Success=true;
	string query, queryI;
	char *width, *height, *type, *group;
	width = new char[33];
	height = new char[33];
	type = new char[33];
	group = new char[33];
	unsigned j=0;


	for (j=1; (j<mNumber)&&(Success); j++)
	{
		gcvt(mClutterTypes[j].sLandCoverID,9,type);
		gcvt(mClassificationGroup,9,group);

		string queryA;
		queryA = " WHERE ";
		queryA += " classgroup = ";
		queryA += group;
		queryA += " and landcoverid = ";
		queryA += type;
		queryA += ";";

		query = "UPDATE cluttertype set ( height, width ) = ( ";
		gcvt(mClutterTypes[j].sHeight,9,height);	
		query += height;
		query += " , ";
		gcvt(mClutterTypes[j].sRho,9,width);	
		query += width;
		query += " ) ";		
		query += queryA;

//		cout << query << endl;
		if(!gDb.PerformRawSql(query))
		{
			string err = "Failure updating the height. Query: ";
			err += query;
			QRAP_WARN(err.c_str());	
			Success = false;
		}
	}

	delete [] width;
	delete [] type;
	delete [] height;
	delete [] group;
	return Success;
}


