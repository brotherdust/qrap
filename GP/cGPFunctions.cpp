/*
 *  cGPFunctions.cpp
 *  GP
 *
 *  Created by Peter Harrington on 7/1/11.
 *  Edited by Magdaleen Ballot 2018-02-13
 *
 */
#include "cGPFunctions.h"
using namespace std;
using namespace Qrap;
#ifndef FGLOBALS
#define FGLOBALS 1
std::default_random_engine 		fRandomGen;
std::normal_distribution<double> 	fGauss;
#endif



//**************************************************************************
//**************************************************************************
//
//			base class GOftn
//
//*************************************************************************
GOftn::GOftn()
{
	mNumChildren=0;
	mIsConstant = false;
	mChild=nullptr;
}

//************************************************************************
void GOftn::mutate(double Scale)
{
}

//************************************************************************
GOftn::~GOftn()
{
	if ((mChild!=nullptr)&&(mNumChildren>0))
		delete [] mChild;
}

//*************************************************************************
string GOftn::getLabel()
{
	return mLabel;
}

//************************************************************************
unsigned GOftn::getTreeDepth(unsigned CurrentDepth)
{
	unsigned i, MaxDepth=0, childDepth;
	for (i=0; i < mNumChildren; i++)
	{
		childDepth = mChild[i]->getTreeDepth(CurrentDepth);
		if ( childDepth > MaxDepth)
			MaxDepth = childDepth;
	}
	CurrentDepth = MaxDepth;
	if (0<mNumChildren) CurrentDepth++;
	return CurrentDepth;
}

//**************************************************************************
//**************************************************************************
//
//		ConstNode
//
//***************************************************************************
ConstNode::ConstNode()
{
	mNumChildren = 0;
	mChild = nullptr;
//	mConstVal = fGauss(fRandomGen)*30;
	mConstVal = 30*(rand()%100)/100;
	double sign = rand()%2 -1;
	mConstVal*=sign;
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
	mIsConstant = true;
}

//**************************************************************************
ConstNode::ConstNode(double preSetVal)
{
	mNumChildren = 0;
	mChild = nullptr;
	mConstVal = preSetVal;
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
	mIsConstant = true;
}

//**************************************************************************
void ConstNode::mutate(double Scale)
{
	mConstVal *=( 1.0 + fGauss(fRandomGen)*Scale);
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
}


//***************************************************************************
tMeasPoint ConstNode::eval(tMeasPoint InPoint)
{
	tMeasPoint outPoint = InPoint;
	outPoint.sReturn = mConstVal;
//	cout << mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//**************************************************************************
ConstNode* ConstNode::clone()
{
	ConstNode* retTree = new ConstNode(mConstVal);
	return retTree; 
}


//**************************************************************************
//**************************************************************************
//
//		DistanceNode
//
//**************************************************************************
DistanceNode::DistanceNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " d ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint DistanceNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDistance/1000;
//	cout <<"	"<< mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//*************************************************************************
DistanceNode* DistanceNode::clone()
{
	DistanceNode* retTree = new DistanceNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		FrequencyNode
//
//**************************************************************************
FrequencyNode::FrequencyNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " f ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint FrequencyNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sFrequency;
//	cout <<"	"<< mLabel << outPoint.sReturn <<endl;
	return outPoint;
}

//*************************************************************************
FrequencyNode* FrequencyNode::clone()
{
	FrequencyNode* retTree = new FrequencyNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		TxHeightNode
//
//**************************************************************************
TxHeightNode::TxHeightNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " h_tx ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint TxHeightNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sTxHeight;
//	cout << mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//*************************************************************************
TxHeightNode* TxHeightNode::clone()
{
	TxHeightNode* retTree = new TxHeightNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		RxHeightNode
//
//**************************************************************************
RxHeightNode::RxHeightNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " h_rx ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint RxHeightNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sRxHeight;
	return outPoint;
}

//*************************************************************************
RxHeightNode* RxHeightNode::clone()
{
	RxHeightNode* retTree = new RxHeightNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		ObstructionNode
//
//**************************************************************************
ObstructionNode::ObstructionNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " L_o ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint ObstructionNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDiffLoss;
//	cout <<"	"<< mLabel << outPoint.sReturn << endl;
	return outPoint;
}

//*************************************************************************
ObstructionNode* ObstructionNode::clone()
{
	ObstructionNode* retTree = new ObstructionNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		ClutterTypeNode
//
//**************************************************************************
ClutterTypeNode::ClutterTypeNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " Ic ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint ClutterTypeNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutter;
	return outPoint;
}

//*************************************************************************
ClutterTypeNode* ClutterTypeNode::clone()
{
	ClutterTypeNode* retTree = new ClutterTypeNode();
	return retTree; 
}

//**************************************************************************
//**************************************************************************
//
//		ClutterHeightNode
//
//**************************************************************************
ClutterHeightNode::ClutterHeightNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " h_c ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint ClutterHeightNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutterHeight;
	return outPoint;
}

//*************************************************************************
ClutterHeightNode* ClutterHeightNode::clone()
{
	ClutterHeightNode* retTree = new ClutterHeightNode();
	return retTree; 
}


//**************************************************************************
//**************************************************************************
//
//		ClutterDepthNode
//
//**************************************************************************
ClutterDepthNode::ClutterDepthNode()
{
	mNumChildren = 0;
	mChild = nullptr;
	mLabel = " h_c ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint ClutterDepthNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutterDistance;
	return outPoint;
}

//*************************************************************************
ClutterDepthNode* ClutterDepthNode::clone()
{
	ClutterDepthNode* retTree = new ClutterDepthNode();
	return retTree; 
}

//************************************************************************
//************************************************************************
//
//		Add
//
//************************************************************************
Add::Add(unsigned NumChildren)
{
	mNumChildren = NumChildren;
	mChild = new GOftn*[mNumChildren];
	mLabel = " + ";
	mIsConstant = false;
}

//************************************************************************
tMeasPoint Add::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint CValue;
	unsigned i, NumChildren = 0;
	double Sum = 0;
	for(i=0; i<mNumChildren; i++)
	{
		if (mChild[i])
		{
			NumChildren++;
			CValue = mChild[i]->eval(inPoint);
			Sum+=CValue.sReturn;
		}
		else 
		{
			cerr << "not all inputs define in Add"<<endl;
			mNumChildren=NumChildren;
			outPoint.sReturn = -1000.0;
			return outPoint;
		}
	}
	outPoint.sReturn = Sum;
//	cout << "	" << mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//***********************************************************************
Add* Add::clone()
{
	Add* retNode = new Add(mNumChildren);
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}


//***********************************************************************
//***********************************************************************
//		Subtract
//
//***********************************************************************
Subtract::Subtract()
{
	mNumChildren = 2;
	mChild = new GOftn*[mNumChildren];
	mLabel = " - ";
	mIsConstant = false;
}

//***********************************************************************
tMeasPoint Subtract::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->eval(inPoint);
		c2 = mChild[1]->eval(inPoint);
		outPoint.sReturn = c1.sReturn - c2.sReturn;
	}
	else 
	{
		cerr << "left and right not defined in Subtract"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//***********************************************************************
Subtract* Subtract::clone()
{
	Subtract* retNode = new Subtract();
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}


//***********************************************************************
//***********************************************************************
//
//		Multiply
//
//**********************************************************************
Multiply::Multiply(unsigned NumChildren)
{
	mNumChildren = NumChildren;
	mChild = new GOftn*[mNumChildren];
	mLabel = " * ";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Multiply::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint CValue;
	unsigned i, NumChildren = 0;
	double Product = 1;
	for(i=0; i<mNumChildren; i++)
	{
		if (mChild[i])
		{
			NumChildren++;
			CValue = mChild[i]->eval(inPoint);
			Product*=CValue.sReturn;
		}
		else 
		{
			cerr << "not all inputs define in Multiply"<<endl;
			mNumChildren=NumChildren;
			outPoint.sReturn = -1000.0;
			return outPoint;
		}
	}
	outPoint.sReturn = Product;
//	cout << "	" << mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//**********************************************************************
Multiply* Multiply::clone()
{
	Multiply* retNode = new Multiply(mNumChildren);
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}


//**********************************************************************
//**********************************************************************
//
//		Divide
//
//**********************************************************************
Divide::Divide()
{
	mNumChildren = 2;
	mChild = new GOftn*[mNumChildren];
	mLabel = " / ";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Divide::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->eval(inPoint);
		c2 = mChild[1]->eval(inPoint);
		if (fabs(c2.sReturn)>0)
		{
			outPoint.sReturn = c1.sReturn/c2.sReturn;
		}
		else
		{
//			cerr << "almost divide by zero "<<endl;
			outPoint.sReturn = -1000.0;
		}
	}
	else 
	{
		cout << "left and right not defined in divide "<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//**********************************************************************
Divide* Divide::clone()
{
	Divide* retNode = new Divide;
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}


//**********************************************************************
//**********************************************************************
//
//		log base 10
//
//**********************************************************************
Log10Node::Log10Node()
{
	mNumChildren = 1;
	mChild = new GOftn*[mNumChildren];
	mLabel = "log10()";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Log10Node::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	if (mChild[0])
	{
		c1 = mChild[0]->eval(inPoint);
		if (c1.sReturn>0)
			outPoint.sReturn = log10(c1.sReturn);
		else
		{
//			cerr << "input to log10 not positive"<<endl;
			outPoint.sReturn = -1000.0;
		}
	}
	else 
	{
		cerr << "input not defined in log10"<<endl;
		outPoint.sReturn = -1000.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn << endl;
	return outPoint;
}

//**********************************************************************
Log10Node* Log10Node::clone()
{
	Log10Node* retNode = new Log10Node;
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}

//**********************************************************************
//**********************************************************************
//
//		Square x^2=x*x
//
//**********************************************************************
Square::Square()
{
	mNumChildren = 1;
	mChild = new GOftn*[mNumChildren];
	mLabel = "Square";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Square::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	if (mChild[0])
	{
		c1 = mChild[0]->eval(inPoint);
		outPoint.sReturn = c1.sReturn*c1.sReturn;
	}
	else 
	{
		cerr << "input not defined in square"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//**********************************************************************
Square* Square::clone()
{
	Square* retNode = new Square;
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}

//***********************************************************************
//***********************************************************************
//
//		Power
//
//**********************************************************************
Power::Power()
{
	mNumChildren = 2;
	mChild = new GOftn*[mNumChildren];
	mLabel = " ^ ";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Power::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->eval(inPoint);
		c2 = mChild[1]->eval(inPoint);
		outPoint.sReturn = pow(c1.sReturn, c2.sReturn);
	}
	else 
	{
		cerr << "left and right not defined in Power"<<endl;
		outPoint.sReturn = -1000.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn << endl;	
	return outPoint;
}

//**********************************************************************
Power* Power::clone()
{
	Power* retNode = new Power();
	for (unsigned i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}

