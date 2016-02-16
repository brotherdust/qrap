 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLinkAnalysis.h
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

#ifndef CLINKANALYSIS_H_
#define CLINKANALYSIS_H_

#include "../Prediction/cLink.h"
#include "../Prediction/PredStructs.h"
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
//#include <qwt_data.h>
#include <qwt_text.h>
#include <qprinter.h>
#include <qfiledialog.h>
#if QT_VERSION >= 0x040000
#include <qprintdialog.h>
#include <qfileinfo.h>
#else
#include <qwt_painter.h>
#endif
#include <qpicture.h>
#include <qpainter.h>
#include <qwt_painter.h>

#include "ui_LinkAnalysis.h"

using namespace std;
namespace Qrap
{
class cLinkAnalysis: public QDialog, private Ui::LinkAnalysis
{Q_OBJECT
public:
	cLinkAnalysis( QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cLinkAnalysis();
	
	void DoAndSetUpDisplay(eOutputUnits Units, bool Downlink, double Frequency, double kFactor,
			double PlotResolution, short int DEMsource, short int ClutterSource, bool &UseClutter,
			int TxID, int RxID, QString LinkName, QString TxName, QString RxName, int LinkID);
	void CreatePDF(QString OutputFileName);
	void SetAutoName(bool Input) {mAutoName=Input;};
	void SetPath(QString Input) {mPath = Input;};
	void MakePDF() {btnPDF->animateClick();};
	void OKClose() {buttonBox->button(QDialogButtonBox::Ok)->animateClick();};
										
public slots:
	void on_btnSave_clicked();
	void on_btnPDF_clicked();
	void on_btnAlign_clicked();
	void on_btnRedo_clicked();
			
private:
	QwtPlotCurve *mEffProf;
	QwtPlotCurve *mLOS;
	QwtPlotCurve *mFresH;
	QwtPlotCurve *mFresL;
	QwtPlotCurve *mFresSH;
	QwtPlotCurve *mFresSL;
	QwtPlotCurve *mClear;
	QwtPlotCurve *mLoss;
	QwtPlotCurve *mRxLev;
	QwtPlotCurve *mGrid1;
	QwtPlotCurve *mGrid2;
	QwtPlotCurve *mGrid3;
	cLink mLink;
	int mTxID;
	int mRxID;
	int mLinkID;
	int mSize;
	double mInterPixDist;
	QString mLinkName;
	QString mRxName;
	QString mTxName;
	double mTxTiltR;
	double mRxTiltR;
	double mTxAzimuthR;
	double mRxAzimuthR;
	double mTxTilt;
	double mRxTilt;
	double mTxAzimuth;
	double mRxAzimuth;
	double mTxAntHeight;
	double mRxAntHeight;
	double mMinClearance;
	double mFrequency;
	double mRxLevEnd;
	double mPathLoss;
	double mTxElevation;
	double mRxElevation;
	double mkFactor;
	double mTxLat, mTxLon, mRxLat, mRxLon;
	eOutputUnits mUnits;
	bool mDownlink; 
	double mRqSN;
	double mRxMin;
	double mFadeMargin; 
	double mRqEbNo;
	double mNoiseLevel; 
	double mPlotResolution;
	short int mDEMsource;
	short int mClutterSource; 
	bool mUseClutter;
	bool mAutoName;
	QPrinter::PageSize mPageSize;
	DegreeFormat mOutputFormat;
	QString mOutputFileName;
	QString mPath;
};
}
#endif /*CLINKANALYSIS_H_*/
