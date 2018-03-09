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
#include <time.h>

// Include local headers
#include "../Prediction/cMeasAnalysisCalc.h"
#include "../Prediction/cClutter.h"
#include "cGPFunctions.h"

// local defines
#define NUM_INIT_CANDIDATES 20
//#define NUM_INIT_CANDIDATES 500 //recommended in GP field guide
#define NUM_GENERATIONS 10 // GP field guide suggest between 10 and 50
#define NUM_POINT_PER_EVAL 1000
//how much of the population we loose per generation
#define DEATH_RATE 33 // As percentage.
#define UNFIT_LIMIT 1000

#define MAX_TREE_DEPTH 10 
#define PROP_MUTATE 0.5
#define PROP_CROSSOVER 0.5

using namespace std;
using namespace Qrap;


typedef vector<GOftn *> vConstants;

struct SCandidate
{ 
	unsigned	sNumClutter;
	unsigned * 	sClutterType;
	double * 	sClutterHeight;
	GOftn * 	sTree;
	double		sCorrC;
	double		sStdDev;
	double		sMean;
	double		sFitness;
	unsigned	sRank;
	unsigned	sDepth;
	bool		sPareto;
	vConstants	sConstants;
	bool		sOptimised;

	SCandidate operator=(SCandidate Right)
	{
		unsigned i;
		sNumClutter	= Right.sNumClutter;
		sClutterType = new unsigned[sNumClutter];
		sClutterHeight = new double[sNumClutter];
		for (i=0; i<sNumClutter; i++)
		{
			sClutterType[i] = Right.sClutterType[i];
			sClutterHeight[i] = Right.sClutterHeight[i];
		}
		sTree		= Right.sTree->clone();
		sTree->getConstants(sConstants);
		sCorrC		= Right.sCorrC;
		sStdDev		= Right.sStdDev;
		sMean		= Right.sMean;
		sFitness	= Right.sFitness;
		sRank		= Right.sRank;
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

			int CostFunction(unsigned CandidateIndex, double &Mean, double &MeanSquareError,
				double &StDev, double &CorrC, 
				bool CalcNewObstruction=true, unsigned Clutterfilter=0);


			void printTree(GOftn* inTree, int depth=0);

			void mutateTree(GOftn* &inTree, int depth=0, 
							bool grow = false, 
							double PropMutate=0.3);

			void crossOverTree(GOftn* treeToAlter, GOftn* donatingTree);
	
			void deleteTree(GOftn* inTree);

			void mutateCandidate(unsigned CandidateIndex, bool grow=false);

			void crossOverCandidate(unsigned CandidateIndex, unsigned donatorIndex);

			void replaceTree(unsigned CandidateIndex);
	
			void deleteCandidate(unsigned CandidateIndex);
	
			static bool SortCriteriaOnCorrC(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnStdDev(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnRank(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnFitness(SCandidate c1, SCandidate c2);
		
			GOftn* createRandomNode(int depth, bool grow=false);

			GOftn* createRandomTree(int depth=0, bool grow=false);

			int getRandSurvivor(unsigned popSize);

			bool optimiseConstants(unsigned Index);

		private:

			double 		mMutationThreshold;
			unsigned	mNumToDie;
			unsigned	mNumCandidates;
			double		mMinFitness;
			vCandidates 	mCandidate;
			vCandidates 	mStars;
			cMeasAnalysisCalc mMeas;

	};
}

#endif
