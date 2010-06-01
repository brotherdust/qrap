 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSpectralPlot.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Shaun Barlow 
 *                : (Magdaleen Ballot) (magdaleen.ballot@up.ac.za)
 *    Description : Combine results of many links and set-up frequency plot 
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

#include "cSpectralPlot.h"
using namespace Qrap;
using namespace std;
#include <fstream>


//**********************************************************************************
cSpectralPlot::cSpectralPlot()
{
}


//************************************************************************************
cSpectralPlot::~cSpectralPlot()
{
}

//*********************************************************************************************
bool cSpectralPlot::SetPlotTask(unsigned Rx, QList<unsigned> Tx, double FrequencySpacing, double kFactor)
{
	mFrequencySpacing=FrequencySpacing;
	mkFactor=kFactor;
	
	for(int i=0;i<Tx.size();i++)
	{
		if (Rx!=Tx[i])
		{
			mLinks.push_back(new cSpectralLink(&mDem, Rx,Tx[i],mFrequencySpacing,mkFactor));
			cout<<"Made a new link from "<<Rx<<" to "<<Tx[i]<<endl;
		}
	}
}

//**********************************************************************************************
bool cSpectralPlot::ReadPlotRequest(const char *filename)
{
	const unsigned TEMPSIZE=1000;
	char temp[TEMPSIZE];
	unsigned i;
	bool FoundRasterSet = TRUE;

	unsigned RxInstKey;
	
	
	ifstream infile(filename);
	if (!infile)
	{
		string err = "In cLink:ReadPlotRequest Error opening file: ";
		err+=filename;
		QRAP_ERROR(err.c_str());
		return false;
	}

	infile >> temp;
	if (infile.eof())
	{
		string err = "Error reading file: ";
		err+=+filename;
		err+=" at Units. End of file reached.";
		QRAP_WARN(err.c_str());
		return false;
	}
	else if (!strcasecmp(temp,"dB"))
		mUnits = dB;
	else if (!strcasecmp(temp,"dBW"))
		mUnits = dBW;
	else if (!strcasecmp(temp,"dBm"))
		mUnits = dBm;	
	else if (!strcasecmp(temp,"dBuV"))
		mUnits = dBuV;
	else if (!strcasecmp(temp,"dBuVm"))
		mUnits = dBuVm;
	else if (!strcasecmp(temp,"dBWm2Hz"))
		mUnits = dBWm2Hz;
	else if (!strcasecmp(temp,"dBWm2"))
		mUnits = dBWm2;
	else
	{
		string err = "Error reading file: ";
		err+=+filename;
		err+=" at Units. Using default.";
		QRAP_WARN(err.c_str());
	}
	infile >> temp;

	if (!strcasecmp(temp, "Resolution"))
	{
		infile >> mFrequencySpacing;
		infile >> temp;
		cout << "The resolution is "<<mFrequencySpacing<<endl;
	}
	if (!strcasecmp(temp, "Rx"))
	{
		infile >>  RxInstKey;
		infile >> temp;
	}
	mLinks.empty();
	bool TxDone=false;
	while (!strcasecmp(temp, "Tx"))
	{
		temp[0]=0;
		unsigned tempSiteID;
		infile >> tempSiteID;
		infile >> temp;
		mLinks.push_back(new cSpectralLink(&mDem, RxInstKey,tempSiteID,mFrequencySpacing,mkFactor));
		cout<<"Made a new link from "<<RxInstKey<<" to "<<tempSiteID<<endl;
	}
}

//********************************************************************************************
bool cSpectralPlot::CombinePlots()
{
	
	//First find the minimum and maximum frequencies for the combined links
	double minFreq = 1E12;
	double maxFreq = 0;
	for(vector<cSpectralLink *>::iterator i=mLinks.begin();i!=mLinks.end();i++)
	{
		if ((*i)->GetMinFreq()<minFreq)
			minFreq = (*i)->GetMinFreq();
		if ((*i)->GetMaxFreq()>maxFreq)
			maxFreq = (*i)->GetMaxFreq();
	}
	cout<<"Max : "<<maxFreq<<" Min : "<<minFreq<<endl;
	mNumFreqs = (int)((maxFreq - minFreq)/mFrequencySpacing + 1);
	cout<<"NumFreqs : "<<mNumFreqs<<endl;
	
	//Generate the memory for our plot
	mPFDFreq = new double[mNumFreqs+1];
	mPFDValue = new double[mNumFreqs+1];
	
	//Initialize the plot for freqs and zero it
	double freq=minFreq;
	for(int i=0;i<mNumFreqs;i++)
	{
		mPFDFreq[i]=freq;
		mPFDValue[i]=0;
		freq+=mFrequencySpacing;
	}
	
	//Combine the links into combined plot
	for(vector<cSpectralLink *>::iterator i=mLinks.begin();i!=mLinks.end();i++)
	{
		int startIndex = (int)(((*i)->GetMinFreq() - minFreq)/mFrequencySpacing);
		int linkFreqs = (*i)->GetEnvelopeSize();
		cout<<"Startindex : "<<startIndex<<" and there are "<<linkFreqs<<" Freqs"<<endl;
		for(int j=0;j<linkFreqs;++j)
		{
			mPFDValue[startIndex+j]+=pow(10,(*i)->GetEnvelopeValue(j)/10);
		}
	}
	for(int i=0;i<mNumFreqs;i++)
	{
		mPFDValue[i]=10*log10(mPFDValue[i]);
	}
}

//******************************************************************************************************
bool cSpectralPlot::DoPlot()
{
	bool success=true;
	int i;
	mNumLinks = mLinks.size();
	cout << "In cSpectralPlot::DoPlot() aantal Links " << mLinks.size() << endl;
	for(i=0;i<mNumLinks;i++)
	{
		cout << " In cSpectralPlot::DoPlot() before (*i)->CalcDistribution()" << endl;
		if (mLinks[i]->CalcDistribution())
		{
			cout << " In cSpectralPlot::DoPlot() before (*i)->DoLink()" <<  endl;
			mLinks[i]->DoLink();
		}
		else success=false;
	}
	cout << " In cSpectralPlot::DoPlot() before CombinePlots()" << endl;
	if (success) CombinePlots();
	cout << " In cSpectralPlot::DoPlot() after CombinePlots()" << endl;
	return success;
}


//************************************************************************************************************
bool cSpectralPlot::SavePlot(string FileName)
{
	ofstream outfile(FileName.c_str());
	outfile<<"\"Frequency(MHhz)\" , \"Spectral Flux\""<<endl;
	for(int i=0;i<mNumFreqs;++i)
	{
		outfile<<mPFDFreq[i]<<" , "<<mPFDValue[i]<<endl;
	}
	outfile.close();
}

