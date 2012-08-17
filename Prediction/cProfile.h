 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cProfile.h
 *    Copyright   : (c) Meraka Institute CSIR (1997) & University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Helper class for cPathloss. Contains values along a path
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

#ifndef Qrap_cProfile_h
#define Qrap_cProfile_h

// Include local headers
#include "cGeoP.h"

using namespace std;

namespace Qrap
{
	/**
	 * This class encapsulates the data and functionality of a profile of heights that is used for radio propagation prediction.
	 */
	class cProfile
	{
		
		public:
			/**
			 * Constructor
			 */
			cProfile ();
			
			/**
			 * Overloaded constructor
			 * 
			 * @param right Description
			 */
			cProfile (const cProfile &right);
			
			/**
			 * Overloaded constructor
			 * 
			 * @param size Description
			 * @param interPixDist Description
			 */
			cProfile (const int size,
				  const double interPixDist = 90.0);
			
			/**
			 * Overloaded constructor
			 * 
			 * @param size Description
			 * @param profile Description
			 * @param interPixDist Description
			 */
			cProfile (const int size, float *&profile, double interPixDist = 90.0);
			
			/**
			 * Destructor
			 */
			virtual ~cProfile();
			
			/**
			 * The function gets the size of the profile.
			 * 
			 * @return The size of the profile.
			 */
			int GetSize() const { return m_size; }
			
			/**
			 * Description of ReduceSize
			 */
			void ReduceSize() { if (m_size>1) m_size--; }
			
			/**
			 * Gets the inter pixel distance.
			 * 
			 * @return The inter pixel distance.
			 */
			double GetInterPixelDist() const { return m_interPixelDist; }
			
			/**
			 * Description of SetInterPixelDist
			 * 
			 * @param interPixelDist Description
			 */
			void SetInterPixelDist(double interPixelDist);
			
			/**
			 * Pust the array size in "size" and the profile in "profile".
			 * 
			 * @param size Description
			 * @param profile Description
			 */
			void SetProfile(int size, float *&profile);
			
			/**
			 * Description of GetProfile
			 * 
			 * @param size Description
			 * @param profile Description
			 */
			void GetProfile(int &size, float *&profile) const;
			

			/**
			 * Description of GetLastValue
			 */
			double GetLastValue() { return (double)m_profile[m_size-1]; };


			/**
			 * Description MoveOneUp
			 */
			void MoveOneUp();
			
			/**
			 * Description of GetStartLoc
			 * 
			 * @return A cGeoP.
			 */
			cGeoP GetStartLoc() const {return m_startLoc;}
			
			/**
			 * Description of GetEndLoc
			 * 
			 * @return A cGeoP.
			 */
			cGeoP GetEndLoc() const {return m_endLoc;}
			
			/**
			 * Description of SetStartLoc
			 * 
			 * @param point Description
			 */
			inline void SetStartLoc(const cGeoP &point) {m_startLoc=point;}
			
			/**
			 * Description of SetEndLoc
			 * 
			 * @param point Description
			 */
			inline void SetEndLoc(const cGeoP &point) {m_endLoc=point;}
			
			/**
			 * Dump array contents to screen MMD 10/5
			 */
			void Display();
			
			
			/**
			 * Overloaded operator
			 */
			const cProfile & operator=(const cProfile &right);
		
		protected:
			int m_size;			///< Description
			cGeoP m_startLoc;		///< Description
			cGeoP m_endLoc;			///< Description
			float *m_profile;		///< Description
			double m_interPixelDist;	///< Description
	
	};
}
#endif
