 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/08/12
 *    License     : GNU GPLv3
 *    File        : cClutter.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Contains data on different clutter / land cover types
 *		    such as width, height and the coefficients used for 
 *	            propagation prediction.
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

#ifndef Qrap_cClutter_h
#define Qrap_cClutter_h

#include <pqxx/pqxx>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../DataBase/Config.h"
#include "../DataBase/cDatabase.h"

//The following terms in used in the clutter dependant propagation Prediction
#define NUMTERMS 9
 
#define TERM0	1		//Constant offset
#define TERM1 log10(mLinkLength) // This term occur in virtually all models. Here it will refer loss over and above free-space. 
				// The coefficient may only be more than zero.
#define TERM2 log10(m_htx)*log10(mLinkLength)	// This term occurs amongst others in the Okumura Hata model
#define TERM3 log10(m_freq)	// This term also occur often e.g. to account for the difference between plane-earth and free-space loss
#define TERM4 m_freq
#define TERM5 log10(m_freq)*log10(m_freq)		// This term occurs in ECC-33 and Ericsson models

#define TERM6 pow(m_freq,0.284)		// Weissberger-like term for vegetation loss
						// ... depth through trees are assumed shallow and more or less constant
#define TERM7 log10(m_htx)	// we assume a constant mobile/customer antenna height, hence this term is assumed to be incorporated 
				// in the offset.
#define TERM8 log10(m_htx-Cheight)		// Bertoni Walfish 



using namespace std;

namespace Qrap
{

	struct sClutter 
	{
		unsigned	sLandCoverID;
		double  	sWidth;
		double 		sHeight;
		double*		sCoefficients;
		bool*		sAllowCchange;		
	};


	/**
	 * This class encapsulates the data and functionality of the clutter information as used in cPathLossPredictor.
	 */
	class cClutter
	{
		
		public:
			/**
			 * Constructor
			 */
			cClutter ();

			/**
			 * Overload Constructor with the Clutter Classification Group as input 
			 */
			cClutter (unsigned ClassGroup);
 
			/**
			 * Destructor
			 */
			virtual ~cClutter();

			/**
			 * Overload Constructor with the Clutter Classification Group as input 
			 */
			bool Reset (unsigned ClassGroup=9999);
					
			/**
			 * Display the content of the variables
			 */
			void Display();
			
			/**
			 * Overloaded operator
			 */
			const cClutter & operator=(const cClutter &right);

			/**
			 * In this function the coeeficients of the specific ClutterType is updated.
			 */
			bool UpdateCoefficients(unsigned ClutterType);

		private:
			/**
			 * Query the database for the stored data
			 */
			bool GetFromDatabase(unsigned ClassGroup=9999);

			/**
			 * Destroy content: called by destructor and reset
			 */
			bool Destroy();
			
		
		public:
			unsigned mNumber;			///< the number of Clutter Types in the set. 
			sClutter * mClutterTypes;
			unsigned mClassificationGroup;
	};
}
#endif
