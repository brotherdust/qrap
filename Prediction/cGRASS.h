 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGRASS.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of grass file formats. 
 *                : Can slot into cRaster. Not really used
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

#ifndef CGRASS_H_
#define CGRASS_H_

#include "cGeoP.h"
#include "cMemmngr.h"
extern "C"{
#include <grass/gis.h>
#include <grass/gisdefs.h>
}
#include "cGDAL.h"
namespace Qrap{
typedef	vector<string> 	 	StrVec;

class cGRASS
{
public:
	cGRASS();
	virtual ~cGRASS();
	int openFile(Float2DArray &Raster,string Directory, string FileName, 
						cGeoP &NW, cGeoP &SE, GeoType &Proj,  projPJ &Proj4,
						uint &rows, uint &cols, double &ns_res, double &ew_res,
						float &min,	float &max,int CentMer);
	bool readRaster(Float2DArray &Raster);
	int writeFile(Float2DArray &Raster, string Directory, 
					string FileName,  cGeoP NW, cGeoP SE, uint rows, 
					uint cols, GeoType proj,
				double NSRes,
				double EWRes);
private:
	int generateRasterHeadder();
	void computeResolutions();
	int determineProj(GeoType Type); 
	GeoType determineProj(int Type);
	void breakDir(string Dir);
	void Rainbow();
	
	uint mRows,mCols;
	cGeoP mNW,mSE;
	GeoType mProj;
	projPJ Proj4;
	
	string GDBase;
	string Location;
	string MapSet;
		
	int fd;
	FCELL *buf;
	struct Cell_head cellhd;
	struct Colors colors;
	struct Categories cats;
	
	Float2DArray Raster;
	bool computeMinMax;
	
	RASTER_MAP_TYPE out_type;
	int mProjection;
	int mFormat;
	int GRASSMAX;
	static int error_routine ( char *msg, int fatal); // static because pointer to this function is set later
	double mNSRes,mEWRes;
	string mDir, mFile;

};
}
#endif /*GRASS_H_*/
