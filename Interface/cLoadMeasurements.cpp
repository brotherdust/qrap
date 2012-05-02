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

//****************************************************************
void cLoadMeasurements::on_pButtonClose_clicked()
{
	close();
}

//***********************************************************
void cLoadMeasurements::on_cBoxFileType_currentIndexChanged()
{
	bool InputsRequired=false;
	if (1==cBoxFileType->currentIndex())
	{
		mFileFormat=eSpaceDelimited;
		InputsRequired=true;
	}
	else if (2==cBoxFileType->currentIndex())
	{
		mFileFormat=eCSV;
		InputsRequired=true;
	}
	else if (0==cBoxFileType->currentIndex())
	{
		mFileFormat=eDriveTest;
		InputsRequired=false;
	}
	if (InputsRequired)
	{
		StringIntArray vals;
		StringIntArrayIterator iterator;
		int j=1;
		cBoxCell->insertItem(0,"0:NULL");
		gDb.GetFieldUiParams("measurement", "ci" ,vals);
		for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
		{
			QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
			cBoxCell->insertItem(j,temp,qVariantFromValue(iterator->first));
			j++;
		} // for
		cBoxCell->setEnabled(true);
		lTxCell->setEnabled(true);
		lFrequency->setEnabled(true);
		dSpinBoxFreq->setEnabled(true);
	}
	else
	{
		cBoxCell->setEnabled(false);
		lTxCell->setEnabled(false);
		lFrequency->setEnabled(false);
		dSpinBoxFreq->setEnabled(false);
	}
}

//****************************************************************************
void cLoadMeasurements::on_pButtonLoad_clicked()
{

	QStringList parts;
	cout << " In cLoadMeasurements::on_pButtonLoad_clicked()" << endl;
	parts = cBoxCell->currentText().split(":");
	unsigned CI = (unsigned) parts[0].toInt();

	parts = cBoxMeasType->currentText().split(":");
	unsigned MeasType = (unsigned) parts[0].toInt();
	
	parts = cBoxMeasSource->currentText().split(":");
	unsigned MeasSource = (unsigned) parts[0].toInt();

	parts = cBoxPosSource->currentText().split(":");
	unsigned PosSource = (unsigned) parts[0].toInt();

	double Sensitivity = dSpinBoxSensitivity->value();
	double MobileHeight = dSpinBoxMobileH->value();
	double Frequency = dSpinBoxFreq->value();

	cout << " In cLoadMeasurements::on_pButtonLoad_clicked(), ";
	cout << "cBoxFileType->currentIndex() = " << cBoxFileType->currentIndex() << endl;
	if (1==cBoxFileType->currentIndex())
	{
		mFileFormat=eSpaceDelimited;
		cMeasImportSpace MeasImport(Sensitivity, Frequency, MobileHeight,
					CI, MeasType, MeasSource, PosSource);

		QStringList::Iterator it = mFiles.begin();
		QString File;
		char * filename;
		filename = new char[MAX_PATH];
		string directory = mSourceDir.toStdString();
		string file;
		pButtonLoad->setEnabled(false);
		pButtonBrowse->setEnabled(false);
		pButtonClose->setEnabled(false);
	   	while( it != mFiles.end() ) 
	    	{
			File = *it;
			file = File.toStdString(); 
			sprintf(filename,"%s/%s",directory.c_str(),file.c_str());
			MeasImport.LoadMeasurement(filename);
	    		++it;
	    	}
		pButtonLoad->setEnabled(true);
		pButtonBrowse->setEnabled(true);
		pButtonClose->setEnabled(true);
		delete [] filename;
	}
	else if (2==cBoxFileType->currentIndex())
	{
		mFileFormat=eCSV;
	}
	else if (0==cBoxFileType->currentIndex())
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
	
	cBoxMeasType->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("testpoint","meastype",vals);
	
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasType->insertItem(j,temp);
		j++;
	} // for

	cBoxMeasSource->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("testpoint","measdatasource",vals);
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxMeasSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for


	cBoxPosSource->insertItem(0,"0:NULL");
	gDb.GetFieldUiParams("testpoint","positionsource",vals);
//	cout << vals.begin()->second << endl;
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		cBoxPosSource->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for

}//end cLoadMeasurements::LoadData()




