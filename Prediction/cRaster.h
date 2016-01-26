 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRaster.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents a raster of points e.g. DEM raster file
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


#ifndef Qrap_cRaster_h
#define Qrap_cRaster_h


// Include local headers
#include "cGeoP.h"
#include "cMemmngr.h"
#include "cGDAL.h"
#include "cBIN.h"

namespace Qrap
{
	#define OUTOFRASTER -9999.9
	/**
	 * enum FileType - For Files to read
	 */
	enum FileType 
	{
		UNKNOWNFILE, ///< Unknown File Type
		BINFILE, ///< QRAP Binary File Type
		NESFILE, ///< NES File Type
		GRASSFILE,
		GDALFILE,///< Any GDAL File Type
		ORTFILE	///< ORT File Type
	};
	/**
	 * enumOutPut - For Files to write
	 */
	enum OutPut {
		BIN, ///< QRAP Binary File Type
		GRASS,///< GOETiff GDAL File Type
		VRT,///< VRT GDAL File Type
		NITF,///< NITF GDAL File Type
		HFA,///< HFA GDAL File Type
		ELAS,///< ELAS GDAL File Type
		AAIGRID,///< AAIGrid GDAL File Type
		MEM, ///< MEM GDAL File Type
		GIF, ///< GIF GDAL File Type
		XPM,///< XPM GDAL File Type
		BMP,///< BMP GDAL File Type
		PCIDSK, ///< PCIDSK GDAL File Type
		PCRASTER,///< PCRaster GDAL File Type
		ILWIS, ///< ILWIS GDAL File Type
		GMT, ///< GMT GDAL File Type
		NETCDF, ///< netCDF GDAL File Type
		HDF4IMAGE, ///< HDF4Image GDAL File Type
		ENVI, ///< ENVI GDAL File Type
		ENDR, ///< ENdr GDAL File Type
		PAUX, ///< PAux GDAL File Type
		MFF, ///< MFF GDAL File Type
		MFF2, ///< MFF2 GDAL File Type
		BT, ///< BT GDAL File Type
		IDA, ///< IDA GDAL File Type
		JPEG2000, ///< JPEG2000 GDAL File Type
		RMF, ///< RMF GDAL File Type
		RST,///< RST GDAL File Type
		GEOTIFF
		
		
	};

	/**
	 * Description of cRaster
	 */
	class cRaster
	{
		public:
			/**
			 * Constructor
			 */
			cRaster ();
			
			/**
			 * Overloaded constructor
			 * 
			 * @param Directory Description
			 * @param FileName Description
			 * @param JustHeader Description
			 *
			 * @return A cRaster.
			 */
			cRaster(string Directory, 
				string FileName,
				FileType filetype = UNKNOWNFILE,
				GeoType Proj= NDEF,
				string Proj4String = "",
				int CentMer = -1,
				bool Hem = true,
				float Min = -MAXFLOAT,
				float Max = MAXFLOAT);
			
			/**
			 * Descructor
			 */
			virtual ~cRaster ();
			
			/**
			 * Description of WriteFile
			 * 
			 * @param Raster Description
			 * @param Directory Description
			 * @param FileName Description
			 * @param NW Description
			 * @param SE Description
			 * @param Rows Description
			 * @param Cols Description
			 * @param Proj Description
			 *
			 * @return An integer.
			 */
			bool WriteFile(Float2DArray &Raster,
				cGeoP NW,
				cGeoP SE, 
				unsigned Rows,
				unsigned Cols,
				double NSRes,
				double EWres,
				string Directory, 
				string FileName,
				OutPut filetype = BIN,
				GeoType OriginalProj = WGS84GC,
				GeoType ResultProj = DEG,
				int OriginalCentMer = -1,
				int ResultCentMer = -1,
				string Proj4String = "");
			
			/**
			 * Description of ReadFile
			 * 
			 * @param ReadFile Description
			 * @param FileName Description
			 * @param JustHeader Description
			 *
			 * @return An integer.
			 */
			bool ReadFile(string Directory, 
				string FileName,
				FileType filetype = UNKNOWNFILE,
				GeoType Proj= NDEF,
				string Proj4String = "",
				int CentMer = -1,
				bool Hem = true,
				float Min = -MAXFLOAT,
				float Max = MAXFLOAT);
			
			/**
			 * Description of GetHeader
			 * 
			 * @param NW Description
			 * @param SE Description
			 * @param Proj Description
			 * @param Rows Description
			 * @param Cols Description
			 *
			 * @return An integer.
			 */
			int GetHeader(cGeoP &NW, 
									cGeoP &SE, 
									int &CentMer,
									uint &Rows, 
									uint &Cols,
									double &NSres,
									double &EWres,
									GeoType &Proj,
									string &Proj4,
									FileType &Type);
			
			/**
			 * Description of GetValue
			 * 
			 * @param Point Description
			 * @param Interpolation Description
			 *
			 * @return A float.
			 */
			float GetValue(cGeoP &Point, 
					int Interpolation = 0);
			
			/**
			 * Description of GetRes
			 * 
			 * @param NSres Description
			 * @param EWres Description
			 */
			double GetRes ();
	
			
			void GetCorners (cGeoP &NW, cGeoP &SE)
			{
				NW = mNW;
				SE = mSE;
			};
			
			
			/**
			 * Description of IsIn
			 * 
			 * @param point Description
			 *
			 * @return A boolean.
			 */
			bool IsIn (cGeoP point);
			
			bool mUsed;			///< Description

			bool GetRaster(Float2DArray &Raster, uint &Rows, uint &Cols);
			void Display();
			void GetMap(Float2DArray &Raster, unsigned &rows, unsigned &cols);
			int GetSize();
			string		mFilename;	///< Description
		private:

			Float2DArray	mRaster;	///< Description
			GeoType		mProjType;	///< Description
			int 		mCentMer;
			cGeoP		mNW;		///< Description
			cGeoP		mSE;		///< Description
			unsigned	mRows;		///< Description
			unsigned	mCols;		///< Description
			double		mNSres;		///< Description
			double		mEWres;		///< Description
			string		mDirectory;	///< Description
			
			float		mMin;		///< Description
			float		mMax;		///< Description
			projPJ		mProj4;
			bool 		mSouth;
			bool ReturnProj4(GeoType PointType,
									int PointCM,
									bool South, 
									projPJ &Proj);
			FileType mFileType;
			double 		mMapLat;
			double		mMapLon;
			int		mMapCM;
			GeoType		mMapType;
	};
}
#endif
