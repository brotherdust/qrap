 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAntpatn.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents an antenna in a radio installation
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

#include "cAntpatn.h"

using namespace Qrap;

// Class CAntennaPattern
//## Constructors (implementation)
// CAntennaPattern:: Default Constructor
//*********************************************************************
cAntennaPattern::cAntennaPattern()
{
	mNAA = 3;
	mNEA = 3;
	mFreq = 900.0;
	mGain = 0.0;
	mBearing = 0.0;
	mMechTilt = 0.0;
	mFtoB = 0.0;
	mBeamW = 360.0;
	mVBeamW = 90.0;
	mPrice = 0.0;
	mRef180az = 0;
	mRef0el = 0;
	mAziAngles = new float[mNAA];
	mAziAngles[0]=0;
	mAziAngles[1]=120;
	mAziAngles[2]=240;
	mAziValues = new float[mNAA];
	mAziValues[0]=0;
	mAziValues[1]=0;
	mAziValues[2]=0;
	mElevAngles = new float[mNEA];
	mElevAngles[0]=-85;
	mElevAngles[1]=0;
	mElevAngles[2]=85;
	mElevValues = new float[mNEA];
	mElevValues[0]=0;
	mElevValues[1]=0;
	mElevValues[2]=0;
	mAntPattern = new_Float2DArray(mNAA,mNEA);
	for (unsigned i=0; i<mNAA; i++)
		for (unsigned j=0; j<mNEA; j++)
			mAntPattern[i][j]=0.0;
	mFile = new char[DB_FILE_SIZE];
	mName = new char[DB_NAME_SIZE];
	mMake = new char[DB_MAKE_SIZE];
	mDescription = new char[DB_DESCRIB_SIZE];
	mPol = new char[2];
	mPol[0] ='V';
	mPol[1] ='\0'; 
	mAntennasANN = new FANN::neural_net[2];
	ResetUseAntANN();

}/* end CAntennaPattern:: Default Constructor */

//*********************************************************************
bool cAntennaPattern:: ResetUseAntANN()
{
	string setting = gDb.GetSetting("UseAntANN");
	if (setting=="true")
		mUseANN = true;
	else mUseANN = false;
	return mUseANN;
}/* end CAntennaPattern:: ResetUseAntANN */


//*********************************************************************
// CAntennaPattern:: Destructor
cAntennaPattern::~cAntennaPattern()
{
	delete [] mAziAngles;
	delete [] mAziValues;
	delete [] mElevAngles;
	delete [] mElevValues;
	delete_Float2DArray(mAntPattern);
	delete [] mFile;
	delete [] mName;
	delete [] mMake;
	delete [] mDescription;
	delete [] mPol; 
	if (mUseANN) mAntennasANN->destroy();
}/* end CAntennaPattern:: Destructor */

// ********************************************************************
// Set antenna pattern file
bool cAntennaPattern::SetAntennaPattern(int Key, eAnt Type, 
																					double Bearing, double MechTilt)
{	

	if (Type ==Mobile)
		mUseANN = false;

	pqxx::result r;

	string QueryResult, query, filename;
	string Result;
	unsigned NumBytes;
	unsigned i,k,l;
//	int DeviceKey;
	unsigned AntKey;
	char* Temp;
	Temp = new char[12];

//	cout << "cAntennaPattern::SetAntennaPattern. Before if (mUseANN) " << endl;
	if (mUseANN)
	{
//		Bearing and MechTilt is not yet affected for ANNantenna patterns
		query = "select filename from AntNeuralNet cross join radioinstallation ";
		query += "where radid=radioinstallation.id and radioinstallation.id = ";
		gcvt(Key, 8, Temp);
		query += Temp;
		query += ";";	

		if(!gDb.PerformRawSql(query))
		{
				string err = "cAntennaPattern::SetAntennaPattern: Query to antenna ANN file failed. Failed query: ";
				err+=query;
				cout << err << endl;
//				QRAP_WARN(err.c_str());
				mUseANN = false;
		}
		else
		{
			gDb.GetLastResult(r);
			if(r.size()>0)
			{
				filename = r[0]["filename"].c_str();
//				cout << "cAntennaPattern::SetAntennaPattern: file name = " << filename << endl;
//				mAntennasANN->destroy();
				mAntennasANN->create_from_file(filename);
				mGain = 0;
			}
			else
			{
				cout << "Antenna ANN not found for radioinstallation " << Temp;
				cout << "	Antenna pattern in supplied file is used instead."  << endl;
				mUseANN = false;
			}
		}
		delete [] Temp;
		return true;
	}
	
//	cout << "cAntennaPattern::SetAntennaPattern. Na if (mUseANN) " << endl;
//	cout << "RadKey = "	<< Key << "	Bearing = " << Bearing << "	Tilt = " << MechTilt << endl;

	int pp, ss, sign; 	//point position
	float *TempAziAngles;
	float *TempElevAngles;
	float *TempAgterAngles;
	float *TempAgterValues;
	unsigned *TempAgterRef;
	unsigned *TempRef_mAzi;
	unsigned *TempRef_mElev;
	unsigned I_swap;
	float A_swap, V_swap; 
	
	mBearing = Bearing;
	mMechTilt = MechTilt;

	query = "select antdevicekey, antennapattern.id as antid, Frequency, Gain, AziBeamwidth, patternFile ";
	if (Type	==Mobile )
	{
		query += "from mobile cross join antennapattern where " ;
		query += "mobile.antpatternkey = AntennaPattern.id ";
 		query += "and mobile.id = ";
		cout << "Mobile Antenna" << endl;
	}
	else
	{
		query+=" FROM AntennaPattern cross join radioinstallation WHERE " ;
		query += "AntennaPattern.ID = radioinstallation.";
		if (Type==Tx)
			query += "tx";
		else
			query += "rx";
		query += "antpatternkey AND radioinstallation.ID = ";
	}
	gcvt(Key, 8, Temp);
	query += Temp;
	query += ";";

	cout << query << endl;	
//	cout << "cAntennaPattern::SetAntennaPattern. Query prepared. Before run query " << endl;

	if(!gDb.PerformRawSql(query))
	{
		string err = "Get AntennaPattern Parameters Failed. Query Failed: ";
		err+=query;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		return false;
	}
	else
	{
//		cout << "cAntennaPattern::SetAntennaPattern. No Err " << endl;
		gDb.GetLastResult(r);
		if(r.size()>0)
		{
//			cout << "cAntennaPattern::SetAntennaPattern. Size > 0 " << endl;
//			DeviceKey = atoi(r[0]["antdevicekey"].c_str());
//			cout << "cAntennaPattern::SetAntennaPattern. Before AntID " << endl;
			AntKey = atoi(r[0]["antid"].c_str());
			mFreq = atof(r[0]["frequency"].c_str());
			mGain = atof(r[0]["gain"].c_str());
			mBeamW = atof(r[0]["azibeamwidth"].c_str());
			strcpy(mFile, r[0]["patternFile"].c_str());
//			cout << mFile << endl;
		}
		else
		{
			string err = "AntennaPattern Entry not found, assuming zero gain. Empty Query: ";
			err += query;
			cout << err << endl;
			QRAP_WARN(err.c_str());
			return false;
		}
	}
	
//	cout << "After got basic info. RadKey := " << Key << endl;
	
	string CmdStr;
//   cout << "After declaring CmdStr"  << endl;

	CmdStr = "SELECT numazipoints, azimuthangles, azimuthpattern,";
	CmdStr+=" numelevpoints, elevationangles, elevationpattern ";
	CmdStr+="FROM AntennaPattern WHERE ID =";

   cout << "After setting basic CmdStr"  << endl;
 
	gcvt(AntKey, 8, Temp);
//	cout << Temp << endl;
	CmdStr += Temp;
	CmdStr += ";";
	
	cout << CmdStr << endl;
	if(!gDb.PerformRawSql(CmdStr))
	{
		string err = "Get AntennaPattern Failed. Query Failed: ";
		err+=CmdStr;
		cout << err << endl;
		QRAP_ERROR(err.c_str());
		return false;
	}
	else
	{
		cout << "Before get Query results " << endl;
		gDb.GetLastResult(r);
		if(r.size()!=0)
		{
			cout << "Before get mNAA " << endl;
			mNAA = atoi(r[0]["numazipoints"].c_str());
			cout << "mNAA = " << mNAA << endl;
			delete [] mAziAngles;
			delete [] mAziValues;
			mAziAngles = new float[mNAA];
			mAziValues = new float[mNAA];
			QueryResult=r[0]["azimuthangles"].c_str();
			NumBytes = QueryResult.length();
			i=1;
			k=0;
			sign = 1;
			// \TODO: I am sure there are more elegant ways, but for now it will do
			while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<mNAA))
			{
				mAziAngles[k] = 0;
				pp = 0;
				ss = 0;
				sign = 1;
				while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
				{
					if (QueryResult[i]=='-') 
						sign = -1;
					else if (QueryResult[i]!='.')
					{
						mAziAngles[k] = mAziAngles[k]*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;
					i++;	
				}
				if (pp) mAziAngles[k]/=exp10(ss-pp);
				mAziAngles[k]*=(double)sign;
				i++;
				k++;
			}	
			QueryResult=r[0]["azimuthpattern"].c_str();
			NumBytes = QueryResult.length();
			i=1;
			k=0;
			while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<mNAA))
			{
				mAziValues[k] = 0;
				pp = 0;
				ss = 0;
				sign =1;
				while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
				{
					if (QueryResult[i]=='-')
						sign =-1;
					else if (QueryResult[i]!='.')
					{
						ss++;
						mAziValues[k] = mAziValues[k]*10 + (QueryResult[i]-'0');
					}
					else pp = ss;
					i++;	
				}
				if (pp) mAziValues[k]/=exp10(ss-pp);
				mAziValues[k]*=(double)sign;
				i++;
				k++;
			}	
		
			mNEA = atoi(r[0]["numelevpoints"].c_str());
			delete [] mElevAngles;
			delete [] mElevValues;
			mElevAngles = new float[mNEA];
			mElevValues = new float[mNEA];
			QueryResult=r[0]["elevationangles"].c_str();
			NumBytes = QueryResult.length();
			i=1;
			k=0;
			while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<mNEA))
			{
				mElevAngles[k] = 0;
				pp = 0;
				ss = 0;
				sign = 1;
				while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
				{
					if (QueryResult[i]=='-') 
						sign =-1;
					else if (QueryResult[i]!='.')
					{
						mElevAngles[k] = mElevAngles[k]*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;
					i++;	
				}
				if (pp) mElevAngles[k]/=exp10(ss-pp);
				mElevAngles[k]*=(double)sign;
				i++;
				k++;
				
			}	
			QueryResult=r[0]["elevationpattern"].c_str();
			NumBytes = QueryResult.length();
			i=1;
			k=0;
			while ((i<NumBytes)&&(QueryResult[i]!='}')&&(k<mNEA))
			{
				mElevValues[k] = 0;
				pp = 0;
				ss = 0;
				sign =1;
				while ((QueryResult[i]!=',')&&(QueryResult[i]!='}'))
				{
					if (QueryResult[i]=='-')
						sign =-1;
					else if (QueryResult[i]!='.')
					{
						mElevValues[k] = mElevValues[k]*10 + (QueryResult[i]-'0');
						ss++;
					}
					else pp = ss;
					i++;	
				}
				if (pp) mElevValues[k]/=exp10(ss-pp);
				mElevValues[k]*=(double)sign;
				i++;
				k++;
			}
		} // if r.size()
		else
		{
			// \TODO: Messages
			cout << "There was no antennaPattern, using default isotropic" << endl;
			cout << "Empty Query: " << CmdStr << endl;
			return false;	
		} // else r.size()
	}//else !gDb->PerformRawSql(CmdStr1)

	cout << "After read database" << endl;
	
	// Prepare angular values for mask
	TempAziAngles = new float[mNAA+3];
	TempRef_mAzi = new unsigned[mNAA+3];
	for (k=0;k<mNAA+3;k++)
	{
		TempRef_mAzi[k]=9999;
		TempAziAngles[k]=9999.0;
	}
	TempAziAngles[0] = 0.0;
	TempAziAngles[1] = 180.0;
	TempAziAngles[2] = 360.0;
	for (k=0; k<mNAA; k++)
	{
		if (mAziAngles[k]<0) mAziAngles[k]+=360.0;
		else if (mAziAngles[k]>360) mAziAngles[k]-=360.0;
		if (fabs(mAziAngles[k]-0.0)<0.2) 
			TempRef_mAzi[0]=k;
		else if (fabs(mAziAngles[k]-180.0)<0.2)
			TempRef_mAzi[1]=k;
		else if (fabs(mAziAngles[k]-360.0)<0.2)
			TempRef_mAzi[2]=k;
		else
		{
			 TempAziAngles[k+3]=mAziAngles[k];
			 TempRef_mAzi[k+3]=k;
		}
	}
	for (k=mNAA;k<mNAA+3;k++)
	{
		if (TempRef_mAzi[k]==9999) 
			if (mNAA>=3) 
			{	
				TempAziAngles[k]=mAziAngles[k-3] ;
				TempRef_mAzi[k]=k-3;
			}
	}		
	//  if no entry was found for 360 degrees make it equal to that of 0 degrees
	if (TempRef_mAzi[2]>=mNAA) 
		TempRef_mAzi[2]=TempRef_mAzi[0];
	//  if no entry was found for 0 degrees make it equal to that of 360 degrees
	if (TempRef_mAzi[0]>=mNAA) 
		TempRef_mAzi[0]=TempRef_mAzi[2];

	for (k=0;k<mNAA+3;k++)
	{
		for (l=k+1;l<mNAA+3;l++)
		{
			if ((fabs(TempAziAngles[k]-TempAziAngles[l])<0.1)&&(TempRef_mAzi[k]<mNAA+3))//merge the two points
			{
//				cout << "Merge Azi " << k << "  " << TempAziAngles[k] << "    " << l << "  " << TempAziAngles[l] << endl;
				mAziValues[TempRef_mAzi[k]]= (mAziValues[TempRef_mAzi[k]]+mAziValues[TempRef_mAzi[l]])/2.0;
				// given that we are dealing with negative log10 values this is probably WRONG!!
				TempRef_mAzi[l]=9999;
				TempAziAngles[l]=9999.0;
			}
			else if (TempAziAngles[l]<TempAziAngles[k])	//swap
			{
				A_swap = TempAziAngles[l];
				I_swap = TempRef_mAzi[l];
				TempAziAngles[l]=TempAziAngles[k];
				TempRef_mAzi[l]=TempRef_mAzi[k];
				TempAziAngles[k] = A_swap;
				TempRef_mAzi[k] = I_swap;
			}
		}
	}

	for (k=0; k<mNAA; k++)
	{
		mAziAngles[k]=TempAziAngles[k];		
		if (fabs(mAziAngles[k]-180.0)<0.1) mRef180az = k;
	}

/*
	for (k=0; k<mNAA+3; k++)
	{
		cout << k << ":  " << TempRef_mAzi[k] << "  h: " 
		<< TempAziAngles[k] << "  w: " << mAziValues[TempRef_mAzi[k]] << endl;
	}
	cout << endl;
*/

	TempElevAngles = new float[mNEA+3];
	TempRef_mElev = new unsigned[mNEA+3];
	TempAgterAngles = new float[mNEA+3]; 
	TempAgterValues = new float[mNEA+3];
	TempAgterRef = new unsigned[mNEA+3];
	for (k=0;k<mNEA+3;k++)
	{
		TempRef_mElev[k]=9999;
		TempElevAngles[k]=9999.0;
		TempAgterAngles[k]=9999.0;
		TempAgterValues[k]=9999.0;
		TempAgterRef[k]=9999;
	}
	TempElevAngles[0] = -90.0;
	TempElevAngles[1] = 0.0;
	TempElevAngles[2] = 90.0;
	TempAgterAngles[0] = -90.0;
	TempAgterAngles[1] = 0.0;
	TempAgterAngles[2] = 90.0;
	for (k=0; k<mNEA; k++)
	{
//		cout << k << "   " <<  mElevAngles[k] << endl;
		if (mElevAngles[k]<-90.0) mElevAngles[k]+=360.0;
		if (mElevAngles[k]>270.0) mElevAngles[k]-=360.0;
		if (fabs(mElevAngles[k]-0.0)<0.2) 
		{
			TempRef_mElev[1] = k;
//			cout << mElevAngles[k] << " ?= "<< mElevAngles[TempRef_mElev[1]] << endl;
		}
		else if (fabs(mElevAngles[k]-180.0)<0.2)
		{
			TempAgterRef[1] = k;
			TempAgterValues[1] = mElevValues[k];
		}
		else if (fabs(mElevAngles[k]-90.0)<0.2)
		{
			TempRef_mElev[2] = k;
			TempAgterRef[2] = k;
			TempAgterValues[2] = mElevValues[k]	;
		}
		else if ((fabs(mElevAngles[k]-270.0)<0.2)||(fabs(mElevAngles[k]+90.0)<0.2))
		{
			TempRef_mElev[0] = k;
			TempAgterRef[0] = k;
			TempAgterValues[0] = mElevValues[k]	;
		}
		else if ((mElevAngles[k]>90.0)&&(mElevAngles[k]<270.0))
		{
			TempElevAngles[k+3] = 180 - mElevAngles[k];
			TempRef_mElev[k+3] = 7777;
			TempAgterAngles[k+3]= 180 - mElevAngles[k]; 
			TempAgterRef[k+3] = k;
			TempAgterValues[k+3] = mElevValues[k]; 
		}
		else
		{
			 TempElevAngles[k+3] = mElevAngles[k];
			 TempRef_mElev[k+3] = k;
			 TempAgterAngles[k+3] = mElevAngles[k];
			 TempAgterRef[k+3]= 7777;
		}
	}

//************************?????????????????????????????????????????????	
 if (TempAgterRef[1]>mNEA)
	{
		TempAgterRef[1] = TempRef_mElev[1];
		TempAgterValues[1] = mAziValues[mRef180az];
//		cout << TempAgterValues[1] << endl;
	}


	for (k=0;k<mNEA+3;k++)
	{
		for (l=k+1;l<mNEA+3;l++)
		{
			if ((fabs(TempAgterAngles[k]-TempAgterAngles[l])<0.2)&&(TempAgterRef[k]<8888))
			{	//merge the two points
				if (TempAgterRef[k]==7777)
				{
					TempAgterRef[k]=TempAgterRef[l];
					TempAgterValues[k] = TempAgterValues[l];
					TempAgterRef[l]=9999;
					TempAgterAngles[l]=9999.0;
					TempAgterValues[l]=9999.0;
				}
				else if (TempAgterRef[l]==7777)
				{
					TempAgterRef[l]=9999;
					TempAgterAngles[l]=9999.0;
				 	TempAgterValues[l]=9999.0;
				}
				else
				{
					TempAgterValues[k]= (TempAgterValues[l]+TempAgterValues[l])/2.0;
					// given that we are dealing with negative log10 values this is probably WRONG!!
					TempAgterRef[l] = 9999;
					TempAgterAngles[l] = 9999.0;
					TempAgterValues[l]=9999.0;
				}
			}
			else if ((TempAgterAngles[l]<TempAgterAngles[k]))	//swap
			{
				A_swap = TempAgterAngles[l];
				I_swap = TempAgterRef[l];
				V_swap = TempAgterValues[l];
				TempAgterAngles[l] = TempAgterAngles[k];
				TempAgterRef[l] = TempAgterRef[k];
				TempAgterValues[l] = TempAgterValues[k];
				TempAgterAngles[k] = A_swap;
				TempAgterRef[k] = I_swap;
				TempAgterValues[k] = V_swap;
			}
		}
	}
	
	for (k=0;k<mNEA+3;k++)
	{
		for (l=k+1;l<mNEA+3;l++)
		{
			if ((fabs(TempElevAngles[k]-TempElevAngles[l])<0.2)&&(TempRef_mElev[k]<8888))	
			{	//merge the two points
				if (TempRef_mElev[k]==7777)
				{
					TempRef_mElev[k]=TempRef_mElev[l];
					TempRef_mElev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
				else if (TempRef_mElev[l]==7777)
				{
					TempRef_mElev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
				else
				{
					mElevValues[TempRef_mElev[k]]= (mElevValues[TempRef_mElev[k]]+mElevValues[TempRef_mElev[l]])/2.0;
					// \TODO given that we are dealing with negative log10 values this is probably WRONG!!
					TempRef_mElev[l]=9999;
					TempElevAngles[l]=9999.0;
				}
			}
			else if ((TempElevAngles[l]<TempElevAngles[k])||(TempRef_mElev[k]==9999))	//swap
			{
				A_swap = TempElevAngles[l];
				I_swap = TempRef_mElev[l];
				TempElevAngles[l]=TempElevAngles[k];
				TempRef_mElev[l]=TempRef_mElev[k];
				TempElevAngles[k]=A_swap;
				TempRef_mElev[k]=I_swap;
			}
		}
	}

	//????
	//Anders as oorspronklike kode
	//Maak ekstra (onnodige) waardes gelyk aan die laaste waarde
	for (k=mNEA;k<mNEA+3;k++)
	{
		if (TempRef_mElev[k]>8000) 
		{
			if (k-3>0)
			{	//Vir die waardes van die agterkant van die antenna as daar nie eintlik 'n ref is nie 
				if ((mElevAngles[k-3]>90.0)&&(mElevAngles[k-3]<270.0))
				{
					TempElevAngles[k] = 180 - mElevAngles[k-3];
					TempRef_mElev[k] = 7777;
					TempAgterAngles[k]= 180 - mElevAngles[k-3]; 
					TempAgterRef[k] = k-3;
					TempAgterValues[k] = mElevValues[k-3]; 
				}
				else
				{ //Voorkant
			 		TempElevAngles[k] = mElevAngles[k-3];
			 		TempRef_mElev[k] = k-3;
			 		TempAgterAngles[k] = mElevAngles[k-3];
			 		TempAgterRef[k]= 7777;
				}
			}
		}
	}


	
/*	for (k=0; k<mNEA+3; k++)
	{
		cout << k << ":  " << TempRef_mElev[k];
		if (TempRef_mElev[k]<mNEA+1)
		{
			cout << "  " << TempElevAngles[k] << "  ";
			cout << mElevValues[TempRef_mElev[k]];
		}
		cout << "  a: " << TempAgterRef[k] << "  ";
		if (TempAgterRef[k]< mNEA+1)
	    {
	    	cout << TempAgterAngles[k] << "  " << TempAgterValues[k]; 
	    }
	    cout << endl; 
	}
	cout << endl;
*/	
	k=0;
	while ((k<mNAA+3)&&((TempRef_mAzi[k]<9999)||TempAziAngles[k]<361))
		k++;
	mNAA = k;
	
	k=0;
	while ((k<mNEA+3)&&((TempRef_mElev[k]<9999)||TempElevAngles[k]<91))
		k++;
	mNEA = k;

//	cout << "cAntennaPattern::SetAntennaPattern:	"<< mNEA << "	" << TempRef_mElev[k] << "	" <<TempAziAngles[k]<< endl;
//	cout << "cAntennaPattern::SetAntennaPattern:	" << mNAA << "	" << TempRef_mAzi[k]<< "	" <<TempAziAngles[k] << endl;
	
/*	for (k=0;k<mNEA+3;k++)
		cout << k << "  " << TempRef_mElev[k] << endl;
	cout << "Azi " << mNAA << "  Elev " << mNEA << endl;
*/	
	delete [] mElevAngles;
	delete [] mAziAngles;
	mElevAngles = new float[mNEA];
	mAziAngles = new float[mNAA];
	delete_Float2DArray(mAntPattern); 
	mAntPattern = new_Float2DArray(mNAA,mNEA);
	for (k=0;k<mNAA;k++)
		for (l=0;l<mNEA; l++)
			mAntPattern[k][l]=9999.9;
	
	for (k=0; k<mNAA; k++)
	{
		mAziAngles[k]=TempAziAngles[k];
		if (fabs(mAziAngles[k]-180.0)<0.1) mRef180az = k;
	}
	
	for (k=0; k<mNEA; k++)
	{
		mElevAngles[k]=TempElevAngles[k];
		if (fabs(mElevAngles[k]-0.0)<0.1) mRef0el = k;
	}
	
	for (k=0; k<mNAA; k++) //populate pattern at elevation -90, 0 and 90 
	{
		mAntPattern[k][mRef0el] = mAziValues[TempRef_mAzi[k]];
		mAntPattern[k][0] = mElevValues[TempRef_mElev[0]];
		mAntPattern[k][mNEA-1] = mElevValues[TempRef_mElev[mNEA-1]];
	}
	
	for (k=1; k<mNEA-1; k++)	//populate pattern at azimuth 0, 360
	{
		if (k!=mRef0el)
		{
			mAntPattern[0][k]= mElevValues[TempRef_mElev[k]];
			mAntPattern[mNAA-1][k]= mElevValues[TempRef_mElev[k]];
		}
	}

 	for (k=1; k<mNEA-1; k++)	//populate pattern at azimuth 180
	{
		if (k!=mRef0el)
		{
			if (TempAgterValues[k]<7777.0)
			{
				mAntPattern[mRef180az][k]= TempAgterValues[k];
//				cout << " Normal Agter value = " << mAntPattern[mRef180az][k] << endl;
			}
			else 
			{
				mAntPattern[mRef180az][k] = mAntPattern[mRef180az][mRef0el]+mAntPattern[0][k];
//				cout << " >7777 Agter value = " << mAntPattern[mRef180az][k] << endl;
			}
		}
	}

	
//	cout << mElevValues[TempRef_mElev[0]] << endl;
//	cout << mAntPattern[mRef180az][mRef0el] << endl;

/*	for (k=0; k<mNAA; k++)
	{
		cout << k << " k- "  ;
		for (l=0; l<mNEA; l++)
			cout << "  " << mAntPattern[k][l];
		cout << endl;
	}
	cout << endl;
	cout << "180az  "<< mRef180az << " 0el  " << mRef0el << endl;
*/	
	delete [] mElevValues;
	delete [] mAziValues;
	delete [] TempAgterValues;
	delete [] TempAgterAngles;
	delete [] TempRef_mElev;
	delete [] TempAgterRef;
	delete [] TempElevAngles;
	delete [] TempAziAngles;
	delete [] TempRef_mAzi;
	
	mElevValues = new float[mNEA];
	mAziValues = new float[mNAA];
	
//  populate remaining values
//	cout << "Azimuth" << endl;
//	cout << "Azimuth, Value" << endl;
	for (k=0; k<mNAA; k++)
	{
		mAziValues[k] = mAntPattern[k][mRef0el];
//		cout << mAziAngles[k] << ", " << mAziValues[k]<< endl;
	}
//	cout << "Elevation" << endl;
//	cout << "Tilt, Value, Agter" << endl;
	for (l=0; l<mNEA; l++)
	{
		mElevValues[l] = mAntPattern[0][l];
//		cout << mElevAngles[l] << ", " << mElevValues[l] << ", " << mAntPattern[mRef180az][l] << endl;
	}

	double ** corners;
	double DeltaAzi, DeltaEl; 
	corners = new double*[3];
	for (i=0;i<3; i++)
		corners[i] = new double[3];

	if (mAntPattern[0][0]>1000)
		mAntPattern[0][0]=mAntPattern[1][0];
	if (mAntPattern[0][mRef0el]>1000)
		mAntPattern[0][mRef0el]=mAntPattern[1][mRef0el];
	if (mAntPattern[0][mNEA-1]>1000)
		mAntPattern[0][mNEA-1]=mAntPattern[1][mNEA-1];
	if (mAntPattern[mRef180az][0]>1000)
		mAntPattern[mRef180az][0]=(mAntPattern[mRef180az-1][0]+mAntPattern[mRef180az+1][0])/2;
	if (mAntPattern[mRef180az][mRef0el]>1000)
		mAntPattern[mRef180az][mRef0el] = (mAntPattern[mRef180az+1][mRef0el] + mAntPattern[mRef180az-1][mRef0el])/2; 
	if (mAntPattern[mRef180az][mNEA-1]>1000)
		mAntPattern[mRef180az][mNEA-1] = (mAntPattern[mRef180az-1][mNEA-1] + mAntPattern[mRef180az+1][mNEA-1])/2;
	if (mAntPattern[mNAA-1][0] > 1000)
		mAntPattern[mNAA-1][0] = mAntPattern[mNAA-2][0];
	if (mAntPattern[mNAA-1][mRef0el]>1000)
		mAntPattern[mNAA-1][mRef0el] = mAntPattern[mNAA-2][mRef0el];
	if (mAntPattern[mNAA-1][mNEA-1]>1000)
		mAntPattern[mNAA-1][mNEA-1] = mAntPattern[mNAA-2][mNEA-1];

/*
	corners[0][0]=exp10(-mAntPattern[0][0]/10);
	corners[0][1]=exp10(-mAntPattern[0][mRef0el]/10);
	corners[0][2]=exp10(-mAntPattern[0][mNEA-1]/10);
	corners[1][0]=exp10(-mAntPattern[mRef180az][0]/10);
	corners[1][1]=exp10(-mAntPattern[mRef180az][mRef0el]/10);
	corners[1][2]=exp10(-mAntPattern[mRef180az][mNEA-1]/10);
	corners[2][0]=exp10(-mAntPattern[mNAA-1][0]/10);
	corners[2][1]=exp10(-mAntPattern[mNAA-1][mRef0el]/10);
	corners[2][2]=exp10(-mAntPattern[mNAA-1][mNEA-1]/10);
*/

	corners[0][0]=mAntPattern[0][0];
	corners[0][1]=mAntPattern[0][mRef0el];
	corners[0][2]=mAntPattern[0][mNEA-1];
	corners[1][0]=mAntPattern[mRef180az][0];
	corners[1][1]=mAntPattern[mRef180az][mRef0el];
	corners[1][2]=mAntPattern[mRef180az][mNEA-1];
	corners[2][0]=mAntPattern[mNAA-1][0];
	corners[2][1]=mAntPattern[mNAA-1][mRef0el];
	corners[2][2]=mAntPattern[mNAA-1][mNEA-1];

/*	for (i=0;i<3; i++)
		for (k=0; k<3; k++)
			cout << i << "	"<< k << "		" << (corners[i][k]) << endl;
*/
	double tydelikBO, tydelikON, tydelikL, tydelikR, tydelikV, tydelikH, Value, minEl, minAzi;

//There are 2 valid approximations for mAntPattern[k][l] namely tydelikV and tydelikH
// The question is how to combine them?


	for (k=1; k<mNAA-1; k++)
	{
		for (l=1; l<mNEA-1; l++)
		{ 
			if ((mAntPattern[k][l]>3000.0))
			{
				Value =0.0;
				if (l<mRef0el)
				{
					tydelikBO = exp10(-mAntPattern[k][0]/10);
					tydelikON = exp10(-mAntPattern[k][mRef0el]/10);
					DeltaEl =  90 + mElevAngles[l];
				}
				else
				{
					tydelikBO = exp10(-mAntPattern[k][mRef0el]/10);
					tydelikON = exp10(-mAntPattern[k][mNEA-1]/10);
					DeltaEl = mElevAngles[l];
				}
				tydelikV = tydelikBO+DeltaEl*(tydelikON-tydelikBO)/90;
				if (DeltaEl>45)
					minEl = 90-DeltaEl;
				else minEl=DeltaEl;

				if (k<mRef180az)
				{ 	
					tydelikL = exp10(-mAntPattern[0][l]/10);
					tydelikR = exp10(-mAntPattern[mRef180az][l]/10);
					DeltaAzi = mAziAngles[k];
				}
				else
				{ 
					tydelikL = exp10(-mAntPattern[mRef180az][l]/10);
					tydelikR = exp10(-mAntPattern[mNAA-1][l]/10);
					DeltaAzi = mAziAngles[k] -180;
				}
				tydelikH = tydelikL+DeltaAzi*(tydelikR-tydelikL)/180;
				if (DeltaAzi>90)
					minAzi = 180-DeltaAzi;
				else minAzi=DeltaAzi;

				Value = ((180-minAzi)*tydelikH/180 + minAzi*tydelikV/180 + (90-minEl)*tydelikV/90 + minEl*tydelikH/90)/2 ;
	
				mAntPattern[k][l] = -10.0*log10(Value);
			} // end if
		} // end for step through elevation angles
	} // end for step through Azimuth angles


/*
	for (k=1; k<mNAA-1; k++)
	{
		for (l=1; l<mNEA-1; l++)
		{ 
			if ((mAntPattern[k][l]>3000.0))
			{
				if (l<mRef0el)
				{
					tydelikBO = mAntPattern[k][0];
					tydelikON = mAntPattern[k][mRef0el];
					DeltaEl =  - mElevAngles[l];
				}
				else
				{
					tydelikBO = mAntPattern[k][mRef0el];
					tydelikON = mAntPattern[k][mNEA-1];
					DeltaEl = 90 - mElevAngles[l];
				}
				tydelikV = tydelikON+DeltaEl*(tydelikBO-tydelikON)/90;
//				cout <<"V: " << tydelikV << "	BO:" << tydelikBO << "	ON: " << tydelikON << "		ElevAngles: "	<< mElevAngles[l] << "	DeltaEl:	" << DeltaEl << endl;

				if (fabs(DeltaEl)>45)
					minEl = 90-fabs(DeltaEl);
				else minEl=fabs(DeltaEl);

				if (k<mRef180az)
				{ 	
					tydelikL = mAntPattern[0][l];
					tydelikR = mAntPattern[mRef180az][l];
					DeltaAzi = mAziAngles[k];
				}
				else
				{ 
					tydelikL = mAntPattern[mRef180az][l];
					tydelikR = mAntPattern[mNAA-1][l];
					DeltaAzi = mAziAngles[k] -180;
				}
				tydelikH = tydelikL+DeltaAzi*(tydelikR-tydelikL)/180;
				if (fabs(DeltaAzi)>90)
					minAzi = 180-fabs(DeltaAzi);
				else minAzi=fabs(DeltaAzi);
//				cout << "H: "<< tydelikH << "	R:" << tydelikR << "	L: " << tydelikL << "		AziAngles: "	<< mAziAngles[k] << "	DeltaAzi:	" << DeltaAzi << endl;

				Value = ((180-minAzi)*tydelikH/180 + minAzi*tydelikV/180 + (90-minEl)*tydelikV/90 + minEl*tydelikH/90)/2 ;

//				cout << "Value: "<< Value << "	minAzi:" << minAzi << "	minEl" << minEl << endl;
	
				mAntPattern[k][l] = Value;
			} // end if
		} // end for step through elevation angles
	} // end for step through Azimuth angles
*/

/*
	for (k=0; k< mNAA; k++)
	{
		cout << k << " k:: ";
		for (l=0; l<mNEA; l++)
			cout << "   " << mAntPattern[k][l] << ",";
		cout << endl;
	}	
*/
	delete [] Temp;
	return 1;
} // End SetAntennaPattern


//***********************************************************************
double cAntennaPattern::GetPatternValue(double Azimuth, double Elevation)
{
	
	if (mUseANN)
	{
		double *AntANNInput;
		AntANNInput = new double[5];
		double *AntANNValue;
		AntANNValue = new double[1];
		double AntValue;
		AntANNInput[0]  = 1;
		AntANNInput[1] = cos(Azimuth*PI/180);
		AntANNInput[2] = sin(Azimuth*PI/180);
		AntANNInput[3] = cos(Elevation*PI/180);
		AntANNInput[4] = sin(Elevation*PI/180);
//		cout << "cAntennaPattern::GetPatternValue: Voor using ANN.";
//		cout << " Azimuth =  " << Azimuth << "	Elevation = " << Elevation;
		AntANNValue = mAntennasANN->run(AntANNInput);
		AntValue = (AntANNValue[0]+0.5)*ANTENNASCALE;
//		cout << "	AntValue =  " <<  AntValue <<  endl;
		if (0==AntANNValue) delete [] AntANNValue;
		delete [] AntANNInput;
		return AntValue;
	}

//	cout << "Not antenna ANN" << endl;
	if ((mNAA>9990)||(mNEA>9990)
			||(Azimuth>500)||(Azimuth<-500)||(Elevation>180)||(Elevation<-180))
	{
		cout << "There seems to be a problem with the antenna pattern "
			<< mName << endl;
		cout << "Azi: " << Azimuth << "  Elev: " << Elevation << endl;  
		return 0;
	}

	float ValueAz1, ValueAz2,Value;
	float Az, Azz, El, Ell;
	float minDelta;
	unsigned ref_Azi, ref_El, kk, ll;
	
	Az = Azimuth - mBearing;

	if (Az>=360.0) Az-=360.0;
	else if (Az<0.0) Az+=360.0;
	
	El = Elevation;
	El = El - cos(Az*PI/180)*mMechTilt;

	if (El<-90.0) El+=360.0;
	else if (El>270.0) El-=360.0;
	if (El>90.0)
	{ 
		Azz = 180.0 - Az;
		Ell = 180.0 - El;
		Az = Azz;
		El = Ell;
	}
	else if (El<-90.0)
	{
		Azz = 180.0 - Az;
		Ell = -180.0 - El;
		Az = Azz;
		El = Ell;
	}

	ref_Azi=0;
	minDelta = 360.0;
	for (kk=0;(kk<mNAA); kk++)
	{
		if (fabs(Az-mAziAngles[kk])<minDelta)
		{
			ref_Azi=kk;
			minDelta = fabs(Az-mAziAngles[kk]);
		}
	}
	if ((Az-mAziAngles[ref_Azi])<0)
	{
		if (ref_Azi>0)  ref_Azi--;
		else 
		{
			cout << "Azi rap around" << endl;
			ref_Azi=mNAA-1; // Because the azimuth values rap around
		}
	}
	
	// What happens if value lies just before ref0??
	ref_El=0;
	minDelta = 360.0;
	for (ll=0;(ll<mNEA); ll++)
		if (fabs(El-mElevAngles[ll])<minDelta)
		{
			ref_El=ll;
			minDelta = fabs(El-mElevAngles[ll]);
		}
	if ((El-mElevAngles[ref_El])<0)
	{
		if (ref_El>0) ref_El--;
		else ref_El=mNEA-1;
	}
	
	if (ref_El>mNEA-2)
	{
		cout << "El_ref: " << ref_El << endl; 
		if (ref_Azi>mNAA-2)
		{
			ValueAz1 = mAntPattern[ref_Azi][ref_El];
			ValueAz2 = mAntPattern[0][ref_El];
			Value = ValueAz1 + (Az-mAziAngles[ref_Azi])*(ValueAz2-ValueAz1)
						/(360+mAziAngles[0]-mAziAngles[ref_Azi]);
		}
		else
		{
			ValueAz1 = mAntPattern[ref_Azi][ref_El];
			ValueAz2 = mAntPattern[ref_Azi+1][ref_El];
			Value = ValueAz1 + (Az-mAziAngles[ref_Azi])*(ValueAz2-ValueAz1)
						/(mAziAngles[ref_Azi+1]-mAziAngles[ref_Azi]);
		}
	}
	else if (ref_Azi>mNAA-2)
	{
//		cout<< "Az="<< Az << "	Azi_ref=" << ref_Azi;
//		cout << "	El_ref=" << ref_El << endl; 
//		cout << "Azi_ref: " << ref_Azi <<  endl; 
		ValueAz1 = mAntPattern[ref_Azi][ref_El];
		Value = ValueAz1 + (El-mElevAngles[ref_El])
				*(mAntPattern[ref_Azi][ref_El+1]-ValueAz1)
				/(mElevAngles[ref_El+1]-mElevAngles[ref_El]);
/*			ValueAz1 = mAntPattern[ref_Azi][ref_El];
			ValueAz1 = ValueAz1 + (El-mElevAngles[ref_El])
									*(mAntPattern[ref_Azi][ref_El+1]-ValueAz1)
									/(mElevAngles[ref_El+1]-mElevAngles[ref_El]);
			ValueAz2 = mAntPattern[0][ref_El];
			ValueAz2 = ValueAz2 + (El-mElevAngles[ref_El])
									*(mAntPattern[0][ref_El+1]-ValueAz2)
										/(mElevAngles[ref_El+1]-mElevAngles[ref_El]);
			Value = ValueAz1 + (Az-mAziAngles[ref_Azi])*(ValueAz2-ValueAz1)
						/(360+mAziAngles[0]-mAziAngles[ref_Azi]);
*/
	}
	else
	{
		ValueAz1 = mAntPattern[ref_Azi][ref_El];
		ValueAz1 = ValueAz1 + (El-mElevAngles[ref_El])
				*(mAntPattern[ref_Azi][ref_El+1]-ValueAz1)
				/(mElevAngles[ref_El+1]-mElevAngles[ref_El]);
		ValueAz2 = mAntPattern[ref_Azi+1][ref_El];
		ValueAz2 = ValueAz2 + (El-mElevAngles[ref_El])
				*(mAntPattern[ref_Azi+1][ref_El+1]-ValueAz2)
				/(mElevAngles[ref_El+1]-mElevAngles[ref_El]);
		Value = ValueAz1 + (Az-mAziAngles[ref_Azi])*(ValueAz2-ValueAz1)
						/(mAziAngles[ref_Azi+1]-mAziAngles[ref_Azi]);
	}
	return Value;
}
