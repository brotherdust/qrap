 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cProfile.cpp
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

// Include global headers
#include <string.h>

// Include local headers
#include "cProfile.h"


using namespace Qrap;


//*************************************************************************
// Default constructor
cProfile::cProfile()
{
	m_size = 1;
	m_interPixelDist = 200;
	m_profile = new float[m_size];
	m_profile[0] = 0.0;

}/*end CProfile Default Constructor */


//*************************************************************************
// Copy constructor
cProfile::cProfile(const cProfile &right)
{
	int i=0;
  m_size = right.m_size;
  m_startLoc = right.m_startLoc;
  m_endLoc = right.m_endLoc;
  m_profile = new float[m_size];
  for(i=0; i<m_size; i++)
  	m_profile[i] = right.m_profile[i];
  m_interPixelDist = right.m_interPixelDist;
}/* end CProfile Copy Constructor */


//*************************************************************************
// construct profile with only size and interpixel distance provided
cProfile::cProfile(const int size, const double interPixDist)
{
	int i;
	m_size = size;
	m_interPixelDist = interPixDist;
	m_profile = new float[m_size];
	for (i=0; i<m_size; i++)
		*(m_profile+i) = 0.0;

}/* end CProfile Constructor */


//*************************************************************************
// construct profile with size and profile and interpixel distance provided
cProfile::cProfile(int size, float *&profile, double interPixDist)
{
	int i;
  m_size = size;
  m_interPixelDist = interPixDist;
  m_profile = new float[m_size];
  for (i=0; i<m_size; i++)
  {
  		if (profile[i] <-7777.0) m_profile[i]=0.0;
      else m_profile[i] = profile[i];
  }
}/* CProfile Constructor */

//*************************************************************************
//## Destructor (implementation)
cProfile::~cProfile()
{
	delete [] m_profile;
}


//*************************************************************************
//## Assignment Operation (implementation)
const cProfile & cProfile::operator=(const cProfile &right)
{

	int i;
	if (this == &right)
		return (*this);

	delete [] m_profile;

	m_size = right.m_size;
	m_startLoc = right.m_startLoc;
	m_endLoc = right.m_endLoc;

	m_profile = new float[m_size];

   for (i=0; i<m_size; i++)
		m_profile[i] = right.m_profile[i];

	m_interPixelDist = right.m_interPixelDist;

return (*this);
}/* end CProfile Assignment */


//*************************************************************************
void cProfile::SetInterPixelDist(double interPixelDist)
{
	m_interPixelDist = interPixelDist;
}

//**************************************************************************
void cProfile::SetProfile(int size, float *&profile)
{
	int i;
	if (m_profile == profile)
		return;
	m_size = size;
	delete [] m_profile;
	m_profile = new float[m_size];
   for (i=0; i<m_size; i++)
   {
   	if (profile[i] < -7777.0) m_profile[i] = 0.0;
		else m_profile[i] = profile[i];
   }

}/*end CProfile::SetProfile */


//*************************************************************************
void cProfile::GetProfile(int &size, float *&profile) const
{
	int i;
	if (m_profile == profile)
		return;
	size = m_size;
	delete [] profile;
	profile = new float[size];
	for (i=0; i<m_size; i++)
		profile[i] = m_profile[i];

}/* end CProfile::GetProfile */

//**********************************************************************
void cProfile::MoveOneUp()
{
	int i;
	for (i=0; i<m_size-1; i++)
		m_profile[i] = m_profile[i+1];

}/* end CProfile::MoveOneUp */


//*************************************************************************
void cProfile::Display()
{
   #ifndef NO_DEBUG
   int i;

   cout << "Profile: ";
   for (i=0; i<m_size; i++)
   {
	cout << m_profile[i] << " ";
   }
   cout << endl;
   #endif
   return;
}
