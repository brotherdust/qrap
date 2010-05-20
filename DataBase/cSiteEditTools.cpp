
 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSiteEditTools.cpp
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

#include "cSiteEditTools.h"
#include "Types.h"

/*
 * Explicitly define the classes we're using from each namespace.
 */
using namespace std;
using namespace Qrap;


//****************************************************************
QString Qrap::FindLatLon(QString lat,QString lon)
{
	Qrap::DegreeFormat locationFormat;
	string setting = Qrap::gDb.GetSetting("location");
			
	QString format;
	QString latitude;
	QString longitude;
	QString latInput;
	QString longInput;
		
	// if the degrees minutes seconds radio is checked then do the relevant conversion
	if(setting=="DD:MM:SS" )
	{				
		locationFormat = dfDegMinSec;
				
		// Get the latitude value
		latInput = lat;
		if( (latitude = QString::fromStdString(ExtractDecimalDegrees(latInput.toStdString(),dfDegMinSec,true)))=="" )
			return false;
			
		// Get the longitude value
		longInput = lon;
		if( (longitude = QString::fromStdString(ExtractDecimalDegrees(longInput.toStdString(),dfDegMinSec,true)))=="" )
			return "";
				
		// Get the value to send to the table view
		//	tableViewValues.insert(parts[0],QVariant(latInput + " | " + longInput));
	} // if DDMMSS
	else
	{
		// if the degrees minutes decimals radio button is checked then do the relevant conversion
		if(setting=="DD:MM.mm")
		{
			locationFormat = dfDegMin;
			latInput = lat;
			if( (latitude = QString::fromStdString(ExtractDecimalDegrees(latInput.toStdString(),dfDegMin,true)))=="")
				return "";
			longInput = lon;
			if( (longitude = QString::fromStdString(ExtractDecimalDegrees(longInput.toStdString(),dfDegMin,true)))=="")
				return "";
		
			// Get the value to send to the table view
			//	tableViewValues.insert(parts[0],QVariant(latInput + " | " + longInput));
		} // if DDMMmm
		else
		{
			// if the Degrees decimals radio is checked then do the relevant conversion
			if(setting=="DD.dddd")
			{
				locationFormat = dfDeg;
				latitude = lat;
				longitude = lon;
				
				// Check if the format of the degrees decimals is correct
				if(!latitude.contains(QRegExp("^[-+ ]{0,2}[0-9]{1,4}[.]{0,1}[0-9]{0,10}[ ]{0,1}$")) )
				{
//					QMessageBox::information(this, "QRap", "Incorrect latitude format.");
					QRAP_WARN("Incorrect latitude format.");
					return "";
				} // if latitude contains
					
				if(!longitude.contains(QRegExp("^[-+ ]{0,2}[0-9]{1,4}[.]{0,1}[0-9]{0,10}[ ]{0,1}$")) )
				{
//					QMessageBox::information(this, "QRap", "Incorrect longitude format.");					
//					QRAP_ERROR_CODE("Incorrect longitude format.", acInvalidInputFormat);
					QRAP_WARN("Incorrect longitude format.");
					return "";
				} // if longitude contains
				// Get the value to send to the table view
			} // if DDdddd
			else
			{
				format = "";
//				QMessageBox::information(this, "QRap", "No location format specified. Choose either,\n\nDD:MM:SS X\nDD:MM.mm X\n\261DD.dddd");
				QRAP_WARN("No location format specified. Choose either,\n\nDD:MM:SS X\nDD:MM.mm X\n\261DD.dddd");
				return "";
			} // if DDdddd
		} // else DDMMmm
	} // else DDMMSS
	return QString("GeomFromText('POINT(%1 %2)',4326)").arg(longitude).arg(latitude);
}


//**********************************************************************************************
pqxx::result Qrap::FindClosebySites(cGeoP Here, int distance)
{
	string query;
	pqxx::result r;
	char *text= new char[33];

	string SiteSense;
	pqxx::result SiteCloseby;
	cGeoP There;
	There.SetGeoType(DEG,-1);
	Here.SetGeoType(DEG,-1);
	double North,South,East,West;
	Here.Get(South,East);
	query = "SELECT id,sitename,AsText(location) AS ligging,groundheight,status, ";
	query += " ST_Distance(ST_GeomFromText('POINT(";
	gcvt(South,10,text);
	query += text;
	query +=" ";
	gcvt(East,10,text);
	query += text;
	query +=")',4326)::geometry";
	query +=", location::geometry) as distance ";
       	query +=" FROM site WHERE location && SetSRID('BOX3D(";
	double scale = distance*1.42; 
	There.FromHere(Here,scale,315);
	There.SetGeoType(DEG);
	There.Get(North,West);
	There.FromHere(Here,scale,135);
	There.SetGeoType(DEG);
	There.Get(South,East);
	gcvt(West,10,text);
	query += text;
	query +=" ";
	gcvt(North,10,text);
	query += text;
	query +=", ";		
	gcvt(East,10,text);
	query += text;
	query +=" ";	
	gcvt(South,10,text);
	query += text;
	query += ")'::box3d,4326) order by distance asc;";	
	if (!gDb.PerformRawSql(query))
	{
		cout << "Database Select on sites table failed"<< endl;
//		QMessageBox::information(this, "QRap", "Database select on sites table for sites in area failed");
		QRAP_WARN("Database select on sites table for sites in area failed");
	}
	Qrap::gDb.GetLastResult(SiteCloseby);

	return SiteCloseby;
}

//*********************************************************************************************
int Qrap::GetGroundHeight(double lat, double lon)
{

	string query;
	pqxx::result r;
	char *text= new char[33];
	
	QStringList Filesets;
	int GroundHeight = 0;
	QString Result;
	query = "SELECT orderarray FROM filesetsused WHERE type = 'DEM'";
	if (!gDb.PerformRawSql(query))
	{
//		QMessageBox::information(this, "QRap", "Error finding the DEM order array!");
		QRAP_WARN( "Error finding the DEM order array!");
	}
	gDb.GetLastResult(r);
	if (r.size() < 1)
	{
//		QMessageBox::information(this, "QRap", "There are no DEM filesets declared!");
		QRAP_WARN("There are no DEM filesets declared!");
		Result = "";
	}
	else
	{
		Result = QString ("%1").arg(r[0]["orderarray"].c_str());
		Result = Result.mid(1,Result.length()-2);
		Filesets=Result.split(",");
	}
	QStringListIterator i(Filesets);
	GroundHeight = -9999;
	while (i.hasNext() && GroundHeight == -9999)
	{			
		QString FS = i.next();	
		query = "SELECT filename, location,centmer,filetype,projection,fileformat ";
		query += " FROM sourcefiles cross join filesets WHERE filesets.id = ";
		query += FS.toStdString().c_str();
		query += " AND filesets.id = sourcefiles.filesetkey AND POINT(";
		gcvt(lat,10,text);
		query += text;
		query += ", ";
		gcvt(lon,10,text);
		query += text;
		query +=") <@ areasquare";
		if (!gDb.PerformRawSql(query))
		{
//			QMessageBox::information(this, "QRap", "Error finding the height of the site");
			QRAP_WARN("Error finding the height of the site");
		}
		else
		{
			gDb.GetLastResult(r);
			if (r.size() >= 1)
			{
				FileType Type;
				if (r[0]["fileformat"].c_str() == "GDALFILE")
				{
					Type = GDALFILE;
				}
				else 
				{
					if (r[0]["fileformat"].c_str() == "BINFILE")
					{
						Type = BINFILE;
					}
					else
					{
						Type = GDALFILE;
					}
				}
				cRaster CRaster(r[0]["location"].c_str(),r[0]["filename"].c_str(),Type);
				cGeoP Temp(lat,lon,DEG);
				GroundHeight = CRaster.GetValue(Temp);
			}
		}
	}
	if (GroundHeight == -9999)
	{
		GroundHeight = 0;
//		QMessageBox::information(this, "QRap", "There are no height data for this point!");
		QRAP_WARN("There are no height data for this point!");
	}
	delete [] text;
	return GroundHeight;
}

//*************************************************************************
string Qrap::ExtractDecimalDegrees (const string& val, DegreeFormat format, bool directionText, bool latitude)
{

	char   *ret;
	ret = new char[33];
	double deg = GetDecimalDegrees(val, format, directionText, latitude);
	sprintf(ret, "%.6lf", deg);
	string ReturnValue = string(ret);
	delete [] ret;
	return ReturnValue;
}

//*************************************************************************
double Qrap::GetDecimalDegrees (const string& val, DegreeFormat format, bool directionText, bool latitude)
{
	unsigned    i = FindNonWhitespace(val, 0, true), len = val.length();
	string s;
	double deg=0.0, min=0.0, sec=0.0, mult = 1.0;
	
	// get the degrees
	s = ExtractLiteral(val, i);
	deg = atof(s.c_str());
	
	if (latitude)
	{
		if ((deg > 90.0) || (deg < -90.0))
		{
			QRAP_ERROR_CODE("Latitudes must be between -90 and +90 degrees.", acInvalidInputFormat);
			return 0;
		}
	} else
	{
		// if it's a longitude
		if ((deg > 180.0) || (deg < -180.0))
		{
			QRAP_ERROR_CODE("Longitudes must be between -180 and +180 degrees.", acInvalidInputFormat);
			return 0;
		}
	}
	
	i += s.length();
	// skip past any whitespace
	i = FindNonWhitespace(val, i, true);
	
	if (format != dfDeg)
	{
		// get the minutes
		s = ExtractLiteral(val, i);
		min = atof(s.c_str());
		if (min > 60.0)
		{
			QRAP_ERROR_CODE("Minutes have a maximum value of 59.", acInvalidInputFormat);
			return 0;
		}
		i += s.length();
		// skip past any whitespace
		i = FindNonWhitespace(val, i, true);
		
		// get the minutes' decimal value
		s = ExtractLiteral(val, i);
		sec = atof(s.c_str());
		
		if (format == dfDegMin)
		{
			len = s.length();
			for (i=0;i<len;i++)
				mult *= 10.0;
	
			min += sec/mult;
		} else
		if (format == dfDegMinSec)
		{
			if (sec >= 60.0)
			{
				QRAP_ERROR_CODE("Seconds have a maximum value of 59.", acInvalidInputFormat);
				return 0;
			}
			
			min += sec/60.0;
		}
		
		if (directionText)
		{
			// skip ahead to the north/south/east/west specifier
			i += s.length();
			i = FindNonWhitespace(val, i, true);
			if (i >= val.length())
			{
				QRAP_ERROR_CODE("A direction specifier is required for a latitude or longitude co-ordinate", acInvalidInputFormat);
				return 0;
			}
			// extract the value
			s = ExtractLiteral(val, i);
		
			// Uses the convention that North and East = positive,
			// and South and West = negative.
			switch (tolower(s[0]))
			{
			case 'n':
			case 'e':
				break;
			case 'w':
			case 's':
				deg *= -1.0;
				break;
			default:
				QRAP_ERROR_CODE("Invalid direction specifier: "+s, acInvalidInputFormat);
				return 0;
				break;
			}
		}
		
		if (deg > 0.0)
			deg += min/60.0;
		else
			deg -= min/60.0;
	}
	
	// convert the degrees to a string
	return deg;
}


//**************************************************************************
bool Qrap::ExtractLatLongFromPoint (const string& pointStr, double& latitude, double& longitude)
{
	int    i, len = pointStr.length();
	string temp;
//	cout << pointStr << "  in cBase ExtractLatLongFromPoint "<< endl;
	// check that the string contains the "POINT("
	if (pointStr.substr(0, 6) != "POINT(")
		return false;
	
	// find the first co-ordinate
	i = FindNonWhitespace(pointStr, 6);
	if (i >= len)
		return false;
	
	// extract the longitude
	temp = ExtractLiteral(pointStr, i);
	longitude = atof(temp.c_str());
	i += temp.length();
	
	// skip past any whitespace to the next co-ordinate
	i = FindNonWhitespace(pointStr, i);
	if (i >= len)
		return false;
	// extract the latitude
	temp = ExtractLiteral(pointStr, i);
	latitude = atof(temp.c_str());
//	cout << latitude << "  in cBase Lat Long "<< longitude << endl;
	return true;
}

//*************************************************************************
bool Qrap::ExtractLatLongFromPoint (const string& pointStr, DegreeFormat outputFormat,
		string& latitude, string& longitude)
{
	double la, lo, deg, min, sec;
	bool   north, east;
	char *text = new char[33];
	
	// get the values
	if (!ExtractLatLongFromPoint(pointStr, la, lo))
	{
		delete [] text;
		return false;
	}
	
	// if it's just wanted in decimal degrees, don't waste time here
	if (outputFormat == dfDeg)
	{
		gcvt(la,10,text);
		latitude = text;
		gcvt(lo,10,text);
		longitude = text;
		delete [] text;
		return true;
	}
	
	north = (la >= 0.0) ? true : false;
	east = (lo >= 0.0) ? true : false;
	
	la = fabs(la);
	lo = fabs(lo);
	
	switch (outputFormat)
	{
	case dfDegMinSec:
		// compute the latitude
		deg = trunc(la);
		la -= deg;
		min = trunc(la*60.0);
		la -= min/60.0;
		sec = la*3600.0;
		gcvt((int)deg,8,text);
		latitude =text;
		latitude +=":";
		gcvt((int)min,8,text);
		latitude +=text;
		latitude +=":";
		gcvt(sec,10,text);
		latitude +=text;
		
		// now compute the longitude
		deg = trunc(lo);
		lo -= deg;
		
		min = trunc(lo*60.0);
		lo -= min/60.0;
		
		sec = lo*3600.0;
		
		gcvt((int)deg,8,text);
		longitude =text;
		longitude +=":";
		gcvt((int)min,8,text);
		longitude +=text;
		longitude +=":";
		gcvt(sec,10,text);
		longitude +=text;
		break;
	
	case dfDegMin:
		// compute the longitude
		deg = trunc(la);
		la -= deg;
		min = la*60.0;
		gcvt((int)deg,8,text);
		latitude =text;
		latitude +=":";
		gcvt(min,10,text);
		latitude +=text;
		
		// compute the longitude
		deg = trunc(lo);
		lo -= deg;
		min = lo*60.0;
		gcvt((int)deg,8,text);
		longitude =text;
		longitude +=":";
		gcvt(min,10,text);
		longitude +=text;
		break;
	default:
		delete [] text;
		return false;
	}
	
	latitude += (north) ? " N" : " S";
	longitude += (east) ? " E" : " W";
	delete [] text;
	return true;
}

//*************************************************************************
QString Qrap::MakeStringFromDecDegrees ( double decimaldegrees, DegreeFormat outputFormat, bool latitude)
{
	double deg=0.0, min=0.0, sec=0.0;
	bool   northOReast;
	QString Output;	
	char *text = new char[33];
	
	// if it's just wanted in decimal degrees, don't waste time here
	if (outputFormat == dfDeg)
	{
		gcvt(decimaldegrees,10,text);
		Output =text;
		delete [] text;
		return Output;
	}
	
	northOReast = (decimaldegrees >= 0.0) ? true : false;
	decimaldegrees = fabs(decimaldegrees);
	deg = trunc(decimaldegrees);
	decimaldegrees -= deg;

	switch (outputFormat)
	{
	case dfDegMinSec:
		min = trunc(decimaldegrees*60.0);
		decimaldegrees -= min/60.0;
		
		sec = decimaldegrees*3600.0;
		gcvt((int)deg,8,text);
		Output =text;
		Output +=":";
		gcvt((int)min,8,text);
		Output +=text;
		Output +=":";
		gcvt(sec,10,text);
		Output +=text;
		break;
	
	case dfDegMin:
		min = decimaldegrees*60.0;
		gcvt((int)deg,8,text);
		Output =text;
		Output +=":";
		gcvt(min,10,text);
		Output +=text;
		break;
	default:
		Output = "format?";
	}
	
	if (latitude)
		Output += (northOReast) ? " N" : " S";
	else
		Output += (northOReast) ? " E" : " W";
	
	delete [] text;
	return Output;
}




