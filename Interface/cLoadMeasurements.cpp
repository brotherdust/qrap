 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2012/04/16
 *    License     : GNU GPLv3
 *    File        : cLoadMeasurements.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for importing measurements
 *                : into the database.
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


#include "cLoadMeasurements.h"
using namespace Qrap;

//**********************************************************
cLoadMeasurements::cLoadMeasurements():QDialog()
{
	Setup();
}

//**********************************************************
cLoadMeasurements::cLoadMeasurements(QWidget* parent, Qt::WFlags fl)
:QDialog()
{
	Setup();
}

//**********************************************************
cLoadMeasurements::~cLoadMeasurements()
{
}

//***********************************************************
void cLoadMeasurements::on_cBoxMeasType_currentIndexChanged()
{
	if ("CW *.csv with Timestamp, Lat, Lon, and RSSI"==cBoxMeasType->currentText())
	{
		mFileFormat=eCSV;
		cBoxCell->setEnabled(true);
		lTxCell->setEnabled(true);
	}
	else if ("CW space delimited Lat, Lon and RxLev"==cBoxMeasType->currentText())
	{
		mFileFormat=eSpaceDelimited;
		cBoxCell->setEnabled(true);
		lTxCell->setEnabled(true);
	}
	else if ("CW *.val file"==cBoxMeasType->currentText())
	{
		mFileFormat=eVal;
		cBoxCell->setEnabled(true);
		lTxCell->setEnabled(true);
	}
	else if ("GSM Drivetest"==cBoxMeasType->currentText())
	{
		mFileFormat=eDriveTest;
		cBoxCell->setEnabled(false);
		lTxCell->setEnabled(false);
	}
}

//****************************************************************************
void cLoadMeasurements::on_pButtonLoad_clicked()
{
	if ("CW *.csv with Timestamp, Lat, Lon, and RSSI"==cBoxMeasType->currentText())
	{
		mFileFormat=eCSV;
	}
	else if ("CW space delimited Lat, Lon and RxLev"==cBoxMeasType->currentText())
	{
		mFileFormat=eSpaceDelimited;
	}
	else if ("CW *.val file"==cBoxMeasType->currentText())
	{
		mFileFormat=eVal;
	}
	else if ("GSM Drivetest"==cBoxMeasType->currentText())
	{
		mFileFormat=eDriveTest;
	}
}


//***********************************************************
void cLoadMeasurements::on_pButtonBrowse_clicked()
{
	mFileCount=0;
	QDir MyDir;
	QStringList list = QFileDialog::getOpenFileNames("","",this,
							"open file dialog",
					                "Choose files to load" );
    	if (list.size() > 0)
	{
		pButtonLoad->setEnabled(true);
		QStringList::Iterator it = list.begin();
		QString String = *it;
		QString File;
		mFileCount = 0;
   		mSourceDir = String.mid(0,String.lastIndexOf("/"));
    		while( it != list.end() ) 
    		{
    			String = *it;
    			File = String.mid(mSourceDir.length()+1);
    			mFiles.append(File);
    			QListWidgetItem *Item = new QListWidgetItem(File,listWidgetFiles);
    			mFileCount++;
    			++it;
    		}
    	}
    	else
    	{
    		pButtonLoad->setEnabled(false);
    	}
}

//******************************************************
void cLoadMeasurements::Finished()
{
	pButtonLoad->setEnabled(false);
	pButtonBrowse->setEnabled(true);
}

//******************************************************
void cLoadMeasurements::Setup()
{
	setupUi(this);
	LoadData();
}

//*****************************************************
void cLoadMeasurements::LoadData()
{
	StringIntArray vals;
	StringIntArrayIterator iterator;
	int j=1;
	int key = 0;
	
	cBoxMeasType->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("meastype","name",vals);
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasType->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	cBoxMeasType->setCurrentIndex(key);

	cBoxMeasSource->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("measdatasource","name",vals);
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	cBoxMeasSource->setCurrentIndex(key);


	cBoxPosSource->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("positionsource","name",vals);
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxPosSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	cBoxPosSource->setCurrentIndex(key);

}//end cLoadMeasurements::LoadData()




