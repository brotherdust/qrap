 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGDAL.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of general file formats that
 *                : can be handled by gdal. Slots into cRaster
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

#ifndef Qrap_cGDAL_h
#define Qrap_cGDAL_h

using namespace std;

// Include local headers
#include "cGeoP.h"
#include "cMemmngr.h"
#include <string>
#include <stdio.h>
#include "../DataBase/Config.h"

// Include global headers
#include <gdal.h>
#include <cpl_string.h>
#include <ogr_srs_api.h>
#include <ogr_spatialref.h>
#include <gdal_priv.h>

#undef CPL_SUPRESS_CPLUSPLUS

namespace Qrap
{
	/**
	 * Description of class cGDAL
	 */
	class cGDAL
	{
		public:
			/**
			 * Constructor
			 */
			cGDAL();
			
			/**
			 * Descructor
			 */
			virtual ~cGDAL();
			
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
			bool openFile (Float2DArray &Raster,
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
			 * Description of readRaster
			 * 
			 * @param Raster Description
			 *
			 * @return A boolean.
			 */
			//bool readRaster(Float2DArray &Raster);

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
				string type,
				double NSRes,
				double EWRes);
				
		bool writeFile(Float2DArray &Raster,
				string Directory, 
				string FileName, 
				cGeoP &NW, 
				GeoType &InProj,
				GeoType &OutProj, 
				projPJ &InProj4,
				projPJ &OutProj4,
				uint &rows, 
				uint &cols,
				int CentMer,
				string type,
				double NSRes,
				double EWRes);
			
		private:
			string ReturnProj4(GeoType &Type,int CentMer, bool Hem=true);
			GDALDataset  	*poDataset;
			string 		m_file_name;
			GDALRasterBand  *poBand;
			int             nBlockXSize, nBlockYSize;
			int             bGotMin, bGotMax;
			double          adfMinMax[2];
			float 		*pafScanline;
			unsigned 	Rows,Cols;
			double *       	mAdfGeoTransform;
			OGRSpatialReference poSRS;
			static char 	**temp1;
			bool		mSouth;
	};
}
#endif /*GDAL_H_*/
