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
#include "../DataBase/Config.h"
#include "cGPFunctions.h"

//general debugging print macro
#ifndef   DEBUG
	#define   PRINTD(arg, ...)
#else
	#define   PRINTD(arg, ...)     printf(arg, ##__VA_ARGS__)
#endif

using namespace std;

// local defines
const int NUM_INIT_CANDIDATES = 300;
const int NUM_GENERATIONS = 200;
const int NUM_POINT_PER_EVAL = 2000;
//how much of the population we loose per generation
const int DEATH_RATE = 30; 

const int MAX_TREE_DEPTH = 4; 
const double MUTATION_THRESH = 0.1;

namespace Qrap
{

	struct SCandidate
	{ 
		unsigned	sNumClutter;
		unsigned * 	sClutterType;
		double * 	sClutterHeight;
		GOftn * 	sTree;
		double		sCorrC;
		double		sMSE;
		unsigned	sRank;
	};

	typedef vector<SCandidate> vCandidates;

	class cMeasAnalysisCalc; 

	class cGPpropModel 
	{
		public:

			cGPpropModel();

			~cGPpropModel();
		
			int mainTuning();

			int CostFunction(unsigned CanIndex, double &Mean, double &MeanSquareError,
				double &StDev, double &CorrC, unsigned Clutterfilter=0);


			void printTree(GOftn* inTree, int depth=0);

			void mutateTree(GOftn** inTree, int depth=0);

			void crossOver(GOftn* treeToAlter, GOftn* donatingTree);
	
			void deleteTree(GOftn* inTree);
	
			static bool SortCriteriaOnCorrC(SCandidate c1, SCandidate c2);
			static bool SortCriteriaOnMSE(SCandidate c1, SCandidate c2);
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
