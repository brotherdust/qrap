 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralLink.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow (magdaleen.ballot@up.ac.za)
 *    Description : Does the link for each potential interferer
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

#include "cSpectralLink.h"
#include <vector>
#include <sstream>

using std::vector;
using std::stringstream;

namespace Qrap
{
	
//**************************************************************************************
bool MakeArrayFromDB(vector<float> &result, string input)
{
	result.empty();
	size_t pos;
	if ((pos=input.find_first_of("{"))!=string::npos)
	{
		result.push_back(atof(input.c_str()+pos+1));
		while ((pos=input.find_first_of(",", pos+1))!=string::npos)
			result.push_back(atof(input.c_str()+pos+1));
	}
	else
	{
		cout<<"Input does not contain a valid array"<<endl;
	}
}

//*******************************************************************************************
string MakeDBFromArray(vector<float> &input)
{
	stringstream content;
	content<<"{";
	vector<float>::iterator i=input.begin();
	content<<*i;
	++i;
	for(;i!=input.end();++i)
	{
		content<<","<<*i;
	}
	content<<"}";
	return content.str();
}

//**********************************************************************************************
cSpectralLink::cSpectralLink(cRasterFileHandler *Dem, unsigned int &RxKey, 
				unsigned int &TxKey, double &Resol, double &kFactor)
{
	mUseAntennaANN=false;
	string setting = gDb.GetSetting("UseAntennaANN");
	if (setting=="true")
		mUseAntennaANN=true;

	mUseClutter = mClutter.SetRasterFileRules(mClutterSource);
	if (mUseClutter) 
		mClutterClassGroup = mClutter.GetClutterClassGroup();
	if (mUseClutter) mClutterset.Reset(mClutterClassGroup);

	bool FoundRasterSet=FALSE;
	mFrequencySpacing=Resol;
	mTxInst.sInstKey=TxKey;
	mRxInst.sInstKey=RxKey;
	GetDBinfo(mRxInst);
	GetDBinfo(mTxInst);
	
	mEnvelopeFreq=NULL;
	mEnvelopeValue=NULL;
	
	mDEM=Dem;
	
	mMinClearance = 3.402823e+38;
	mUnits = dBm;
	mFrequency = 60000;
	mkFactor = kFactor;
	mDEMsource = 1;
	mClutterSource = 1;
//	mUseClutter = false;
	mPlotResolution = 90;
	
	FoundRasterSet = mDEM->SetRasterFileRules(mDEMsource);
	if (!FoundRasterSet)
	{
		string err = "Error getting ";
		err+=" DEM: Trouble getting DEM list. Using default";
		QRAP_WARN(err.c_str());
	}
	if (mUseClutter)
	{
		FoundRasterSet = mClutter.SetRasterFileRules(mClutterSource);
		mUseClutter = FoundRasterSet;
		if (!FoundRasterSet)
		{
			mUseClutter=false;
			string err = "Error fetching clutter ";
			err+=": Trouble getting Clutter list. Using default.";
			QRAP_WARN(err.c_str());
		}
	}
}

//******************************************************************************************
cSpectralLink::cSpectralLink(cRasterFileHandler *Dem)
{
	mDEM=Dem;
	
	mLength = 2;
	mSlope = 0.0;
	//mEffRadius = reR;
	mMinClearance = 3.402823e+38;
	mUnits = dBm;
	mFrequency = 60000;
	mkFactor = 1.33;
	mDEMsource = 1;
	mClutterSource = 1;
	mUseClutter = false;
	mPlotResolution = 10;
	mTxInst.sInstKey=491;
	mTxInst.sSiteID=1;
	mTxInst.sSitePos.Set(-25, 25);
	mTxInst.sFrequency=2400;
	mTxInst.sEIRP=33;
	mTxInst.sTxPower=2;
	mTxInst.sTxSysLoss=0;
	mTxInst.sRxSysLoss=0;
	mTxInst.sRxSens=-105;
	mTxInst.sTxPatternKey=1;
	mTxInst.sTxAzimuth=0;
	mTxInst.sTxMechTilt=0;
	mTxInst.sRxPatternKey=1;
	mTxInst.sRxAzimuth=0;
	mTxInst.sRxMechTilt=0;
	mTxInst.sDiversityGain=0;
	mTxInst.sTxHeight=10;
	mTxInst.sRxHeight=10;
	mTxInst.sRange=0;
	mTxInst.sFEdge=0;
	mRxInst.sBEdge=0;
	mRxInst.sInstKey=1;
	mRxInst.sSiteID=1;
	mRxInst.sSitePos.Set(-25, 25.5);
	mRxInst.sFrequency=2400;
	mRxInst.sEIRP=33;
	mRxInst.sTxPower=2;
	mRxInst.sTxSysLoss=0;
	mRxInst.sRxSysLoss=0;
	mRxInst.sRxSens=-105;
	mRxInst.sTxPatternKey=1;
	mRxInst.sTxAzimuth=0;
	mRxInst.sTxMechTilt=0;
	mRxInst.sRxPatternKey=1;
	mRxInst.sRxAzimuth=0;
	mRxInst.sRxMechTilt=0;
	mRxInst.sDiversityGain=0;
	mRxInst.sTxHeight=10;
	mRxInst.sRxHeight=1;
	mRxInst.sRange=0;
	mRxInst.sFEdge=0;
	mRxInst.sBEdge=0;

	mEnvelopeFreq=NULL;
	mEnvelopeValue=NULL;

	mFrequencySpacing=0.1;
}

//****************************************************************************************
cSpectralLink::~cSpectralLink()
{
	//cout<< " DESTROY SPECTRAL LINK "<<endl;
}

//******************************************************************************************
bool cSpectralLink::DoLink()
{
	float Tilt;
	double EIRP, LinkOtherGain, AntValue;
	cProfile Clutter;
	cProfile DEM;
	cPathLossPredictor PathLoss;
	bool AfterReceiver = (mUnits!=dBWm2Hz) && (mUnits!=dBWm2);

	mTxBearing = mTxInst.sSitePos.Bearing(mRxInst.sSitePos);
	if (mTxBearing < 180.0)
		mRxBearing = mTxBearing + 180.0;
	else
		mRxBearing = mTxBearing - 180.0;

	eAnt Which;
	if (mDownlink) Which = Tx;
	else Which = Rx;
	mTxAnt.SetAntennaPattern(mTxInst.sInstKey, mUseAntennaANN, Which, 
														mTxInst.sTxAzimuth, mTxInst.sTxMechTilt);
	if (mDownlink) Which = Rx;
	else Which = Tx;
	mRxAnt.SetAntennaPattern(mRxInst.sInstKey, mUseAntennaANN, Which, 
														mRxInst.sRxAzimuth, mRxInst.sRxMechTilt);
	EIRP = mTxInst.sTxPower - mTxInst.sTxSysLoss + mTxAnt.mGain;

	PathLoss.setParameters(mkFactor, mTxInst.sFrequency,
					mTxInst.sTxHeight, mRxInst.sRxHeight, 
					mUseClutter, mClutterClassGroup);
	//\\TODO Check and verify this
	LinkOtherGain = EIRP + 20.0*log10(mTxInst.sFrequency) + 32.44;
	//Loss_qrap -20log(f) - 32.44 + 10log(4*PI)
	cout<<"PreDEM"<<endl;
	mDEM->GetForLink(mTxInst.sSitePos, mRxInst.sSitePos, mPlotResolution, DEM);
	cout<<"PostDEM"<<endl;
	if (mUseClutter)
		mClutter.GetForLink(mTxInst.sSitePos, mRxInst.sSitePos,	mPlotResolution,Clutter);
	cout<<"PrePathloss"<<endl;
	double DiffLoss;
	mPropLoss = PathLoss.TotPathLoss(DEM, Tilt, Clutter,DiffLoss);
	cout<<"PostPathloss"<<endl;
	AntValue = mTxAnt.GetPatternValue(mTxBearing, Tilt)
			+mRxAnt.GetPatternValue(mRxBearing, -Tilt);
	mRxLev = -mPropLoss + LinkOtherGain ;//- AntValue;
	cout<<"PostRxLev"<<endl;
/*	for (int i=0; i<mEnvelopeSize; ++i)
	{
		//mEnvelopeValue[i]/=surface;
		//cout<<"Surf Val "<<i<<" = "<<mEnvelopeValue[i]<<endl;
	}
	//cout<<"PostSurface"<<endl;
	// Convert power distribution to dB and apply total losses to reciever
	//cout<<"PreDBise"<<endl;
*/	for (int i=0; i<mEnvelopeSize; ++i)
	{
		mEnvelopeValue[i]=10*log10(mEnvelopeValue[i])-mRxLev;
		//cout<<"Fin Val "<<i<<" = "<<mEnvelopeValue[i]<<endl;
	}
	//cout<<"PostDBise"<<endl;
	/*
	 Initialize(DEM,Clutter);	
	 SetEffProfile();	//Calculates the effective profile
	 SetLineOfSight();
	 SetFresnelClear();*/
	mTxTilt = Tilt;
	mRxTilt = -mTxTilt;
	return true;
}

//**********************************************************************************************
bool cSpectralLink::GetDBinfo(tFixed &Inst)
{
	pqxx::result r;
	string query;
	char text[33];

	gcvt(Inst.sInstKey,10,text);
	query = "SELECT siteid,eirp,radioinstallation_view.txpower as txpower,txlosses,rxlosses,";
	query +="rxsensitivity,startfreq,spacing,channel,";
	query+= "txantpatternkey,txbearing,txmechtilt,rxantpatternkey,";
	query+="rxbearing,rxmechtilt,txantennaheight,rxantennaheight ";
	query+= "FROM radioinstallation_view FULL JOIN cell ";
	query+= "ON cell.risector=radioinstallation_view.id ";
	query+= "FULL JOIN frequencyallocationlist ";
	query += "ON cell.id=ci FULL JOIN technology ";
	query +="ON radioinstallation_view.techkey=technology.id ";
	query += "WHERE radioinstallation_view.id =";
	query += text;
	query +=";";

	if (!gDb.PerformRawSql(query))
	{
		string err ="In cLink: Database Select on tables radioinstallation_view ";
		err +="and frequencyallocation failed: Query:";
		err+=query;
		QRAP_ERROR(err.c_str());
		return false;
	} // if
	else
	{
		gDb.GetLastResult(r);

		int size = r.size();
		if (size!=0)
		{
			Inst.sSiteID = atoi(r[0]["siteid"].c_str());
			Inst.sEIRP = atof(r[0]["eirp"].c_str());
			Inst.sTxPower = atof(r[0]["txpower"].c_str());
			Inst.sTxSysLoss = atof(r[0]["txlosses"].c_str());
			Inst.sRxSysLoss = atof(r[0]["rxlosses"].c_str());
			Inst.sRxSens = atof(r[0]["rxsensitivity"].c_str());
			Inst.sTxPatternKey = atoi(r[0]["txantpatternkey"].c_str());
			Inst.sTxAzimuth = atof(r[0]["txbearing"].c_str());
			Inst.sTxMechTilt = atof(r[0]["txmechtilt"].c_str());
			Inst.sRxPatternKey = atoi(r[0]["rxantpatternkey"].c_str());
			Inst.sRxAzimuth = atof(r[0]["rxbearing"].c_str());
			Inst.sRxMechTilt = atof(r[0]["rxmechtilt"].c_str());
			Inst.sTxHeight = atof(r[0]["txantennaheight"].c_str());
			Inst.sRxHeight = atof(r[0]["rxantennaheight"].c_str());
			//if (mFrequency>59999)
			Inst.sFrequency = atof(r[0]["startfreq"].c_str())+atof(r[0]["channel"].c_str())*atof(r[0]["spacing"].c_str())/1000;
			//else Inst.sFrequency = mFrequency;
			cout<<"Startfreq : "<<atof(r[0]["startfreq"].c_str())<<endl;
			cout<<"Channel no : "<<atof(r[0]["channel"].c_str())<<endl;
		}
		else
		{
			// Log and display a warning.
#ifdef QRAP_DEBUG
			//			Base::Log("Warning, radio installation " + IntToString(mFixedInsts[i].sInstKey) + " does not exist in the database.");
#endif
			// \TODO: exceptions and messages
			gcvt(Inst.sInstKey,8,text);
			cout << "Warning, radio installation " << Inst.sInstKey << " does not exist in the database." << endl;
			return false;
		} // else
	} // else
	return true;
}

//*******************************************************************************************************************
bool cSpectralLink::CalcDistribution()
{
	bool returnval = true;
	// \TODO: Fix once we have a radio installation to work with
	pqxx::result r;
	char *text =new char[33];
	string query = "SELECT offsets,values,numpoints ";
	query += "FROM radioinstallation FULL JOIN ";
	query += "(technology FULL JOIN envelopes ";
	query += "ON envelopes.techkey = technology.id) ";
	query += " ON radioinstallation.techkey = technology.id ";
	query += "WHERE radioinstallation.id =";
	gcvt(mTxInst.sInstKey,8,text);
	query+=text;
	query+=";";
	//query += "FROM envelopes WHERE envelopes.id = '1';";
	if (!gDb.PerformRawSql(query))
	{
		string err = "In cSpectralLink:";
		err+=" Database Select on tables radioinstallation and envelopes failed: Query:";
		err+=query;
		QRAP_ERROR(err.c_str());
		return false;
	} // if
	else
	{
		gDb.GetLastResult(r);

		int size = r.size();
		if (size!=0)
		{
			mDescSize = atoi(r[0]["numpoints"].c_str());
			if (mDescSize<=0)
			{
				string err = " Relavant frequency allocation or spectral envelop does not exist in the database."; 
				cout << err.c_str()  << endl;
				QRAP_WARN(err.c_str());
				return false;
			}
				
			cout<<"Description size : "<<mDescSize<<endl;
			MakeArrayFromDB(mDescOffset, r[0]["offsets"].c_str());
			for (int i=0; i<mDescSize; ++i)
				cout<<mDescOffset[i]<<",";
			cout<<endl;
			MakeArrayFromDB(mDescValue, r[0]["values"].c_str());
			for (int i=0; i<mDescSize; ++i)
				cout<<mDescValue[i]<<",";
			cout<<endl;
		}
		else
		{
			string err = "Warning, radio installation ";
			gcvt(mTxInst.sInstKey,8,text);
			err += text;
			err += " or relavant frequency allocation or spectral envelop does not exist in the database."; 
			cout << err.c_str()  << endl;
			QRAP_WARN(err.c_str());
			return false;
		} // else
	} // else
	double *freqReal;
	freqReal=new double[mDescSize];
	for (int i=0; i<mDescSize; ++i)
		freqReal[i]=mDescOffset[i]/1000.0+mTxInst.sFrequency;
	double minFreq = floor((mTxInst.sFrequency + mDescOffset[0]/1000.0)
			/mFrequencySpacing)*mFrequencySpacing;
	double maxFreq = ceil((mTxInst.sFrequency + mDescOffset[mDescSize-1]/1000.0)
			/mFrequencySpacing)*mFrequencySpacing;
	cout<<"Center Freq : "<<mTxInst.sFrequency<<endl;
	cout<<"Spacing : "<<mFrequencySpacing<<endl;
	cout<<"Min : "<<minFreq<<" and Max : "<<maxFreq<<endl;
	double tempEnvelopeSize=(maxFreq-minFreq)/mFrequencySpacing;
	if ((tempEnvelopeSize-floor(tempEnvelopeSize))>0.5)
		mEnvelopeSize= (int)(ceil(tempEnvelopeSize)+1);
	else
		mEnvelopeSize= (int)(floor(tempEnvelopeSize)+1);
	if (mEnvelopeFreq)
		delete [] mEnvelopeFreq;
	mEnvelopeFreq = new float[mEnvelopeSize];
	if (mEnvelopeValue)
		delete [] mEnvelopeValue;
	mEnvelopeValue = new float[mEnvelopeSize];
	int offsetIndex=0;
	mEnvelopeTotal=0;
	for (int i=0; i<mEnvelopeSize; ++i)
	{
		double freq=minFreq+i*mFrequencySpacing;
		mEnvelopeFreq[i]=freq;
		if (freq<=freqReal[offsetIndex])
		{
			mEnvelopeValue[i]=mDescValue[offsetIndex];
		}
		else
		{
			if (offsetIndex<(mDescSize-1))
			{
				//Normal case
				while (freq>freqReal[offsetIndex+1]&&(offsetIndex !=(mDescSize-1)))
					++offsetIndex;
				if (offsetIndex==(mDescSize-1))
					mEnvelopeValue[i]=mDescValue[offsetIndex];
				else
					mEnvelopeValue[i]=mDescValue[offsetIndex]+(freq
							-freqReal[offsetIndex])/(freqReal[offsetIndex+1]
							-freqReal[offsetIndex])*(mDescValue[offsetIndex+1]
							-mDescValue[offsetIndex]);
			}
			else
			{
				//Beyond last bound
				mEnvelopeValue[i]=mDescValue[mDescSize-1];
			}
		}
		mEnvelopeValue[i]=pow(10, mEnvelopeValue[i]/10);
		
		//cout<<mEnvelopeFreq[i]<<":"<<mEnvelopeValue[i]<<endl;
		mEnvelopeTotal+=mEnvelopeValue[i];
	}
	// \TODO: Are there other factors as well from Transmitter?
/*	cout<<"Normalizing ... "<<endl;
	double TxPower = mTxInst.sTxPower;
	//mEnvelopeTotal=pow(10, mEnvelopeTotal/10);
	for (int i=0; i<mEnvelopeSize; ++i)
	{
		mEnvelopeValue[i]=mEnvelopeValue[i]*TxPower/(mEnvelopeTotal*mFrequencySpacing*1000);
		//cout<<mEnvelopeFreq[i]<<":"<<mEnvelopeValue[i]<<endl;
	}
*/
	delete [] text;
	return true;
}

}
