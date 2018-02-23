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
#define NUM_INIT_CANDIDATES 30
#define NUM_GENERATIONS 200
#define NUM_POINT_PER_EVAL 1000
//how much of the population we loose per generation
#define DEATH_RATE 33 // As percentage.

#define MAX_TREE_DEPTH 20 

struct SCandidate
{ 
	unsigned	sNumClutter;
	unsigned * 	sClutterType;
	double * 	sClutterHeight;
	GOftn * 	sTree;
	double		sCorrC;
	double		sStdDev;
	double		sMean;
	unsigned	sRank;
	unsigned	sDepth;
	bool		sPareto;

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
		sCorrC		= Right.sCorrC;
		sStdDev		= Right.sStdDev;
		sRank		= Right.sRank;
		sDepth		= Right.sDepth;
		sPareto		= Right.sPareto;
		return *this;
	}
};

typedef vector<SCandidate> vCandidates;


using namespace std;
using namespace Qrap;

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

			int CostFunction(unsigned CanIndex, double &Mean, double &MeanSquareError,
				double &StDev, double &CorrC, unsigned Clutterfilter=0);


			void printTree(GOftn* inTree, int depth=0);

			void mutateTree(GOftn* &inTree, int depth=0, double PropMutate=0.3);

			void crossOverTree(GOftn* treeToAlter, GOftn* donatingTree);
	
			void deleteTree(GOftn* inTree);

			void mutateCandidate(unsigned Index);

			void crossOverCandidate(unsigned Index, unsigned donatorIndex);

			void replaceTree(unsigned Index);
	
			void deleteCandidate(unsigned Index);
	
			static bool SortCriteriaOnCorrC(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnStdDev(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnRank(SCandidate c1, SCandidate c2);
		
			GOftn* createRandomNode(int depth);
	
			GOftn* createRandomTree(int depth=0);

			int getRandSurvivor(unsigned popSize);

		private:

			double 		mMutationThreshold;
			unsigned	mNumToDie;
			unsigned	mNumCandidates;
			cClutter	mClutter;
			vCandidates 	mCandidate;
			cMeasAnalysisCalc mMeas;

	};
}

#endif
