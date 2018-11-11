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

#ifndef Qrap_cGPpropModel_h
#define Qrap_cGPpropModel_h

// Include global headers
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <ctime>
#include <random>
#include <time.h>
#include <thread>
#include <chrono> 
#include <mutex>

// Include local headers
#include "../Prediction/cMeasAnalysisCalc.h"
#include "../Prediction/cClutter.h"
#include "cGPFunctions.h"
#include "../DataBase/Config.h"

// local defines
#define NUM_INIT_CANDIDATES 400
//#define NUM_INIT_CANDIDATES 500 //recommended in GP field guide
#define MAX_NUM_IN_CACHE 10
#define NUM_GENERATIONS 15 // GP field guide suggest between 10 and 50
#define NUM_POINT_PER_EVAL 1000

#define MAX_TREE_DEPTH 10 
#define PROP_MUTATE 0.5
#define CROSSOVERSCALE 4 // impacts on how often cross over takes place individuals taking part in a cross over

//Affects how elitist the algorithm is ... the smaller the more elitist
#define GAUSSDIST 1

#define CONSTMUTATE 0.15

#define MAXOPTLOOPS 500
#define MAXOPTCALC 2500
#define MAXMINAGE 30

#define CLUTTER 0

#define FITNESS 0.5*(100*(1-mCandidate[i].sCorrC)-17) + (mCandidate[i].sStdDev-7)

//impacts on how much of the population we loose per generation
#define UNFIT_LIMIT 200

using namespace std;

using namespace Qrap;

typedef vector<GOftn *> vConstants;

struct SCandidate
{ 
//	unsigned	sNumClutter=0;
//	unsigned * 	sClutterType=nullptr;
//	double * 	sClutterHeight=nullptr;
	unsigned	sNumber;
	GOftn * 	sTree=nullptr;
	double		sCorrC;
	double		sStdDev;
	double		sMean;
	double		sMSE;
	double		sFitness;
	unsigned	sRank;
	unsigned	sDepth;
	unsigned	sForm;
	bool		sPareto;
	vConstants	sConstants;
	bool		sOptimised;

 	SCandidate() 
	{
		renew();
	}

	void renew()
	{
/*		sNumClutter=2;
		sClutterType = new unsigned[sNumClutter];
		sClutterHeight = new double[sNumClutter];
*/		sTree = new ConstNode(0.0);
	}

	void deleteTree(GOftn* inTree)	
	{
	 //free any children first
		for (unsigned i=0; i<inTree->mNumChildren; i++) 
		{ 
			if (inTree->mChild[i]!=nullptr) deleteTree(inTree->mChild[i]);
		}
//		if ((inTree!=nullptr)) delete inTree;
		inTree = nullptr;
	}

 	~SCandidate() 
	{
/*		if (sClutterType!=nullptr) delete [] sClutterType;
		sClutterType = nullptr;
		if (sClutterHeight!=nullptr) delete [] sClutterHeight;	
		sClutterHeight = nullptr;
		sConstants.clear();
		if (sTree!=nullptr) deleteTree(sTree);
*/	}

	SCandidate & operator=(SCandidate Right)
	{
		unsigned i;
/*		sNumClutter	= Right.sNumClutter;
		delete [] sClutterType;
		delete [] sClutterHeight;
		sClutterType = new unsigned[sNumClutter];
		sClutterHeight = new double[sNumClutter];
		for (i=0; i<sNumClutter; i++)
		{
			sClutterType[i] = Right.sClutterType[i];
			sClutterHeight[i] = Right.sClutterHeight[i];
		}
*/		sConstants.clear();
		if (sTree!=nullptr) deleteTree(sTree);
		sTree = Right.sTree->clone();
		sTree->getConstants(sConstants);
		sCorrC		= Right.sCorrC;
		sStdDev		= Right.sStdDev;
		sMean		= Right.sMean;
		sMSE		= Right.sMSE;
		sFitness	= Right.sFitness;
		sRank		= Right.sRank;
		sForm		= Right.sForm;
		sDepth		= Right.sDepth;
		sPareto		= Right.sPareto;
		sOptimised	= Right.sOptimised;
		return *this;
	}
	
};

typedef vector<SCandidate> vCandidates;

namespace Qrap
{
	class cMeasAnalysisCalc; 

	class cGPpropModel 
	{
		public:

			cGPpropModel();

			~cGPpropModel();
		
			int mainTuning();

		private:

			void ComputeCandidates(unsigned Begin, unsigned Skip);

			void ComputeCandidate(unsigned CandidateIndex);

			int CostFunction(unsigned CandidateIndex, double &Mean, double &MeanSquareError,
				double &StDev, double &CorrC, 
				bool CalcNewObstruction=true, unsigned Clutterfilter=CLUTTER);

			int CostFunctionTreeOnly(unsigned CIndex, double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, unsigned Clutterfilter=CLUTTER);

			int AutoFix(unsigned CIndex, double &Mean, double &MeanSquareError,
					double &StDev, double &CorrC, unsigned Clutterfilter=CLUTTER);

			void optimiseConstants(unsigned Index);

			void optimiseConstantsSTDev(unsigned Index);

			void optimiseConstantsSTDevMO(unsigned Index);

			void optimiseConstantsCorrC(unsigned Index);

			void optimiseConstantsCorrCMO(unsigned Index);

			void printTree(GOftn* inTree, int depth=0);

			bool mutateTreeManyNode(GOftn* &inTree, int depth=0, 
							bool grow = false, 
							double PropMutate=0.3);

			bool mutateTreeSingleBranch(GOftn* &inTree, int depth=0, 
							bool grow = false, 
							double PropMutate=0.3);

			bool crossOverTree(GOftn* treeToAlter, GOftn* donatingTree);
	
			void deleteTree(GOftn* inTree);

			void mutateCandidate(unsigned CandidateIndex, bool grow=false);

			void mutateThread(unsigned Begin, unsigned Skip);

			void crossOverCandidate(unsigned CandidateIndex, unsigned donatorIndex);

			void replaceTree(unsigned CandidateIndex);
	
			void deleteCandidate(unsigned CandidateIndex);
	
			static bool SortCriteriaOnCorrC(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnStdDev(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnStdDevInverse(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnRank(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnFitness(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnFitnessInverse(SCandidate c1, SCandidate c2);
		
			GOftn* createRandomNode(int depth, bool grow=false);

			GOftn* createRandomTree(int depth=0, bool grow=false);

		private:

			double 		mMutationThreshold;
			unsigned	mNumToDie;
			unsigned	mNumCandidates;
			unsigned	mNumStars;
			double		mMinFitness;
			vCandidates 	mCandidate;
			vCandidates 	mStars;
			cMeasAnalysisCalc mMeas;

	};
}

#endif
