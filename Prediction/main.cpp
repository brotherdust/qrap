 /*
 *    Q-Rap Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : main.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Mostly for internal test purposes. Misused for background
 *                : raster file cutting
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

// Include local headers
#include <iostream>
#include "../DataBase/Config.h"
#include "cPlotTask.h"
#include "cLoadFile.h"
#include "cLink.h"
#include "cRasterFileHandler.h"
#include "cMeasAnalysisCalc.h"
#include "cMeasImportSpace.h"
#include "cMeasImportCSV.h"
#include "cGeoP.h"
#include "cPosEstimation.h"
#include "cTrainPosNetDistAngle.h"
#include "cTrainAntPattern.h"
#include "cTrafficDist.h"


using namespace std;
using namespace Qrap;

int main (int argc, char **argv)

{

//	bool error = false;
	cout << "Main 1" << endl;
	string queryC, queryN, queryP, queryA, query;
	
	if (!gSettings.LoadFromFile("/usr/lib/qgis/plugins/settings.xml"))
		return 0;

	if(!gDb.Connect ("postgres", "postqrap"))
	{
		cout << "Database connect failed:" << endl;
		return 0;
	}

	cout << "Connected " << endl;

/*	cTrafficDist TrafficDistribution;
	short int DEMsourceList=1;
	short int ClutterSourceList=2;
	bool UseClutter=true;
	double PlotRes = 5;
	double AngleRes = 1; 
	int Mobile = 1;
	double Range = 35;
	string StoreDir="/home/maggie/MTN2016/";
	string ResultFile="TrafficDist.img";
	cGeoP NW(-26.035, 27.975);
	cGeoP SE(-26.109, 28.075);

	TrafficDistribution.SetTrafficDist(dBm,true,8,-115,3,8,-120,1.33,
						DEMsourceList, ClutterSourceList, UseClutter,
						NW,SE, PlotRes, AngleRes, Mobile,
						Range, StoreDir, ResultFile);
	TrafficDistribution.PrimServPlot();
	TrafficDistribution.DetermineClutterDist();
	TrafficDistribution.WriteOutput(DEG);
*/
/*
	cGeoP point(-34,27.9);
	point.SetGeoType(WGS84GC);
	point.Display();
	point.SetGeoType(UTM);
	point.Display();
	point.Set(-34,27.9);
	point.SetGeoType(UTM);
	point.Display();
	point.SetGeoType(WGS84GC);
	point.Display();
*/
//	cAntennaPattern FixedAnt;
//	FixedAnt.SetAntennaPattern(29641, Tx, 0,  0);

/*
	cMeasImportOldTEMS VCMeasurements(2, 5, 1);
	cout << "Constructed " << endl;

	VCMeasurements.GetCurrentCellList();
	cout << endl << "got Celllist " << endl;

	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/23aug2.fmt" , '23/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/24aug1.fmt" , '24/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/24aug2.fmt" , '24/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/24aug3.fmt" , '24/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/campus.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv21a2.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv21a3.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv21a4.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv21a5.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv21aug.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv23aug.fmt" , '23/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drvbrook.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drvcityh.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_cty_hatf.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_ctypaulkruger.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_hatf_brook.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_hatf_cont.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_hatf.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_nrthPret_cont.fmt" , '21/08/2004');
	VCMeasurements.ReadTEMSfile("/home/maggie/Positioning/VC_Drive-tests2006/drv_nrthPret.fmt" , '21/08/2004');

	cout << endl << "Read File(s) " << endl;
	VCMeasurements.SortReduceCellList();
	cout << endl << "SortedCellList " << endl;
	VCMeasurements.WriteCellListToDB();
	cout << endl << "Wrote CellList to DB " << endl;
	VCMeasurements.GetCIfromBSICBCCH();
	cout << endl << "Got CI's " << endl;
	VCMeasurements.WriteToDatabase();
	cout << endl << "Wrote Measurements to DataBase" << endl;
*/

  	queryC = "update coefficients set coefficient=0.0;";
	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}

  	queryN = "update qrap_config set value='false' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(queryN))
	{
		cout << "Error updating qrap_config" << endl;
	}


	cGeoP *Hoek;
	unsigned NumHoek=4;
/*
//Gauteng 20m DEM
	NumHoek=19;
	Hoek = new cGeoP[NumHoek];

	Hoek[0].Set(-25.01, 28.99);
	Hoek[1].Set(-25.01, 28.51);
	Hoek[2].Set(-25.26, 28.51);
	Hoek[3].Set(-25.26, 28.01);
	Hoek[4].Set(-25.51, 28.01);
	Hoek[5].Set(-25.51, 27.76);
	Hoek[6].Set(-25.76, 27.76);
	Hoek[7].Set(-25.76, 27.26);
	Hoek[8].Set(-26.49, 27.26);
	Hoek[9].Set(-26.49, 27.51);
	Hoek[10].Set(-26.99, 27.51);
	Hoek[11].Set(-26.99, 28.49);
	Hoek[12].Set(-26.74, 28.49);
	Hoek[13].Set(-26.74, 28.99);
	Hoek[14].Set(-25.74, 28.99);
	Hoek[15].Set(-25.74, 29.24);
	Hoek[16].Set(-25.26, 29.24);
	Hoek[17].Set(-26.26, 28.99);
	Hoek[18].Set(-25.01, 28.99);
*/
	
//	Country
//	NumHoek=4;
//	Hoek = new cGeoP[NumHoek];
//	Hoek[0].Set(-24.01, 30.99);
//	Hoek[1].Set(-30.01, 30.99);
//	Hoek[2].Set(-30.01, 23.00);
//	Hoek[3].Set(-24.01, 23.00);

//	Bryanston
	NumHoek=4;
	Hoek = new cGeoP[NumHoek];
	Hoek[0].Set(-26.036, 27.976);
	Hoek[1].Set(-26.109, 27.976);
	Hoek[2].Set(-26.109, 28.075);
	Hoek[3].Set(-26.036, 28.075);

//	Tembisa
/*	NumHoek=6;
	Hoek = new cGeoP[NumHoek];
	Hoek[0].Set(-25.965, 28.210);
	Hoek[1].Set(-25.970, 28.245);
	Hoek[2].Set(-26.015, 28.255);
	Hoek[3].Set(-26.062, 28.175);
	Hoek[4].Set(-25.990, 28.150);
	Hoek[5].Set(-25.965, 28.180);
*/
	//Tembisa bigger
//	NumHoek=5;	
//	Hoek = new cGeoP[NumHoek];
//
//	Hoek[0].Set(-26.06, 28.26);
//	Hoek[1].Set(-25.94, 28.26);
//	Hoek[2].Set(-25.94, 28.113);
//	Hoek[3].Set(-26.06, 28.113);
//	Hoek[4].Set(-26.06, 28.26);

	vPoints Punte;
	for (unsigned i=0; i<NumHoek; i++)
		Punte.push_back(Hoek[i]);
	
	delete [] Hoek;

	double Mean, MSE, StDev, CorrC;
	cMeasAnalysisCalc Meas;
	int Num;
/*
	Meas.SetUseAntANN(false);

//	Meas.mPathLoss.mClutter.Reset(2);

	cout << "Loading measurements ... in main()" << endl;

	Meas.SetPlotResolution(30);
	Meas.LoadMeasurements(Punte,2,1,6);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);

	cout<< "Nach1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;

//	Meas.SetSeekWidthBest(1);
//	Meas.SetSmoothWidthBest(1);
//	Meas.OptimiseSeekWidth();
//        Meas.OptimiseHeights(0);
	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}

//	Meas.mPathLoss.mClutter.Reset(1);
	Meas.SetPlotResolution(30);
	Meas.LoadMeasurements(Punte,2,1,6);
	Meas.OptimiseModelCoefAllTotal(0);
   	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Result" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/
/*	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas.SetPlotResolution(5);
	Meas.LoadMeasurements(Punte,0,0,0);
   	Meas.OptimiseModelCoefD(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;

*/
/*	if (!gDb.PerformRawSql(queryC))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas.SetPlotResolution(5);
	Meas.LoadMeasurements(Punte,0,0,0);
	Meas.OptimiseOffsets(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "PostOffsets" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;


*/
//	cout<< "Na1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;

/*
	cTrainAntPattern NeuralNets;
	double Azimuth;

  	query = "truncate table AntNeuralNet cascade;";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error truncating Antenna Neural Nets" << endl;
	}
	
	cout << "In main Loading measurements " << endl;
	NeuralNets.LoadMeasurements(Punte,0,6);

	cout << "In main training nets " << endl;
	NeuralNets.TrainANDSaveANDTest();


  	query = "update qrap_config set value='true' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error updating qrap_config" << endl;
	}
	Meas.SetUseAntANN(true);
   Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Result" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/
//   Meas.OptimiseHeights(1);
/*
	Meas.OptimiseModelCoefAllTotal(1);
   Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Result" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
   Meas.OptimiseModelCoefD(1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
	cout<< "Na1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;
*/
/*
int i,j;

for ( i=4; i>=0; i--)
{
	Meas.LoadMeasurements(Punte, 0,0,i);
	Meas.SetSeekWidthBest(1);
	Meas.SetSmoothWidthBest(1);
	for(j=10;j<51;j=j+10)
	{
		cout <<"Set="<< i << "		 Res="	<< j << "	";
		Meas.SetPlotResolution(j);
//		Meas.OptimiseSeekWidth();
		Num = Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
		cout <<"Set="<< i << "	Res="	<< j << "	Num = " << Num << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl;
	}
}
*/
//


/*
  	query = "update coefficients set coefficient=0.0;";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}

	Meas.SetUseAntANN(false);
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,0);
	Meas.OptimiseModelCoefAllTotal(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;
   Meas.OptimiseModelCoefD(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/
	
	cTrainPosNetDistAngle NeuralNets;
	cout << "In main Loading measurements " << endl;
	NeuralNets.LoadSites(Punte,1,1,1,1);
	NeuralNets.LoadMeasurements(Punte,1,1,1,1,"Test",false);
	NeuralNets.LoadMeasurements(Punte,1,1,1,1,"Train",true);

	cout << "In main training nets " << endl;
	cout << "In main training nets " << endl;
	NeuralNets.TrainANDSaveANDTest();

/*
	query = "update coefficients set coefficient=0.0;";

	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}


  	query = "update qrap_config set value='false' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error updating qrap_config" << endl;
	}

	Meas.SetUseAntANN(false);

	cTrainAntPattern AntNeuralNets;
	double Azimuth;

  	query = "truncate table AntNeuralNet cascade;";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error truncating Antenna Neural Nets" << endl;
	}
	
	cout << "In main Loading measurements " << endl;
	AntNeuralNets.LoadMeasurements(Punte,0,6);

	cout << "In main training nets " << endl;
	AntNeuralNets.TrainANDSaveANDTest();

  	query = "update qrap_config set value='true' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error updating qrap_config" << endl;
	}

	Meas.LoadMeasurements(1,0,1);
	Meas.SetUseAntANN(true);

	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Result" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/
/*
	Meas.OptimiseOffsets(6);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "PostOffsets" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/

/*
	Meas.SetUseAntANN(true);
	cout << "Before PerformAnalysis ... in main()" << endl;
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 6);
	cout<< "Result" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;

  	query = "update coefficients set coefficient=0.0;";
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,0);
	Meas.OptimiseModelCoefAllTotal(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;
   Meas.OptimiseModelCoefD(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/
/*
	double hoek = 45;
	double x=cos(hoek/180*PI);
	double y=sin(hoek/180*PI);
	double angle = 180*atan2(y,x)/PI;
	cout << hoek << "		" << angle << endl;

	hoek = 135;
	x=cos(hoek/180*PI);
	y=sin(hoek/180*PI);
	angle = 180*atan2(y,x)/PI;
	cout << hoek << "		" << angle << endl;

	hoek = -45;
	x=cos(hoek/180*PI);
	y=sin(hoek/180*PI);
	angle = 180*atan2(y,x)/PI;
	cout << hoek << "		" << angle << endl;

	hoek = -135;
	x=cos(hoek/180*PI);
	y=sin(hoek/180*PI);
	angle = 180*atan2(y,x)/PI;
	cout << hoek << "		" << angle << endl;
*/

	cout << "Voor constructor" << endl;
	cPosEstimation Positioning;
	cout << "Na constructor" << endl;

 	queryP = "truncate positionestimate cascade;";
	if (!gDb.PerformRawSql(queryP))
	{
		cout << "Error truncating positionestimate" << endl;
	}
  	queryA = "update qrap_config set value='false' where name = 'UseAntANN';";
	if (!gDb.PerformRawSql(queryA))
	{
		cout << "Error updating qrap_config" << endl;
	}

	cout << "Voor LoadMeasurements" << endl;
	Positioning.SetUseAntANN(false);	
	Positioning.LoadMeasurements(Punte,1,1,1,1);
	cout << "Na LoadMeasurements" << endl;

	cout << " Clearing Punte " << endl;
	Punte.clear();

	cout << " Estimating Positions " << endl;
	Positioning.EstimatePositions();

	cout << " Saving Results " << endl;
	Positioning.SaveResults();

/*
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);

//	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);

	Meas. LoadMeasurements(0,0,4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Voor4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Voor3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC <<  endl;
	Meas. LoadMeasurements(0,0,2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Voor2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Voor1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl; 

	Meas. LoadMeasurements(0,0,0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Voor0,0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;


	Meas. LoadMeasurements(0,0,0);
	Meas.OptimiseModelCoefAllTotal(0);

	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0,0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0,1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0,2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC  << endl;
	Meas. LoadMeasurements(0,0,3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0,3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC  << endl;
	Meas. LoadMeasurements(0,0,4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na0,4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
*/
/*

	Meas. LoadMeasurements(0,0,0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);

	cout<< "Voor0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas.OptimiseModelCoefD(0);
	Meas. LoadMeasurements(0,0,0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nao0,0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nao0,1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC  << endl;
	Meas. LoadMeasurements(0,0,2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nao0,2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nao0,3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
	Meas. LoadMeasurements(0,0,4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nao0,4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl;
*/

/*
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements();
	Meas.OptimiseModelCoefD(0);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,0" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
   Meas. LoadMeasurements(0,0,1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,1" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
	Meas. LoadMeasurements(0,0,2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
	Meas. LoadMeasurements(0,0,3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
	Meas. LoadMeasurements(0,0,4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
	Meas. LoadMeasurements(0,0,5);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nac0,5" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev << "	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/

/*
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,5);
//	Meas.OptimiseModelCoefAllTotal(1);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na5" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC <<endl<< endl << endl << endl << endl;
	Meas.OptimiseModelCoefD(5);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach5" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC <<endl<< endl << endl << endl << endl;
*/
/*
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,2);
	Meas.OptimiseModelCoefAllTotal(2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC <<endl<< endl << endl << endl << endl;
	Meas.OptimiseModelCoefD(2);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nacb2" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC <<endl<< endl << endl << endl << endl;
*/

/*
	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,3);
	Meas.OptimiseModelCoefAllTotal(3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;
	Meas.OptimiseModelCoefD(3);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach3" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;


	if (!gDb.PerformRawSql(query))
	{
		cout << "Error clearing coefficients" << endl;
	}
	Meas.mPathLoss.mClutter.Reset(1);
	Meas. LoadMeasurements(0,0,4);
//	Meas.OptimiseModelCoefAllTotal(4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Na4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl  << endl << endl;
	Meas.OptimiseModelCoefD(4);
	Meas.PerformAnalysis(Mean, MSE, StDev, CorrC, 0);
	cout<< "Nach4" << "	Mean=" << Mean << "	MSE=" << MSE << "	StDev=" << StDev <<"	CorrC=" << CorrC << endl<< endl << endl << endl << endl;
*/

/*
{
cMeasImportCSV MeasImport(-120, 390,8, 1, 1, 1);
//MeasImport.SetCI(196);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Helderkruin.csv");
MeasImport.SetCI(8);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Kwamalhanga_0000_to_0131.csv");
MeasImport.SetCI(10);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Ronesterkop_0000_to_0262.csv");
//MeasImport.SetCI(11);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/RUST DE WINTER0.csv");
MeasImport.SetCI(164);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Benoni_0000_to_0134.csv");
MeasImport.SetCI(166);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Braamfontein_0000_to_0102.csv");
MeasImport.SetCI(168);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Bronkhorstspruit_0000_to_0419.csv");
MeasImport.SetCI(170);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/cable hill_0001_to_0147.csv");
//MeasImport.SetCI(176);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/daveyton_0000.csv");
MeasImport.SetCI(178);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Dawn Park_0000_to_0163.csv");
MeasImport.SetCI(182);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/DE TWEEDE SPRUIT_0002_to_0166.csv");
MeasImport.SetCI(184);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Devon_0000_to_0170.csv");
MeasImport.SetCI(186);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Donkerhoek_0000_to_0185.csv");
MeasImport.SetCI(190);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Groienfontien_0000_to_0229.csv");
MeasImport.SetCI(194);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/hekpoort_0000_to_0118.csv");
//MeasImport.SetCI(198);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Hennopsridge_0000_to_0079.csv");
MeasImport.SetCI(200);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/HoneyDew_0000_to_0074.csv");
MeasImport.SetCI(202);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Horizon Park_0000_to_0109.csv");
MeasImport.SetCI(204);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Jabulani_0000_to_0199.csv");
MeasImport.SetCI(206);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Johannesburg Hospital_0000_to_0033.csv");
MeasImport.SetCI(208);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Kameeldrift_0000_to_0116.csv");
MeasImport.SetCI(212);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Kloof Mine_0000_to_0207.csv");
MeasImport.SetCI(214);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Kromdraai_0000_to_0087.csv");
MeasImport.SetCI(216);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Langerand_0000_to_0201.csv");
MeasImport.SetCI(218);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Laudium_0000_to_0092.csv");
MeasImport.SetCI(220);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Lenasia South_0000_to_0185.csv");
MeasImport.SetCI(222);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Lindley_0000_to_0090.csv");
MeasImport.SetCI(225);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/MALGALIES_0000_to_0288.csv");
MeasImport.SetCI(227);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Midrand_0000_to_0112.csv");
MeasImport.SetCI(229);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Millgate Farm_0000_to_0132.csv");
MeasImport.SetCI(231);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/minerva_0000_to_0133.csv");
//MeasImport.SetCI(233);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/moreleta park_0000_to_0084.csv");
MeasImport.SetCI(235);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Modderfontein_0000_to_0155.csv");
MeasImport.SetCI(237);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Munsieville_0000_to_0134.csv");
MeasImport.SetCI(239);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/northclif_0002_to_0213.csv");
MeasImport.SetCI(241);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Observatory_0000_to_0063.csv");
MeasImport.SetCI(243);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/platberg_0000_to_0289.csv");
MeasImport.SetCI(245);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Protea Ridge_0000_to_0078.csv");
MeasImport.SetCI(247);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Randfontein_0000_to_0186.csv");
MeasImport.SetCI(249);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/randmarcht bank_0000_to_0083.csv");
MeasImport.SetCI(251);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/risana_0000_to_0100.csv");
MeasImport.SetCI(253);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/SEEKOEIFONTEIN_0000_to_0105.csv");
MeasImport.SetCI(256);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Silverton_0000_to_0065.csv");
MeasImport.SetCI(258);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Soshangeuwe_0000_to_0132.csv");
MeasImport.SetCI(260);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/SPRINGS_0000_to_0196.csv");
MeasImport.SetCI(264);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Tembisa_0000_to_0056.csv");
MeasImport.SetCI(266);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Tierpoort_0000_to_0117.csv");
MeasImport.SetCI(268);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/vaal marina_0000_to_0066.csv");
MeasImport.SetCI(268);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Vaal Marina_0000_to_0073.csv");
MeasImport.SetCI(270);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Vanderbijlpark_0000_to_0134.csv");
MeasImport.SetCI(272);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Vereeniging_0000_to_0135.csv");
MeasImport.SetCI(274);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Waverly_0000_to_0163.csv");
MeasImport.SetCI(276);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/Welbekend_0000_to_0326.csv");
MeasImport.SetCI(280);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/zonkizizwe_0000_to_0157.csv");
MeasImport.SetCI(85051);    MeasImport.LoadMeasurement("/home/maggie/MeasData/SAPS TETRA GAUTENG MEASUREMENT DATA/SAPS Gauteng Coverage/waterkoolf_0000_to_0124.csv");
}

{
cMeasImportSpace MeasImport(-120, 2145,85070, 1, 3, 1);
MeasImport.SetCI(85070);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0603r1.dat");
MeasImport.SetCI(85070);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0603r2.dat");
MeasImport.SetCI(85070);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0603r3.dat");
MeasImport.SetCI(85073);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2996r1.dat");
MeasImport.SetCI(85073);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2996r2.dat");
MeasImport.SetCI(85074);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0001r1.dat");
MeasImport.SetCI(85074);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0001r2.dat");
MeasImport.SetCI(85074);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0001r3.dat");
MeasImport.SetCI(85077);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0602r1.dat");
MeasImport.SetCI(85077);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T0602r2.dat");
MeasImport.SetCI(85078);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2135r1.dat");
MeasImport.SetCI(85078);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2135r2.dat");
MeasImport.SetCI(85079);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2997r1.dat");
MeasImport.SetCI(85079);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2997r2.dat");
MeasImport.SetCI(85080);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T3986r1.dat");
MeasImport.SetCI(85080);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T3986r2.dat");
MeasImport.SetCI(85080);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T3986r3.dat");
MeasImport.SetCI(85080);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T3986r4.dat");
MeasImport.SetCI(85081);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2518r1.dat");
MeasImport.SetCI(85081);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Dense Urban Sites/Tuning/T2518r2.dat");
MeasImport.SetCI(85066);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0205r1.dat");
MeasImport.SetCI(85066);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0205r2.dat");
MeasImport.SetCI(85076);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0237r1.dat");
MeasImport.SetCI(85076);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0237r2.dat");
MeasImport.SetCI(85076);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0237r3.dat");
MeasImport.SetCI(85072);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0277r1.dat");
MeasImport.SetCI(85072);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0277r2.dat");
MeasImport.SetCI(85072);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0277r3.dat");
MeasImport.SetCI(85075);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0411r1.dat");
MeasImport.SetCI(85075);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0411r2.dat");
MeasImport.SetCI(85075);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0411r3.dat");
MeasImport.SetCI(85067);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T0435r2.dat");
MeasImport.SetCI(85068);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T1086r1.dat");
MeasImport.SetCI(85068);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T1086r2.dat");
MeasImport.SetCI(85060);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T1440r1.dat");
MeasImport.SetCI(85060);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T1440r2.dat");
MeasImport.SetCI(85071);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T2197r1.dat");
MeasImport.SetCI(85071);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T2197r2.dat");
MeasImport.SetCI(85071);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T2197r3.dat");
MeasImport.SetCI(85065);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T2525r2.dat");
MeasImport.SetCI(85069);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T3322r1.dat");
MeasImport.SetCI(85069);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Urban Sites/Tuning/T3322r2.dat");
MeasImport.SetCI(85062);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0054r1.dat");
MeasImport.SetCI(85062);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0054r2.dat");
MeasImport.SetCI(85062);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0054r3.dat");
//MeasImport.SetCI(85061);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0200r1.dat");
//MeasImport.SetCI(85061);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0200r2.dat");
MeasImport.SetCI(85057);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0370r1.dat");
MeasImport.SetCI(85057);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0370r2.dat");
MeasImport.SetCI(85054);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0471r1.dat");
MeasImport.SetCI(85054);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0471r2.dat");
MeasImport.SetCI(85053);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0477r1.dat");
MeasImport.SetCI(85053);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0477r2.dat");
MeasImport.SetCI(85053);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0477r3.dat");
MeasImport.SetCI(85058);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0500r1.dat");
MeasImport.SetCI(85058);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0500r2.dat");
MeasImport.SetCI(85058);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0500r3.dat");
MeasImport.SetCI(85056);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0615r1.dat");
MeasImport.SetCI(85056);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0615r2.dat");
MeasImport.SetCI(85056);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0615r3.dat");
MeasImport.SetCI(85064);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0757r1.dat");
MeasImport.SetCI(85064);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0757r2.dat");
MeasImport.SetCI(85064);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0757r3.dat");
MeasImport.SetCI(85063);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0894r1.dat");
MeasImport.SetCI(85063);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T0894r2.dat");
MeasImport.SetCI(85052);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1031r1.dat");
MeasImport.SetCI(85052);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1031r2.dat");
MeasImport.SetCI(85052);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1031r3.dat");
MeasImport.SetCI(85052);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1031r4.dat");
MeasImport.SetCI(85055);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1944r1.dat");
MeasImport.SetCI(85055);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1944r2.dat");
MeasImport.SetCI(85055);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTNUMTS/Suburban Sites/Tuning/T1944r3.dat");
}


{
cMeasImportSpace MeasImport(-120, 1817.4,85082, 1, 4, 1);
//RSA Centre - 1817.4MHz
MeasImport.SetCI(85082);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T0002_1800.dat");
//Royal York Flats - 1817.6MHz
MeasImport.SetCI(85084);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T0236_1800.dat");
// Konica Minolta - 1817.4MHz
MeasImport.SetCI(85085);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T5997_1800.dat");
//MTN Doornfontein Switch - 1817.4MHz
//MeasImport.SetCI(85086);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T3986_1800.dat");
//Old Medical School -1817.4MHz
MeasImport.SetCI(85087);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T0658_1800.dat");
//Commanche Flats - 1817.6MHz
MeasImport.SetCI(85095);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T3748_1800.dat");
//Atteridgeville W/Res - 1817.4MHz
MeasImport.SetCI(85099);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T0053_1800.dat");
}
{
cMeasImportSpace MeasImport(-120, 1829.8,85089, 1, 4, 1);
//Princess Towers - 1829.8MHz
//MeasImport.SetCI(85089);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T0435_1800.dat");
//Sanlam Plaza East - 1829.8MHz
MeasImport.SetCI(85090);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T0049_1800.dat");
//BMW warehouse - 1829.8MHz
//MeasImport.SetCI(85091);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T0411_1800.dat");
//Discount Glass and Aluminium- 1829.8MHz
//MeasImport.SetCI(85092);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T0606_1800.dat");
//Laerskool Simon Bekker- 1829.8MHz
MeasImport.SetCI(85093);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T2525_1800.dat");
//270 Strubenstraat - 1829.8MHz
//MeasImport.SetCI(85094);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T2998_1800.dat");
//Hatfield Metro Virgin Active - 1829.8MHz
//MeasImport.SetCI(85096);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T3764_1800.dat");
//Avril Flats - 1829.8MHz
MeasImport.SetCI(85097);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Urban_1800/T4142_1800.dat");
//Willmor Park - 1829.8MHz
MeasImport.SetCI(85098);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T0052_1800.dat");
//Garsfontein High School - 1829.8MHz
MeasImport.SetCI(85100);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T0868_1800.dat");
//Lehlabile Secondary School - 1829.8MHz
MeasImport.SetCI(85101);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T3009_1800.dat");
//Taphuis Restaurant - 1829.8MHz
MeasImport.SetCI(85102);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T3021_1800.dat");
//Highveld Park S/Centre- 1829.8MHz
MeasImport.SetCI(85103);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T5354_1800.dat");
//Palmland Nursery Akasia - 1829.8MHz
MeasImport.SetCI(85104);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T5380_1800.dat");
//Holding 66 Montana AH - 1829.8MHz
MeasImport.SetCI(85105);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Suburban_1800/T5924_1800.dat");
//Prinsloo - 1829.8MHz
MeasImport.SetCI(85106);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T0151_1800.dat");
//Suikerbosfontein Farm - 1829.8MHz
MeasImport.SetCI(85107);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T0152_1800.dat");
//De Wagensdrift Farm - 1829.8MHz
//MeasImport.SetCI(85108);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T1525_1800.dat");
//Sterling Farm Delmas - 1829.8MHz
MeasImport.SetCI(85109);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T2186_1800.dat");
//Renosterkop - 1829.8MHz
MeasImport.SetCI(85110);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T2398_1800.dat");
//Mahlangu Farm - 1829.8MHz
MeasImport.SetCI(85111);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T2932_1800.dat");
//Schultz Farm - 1829.8MHz
MeasImport.SetCI(85112);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T3684_1800.dat");
//Geldenhuys Farm- 1829.8MHz
MeasImport.SetCI(85113);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia files Rural_1800/T3690_1800.dat");
//Bryanston NW CC - 1830MHz
//MeasImport.SetCI(85088);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T4683_1800.dat");
}

{
cMeasImportSpace MeasImport(-120, 1876.8,85083, 1, 4, 1);
//Baragwana Nurses Home - 1876.8 MHz
MeasImport.SetCI(85083);    MeasImport.LoadMeasurement("/home/maggie/MeasData/MTN1800/Signia_1800_Urban_Johannesburg/T0066_1800.dat");
}


{
cMeasImportSpace MeasImport(-120, 945,539, 1, 2, 1);
MeasImport.SetCI(539);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T2567all.dat");
MeasImport.SetCI(540);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T2857all.dat");
MeasImport.SetCI(541);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T2899all.dat");
//MeasImport.SetCI(542);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T2911all.dat");
MeasImport.SetCI(543);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T2999all.dat");
MeasImport.SetCI(544);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T3004all.dat");
MeasImport.SetCI(545);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T3007all.dat");
//MeasImport.SetCI(545);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CWMeas/c_T3007all.dat");
MeasImport.SetCI(546);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T3540all.dat");
MeasImport.SetCI(547);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T3519all.dat");
MeasImport.SetCI(548);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CW2001/c_T3506all.dat");
MeasImport.SetCI(550);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CWMeas/c_T0945all.dat");
MeasImport.SetCI(551);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CWMeas/c_T1440All.dat");
MeasImport.SetCI(552);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CWMeas/c_T2525all.dat");
MeasImport.SetCI(553);    MeasImport.LoadMeasurement("/home/maggie/MeasData/CWMeas/c_T3024all.dat");
}
*/
/*
	char *text = new char[33];

	query = "INSERT INTO filesets values (1, now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' BryanstonDEM ', ' BryanstonDEM ', 'DEM', null, 5, now(), true, ";
	query += " false, null, 'GDALFILE', 'UTM', ";
	query += "'+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs');";
	if ((!gDb.PerformRawSql(query)))

	{
		cout << "Database Update on filesets table failed"<< endl;
		return false;
	}
	query = "INSERT INTO filesets values (2,now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' BryanstonDEMcut ', ' BryanstonDEMcut ', 'DEM', null, 5, now(), true, ";
	query += " true, 1, 'BINFILE', 'WGS84GC', '');";

	if ((!gDb.PerformRawSql(query)))
	{
		cout << "Database Update on filesets table failed"<< endl;

		return false;
	}


	string dir="/home/maggie/GISdata/Bryanston/";
	cLoadFile Files(1,2,dir);
	Files.LoadFile("/home/maggie/GISdata/Bryanston/","DEM_5m.img",GDALFILE,UTM,
			"+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
			35, true);
	Files.WriteDB();

	query = "INSERT INTO filesets values (3, now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' BryanstonClutter ', ' BryanstonClutter ', 'Clutter', null, 5, now(), true, ";
	query += " false, null, 'GDALFILE', 'UTM', ";
	query += "'+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs');";
	if ((!gDb.PerformRawSql(query)))

	{
		cout << "Database Update on filesets table failed"<< endl;
		return false;
	}
	query = "INSERT INTO filesets values (4,now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' BryanstonClutterCut ', ' BryanstonClutterCut ', 'Clutter', null, 5, now(), true, ";
	query += " true, 3, 'BINFILE', 'WGS84GC', '');";

	if ((!gDb.PerformRawSql(query)))
	{
		cout << "Database Update on filesets table failed"<< endl;

		return false;
	}


	cLoadFile Files2(3,4,dir);
	Files2.LoadFile("/home/maggie/GISdata/Bryanston/","Clutter_5m.img",GDALFILE,UTM,
			"+proj=utm +zone=35 +south +ellps=WGS84 +datum=WGS84 +units=m +no_defs",
			35, true);
	Files2.WriteDB();
*/

//	string dir="/home/maggie/DEM/SRTM/BIN/";
	//	string dir="/home/anita/SRTM/BIN/";
//	cLoadFile Files(3,4,dir);
//	Files.LoadFile("../Data/SRTM/srtm_40_19/","srtm_40_19.asc",GDALFILE,DEG);
//	Files.WriteDB();
//	delete [] text;
//	Files.CutCurrentSet(3,4);


/*	string dir="/home/anita/GISdata/SRTM/BIN/";
	cLoadFile Files(3,4,dir);
	cout << "Voor cut" << endl;
	Files.CutCurrentSet(3,4, true, 1);
*/
	// For Arc Grid
/*	string dir="/home/maggie/Data/RasterData/BIN";
	cLoadFile Files(1,2,dir);
	Files.LoadFile("/home/maggie/Data/RasterData/dtm_gt_bot","w001001a.adf",GDALFILE,DEG);
	Files.LoadFile("/home/maggie/Data/RasterData/dtm_gt_top","w001001.adf",GDALFILE,DEG);
	Files.WriteDB();
*/
/*	char *text = new char[33];
	string query = "INSERT INTO filesets values (11, now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' Original SRTM 90m ArcASCII ', 'DEM', null, 90, now(), true, ";
	query += " false, null, 'GDALFILE', 'DEG', ");";
	if ((!gDb.PerformRawSql(query)))
	{
		cout << "Database Update on filesets table failed"<< endl;
		return false;
	}	
	query = "INSERT INTO filesets values (12,now(),";
	gcvt(gDb.globalMachineID,10,text);
	query +=text; 
	query += ", ";	
	query += " ' Cut SRTM 90m ArcASCII ', 'DEM', null, 90, now(), true, ";
	query += " true, 11, 'BINFILE', 'WGS84GC', ");";
	if ((!gDb.PerformRawSql(query)))
	{
		cout << "Database Update on filesets table failed"<< endl;
		return false;
	}
*/
/*
	string dir="/home/maggie/GISdata/SRTM/";
	//	string dir="/home/anita/SRTM/BIN/";
	cLoadFile Files(1,1,dir);
	Files.LoadFile("/home/maggie/GISdata/SRTM","srtm_42_18.asc",GDALFILE,DEG);
	Files.WriteDB();
//	delete [] text;
//	Files.CutCurrentSet(3,4);
*/
// For Ort files for general loads
/*	cLoadFile Files;

Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/cd/Pro",dir,"2530cd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2328/ba",dir,"2328ba25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/dd",dir,"2530dd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2525/bc",dir,"2525bc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/cc",dir,"2530cc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/ad",dir,"2330ad25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/bd",dir,"2330bd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2527/bd",dir,"2527bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/Bd",dir,"2528Bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/da",dir,"3228da25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2228/dc",dir,"2228dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2228/dd",dir,"2228dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2229/cc",dir,"2229cc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Bc",dir,"2230Bc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Bd",dir,"2230Bd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Da",dir,"2230Da25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Db",dir,"2230Db25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Dc",dir,"2230Dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2230/Dd",dir,"2230Dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2326/Db/Pro",dir,"2326Db25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2326/Dd/Pro",dir,"2326Dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Ac/Pro",dir,"2327Ac25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/ad/Pro",dir,"2327ad25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Ba/Pro",dir,"2327Ba25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/bb",dir,"2327bb25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Bc/Pro",dir,"2327Bc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/bd",dir,"2327bd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Ca/Pro",dir,"2327Ca25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Cb/Pro",dir,"2327Cb25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Cc/Pro",dir,"2327Cc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Cd/Pro",dir,"2327Cd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Da/Pro",dir,"2327Da25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Db/Pro",dir,"2327Db25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Dc/Pro",dir,"2327Dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2327/Dd/Pro",dir,"2327Dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2328/bb",dir,"2328bb25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2328/Da",dir,"2328Da25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2329/aa",dir,"2329aa25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2329/cd",dir,"2329cd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2329/dc",dir,"2329dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/aa",dir,"2330aa25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/ab",dir,"2330ab25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/ac",dir,"2330ac25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/Ba",dir,"2330Ba25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/Bb",dir,"2330Bb25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/bc",dir,"2330bc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/da",dir,"2330da25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/db",dir,"2330db25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/dc",dir,"2330dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2330/dd",dir,"2330dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Ca",dir,"2331Ca25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Cb",dir,"2331Cb25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Cc",dir,"2331Cc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Cd",dir,"2331Cd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Da",dir,"2331Da25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Dc",dir,"2331Dc25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2331/Dd",dir,"2331Dd25m.bin",4,23);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2429/AA",dir,"2429AA25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2429/Ba",dir,"2429Ba25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2429/Cc",dir,"2429Cc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/AC",dir,"2430AC25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/AD",dir,"2430AD25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/Ba",dir,"2430Ba25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/Bb",dir,"2430Bb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/BC",dir,"2430BC25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/BD",dir,"2430BD25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/da",dir,"2430da25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2430/db",dir,"2430db25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Aa",dir,"2431Aa25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Ab",dir,"2431Ab25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Ca",dir,"2431Ca25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Cb",dir,"2431Cb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Cc",dir,"2431Cc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2431/Cd",dir,"2431Cd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2525/bd",dir,"2525bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2525/Cd",dir,"2525Cd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2525/dc",dir,"2525dc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2525/dd",dir,"2525dd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2526/Ba/Pro",dir,"2526Ba25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2526/Bb/Pro",dir,"2526Bb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2526/Bc/Pro",dir,"2526Bc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2526/Bd/Pro",dir,"2526Bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2527/ac",dir,"2527ac25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2527/AD",dir,"2527AD25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2527/DB",dir,"2527DB25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2527/Dd",dir,"2527Dd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/ab",dir,"2528ab25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/ac",dir,"2528ac25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/ad",dir,"2528ad25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/ba",dir,"2528ba25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/bc",dir,"2528bc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/ca",dir,"2528ca25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/cb",dir,"2528cb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/cc",dir,"2528cc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/cd",dir,"2528cd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/da",dir,"2528da25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2528/dc",dir,"2528dc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/Aa",dir,"2529Aa25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/Ac",dir,"2529Ac25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/Ad",dir,"2529Ad25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/bb",dir,"2529bb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/bd",dir,"2529bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/Ca",dir,"2529Ca25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/Cb",dir,"2529Cb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2529/dc",dir,"2529dc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/Aa",dir,"2530Aa25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/Ab",dir,"2530Ab25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/Ac",dir,"2530Ac25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/Ad",dir,"2530Ad25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/bd",dir,"2530bd25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/ca",dir,"2530ca25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/cb",dir,"2530cb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/Da",dir,"2530Da25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/db",dir,"2530db25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2530/dc",dir,"2530dc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/ac",dir,"2531ac25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/ad",dir,"2531ad25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/bc",dir,"2531bc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/ca",dir,"2531ca25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/cb",dir,"2531cb25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/cc",dir,"2531cc25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2531/da",dir,"2531da25m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/ca",dir,"2623ca25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/cb",dir,"2623cb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/cc",dir,"2623cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/cd",dir,"2623cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/da",dir,"2623da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2623/dc",dir,"2623dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2625/Ab",dir,"2625Ab25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2625/Ba",dir,"2625Ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2625/Bb",dir,"2625Bb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2626/ba",dir,"2626ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2626/bb",dir,"2626bb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2626/dc",dir,"2626dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/Ba",dir,"2627Ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/bb",dir,"2627bb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/bc",dir,"2627bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/da",dir,"2627da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/db",dir,"2627db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2627/dd",dir,"2627dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/aa",dir,"2628aa25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/ab",dir,"2628ab25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/ac",dir,"2628ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/ad",dir,"2628ad25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/ba",dir,"2628ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/bc",dir,"2628bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/ca",dir,"2628ca25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/cb",dir,"2628cb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2628/da",dir,"2628da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2629/dc",dir,"2629dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2629/dd",dir,"2629dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/aa",dir,"2630aa25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/ab",dir,"2630ab25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/ac",dir,"2630ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/ad",dir,"2630ad25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/ba",dir,"2630ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/bb",dir,"2630bb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/bc",dir,"2630bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/bd",dir,"2630bd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/ca",dir,"2630ca25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/cb",dir,"2630cb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/cc",dir,"2630cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/cb",dir,"2630cb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/cc",dir,"2630cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/Da",dir,"2630Da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2630/Db",dir,"2630Db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2631/Aa",dir,"2631Aa25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2632/Cc",dir,"2632Cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2632/Cd",dir,"2632Cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2632/Dc",dir,"2632Dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2632/Dd",dir,"2632Dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2722/Bd",dir,"2722Bd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2722/Db",dir,"2722Db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2722/Dd",dir,"2722Dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/AA",dir,"2723AA25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/AB",dir,"2723AB25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/Ac",dir,"2723Ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/AD",dir,"2723AD25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/BA",dir,"2723BA25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/BB",dir,"2723BB25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/BC",dir,"2723BC25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/BD",dir,"2723BD25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/Ca",dir,"2723Ca25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2723/Cc",dir,"2723Cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2724/Bc",dir,"2724Bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2724/Bd",dir,"2724Bd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/aa",dir,"2728aa25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/ab",dir,"2728ab25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/ac",dir,"2728ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/ad",dir,"2728ad25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/bc",dir,"2728bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/cd",dir,"2728cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/da",dir,"2728da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/db",dir,"2728db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/dc",dir,"2728dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2728/dd",dir,"2728dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2729/dd",dir,"2729dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/Ac",dir,"2730Ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/Ad",dir,"2730Ad25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/Bc",dir,"2730Bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/cc",dir,"2730cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/cd",dir,"2730cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/db",dir,"2730db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/dc",dir,"2730dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2730/dd",dir,"2730dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2731/Cd",dir,"2731Cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2731/Da",dir,"2731Da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2731/Db",dir,"2731Db25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2731/Dd",dir,"2731Dd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Aa",dir,"2732Aa25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Ab",dir,"2732Ab25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Ac",dir,"2732Ac25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Ad",dir,"2732Ad25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Ba",dir,"2732Ba25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Bb",dir,"2732Bb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Bc",dir,"2732Bc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Bd",dir,"2732Bd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Ca",dir,"2732Ca25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Cb",dir,"2732Cb25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Cc",dir,"2732Cc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Cd",dir,"2732Cd25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Da",dir,"2732Da25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2732/Dc",dir,"2732Dc25m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2819/Cb",dir,"2819Cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2819/Cc",dir,"2819Cc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2819/Cd",dir,"2819Cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2819/Da",dir,"2819Da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Ac",dir,"2820Ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Ca",dir,"2820Ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Cb",dir,"2820Cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Cd",dir,"2820Cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Da",dir,"2820Da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Db",dir,"2820Db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Dc",dir,"2820Dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2820/Dd",dir,"2820Dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2821/Ad",dir,"2821Ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2821/Bc",dir,"2821Bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2825/CA",dir,"2825CA25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2825/CB",dir,"2825CB25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2826/ba",dir,"2826ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2826/bb",dir,"2826bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2826/cb",dir,"2826cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2826/da",dir,"2826da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/bc",dir,"2827bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/Bd",dir,"2827Bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/cb",dir,"2827cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/Cd",dir,"2827Cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/DB",dir,"2827DB25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/Dc",dir,"2827Dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2827/Dd",dir,"2827Dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Aa",dir,"2828Aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Ab",dir,"2828Ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Ac",dir,"2828Ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Ad",dir,"2828Ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/bb",dir,"2828bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/bc",dir,"2828bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/bd",dir,"2828bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Ca",dir,"2828Ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/Cb",dir,"2828Cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/da",dir,"2828da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2828/db",dir,"2828db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/aa",dir,"2829aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/ac",dir,"2829ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/ad",dir,"2829ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/bc",dir,"2829bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/bd",dir,"2829bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Ca",dir,"2829Ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Cb",dir,"2829Cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Cc",dir,"2829Cc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Cd",dir,"2829Cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Da",dir,"2829Da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Db",dir,"2829Db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Dc",dir,"2829Dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2829/Dd",dir,"2829Dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/aa",dir,"2830aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/ab",dir,"2830ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/ac",dir,"2830ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/ad",dir,"2830ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/ba",dir,"2830ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/bb",dir,"2830bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/bc",dir,"2830bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/bd",dir,"2830bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/ca",dir,"2830ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/cc",dir,"2830cc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/cd",dir,"2830cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/da",dir,"2830da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/db",dir,"2830db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/dc",dir,"2830dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2830/dd",dir,"2830dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/aa",dir,"2831aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/ab",dir,"2831ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/ac",dir,"2831ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/ad",dir,"2831ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/ba",dir,"2831ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/bb",dir,"2831bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/bc",dir,"2831bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/ca",dir,"2831ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/cb",dir,"2831cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/cd",dir,"2831cd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/da",dir,"2831da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/db",dir,"2831db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/dc",dir,"2831dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2831/dd",dir,"2831dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/aa",dir,"2832aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/Ab",dir,"2832Ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/Ad",dir,"2832Ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/Ba",dir,"2832Ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/ca",dir,"2832ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/cb",dir,"2832cb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2832/cc",dir,"2832cc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2919/Aa",dir,"2919Aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/aa",dir,"2922aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/ab",dir,"2922ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/ac",dir,"2922ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/ad",dir,"2922ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/da",dir,"2922da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/db",dir,"2922db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/bc",dir,"2922bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2922/bd",dir,"2922bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2923/ac",dir,"2923ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2923/ca",dir,"2923ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2924/ca",dir,"2924ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/aa",dir,"2926aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/ab",dir,"2926ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/ba",dir,"2926ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/bb",dir,"2926bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/bc",dir,"2926bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/bd",dir,"2926bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2926/db",dir,"2926db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/aa",dir,"2927aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/ab",dir,"2927ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/ac",dir,"2927ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/ba",dir,"2927ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/bb",dir,"2927bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2927/ca",dir,"2927ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2929/bb",dir,"2929bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2929/bc",dir,"2929bc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2929/bd",dir,"2929bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2929/da",dir,"2929da25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2929/dc",dir,"2929dc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/Aa",dir,"2930Aa25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/ba",dir,"2930ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/bb",dir,"2930bb25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/bd",dir,"2930bd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/Ca",dir,"2930Ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/Db",dir,"2930Db25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2930/Dd",dir,"2930Dd25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/ab",dir,"2931ab25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/ac",dir,"2931ac25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/ad",dir,"2931ad25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/ba",dir,"2931ba25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/ca",dir,"2931ca25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2931/cc",dir,"2931cc25m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3026/cd",dir,"3026cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3026/Da",dir,"3026Da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3026/Db",dir,"3026Db25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3027/ac",dir,"3027ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3027/Cc/Pro",dir,"3027Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3027/Cd/Pro",dir,"3027Cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3027/Dc/Pro",dir,"3027Dc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3027/Dd/Pro",dir,"3027Dd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/CB",dir,"3028CB25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/Cc",dir,"3028Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/Cd",dir,"3028Cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/DA",dir,"3028DA25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/DB",dir,"3028DB25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/Dc",dir,"3028Dc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3028/Dd",dir,"3028Dd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Ac",dir,"3029Ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Ad",dir,"3029Ad25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Ca",dir,"3029Ca25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Cb",dir,"3029Cb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Cc",dir,"3029Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Cd",dir,"3029Cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Da",dir,"3029Da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Db",dir,"3029Db25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Dc",dir,"3029Dc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3029/Dd",dir,"3029Dd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/aa",dir,"3030aa25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/ab",dir,"3030ab25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/ac",dir,"3030ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/ad",dir,"3030ad25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/bb",dir,"3030bb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/bd",dir,"3030bd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/ca",dir,"3030ca25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/cb",dir,"3030cb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/cc",dir,"3030cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/cd",dir,"3030cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3030/da",dir,"3030da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3124/Cc",dir,"3124Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3124/Cd",dir,"3124Cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3126/ab",dir,"3126ab25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3126/bb",dir,"3126bb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3126/bd",dir,"3126bd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Ac",dir,"3127Ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Ad",dir,"3127Ad25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Bc",dir,"3127Bc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Bd",dir,"3127Bd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Ca",dir,"3127Ca25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Cb",dir,"3127Cb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Cc",dir,"3127Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Cd",dir,"3127Cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/da",dir,"3127da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/db",dir,"3127db25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/dc",dir,"3127dc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3127/Dd",dir,"3127Dd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/aa",dir,"3128aa25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/ab",dir,"3128ab25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Ac",dir,"3128Ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Ad",dir,"3128Ad25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Ba",dir,"3128Ba25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Bb",dir,"3128Bb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Bc",dir,"3128Bc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Bb",dir,"3128Bb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Ca",dir,"3128Ca25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Cb",dir,"3128Cb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/Cc",dir,"3128Cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/cd",dir,"3128cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/da",dir,"3128da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/db",dir,"3128db25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/dc",dir,"3128dc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3128/dd",dir,"3128dd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Aa",dir,"3129Aa25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Ab",dir,"3129Ab25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/ac",dir,"3129ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Ad",dir,"3129Ad25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Ba",dir,"3129Ba25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Bb",dir,"3129Bb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/bc",dir,"3129bc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Bd",dir,"3129Bd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/ca",dir,"3129ca25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/Cb",dir,"3129Cb25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/cc",dir,"3129cc25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/cd",dir,"3129cd25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3129/da",dir,"3129da25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3130/aa",dir,"3130aa25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3130/ab",dir,"3130ab25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3130/ac",dir,"3130ac25m.bin",4,31);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3217/db",dir,"3217db25m.bin",4,32);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3217/dd",dir,"3217dd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3218/ca",dir,"3218ca25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3218/cc",dir,"3218cc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3222/Ab",dir,"3222Ab25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3222/Ad",dir,"3222Ad25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3222/Ba",dir,"3222Ba25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3222/Bc",dir,"3222Bc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3224/Ab",dir,"3224Ab25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3224/Ad",dir,"3224Ad25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3224/Ba",dir,"3224Ba25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3224/Bc",dir,"3224Bc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Ad",dir,"3226Ad25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Bc",dir,"3226Bc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Ca",dir,"3226Ca25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Cb",dir,"3226Cb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Cc",dir,"3226Cc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Cd",dir,"3226Cd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Da",dir,"3226Da25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Db",dir,"3226Db25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Dc",dir,"3226Dc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3226/Dd",dir,"3226Dd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3227/Ca",dir,"3227Ca25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3227/Cc",dir,"3227Cc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3227/Cd",dir,"3227Cd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/ab",dir,"3228ab25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/ac",dir,"3228ac25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/ad",dir,"3228ad25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/BA",dir,"3228BA25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/bb",dir,"3228bb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/bc",dir,"3228bc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/bd",dir,"3228bd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/cb",dir,"3228cb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/cc",dir,"3228cc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3228/cd",dir,"3228cd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3229/aa",dir,"3229aa25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3317/bb",dir,"3317bb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/aa",dir,"3318aa25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/cb",dir,"3318cb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/cd",dir,"3318cd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/Da",dir,"3318Da25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/db",dir,"3318db25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/dc",dir,"3318dc25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3318/dd",dir,"3318dd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3319/cb",dir,"3319cb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3319/da",dir,"3319da25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3322/Cb",dir,"3322Cb25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3322/Cd",dir,"3322Cd25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3322/Da",dir,"3322Da25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3324/DC",dir,"3324DC25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3324/DD",dir,"3324DD25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3327/Aa",dir,"3327Aa25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3327/Ab",dir,"3327Ab25m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3418/AB",dir,"3418AB25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3418/Ad",dir,"3418Ad25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3418/ba",dir,"3418ba25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3418/bb",dir,"3418bb25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3419/ac",dir,"3419ac25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3419/ad",dir,"3419ad25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3424/ba",dir,"3424ba25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/3424/bb",dir,"3424bb25m.bin",4,35);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2429ab",dir,"2429ab50m.bin",4,24);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2527ab",dir,"2527ab50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2527ad",dir,"2527ad50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2527da",dir,"2527da50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2527dc",dir,"2527dc50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2527dd",dir,"2527dd50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2529cc",dir,"2529cc50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2529cd",dir,"2529cd50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2529dc",dir,"2529dc50m.bin",4,25);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2626bb",dir,"2626bb50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2627ba",dir,"2627ba50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2627bc",dir,"2627bc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2627ca",dir,"2627ca50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628aa",dir,"2628aa50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628bb",dir,"2628bb50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628cc",dir,"2628cc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628cd",dir,"2628cd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628dc",dir,"2628dc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2628dd",dir,"2628dd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2629aa",dir,"2629aa50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2726ba",dir,"2726ba50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2726bb",dir,"2726bb50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2726bc",dir,"2726bc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2726dc",dir,"2726dc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2726dd",dir,"2726dd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727ab",dir,"2727ab50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727ad",dir,"2727ad50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727ba",dir,"2727ba50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727bb",dir,"2727bb50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727bc",dir,"2727bc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727ca",dir,"2727ca50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727CB",dir,"2727CB50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2727da",dir,"2727da50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2728ba",dir,"2728ba50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2729bd",dir,"2729bd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2729dd",dir,"2729dd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2729db",dir,"2729db50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2730/da",dir,"2730da50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2731/cc",dir,"2731cc50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2731/cd",dir,"2731cd50m.bin",4,27);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2826/ba",dir,"2826ba50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2826/bb",dir,"2826bb50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926aa",dir,"2926aa50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926ab",dir,"2926ab50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926ba",dir,"2926ba50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926bb",dir,"2926bb50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926bc",dir,"2926bc50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/2926bd",dir,"2926bd50m.bin",4,29);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3222bc",dir,"3222bc50m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3227cd",dir,"3227cd50m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3227dc",dir,"3227dc50m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3227dd",dir,"3227dd50m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3228aa",dir,"3228aa50m.bin",4,33);
Files.LoadOrt("/media/STORAGEBIRD/DEMS/50M_W_DEM1/3228ac",dir,"3228ac50m.bin",4,33);
*/
//Die volgende vier leers het 4 kolomme
//Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2824/da",dir,"2824da25m.bin",4,29);
//Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2824/db",dir,"2824db25m.bin",4,29);
//Files.LoadOrt("/media/STORAGEBIRD/DEMS/50 M Dems/2831/aa",dir,"2831aa50m.bin",4,29);
//Files.LoadOrt("/media/STORAGEBIRD/DEMS/25m DEMS/2626/dd",dir,"2626dd25m.bin",4,27);

//	

/*	cPlotTask prediction;	
	// Get the test data to run a prediction
	prediction.ReadPlotRequest("ExamplePlotRequest");
	
	// Run the prediction
	prediction.CombineCov();
	prediction.WriteOutput(DEG);
*/
/*	cLink Link;
	Link.ReadPlotRequest("ExampleLinkRequest");
	Link.DoLink();
	FILE *fp = fopen ("/home/maggie/LinkOutput","w");
	Link.WriteASCIIMatrix(fp);
	fclose(fp);
*/
/*	cPlotTask prediction;	
	// This is to test getting the DEM data
	prediction.ReadPlotRequest("ExamplePlotRequest");
 	cRaster Output;
	cRasterFileHandler DEM(1);
	string Dir = "/home/maggie/BTLplots/";
	string OutputFile ="DEMWat";
	cGeoP NW(-29.9,24.9,DEG);
	cGeoP SE(-30.1,25.1,DEG);
	int central = 25;
	double DistRes = 90;
	unsigned Rows=2, Cols=2;
	Float2DArray Data;
	Data = new_Float2DArray(Rows, Cols);
	DEM.GetForDEM(NW, SE, DistRes, Rows, Cols, Data,DEG);
	cout << Rows << "  " << Cols << endl;
	NW.SetGeoType(DEG);
	SE.SetGeoType(DEG);
	DistRes*=cDegResT;
	Output.WriteFile(Data, NW, SE, Rows, Cols, DistRes, DistRes,
					Dir,OutputFile,HFA,DEG,DEG,central);
	delete_Float2DArray(Data);
*/

	return 0;
}

