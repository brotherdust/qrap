/*
 *  TreeGeneOps.cpp
 *  GP
 *
 *  Created by Peter Harrington on 7/15/11.
 *  Edited by Magdaleen Ballot 2018-02-13
 *
 */
#include "TreeGeneOps.h"

using namespace std;

const int MAX_TREE_DEPTH = 4; 
const double MUTATION_THRESH = 0.1;

//*************************************************************************
void crossOver(GOftn* treeToAlter, GOftn* donatingTree)
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
void mutateTree(GOftn** inTree, int depth)
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
void printTree(GOftn* inTree, int depth)
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
GOftn* createRandomTree(int depth)
{
	GOftn* retFtn = createRandomNode(depth);
	
	for (int i=0; i < retFtn->mNumChildren; i++) 
	{
		retFtn->mChild[i] = createRandomTree(depth+1);
	}
	return retFtn;
}

//******************************************************************************
GOftn* createRandomNode(int depth)
{
	int randn = 0;
	int numFtnChoices = 2;
	GOftn* retFtn;

	//if the depth is greater than 3 only allow const or inputs
	if (depth < MAX_TREE_DEPTH) numFtnChoices = 9;
	//generate random int
	randn = rand() % numFtnChoices;		
	
	switch (randn) 
	{	//move random 
		case 0:
			retFtn = new ConstNode();
			break;
		case 1:
			retFtn = new InputNode(NUM_IPUTS);
			break;
		case 2:
			retFtn = new Add();
			break;
		case 3:
			retFtn = new Subtract();
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
			retFtn = new Log10();
			break;
		case 8:
			retFtn = new Power();
			break;
		default:
			cerr<<"invalid random number, bad news\n\n\n";
			break;
	}
	return retFtn;
}

//***********************************************************************
void deleteTree(GOftn* inTree)
{
	 //free any children first
	for (int i=0; i<inTree->mNumChildren; i++) 
	{ 
		deleteTree(inTree->mChild[i]);
	}
	free(inTree);
}

//***************************************************************************
bool treeSortPredIncre(GOftn* t1, GOftn* t2)
{
	return t1->mPerfScore > t2->mPerfScore;
}
