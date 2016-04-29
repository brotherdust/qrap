 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLinkAnalysis.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : This is the interface class to to display the link
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

#include "cLinkAnalysis.h"

#include "cConfirmLink.h"
#include <qwt_legend.h>


//***************************************************************/
cLinkAnalysis::cLinkAnalysis(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	setupUi(this);
	this->setModal(true);
	string location = gDb.GetSetting("location");
	if (location=="DD:MM:SS") mOutputFormat= dfDegMinSec;
	else if (location=="DD:MM.mm") mOutputFormat= dfDegMin;
	else if (location=="DD.dddd") mOutputFormat= dfDeg;
	else mOutputFormat= dfDegMinSec;
	mAutoName = false;
}


//***********************************************************************************
cLinkAnalysis::~cLinkAnalysis()
{
}


//************************************************************************************
void cLinkAnalysis::DoAndSetUpDisplay(eOutputUnits Units, bool Downlink, double Frequency, 
					double kFactor, double PlotResolution, 
					short int DEMsource, short int ClutterSource, 
					bool &UseClutter,
					int TxID, int RxID, QString LinkName, 
					QString TxName, QString RxName, int LinkID)
{
	mLinkID = LinkID;
	btnAlign->setEnabled(true);
	btnAlign->setVisible(true);
	btnRedo->setEnabled(false);
	btnRedo->setVisible(false);
	mTxID = TxID;
	mRxID = RxID;
	mLinkName = LinkName;
	mRxName = RxName;
	mTxName =TxName;
	mkFactor = kFactor;
	mUnits = Units;
	mDownlink = Downlink; 
	mPlotResolution = PlotResolution;
	mDEMsource = DEMsource;
	mClutterSource = ClutterSource; 
	mUseClutter = UseClutter;
	double maxH, minH;

//	cout << " mTxID: " << mTxID << "	mRxID: " << mRxID << endl;
	mLink.SetLink(Units, Downlink, Frequency, kFactor, mPlotResolution,
			DEMsource, ClutterSource, mUseClutter, mTxID, mRxID);
//	cout << "In cLinkAnalysis::DoAndSetUpDisplay( ... ) before mLink.DoLink()" << endl;
	mLink.DoLink();
//	cout << "In cLinkAnalysis::DoAndSetUpDisplay( ... ) after mLink.DoLink() before mLink.GetLinkInfo(...)" << endl;
	mLink.GetLinkInfo(mSize, mInterPixDist, 
			mMinClearance, mFrequency,
			mTxElevation, mRxElevation, 
			mTxTiltR, mRxTiltR, mTxAzimuthR, mRxAzimuthR,
			mPathLoss, mRxLevEnd, mTxAntHeight, mRxAntHeight, 
			mTxTilt,mRxTilt, mTxAzimuth, mRxAzimuth,
			mTxLat, mTxLon, mRxLat, mRxLon, maxH, minH);
	UseClutter = mUseClutter;

	lblTxName->setText(TxName);
	lblTxLat->setText(MakeStringFromDecDegrees(mTxLat,mOutputFormat,true));
	lblTxLon->setText(MakeStringFromDecDegrees(mTxLon,mOutputFormat,false));
	lblTxBearing->setText(QString("%1").arg(mTxAzimuth));
	lblTxTilt->setText(QString("%1").arg(mTxTilt));
	lblTxAntHeight->setText(QString("%1").arg(mTxAntHeight));
	lblRxName->setText(RxName);
	lblRxLat->setText(MakeStringFromDecDegrees(mRxLat,mOutputFormat,true));
	lblRxLon->setText(MakeStringFromDecDegrees(mRxLon,mOutputFormat,false));
	lblRxBearing->setText(QString("%1").arg(mRxAzimuth));
	lblRxTilt->setText(QString("%1").arg(mRxTilt));
	lblRxAntHeight->setText(QString("%1").arg(mRxAntHeight));
	lblkfactor->setText(QString("%1").arg(kFactor));
	lblFrequency->setText(QString ("%1").arg(mFrequency));
    	lblClearance->setText(QString("%1").arg(mMinClearance*100.0));
    	lblRxPower->setText(QString("%1").arg(mRxLevEnd));
	lblPathLoss->setText(QString("%1").arg(mPathLoss));
    	lblLength->setText(QString("%1").arg(mSize*mInterPixDist/1000.0));

	qwtLinkPlot->setTitle("Link Analysis: "+ LinkName+"  "+TxName+" to "+ RxName);
    	qwtLinkPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

    	// Set axis
	qwtLinkPlot->setAxisScale(qwtLinkPlot->yLeft,minH,maxH);
    	qwtLinkPlot->setAxisTitle(qwtLinkPlot->xBottom, "km");
    	qwtLinkPlot->setAxisTitle(qwtLinkPlot->yLeft, "Effective Height (m)");
    
    	qwtBTLPlot->insertLegend(new QwtLegend(), QwtPlot::RightLegend);

   	 // Set axis titles
    	qwtBTLPlot->setAxisTitle(qwtBTLPlot->xBottom, "km");
    	qwtBTLPlot->setAxisTitle(qwtBTLPlot->yLeft, "(dBm)");
   
	lblClearance->setText(QString("%1").arg(100*mMinClearance));
	lblTxElevation->setText(QString("%1").arg(mTxElevation));
	lblRxElevation->setText(QString("%1").arg(mRxElevation));
	lblTxTiltR->setText(QString("%1").arg(mTxTiltR));
	lblRxTiltR->setText(QString("%1").arg(mRxTiltR));
	lblTxBearingR->setText(QString("%1").arg(mTxAzimuthR));
	lblRxBearingR->setText(QString("%1").arg(mRxAzimuthR));
	
	mEffProf = new QwtPlotCurve("Effective Profile");
#if QT_VERSION >= 0x040000
	mEffProf->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mEffProf->setPen(QPen(Qt::darkRed));
    	mEffProf->attach(qwtLinkPlot);
    
    	mLOS = new QwtPlotCurve("Line of Sight");
#if QT_VERSION >= 0x040000
    	mLOS->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mLOS->setPen(QPen(Qt::green));
    	mLOS->attach(qwtLinkPlot);
    
    	mFresH = new QwtPlotCurve("100% Fresnel Z");
#if QT_VERSION >= 0x040000
    	mFresH->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mFresH->setPen(QPen(Qt::blue));
    	mFresH->attach(qwtLinkPlot);
    
    	mFresL = new QwtPlotCurve("100% Fresnel Z");
#if QT_VERSION >= 0x040000
    	mFresL->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mFresL->setPen(QPen(Qt::blue));
    	mFresL->attach(qwtLinkPlot);
    
    	mFresSH = new QwtPlotCurve("60% Fresnel Zone");
#if QT_VERSION >= 0x040000
    	mFresSH->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mFresSH->setPen(QPen(Qt::yellow));
    	mFresSH->attach(qwtLinkPlot);
    
    	mFresSL = new QwtPlotCurve("60% Fresnel Zone");
#if QT_VERSION >= 0x040000
    	mFresSL->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mFresSL->setPen(QPen(Qt::yellow));
    	mFresSL->attach(qwtLinkPlot);
    
//    	mClear = new QwtPlotCurve("Clearance");
//#if QT_VERSION >= 0x040000
//    	mClear->setRenderHint(QwtPlotItem::RenderAntialiased);
//#endif
//    	mClear->setPen(QPen(Qt::red));
//    	mClear->attach(qwtBTLPlot);
//    
/*    	mLoss = new QwtPlotCurve("Basic Transmission Loss");
//#if QT_VERSION >= 0x040000
    	mLoss->setRenderHint(QwtPlotItem::RenderAntialiased);
//#endif
    	mLoss->setPen(QPen(Qt::darkCyan));
    	mLoss->attach(qwtBTLPlot);
*/    
    	mRxLev = new QwtPlotCurve("Receiver Level");
#if QT_VERSION >= 0x040000
    	mRxLev->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mRxLev->setPen(QPen(Qt::darkMagenta));
    	mRxLev->attach(qwtBTLPlot);
 
//	cout << "In cLinkAnalysis::DoAndSetUpDisplay( ... ) before reassigning " << endl;
// The next code essentially translate the floats in cLink to doubles for the qwtplot     
    	double *Eff = new double[mSize];
    	double *Los = new double[mSize];
    	double *FrH = new double[mSize];
    	double *FrL = new double[mSize];
    	double *FrSH = new double[mSize];
    	double *FrSL = new double[mSize];
    	double *Grid = new double[mSize];
	double *Loss = new double[mSize];
	double *RxLev = new double[mSize];
	double *Grid1 = new double[mSize];
	double *Grid2 = new double[mSize];
	double *Grid3 = new double[mSize];
    	for (int i = 0;i <mSize;i++)
    	{
    		Eff[i] = (double)mLink.mEffProfile[i];
    		Los[i] = (double)mLink.mLineOfSight[i];
    		FrH[i] = (double)mLink.mFresnelH[i];
    		FrL[i] = (double)mLink.mFresnelL[i];
    		FrSH[i] = (double)mLink.mFresnelSH[i];
    		FrSL[i] = (double)mLink.mFresnelSL[i];
    		Grid[i] = i*mInterPixDist/1000;
		Loss[i] = (double)mLink.mPropLoss[i];
		RxLev[i] = (double)mLink.mRxLev[i];
		Grid1[i] = (double)mLink.mTrueHeightGrid1[i];
		Grid2[i] = (double)mLink.mTrueHeightGrid2[i];
		Grid3[i] = (double)mLink.mTrueHeightGrid3[i];
    	}  


	QPen GridPen(Qt::gray);
	GridPen.setStyle(Qt::DotLine);
	GridPen.setWidth(0);
    
	char *text = new char[33];
	gcvt(Grid1[(int)(mSize/2)],8,text); 
	mGrid1 = new QwtPlotCurve(text);
#if QT_VERSION >= 0x040000
    	mGrid1->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mGrid1->setPen(GridPen);
    	mGrid1->attach(qwtLinkPlot);

	gcvt(Grid2[(int)(mSize/2)],8,text); 
	mGrid2 = new QwtPlotCurve(text);
#if QT_VERSION >= 0x040000
    	mGrid2->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mGrid2->setPen(GridPen);
    	mGrid2->attach(qwtLinkPlot);

	gcvt(Grid3[(int)(mSize/2)],8,text);  
	mGrid3 = new QwtPlotCurve(text);
#if QT_VERSION >= 0x040000
    	mGrid3->setRenderHint(QwtPlotItem::RenderAntialiased);
#endif
    	mGrid3->setPen(GridPen);
    	mGrid3->attach(qwtLinkPlot);

  	mEffProf->setRawData(Grid,Eff,mSize);
  	mLOS->setRawData(Grid,Los,mSize);
  	mFresH->setRawData(Grid,FrH,mSize);
  	mFresL->setRawData(Grid,FrL,mSize);
  	mFresSH->setRawData(Grid,FrSH,mSize);
  	mFresSL->setRawData(Grid,FrSL,mSize);
  	//mClear->setRawData(Grid,Clr,mSize);
//  	mLoss->setRawData(Grid,Loss,mSize); 		
 	mRxLev->setRawData(Grid,RxLev,mSize);
	mGrid1->setRawData(Grid,Grid1,mSize);
	mGrid2->setRawData(Grid,Grid2,mSize);
	mGrid3->setRawData(Grid,Grid3,mSize);
}


//*****************************************************************************
void cLinkAnalysis::on_btnSave_clicked()
{
	//TODO call save Link from cLink.
	string LinkName = mLinkName.toStdString();
	mLink.SaveLink(LinkName,mLinkID);
	btnSave->setEnabled(false);
	btnSave->setVisible(false);
//	accept();
}

//*****************************************************************************
void cLinkAnalysis::on_btnAlign_clicked()
{
	btnAlign->hide();
	btnAlign->setEnabled(false);
	btnAlign->setVisible(false);

	char *temp = new char[33];
	gcvt(mTxTiltR,8,temp);
	string query = "Update radioinstallation set txmechtilt= ";
	query +=temp;
	query += ", rxmechtilt= ";
	query +=temp;
	gcvt(mTxAzimuthR,8,temp);
	query += ", txBearing= ";
	query +=temp;
	query += ", rxBearing= ";
	query +=temp;
	query += ", lastmodified=now() where ID=";
	gcvt(mTxID,8,temp);
	query +=temp;
	query +=";";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database link update failed"<< endl;
	}

	gcvt(mRxTiltR,8,temp);
	query = "Update radioinstallation set txmechtilt= ";
	query +=temp;
	query += ", rxmechtilt= ";
	query +=temp;
	gcvt(mRxAzimuthR,8,temp);
	query += ", txbearing= ";
	query +=temp;
	query += ", rxbearing= ";
	query +=temp;
	query += ", lastmodified=now() where ID=";
	gcvt(mRxID,8,temp);
	query +=temp;
	query +=";";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database link update failed"<< endl;
	}
	delete [] temp;
	mLink.DoLink();
	double maxH, minH;
	mLink.GetLinkInfo(mSize, mInterPixDist, mMinClearance, mFrequency,
			mTxElevation, mRxElevation, mTxTiltR, mRxTiltR, mTxAzimuthR, mRxAzimuthR,
			mPathLoss, mRxLevEnd, mTxAntHeight, mRxAntHeight,
			mTxTilt,mRxTilt, mTxAzimuth, mRxAzimuth,
			mTxLat, mTxLon, mRxLat, mRxLon, maxH, minH);

	lblTxName->setText(mTxName);
	lblTxLat->setText(QString("%1").arg(mTxLat));
	lblTxLon->setText(QString("%1").arg(mTxLon));
	lblTxBearing->setText(QString("%1").arg(mTxAzimuth));
	lblTxTilt->setText(QString("%1").arg(mTxTilt));
	lblTxAntHeight->setText(QString("%1").arg(mTxAntHeight));
	lblRxName->setText(mRxName);
	lblRxLat->setText(QString("%1").arg(mRxLat));
	lblRxLon->setText(QString("%1").arg(mRxLon));
	lblRxBearing->setText(QString("%1").arg(mRxAzimuth));
	lblRxTilt->setText(QString("%1").arg(mRxTilt));
	lblRxAntHeight->setText(QString("%1").arg(mRxAntHeight));
	lblkfactor->setText(QString("%1").arg(mkFactor));
	lblFrequency->setText(QString ("%1").arg(mFrequency));
    	lblClearance->setText(QString("%1").arg(mMinClearance*100));
    	lblRxPower->setText(QString("%1").arg(mRxLevEnd));
	lblPathLoss->setText(QString("%1").arg(mPathLoss));
    	lblLength->setText(QString("%1").arg(mSize*mInterPixDist/1000.0));
	lblTxElevation->setText(QString("%1").arg(mTxElevation));
	lblRxElevation->setText(QString("%1").arg(mRxElevation));
	lblTxTiltR->setText(QString("%1").arg(mTxTiltR));
	lblRxTiltR->setText(QString("%1").arg(mRxTiltR));
	lblTxBearingR->setText(QString("%1").arg(mTxAzimuthR));
	lblRxBearingR->setText(QString("%1").arg(mRxAzimuthR));
	btnRedo->show();
	btnRedo->setEnabled(true);
	btnRedo->setVisible(true);
	btnAlign->setEnabled(false);
	btnAlign->setVisible(false);
	btnSave->setEnabled(true);
	btnSave->setVisible(true);
//	accept();
}

//*****************************************************************************
void cLinkAnalysis::on_btnRedo_clicked()
{
	btnRedo->hide();
	btnRedo->setEnabled(false);
	btnRedo->setVisible(false);
	mLink.DoLink();
	double maxH, minH;
	mLink.GetLinkInfo(mSize, mInterPixDist, mMinClearance, mFrequency,
			mTxElevation, mRxElevation, mTxTiltR, mRxTiltR, mTxAzimuthR, mRxAzimuthR,
			mPathLoss, mRxLevEnd, mTxAntHeight, mRxAntHeight,
			mTxTilt,mRxTilt, mTxAzimuth, mRxAzimuth,
			mTxLat, mTxLon, mRxLat, mRxLon,maxH, minH);

	lblTxName->setText(mTxName);
	lblTxLat->setText(MakeStringFromDecDegrees(mTxLat,mOutputFormat,true));
	lblTxLon->setText(MakeStringFromDecDegrees(mTxLon,mOutputFormat,false));
	lblTxBearing->setText(QString("%1").arg(mTxAzimuth));
	lblTxTilt->setText(QString("%1").arg(mTxTilt));
	lblTxAntHeight->setText(QString("%1").arg(mTxAntHeight));
	lblRxName->setText(mRxName);
	lblRxLat->setText(MakeStringFromDecDegrees(mRxLat,mOutputFormat,true));
	lblRxLon->setText(MakeStringFromDecDegrees(mTxLon,mOutputFormat,false));
	lblRxBearing->setText(QString("%1").arg(mRxAzimuth));
	lblRxTilt->setText(QString("%1").arg(mRxTilt));
	lblRxAntHeight->setText(QString("%1").arg(mRxAntHeight));
	lblkfactor->setText(QString("%1").arg(mkFactor));
	lblFrequency->setText(QString ("%1").arg(mFrequency));
    	lblClearance->setText(QString("%1").arg(mMinClearance*100.0));
    	lblRxPower->setText(QString("%1").arg(mRxLevEnd));
	lblPathLoss->setText(QString("%1").arg(mPathLoss));
    	lblLength->setText(QString("%1").arg(mSize*mInterPixDist/1000.0));
	lblTxElevation->setText(QString("%1").arg(mTxElevation));
	lblRxElevation->setText(QString("%1").arg(mRxElevation));
	lblTxTiltR->setText(QString("%1").arg(mTxTiltR));
	lblRxTiltR->setText(QString("%1").arg(mRxTiltR));
	lblTxBearingR->setText(QString("%1").arg(mTxAzimuthR));
	lblRxBearingR->setText(QString("%1").arg(mRxAzimuthR));
	btnRedo->show();
	btnSave->setEnabled(true);
	btnSave->setVisible(true);
}

//***********************************************************************************************
void cLinkAnalysis::on_btnPDF_clicked()
{
	if (mAutoName)
	{
		mOutputFileName = mPath+mLinkName;
	}
	else
	{
		mOutputFileName = QFileDialog::getSaveFileName(this, "Export File Name", QString(),"PDF Documents (*.pdf)");
		if (mOutputFileName.isEmpty() )
    			return;
	}
	CreatePDF(mOutputFileName);
}

//*************************************************************************************************
//Print code addapted from qgsquickprint.cpp by (c) Tim Sutton, 2008
//
void cLinkAnalysis::CreatePDF(QString OutputFileName)
{
	mOutputFileName = OutputFileName;
  	//ensure the user never omitted the extension from the file name
 	if ( !mOutputFileName.toUpper().endsWith( ".PDF" ) )
   		 mOutputFileName += ".pdf";

  	// Initialising the printer this way lets us find out what
  	// the screen resolution is which we store and then
  	// reset the resolution of the printer after that...
  	QPrinter myPrinter( QPrinter::ScreenResolution );
	
  	// Try to force the printer resolution to 300dpi
  	// to get past platform specific defaults in printer
  	// resolution...
  	//
  	int myPrintResolutionDpi = 300;
  	myPrinter.setResolution( myPrintResolutionDpi );
  	myPrinter.setOutputFormat( QPrinter::PdfFormat );
  	myPrinter.setPageSize( QPrinter::A4 );
  	myPrinter.setOutputFileName( mOutputFileName );
  	myPrinter.setOrientation( QPrinter::Landscape );
  	myPrinter.setDocName( "quickprint Report" );
  	QPainter *myPrintPainter= new QPainter(&myPrinter);
  	myPrintPainter->setPen( Qt::gray );
  	myPrintPainter->setBrush( Qt::white );

	//set the top left origin for the print layout
  	int myOriginX = myPrinter.pageRect().left();
  	int myOriginY = myPrinter.pageRect().top();
  	int myDrawableWidth = myPrinter.pageRect().width() - myOriginX;
  	int myDrawableHeight = myPrinter.pageRect().height() - myOriginY;

  	//define the spacing between layout elements
  	int myHorizontalSpacing = myDrawableWidth / 100; // 1%
  	int myVerticalSpacing = myDrawableHeight / 100; // 1%

  	//define the proportions for the page layout
  	int myPlotWidthPercent = 100;
  	int myPlotHeightPercent =65;
  	int myTextWidthPercent = 100;
  	int myTextHeightPercent = 35;

  	//define the font sizes and family
  	QString myFontFamily = "Arial";
  	int myFontSize = 14;
	#ifdef Q_OS_LINUX
  	myFontSize -= 2;
	#endif
	#ifdef WIN32
  	myFontSize /= 2;
	#endif

  	// Background colour for pixmaps
  	QColor myLegendBackgroundColour = Qt::white;

  	// Draw the PageBorder
  	myPrintPainter->drawRect(myOriginX, myOriginY, myDrawableWidth, myDrawableHeight );

	//Draw the graph
	QRect myPlotRect( myOriginX, myOriginY, myDrawableWidth, myDrawableHeight*myPlotHeightPercent/100.0);
	QwtPlotPrintFilter pfilter = QwtPlotPrintFilter();
        qwtLinkPlot->print(myPrintPainter,myPlotRect,pfilter);
  
  	//SetUp the Font
  	QFont myFont( myFontFamily, myFontSize );
  	myPrintPainter->setFont( myFont );
  	QFontMetrics myFontMetrics( myFont, &myPrinter );
  	int myFontHeight = myFontMetrics.height();
	int myFontWidth = myFontMetrics.maxWidth();
	QString myText = " Received Power (dBm) :  ";
	int myColumnWidth = myFontMetrics.width(myText);

  	myOriginX += myHorizontalSpacing + myFontWidth;
  	myOriginY += myDrawableHeight*myPlotHeightPercent/100.0  + myVerticalSpacing + myFontHeight;

	DegreeFormat outputFormat;
	string location = gDb.GetSetting("location");
	if (location=="DD:MM:SS") outputFormat= dfDegMinSec;
	else if (location=="DD:MM.mm") outputFormat= dfDegMin;
	else if (location=="DD.dddd") outputFormat= dfDeg;
	else outputFormat= dfDegMinSec;

	//SetUp Tx Information
  	QRect myRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=mTxName;
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=MakeStringFromDecDegrees (mTxLat,outputFormat,true);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText= MakeStringFromDecDegrees (mTxLon,outputFormat,false);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Elevation : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mTxElevation);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Antenna Height : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mTxAntHeight);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="TxAzimuth : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mTxAzimuth);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="TxTilt : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mTxTilt);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );

	//SetUp Rx Information
	myOriginX=myPrinter.pageRect().left()+myDrawableWidth-myColumnWidth*2;
	myOriginY = myPrinter.pageRect().top()+myDrawableHeight*myPlotHeightPercent/100.0  + myVerticalSpacing + myFontHeight;

  	myRect.setRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=mRxName;
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=MakeStringFromDecDegrees (mRxLat,outputFormat,true);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX+myColumnWidth*0.5, myOriginY, myColumnWidth*1.5, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText= MakeStringFromDecDegrees (mRxLon,outputFormat,false);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Elevation (m) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mRxElevation);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Antenna Height (m) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mRxAntHeight);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="RxAzimuth : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mRxAzimuth);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="RxTilt : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mRxTilt);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );	

	//SetUp Link (middel) Information
	myOriginX=myPrinter.pageRect().left()+myDrawableWidth/2-myColumnWidth;
	myOriginY = myPrinter.pageRect().top()+myDrawableHeight*myPlotHeightPercent/100.0  + myVerticalSpacing + 2*myFontHeight;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Received Power (dBm) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mRxLevEnd);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Pathloss (dB) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mPathLoss);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Frequency(MHz) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mFrequency);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="k-Factor : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mkFactor);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Fresnel Clearance(%) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mMinClearance*100);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myRect.setRect( myOriginX, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText="Distance(km) : ";
	myPrintPainter->drawText( myRect, Qt::AlignRight, myText );
  	myRect.setRect( myOriginX+myColumnWidth, myOriginY, myColumnWidth, myFontHeight );
  	myPrintPainter->setPen( Qt::white );
  	myPrintPainter->drawRect( myRect );
  	myPrintPainter->setPen( Qt::black );
  	myText=QString("%1").arg(mSize*mInterPixDist/1000.0);
  	myPrintPainter->drawText( myRect, Qt::AlignLeft, myText );
	myOriginY +=myFontHeight+myVerticalSpacing;

  	myPrintPainter->end();
}



