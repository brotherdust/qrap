/********************************************************
 *
 * Sample GP program
 *
 *  Created by Peter Harrington on 6/28/11.
 *  Edited by Magdaleen Ballot 2018-02-13
 *
*********************************************************/
			
#include "General.h"
#include "TreeGeneOps.h"

using namespace std;

const int NUM_INIT_TREES = 300;
const int NUM_GENERATIONS = 200;
//how much of the population we loose per generation
const int DEATH_RATE = 30; 

int getRandSurvior(unsigned popSize);

int main (int argc, char * const argv[]) 
{
	unsigned numTrees = NUM_INIT_TREES; 
	unsigned numToDie = (unsigned)numTrees*DEATH_RATE/100;
	unsigned i=0, j=0, k=0;
	srand(time(NULL));  //seed the RNG
	double sumAbsErr = 0;
	double err = 1000;
	//create array to store N trees
	vector<GOftn*> tree;
	GOftn* newTree = nullptr;

	const int NUM_INPUTS = 9;
	double inputs[NUM_INPUTS] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	double outputs[NUM_INPUTS] = {6, 15, 28, 45, 66, 91, 120, 153, 190}; //2x^2 + 3x + 1
	
	// create the inital tree population 
	for (i=0; i<numTrees; i++)
	{
		newTree = createRandomTree();
		tree.push_back(newTree);
	}
	
	for (k=0; k<NUM_GENERATIONS; k++) 
	{
		numTrees = tree.size();
		// test the performance of each one
		// this should be problem-dependent, and implemented in another file
		for (i=0; i<numTrees; i++)
		{
			sumAbsErr = 0.0;
			for (j=0; j<NUM_INPUTS; j++) 
			{
				err = outputs[j] - (tree[i]->eval(inputs[j]));
				sumAbsErr += abs(err);
			}
			PRINTD("evaluating tree %d: %f \n", i, sumAbsErr);
			if (sumAbsErr == 0)
			{
				printf("found a solution with 0 error\n");
				printTree(tree[i]);
				break;
			}
			if (isnan(sumAbsErr)) sumAbsErr = 1E+37; //max double val
			tree[i]->mPerfScore = sumAbsErr;
		}
		
		//sort by performance (sort in increasing order so we work on first N)
		sort(tree.begin(), tree.end(), treeSortPredIncre);
		
		numToDie = (unsigned)(numTrees*DEATH_RATE/100);
		for (i=0; i<numToDie; i++)
		{
			//toss out losers
			deleteTree(tree[i]);			
			//randomly select one of the survivors and clone
			tree[i] = tree[getRandSurvior(numTrees)]->clone();
			//do cross over with survivors 
			//crossOver(initTrees[i], initTrees[getRandSurvior(numTrees)]);
			//cross over with best
			crossOver(tree[i], tree[numTrees - 1]);  
			mutateTree(&tree[i]);	//mutate
		}
		printf("Generation %d the best score is: %f\n", k, tree[numTrees - 1]->mPerfScore);
	}
	printTree(tree[numTrees - 1]);
	for (i=0; i<numTrees; i++)
	{
		deleteTree(tree[i]);
	} 
	tree.clear();
    	return 0;
}

//******************************************************
int getRandSurvior(unsigned popSize)
{
	unsigned numToDie = (unsigned)(popSize*DEATH_RATE/100);
	int randn = rand() % (popSize - numToDie);
	return (numToDie + randn);
}
