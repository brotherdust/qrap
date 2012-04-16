 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : IntMatrixCalc.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Defines inline functions used in cPlotask to calculate
 *			the elements of the interference matrix(s)
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


#ifndef Qrap_IntMatrixCalc_h
#define Qrap_IntMatrixCalc_h
using namespace std; 

namespace Qrap
{

	enum eIntMatrixType ///Identifies the type of interference matrix
	{
		Area,
		CSTraffic,
		PSTraffic,
	};

	enum eTechMod ///Identifies the modulation scheme being used
	{
		GSM,
		LTE,	
		GMSK,
		BPSK,
		QPSK,
		QAM16,
		QAM64
	};

	///*******************************************************************************************
	inline double CoChannelEffect(double C_I,eTechMod Modulation)
	{
		double value;
		switch (Modulation)
		{
			case GSM: /// this model the Frame Erarasure Rate of a non-hopping channel
				value = 10000.0*pow(10, -0.0033*C_I*C_I - 0.0391*C_I - 0.2535);
				return value; 
				break;
			case LTE: /// this is an attempt to model the reduction in effiency relative to maximum efficiency
				value = pow(10,(( -0.002252301*C_I*C_I*C_I+0.04417294*C_I*C_I- 0.3633133*C_I + 0.07769664)/10));
				return value; 
				break;
			default:
				value = 10000.0*pow(10, -0.0033*C_I*C_I - 0.0391*C_I - 0.2535);
				return value; 
			
		}
		return 0.0;
	}

	///********************************************************************************************
	inline double AdChannelEffect(double C_I,eTechMod Modulation)
	{
		double value;
		switch (Modulation)
		{
			case GSM: /// this model the Frame Erarasure Rate of a non-hopping channel
				C_I = C_I - 9;
				value = 10000.0*pow(10, -0.0033*C_I*C_I - 0.0391*C_I - 0.2535);
				return value; 
				break;
			case LTE: /// this is an attempt to model the reduction in effiency relative to maximum efficiency
				C_I = C_I - 30;
				value = pow(10,(( -0.002252301*C_I*C_I*C_I+0.04417294*C_I*C_I- 0.3633133*C_I + 0.07769664)/10));
				return value; 
				break;
			default:
				C_I = C_I - 9;
				value = 10000.0*pow(10, -0.0033*C_I*C_I - 0.0391*C_I - 0.2535);
				return value; 
			
		}
		return 0.0;
	}
}
#endif
