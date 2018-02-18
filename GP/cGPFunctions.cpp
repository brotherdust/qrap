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

//**************************************************************************
//**************************************************************************
//
//			base class GOftn
//
//*************************************************************************
GOftn::GOftn()
{
	mNumChildren=0;
	mChild=nullptr;
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
	mConstVal = rand()/(double)RAND_MAX;
	char* str;
	str = new char[20];
	sprintf(str, "Const: %f", mConstVal);
	mLabel = str;
	delete [] str;
}

//**************************************************************************
ConstNode::ConstNode(double preSetVal)
{
	mNumChildren = 0;
	mChild = nullptr;
	mConstVal = preSetVal;
	char* str;
	str = new char[20];
	sprintf(str, "Const: %f", mConstVal);
	mLabel = str;
	delete [] str;
}

//***************************************************************************
tMeasPoint ConstNode::eval(tMeasPoint InPoint)
{
	tMeasPoint outPoint = InPoint;
	outPoint.sReturn = mConstVal;
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
}

//**************************************************************************
tMeasPoint DistanceNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDistance;
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
}

//**************************************************************************
tMeasPoint FrequencyNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sFrequency;
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
}

//**************************************************************************
tMeasPoint TxHeightNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sTxHeight;
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
	mLabel = " L_c ";
}

//**************************************************************************
tMeasPoint ObstructionNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDiffLoss;
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
}

//**************************************************************************
tMeasPoint ClutterHeightNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = mClutter.mClutterTypes[inPoint.sClutter].sHeight;
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
Add::Add()
{
	mNumChildren = 2;
	mChild = new GOftn*[mNumChildren];
	mLabel = " + ";
}

//************************************************************************
tMeasPoint Add::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->eval(inPoint);
		c2 = mChild[1]->eval(inPoint);
		outPoint.sReturn = c1.sReturn + c2.sReturn;
	}
	else 
	{
		cerr << "left and right not defined in power"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//***********************************************************************
Add* Add::clone()
{
	Add* retNode = new Add();
	for (int i=0; i<mNumChildren; i++) 
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
		cerr << "left and right not defined in power"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//***********************************************************************
Subtract* Subtract::clone()
{
	Subtract* retNode = new Subtract();
	for (int i=0; i<mNumChildren; i++) 
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
Multiply::Multiply()
{
	mNumChildren = 2;
	mChild = new GOftn*[mNumChildren];
	mLabel = " * ";
}

//**********************************************************************
tMeasPoint Multiply::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->eval(inPoint);
		c2 = mChild[1]->eval(inPoint);
		outPoint.sReturn = c1.sReturn*c2.sReturn;
	}
	else 
	{
		cerr << "left and right not defined in Multiply."<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//**********************************************************************
Multiply* Multiply::clone()
{
	Multiply* retNode = new Multiply();
	for (int i=0; i<mNumChildren; i++) 
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
			cerr << "almost divide by zero "<<endl;
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
	for (int i=0; i<mNumChildren; i++) 
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
			cerr << "input to log10 not positive"<<endl;
			outPoint.sReturn = -1000.0;
		}
	}
	else 
	{
		cerr << "input not defined in square"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//**********************************************************************
Log10Node* Log10Node::clone()
{
	Log10Node* retNode = new Log10Node;
	for (int i=0; i<mNumChildren; i++) 
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
	for (int i=0; i<mNumChildren; i++) 
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
		cerr << "left and right not defined in power"<<endl;
		outPoint.sReturn = -1000.0;
	}
	return outPoint;
}

//**********************************************************************
Power* Power::clone()
{
	Power* retNode = new Power();
	for (int i=0; i<mNumChildren; i++) 
	{
		retNode->mChild[i] = mChild[i]->clone();
	}
	return retNode;
}

