/*
 *  cGPFunctions.h
 *  GP
 *
 *  Edited by Magdaleen Ballot 2018-02-13
 *
 */


#ifndef BASIC_FTNS_H
#define BASIC_FTNS_H 1

#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <random>
#include <time.h>
#include "../Prediction/cMeasAnalysisCalc.h"



using namespace std;
using namespace Qrap;
	
namespace Qrap
{

//**************************************************************************************
//Master abstract class: GOftn: Genetic Optimize Function
class GOftn
{	

typedef vector<GOftn *> vConstants;
public:
	double mConstVal;
	string mLabel;
	unsigned mNumChildren;
	bool mIsConstant;
	GOftn* getNewNode();
	GOftn** mChild;
	GOftn();
	~GOftn();
	virtual tMeasPoint eval(tMeasPoint inPoint) = 0;  //setting the 0 makes it a PURE
	tMeasPoint evalfix(tMeasPoint inPoint);
	virtual GOftn* clone() = 0; //make a deep copy of the current tree
	void mutate(double Scale); //used in mutating constants
	void setValue(double newValue); //used in tuning constants
	double getValue(); //used in mutating constants
	string getLabel();
	unsigned getTreeDepth();
	unsigned getConstants(vConstants &Constants);
};


//*****************************************************************************************
//class for using distance between tx and rx as input
class DistanceNode : public GOftn 
{
public:
	DistanceNode();
	tMeasPoint eval(tMeasPoint inPoint);
	DistanceNode* clone();
};

//*****************************************************************************************
//class for using operating frequency as input
class FrequencyNode : public GOftn 
{
public:
	FrequencyNode();
	tMeasPoint eval(tMeasPoint inPoint);
	FrequencyNode* clone();
};

//*****************************************************************************************
//class for using transmitter antenna height as input
class TxHeightNode : public GOftn 
{
public:
	TxHeightNode();
	tMeasPoint eval(tMeasPoint inPoint);
	TxHeightNode* clone();
};

//*****************************************************************************************
//class for using receiver antenna height as input
class RxHeightNode : public GOftn 
{
public:
	RxHeightNode();
	tMeasPoint eval(tMeasPoint inPoint);
	RxHeightNode* clone();
};

//*****************************************************************************************
//class for using obstructionloss as input
class ObstructionNode : public GOftn 
{
public:
	ObstructionNode();
	tMeasPoint eval(tMeasPoint inPoint);
	ObstructionNode* clone();
};

//*****************************************************************************************
//class for using Clutter type index as input
class  ClutterTypeNode : public GOftn 
{
public:
	ClutterTypeNode();
	tMeasPoint eval(tMeasPoint inPoint);
	ClutterTypeNode* clone();
};

//*****************************************************************************************
//class for using clutter height as input
class  ClutterHeightNode : public GOftn 
{
public:
	ClutterHeightNode();
	tMeasPoint eval(tMeasPoint inPoint);
	ClutterHeightNode* clone();
};

//*****************************************************************************************
//class for using clutter depth as input
class  ClutterDepthNode : public GOftn 
{
public:
	ClutterDepthNode();
	tMeasPoint eval(tMeasPoint inPoint);
	ClutterDepthNode* clone();
};

//****************************************************************************
//addition
class Add : public GOftn 
{
public:
	Add(unsigned NumChildren=2);
	tMeasPoint eval(tMeasPoint inPoint);
	Add* clone();
};

//*******************************************************************************
//subtraction
class Subtract : public GOftn 
{
public:
	Subtract();
	tMeasPoint eval(tMeasPoint inPoint);
	Subtract* clone();
};

//******************************************************************************
//multiplication
class Multiply : public GOftn 
{
public:
	Multiply(unsigned NumChildren=2);
	tMeasPoint eval(tMeasPoint inPoint);
	Multiply* clone();
};

//*******************************************************************************
//division
class Divide : public GOftn 
{
public:
	Divide();
	tMeasPoint eval(tMeasPoint inPoint);
	tMeasPoint evalfix(tMeasPoint inPoint);
	Divide* clone();
};


//*******************************************************************************
//Log base 10
class Log10Node : public GOftn 
{
public:
	Log10Node();
	tMeasPoint eval(tMeasPoint inPoint);
	tMeasPoint evalfix(tMeasPoint inPoint);
	Log10Node* clone();
};

//*******************************************************************************
//Log base 10
class Exponent : public GOftn 
{
public:
	Exponent();
	tMeasPoint eval(tMeasPoint inPoint);
	Exponent* clone();
};

//*******************************************************************************
// Square x^2
class Square : public GOftn 
{
public:
	Square();
	tMeasPoint eval(tMeasPoint inPoint);
	Square* clone();
};

//*******************************************************************************
//power
class Power : public GOftn 
{
public:
	Power();
	tMeasPoint eval(tMeasPoint inPoint);
	tMeasPoint evalfix(tMeasPoint inPoint);
	Power* clone();
};



//*************************************************************************************
//class for storing constant values
class ConstNode : public GOftn 
{
public:
	ConstNode();
	ConstNode(double preSetVal);
	void mutate(double Scale);
	void setValue(double newValue);
	double getValue();
	tMeasPoint eval(tMeasPoint inPoint);
	ConstNode* clone();
};


}
#endif
