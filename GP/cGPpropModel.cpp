 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cPthlssp.h
 *    Copyright   : (c) Meraka Institute CSIR (1997) & University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Core propagation prediction algorithm
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

#include "cGPpropModel.h"
#include "cGPFunctions.h"
#ifndef GLOBALS
#define GLOBALS 1
std::default_random_engine 		gRandomGen;
std::normal_distribution<double> 	gGauss;
#endif

using namespace Qrap;
using namespace std;

//**************************************************************************
cGPpropModel::cGPpropModel()
{
	cout << "Entering cGPpropModel::cGPpropModel()" << endl;
	mNumCandidates = NUM_INIT_CANDIDATES; 
	mNumToDie = (unsigned)(mNumCandidates*DEATH_RATE/100);
	cout << "Leaving cGPpropModel::cGPpropModel()" << endl;
}

//*************************************************************************
cGPpropModel::cGPpropModel(const cGPpropModel &right)
{
}

//*************************************************************************
cGPpropModel::~cGPpropModel()
{
	unsigned i;
	mNumCandidates = mCandidate.size();
	for (i = 0; i < mNumCandidates; i++)
	{
		delete [] mCandidate[i].sClutterType;
		delete [] mCandidate[i].sClutterHeight; 
		deleteTree(mCandidate[i].sTree);
	}
	mCandidate.clear();
	mNumCandidates = mStars.size();
	for (i = 0; i < mNumCandidates; i++)
	{
		delete [] mStars[i].sClutterType;
		delete [] mStars[i].sClutterHeight; 
		deleteTree(mStars[i].sTree);
	}
	mStars.clear();
}

//*************************************************************************
int cGPpropModel:: mainTuning()
{
	cout << "Entering cGPpropModel:: mainTuning()" << endl;
	unsigned i=0, j=0, k=0, numConst = 0;

	cGeoP *Hoek;
	unsigned NumHoek=4;

//Gauteng 20m DEM
	NumHoek=19;
	Hoek = new cGeoP[NumHoek];

	Hoek[0].Set(-25.01, 28.99);
	Hoek[1].Set(-25.01, 28.51);
	Hoek[2].Set(-25.26, 28.51);
	Hoek[3].Set(-25.26, 28.01);
	Hoek[4].Set(-25.51, 28.01);
	Hoek[5].Set(-25.51, 27.76);
	Hoek[6].Set(-25.76, 27.76);
	Hoek[7].Set(-25.76, 27.26);
	Hoek[8].Set(-26.49, 27.26);
	Hoek[9].Set(-26.49, 27.51);
	Hoek[10].Set(-26.99, 27.51);
	Hoek[11].Set(-26.99, 28.49);
	Hoek[12].Set(-26.74, 28.49);
	Hoek[13].Set(-26.74, 28.99);
	Hoek[14].Set(-25.74, 28.99);
	Hoek[15].Set(-25.74, 29.24);
	Hoek[16].Set(-25.26, 29.24);
	Hoek[17].Set(-26.26, 28.99);
	Hoek[18].Set(-25.01, 28.99);

//	Bryanston
/*	NumHoek=4;
	Hoek = new cGeoP[NumHoek];
	Hoek[0].Set(-26.036, 27.976);
	Hoek[1].Set(-26.108, 27.976);
	Hoek[2].Set(-26.108, 28.074);
	Hoek[3].Set(-26.036, 28.074);
*/
	vPoints Punte;
	for (i=0; i<NumHoek; i++)
		Punte.push_back(Hoek[i]);
	
	delete [] Hoek;

	mMeas.SetUseAntANN(false);

//	mMeas.mPathLoss.mClutter.Reset(1);

	cout << "Loading measurements ... in main()" << endl;

	mMeas.SetPlotResolution(5);
	mMeas.LoadMeasurements(Punte,0,0,0);

	double Mean, MSE, StDev, CorrC;
	mMeas.PerformAnalysis(Mean, MSE, StDev, CorrC);

	cout << "Initial calculation done" << endl;
	cout << "Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl << endl << endl;

	mMeas.PerformAnalysis(Mean, MSE, StDev, CorrC);

	cout << "Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl << endl << endl;
	GOftn* newTree = nullptr;
	SCandidate newCandidate;

	double PathLoss,Tilt,DiffLoss,sClutterDistance;
	

	// ***********************************************************************
	// Initialise candidates of known models

	// Basic free space plus obstruction loss (Q-Rap basic model)
	newTree = new Add(4);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();

	newCandidate.sTree = newTree;
	newCandidate.sDepth = 3;
	mCandidate.push_back(newCandidate);


	newTree = new Add(4);
	newTree->mChild[0] = new ConstNode(39.40);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(27.74);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(23.3);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();

	newCandidate.sTree = newTree;
	newCandidate.sDepth = 3;
	mCandidate.push_back(newCandidate);

	newTree = new Add(5);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[4]->mChild[1] = new Power();
	newTree->mChild[4]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[4]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[4]->mChild[2] = new Power();
	newTree->mChild[4]->mChild[2]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[4]->mChild[2]->mChild[1] = new ConstNode(0.5);

	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);


	// Tuned Q-Rap model ... classic tuning
	newTree = new Add(7);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(34.1453);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(38.5005);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(0.340187);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(-14.1625);
	newTree->mChild[4]->mChild[1] = new Log10Node();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[2] = new Log10Node();
	newTree->mChild[4]->mChild[2]->mChild[0] = new TxHeightNode();
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(-4.74767);
	newTree->mChild[5]->mChild[1] = new Log10Node();
	newTree->mChild[5]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6] = new Multiply();
	newTree->mChild[6]->mChild[0] = new ConstNode(-0.47927);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);

	
	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);

	// Tuned Q-Rap model ... with clutterdepth
	newTree = new Add(7);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(34.1453);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(38.5005);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(0.340187);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(-14.1625);
	newTree->mChild[4]->mChild[1] = new Log10Node();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[2] = new Log10Node();
	newTree->mChild[4]->mChild[2]->mChild[0] = new TxHeightNode();
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(-4.74767);
	newTree->mChild[5]->mChild[1] = new Log10Node();
	newTree->mChild[5]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6] = new Multiply(3);
	newTree->mChild[6]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[6]->mChild[2] = new Power();
	newTree->mChild[6]->mChild[2]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[6]->mChild[2]->mChild[1] = new ConstNode(0.5);

	
	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);


	// Hata - Suburban
	newTree = new Add(9);
	newTree->mChild[0] = new ConstNode(63.35);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(27.72);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(44.9);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(-6.55);
	newTree->mChild[4]->mChild[1] = new Log10Node();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[2] = new Log10Node();
	newTree->mChild[4]->mChild[2]->mChild[0] = new TxHeightNode();
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(-13.82);
	newTree->mChild[5]->mChild[1] = new Log10Node();
	newTree->mChild[5]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6] = new Multiply(3);
	newTree->mChild[6]->mChild[0] = new ConstNode(-1.1);
	newTree->mChild[6]->mChild[1] = new RxHeightNode();
	newTree->mChild[6]->mChild[2] = new Log10Node();
	newTree->mChild[6]->mChild[2]->mChild[0] = new FrequencyNode();
	newTree->mChild[7] = new Multiply();
	newTree->mChild[7]->mChild[0] = new ConstNode(-2.0);
	newTree->mChild[7]->mChild[1] = new Square();
	newTree->mChild[7]->mChild[1]->mChild[0] = new Log10Node();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0] = new Divide();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0]->mChild[0] = new FrequencyNode();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0]->mChild[1] = new ConstNode(28);
	newTree->mChild[8] = new Multiply();
	newTree->mChild[8]->mChild[0] = new ConstNode(0.7);
	newTree->mChild[8]->mChild[1] = new RxHeightNode();


	newCandidate.sTree = newTree;
	newCandidate.sDepth = 6;
	mCandidate.push_back(newCandidate);

	// Hata - Suburban altered with vegetation loss term
	newTree = new Add(10);
	newTree->mChild[0] = new ConstNode(63.35);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(27.72);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(44.9);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(-6.55);
	newTree->mChild[4]->mChild[1] = new Log10Node();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[2] = new Log10Node();
	newTree->mChild[4]->mChild[2]->mChild[0] = new TxHeightNode();
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(-13.82);
	newTree->mChild[5]->mChild[1] = new Log10Node();
	newTree->mChild[5]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6] = new Multiply(3);
	newTree->mChild[6]->mChild[0] = new ConstNode(-1.1);
	newTree->mChild[6]->mChild[1] = new RxHeightNode();
	newTree->mChild[6]->mChild[2] = new Log10Node();
	newTree->mChild[6]->mChild[2]->mChild[0] = new FrequencyNode();
	newTree->mChild[7] = new Multiply();
	newTree->mChild[7]->mChild[0] = new ConstNode(-2.0);
	newTree->mChild[7]->mChild[1] = new Square();
	newTree->mChild[7]->mChild[1]->mChild[0] = new Log10Node();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0] = new Divide();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0]->mChild[0] = new FrequencyNode();
	newTree->mChild[7]->mChild[1]->mChild[0]->mChild[0]->mChild[1] = new ConstNode(28);
	newTree->mChild[8] = new Multiply();
	newTree->mChild[8]->mChild[0] = new ConstNode(0.7);
	newTree->mChild[8]->mChild[1] = new RxHeightNode();
	newTree->mChild[9] = new Multiply(3);
	newTree->mChild[9]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[9]->mChild[1] = new Power();
	newTree->mChild[9]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[9]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[9]->mChild[2] = new Power();
	newTree->mChild[9]->mChild[2]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[9]->mChild[2]->mChild[1] = new ConstNode(0.5);


	newCandidate.sTree = newTree;
	newCandidate.sDepth = 6;
	mCandidate.push_back(newCandidate);

	newTree = new Add(5);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[4]->mChild[1] = new Power();
	newTree->mChild[4]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[4]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[4]->mChild[2] = new Power();
	newTree->mChild[4]->mChild[2]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[4]->mChild[2]->mChild[1] = new ConstNode(0.5);


	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);
	newTree = new Add(9);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[4]->mChild[1] = new Power();
	newTree->mChild[4]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[4]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[4]->mChild[2] = new Power();
	newTree->mChild[4]->mChild[2]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[4]->mChild[2]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[5]->mChild[1] = new Power();
	newTree->mChild[5]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[5]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[6] = new Multiply();
	newTree->mChild[6]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[7] = new Multiply();
	newTree->mChild[7]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[7]->mChild[1] = new Power();
	newTree->mChild[7]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[7]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[8] = new Multiply();
	newTree->mChild[8]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[8]->mChild[1] = new Power();
	newTree->mChild[8]->mChild[1]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[8]->mChild[1]->mChild[1] = new ConstNode(0.5);


	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);

	newTree = new Add(8);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply();
	newTree->mChild[4]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[4]->mChild[1] = new Power();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[5]->mChild[1] = new Power();
	newTree->mChild[5]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[5]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[6] = new Multiply();
	newTree->mChild[6]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[7] = new Multiply();
	newTree->mChild[7]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[7]->mChild[1] = new Power();
	newTree->mChild[7]->mChild[1]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[7]->mChild[1]->mChild[1] = new ConstNode(0.5);


	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);



	newTree = new Add(8);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply();
	newTree->mChild[4]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[4]->mChild[1] = new Power();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[5]->mChild[1] = new Power();
	newTree->mChild[5]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[5]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[6] = new Multiply();
	newTree->mChild[6]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);
	newTree->mChild[7] = new Multiply();
	newTree->mChild[7]->mChild[0] = new ConstNode(0.2);
	newTree->mChild[7]->mChild[1] = new Power();
	newTree->mChild[7]->mChild[1]->mChild[0] = new ClutterDepthNode();
	newTree->mChild[7]->mChild[1]->mChild[1] = new ConstNode(0.5);

	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);

	mNumCandidates = mCandidate.size();

	srand(time(NULL));
	
	// create the inital tree population 
	for (i=mNumCandidates; i<NUM_INIT_CANDIDATES; i++)
	{
		newTree = createRandomTree();
		newCandidate.sTree = newTree;
		mCandidate.push_back(newCandidate);
	}

	mNumCandidates = mCandidate.size();
	unsigned NumStars;
	double CrossOverProp = 0;
	for (i=0; i<mNumCandidates; i++)
	{
		mCandidate[i].sCorrC = -1.0;
		mCandidate[i].sStdDev = 999;
		mCandidate[i].sMean = 999;
		mCandidate[i].sFitness = 999;
		mCandidate[i].sRank = 2*(mNumCandidates-1);
		mCandidate[i].sPareto = true;
		mCandidate[i].sOptimised =false;
		mCandidate[i].sNumClutter = mMeas.mPathLoss.mClutter.mNumber;
		mCandidate[i].sClutterType  = new unsigned[mMeas.mPathLoss.mClutter.mNumber];
		mCandidate[i].sClutterHeight = new double[mMeas.mPathLoss.mClutter.mNumber];
		mCandidate[i].sDepth=mCandidate[i].sTree->getTreeDepth();
		numConst = mCandidate[i].sTree->getConstants(mCandidate[i].sConstants);
		for (j=0; j<mMeas.mPathLoss.mClutter.mNumber; j++)
		{
			mCandidate[i].sClutterType[j] = mMeas.mPathLoss.mClutter.mClutterTypes[j].sLandCoverID;	
			mCandidate[i].sClutterHeight[j] = mMeas.mPathLoss.mClutter.mClutterTypes[j].sHeight;
//			cout << "Cj = " <<  mMeas.mPathLoss.mClutter.mNumber 
//			<< "	Height= " << mCandidate[i].sClutterHeight[j] << endl;
		} 
	}

	unsigned IndexForCrossOver = 0;
	unsigned IndexForClone = 0;
	double growProp;
	unsigned Nsamples=0;
	double MaxFitness=0;
	mMinFitness = 2000;

	unsigned NumThread = 8;
    	NumThread = min(NumThread,std::thread::hardware_concurrency());
	if (NumThread<1) NumThread=1;
	std::thread calcThread[NumThread];
	unsigned Skip = NumThread;

	for (k=0; k<NUM_GENERATIONS; k++) 
	{
		double MinFitness=999, MaxFitness=0;
		mNumCandidates = mCandidate.size();

		for (i=0; i<mNumCandidates; i++)
		{
			if ((k<4)&&(i<10)) mCandidate[i].sOptimised = false;
		}

//		ComputeCandidates(0,1);

		for (int i = 0; i < NumThread; ++i) 
		{
			cout << "generating thread: " << i << endl;
             		calcThread[i] = std::thread(&cGPpropModel::ComputeCandidates, this, i, Skip);
			// Wait for cPathLossPredictor and cMeasAnalysis constructors to complete
			// To avoid database clashes
			 std::this_thread::sleep_for (std::chrono::milliseconds(100));
         	}
 
         	for (int i = 0; i < NumThread; ++i) 
		{
			cout << "joining thread: " << i << endl;
			if (calcThread[i].joinable())
             			calcThread[i].join();	
		}

		cout << "GENERATION = " << k << endl;
		for (i=0; i<mNumCandidates; i++)
		{
			cout << "i = " << i << "	Rank=" << mCandidate[i].sRank
				<< "	Fitness=" << mCandidate[i].sFitness				
				<<"	CorrC=" << mCandidate[i].sCorrC << "	StDev=" << mCandidate[i].sStdDev
				<< "	Mean=" << mCandidate[i].sMean  
				<< "	Depth=" << mCandidate[i].sDepth << endl;
			if (mCandidate[i].sFitness < mMinFitness)
				mMinFitness = mCandidate[i].sFitness;
			else if (mCandidate[i].sFitness>MaxFitness)
				MaxFitness = mCandidate[i].sFitness;
		}


		//sort by performance (sort in increasing order so we work on first N)
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnCorrC);
		for (i=0; i<mNumCandidates; i++)
		{
			mCandidate[i].sRank = i;
//			cout << i << "	CorrC = " << mCandidate[i].sCorrC << endl;
		}
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnStdDev);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	StdDev = " << mCandidate[i].sStdDev << endl;
			mCandidate[i].sRank += i;
		}

		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnFitness);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	Fitness = " << mCandidate[i].sFitness << endl;
		}
		
		for (i=0; i<mNumCandidates; i++)
		{
			mCandidate[i].sPareto = true;
			for (j=0; j<mNumCandidates; j++)
			{
				if (j!=i)
				{
					mCandidate[i].sPareto = (mCandidate[i].sPareto) &&
						(!((mCandidate[j].sCorrC>=mCandidate[i].sCorrC)&&
						(mCandidate[j].sStdDev<=mCandidate[i].sStdDev)));
				}
			}
			if (mCandidate[i].sPareto)
			{
				mCandidate[i].sRank=1;
				mCandidate[i].sFitness = mMinFitness;
				for (j=0; j<mStars.size(); j++)
					mCandidate[i].sPareto = (mCandidate[i].sPareto) &&
						(!((mStars[j].sCorrC>=mCandidate[i].sCorrC)&&
						(mStars[j].sStdDev<=mCandidate[i].sStdDev)));
				if (mCandidate[i].sPareto)
				{
					optimiseConstants(i);
					mStars.push_back(mCandidate[i]);
				}
							
			}
		} 
		
		if (mStars.size()>0)	
			sort(mStars.begin(), mStars.end(), SortCriteriaOnFitness);

//		if too many stars, clearout (some) non-pareto candidates
		i=mStars.size()-1;
		while ((mStars.size()>NUM_INIT_CANDIDATES/3)&&(i>0))
		{
			cout << "Checking Stars " << endl;
			mStars[i].sPareto = true;
			for (j=0; j<mStars.size(); j++)
			{
				if (j!=i)
				{
					mStars[i].sPareto = (mStars[i].sPareto) &&
						(!((mStars[j].sCorrC>=mStars[i].sCorrC)&&
						(mStars[j].sStdDev<=mStars[i].sStdDev)));
				}
			}	
			if (!mStars[i].sPareto)
			{
				cout << "erasing mStar " << i << endl;
				mStars.erase (mStars.begin()+i);
				i--;
			}
			i--;
		}
	
		if (mCandidate.size()>0)
			sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnFitness);

		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	Fitness = " << mCandidate[i].sFitness << endl;
		}

		//....................................................................................
		// Replace completely unfit individuals		
		mNumToDie = 0;
		while ((mNumToDie < mNumCandidates-1)
			&&(mCandidate[mNumCandidates-1-mNumToDie].sFitness>UNFIT_LIMIT))
			mNumToDie++;
		
		NumStars = mStars.size();
		for (i=0; i<mNumToDie; i++)
		{
			IndexForClone = mNumCandidates -1;
			while (IndexForClone>(mNumCandidates-mNumToDie))
				IndexForClone = (unsigned)(fabs(gGauss(gRandomGen)*(mNumCandidates-mNumToDie)));
			mCandidate[mNumCandidates-1-i] = mCandidate[IndexForClone];
			mCandidate[mNumCandidates-1-i].sPareto=false;
			mCandidate[mNumCandidates-1-i].sOptimised = false;
			mutateCandidate(mNumCandidates-1-i,true);
			if (NumStars>1)
			{ 
				IndexForCrossOver = NumStars;
				while (IndexForCrossOver>NumStars-1)
					IndexForCrossOver = (unsigned)(fabs(gGauss(gRandomGen))*NumStars);
				crossOverTree(mCandidate[mNumCandidates-1-i].sTree, 
						mStars[IndexForCrossOver].sTree);  
			}
			mCandidate[mNumCandidates-1-i].sRank=2*mNumCandidates;
			mCandidate[mNumCandidates-1-i].sFitness = MaxFitness;
		}
		//...................................................................................
//
		for (i=0; i<mNumCandidates-mNumToDie; i++)
		{
			growProp = fabs(gGauss(gRandomGen))*mCandidate[i].sFitness/(4*mMinFitness);
			mutateCandidate(i,(growProp>1));
			CrossOverProp = fabs(gGauss(gRandomGen))*mCandidate[i].sFitness/(4*mMinFitness);
			if ((NumStars>1)&&(CrossOverProp>1))
			{
				IndexForCrossOver = NumStars;
				while (IndexForCrossOver>NumStars-1)
					IndexForCrossOver = (unsigned)(fabs(gGauss(gRandomGen))*NumStars);
				crossOverTree(mCandidate[i].sTree, mStars[IndexForCrossOver].sTree);  
			}
		}

		cout << "Best candidate:	CorrC = " << mCandidate[0].sCorrC 
			<< "	StdDev = " << mCandidate[0].sStdDev << endl << endl;
	}
	i=0;
	while (mCandidate[i].sPareto)
	{
		cout << "i = " << i << "	Rank=" << mCandidate[i].sRank
			<< "	Fitness=" << mCandidate[i].sFitness 
			<< "	CorrC=" << mCandidate[i].sCorrC 
			<< "	StDev=" << mCandidate[i].sStdDev
			<< "	Mean=" << mCandidate[i].sMean  
			<< "	Depth=" << mCandidate[i].sDepth <<endl;
		printTree(mCandidate[i].sTree);

		for (j=0;j<mCandidate[i].sNumClutter;j++)
		{
			cout << "	[" << j<< "] " << mCandidate[i].sClutterHeight[j];
		}
		cout << endl << endl;
		i++;
	}

	for (i=0; i<mStars.size(); i++)
	{
		cout << " Star.	i = " << i << "	Rank=" << mStars[i].sRank
			<< "	Fitness=" << mStars[i].sFitness 
			<< "	CorrC=" << mStars[i].sCorrC 
			<< "	StDev=" << mStars[i].sStdDev
			<< "	Mean=" << mStars[i].sMean  
			<< "	Depth=" << mStars[i].sDepth <<endl;
		printTree(mStars[i].sTree);

		for (j=0;j<mStars[i].sNumClutter;j++)
		{
			cout << "	[" << j<< "] " << mStars[i].sClutterHeight[j];
		}
		cout << endl << endl;
	}
    	return 0;
}

//******************************************************************************
void cGPpropModel::ComputeCandidates(unsigned Begin, unsigned Skip)
{
	double MSE;
	int Nsamples;
	unsigned Index;
	cout << "In cGPpropModel::ComputeCandidates: Begin = " 
		<< Begin << "	Skip = " << Skip << endl;
	for (Index = Begin; Index < mNumCandidates; Index+=Skip)
	{ 
		Nsamples = CostFunction(Index, mCandidate[Index].sMean ,MSE,
			 mCandidate[Index].sStdDev, mCandidate[Index].sCorrC,false);

		if ((isnan(mCandidate[Index].sStdDev))||(isinf(mCandidate[Index].sStdDev))
			||(mCandidate[Index].sStdDev<0)||(isinf(MSE))||(isnan(MSE)))
		{
			mCandidate[Index].sStdDev = 999;
		}
		if ((isnan(mCandidate[Index].sCorrC))||(isinf(mCandidate[Index].sCorrC))
			||(mCandidate[Index].sCorrC<-1.0)||(isinf(MSE))||(isnan(MSE)))
		{
			mCandidate[Index].sCorrC = -0.999;
		}

		mCandidate[Index].sFitness = 100*(1-mCandidate[Index].sCorrC)
						+ mCandidate[Index].sStdDev;
	
		if (
			(mCandidate[Index].sCorrC>0.0)
			&&(mCandidate[Index].sStdDev<1300)
			&&(mCandidate[Index].sFitness<1300)
			&&(!mCandidate[Index].sOptimised)
		   )
		{
			optimiseConstants(Index);
		}

		mCandidate[Index].sFitness = 100*(1-mCandidate[Index].sCorrC) 
						+ mCandidate[Index].sStdDev;
	}
}

//******************************************************************************
int cGPpropModel::CostFunction(unsigned CIndex, double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, 
					bool CalcNewObstruction, unsigned Clutterfilter)
{
	bool *ClutterOccur;
	cPathLossPredictor PathLossPred = mMeas.mPathLoss;
	PathLossPred.set_Tuning(false);
	double QRapCore = 0;
	double CsumOfAntValue = 0, CsumOfPathLoss =0;
	ClutterOccur = new bool[mCandidate[CIndex].sNumClutter];
	unsigned * LOS;
	unsigned * NLOS;
	LOS = new unsigned[mCandidate[CIndex].sNumClutter];
	NLOS = new unsigned[mCandidate[CIndex].sNumClutter];
	mMeas.mClutterFilter = Clutterfilter;
	unsigned i=0, j=0, k=0, NumUsed = 0, CNumUsed =0;
	unsigned MobileNum=0, FixedNum=0;
	double Error=0, TotalError=0, TotalSError=0, TotalMeas=0;
	double TotalPred=0, TotalSMeas=0, TotalSPred=0, TotalMeasPred=0;
	double CError=0, CTotalError=0, CTotalSError=0, CTotalMeas=0;
	double CTotalPred=0, CTotalSMeas=0, CTotalSPred=0, CTotalMeasPred=0;
	int currentInst=0;
	unsigned currentMobile=0;
	cAntennaPattern FixedAnt, MobileAnt;
	bool UseClutter = mMeas.mUseClutter;

	tMeasPoint tempMeas;
	double DiffLoss = 0, PathLoss, ClutterDistance, Distance, ClutterHeight;
	double Azimuth = 0;
	float Tilt = 0;
	unsigned ClutterType;
	double CMean = 0, CMeanSquareError=0, CCorrC=0.0, CStDev=0.0;
	Mean = 0; 
	MeanSquareError=0;
	StDev = 0;
	CorrC = 0;

	unsigned Length;
	cProfile Clutter;
	cProfile DEM;
	double AntValue=0, EIRP=0;
//	double tPathLoss =0;
//	unsigned tClutter = 0;

	if (CalcNewObstruction)
	{
		for (j=0;j<PathLossPred.mClutter.mNumber;j++)
		{
//			if (mMeas.mUseClutter) mMeas.mClutterCount[j]=0;
			LOS[j]=0;
			NLOS[j]=0;
			PathLossPred.mClutter.mClutterTypes[j].sHeight = mCandidate[CIndex].sClutterHeight[j];
			for (i=0;i<NUMTERMS;i++)
				PathLossPred.mClutter.mClutterTypes[j].sCoefficients[i]=0.0;
//			cout << "Cj = " <<  PathLossPred.mClutter.mNumber 
//				<< "	Height= " << PathLossPred.mClutter.mClutterTypes[j].sHeight << endl;
		}
	}

	NumUsed = 0;
	
	if (0==mMeas.mFixedInsts.size()) return 0;

//	cout <<"cGPpropModel::CostFunction: mNumMeas = " << mNumMeas << endl;

	unsigned SkipNumber = min((mMeas.mNumMeas/NUM_POINT_PER_EVAL),1+(unsigned)(mCandidate[CIndex].sFitness/25));
	if (SkipNumber<1) SkipNumber = 1;
	if ((mCandidate[CIndex].sPareto)&&(k>0)) SkipNumber = 1;
	if (!CalcNewObstruction) SkipNumber = 1;

	unsigned FirstMeas = SkipNumber * ((double) rand() / (RAND_MAX));
//	cout << "SkipNumber=" << SkipNumber <<"	FirstMeas = " << FirstMeas << endl;
	for (i=FirstMeas; i<mMeas.mNumMeas; i=i+SkipNumber)
	{
		tempMeas = mMeas.mMeasPoints[i];
		if ((0==Clutterfilter)||(0==tempMeas.sClutter)
			||(Clutterfilter==tempMeas.sClutter))
		{

			//Change settings if the mobile installation changed
			if (tempMeas.sInstKeyMobile!=currentMobile)
			{
				currentMobile = tempMeas.sInstKeyMobile;
				while ((mMeas.mMobiles[MobileNum].sInstKey!=currentMobile)
					&&(MobileNum < mMeas.mMobiles.size()))
					MobileNum++;
				if (MobileNum == mMeas.mMobiles.size())
				{
					cout << "MobileNum reached limit ... ending measurement analysis" <<endl;
					return 0;
				}
//				cout << "Setting Mobile Antenna, mMobiles[MobileNum].sInstKey =" 
//					<< mMeas.mMobiles[MobileNum].sInstKey << endl;
				gDb.mgDBlock.lock();
				MobileAnt.SetAntennaPattern(mMeas.mMobiles[MobileNum].sInstKey, Mobile, 0, 0);
				gDb.mgDBlock.unlock();

//				cout << "Setting Path loss parameters" << endl;
//				cout << "FixedNum = " << FixedNum << endl;

				if (CalcNewObstruction)
				{
					PathLossPred.setParameters(mMeas.mkFactor,
							mMeas.mFixedInsts[FixedNum].sFrequency,
							mMeas.mFixedInsts[FixedNum].sTxHeight,
							mMeas.mMobiles[MobileNum].sMobileHeight,
							UseClutter, mMeas.mClutterClassGroup); 
/*					cout << "kFactor=" << mMeas.mkFactor 
						<<"	Freq = " << mMeas.mFixedInsts[FixedNum].sFrequency
						<< "	TxHeight = " << mMeas.mFixedInsts[FixedNum].sTxHeight 
						<< "	RxHeight = " << mMeas.mMobiles[MobileNum].sMobileHeight
						<< "	ClassGroup = " << mMeas.mClutterClassGroup
						<< "	PlotRes = " << mMeas.mPlotResolution;
					if (mMeas.mUseClutter)
						cout << "	Using Clutter" << endl;
					else cout << "	NOT using Clutter" << endl;	
				
*/				}

			}

			//Change settings if the Fixed Installation changed
			if (mMeas.mMeasPoints[i].sInstKeyFixed!=currentInst)
			{
//				cout << "i=" << i << "	NextInst = " << mMeasPoints[i].sInstKeyFixed << endl;
				if (CNumUsed>0)
				{
					CMean = CTotalError/CNumUsed;
					CMeanSquareError = CTotalSError/CNumUsed;
					CStDev = sqrt(CMeanSquareError-CMean*CMean);
	
					double CTempMeas = sqrt(CNumUsed*CTotalSMeas-CTotalMeas*CTotalMeas);
					double CTempPred = sqrt(CNumUsed*CTotalSPred-CTotalPred*CTotalPred);
					CCorrC = (CNumUsed*CTotalMeasPred - CTotalMeas*CTotalPred) 
						/ (CTempMeas*CTempPred);

/*					cout << "Inst: " << currentInst << "	#: " << CNumUsed  
						<< "	Freq =" << mMeas.mFixedInsts[FixedNum].sFrequency 
						<< "	M: "<< CMean 					
						<< "	MSE: " << CMeanSquareError 
						<< "	StDev: " << CStDev
						<< "	Corr: " << CCorrC << endl;
//					cout	<< "	AntVal: " << CsumOfAntValue 
//						<< " /N: "<< CsumOfAntValue/CNumUsed
//						<< "	PathLoss: " << CsumOfPathLoss 
//						<< " /N: " << CsumOfPathLoss/CNumUsed << endl;
*/
				}

				CNumUsed = 0;
				CError=0;
				CTotalError=0; 
				CTotalSError=0;
				CTotalMeas=0;
				CTotalPred=0;
				CTotalSMeas=0;
				CTotalSPred=0;
				CTotalMeasPred=0;
				CMean = 0;
				CMeanSquareError=0; 
				CStDev = 0;
				CCorrC = 0;
				CsumOfAntValue = 0;
				CsumOfPathLoss = 0;
				
				currentInst = mMeas.mMeasPoints[i].sInstKeyFixed;
				
				while ((mMeas.mFixedInsts[FixedNum].sInstKey!=currentInst)
					&&(FixedNum < mMeas.mFixedInsts.size()))
					FixedNum++;

				if (FixedNum == mMeas.mFixedInsts.size())
				{
					cout << "FixedNum reached limit ... ending measurement analysis"<<endl;			
					return 0;
				}

				FixedAnt.SetUseAntANN(mMeas.mUseAntANN);
//				cout << "Setting Antenna Pattern to match FixedInstallation" << endl;
				gDb.mgDBlock.lock();
				FixedAnt.SetAntennaPattern(mMeas.mFixedInsts[FixedNum].sInstKey, Tx,
							mMeas.mFixedInsts[FixedNum].sTxAzimuth,  
							mMeas.mFixedInsts[FixedNum].sTxMechTilt);
				gDb.mgDBlock.unlock();
				EIRP = mMeas.mFixedInsts[FixedNum].sTxPower 
					- mMeas.mFixedInsts[FixedNum].sTxSysLoss 
					+ FixedAnt.mGain + MobileAnt.mGain;
//				cout << "EIRP = " << EIRP << endl;

//				cout << "cGPpropModel::CostFunction: Setting Prediction parameters to match FixedInstallation" << endl;
				if (CalcNewObstruction)
				{
					PathLossPred.setParameters(mMeas.mkFactor,
								mMeas.mFixedInsts[FixedNum].sFrequency,
								mMeas.mFixedInsts[FixedNum].sTxHeight,
								mMeas.mMobiles[MobileNum].sMobileHeight,
								UseClutter, mMeas.mClutterClassGroup);
/*					cout << "kFactor=" << mMeas.mkFactor 
						<<"	Freq = " << mMeas.mFixedInsts[FixedNum].sFrequency
						<< "	TxHeight = " << mMeas.mFixedInsts[FixedNum].sTxHeight 
						<< "	RxHeight = " << mMeas.mMobiles[MobileNum].sMobileHeight
						<< "	ClassGroup = " << mMeas.mClutterClassGroup
						<< "	PlotRes = " << mMeas.mPlotResolution;
					if (mMeas.mUseClutter)
						cout << "	Using Clutter" << endl;
					else cout << "	NOT using Clutter" << endl;
*/				}
				
			}
			
			tempMeas.sDistance = mMeas.mFixedInsts[FixedNum].sSitePos.Distance(tempMeas.sPoint);
//			cout << "cGPpropModel::CostFunction:  Before mDEM.GetForLink" << endl;
			if (CalcNewObstruction)
			{
				
				mMeas.mDEM.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos, 
							tempMeas.sPoint, 
							mMeas.mPlotResolution, DEM);
				Length = DEM.GetSize();
				
			}
			else 
			{
				Length = tempMeas.sDistance/mMeas.mPlotResolution;
			}
//			cout << "cGPpropModel::CostFunction: After mDEM.GetForLink" << endl;
			
		
			if (Length > 3)
			{
				NumUsed ++ ;
				CNumUsed ++;

				if ((mMeas.mUseClutter)&&(CalcNewObstruction))
				{
					
//					cout << "cGPpropModel::CostFunction:  Before mClutter.GetForLink" << endl;
					mMeas.mClutterRaster.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos,
									tempMeas.sPoint,
									mMeas.mPlotResolution,Clutter);
				}

				if (CalcNewObstruction)
				{
					PathLoss = PathLossPred.TotPathLoss(DEM,Tilt,Clutter,DiffLoss,ClutterDistance);
					Distance = PathLossPred.getLinkLength();
					ClutterType = PathLossPred.get_Clutter();
					ClutterHeight = mCandidate[CIndex].sClutterHeight[ClutterType];
					QRapCore = PathLoss;

					tempMeas.sDistance = Distance;
					tempMeas.sTilt = Tilt;
					tempMeas.sDiffLoss = DiffLoss;
					tempMeas.sClutterDistance = ClutterDistance;
					tempMeas.sClutter = ClutterType;
					tempMeas.sClutterHeight = ClutterHeight; 

//					cout << " RawLoss = " << PathLoss
//						<< "	O_L = " << DiffLoss;
//					cout << endl;

					if (DiffLoss>0)
						NLOS[ClutterType]++;
					else
						LOS[ClutterType]++;

				}
				
				tempMeas = mCandidate[CIndex].sTree->eval(tempMeas);

				PathLoss = tempMeas.sReturn;
	
				if ((isnan(PathLoss))||(isinf(PathLoss)))
				{
//					cout << "Invalid pathloss returned from tree" << endl;
					tempMeas.sReturn = -200;
					tempMeas.sPathLoss = -200;
				}
//				cout <<endl;	
//				cout << "	TreeLoss = " << tempMeas.sPathLoss << endl;

				tempMeas.sPathLoss = PathLoss;
				tempMeas.sAzimuth = mMeas.mFixedInsts[FixedNum].sSitePos.
									Bearing(mMeas.mMeasPoints[i].sPoint);

				AntValue = FixedAnt.GetPatternValue(tempMeas.sAzimuth, tempMeas.sTilt)
									+ MobileAnt.GetPatternValue(0, 
									- tempMeas.sTilt);

				tempMeas.sPredValue = - tempMeas.sPathLoss + EIRP - AntValue;
//				cout << "tempMeas.sDistance = " << tempMeas.sDistance << endl;
/*				if ((fabs(QRapCore-tempMeas.sPathLoss)>0.1)&&(CalcNewObstruction))
				{
					cout << "Difference in results:  CoreAlgorithm = " << QRapCore 
						<< "	Tree = " << tempMeas.sPathLoss 
						<<  "	AntValue = " << AntValue << endl;
				}
*/
				Error = - tempMeas.sMeasValue +tempMeas.sPredValue;
/*				cout << "i= " << i << "	err= " << Error 
					<< "	PL= " << tempMeas.sPathLoss
					<< "	meas= " << tempMeas.sMeasValue << endl;
*/				TotalError += Error;  
				TotalSError += Error*Error;
				TotalMeas += tempMeas.sMeasValue; 
				TotalSMeas += tempMeas.sMeasValue * tempMeas.sMeasValue; 
				TotalPred += tempMeas.sPredValue;
				TotalSPred += tempMeas.sPredValue * tempMeas.sPredValue;				
				TotalMeasPred+= tempMeas.sPredValue * tempMeas.sMeasValue;

				CTotalError += Error;  
				CTotalSError += Error*Error;
				CTotalMeas += tempMeas.sMeasValue; 
				CTotalSMeas +=tempMeas.sMeasValue * tempMeas.sMeasValue; 
				CTotalPred += tempMeas.sPredValue;
				CTotalSPred += tempMeas.sPredValue * tempMeas.sPredValue;				
				CTotalMeasPred+= tempMeas.sPredValue * tempMeas.sMeasValue;
				CsumOfAntValue += fabs(AntValue);
				CsumOfPathLoss += tempMeas.sPathLoss;

			}
		}// if included in calculations
		// writing needs to be under 'mutex'
//		mMeas.mMeasPoints[i] = tempMeas;
	}//for all measurements


	if (CNumUsed>0)
	{
		CMean = CTotalError/CNumUsed;
		CMeanSquareError = CTotalSError/CNumUsed;
		CStDev = sqrt(CMeanSquareError-CMean*CMean);
		double CTempMeas = sqrt(CNumUsed*CTotalSMeas-CTotalMeas*CTotalMeas);
		double CTempPred = sqrt(CNumUsed*CTotalSPred-CTotalPred*CTotalPred);
		CCorrC = (CNumUsed*CTotalMeasPred - CTotalMeas*CTotalPred) / (CTempMeas*CTempPred);
/*		cout << "Inst: " << currentInst << "	#: " << CNumUsed  
			<< "	Freq =" << mMeas.mFixedInsts[FixedNum].sFrequency 
			<< "	M: "<< CMean 					
			<< "	MSE: " << CMeanSquareError 
			<< "	StDev: " << CStDev
			<< "	Corr: " << CCorrC << endl;
//		cout	<< "	AntVal: " << CsumOfAntValue << " /N: "<< CsumOfAntValue/CNumUsed
//			<< "	PathLoss: " << CsumOfPathLoss << " /N: " << CsumOfPathLoss/CNumUsed << endl;
*/	}

	if (NumUsed>0)
	{
		Mean = TotalError/NumUsed;
		MeanSquareError = TotalSError/NumUsed;
		StDev = sqrt(MeanSquareError-Mean*Mean);
	
		double TempMeas = sqrt(NumUsed*TotalSMeas-TotalMeas*TotalMeas);
		double TempPred = sqrt(NumUsed*TotalSPred-TotalPred*TotalPred);
		CorrC = (NumUsed*TotalMeasPred - TotalMeas*TotalPred) / (TempMeas*TempPred);
	}
	else cout << "No predictions done." << endl;
/*
	for (j=0;j<mPathLoss.mClutter.mNumber;j++)
	{
		cout << j<< "	Num: " << mClutterCount[j];
		cout << "	#LOS: " << LOS[j];
		cout << "	#NLOS: " << NLOS[j] << endl;
	}
*/

//	cout << " Leaving cGPpropModel::CostFunction   NumUsed = " << NumUsed << endl;
	delete [] LOS;
	delete [] NLOS;
	delete [] ClutterOccur;

	return NumUsed;
}

//**************************************************************************
bool cGPpropModel::optimiseConstants(unsigned Index)
{
	double oldCorrC, oldStdDev, oldMean, oldMSE;
	double newCorrC, newStdDev, newMean, newMSE;
	double ACorrC, AStdDev;
	double alphaC, alphaS;
	double Mean, MSE;

	double TempDelta=0, DeltaA = 5.0/0.8;
	double OmegaSize =0.0, OmegaI;
	unsigned i=0, numConsts=0;
	bool Continue = true;
	bool NotDecreasing = true;
	numConsts = mCandidate[Index].sConstants.size();
	double * Delta;
	double * DiffC;
	double * DiffS;
	double * OldValue;
	Delta = new double[numConsts];
	DiffC = new double[numConsts];
	DiffS = new double[numConsts];
	OldValue = new double[numConsts];
	double ModOfDiffC = 0.0;
	double ModOfDiffS = 0.0;
	double ProductCS = 0.0;
	double SizeDelta = 0.0;

	cout << endl;
	cout << "Optimising Constants for Candidate Tree. INDEX = " << Index << endl;
	for (i=0; i<numConsts; i++)
	{
		OldValue[i] = mCandidate[Index].sConstants[i]->getValue();
//		cout << "	" << OldValue[i];
		if (fabs(OldValue[i])>0.0001)
			Delta[i] = -0.01*fabs(OldValue[i])/0.9;
		else Delta[i] = -0.01/0.9;
		DiffC[i] = 0.0;
		DiffS[i] = 0.0;
	}
//	cout << endl;

	CostFunction(Index, oldMean ,oldMSE, oldStdDev, oldCorrC, false);
//	cout << "OldCost:  Index = " << Index << "		CorrC=" << oldCorrC 
//		<< "	StDev=" << oldStdDev << "	Mean=" << oldMean  <<  "	MSE=" << oldMSE << endl;

	Continue = true;
	while (Continue)
	{
		NotDecreasing = true;
		ModOfDiffC = 0.0;
		ModOfDiffS = 0.0;
		ProductCS = 0.0;
		DeltaA *=2 ;
		for (i=0; i<numConsts; i++)
		{
			newStdDev = 1000;
			newCorrC = 0.0;
			newMSE = 200000;
			DiffC[i] = 0.0;
			DiffS[i] = 0.0;
			OldValue[i] = mCandidate[Index].sConstants[i]->getValue();
			if (fabs(OldValue[i])>0.0001)
				Delta[i] = -0.01*fabs(OldValue[i])/0.9;
			else Delta[i] = -0.01/0.9;
//			cout << "Delta[i]= "<< Delta[i];
			while ( (fabs(Delta[i])>1e-6)
				&&( (fabs(newStdDev)>900)||(newCorrC<0.0)
				||(isnan(newStdDev))||(isinf(newStdDev))||(newStdDev<0)
				||(isnan(newMSE))||(isinf(newMSE))
				||(isnan(newCorrC))||(isinf(newCorrC)) ) )
			{
				Delta[i] *=-0.9;
				mCandidate[Index].sConstants[i]->setValue(OldValue[i] + Delta[i]);
				CostFunction(Index, newMean, newMSE, newStdDev, newCorrC, false);
			}
/*			cout << "	OldValue[i]="<< OldValue[i] << " 	Ci = " << i 
				<< "		CorrC=" << oldCorrC 
				<< "	StDev=" << oldStdDev << "	Mean=" << oldMean  
				<< "	MSE=" << oldMSE << endl;
			cout << "Delta[i]= "<< Delta[i] << "	Ci = " << i 
				<< "		CorrC=" << newCorrC 
				<< "	StDev=" << newStdDev << "	Mean=" << newMean  
				<< "	MSE=" << newMSE << endl;
*/
			DiffC[i] = (100.0*(oldCorrC -newCorrC))*Delta[i]/fabs(Delta[i]); //
			DiffS[i] = (newStdDev - oldStdDev)*Delta[i]/fabs(Delta[i]);
			
//			cout<<"DiffC[i]:"<<DiffC[i]<<"	DiffS[i]:"<<DiffS[i] <<"	Delta[i]:"<<Delta[i] << endl; 

			if ((newStdDev<900)&&(newCorrC>0.0))
			{
				ModOfDiffC += DiffC[i]*DiffC[i]/(Delta[i]*Delta[i]);
				ModOfDiffS += DiffS[i]*DiffS[i]/(Delta[i]*Delta[i]);
				ProductCS +=  DiffC[i]*DiffS[i]/(Delta[i]*Delta[i]);
			}
			else
			{
//				cout << "Make zero" << endl;
				DiffC[i] = 0.0;
				DiffS[i] = 0.0;
				Delta[i] = 1e-10;
				NotDecreasing = false;
				Continue = false;
			}
			//return value to original value
			mCandidate[Index].sConstants[i]->setValue(OldValue[i]);
		} // for mNumConst

//		cout << "MC:" << ModOfDiffC << "	MS:" << ModOfDiffS << "	PCS:" << ProductCS << endl; 
		if ((ProductCS<ModOfDiffC)&&(ProductCS<ModOfDiffS))
			alphaC = (ModOfDiffS - ProductCS)/(ModOfDiffS + ModOfDiffC - 2*ProductCS);
		else if (ModOfDiffS<ModOfDiffC) alphaC = 0.0;
		else alphaC = 1.0;
		alphaS = 1 - alphaC;
//		cout << "alphaC=" << alphaC << "	alphaS=" << alphaS << endl;		

		while (NotDecreasing)
		{
			DeltaA*=0.8;
			SizeDelta = 0.0;
			for (i=0; i<numConsts; i++)
			{
				TempDelta = -(alphaC*DiffC[i]/fabs(Delta[i]) + alphaS*DiffS[i]/fabs(Delta[i]))*DeltaA;
//				cout << TempDelta << "	";
				SizeDelta += TempDelta*TempDelta;
				mCandidate[Index].sConstants[i]->setValue(OldValue[i] + TempDelta);
			}
//			cout << endl;
			CostFunction(Index, Mean, MSE, AStdDev, ACorrC, false);
//			cout << "DeltaA = " << DeltaA << "		CorrC=" << ACorrC 
//				<< "	StDev=" << AStdDev << "	Mean=" << Mean  <<  "	MSE=" << MSE << endl;
			NotDecreasing = (AStdDev>=oldStdDev)||(ACorrC<=oldCorrC)
					||(isnan(AStdDev))||(isinf(AStdDev))||(AStdDev<0)
					||(isnan(MSE))||(isinf(MSE))
					||(isnan(ACorrC))||(isinf(ACorrC));
			NotDecreasing = NotDecreasing && (SizeDelta>1e-10);
		}
		if (Continue)
		{
			cout << Index << "	DeltaA = " << DeltaA << "		CorrC=" << ACorrC 
					<< "	StDev=" << AStdDev << "	Mean=" << Mean  <<  "	MSE=" << MSE << endl;
//			cout << endl;
			OmegaSize = 0;
			for (i=0; i<numConsts; i++)
			{
				OmegaI = alphaC*DiffC[i]/fabs(Delta[i]) + alphaS*DiffS[i]/fabs(Delta[i]);
				OmegaSize+= OmegaI*OmegaI;
				Delta[i] = -OmegaI*DeltaA;
				mCandidate[Index].sConstants[i]->setValue(OldValue[i] + Delta[i]);
			}
			CostFunction(Index, newMean, newMSE, newStdDev, newCorrC, false);
//				cout << "NewCost:  Index = " << Index << "		CorrC=" << newCorrC 
//					<< "	StDev=" << newStdDev << "	Mean=" << newMean  
//					<<  "	MSE=" << newMSE << endl;
		
			Continue = (OmegaSize>1e-6)&&(fabs((newStdDev-oldStdDev)/newStdDev)>1e-5)
					&&(fabs((newCorrC-oldCorrC)/newCorrC)>1e-5);
			oldCorrC = newCorrC; oldStdDev=newStdDev; oldMean=newMean; oldMSE=newMSE;
		}
	}
	CostFunction(Index, oldMean, oldMSE, oldStdDev, oldCorrC, false);
	mCandidate[Index].sOptimised = true;
	mCandidate[Index].sMean = oldMean;
	mCandidate[Index].sStdDev = oldStdDev;
	mCandidate[Index].sCorrC = oldCorrC;
}


//********************************************************************************
void cGPpropModel::mutateCandidate(unsigned Index, bool grow)
{
	unsigned j;
	mCandidate[Index].sOptimised =false;
/*	for (j=0; j<mMeas.mPathLoss.mClutter.mNumber; j++)
	{
		mCandidate[Index].sClutterHeight[j] *= (1.0 
			+ gGauss(gRandomGen)*mCandidate[Index].sFitness/(4*mMinFitness));
		if (mCandidate[Index].sClutterHeight[j]<0.0)
			mCandidate[Index].sClutterHeight[j]=0.0;
	}
*/	unsigned depth=0;
	mCandidate[Index].sDepth=mCandidate[Index].sTree->getTreeDepth(depth);
	depth = max(0, (int)mCandidate[Index].sDepth); 
//	cout << "Tree depth = " << TreeDepth << endl;
	double random = gGauss(gRandomGen);
//	cout << "	random=" << random;
	unsigned mutateDepth = min(depth, (unsigned)max(0,(int)(depth*(1-fabs(random)*mCandidate[Index].sFitness/mMinFitness))));
	random = gGauss(gRandomGen);
//	cout << "	random=" << random;
	double mutateProp = min(0.5, fabs(random)*mCandidate[Index].sFitness/(4*mMinFitness));
//	cout << "mutateProp = " << mutateProp << "	mutateDepth = " << mutateDepth << endl;	
	mutateTree(mCandidate[Index].sTree, mutateDepth, mutateProp, grow); 
}

//********************************************************************************
void cGPpropModel::crossOverCandidate(unsigned Index, unsigned donatorIndex)
{
}

//********************************************************************************
void cGPpropModel::replaceTree(unsigned Index)
{
}

//********************************************************************************	
void cGPpropModel::deleteCandidate(unsigned Index)
{
}

//*************************************************************************
void cGPpropModel::crossOverTree(GOftn* treeToAlter, GOftn* donatingTree)
{
	if ((treeToAlter->mNumChildren > 0) && (donatingTree->mNumChildren > 0)) 
	{
		//randomly select side of treeToAlter and donatingTree
		int childToAlter = rand() % treeToAlter->mNumChildren;
		int childToDonate = rand() % donatingTree->mNumChildren;
		//delete side of treeToAlter
		deleteTree(treeToAlter->mChild[childToAlter]);
		//clone side of donatingTree
		treeToAlter->mChild[childToAlter] = (donatingTree->mChild[childToDonate])->clone();
	}
	else
	{
		cout << "not enough child nodes to do crossover" << endl;
	}
}

//*************************************************************************
void cGPpropModel::mutateTree(GOftn* &inTree, int depth, bool grow, double PropMutate)
{
	if (inTree->mIsConstant)
	{
		inTree->mutate(PropMutate);
		return;
	}
	double randNum0t1 = rand()/(double)RAND_MAX;
	if (randNum0t1 < PropMutate) 
	{
//		cout << "SHOULD MUTATE" << endl;
		//create new node
		//create a random node to replace current node
		GOftn* newNode = createRandomNode(depth + 1);
		unsigned childrenToMove = min(newNode->mNumChildren, inTree->mNumChildren);
		for (unsigned i=0; i<childrenToMove; i++)
		{
			newNode->mChild[i] = inTree->mChild[i];
		}
		if ((inTree->mNumChildren>2)&&(inTree->mNumChildren>newNode->mNumChildren)&&(depth>2))
		{
			newNode->mNumChildren = childrenToMove;
		}
		else
		{
			unsigned numNewChildren = max(0,(int)newNode->mNumChildren - (int)childrenToMove);
			for (unsigned i=childrenToMove; i<(childrenToMove + numNewChildren); i++)
			{
				newNode->mChild[i] = createRandomTree(depth + 2, grow);
			}
		}
		if (inTree!=nullptr) deleteTree(inTree); //free memory held by old node
		inTree = newNode;	//replace old ptr with ptr to new
	}
	for (unsigned i=0; i<inTree->mNumChildren; i++) 
	{
		mutateTree(inTree->mChild[i], depth + 2, grow);
	}
}

//***************************************************************************
void cGPpropModel::printTree(GOftn* inTree, int depth)
{
	for (int j=0; j<depth; j++) 
	{
		printf(" . ");
	}
	cout << inTree->mLabel.c_str() <<endl;
	for (unsigned i=0; i<inTree->mNumChildren; i++) 
	{
		printTree(inTree->mChild[i], depth+1);
	}
}

//********************************************************************************
GOftn* cGPpropModel::createRandomTree(int depth, bool grow)
{
	unsigned i=0, begin =0;
	double random;
	int randn;
	GOftn* retFtn;
	if (0==depth)
	{
		randn = rand()%6+4;
		retFtn = new Add(randn);
		random = rand()%70 + 30;
		retFtn->mChild[0]= new ConstNode(random);
		retFtn->mChild[1]= new Multiply();
		random = (rand()%101)/100;
		retFtn->mChild[1]->mChild[0] = new ConstNode(random);
		retFtn->mChild[1]->mChild[1] = new ObstructionNode();
		for (i=2; i<4; i++)
		{
			retFtn->mChild[i]= new Multiply();
			random = rand()%16 + 20;
			retFtn->mChild[i]->mChild[0] = new ConstNode(random);
			retFtn->mChild[i]->mChild[1] = new Log10Node();
		}
		retFtn->mChild[2]->mChild[1]->mChild[0] = new FrequencyNode();
		retFtn->mChild[3]->mChild[1]->mChild[0] = new DistanceNode();
 		begin = 4;
	}
	else
	{
		retFtn = createRandomNode(depth,grow);
		begin=0;
	}
	for (i=begin; i < retFtn->mNumChildren; i++) 
	{
		retFtn->mChild[i] = createRandomTree(depth+1,grow);
	}
	return retFtn;
}

//***********************************************************************
void cGPpropModel::deleteTree(GOftn* inTree)
{
	 //free any children first
//	cout << "delete" << endl;
//	printTree(inTree);
	for (unsigned i=0; i<inTree->mNumChildren; i++) 
	{ 
		if (inTree->mChild[i]!=nullptr) deleteTree(inTree->mChild[i]);
	}
//	if ((inTree!=nullptr)/*&&(inTree->mNumChildren>0)*/) free(inTree);
}

//******************************************************************************
GOftn* cGPpropModel::createRandomNode(int depth, bool grow)
{

	int randn = 0;
	int numFtnChoices = 9;
	GOftn* retFtn;

/*	if (0==depth)
	{
		randn = rand() % 5 + 4;		
		retFtn = new Add(randn);
		return retFtn;
	}
	if (1==depth)
	{
		randn = rand() % 2 + 2;		
		retFtn = new Multiply(randn);
		return retFtn;
	}

	randn = rand() % 10;	
	if ((2==depth)&&(randn < 4))
	{
		retFtn = new Log10Node();
		return retFtn;
	}

	randn = rand() % 7;
	switch	(randn)
	{
		case 0:
			retFtn = new ConstNode();
			break;
		case 1:
			retFtn = new ObstructionNode();
			break;
		case 2:
			retFtn = new DistanceNode();
			break;
		case 3:
			retFtn = new FrequencyNode();
			break;
		default: break;
	}

*/

	//if the depth reached max depth only allow constants and inputs
	if (depth < MAX_TREE_DEPTH) numFtnChoices = 15;
	else numFtnChoices = 9;
	randn = rand() % numFtnChoices;	
	if ((grow)&&(depth < MAX_TREE_DEPTH))
		randn = rand() % 6 + 9;

	//generate random int
	
	
	switch (randn) 
	{	//move random 
		case 0:
			retFtn = new ConstNode();
			break;
		case 1:
			retFtn = new ObstructionNode();
			break;
		case 2:
			retFtn = new DistanceNode();
			break;
		case 3:
			retFtn = new FrequencyNode();
			break;
		case 4:
			retFtn = new TxHeightNode();
			break;
		case 5:
			retFtn = new RxHeightNode();
			break;
		case 6:
			retFtn = new ClutterTypeNode();
			break;
		case 7:
			retFtn = new ClutterHeightNode();
			break;
		case 8:
			retFtn = new ClutterDepthNode();
			break;
		case 9:
			retFtn = new Add();
			break;
		case 10:
			retFtn = new Multiply();
			break;
		case 11:
			retFtn = new Divide();
			break;
		case 12:
			retFtn = new Square();
			break;
		case 13:
			retFtn = new Log10Node();
			break;
		case 14:
			retFtn = new Power();
			break;

		default:
			cerr<<"invalid random number, bad news\n\n\n";
			break;
	}
	return retFtn;
}



//***************************************************************************
bool cGPpropModel::SortCriteriaOnCorrC(SCandidate c1, SCandidate c2)
{
	return (c1.sCorrC > c2.sCorrC);
}

//***************************************************************************
bool cGPpropModel::SortCriteriaOnStdDev(SCandidate c1, SCandidate c2)
{
	return (c1.sStdDev < c2.sStdDev);
}

//***************************************************************************
bool cGPpropModel::SortCriteriaOnRank(SCandidate c1, SCandidate c2)
{
	return (c1.sRank < c2.sRank);
}

//***************************************************************************
bool cGPpropModel::SortCriteriaOnFitness(SCandidate c1, SCandidate c2)
{
	return (c1.sFitness < c2.sFitness);
}


//******************************************************
int cGPpropModel::getRandSurvivor(unsigned popSize)
{
	int randn = rand() % (popSize - 2*mNumToDie);
	return (randn);
}



