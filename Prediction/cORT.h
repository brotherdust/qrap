 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cORT.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of perfectly useless *.ort files
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

#ifndef Qrap_cORT_h
#define Qrap_cORT_h
#include "cMemmngr.h"
#include "cGeoP.h"
#include "cRaster.h"
#include <sys/dir.h>
#include <stdio.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Qrap
{
	/**
	 * This class is used to create and read QRap's binary files needed for a fast DEM 
	 * and Clutter profile setup
	 */
struct tPoint
{
	float x;
	float y;
	float h;
};
typedef vector <tPoint> PointsArray;

class cORT
{
public:
		/**
		 * Constructor
		 */
		cORT();
		/**
		 * Destructor
		 */
		virtual ~cORT();
			/**
			 * Description of openFile
			 * 
			 * @param Raster Description
			 * @param Directory Description
			 * @param FileName Description
			 * @param NW Description
			 * @param SE Description
			 * @param Proj Description
			 * @param rows Description
			 * @param cols Description
			 * @param ns_res Description
			 * @param ew_res Description
			 *
			 * @return An integer.
			 */
		bool openFile(Float2DArray &Raster,
				string Directory, 
				cGeoP &NW, 
				cGeoP &SE, 
				GeoType &Proj, 
				projPJ &Proj4,
				uint &rows, 
				uint &cols, 
				double &ns_res, 
				double &ew_res,
				float &min,
				float &max,
				int &CentMer);
			
	};
}
#endif /*CORT_H_*/
