 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSiteEditTools.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Various useful functions used for editing site info
 *
 ***************************************************************************
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

#ifndef CSITEEDITTOOLS_H_
#define CSITEEDITTOOLS_H_

#include "Config.h"
#include "Types.h"
#include "../Prediction/cRaster.h"
#include "../Prediction/cGeoP.h"
#include <stdio.h>
#include <string.h>
#include <QMessageBox>


using std::string;
using namespace pqxx;

namespace Qrap
{
	/**
	 * The cSitesEditTools contains routines useful for Site Editing.
	 * It is used in cPlaceSite and cRapFormBuilders 
	 */

	QString FindLatLon(QString lat,QString lon);
	pqxx::result FindClosebySites(cGeoP point, int distance);
	int GetGroundHeight(double lat, double lon);
	bool InsertDefaultRadioInsts(int SiteId);
	bool DeleteBTL(int SiteId);

	/**
	 * Extracts a value from the given string and converts to a decimal degrees format up to 6
	 * decimal places.
	 * 
	 * This is for the SAPS-specific CSV file importing routine.
	 * 
	 * @param val The string value from which to extract the degree value.
	 * @param format The format in which the given string value is to be read.
	 * @param directionText Whether the direction is given as text, i.e. an appended
	 * "n", "s", "w" or "e". Case-insensitive. If this is false, it is assumed that the
	 * direction is given by the sign of the specified value.
	 * @param latitude If true, it means that the result will be treated as a latitude,
	 * otherwise it will be treated as a longitude.
	 * 
	 * @return A string containing the degree value up to 6 decimal places.
	 */
	std::string ExtractDecimalDegrees (	const std::string& val, DegreeFormat format,
						bool directionText = true, bool latitude = true);

	double GetDecimalDegrees (	const std::string& val, DegreeFormat format, 
					bool directionText = true, bool latitude=true);
	
	/**
	 * Extracts a longitude and latitude from a PostGIS output point string.
	 * The format of a PostGIS point string is "POINT( [longitude]  [latitude] )", e.g.
	 * <pre>POINT(25.3 21.98)</pre>.
	 * 
	 * @param pointStr The PostGIS point string containing the longitude and latitude pair.
	 * @param outputFormat The required output string format.
	 * @param latitude An output string in which the formatted latitude will be stored.
	 * @param longitude An output string in which the formatted longitude will be stored.
	 * 
	 * @return true on success, false on parsing error.
	 */
	bool ExtractLatLongFromPoint (const std::string& pointStr, DegreeFormat outputFormat,
		std::string& latitude, std::string& longitude);
	
	QString MakeStringFromDecDegrees ( double decimaldegrees, DegreeFormat outputFormat, bool latitude);

	/**
	 * Extracts a longitude and latitude from a PostGIS output point string in a similar fashion
	 * to its string-output counterpart, except it returns the output as two double values.
	 * 
	 * @param pointStr The PostGIS point string containing the longitude and latitude pair.
	 * @param latitude The variable in which to store the extracted latitude (as decimal degrees).
	 * @param longitude The variable in which to store the extracted longitude (as decimal degrees).
	 * 
	 * @return true on success, false on parsing error.
	 */
	bool ExtractLatLongFromPoint (const std::string& pointStr, double& latitude, double& longitude);

};

#endif /*CSITEEDITTOOLS_H_*/
