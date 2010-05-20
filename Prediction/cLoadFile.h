 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadFile.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : The actual method used to do raster file-loading 
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
 
 
#ifndef Qrap_cLoadFile_h
#define Qrap_cLoadFile_h

#include "../DataBase/Config.h"
#include "cRaster.h"
#include "cRasterFileHandler.h"
#include "cGeoP.h"
#include "cMemmngr.h"
#include <string>
#include <stdio.h>
#include <math.h>

using namespace std; 
namespace Qrap
{
		//
		struct StructMetaData
		{
			double North;
			double South;
			double East;
			double West;
			int CentralMeridian;
			double Resolution;
			GeoType Proj;
			string Proj4;
			string Location;
			string FileName;
			unsigned FileSet;
			FileType Filetype;
		};
		
		typedef vector<StructMetaData> MetaData;
		#define OUTFILETYPE BIN;
		
		/**
	 	* Description of class cLoadFile
	 	*/
		class cLoadFile
		{
		public:
			/**
	 		* Constructor
	 		*/
			cLoadFile();
			/**
	 		* Constructor
	 		* @param SourceFileSet Filesetkey of the source fileset
	 		* @param BinaryFileSet Filesetkey of the binary fileset
	 		* @param BinaryDirectory The directory to which the binary file will be written
	 		*/
			cLoadFile(int SourceFileSet, 
							int BinaryFileSet=0, 
							string BinaryDirectory="");
			/**
	 		* Destructor
	 		*/
			virtual ~cLoadFile();
			
			/**
			 * Description of LoadFile
			 * LoadFile is a function to load a source file's header data
			 * and cut it into smaller maps of less than 2x2 Deg or 4MB in memory
			 * size. The binary maps are transposed to WGS84GC for faster prediction computation
			 * 
			 * 
			 * @param Directory 		Directory of the sourcefile
			 * @param FileName 		File  name of the source file
			 * @param filetype 			GDALFILE, BINFILE, NESFILE, ORTFILE or GRASSFILE
			 * @param Proj4String 	Proj 4 Projection string
			 * @param CentMer		The Central Meridian of the source or the output (WGS84GC)
			 * @param Hem				Hemesphere true = South
			 *
			 * @return A bool.
			 */		
			 bool LoadFile(			string SrcDirectory, 
			 						string FileName,
			 						FileType filetype = UNKNOWNFILE,
									GeoType Proj= NDEF,
									string Proj4String = "",
									int CentMer = -1,
									bool Hem = true);
			 
				StructMetaData CopyMapHed (StructMetaData Old)
				{
					StructMetaData New;
					New.North = Old.North;
					New.South = Old.South;
					New.East = Old.East;
					New.West = Old.West;
					New.CentralMeridian = Old.CentralMeridian;
					New.Resolution = Old.Resolution;
					New.Proj = Old.Proj;
					New.Proj4 = Old.Proj4;
					New.Location = Old.Location;
					New.FileName = Old.FileName;
					New.FileSet = Old.FileSet;
					New.Filetype = Old.Filetype;
					return New;
				}
			 
			 /**
			 * Description of CutCurrentSet
			 * @return A bool.
			 */	
			 bool CutCurrentSet(	unsigned OriginalFileSet, 
			 						unsigned ResultFileSet);
			 
			 /**
			 * Description of WriteDB
			 * This fuction writes the Loaded Files to the 
			 * database. This function should only be called after 
			 * all the files has been loaded
			 * 
			 * @return A bool.
			 */		
			 bool WriteDB();
			 
			 /**
			 * Description of LoadTextList
			 * Reads a list of files from a text file and call LoadFile for each
			 * of the instances. After all the files has been processed, the
			 * WriteDB function is called automatically.
			 * 
			 * @param Directory 		Directory of the sourcefile
			 * @param FileName 		File  name of the list text file
			 * @param filetype 			GDALFILE, BINFILE, NESFILE, ORTFILE or GRASSFILE
			 * @param Proj4String 	Proj 4 Projection string
			 * @param CentMer		The Central Meridian of the source or the output (WGS84GC)
			 * @param Hem				Hemesphere true = South
			 *
			 * @return A bool.
			 */		
			 bool LoadTextList(string Directory, 
			 						string List,
			 						FileType filetype = UNKNOWNFILE,
									GeoType Proj= NDEF,
									string Proj4String = "",
									int CentMer = -1,
									bool Hem = true);
			 
			 bool LoadOrt(	string sourceDirectory, 
					 		string resultDirectory, 
			 				string resultFileName, 
			 				unsigned ResultFileSet, 
			 				int CentMer);
			 
		private:
			MetaData mMetaData;
			int mSourceFileSet;
			int mBinaryFileSet;
			string mBinaryDirectory;
			int mGlobalCounter;
				 
	};
}

#endif /*CLOADFILE_H_*/
