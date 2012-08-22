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
	cout << "In cClutter Default constructor" << endl;
	mNumber = 0;
	GetFromDatabase();
}/*end cClutter Default Constructor */


//*************************************************************************
// Overloaded constructor, where a specific Clutter Classification Group 
// is specified. 
cClutter::cClutter(unsigned ClassGroup)
{
	cout << "In cClutter Overlaod constructor" << endl;
	mNumber = 0;
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
		mClutterTypes[i].sWidth 	= 	right.mClutterTypes[i].sWidth;
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
		cout << "	W: " << mClutterTypes[i].sWidth;
		cout << "	H: " << mClutterTypes[i].sHeight << endl;
		for (j=0; j<NUMTERMS; j++)
			cout << "TERM" << j<< "	" << mClutterTypes[i].sCoefficients[j] << endl;
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

	queryU = "SELECT term, coefficient from coefficients ";
	queryU += " cross join cluttertype";
	queryU += " where standardtype is NULL ";
	queryU += " and coefficients.cluttertype = ";
	
	query = "SELECT this.id, this.landcoverid as landcoverid, standard.height as height, standard.width as width";
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
					
	if(!gDb.PerformRawSql(query))
	{
		string err = "Not Using Clutter because of failure of Database Select for basic Clutter information. Query: ";
		err += query;
		QRAP_WARN(err.c_str());
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
				mClutterTypes[j].sWidth = 0;
				mClutterTypes[j].sHeight = 0;

				for (jj=0; jj<NUMTERMS; jj++)
				{
					mClutterTypes[j].sCoefficients[jj]=0.0;
					mClutterTypes[j].sAllowCchange[jj] = false;
				}
			}
			for (j=0; j<mNumber; j++)
			{
				landcoverid = atoi(r[j]["landcoverid"].c_str());
				mClutterTypes[landcoverid].sLandCoverID = landcoverid;
				mClutterTypes[landcoverid].sWidth = atof(r[j]["width"].c_str());
				mClutterTypes[landcoverid].sHeight = atof(r[j]["height"].c_str());

				gcvt(atoi(r[j]["id"].c_str()),9,idnr);
				gcvt(landcoverid,9,type);
				queryCC = queryU;
				queryCC += type;
				queryCC += queryC;
				queryCC += type;
				queryCC += " order by term;";
				if(!gDb.PerformRawSql(queryCC))
				{
					string err = "Database Select for Clutter Coefficients failed. Query: ";
					err += queryCC;
					QRAP_WARN(err.c_str());
				}
				else
				{
					gDb.GetLastResult(CC);
					
					if (CC.size()>0)
					{
						if (CC.size()==NUMTERMS)
						{
							for (jj=0; jj<CC.size(); jj++)
							{
								CoefIndex = atoi(CC[jj]["term"].c_str());
								if (CoefIndex<NUMTERMS)
									mClutterTypes[landcoverid].sCoefficients[CoefIndex]
										=atof(CC[jj]["coefficient"].c_str());
							}
						}
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
					}
					else
					{
						for (jj=0; jj<NUMTERMS; jj++)
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
			mClutterTypes[j].sWidth = 0;
			mClutterTypes[j].sHeight = 0;

			for (jj=0; jj<NUMTERMS; jj++)
			{
				mClutterTypes[j].sCoefficients[jj]=0.0;
				mClutterTypes[j].sAllowCchange[jj] = false;
			}
		}
	}
	
	mNumber = MaxLandCoverID+1;
	delete [] type;
	delete [] temp;
	delete [] idnr; 
	return Success;
}

//**************************************************************************************
//* Delete the content of the class
bool cClutter::Destroy()
{
	unsigned i;
	for (i=0; i<mNumber; i++)
	{
		delete [] mClutterTypes[i].sCoefficients;
		delete [] mClutterTypes[i].sAllowCchange;
	}

	delete [] mClutterTypes;
}

//**************************************************************************************
//* Reset the Clutter class to reflect the content of the specified Classification Group
bool cClutter::Reset (unsigned ClassGroup)
{
//	cout << " In cClutter::Reset " << endl;
	bool value;
	Destroy();
	value = GetFromDatabase(ClassGroup);
	return value;
}

//*************************************************************************************************
//*
bool cClutter::UpdateCoefficients(unsigned ClutterType)
{
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

	for (j=0; j<NUMTERMS; j++)
	{
		query = "UPDATE coefficients set coefficient = ";
		gcvt(mClutterTypes[ClutterType].sCoefficients[j],9,coef);	
		query += coef;
		query += " WHERE term = ";
		gcvt(j,9,term);	
		query += term;
		query += queryA;

		if(!gDb.PerformRawSql(query))
		{
			string err = "Failure updating the coefficients. Query: ";
			err += query;
			QRAP_WARN(err.c_str());	
			Success = false;
		}
	}

	delete [] term;
	delete [] type;
	delete [] coef;
	delete [] group;
	return Success;
}

