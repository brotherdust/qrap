 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cSettings.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Thane Thomson (roeland.frans@gmail.com)
 *		  : (Magdaleen Ballot (magdaleen.ballot@up.ac.za))
 *    Description : Settings and Structure store encapsulation, implementation
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
//\TODO We need to get rid of Poco for the xml parsing and rather use Qt

#ifndef Qrap_cSettings_h
#define Qrap_cSettings_h

#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/AutoPtr.h>
#include <Poco/DOM/AttrMap.h>
#include <Poco/DOM/Attr.h>
#include <Poco/DOM/ElementsByTagNameList.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/Exception.h>

namespace Qrap
{
	/**
	 * The global settings store class for a QRAP client or server.
	 *
	 * All global machine-specific settings are loaded into this class from an external
	 * settings file located in a predefined location. This should be the first class
	 * in the QRAP system to be initialised, as it specifies the name of the debug log file,
	 * should debugging be necessary.
	 *
	 * \todo Decide on a location, such as /etc/gisrap/, for the storage of the settings file,
	 * as well as a name for the settings file, such as gisrap.conf.
	 *
	 * \todo Decide on a generic name for the database to which to connect, e.g. "gisrap".
	 *
	 */
	
	class cSettings: public cBase
	{
	public:
		/**
		 * Constructor.
		 *
		 */
		cSettings ();
		
		/**
		 * Destructor.
		 */
		~cSettings ();
		
		/**
		 * Attempts to load the configuration settings from the given XML file.
		 */
		bool LoadFromFile (const std::string& fileName);
		
		/**
		 * Retrieves the specified setting's value (or empty string if the setting cannot be
		 * found).
		 */
		std::string GetValue (const std::string& setting) const;
		
		/**
		 * Copies the database structure into the given structure map.
		 * 
		 * @param output The map of string maps into which the database structure will be copied.
		 */
		void GetDbStructure (DbStructure& output) const
			{ output = mStructure; };
	
	private:
		Poco::XML::DOMParser                    mSettingsParser;
		Poco::XML::AutoPtr<Poco::XML::Document> mSettingsDoc;
		Poco::XML::Element*                     mSettingsRoot;         ///< The root settings element.
		DbStructure                             mStructure;            ///< The database's internal structure.
		
		// NOTE: msLogFileName will be inherited
	};
	
	/**
	 * Global settings store.
	 */
	extern cSettings gSettings;

}

#endif
