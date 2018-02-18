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

using namespace Qrap;
using namespace std;

//**************************************************************************
cGPpropModel::cGPpropModel()
{
	mNumCandidates = NUM_INIT_CANDIDATES; 
	mNumToDie = (unsigned)(mNumCandidates*DEATH_RATE/100);
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
	unsigned i=0, j=0, k=0;
	string queryC, queryN;
	
	if (!gSettings.LoadFromFile("/usr/lib/qgis/plugins/settings.xml"))
		return 0;

	if(!gDb.Connect ("postgres", "postqrap"))
	{
		cout << "Database connect failed:" << endl;
		return 0;
	}

	cout << "Connected " << endl;

  	queryC = "update coefficients set coefficient=0.0;";
	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}

  	queryN = "update qrap_config set value='false' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(queryN))
	{
		cout << "Error updating qrap_config" << endl;
	}

	cGeoP *Hoek;
	unsigned NumHoek=19;
/*
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
*/

	vPoints Punte;
	for (i=0; i<NumHoek; i++)
		Punte.push_back(Hoek[i]);
	
	delete [] Hoek;

	mMeas.SetUseAntANN(false);

	mMeas.mPathLoss.mClutter.Reset(1);

	cout << "Loading measurements ... in main()" << endl;

	mMeas.SetPlotResolution(30);
	mMeas.LoadMeasurements(Punte,1,1,1);

	double Mean, MSE, StDev, CorrC;
	mMeas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);

	GOftn* newTree = nullptr;
	SCandidate newCandidate;
	unsigned ClutterFilter = 0;

	// ***********************************************************************
	// Initialise candidates of known models

	// Basic free space plus obstruction loss (Q-Rap basic model)
	newTree = new Add();
	newTree->mChild[0] = new Add();
	newTree->mChild[1] = new Add();
	newTree->mChild[0]->mChild[0] = new Multiply();
	newTree->mChild[0]->mChild[0]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[0]->mChild[0]->mChild[1] = new Log10Node();
	newTree->mChild[0]->mChild[0]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[0]->mChild[1] = new Multiply();
	newTree->mChild[0]->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[0]->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[0]->mChild[1]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[1]->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1]->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[1]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[1]->mChild[1]->mChild[1] = new ObstructionNode();

	newCandidate.sTree = newTree;
	mCandidate.push_back(newCandidate);

	// Hata - Suburban
/*	newTree = new Add();
	newTree->mChild[0] = new Add();
	newTree->mChild[1] = new Add();
	newTree->mChild[0]->mChild[0] = new Multiply();
	newTree->mChild[0]->mChild[0]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[0]->mChild[0]->mChild[1] = new Log10Node();
	newTree->mChild[0]->mChild[0]->mChild[1]->mChild[0] = new FrequencyNode();
	newTree->mChild[0]->mChild[1] = new Multiply();
	newTree->mChild[0]->mChild[1]->mChild[0] = new ConstNode(20.0);
	newTree->mChild[0]->mChild[1]->mChild[1] = new Log10Node();
	newTree->mChild[0]->mChild[1]->mChild[1]->mChild[0] = new DistanceNode();
	newTree->mChild[1]->mChild[0] = new ConstNode(32.45);
	newTree->mChild[1]->mChild[1] = new Multiply();
	newTree->mChild[1]->mChild[1]->mChild[0] = new ConstNode(1.0);
	newTree->mChild[1]->mChild[1]->mChild[1] = new ObstructionNode();

	newCandidate.sTree = newTree;
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
		mCandidate[i].sMSE = 9999999;
		mCandidate[i].sRank = 2*(mNumCandidates-1);
		mCandidate[i].sNumClutter = mClutter.mNumber;
		mCandidate[i].sClutterType  = new unsigned[mClutter.mNumber];
		mCandidate[i].sClutterHeight = new double[mClutter.mNumber];
		for (j=0; j<mClutter.mNumber; j++)
		{
			mCandidate[i].sClutterType[j] = mClutter.mClutterTypes[j].sLandCoverID;	
			mCandidate[i].sClutterHeight[j] = mClutter.mClutterTypes[j].sHeight;
		} 
	}

	
	for (k=0; k<NUM_GENERATIONS; k++) 
	{
		mNumCandidates = mCandidate.size();
		// test the performance of each one
		// this should be problem-dependent, and implemented in another file
		for (i=0; i<mNumCandidates; i++)
		{
			CostFunction(i, Mean,mCandidate[i].sMSE,
					StDev, mCandidate[i].sCorrC, ClutterFilter);
			
		}
		
		//sort by performance (sort in increasing order so we work on first N)
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnCorrC);
		for (i=0; i<mNumCandidates; i++)
			mCandidate[i].sRank = i;
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnMSE);
		for (i=0; i<mNumCandidates; i++)
			mCandidate[i].sRank += i;
		sort(mCandidate.begin(), mCandidate.end(), SortCriteriaOnRank);
		
		mNumToDie = (unsigned)(mNumCandidates*DEATH_RATE/100);
		for (i=0; i<mNumToDie; i++)
		{
			//toss out losers
			deleteTree(mCandidate[i].sTree);
			//randomly select one of the survivors and clone
			mCandidate[i].sTree = mCandidate[getRandSurvivor(mNumCandidates)].sTree->clone();
			//do cross over with survivors 
			//crossOver(mTree[i], mTree[getRandSurvior(mNumCandidates)]);
			//cross over with best
			crossOver(mCandidate[i].sTree, mCandidate[mNumCandidates - 1].sTree);  
			mutateTree(&mCandidate[i].sTree);	//mutate
			for (j=0; j<mClutter.mNumber; j++)
			{
				mCandidate[i].sClutterType[j] = mCandidate[mNumCandidates - 1].sClutterType[j];	
				mCandidate[i].sClutterHeight[j] = mCandidate[mNumCandidates - 1].sClutterHeight[j];
			}
		}
		cout << "Best candidate: CorrC = " << mCandidate[mNumCandidates - 1].sCorrC 
			<< "	MSE = " << mCandidate[mNumCandidates - 1].sMSE;
	}
	printTree(mCandidate[mNumCandidates - 1].sTree);
    	return 0;
}

//******************************************************************************
int cGPpropModel::CostFunction(unsigned CIndex, double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, unsigned Clutterfilter)
{

	unsigned SkipNumber = (unsigned)(mMeas.mNumMeas/NUM_POINT_PER_EVAL);
	unsigned FirstMeas = rand() % SkipNumber;
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
	double freq, htx;
	
	for (j=0;j<mMeas.mPathLoss.mClutter.mNumber;j++)
	{
		if (mMeas.mUseClutter) mMeas.mClutterCount[j]=0;
		LOS[j]=0;
		NLOS[j]=0;
		mMeas.mPathLoss.mClutter.mClutterTypes[j].sHeight = mCandidate[CIndex].sClutterHeight[j];
	}

	NumUsed = 0;

	if (0==mMeas.mFixedInsts.size()) return 0;

//	cout <<"cMeasAnalysisCalc::PerformAnalysis: mNumMeas = " << mNumMeas << endl;
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

					cout << "Inst: " << currentInst << "	#: " << CNumUsed  
						<< "	Freq =" << mMeas.mFixedInsts[FixedNum].sFrequency 
						<< "	M: "<< CMean 					
						<< "	MSE: " << CMeanSquareError 
						<< "	StDev: " << CStDev
						<< "	Corr: " << CCorrC << endl;

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


//				cout << "cMeasAnalysisCalc::PerformAnalysis: Setting Prediction parameters to match FixedInstallation" << endl;
				mMeas.mPathLoss.setParameters(mMeas.mkFactor,mMeas.mFixedInsts[FixedNum].sFrequency,
								mMeas.mFixedInsts[FixedNum].sTxHeight,
								mMeas.mMobiles[MobileNum].sMobileHeight,
								mMeas.mUseClutter, mMeas.mClutterClassGroup);
				
				freq = mMeas.mFixedInsts[FixedNum].sFrequency;
				htx = mMeas.mFixedInsts[FixedNum].sTxHeight;
			}
			
//			cout << "cMeasAnalysisCalc::PerformAnalysis:  Before mDEM.GetForLink" << endl;
			mMeas.mDEM.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos, mMeas.mMeasPoints[i].sPoint, 
							mMeas.mPlotResolution, DEM);
//			cout << "cMeasAnalysisCalc::PerformAnalysis: After mDEM.GetForLink" << endl;
			mMeas.mMeasPoints[i].sDistance = mMeas.mFixedInsts[FixedNum].sSitePos.Distance(mMeas.mMeasPoints[i].sPoint);
			Length = DEM.GetSize();
			
			if (Length > 3)
			{
				NumUsed ++ ;
				CNumUsed ++;

				if (mMeas.mUseClutter)
				{
					
//					cout << "cMeasAnalysisCalc::PerformAnalysis:  Before mClutter.GetForLink" << endl;
					mMeas.mClutterRaster.GetForLink(mMeas.mFixedInsts[FixedNum].sSitePos,
									mMeas.mMeasPoints[i].sPoint,
									mMeas.mPlotResolution,Clutter);
				}

				mMeas.mMeasPoints[i].sPathLoss = mMeas.mPathLoss.TotPathLoss(DEM,
									mMeas.mMeasPoints[i].sTilt,Clutter,
									mMeas.mMeasPoints[i].sDiffLoss, 
									mMeas.mMeasPoints[i].sClutterDistance);
				mMeas.mMeasPoints[i].sClutter = mMeas.mPathLoss.get_Clutter();
				mMeas.mMeasPoints[i].sClutterHeight = mCandidate[CIndex].sClutterHeight[mMeas.mMeasPoints[i].sClutter]; 

				mCandidate[CIndex].sTree->eval(mMeas.mMeasPoints[i]);

				mMeas.mMeasPoints[i].sPathLoss = mMeas.mMeasPoints[i].sReturn;

				if (DiffLoss>0)
					NLOS[mMeas.mMeasPoints[i].sClutter]++;
				else
					LOS[mMeas.mMeasPoints[i].sClutter]++;
						
				mMeas.mMeasPoints[i].sAzimuth = mMeas.mFixedInsts[FixedNum].sSitePos.
									Bearing(mMeas.mMeasPoints[i].sPoint);

				AntValue = FixedAnt.GetPatternValue(mMeas.mMeasPoints[i].sAzimuth, 
									mMeas.mMeasPoints[i].sTilt)
									+ MobileAnt.GetPatternValue(0, -mMeas.mMeasPoints[i].sTilt);

				mMeas.mMeasPoints[i].sPredValue = -mMeas.mMeasPoints[i].sPathLoss + EIRP - AntValue;
//				cout << "cMeasAnalysisCalc::PerformAnalysis pathloss=" << mMeasPoints[i].sPathLoss;
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
  		cout << "Inst: " << currentInst << "	#: " << CNumUsed <<"	M: " << CMean 
			<< "	MSE: " << CMeanSquareError 
			<< "	StDev: " << CStDev
			<< "	Corr: " << CCorrC << endl;

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

	cout << " Leaving cMeasAnalysisCalc::PerformAnalysis   NumUsed = " << NumUsed << endl;
	delete [] LOS;
	delete [] NLOS;
	delete [] ClutterOccur;

	return NumUsed;
}


//*************************************************************************
void cGPpropModel::crossOver(GOftn* treeToAlter, GOftn* donatingTree)
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
void cGPpropModel::mutateTree(GOftn** inTree, int depth)
{
	double randNum0t1 = rand()/(double)RAND_MAX;
	if (randNum0t1 < MUTATION_THRESH) 
	{
		cout << "SHOULD MUTATE" << endl;
		//create new node
		//create a random node to replace current node
		GOftn* newNode = createRandomNode(depth + 1);
		int childrenToMove = min(newNode->mNumChildren, (*inTree)->mNumChildren);
		for (int i=0; i<childrenToMove; i++)
		{
			newNode->mChild[i] = (*inTree)->mChild[i];
		}
		int numNewChildren = newNode->mNumChildren - childrenToMove;
		for (int i=childrenToMove; i<(childrenToMove + numNewChildren); i++)
		{
			newNode->mChild[i] = createRandomTree(depth + 1);
		}
		free(*inTree);		//free memory held by old node
		*inTree = newNode;	//replace old ptr with ptr to new
	}
	for (int i=0; i<(*inTree)->mNumChildren; i++) 
	{
		mutateTree(&(*inTree)->mChild[i], depth + 1);
	}
}

//***************************************************************************
void cGPpropModel::printTree(GOftn* inTree, int depth)
{
	for (int j=0; j<depth; j++) 
	{
		printf("..");
	}
	//printf("%s\n", inTree->label.c_str());
	cout << inTree->mLabel.c_str() <<endl;
	for (int i=0; i<inTree->mNumChildren; i++) 
	{
		printTree(inTree->mChild[i], depth+1);
	}
}

//********************************************************************************
GOftn* cGPpropModel::createRandomTree(int depth)
{
	GOftn* retFtn = createRandomNode(depth);
	
	for (int i=0; i < retFtn->mNumChildren; i++) 
	{
		retFtn->mChild[i] = createRandomTree(depth+1);
	}
	return retFtn;
}

//******************************************************************************
GOftn* cGPpropModel::createRandomNode(int depth)
{
	int randn = 0;
	int numFtnChoices = 2;
	GOftn* retFtn;

	//if the depth is greater than 3 only allow const or inputs
	if (depth < MAX_TREE_DEPTH) numFtnChoices = 16;
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
			retFtn = new Add();
			break;
		case 3:
			retFtn = new Log10Node();
			break;
		case 4:
			retFtn = new Multiply();
			break;
		case 5:
			retFtn = new Divide();
			break;
		case 6:
			retFtn = new Square();
			break;
		case 7:
			retFtn = new Log10Node();
			break;
		case 8:
			retFtn = new Power();
			break;
		case 9:
			retFtn = new DistanceNode();
			break;
		case 10:
			retFtn = new FrequencyNode();
			break;
		case 11:
			retFtn = new TxHeightNode();
			break;
		case 12:
			retFtn = new RxHeightNode();
			break;
		case 13:
			retFtn = new ClutterTypeNode();
			break;
		case 14:
			retFtn = new ClutterHeightNode();
			break;
		case 15:
			retFtn = new ClutterDepthNode();
			break;
		default:
			cerr<<"invalid random number, bad news\n\n\n";
			break;
	}
	return retFtn;
}

//***********************************************************************
void cGPpropModel::deleteTree(GOftn* inTree)
{
	 //free any children first
	for (int i=0; i<inTree->mNumChildren; i++) 
	{ 
		deleteTree(inTree->mChild[i]);
	}
	free(inTree);
}

//***************************************************************************
bool cGPpropModel::SortCriteriaOnCorrC(SCandidate c1, SCandidate c2)
{
	return (c1.sCorrC < c2.sCorrC);
}
//***************************************************************************
bool cGPpropModel::SortCriteriaOnMSE(SCandidate c1, SCandidate c2)
{
	return (c1.sMSE > c2.sMSE);
}
//***************************************************************************
bool cGPpropModel::SortCriteriaOnRank(SCandidate c1, SCandidate c2)
{
	return (c1.sRank>c2.sRank);
}
//******************************************************
int cGPpropModel::getRandSurvivor(unsigned popSize)
{
	int randn = rand() % (popSize - mNumToDie);
	return (mNumToDie + randn);
}
