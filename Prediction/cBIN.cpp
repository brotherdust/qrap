 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBIN.cpp
 *    Copyright   : (c) University of Pretoria & Meraka Institute CSIR
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of custom binary files
 *                : Slots into cRaster
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


#include "cBIN.h"
using namespace Qrap;

//***************************************************************
cBIN::cBIN()
{
}

//***************************************************************
cBIN::~cBIN()
{
}

//*****************************************************************************
bool cBIN::openFile(Float2DArray &Raster,string Directory, string FileName, 
						cGeoP &NW, cGeoP &SE, GeoType &Proj, projPJ &Proj4,
						uint &rows, uint &cols, double &ns_res, double &ew_res,
						float &min,	float &max,int &CentMer)
{
	char *binfname;
	binfname = new char[100];
	char *QRap;
	QRap = new char[100];
	ifstream NESbinfile;			// DTM data file
 	int central;			// central meridian of data co-ordinates
 	double s_lat, n_lat, e_lon, w_lon;  // boundaries of file data
 	unsigned i,j;                            // loop variables
	sprintf(binfname,"%s/%s",Directory.c_str(),FileName.c_str());
	NESbinfile.open(binfname,  ios::in | ios::binary);
	#ifdef _MSC_VER
	if (!NESbinfile.is_open())		// Binary doesn't file exists
    #else
    if (!NESbinfile)
    #endif
	{
    	string err = "Trouble opening BIN File";
		QRAP_WARN(err.c_str());
		
		cout << "NOT OPENED" <<endl;
		return false;
	} 
	
/*	NESbinfile.read((char *)&QRap,sizeof(QRap));
//	cout << QRap << endl;
	string Temp = QRap;
	if (Temp != "QRap Bin File")
	{
		string err = "Not a BIN File";
		QRAP_WARN(err.c_str());
		cout << "Not Bin " <<QRap <<endl;
		return false;
	}
*/		
	NESbinfile.read((char *)&ns_res,sizeof(ns_res));
	NESbinfile.read((char *)&ew_res,sizeof(ew_res));
	NESbinfile.read((char *)&cols,sizeof(cols));
	NESbinfile.read((char *)&rows,sizeof(rows));
	NESbinfile.read((char *)&n_lat,sizeof(n_lat));
	NESbinfile.read((char *)&w_lon,sizeof(w_lon));
	NESbinfile.read((char *)&s_lat,sizeof(s_lat));
	NESbinfile.read((char *)&e_lon,sizeof(e_lon));
	NESbinfile.read((char *)&central,sizeof(central));
	NESbinfile.read((char *)&Proj,sizeof(Proj));
//	NESbinfile.read((char *)&QRap,sizeof(QRap));
	NW.Set(n_lat,w_lon,Proj,central);
	SE.Set(s_lat,e_lon,Proj,central);
	cGeoP TNW = NW;
	cGeoP TSE = SE;
	TNW.SetGeoType(DEG);
	TSE.SetGeoType(DEG);
	CentMer = central;
//	ns_res = 0;
//	ew_res = 0;
	min = 0; // Need to write this in...
	max = 0; // Need to write this in...
	Proj4 = NULL;
	//pj_free(Proj4);
	//Proj4 = pj_init_plus(QRap);
	//cout << pj_get_def(Proj4,0) << endl;
	// load the DTM data into the array
	delete_Float2DArray(Raster);
	Raster = new_Float2DArray(rows, cols);
	//trace("allocated memory");
	for (i=0; i<rows; i++)
	{
		for (j=0;j<cols;j++)
			NESbinfile.read((char *)&Raster[i][j],sizeof(float));
	}
	cout << "Bin Done" << endl;
	delete [] binfname;
	delete [] QRap;
	return true;
}

//**************************************************************
bool cBIN::writeFile(Float2DArray &Raster,
				string Directory, 
				string FileName, 
				cGeoP &NW, 
				cGeoP &SE, 
				GeoType &Proj, 
				projPJ &Proj4,
				uint &rows, 
				uint &cols,
				int CentMer,
				double NSRes,
				double EWRes)
{
	bool South;
	char binfname[100];
//	char QRap[100];
	ifstream NESbinfile,NESfile;			// DTM data file
 	ofstream NESoutfile;
 	GeoType Type;
 	int central;			// central meridian of data co-ordinates
 	double s_lat, n_lat, e_lon, w_lon;  // boundaries of file data
 	unsigned i,j;                            // loop variables
 	double NSres,EWres;
	NW.Get(n_lat,w_lon,Type,central,South);
	SE.Get(s_lat,e_lon,Type,central,South);
	central = CentMer;
	NSres = NSRes;
	EWres = EWRes;
//	printf("%f/%f\n",NSres,EWres);
//	printf("%f/%f\n",n_lat,w_lon);
//	printf("%f/%f\n",s_lat,e_lon);
	sprintf(binfname,"%s/%s",Directory.c_str(),FileName.c_str());
//	cout << binfname << endl;
//	cout << rows << "   rxc   " << cols << endl;
//	cout << Raster << endl;
	NESoutfile.open(binfname, ios::out | ios::binary);
	
	#ifdef _MSC_VER
	if (NESoutfile.is_open())		// Binary file exists
	#else
	if (NESoutfile)
	#endif
	{
//		sprintf(QRap,"QRap Bin File");
//		NESoutfile.write((char*)&QRap,sizeof(QRap));
		NESoutfile.write((char *)&NSres,sizeof(NSres));
		NESoutfile.write((char *)&EWres,sizeof(EWres));
		NESoutfile.write((char *)&cols,sizeof(cols));
		NESoutfile.write((char *)&rows,sizeof(rows));
		NESoutfile.write((char *)&n_lat,sizeof(n_lat));
		NESoutfile.write((char *)&w_lon,sizeof(w_lon));
		NESoutfile.write((char *)&s_lat,sizeof(s_lat));
		NESoutfile.write((char *)&e_lon,sizeof(e_lon));
		NESoutfile.write((char *)&central,sizeof(central));
		NESoutfile.write((char *)&Proj,sizeof(Proj));
		// \TODO Need to change this...
//		Proj4 = NULL; 
//		sprintf(QRap,"%s",pj_get_def(Proj4,0));
//		cout << "Binary Write: " << QRap <<endl;
//		NESoutfile.write((char *)&QRap,sizeof(QRap));
		for (i=0; i<rows; i++)
		{
//			NESoutfile.write((char *)Raster[i],cols*sizeof(float));
			for (j=0;j<cols;j++)
				NESoutfile.write((char *)&Raster[i][j],sizeof(float));
				
		}
//		cout << endl;
		NESoutfile.close();
		return true; 
	}
	return false;
}

