 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBTLPredict.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Respresents a Basic Transmission Loss Prediction.
 *                : contained by each cCoveragePredict. 
 *                : Calls cPathLoss repeatedly
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

#include "cBTLPredict.h"

using namespace Qrap;

//*********************************************************************
// CBTL Default constructer
cBTLPredict::cBTLPredict()
{
	// \ TODO Check for Windows compatibility
	mArraysPopulated = false;
	mMaxPathLossReached = false;
	mSiteID=1; 
	mAngleRes=1;
	mNumAngles=360;
	mDistanceRes=200;
	mRange=35000;	
	mNumRadialPoints=(int)trunc(mRange/mDistanceRes +1.0);
	mFrequency=900.0;
	mkFactor=1.33;	
			
	mFixedHeight=15.0;
	mMobileHeight=1.0;
	mDTMsource=1;
	mClutterSource=1;
	mBTL = new_Float2DArray(mNumAngles,mNumRadialPoints);
	mTilt = new_Float2DArray(mNumAngles,mNumRadialPoints);
	string setting = gDb.GetSetting("BTLDir");
	if(setting!="")
		mBTLdir=setting;
	else
		mBTLdir="/home/maggie/Data/RasterData/BTL/";
	mBTLfile="Default.btl"; 
}

//********************************************************************
//CBTL destructer
cBTLPredict::~cBTLPredict()
{
	delete_Float2DArray(mBTL);
	delete_Float2DArray(mTilt);
}

//*********************************************************************
bool cBTLPredict::GetBTL(	unsigned &NumAngles,
				unsigned &NumRadialPoints,
				double &Radius, // in meters
				double &DistanceRes) // in meters
{
	NumAngles = mNumAngles;
	mAngleRes = 360.0/mNumAngles;
	NumRadialPoints = mNumRadialPoints;
	Radius = mRange;
	DistanceRes = mDistanceRes;
	
	if (!mArraysPopulated) return mArraysPopulated;
	else return true;
}

//*********************************************************************
int cBTLPredict::Check_and_SetBTL(	int SiteID, 
					double &Radius, // in meters
					double &DistanceRes, // in meters
					unsigned &NumAngles, // input in degrees
					double &Frequency, // in MHz
					double &FixedHeight, // in meters
					double &MobileHeight,
					double &kFactor,
					int DTMsource, // key to source 
					int ClutterSource)
{
	bool sameASthis = ((mSiteID==SiteID)&&
			((mFixedHeight-FixedHeight)<(0.1*LIGSNEL/Frequency/1000000.0))&&
					((mMobileHeight-MobileHeight)<(0.1*LIGSNEL/Frequency/1000000.0))&&
					(fabs((mFrequency-Frequency)/Frequency)<0.1)&&
					(fabs((mkFactor-kFactor)/mkFactor)<0.001)&&
					((mRange>=Radius)||(mMaxPathLossReached))&&
					(mDistanceRes<=DistanceRes)&&
					(mNumAngles>=NumAngles)&&
					(mDTMsource==DTMsource)&&
					(mClutterSource==ClutterSource));
	
	if (!sameASthis)
	{
		mArraysPopulated = false;
		mSiteID=SiteID;
		mNumAngles = NumAngles;
		mAngleRes=360.0/mNumAngles;
		mDistanceRes=DistanceRes;	
		mNumRadialPoints=(int)ceil(Radius/mDistanceRes);
		mRange=Radius;
		mFrequency=Frequency;			
		mkFactor=kFactor;			
		mFixedHeight=FixedHeight;
		mMobileHeight=MobileHeight;			
		mDTMsource= DTMsource;	
		mClutterSource=ClutterSource;
	}
	else
	{
		Radius=mRange;
		NumAngles = mNumAngles;
		DistanceRes=mDistanceRes;
		kFactor=mkFactor;
		Frequency=mFrequency;
		FixedHeight=mFixedHeight;
		MobileHeight=mMobileHeight;
		cout << "cBTLPredict::Check_and_Set: sameASthis .." << endl; 
		cout << "	Freq: " << mFrequency;
		cout << "	k: " << mkFactor;
		cout << "	F_height: " << mFixedHeight;
		cout << "	M_height: " << mMobileHeight << endl;
		if (mArraysPopulated)
			return mBTLid;
	}
	
	// Perform query
	string query; 
	char * temp;
	temp = new char[33];
	query = "SELECT id,distanceres,numangles,radius,frequency,btlplot,maxr FROM btl ";
	query += "WHERE siteid =";
	gcvt(mSiteID,8,temp); query += temp;
	query += " AND ((radius >=";
	gcvt(mRange,8,temp); query += temp;
	query += ") OR (maxR=true)) AND numangles >= ";
	gcvt(mNumAngles,8,temp); query += temp;
	query += " AND distanceres <= ";
	gcvt(mDistanceRes,8,temp); query += temp;
	query += " AND frequency > ";
	gcvt(0.9*mFrequency,8,temp); query += temp;
	query += " AND frequency < "; 
	gcvt(1.1*mFrequency,8,temp); query += temp;
	query += " AND (ABS(kfactor - ";
	gcvt(mkFactor,8,temp); query += temp;
	query += " ) < 0.001) AND (ABS(antennaheight - "; 
	gcvt(mFixedHeight,8,temp); query += temp;
	query += ") < 0.3) AND dtmsource = ";
	gcvt(mDTMsource,8,temp); query += temp;
	query += " AND machineid="; 
	gcvt(gDb.globalMachineID,8,temp); query += temp;
	query += ";";
	
	cout << query << endl;
	// Perform the Raw SQL query
	if(!gDb.PerformRawSql(query))
	{
		string err = "Database Select on table btl failed: Query: ";
		err+=query;
		QRAP_ERROR(err.c_str());
		delete [] temp;
		return -1;
	} //if
	else
	{
		pqxx::result r;
		
		gDb.GetLastResult(r);
		if(r.size()!=0)
		{
			mBTLid = atoi(r[0]["id"].c_str());
			mNumAngles = atoi(r[0]["numangles"].c_str());
			mAngleRes = 360.0/(double)mNumAngles;
			mDistanceRes = atof(r[0]["distanceres"].c_str());
			mRange = atof(r[0]["radius"].c_str());
			Radius = mRange;
			string text = r[0]["maxr"].c_str();
			mMaxPathLossReached = (text[0]='T')||(text[0]='t');
			mNumRadialPoints = (int)floor(mRange/mDistanceRes+0.5);
			mFrequency = atof(r[0]["frequency"].c_str());
			mBTLfile = string(r[0]["btlplot"].c_str());
			DistanceRes=mDistanceRes;
			kFactor=mkFactor;
			Frequency=mFrequency;
			FixedHeight=mFixedHeight;
			MobileHeight=mMobileHeight;
			cout << "cBTLPredict::Check_and_Set 2:"; 
			cout << "	Freq: " << mFrequency;
			cout << "	k: " << mkFactor;
			cout << "	F_height: " << mFixedHeight;
			cout << "	M_height: " << mMobileHeight << endl;
// \ TODO: This is again probably not windows ready			
			mArraysPopulated = ReadBTL();
			// Successfully got the data
			delete [] temp;
			return mBTLid;
		} // if size
 		else 
		{
			delete [] temp;
			return -1;
		}
	} // else
}

//*********************************************************************
bool cBTLPredict::ReadBTL()
{
	ifstream BTLbin, TLTbin;
	delete_Float2DArray(mBTL);
	delete_Float2DArray(mTilt);
	mBTL = new_Float2DArray(mNumAngles,mNumRadialPoints);
	mTilt = new_Float2DArray(mNumAngles,mNumRadialPoints);
	
	int ll = mBTLfile.length();
	unsigned i=ll-7;
	mBTLfile[i]='B'; mBTLfile[i+1]='T'; mBTLfile[i+2]='L'; 
	cout << mBTLfile << endl;
	BTLbin.open(mBTLfile.c_str(), ios::in | ios::binary);
	if (!BTLbin)
	{
		string err = "Unable to open file: ";
		err+=mBTLfile;
		// \TODO: remove from database file file connot be opened
		QRAP_ERROR(err.c_str());
		return false;
	}
	else
	{
		for (i=0; i<mNumAngles; i++)
			BTLbin.read((char *)mBTL[i],mNumRadialPoints*sizeof(float));
		BTLbin.close();
	}
	i = ll-7;
	mBTLfile[i]='T'; mBTLfile[i+1]='L'; mBTLfile[i+2]='T';
	cout << mBTLfile << endl;
	TLTbin.open(mBTLfile.c_str(), ios::in | ios::binary);
	if (!TLTbin)
	{
		string err = "Unable to open file: ";
		err+=mBTLfile;
		// \TODO: remove from database file file connot be opened
		QRAP_ERROR(err.c_str());
	    return false;
	}
	else
	{
		for (i=0; i<mNumAngles; i++)
			TLTbin.read((char *)mTilt[i],mNumRadialPoints*sizeof(float));
		TLTbin.close();
	}
/*	for (i=0; i<mNumAngles; i++)
	{
		cout << "i: " << i;
		for (int j=0; j<mNumRadialPoints; j++)
			cout << "  B:" << mBTL[i][j] << "  T:" << mTilt[i][j]; 
		cout << endl;
	}
*/
	mArraysPopulated=true;
	return mArraysPopulated;
} // end ReadBTL
		
//*********************************************************************
int cBTLPredict::StoreBTL()
{
	char *tempc = new char[33];
	unsigned i,ll;
	int BTLkey;
	// \TODO get mBTLpath from GetSettings
	
	// \TODO this is not windows ready and it assumes that the directory is complete
	mBTLfile =mBTLdir;
	mBTLfile+="S";
	gcvt(mSiteID,8,tempc);
	mBTLfile += tempc;
	mBTLfile+="K";
	
	if (!gDb.PerformRawSql("select max(Id) from btl;"))
	{
		string err = "Database Select on table btl for net primkey failed: ";
		QRAP_ERROR(err.c_str());
		BTLkey=-1;
	}
	else
	{
		pqxx::result r;
		gDb.GetLastResult(r);
		BTLkey = atoi(r[0][0].c_str())+1;
		gcvt(BTLkey,8,tempc);
		mBTLfile+=tempc;
	}
	
/*	for (i=0; i<mNumAngles; i++)
	{
		cout << "i: " << i;
		for (int j=0; j<mNumRadialPoints; j++)
			cout << "  B:" << mBTL[i][j] << "  T:" << mTilt[i][j]; 
		cout << endl;
	}
*/	
	ll = mBTLfile.length();
	mBTLfile+="BTL.bin";
	ofstream BTLbin, TLTbin;
	BTLbin.open(mBTLfile.c_str(), ios::out | ios::binary);
	if (!BTLbin)
	{
		string err = "Unable to open file: ";
		err+=mBTLfile;
		QRAP_ERROR(err.c_str());
		return 0;
	}
	else
	{
		for (i=0; i<mNumAngles; i++)
			BTLbin.write((char *)mBTL[i],mNumRadialPoints*sizeof(float));
		BTLbin.close();
	}
	i =ll;
	mBTLfile[i]='T'; mBTLfile[i+1]='L'; mBTLfile[i+2]='T'; 
	TLTbin.open(mBTLfile.c_str(), ios::out | ios::binary);
	if (!TLTbin)
	{
		string err = "Unable to open file: ";
		err+=mBTLfile;
		QRAP_ERROR(err.c_str());
		return 0;
	}
	else
	{	
		for (i=0; i<mNumAngles; i++)
			TLTbin.write((char *)mTilt[i],mNumRadialPoints*sizeof(float));
		TLTbin.close();
	}
	
/*	for (i=0; i<mNumAngles; i++)
	{
		cout << "i: " << i;
		for (unsigned j=0; j<mNumRadialPoints; j++)
			cout << "  B:" << mBTL[i][j] << "  T:" << mTilt[i][j]; 
		cout << endl;
	}
*/	
	string query;
	char *temp = new char[33];
	if (BTLkey==-1)
	{
		query = "insert into btl (lastmodified, machineid, siteid, dtmsource, cluttersource, antennaheight,";
		query += "mobileheight,frequency,kFactor,radius,numangles,distanceres, maxr, btlplot) values (now(),";
	}
	else
	{
		query = "insert into btl (lastmodified, id, machineid, siteid, dtmsource, cluttersource, antennaheight,";
		query += "mobileheight,frequency,kFactor,radius,numangles,distanceres, maxr, btlplot) values (now(),";
		gcvt(BTLkey,8,temp);		query += temp;	query += ',';
	}
	gcvt(gDb.globalMachineID,8,temp);	query += temp;	query += ',';
	gcvt(mSiteID,8,temp);			query += temp;	query += ',';
	gcvt(mDTMsource,8,temp); 		query += temp;	query += ',';
	gcvt(mClutterSource,8,temp); 	query += temp;	query += ',';
	gcvt(mFixedHeight,8,temp); 		query += temp;	query += ',';
	gcvt(mMobileHeight,8,temp); 	query += temp;	query += ',';
	gcvt(mFrequency,8,temp); 		query += temp;	query += ',';
	gcvt(mkFactor,8,temp); 			query += temp;	query += ',';
	gcvt(mRange,8,temp); 			query += temp;	query += ',';
	gcvt(mNumAngles,8,temp); 		query += temp;	query += ',';
	gcvt(mDistanceRes,8,temp); 		query += temp;	query += ',';
	if (mMaxPathLossReached) query +="true";
	else query+="false";
	query += ",'"+mBTLfile +"');";

	// Perform the Raw SQL query
	if(!gDb.PerformRawSql(query))
	{
		string err = "Database Insert on table btl failed: Query: ";
		err += query;
		QRAP_ERROR(err.c_str());
		return -1;
	} //if
	else
	{
		mBTLid = BTLkey;
		delete [] temp;
		delete [] tempc;
		return mBTLid;
	}
}

//*********************************************************************
int cBTLPredict::PredictBTL(unsigned NumAngles, unsigned NumDistance, 
				double DistanceRes, Float2DArray DTM, 
				bool UseClutter, unsigned ClutterClassGroup, Float2DArray Clutter)
{
	double DiffLoss=0;
	int MaxRadialP=0;
	unsigned i;
	int j;
	cPathLossPredictor PathLoss;
	mNumAngles=NumAngles;
	mAngleRes=360.0/(double)mNumAngles;
	mDistanceRes=DistanceRes;
	mNumRadialPoints=NumDistance;
	mRange=DistanceRes*NumDistance;	
	delete_Float2DArray(mBTL);
	delete_Float2DArray(mTilt);
	mBTL = new_Float2DArray(mNumAngles,mNumRadialPoints);
	mTilt = new_Float2DArray(mNumAngles,mNumRadialPoints);
	
	cProfile DTMProfile;
	cProfile ClutterProfile;
	DTMProfile.SetInterPixelDist(mDistanceRes);
	ClutterProfile.SetInterPixelDist(mDistanceRes);
	
	cout << "cBTLPredict::PredictBTL"; 
	cout << "	Freq: " << mFrequency;
	cout << "	k: " << mkFactor;
	cout << "	F_height: " << mFixedHeight;
	cout << "	M_height: " << mMobileHeight << endl;
	PathLoss.setParameters(mkFactor,mFrequency,mFixedHeight,mMobileHeight,UseClutter,ClutterClassGroup);
	for (i=0; i<NumAngles; i++)
	{	
		DTMProfile.SetProfile(mNumRadialPoints, DTM[i]);
		if (UseClutter)
			ClutterProfile.SetProfile(mNumRadialPoints, Clutter[i]);
		for (j=(NumDistance-1); j>0 ; j--)
		{
			mBTL[i][j] = PathLoss.TotPathLoss(DTMProfile, mTilt[i][j], ClutterProfile,DiffLoss);
			if ((mBTL[i][j]<-mMaxPathLoss)&&(j>MaxRadialP))
			{
				MaxRadialP = j;
				mMaxPathLossReached = true;	
			}
			if (UseClutter) ClutterProfile.ReduceSize();
			DTMProfile.ReduceSize();
		}
		if (((double)i/20.0)==(i/20))
			cout << "cBTLPredict::PredictBTL. Prediction progress: " << 100.0*i/NumAngles << endl;
	}
	for (i=0;i<mNumAngles;i++)
	{
		mBTL[i][0] = mBTL[i][1];
		mTilt[i][0] = mTilt[i][1];
	}
	
	if ((MaxRadialP<(NumDistance-3))&&(mMaxPathLossReached))
	{
		mNumRadialPoints = MaxRadialP;
		mRange = MaxRadialP*mDistanceRes;
	}
	else if (mMaxPathLossReached) mMaxPathLossReached=false;
	mArraysPopulated = true; 
	return StoreBTL(); 
}
