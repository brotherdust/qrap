 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cConfirmPrediction.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    print part used and adapted from qgsquickprint.cpp by Tim Sutton (c) 2008
 *    Description : This is the interface part that initiates area predictions
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


#ifndef CCONFIRMPREDICTION_H_
#define CCONFIRMPREDICTION_H_

#include <stdio.h>

// QRap includes
#include "../DataBase/Config.h"
#include "../Prediction/cGeoP.h"
#include "../Prediction/cPlotTask.h"
#include "ui_ConfirmPrediction.h"

//QT4 includes
#include <QTreeWidgetItem>
#include <QFile>
#include <QDir>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QFileDialog>
#include <QMessageBox>
#include <QObject>
#include <QColor>


//QGIS includes
#include <qgsmaprenderer.h>
#include <qgsrasterrenderer.h>
#include <qgsmapcanvas.h>
#include <qgspoint.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgisinterface.h>
#include <qgisgui.h>
#include <qgsapplication.h>
#include <qgslogger.h>
#include <qgslabelattributes.h>
#include <qgslabel.h>
#include <qgscolorrampshader.h>
#include <qgssinglebandpseudocolorrenderer.h>
#include <qgsrasterlayer.h>

//using namespace std;
namespace Qrap
{

class cConfirmPrediction: public QDialog, private Ui::ConfirmPrediction
{
Q_OBJECT
public:
	cConfirmPrediction();
	cConfirmPrediction( QgisInterface * QgisInt, QWidget* parent = 0, Qt::WFlags fl = 0 );
	virtual ~cConfirmPrediction();
	bool SetPoints(QList<QgsPoint> Points);

public slots:
	void on_btnCancel_clicked();
	void on_btnDo_clicked();
	void on_btnDone_clicked();
	void on_plotTypeCombo_currentIndexChanged(QString File);
	void on_pushButtonPrint_clicked();

private:
	QString mBTLDir;
	void SetOwnColours();
	void renderPrintScaleBar( QPainter * thepPainter,
                              QgsMapRenderer * thepMapRenderer,
                              int theMaximumWidth );
	QStringList wordWrap( QString theString,
                          QFontMetrics theMetrics,
                          int theWidth );
	QgsMapSettings mMapRenderer; 
	QgsMapCanvas * mMapCanvas; 
	
	
	void LoadDefaults();
	
	ePlotType mPlotType; 
	eOutputUnits DisplayUnits;
	bool DownLink;
	double RequiredSignalToNoise;
	double RequiredMinimumReceiverLevel;
	double FadeMargin;
	double RequiredCoChannelCarrierToInterference;
	double RequiredAdjacentCarrierToInterference;
	double RequiredEnergyPerBitToNoiseRatio;
	double NoiseLevel;
	double kFactorForServers;
	double kFactorForInterferers;
	short int DEMsourceList;
	short int ClutterSourceList;
	bool UseClutterDataInPathLossCalculations;
	cGeoP NorthWestCorner;
	cGeoP SouthEastCorner;
	double PlotResolution;
	double MinimumAngularResolution;
	unsigned MobileInstallationKey;
	unsigned NumberOfFixedInstallations;
	unsigned *FixedInstallationKeys;
	double *CoverangeRanges; // In Kilometer
	string DirectoryToStoreResult;
	string OutputFileForResult;
	QgisInterface * mQGisIface;
	
	QList<unsigned> mRadInst;
	QList<double> mRanges;
	QList<QgsColorRampShader::ColorRampItem> mColorRampItems;
	
};
}

#endif /*CCONFIRMPREDICTION_H_*/
