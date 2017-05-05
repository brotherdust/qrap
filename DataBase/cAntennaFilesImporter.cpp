 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cAntennaFilesImporter.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Roeland van Nieuwkerk (roeland.frans@gmail.com)
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : This class creates a Qt4 dialog that will allow the user 
 *                  to import specific antenna files.
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

// include local headers
#include "cAntennaFilesImporter.h"
#include "LogDialog.h"

using namespace std;
using namespace Qrap;

//********************************************************************************
// The contructor
cAntennaFilesImporter::cAntennaFilesImporter (QWidget* parent) : QDialog(parent)
{
	
	setWindowModality(Qt::WindowModal);
	resize(435, 300);
	setWindowIcon(QIcon(QString::fromUtf8(":/images/logo_icon.png")));
	setWindowTitle(QApplication::translate("cAntennaFilesImporter", "QRAP Import Antenna Files", 0, QApplication::UnicodeUTF8));
	
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setGeometry(QRect(255, 260, 171, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
	
	filesList = new QListWidget(this);
	filesList->setObjectName(QString::fromUtf8("fileList"));
	filesList->setGeometry(QRect(10, 30, 201, 200));
	filesList->setSelectionMode(QAbstractItemView::MultiSelection);
	
	browseButton = new QPushButton(tr("Browse..."),this);
	browseButton->setObjectName(QString::fromUtf8("browseButton"));
	browseButton->setGeometry(QRect(10,235,70,28));
	
	fileLabel = new QLabel(tr("Selected Files:"),this);
	fileLabel->setObjectName(QString::fromUtf8("fileLabel"));
	fileLabel->setGeometry(QRect(10,10,80,20));
	
	technologyGroup = new QGroupBox(this);
	technologyGroup->setObjectName(QString::fromUtf8("technologyGroup"));
	technologyGroup->setGeometry(QRect(220, 30, 201, 61));
	technologyGroup->setTitle(QApplication::translate("cAntennaFilesImporter", "Antenna Technology", 0, QApplication::UnicodeUTF8));
	
	technologyLabel = new QLabel(technologyGroup);
	technologyLabel->setObjectName(QString::fromUtf8("technologyLabel"));
	technologyLabel->setGeometry(QRect(10, 25, 71, 18));
	
	technologyCombo = new QComboBox(technologyGroup);
	technologyCombo->setObjectName(QString::fromUtf8("technologyCombo"));
	technologyCombo->setGeometry(QRect(90, 20, 101, 26));
	technologyLabel->setText(QApplication::translate("cAntennaFilesImporter", "Technology", 0, QApplication::UnicodeUTF8));
	
	// Get the techology types that are available
	StringIntArray vals;
	StringIntArrayIterator iterator;
	
	gDb.GetFieldUiParams("radioinstallation","techkey",vals);
	
	technologyCombo->insertItem(0,"0:NULL");
	int j=1;
	
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		technologyCombo->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	
	progress = new QProgressBar(this);
	progress->setGeometry(QRect(220,105,201,30));
	progress->setVisible(false);
	
	connect(browseButton,SIGNAL(clicked()),this,SLOT(SelectFiles()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(ImportAntennas()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}


//**************************************************************************************
void cAntennaFilesImporter::SelectFiles ()
{
	// Create the file dialog and retrieve a list of file names
	QFileDialog fileDialog;
	
	// Setup the file dialog
	fileDialog.setFileMode(QFileDialog::ExistingFiles);
	fileDialog.setViewMode(QFileDialog::Detail);
	fileDialog.setModal(true);
	fileDialog.setWindowTitle("Open Antenna File");
	
	if(fileDialog.exec())
	{
		fileNames = fileDialog.selectedFiles();
	
		// Set the filesList to display the files.
		filesList->clear();
		filesList->insertItems(0,fileNames);
	} // if fileDialog.exec();
}

//***********************************************************************************
// Import a list of antennas
void cAntennaFilesImporter::ImportAntennas ()
{
	if(fileNames.size()>0)
	{
		QString logMessage("Import log:\n\n");
		
		progress->setVisible(true);
		progress->reset();
		progress->setMinimum(0);
		progress->setMaximum(fileNames.count()-1);
		
		// Extract the technology primary key from the technology combo box
		QStringList parts = technologyCombo->currentText().split(":");
		string techKey;
		
		if(parts.size()>0)
		{
			if(parts[1]=="NULL")
				techKey = "NULL";
			else
				techKey = parts[0].toStdString();
		}
		else
			techKey = "NULL";
		
		
		// Import the files
		for( int i=0 ; i<fileNames.count() ; i++)
		{
			progress->setValue(i);
			
			// Open the first file
			QFile file(fileNames[i]);
			if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				logMessage += fileNames[i] + "......Error opening file\n";
			}
			else
			{
				// Open a text stream
				QTextStream inFile(&file);
				bool verticalAngles = false;
				bool horizontalAngles = false;
				
				// Get the defaults for the table
				StringMap antennaDevice, antenna, antennaPattern;
				
				string aziAngles, aziValues;
				string elevAngles, elevValues;
				int numAziPoints = 0;
				int numElevPoints = 0;
				string deviceName;
				
				// Add the file name to the antenna pattern table
				QString fileName = fileNames[i];
				fileName.remove(0,fileName.lastIndexOf("/")+1);
				antennaPattern["patternfile"] = "'" + fileName.toStdString() + "'";
				antennaPattern["techkey"] = techKey;
				
				// Parse the file
				while(!inFile.atEnd())
				{
					QString line = inFile.readLine();
					
					// Parse the single string
					if(line=="NAME")
					{
						logMessage += fileNames[i] + "......Error, file in wrong format\n";
						break;
					} // if
					
					QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
					
//					for (unsigned i=0; i<list.count(); i++)
//						cout << list[i].toStdString() << endl;
					
					if( (list.count()!=0)&&(list.count()>=2) )
					{
						if(list[0]=="HORIZONTAL")
						{
							horizontalAngles = true;
							verticalAngles = false;
							aziAngles = "'{";
							aziValues = "'{";
						} // if HORIZONTAL
						else
						{
							if(list[0]=="VERTICAL")
							{
								verticalAngles = true;
								horizontalAngles = false;
								elevAngles = "'{";
								elevValues = "'{";
							} // if VERTICAL
							else
							{
								if(list[0].contains(QRegExp("[A-Z]")) && (verticalAngles||horizontalAngles))
								{
									verticalAngles = false;
									verticalAngles = false;
									
									// Complete the azimuth and elevation angles and values arrays
									aziAngles = aziAngles.substr(0,aziAngles.length()-1);
									aziAngles += "}'";
									aziValues = aziValues.substr(0,aziValues.length()-1);
									aziValues += "}'";
									elevAngles = elevAngles.substr(0,elevAngles.length()-1);
									elevAngles += "}'";
									elevValues = elevValues.substr(0,elevValues.length()-1);
									elevValues += "}'";
				
									antennaPattern["azimuthangles"] = aziAngles;
									antennaPattern["azimuthpattern"] = aziValues;
									antennaPattern["elevationangles"] = elevAngles;
									antennaPattern["elevationpattern"] = elevValues;
									antennaPattern["numazipoints"] = QString::number(numAziPoints).toStdString();
									antennaPattern["numelevpoints"] = QString::number(numElevPoints).toStdString();
								} // if
							} // else VERTICAL
						} // else HORIZONTAL
						
						
						if(!verticalAngles && !horizontalAngles)
						{
							if(list[0]=="NAME")
							{
								cout << "NAME " << list[1].toStdString() << endl;
								
								//line.replace(list[0],"");
								antennaDevice["devicename"] = "'"+list[1].toStdString() + "'";
								deviceName = list[1].toStdString();
							} // if NAME
							else
							{
								if(list[0]=="MAKE")
								{
									cout << "MAKE " << list[1].toStdString() << endl;
									
									line.replace(list[0],"");
									antennaDevice["manufacturer"] = "'"+line.toStdString()+"'";
								} // if MAKE
								else
								{
									if(list[0]=="H_WIDTH")
									{
										cout << "H_WIDTH " << list[1].toStdString() << endl;
										
										antennaPattern["azibeamwidth"] = list[1].toStdString();
									} // if H_WIDTH
									else
									{
										if(list[0]=="GAIN")
										{
										cout << "GAIN " << list[1].toStdString() << endl;
											
										antennaPattern["gain"] = list[1].toStdString();
										} // if GAIN
										else
										{
										if(list[0]=="TILT")
										{
										cout << "TILT " << list[1].toStdString() << endl;
										if(list[1] == "ELECTRICAL")
										{
										antennaDevice["electilt"] = "TRUE";
										} // if ELECTRICAL
										else
										{
										antennaDevice["electilt"] = "FALSE";
										} // else ELECTRICAL
										} // if TILT
										else
										{
										if(list[0]=="COMMENTS"||list[0]=="COMMENT")
										{
										cout << "COMMENTS " << line.toStdString() << endl;
										line.replace(list[0],"");
										antennaDevice["description"] = "'" + line.toStdString() + "'";
										antennaPattern["description"] = "'" + line.toStdString() + "'";
										} // if COMMENTS
										else
										{
										if(list[0]=="FREQUENCY")
										{
										cout << "FREQUENY " << list[1].toStdString() << endl;
										antennaPattern["frequency"] = list[1].toStdString();
										} // if FREQUENCY
										else
										{
										if(list[0]=="FRONT_TO_BACK")
										{
										cout << "FRONT_TO_BACK " << list[1].toStdString() << endl;
										antennaPattern["fronttoback"] = list[1].toStdString();
										} // if FRONT_TO_BACK
										else
										{
										if(list[0]=="POLARIZATION")
										{
										cout << "POLARIZATION " << list[1].toStdString() << endl;
										antennaPattern["polarization"] = "'"+list[1].toStdString()+"'";
										} // if POLARIZATION
										} // else FRONT_TO_BACK
										} // else FREQUENCY
										} // else COMMENTS
										} // else TILT
										} // else GAIN
									} // else H_WIDTH
								} // else MAKE
							} // else NAME
						}
						else
						{
							if(horizontalAngles && list[0]!="HORIZONTAL")
							{
								aziAngles += list[0].toStdString() + ",";
								aziValues += list[1].toStdString() + ",";
								numAziPoints++;
							}
							else
							{
								if(verticalAngles && list[0]!="VERTICAL" )
								{
									elevAngles += list[0].toStdString() + ",";
									elevValues += list[1].toStdString() + ",";
									numElevPoints++;
								} // if
							} // else
						} // else
					} // if
				} // while
				
				
				if(verticalAngles||horizontalAngles)
				{
					// Complete the azimuth and elevation angles and values arrays
					aziAngles = aziAngles.substr(0,aziAngles.length()-1);
					aziAngles += "}'";
					aziValues = aziValues.substr(0,aziValues.length()-1);
					aziValues += "}'";
					elevAngles = elevAngles.substr(0,elevAngles.length()-1);
					elevAngles += "}'";
					elevValues = elevValues.substr(0,elevValues.length()-1);
					elevValues += "}'";
					
					antennaPattern["azimuthangles"] = aziAngles;
					antennaPattern["azimuthpattern"] = aziValues;
					antennaPattern["elevationangles"] = elevAngles;
					antennaPattern["elevationpattern"] = elevValues;
					antennaPattern["numazipoints"] = QString::number(numAziPoints).toStdString();
					antennaPattern["numelevpoints"] = QString::number(numElevPoints).toStdString();
				} // if
				
				// Check if the antenna device already exists in the antenna device table
				string whereClause;
				
				whereClause = "devicename = '" + deviceName + "'";
				
				// Perform the select query
				if(!gDb.Select("devicename,id","antennadevice",whereClause) )
				{
					// A QRAP_ERROR,QRAP_INFO,QRAP_WARN or QRAP_FATAL is called by the cDatabase class and handled by Qrap::cMainWindow::Alert()
					logMessage += fileNames[i] + "......Error, Select devicename,id from antennadevice where devicename = '" + QString::fromStdString(deviceName) + "';\n";
				} // if
				else
				{
					// Get the result for the select query
					pqxx::result r;
					
					gDb.GetLastResult(r);
					
					long long id;
					bool insertAntennaDevice;
					
					// Check if the antenna device already exists/
					if( (int)r.size()!=0)
					{
						cout << endl << "Got " << (int)r.size() << " results" << endl;
						cout << "Result " << string(r[0]["devicename"].c_str()) << endl << endl;
/*						if(string(r[0]["devicename"].c_str()) != deviceName)
						{
							insertAntennaDevice = true;
						} // if antenna device exists
						else
*/						{
							antennaPattern["antdevicekey"] = string(r[0]["id"].c_str());
							antennaPattern["techkey"] = techKey;
							insertAntennaDevice = false;
						} // if antenna device existss
					}
					else
					{
						insertAntennaDevice = true;
					}
					
					
					if(insertAntennaDevice)
					{
						// Insert the imported data into the database
						if( (id=gDb.Insert(antennaDevice,"antennadevice"))==-1 )
						{
							// A QRAP_ERROR,QRAP_INFO,QRAP_WARN or QRAP_FATAL is called by the cDatabase class and handled by Qrap::cMainWindow::Alert()
								
							logMessage += fileNames[i] + "......Error, Insert into antennadevice;\n";
						} // if insert antennadevice
						else
						{
							antennaPattern["antdevicekey"] = QString::number(id).toStdString();
							antennaPattern["techkey"] = techKey;
						} // if insert antennadevice
					} // if insertAntennaDevice
					
					// Insert the antenna pattern associated with the antenna device
					if(gDb.Insert(antennaPattern, "antennapattern")==-1)
					{
						// A QRAP_ERROR,QRAP_INFO,QRAP_WARN or QRAP_FATAL is called by the cDatabase class and handled by Qrap::cMainWindow::Alert()
						logMessage += fileNames[i] + "......Error, Insert into antennapattern;\n";
					} // if insert antennapattern
					else
					{
						logMessage += fileNames[i] + "......Imported successfully\n";
					} // else insert antennapattern
				} // else select
			} // else File open
		} // for
		
		LogDialog* log = new LogDialog(0,logMessage);
		log->show();
		
		progress->setVisible(false);
		
		accept();
	} // if fileNames.size()
}
