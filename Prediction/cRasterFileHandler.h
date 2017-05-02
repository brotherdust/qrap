 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRasterFileHandler.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Handler of one or many raster files ... gets the info out
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

#ifndef Qrap_cRasterFileHandler_h
#define Qrap_cRasterFileHandler_h

// Include local headers
#include "cRaster.h"
#include "cGeoP.h"
#include "../DataBase/Config.h"
#include "cProfile.h"
#include "cMemmngr.h"


namespace Qrap
{
	/**
	 * Description of VecRaster.
	 */
	typedef cRaster * pcRaster;
	typedef	vector<pcRaster> VecRaster;
	
	/**
	 * Description of class cRasterFileHandler
	 */
	class cRasterFileHandler
	{
		public:
			/**
			 * Constructor
			 */
			cRasterFileHandler();
			
			/**
			 * Overloaded constructor
			 * 
			 * @param Source Description
			 */
			cRasterFileHandler(short int Source);
			
			/**
			 * Destructor
			 */
			virtual ~cRasterFileHandler();
			
			/**
			 * Overloaded SetRasterFileRules
			 * 
			 * @param Source Description
			 *
			 * @return A boolean
			 */
			bool SetRasterFileRules(short int Source);
	
			void SetSampleMethod(int SampleMethod)
			{
				mSampleMethod=SampleMethod;
			}
			
			void DirectChangeSetToUse(int OriginalSet);

			unsigned GetClutterClassGroup();
			
			/**
			 * Description of GetForLink
			 * 
			 * @param TxLoc Description
			 * @param RxLoc Description
			 * @param DistRes Description
			 *
			 * @return cProfile Description
			 */
			bool GetForLink(	cGeoP TxLoc,
			    			cGeoP RxLoc,
			    			double DistRes,
						cProfile &OutProfile);
			
			/**
			 * Description of GetForCoverage
			 * 
			 * @param Fixed Description
			 * @param SitePos Description
			 * @param Range Description
			 * @param DistRes Description
			 * @param AngRes Description
			 * @param NumAngles Description
			 * @param NumDistance Description
			 * @param Data Description
			 *
			 * @return A boolean.
			 */
			bool GetForCoverage(bool Fixed,	
							cGeoP SitePos,
					    		double &Range, 
					    		double &DistRes, 
					    		double &AngRes,
					    		unsigned &NumAngles, 
					    		unsigned &NumDistance,
					    		Float2DArray &Data);

			/**
			 * Description of GetForDEM
			 * 
			 * @param 
			 * 
			 * @return A boolean.
			 */
			bool GetForDEM(	cGeoP &NW, 
							cGeoP &SE, 
							double &DistRes,
							unsigned &Rows, 
							unsigned &Cols, 
							Float2DArray &Data, 
							GeoType ProjIn=DEG);
						
		private:
			/**
			 * Description of AddRaster
			 * 
			 * @param point Description
			 *
			 * @return A boolean.
			 */
			bool AddRaster(cGeoP point, string LoadedRastersList="'niks'");
			
			VecRaster 	mCurrentRasters;	///< Description
			IntArray 	mFileSetOrder;		///< Vector of ints containing file-set preferences
			int 		mSampleMethod;
			cGeoP		mPreferedSetNW;
			cGeoP		mPreferedSetSE;
	};
}
#endif
