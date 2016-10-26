 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGeoP.cpp
 *    Copyright   : (c) University of Pretoria & Meraka Institute
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents basic point location
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

#ifndef Qrap_cGeoP_h
#define Qrap_cGeoP_h

// Include global headers
//#include <projects.h>

// Include local headers
#include <math.h>
#include <proj_api.h>
#include <iostream>
#include <string>
#include <values.h>

#define PI 3.1415926535897932384626433832795
#define LIGSNEL 2.997924562e8

namespace Qrap
{
	const double cDegResT=0.000833/90.0;
	/**
	 * Description of GeoType.
	 */
 	enum GeoType 
 	{
 		DEG,		// decimal degrees in WGS84
 		WGS84GC,	// Gaussian Conform using the WGS84 elipsoids
 		UTM,		// Universal Transverse Mecator using WGS84, 
 				// CentMer actually indicating the Zone.
 		NDEF		// not explicity set
 	};

	
	class cGeoP
	{
		public:
			/**
			 * Default Constructor
			 */
			cGeoP();
			
			/**
			 * Copy Constructor
			 * 
			 * @param right Description
			 */
			cGeoP(const cGeoP &right);
			
			/**
			 * Constructor overload
			 * 
			 * @param lat lati
			 * @param lon Description
			 * @param type Description
			 * @param centMer Description
			 * @param Hemishere Description
			 *
			 * @return A cGeoP.
			 */
			cGeoP(double lat, double lon, GeoType type=DEG, int centMer=-1, bool South=true);
			
			/**
			 * Destructor
			 */
			virtual ~cGeoP();
			
			/**
			 * Description Get
			 * 
			 * @param lat Description
			 * @param lon Description
			 *
			 * @return A boolean.
			 */
			bool Get(double &lat, double &lon);
			
			/**
			 * Description of GetGeoType
			 * 
			 * @return A GeoType.
			 */
			GeoType GetGeoType() {return mType;}; 
			
			/**
			 * Description of SetGeoType
			 * 
			 * @param type  - the GeoType to convert the point to
			 * @param central - the central meridian of 
			 *
			 * @return A boolean.
			 */
			bool SetGeoType(GeoType type = DEG, int central=-1);
			
			/**
			 * Returns 'true' if object lies in block decribed by NW and SE 
			 * 
			 * @return A boolean
			 */
			bool Between(cGeoP NW,cGeoP SE);
			
			/**
			 * Get the member values of the object 
			 * 
			 * @param lat Description
			 * @param lon Description
			 * @param type Description
			 * @param centMer Description
			 * @param Sphere Description
			 *
			 * @return A boolean
			 */
			bool Get(double &lat, double &lon, GeoType &type, int &centMer, bool &Sphere);
			
			/**
			 * Description of Set
			 * 
			 * @param lat Description
			 * @param lon Description
			 * @param type Description
			 * @param centMer Description
			 * @param Sphere Description
			 */
			void Set(double lat, double lon, GeoType type=DEG, int centMer=-1, bool South=true);
			
			/**
			 * Description of Display
			 */
			void Display ();
			
			/**
			 * Description of Distance
			 * 
			 * @param right Description
			 *
			 * @return A double
			 */
			double Distance (const cGeoP right);
			
			/**
			 * Description of Bearing
			 * 
			 * @param right
			 *
			 * @return A double
			 */
			double Bearing (const cGeoP right);
			
			/**
			 * Description of FromHere
			 * 
			 * @param here Description
			 * @param distance Description
			 * @param direction Description
			 */
			void FromHere (cGeoP here, double distance, double direction);
			
			/**
			 * Description of overloaded operator
			 */
			const cGeoP &operator=(const cGeoP &right);
			
			/**
			 * Description of overloaded operator
			 */
			bool operator==(const cGeoP &right) const;
			
			/**
			 * Description of overloaded operator
			 */
			bool operator!=(const cGeoP &right) const;
			
			/**
			 * Determine default centralmeridian
			 */
			inline int DefaultCentMer(GeoType type);
						
			/**
			 * Determine Hemishere -- true = South
			 */
			inline bool Hemisphere()
			{
				GeoType RememberType=mType;
				int CentMer=mCentMer; 
				if (mType!=DEG) SetGeoType(DEG);
				mSouth = (mLat<=0.0);
				SetGeoType(RememberType,CentMer);
				return mSouth;
			};
			
			
		private:
			
			
			/**
			 * Description of DEGtoWGS84CG
			 * 
			 * @param central Description
			 *
			 * @return A boolean.
			 */
			inline bool DEGtoWGS84GC(int central=-1);
			
			/**
			 * Description of WGS84CGtoDEG
			 * 
			 * @return A boolean.
			 */
			inline bool WGS84GCtoDEG();
			
			/**
			 * Description of UTMtoDEG
			 * 
			 * @return A boolean.
			 */
			inline bool UTMtoDEG();
			
			/**
			 * Description of DEGtoUTM
			 * 
			 * @param central Description
			 * 
			 * @return A boolean.
			 */
			inline bool DEGtoUTM(int zone=-1);
			
			/**
			* Description of UTMtoWGS84GC
			* 
			* @return A boolean.
			*/
			inline bool UTMtoWGS84GC(int central=-1);
			
			/**
			 * Description of WGS84GCtoUTM
			 * 
			 * @return A boolean.
			 */			
			inline bool WGS84GCtoUTM(int zone=-1);
			
	
			/**
			* Description of UTMtoUTM
			* 
			* @return A boolean.
			*/				
			inline bool UTMtoUTM(int zone=-1);
			
		
			/**
			* Description of WGS84GCtoWGS84GC
			* 
			* @return A boolean.
			*/				
			inline bool WGS84GCtoWGS84GC(int central=-1);
			
			/**
			* Description of DefaultCentMer
			* 
			* @return A boolean.
			*/
			
			GeoType mType;		///< Description
			double mLat;		///< Description
			double mLon;		///< Description
			int mCentMer;		///< Description
			bool mSouth;		///< Used to indicate Hemisphere Southern = TRUE, North=FALSE
	};
}
#endif

