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
	bool UseClutter = true;
	unsigned ClassGroup=1;
	mMeas.mPathLoss.set_Clutter(UseClutter, ClassGroup); 
	cout << "Leaving cGPpropModel::cGPpropModel()" << endl;
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
}

//*************************************************************************
int cGPpropModel:: mainTuning()
{
	cout << "Entering cGPpropModel:: mainTuning()" << endl;
	unsigned i=0, j=0, k=0;

	cGeoP *Hoek;
	unsigned NumHoek=4;

//Gauteng 20m DEM
/*	NumHoek=19;
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
*/
//	Bryanston
	NumHoek=4;
	Hoek = new cGeoP[NumHoek];
	Hoek[0].Set(-26.036, 27.976);
	Hoek[1].Set(-26.108, 27.976);
	Hoek[2].Set(-26.108, 28.074);
	Hoek[3].Set(-26.036, 28.074);

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
	GOftn* newTree = nullptr;
	SCandidate newCandidate;
	

	// ***********************************************************************
	// Initialise candidates of known models

/*	// Basic free space plus obstruction loss (Q-Rap basic model)
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
*/
/*
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
	newTree->mChild[3]->mChild[0] = new ConstNode(0.0);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();

	newCandidate.sTree = newTree;
	newCandidate.sDepth = 3;
	mCandidate.push_back(newCandidate);
*/
/*
	// Tuned Q-Rap model ... classic tuning
	newTree = new Add(7);
	newTree->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[0] = new ConstNode(34.14);
	newTree->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[1]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[2] = new Multiply();
	newTree->mChild[2]->mChild[0] = new ConstNode(38.50);
	newTree->mChild[2]->mChild[1] = new Log10Node();
	newTree->mChild[2]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[3] = new Multiply();
	newTree->mChild[3]->mChild[0] = new ConstNode(0.34);
	newTree->mChild[3]->mChild[1] = new ObstructionNode();
	newTree->mChild[4] = new Multiply(3);
	newTree->mChild[4]->mChild[0] = new ConstNode(-14.16);
	newTree->mChild[4]->mChild[1] = new Log10Node();
	newTree->mChild[4]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[4]->mChild[2] = new Log10Node();
	newTree->mChild[4]->mChild[2]->mChild[0] = new TxHeightNode();
	newTree->mChild[5] = new Multiply();
	newTree->mChild[5]->mChild[0] = new ConstNode(-4.75);
	newTree->mChild[5]->mChild[1] = new Log10Node();
	newTree->mChild[5]->mChild[1]->mChild[0] = new TxHeightNode();
	newTree->mChild[6] = new Multiply();
	newTree->mChild[6]->mChild[0] = new ConstNode(-0.48);
	newTree->mChild[6]->mChild[1] = new Power();
	newTree->mChild[6]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[6]->mChild[1]->mChild[1] = new ConstNode(0.5);

	
	newCandidate.sTree = newTree;
	newCandidate.sDepth = 4;
	mCandidate.push_back(newCandidate);
*/
/*
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
	newTree->mChild[3]->mChild[0] = new ConstNode(0.0);
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
*/

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
	for (i=0; i<mNumCandidates; i++)
	{
		mCandidate[i].sCorrC = -1.0;
		mCandidate[i].sStdDev = 999;
		mCandidate[i].sRank = 2*(mNumCandidates-1);
		mCandidate[i].sPareto = true;
		mCandidate[i].sNumClutter = mClutter.mNumber;
		mCandidate[i].sClutterType  = new unsigned[mClutter.mNumber];
		mCandidate[i].sClutterHeight = new double[mClutter.mNumber];
		mCandidate[i].sDepth=mCandidate[i].sTree->getTreeDepth();
		for (j=0; j<mClutter.mNumber; j++)
		{
			mCandidate[i].sClutterType[j] = mClutter.mClutterTypes[j].sLandCoverID;	
			mCandidate[i].sClutterHeight[j] = mClutter.mClutterTypes[j].sHeight;
		} 
	}

	unsigned OldRank;
	unsigned IndexForCrossOver = 0;
	unsigned Nsamples=0;
	for (k=0; k<NUM_GENERATIONS; k++) 
	{
		cout << "GENERATION = " << k << endl;
		mNumCandidates = mCandidate.size();
		// test the performance of each one
		// this should be problem-dependent, and implemented in another file
		for (i=0; i<mNumCandidates; i++)
		{
			Nsamples = CostFunction(i, mCandidate[i].sMean ,MSE,
					 mCandidate[i].sStdDev, mCandidate[i].sCorrC);
			cout << "i = " << i << "	Rank=" << mCandidate[i].sRank
				<<"		CorrC=" << mCandidate[i].sCorrC << "	StDev=" << mCandidate[i].sStdDev
				<< "	Mean=" << mCandidate[i].sMean  << "	Depth=" << mCandidate[i].sDepth
				<< "	MSE=" << MSE <<"	N=" << Nsamples << endl ;
			if ((isnan(mCandidate[i].sStdDev))
				||(isinf(mCandidate[i].sStdDev))
				||(mCandidate[i].sStdDev<0)
				||(isinf(MSE))||(isnan(MSE)))
			{
				mCandidate[i].sStdDev = 999;
			}
			if ((isnan(mCandidate[i].sCorrC))
				||(isinf(mCandidate[i].sCorrC))
				||(mCandidate[i].sCorrC<-1.0)
				||(isinf(MSE))||(isnan(MSE)))
			{
				mCandidate[i].sCorrC = -0.999;
			}
			
		}
		
		//sort by performance (sort in increasing order so we work on first N)
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnCorrC);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	CorrC = " << mCandidate[i].sCorrC << endl;
			mCandidate[i].sRank = i;
		}
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnStdDev);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	StdDev = " << mCandidate[i].sStdDev << endl;
			mCandidate[i].sRank += i;
		}
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnRank);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	Rank = " << mCandidate[i].sRank << endl;
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
			if (mCandidate[i].sPareto) mCandidate[i].sRank=1;
		} 
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnRank);
		for (i=0; i<mNumCandidates; i++)
		{
//			cout << i << "	Rank = " << mCandidate[i].sRank << endl;
		}
		
		mNumToDie = (unsigned)(mNumCandidates*DEATH_RATE/100);

		//Clone and Mutate
		for (i=0; i<mNumToDie; i++)
		{
			//replace worst of population with mutations of best
			mCandidate[mNumCandidates-1-i-mNumToDie] = mCandidate[i];
			mCandidate[mNumCandidates-1-i-mNumToDie].sPareto=false;
			mCandidate[mNumCandidates-1-i-mNumToDie].sRank=mNumCandidates;
			mutateCandidate(mNumCandidates-mNumToDie-1-i);
		}		

		//Clone, Mutate and CrossOver
		for (i=0; i<mNumToDie; i++)
		{
			//replace worst of population with mutations of bests
			mCandidate[mNumCandidates-1-i] = mCandidate[i];
			mCandidate[mNumCandidates-1-i].sPareto=false;
			mCandidate[mNumCandidates-1-i].sRank=2*mNumCandidates;
			mutateCandidate(mNumCandidates-1-i);
			IndexForCrossOver = mNumCandidates -1;
			while (IndexForCrossOver>(mNumCandidates-mNumToDie))
				IndexForCrossOver = (unsigned)(fabs(gGauss(gRandomGen)*(mNumCandidates-2*mNumToDie)));	
			crossOverTree(mCandidate[mNumCandidates-1-i].sTree, mCandidate[IndexForCrossOver].sTree);  
		}
		cout << "Best candidate:	CorrC = " << mCandidate[0].sCorrC 
			<< "	StdDev = " << mCandidate[0].sStdDev << endl << endl;
	}
	i=0;
	while (mCandidate[i].sPareto)
	{
		printTree(mCandidate[i].sTree);
		for (j=0;j<mCandidate[i].sNumClutter;j++)
		{
			cout << "	[" << j<< "] " << mCandidate[i].sClutterHeight[j];
		}
		cout << endl << endl;
		i++;
	}
    	return 0;
}

//******************************************************************************
int cGPpropModel::CostFunction(unsigned CIndex, double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, unsigned Clutterfilter)
{
	bool *ClutterOccur;
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

	double DiffLoss = 0;
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

	//These varaibles are local and are such that they can be used with the TERMs defined in cClutter.h
//	double freq, htx;
	
	for (j=0;j<mMeas.mPathLoss.mClutter.mNumber;j++)
	{
		if (mMeas.mUseClutter) mMeas.mClutterCount[j]=0;
		LOS[j]=0;
		NLOS[j]=0;
		mMeas.mPathLoss.mClutter.mClutterTypes[j].sHeight = mCandidate[CIndex].sClutterHeight[j];
	}

	NumUsed = 0;
	CNumUsed = 0;

	if (0==mMeas.mFixedInsts.size()) return 0;

//	cout <<"cGPpropModel::CostFunction: mNumMeas = " << mNumMeas << endl;

	unsigned SkipNumber = min((mMeas.mNumMeas/NUM_POINT_PER_EVAL),20*mCandidate[CIndex].sRank/mNumCandidates);
	if (SkipNumber<1) SkipNumber = 1;
	if ((mCandidate[CIndex].sPareto)&&(k>0)) SkipNumber = 1;
	unsigned FirstMeas = SkipNumber * ((double) rand() / (RAND_MAX));
//	cout << "SkipNumber=" << SkipNumber <<"	FirstMeas = " << FirstMeas << endl;
	for (i=FirstMeas; i<mMeas.mNumMeas; i=i+SkipNumber)
	{
		if ((0==Clutterfilter)||(0==mMeas.mMeasPoints[i].sClutter)
			||(Clutterfilter==mMeas.mMeasPoints[i].sClutter))
		{

			//Change settings if the mobile installation changed
			if (mMeas.mMeasPoints[i].sInstKeyMobile!=currentMobile)
			{
				currentMobile = mMeas.mMeasPoints[i].sInstKeyMobile;
				while ((mMeas.mMobiles[MobileNum].sInstKey!=currentMobile)
					&&(MobileNum < mMeas.mMobiles.size()))
					MobileNum++;
				if (MobileNum == mMeas.mMobiles.size())
				{
					cout << "MobileNum reached limit ... ending measurement analysis" <<endl;
					return 0;
				}
//				cout << "Setting Mobile Antenna, mMobiles[MobileNum].sInstKey =" << mMobiles[MobileNum].sInstKey << endl;
				MobileAnt.SetAntennaPattern(mMeas.mMobiles[MobileNum].sInstKey, Mobile, 0, 0);

//				cout << "Setting Path loss parameters" << endl;
//				cout << "FixedNum = " << FixedNum << endl;

				mMeas.mPathLoss.setParameters(mMeas.mkFactor,mMeas.mFixedInsts[FixedNum].sFrequency,
								mMeas.mFixedInsts[FixedNum].sTxHeight,
								mMeas.mMobiles[MobileNum].sMobileHeight,
								mMeas.mUseClutter, mMeas.mClutterClassGroup); 
//				cout << "TxHeight=" << mMeas.mFixedInsts[FixedNum].sTxHeight << endl;
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
					CCorrC = (CNumUsed*CTotalMeasPred - CTotalMeas*CTotalPred) / (CTempMeas*CTempPred);
/*
					cout << "Inst: " << currentInst << "	#: " << CNumUsed  
						<< "	Freq =" << mMeas.mFixedInsts[FixedNum].sFrequency 
						<< "	M: "<< CMean 					
						<< "	MSE: " << CMeanSquareError 
						<< "	StDev: " << CStDev
						<< "	Corr: " << CCorrC << endl;
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
				FixedAnt.SetAntennaPattern(mMeas.mFixedInsts[FixedNum].sInstKey, Tx,
							mMeas.mFixedInsts[FixedNum].sTxAzimuth,  
							mMeas.mFixedInsts[FixedNum].sTxMechTilt);
				EIRP = mMeas.mFixedInsts[FixedNum].sTxPower 
					- mMeas.mFixedInsts[FixedNum].sTxSysLoss 
					+ FixedAnt.mGain + MobileAnt.mGain;


//				cout << "cGPpropModel::CostFunction: Setting Prediction parameters to match FixedInstallation" << endl;
				mMeas.mPathLoss.setParameters(mMeas.mkFactor,mMeas.mFixedInsts[FixedNum].sFrequency,
								mMeas.mFixedInsts[FixedNum].sTxHeight,
								mMeas.mMobiles[MobileNum].sMobileHeight,
								mMeas.mUseClutter, mMeas.mClutterClassGroup);
				
//				freq = mMeas.mFixedInsts[FixedNum].sFrequency;
//				htx = mMeas.mFixedInsts[FixedNum].sTxHeight;
			}
			
//			cout << "cGPpropModel::CostFunction:  Before mDEM.GetForLink" << endl;
			mMeas.mDEM.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos, mMeas.mMeasPoints[i].sPoint, 
							mMeas.mPlotResolution, DEM);
//			cout << "cGPpropModel::CostFunction: After mDEM.GetForLink" << endl;
			mMeas.mMeasPoints[i].sDistance = mMeas.mFixedInsts[FixedNum].sSitePos.Distance(mMeas.mMeasPoints[i].sPoint);
			Length = DEM.GetSize();
			
			if (Length > 3)
			{
				NumUsed ++ ;
				CNumUsed ++;

				if (mMeas.mUseClutter)
				{
					
//					cout << "cGPpropModel::CostFunction:  Before mClutter.GetForLink" << endl;
					mMeas.mClutterRaster.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos,
									mMeas.mMeasPoints[i].sPoint,
									mMeas.mPlotResolution,Clutter);
				}

				mMeas.mMeasPoints[i].sPathLoss = mMeas.mPathLoss.TotPathLoss(DEM,
									mMeas.mMeasPoints[i].sTilt,Clutter,
									mMeas.mMeasPoints[i].sDiffLoss, 
									mMeas.mMeasPoints[i].sClutterDistance);
				DiffLoss=mMeas.mMeasPoints[i].sDiffLoss;
				mMeas.mMeasPoints[i].sClutter = mMeas.mPathLoss.get_Clutter();
//				cout << " RawLoss = " << mMeas.mMeasPoints[i].sPathLoss
//					<< "	O_L = " << mMeas.mMeasPoints[i].sDiffLoss;
//				cout << endl;
				mMeas.mMeasPoints[i].sClutterHeight = mCandidate[CIndex].sClutterHeight[mMeas.mMeasPoints[i].sClutter]; 
				mMeas.mMeasPoints[i] = mCandidate[CIndex].sTree->eval(mMeas.mMeasPoints[i]);

				mMeas.mMeasPoints[i].sPathLoss = mMeas.mMeasPoints[i].sReturn;
			
				if ((isnan(mMeas.mMeasPoints[i].sPathLoss))||(isinf(mMeas.mMeasPoints[i].sPathLoss)))
				{
					mMeas.mMeasPoints[i].sReturn = -9999;
					mMeas.mMeasPoints[i].sPathLoss = -9999;
				}
//				cout <<endl;	
//				cout << "	TreeLoss = " << mMeas.mMeasPoints[i].sPathLoss << endl;

				if (DiffLoss>0)
					NLOS[mMeas.mMeasPoints[i].sClutter]++;
				else
					LOS[mMeas.mMeasPoints[i].sClutter]++;
						
				mMeas.mMeasPoints[i].sAzimuth = mMeas.mFixedInsts[FixedNum].sSitePos.
									Bearing(mMeas.mMeasPoints[i].sPoint);

				AntValue = FixedAnt.GetPatternValue(mMeas.mMeasPoints[i].sAzimuth, 
									mMeas.mMeasPoints[i].sTilt)
									+ MobileAnt.GetPatternValue(0, 
									- mMeas.mMeasPoints[i].sTilt);

				mMeas.mMeasPoints[i].sPredValue = -mMeas.mMeasPoints[i].sPathLoss + EIRP - AntValue;
//				cout << "cGPpropModel::CostFunction pathloss=" << mMeasPoints[i].sPathLoss;
//				cout << "	AntValue=" << AntValue << endl;

				Error = - mMeas.mMeasPoints[i].sMeasValue + mMeas.mMeasPoints[i].sPredValue;
				TotalError += Error;  
				TotalSError += Error*Error;
				TotalMeas += mMeas.mMeasPoints[i].sMeasValue; 
				TotalSMeas += mMeas.mMeasPoints[i].sMeasValue
						*mMeas.mMeasPoints[i].sMeasValue, 
				TotalPred += mMeas.mMeasPoints[i].sPredValue;
				TotalSPred += mMeas.mMeasPoints[i].sPredValue
						*mMeas.mMeasPoints[i].sPredValue;				
				TotalMeasPred+= mMeas.mMeasPoints[i].sPredValue
						*mMeas.mMeasPoints[i].sMeasValue;

				CTotalError += Error;  
				CTotalSError += Error*Error;
				CTotalMeas += mMeas.mMeasPoints[i].sMeasValue; 
				CTotalSMeas += mMeas.mMeasPoints[i].sMeasValue
						*mMeas.mMeasPoints[i].sMeasValue, 
				CTotalPred += mMeas.mMeasPoints[i].sPredValue;
				CTotalSPred += mMeas.mMeasPoints[i].sPredValue
						*mMeas.mMeasPoints[i].sPredValue;				
				CTotalMeasPred+= mMeas.mMeasPoints[i].sPredValue
						*mMeas.mMeasPoints[i].sMeasValue;

			}
		}// if included in calculations
	}//for all measurements


	if (CNumUsed>0)
	{
		CMean = CTotalError/CNumUsed;
		CMeanSquareError = CTotalSError/CNumUsed;
		CStDev = sqrt(CMeanSquareError-CMean*CMean);
		double CTempMeas = sqrt(CNumUsed*CTotalSMeas-CTotalMeas*CTotalMeas);
		double CTempPred = sqrt(CNumUsed*CTotalSPred-CTotalPred*CTotalPred);
		CCorrC = (CNumUsed*CTotalMeasPred - CTotalMeas*CTotalPred) / (CTempMeas*CTempPred);
/*  		cout << "Inst: " << currentInst << "	#: " << CNumUsed <<"	M: " << CMean 
			<< "	MSE: " << CMeanSquareError 
			<< "	StDev: " << CStDev
			<< "	Corr: " << CCorrC << endl;
*/
	}

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

//********************************************************************************
void cGPpropModel::mutateCandidate(unsigned Index)
{
	unsigned j;
	for (j=0; j<mClutter.mNumber; j++)
	{
		mCandidate[Index].sClutterHeight[j] *= (1.0+(1.0-mCandidate[Index].sCorrC)*gGauss(gRandomGen));
		if (mCandidate[Index].sClutterHeight[j]<0.0)
			mCandidate[Index].sClutterHeight[j]=0.0;
	}
	unsigned depth=0;
	mCandidate[Index].sDepth=mCandidate[Index].sTree->getTreeDepth(depth);
	depth = max(0, (int)mCandidate[Index].sDepth); 
//	cout << "Tree depth = " << TreeDepth << endl;
	unsigned mutateDepth = min(depth, (unsigned)max(0,(int)(depth*(1-fabs(gGauss(gRandomGen))*mCandidate[Index].sStdDev/3))));
	double mutateProp = min(0.5, fabs(gGauss(gRandomGen))*mCandidate[Index].sStdDev/3);
//	cout << "mutateProp = " << mutateProp << "	mutateDepth = " << mutateDepth << endl;	
	mutateTree(mCandidate[Index].sTree, mutateDepth, mutateProp); 
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
void cGPpropModel::mutateTree(GOftn* &inTree, int depth, double PropMutate)
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
				newNode->mChild[i] = createRandomTree(depth + 2);
			}
		}
		if (inTree!=nullptr) deleteTree(inTree); //free memory held by old node
		inTree = newNode;	//replace old ptr with ptr to new
	}
	for (unsigned i=0; i<inTree->mNumChildren; i++) 
	{
		mutateTree(inTree->mChild[i], depth + 2);
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
GOftn* cGPpropModel::createRandomTree(int depth)
{
	GOftn* retFtn = createRandomNode(depth);
	
	for (unsigned i=0; i < retFtn->mNumChildren; i++) 
	{
		retFtn->mChild[i] = createRandomTree(depth+1);
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
GOftn* cGPpropModel::createRandomNode(int depth)
{

	int randn = 0;
	int numFtnChoices = 9;
	GOftn* retFtn;

	if (0==depth)
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


	//if the depth is greater than 3 only allow const or inputs
	if (depth < MAX_TREE_DEPTH) numFtnChoices = 15;
	else numFtnChoices = 9;
	//generate random int
	randn = rand() % numFtnChoices;		
	
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

//******************************************************
int cGPpropModel::getRandSurvivor(unsigned popSize)
{
	int randn = rand() % (popSize - mNumToDie);
	return (randn);
}
