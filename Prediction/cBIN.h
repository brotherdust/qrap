 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cBIN.h
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

#ifndef Qrap_cBIN_h
#define Qrap_cBIN_h
#include "cMemmngr.h"
#include "proj.h"
#include "cGeoP.h"
#include <string.h>
#include <stdio.h>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include "../DataBase/Config.h"

using namespace std;

namespace Qrap
{
	/**
	 * This class is used to create and read QRap's binary files needed for a fast DEM 
	 * and Clutter profile setup
	 */
class cBIN
{
public:
		/**
		 * Constructor
		 */
		cBIN();
		/**
		 * Destructor
		 */
		virtual ~cBIN();
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
				string FileName, 
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
			/**
			 * Description of writeFile
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
			 * @param CentMer Description
			 *
			 * @return An integer.
			 */	
		bool writeFile(Float2DArray &Raster,
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
				double EWRes);
	};
}
#endif /*CBIN_H_*/
