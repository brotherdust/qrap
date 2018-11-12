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
	mConstVal = 0.0;
	mNumChildren=0;
	mIsConstant = false;
	mChild=nullptr;
}

//************************************************************************
GOftn::~GOftn()
{
//	if ((mChild!=nullptr)&&(mNumChildren>0))
//		delete [] mChild;
}

//*************************************************************************
string GOftn::getLabel()
{
	return mLabel;
}

//************************************************************************
unsigned GOftn::getTreeDepth()
{
	unsigned i, MaxDepth=0, childDepth, CurrentDepth;
	for (i=0; i < mNumChildren; i++)
	{
		childDepth = mChild[i]->getTreeDepth();
		if ( childDepth > MaxDepth)
			MaxDepth = childDepth;
	}
	CurrentDepth = MaxDepth;
	if (0<mNumChildren) CurrentDepth++;
	return CurrentDepth;
}

//************************************************************************
unsigned GOftn::getConstants(vConstants &Constants)
{
	unsigned i, number;
	for (i=0; i < mNumChildren; i++)
	{
		number = mChild[i]->getConstants(Constants);
	}
	if (mIsConstant)
	{
		Constants.push_back(&*this);
	}
	number = Constants.size();
	return number;
}

//****************************************************************************
void GOftn::setValue(double newValue) 
{
	mConstVal = newValue;
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
	mIsConstant = true;
}

//*****************************************************************************
double GOftn::getValue() 
{
	if (mIsConstant)
		return mConstVal;
	else return 0;
}

//****************************************************************************
void GOftn::mutate(double Scale)
{
	mConstVal *=( 1.0 + fGauss(fRandomGen)*Scale);
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
	mIsConstant = true;
}

//******************************************************************************
// This function is uded for "fixing" ill conditioned  structures.
GOftn* GOftn::getNewNode()
{
	int randn = rand() % 14;
	double ConstP;
	GOftn* retFtn;
	
	switch (randn) 
	{	//move random 
		case 0:
			ConstP = (double)(rand() % 40) + 0.5;
			retFtn = new ConstNode(ConstP);
			break;
		case 1:
			retFtn = new ObstructionNode();
			break;
		case 2:
			retFtn = new DistanceNode();
			break;
		case 3:
			retFtn = new FrequencyNode();
			break;
		case 4:
			retFtn = new TxHeightNode();
			break;
		case 5:
			retFtn = new RxHeightNode();
			break;
//		case 6:
//			retFtn = new ClutterTypeNode();
//			break;
		case 6:
			retFtn = new ClutterHeightNode();
			break;
		case 7:
			retFtn = new ClutterDepthNode();
			break;
		case 8:
			retFtn = new Add();
			break;
		case 9:
			retFtn = new Multiply();
			break;
		case 10:
			retFtn = new Divide();
			break;
		case 11:
			retFtn = new Log10Node();
			break;
		case 12:
			retFtn = new Power();
			break;
		case 13:
			retFtn = new Exponent();
			break;

		default:
			cerr<<"invalid random number, bad news\n\n\n";
			break;
	}
	for (unsigned i=0; i<retFtn->mNumChildren; i++) 
	{
		retFtn->mChild[i] = getNewNode();
	}
	return retFtn;
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
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint DistanceNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDistance/1000;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
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
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint FrequencyNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sFrequency;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
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
//	cout << mLabel << outPoint.sReturn<<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint TxHeightNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sTxHeight;
//	cout << mLabel << outPoint.sReturn<<"  "<<endl;
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
//	cout << mLabel << outPoint.sReturn<<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint RxHeightNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sRxHeight;
//	cout << mLabel << outPoint.sReturn<<"  "<<endl;
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
//	cout << mLabel << outPoint.sReturn <<"  "<< endl;
	return outPoint;
}

//********************************************************************
tMeasPoint ObstructionNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sDiffLoss;
//	cout << mLabel << outPoint.sReturn <<"  "<< endl;
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
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint ClutterTypeNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutter;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
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
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint ClutterHeightNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutterHeight;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
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
	mLabel = " d_c ";
	mIsConstant = false;
}

//**************************************************************************
tMeasPoint ClutterDepthNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutterDistance;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint ClutterDepthNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = inPoint.sClutterDistance;
//	cout << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//*************************************************************************
ClutterDepthNode* ClutterDepthNode::clone()
{
	ClutterDepthNode* retTree = new ClutterDepthNode();
	return retTree; 
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
	mConstVal = fGauss(fRandomGen)*30;
//	mConstVal = 30*(rand()%100)/100;
//	double sign = 2*(rand()%2 -0.5);
//	mConstVal*=sign;
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
//	cout << mLabel << endl;
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
	mIsConstant = true;
}

//***************************************************************************
void ConstNode::setValue(double newValue)
{
	mConstVal = newValue;
	char* str;
	str = new char[20];
	sprintf(str, " C: %f", mConstVal);
	mLabel = str;
	delete [] str;
	mIsConstant = true;
}

//***************************************************************************
double ConstNode::getValue() 
{
	return mConstVal;
}

//***************************************************************************
tMeasPoint ConstNode::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = mConstVal;
//	cout << mLabel << "  " << outPoint.sReturn <<endl;
	return outPoint;
}

//***************************************************************************
tMeasPoint ConstNode::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	outPoint.sReturn = mConstVal;
//	cout << mLabel << "  " << outPoint.sReturn <<endl;
	return outPoint;
}

//**************************************************************************
ConstNode* ConstNode::clone()
{
	ConstNode* retTree = new ConstNode(mConstVal);
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
			outPoint.sReturn = -200.0;
			return outPoint;
		}
	}
	outPoint.sReturn = Sum;
//	cout << "	" << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint Add::evalfix(tMeasPoint inPoint)
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
			CValue = mChild[i]->evalfix(inPoint);
			Sum+=CValue.sReturn;
		}
		else 
		{
			cerr << "not all inputs define in Add"<<endl;
			mNumChildren=NumChildren;
			outPoint.sReturn = -200.0;
			return outPoint;
		}
	}
	outPoint.sReturn = Sum;
//	cout << "	" << mLabel << outPoint.sReturn <<"  "<<endl;
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
		if (outPoint.sReturn<0) outPoint.sReturn=1e-10;
	}
	else 
	{
		cerr << "left and right not defined in Subtract"<<endl;
		outPoint.sReturn = -200.0;
	}
//	cout << "	" << mLabel << outPoint.sReturn<<"  "<<endl;
	return outPoint;
}

//********************************************************************
tMeasPoint Subtract::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->evalfix(inPoint);
		c2 = mChild[1]->evalfix(inPoint);
		outPoint.sReturn = c1.sReturn - c2.sReturn;
		if (outPoint.sReturn<0) outPoint.sReturn=1e-10;
	}
	else 
	{
		cerr << "left and right not defined in Subtract"<<endl;
		outPoint.sReturn = -200.0;
	}
//	cout << "	" << mLabel << outPoint.sReturn<<"  "<<endl;
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
			cerr << "not all inputs define in Multiply"<<"  "<<endl;
			mNumChildren=NumChildren;
			outPoint.sReturn = -200.0;
			return outPoint;
		}
	}
	outPoint.sReturn = Product;
//	cout << "	" << mLabel << outPoint.sReturn<<endl;
	return outPoint;
}

//**********************************************************************
tMeasPoint Multiply::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint CValue;
	unsigned i, NumChildren = 0, count = 0;
	double Product = 1;
	
	for(i=0; i<mNumChildren; i++)
	{
		if (mChild[i])
		{
			CValue = mChild[i]->evalfix(inPoint);
			while((log10(fabs(CValue.sReturn)))>(log(DBL_MAX)/mNumChildren)&&count<10)
			{
				mChild[1] = getNewNode();
				CValue = mChild[1]->evalfix(inPoint);
				count++;
			}
			NumChildren++;
			Product*=CValue.sReturn;
		}
		else 
		{
			cerr << "not all inputs define in Multiply"<<"  "<<endl;
			mNumChildren=NumChildren;
			outPoint.sReturn = -200.0;
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
		c1 = mChild[0]->eval(outPoint);
		c2 = mChild[1]->eval(outPoint);
		if (fabs(c2.sReturn)>1e-100)
			outPoint.sReturn = c1.sReturn/c2.sReturn;
		else
		{
//			cout << "Divide input value too small 	";
//			cout << mChild[1]->mLabel << " " << c2.sReturn << endl;
			outPoint.sReturn = 1000000000000.0;			
		}
	}
	else 
	{
		cout << "left and right not defined in divide "<<endl;
		outPoint.sReturn = 100000000000000.0;
	}
//	cout << "	" << mLabel << outPoint.sReturn <<"  "<<endl;
	return outPoint;
}

//**********************************************************************
tMeasPoint Divide::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	unsigned count = 0;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->evalfix(outPoint);
		c2 = mChild[1]->evalfix(outPoint);
		while ((fabs(c2.sReturn)<1e-6)&&(count<10))
		{
			mChild[1] = getNewNode();
			c2 = mChild[1]->evalfix(inPoint);
			count++;
		}
		outPoint.sReturn = c1.sReturn/c2.sReturn;
	}
	else 
	{
		cout << "left and right not defined in divide "<<endl;
		outPoint.sReturn = 100000000000000.0;
	}
//	cout << "	" << mLabel << outPoint.sReturn <<"  "<<endl;
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
	double argument;
	if (mChild[0])
	{
		c1 = mChild[0]->eval(inPoint);
		argument = c1.sReturn; 
		
		if (c1.sReturn>1e-20)
			outPoint.sReturn = log10(argument);
		else 
		{
//			cerr << "Log10: input value too small	";
//			cout << mChild[0]->mLabel  << " " << argument << endl;
			outPoint.sReturn = -200.0;
		}

	}
	else 
	{
		cerr << "input not defined in log10"<<endl;
		outPoint.sReturn = -200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn << endl;
	return outPoint;
}

//**********************************************************************
tMeasPoint Log10Node::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	double argument;
	unsigned count = 0;
	if (mChild[0])
	{
		c1 = mChild[0]->evalfix(inPoint);
		argument = c1.sReturn;
		while ((argument<1e-6)&&(count<10))
		{
			mChild[0] = getNewNode();
			c1 = mChild[0]->evalfix(inPoint);
			argument = c1.sReturn;
			count++;
		}
		outPoint.sReturn = log10(argument);
	}
	else 
	{
		cerr << "input not defined in log10"<<endl;
		outPoint.sReturn = -200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"  "<< endl;
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
//		Exponent exp(x)
//
//**********************************************************************
Exponent::Exponent()
{
	mNumChildren = 1;
	mChild = new GOftn*[mNumChildren];
	mLabel = "Exp ";
	mIsConstant = false;
}

//**********************************************************************
tMeasPoint Exponent::eval(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	if (mChild[0])
	{
		c1 = mChild[0]->eval(inPoint);
		outPoint.sReturn = exp(c1.sReturn);
	}
	else 
	{
		cerr << "input not defined in exponent"<<endl;
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"  "<< endl;
	return outPoint;
}

//**********************************************************************
tMeasPoint Exponent::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	unsigned count = 0;
	c1 = mChild[0]->evalfix(inPoint);
	if (mChild[0])
	{
		while ((c1.sReturn>709)&&(count<10))
		{
			mChild[0] = getNewNode();
			c1 = mChild[0]->evalfix(inPoint);
			count++;
		}
		outPoint.sReturn = exp(c1.sReturn);
	}
	else 
	{
		cerr << "input not defined in exponent"<<endl;
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"  "<< endl;
	return outPoint;
}

//**********************************************************************
Exponent* Exponent::clone()
{
	Exponent* retNode = new Exponent;
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
	mLabel = "Square ";
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
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"  "<< endl;
	return outPoint;
}

//**********************************************************************
tMeasPoint Square::evalfix(tMeasPoint inPoint)
{
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1;
	unsigned count = 0;
	if (mChild[0])
	{
		c1 = mChild[0]->evalfix(inPoint);
		while ((fabs(log10(fabs(c1.sReturn)))>151)&&(count<10))
		{
			mChild[0] = getNewNode();
			c1 = mChild[0]->evalfix(inPoint);
			count++;
		}	
		outPoint.sReturn = c1.sReturn*c1.sReturn;
	}
	else 
	{
		cerr << "input not defined in square"<<endl;
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"  "<< endl;
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
	double returnval = 1;
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	double base, exp;
	if (mChild[0] && mChild[1])
	{
//		cout << "Power if true" << endl;
		c1 = mChild[0]->eval(inPoint);
		base = c1.sReturn;
		c2 = mChild[1]->eval(inPoint);
		exp = c2.sReturn; 
//		cout << mChild[0]->mLabel  << " " << base << endl;
		if (base>=0.0)
			outPoint.sReturn = pow(base, exp);
		else if ((exp>=0.0)&&(exp==(double)(int)(exp)))
//		exponent is positive whole number
		{
			returnval = 1;
			unsigned Exp = (unsigned)exp;
			for (unsigned i = 0; i<Exp; i++)
				returnval *=base;
			outPoint.sReturn = returnval;
		}
//		exponent is a negative whole number
		else if ((exp<0.0)&&(exp==(double)(int)(exp)))
		{
			returnval = 1;
			unsigned Exp = (unsigned)(-exp);
			for (unsigned i = 0; i<Exp; i++)
				returnval *= base;
			returnval = 1.0/returnval;
			outPoint.sReturn = returnval;	
		}
		else if ((fabs(base)<1e-100)&&(exp>0))
		{
			outPoint.sReturn = 0.0;
		}
		else
		{
//			cerr << "Power function not defined	";
//			cout << mChild[0]->mLabel  << " " << base << endl;
			outPoint.sReturn = -200.0;
		}
	}
	else 
	{
		cerr << "left and right not defined in Power"<<endl;
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"	"<< endl;	
	return outPoint;
}

//**********************************************************************
tMeasPoint Power::evalfix(tMeasPoint inPoint)
{
	double returnval = 1;
	tMeasPoint outPoint = inPoint;
	tMeasPoint c1, c2;
	unsigned count = 0;
	if (mChild[0] && mChild[1])
	{
		c1 = mChild[0]->evalfix(inPoint);
		c2 = mChild[1]->evalfix(inPoint);
		if (c1.sReturn>=0)
			outPoint.sReturn = pow(c1.sReturn, c2.sReturn);
		else if ((c2.sReturn>=0.0)&&(c2.sReturn==(double)(int)(c2.sReturn)))
//		exponent is positive whole number
		{
			returnval = 1;
			unsigned Exp = (unsigned)c2.sReturn;
			for (unsigned i = 0; i<Exp; i++)
				returnval *= c1.sReturn;
			outPoint.sReturn = returnval;
		}
//		exponent is a negative whole number
		else if ((c2.sReturn<0.0)&&(c2.sReturn==(double)(int)(c2.sReturn)))
		{
			returnval = 1;
			unsigned Exp = (unsigned)(-c2.sReturn);
			for (unsigned i = 0; i<Exp; i++)
				returnval *= c1.sReturn;
			returnval = 1.0/returnval;
			outPoint.sReturn = returnval;	
		}
		else if ((fabs(c1.sReturn)<1e-6)&&(c2.sReturn>0))
		{
			outPoint.sReturn = 0.0;
		}
		else
		{
			while ((c1.sReturn<0.0)&&(count<10))
			{
				mChild[0] = getNewNode();
				c1 = mChild[0]->evalfix(inPoint);
				count++;
			}
			count=0;
			while ((fabs(c2.sReturn)>100)&&(count<10))
			{
				mChild[1] = getNewNode();
				c2 = mChild[1]->evalfix(inPoint);
				count ++;
			}
			outPoint.sReturn = pow(c1.sReturn, c2.sReturn);
		} 
	}
	else 
	{
		cerr << "left and right not defined in Power"<<endl;
		outPoint.sReturn = 200.0;
	}
//	cout <<"	"<< mLabel << outPoint.sReturn <<"	"<< endl;	
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

