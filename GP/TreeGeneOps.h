/*
 *  TreeGeneOps.h
 *  GeneticProg
 *
 *  Created by Peter Harrington on 7/15/11.
 *  Edited by Magdaleen Ballot 2018-02-13
 *
 *	This file includes functions for basic tree genetic operations.
 *	These operations are cross over, mutation, creating a tree, 
 *	deleting a tree and printing a tree.  
 *
 */

#ifndef TreeGeneOps_h
#define TreeGeneOps_h

#include "General.h"
#include "cGPFunctions.h"


void printTree(GOftn* inTree, int depth=0);

void mutateTree(GOftn** inTree, int depth=0);

void crossOver(GOftn* treeToAlter, GOftn* donatingTree);
	
void deleteTree(GOftn* inTree);
	
bool treeSortPredIncre(GOftn* t1, GOftn* t2);

GOftn* createRandomNode(int depth);

GOftn* createRandomTree(int depth=0);


#endif
