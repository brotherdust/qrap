 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCutSet.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for cutting an existing
 *                : raster file set into smaller Guass Conform files.
 *                :  Slots into cLoadRasterFiles
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
//\TODO: This whole LoadRasterFile bunch need to be cleaned up.

#include "cCutSet.h"

//***********************************************
cCutSet::cCutSet():QWidget()
{
	Setup();
	LoadData();
}

//*********************************************
cCutSet::cCutSet(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	Setup();
	LoadData();
}
//********************************************
cCutSet::~cCutSet()
{
}

//********************************************
void cCutSet::on_pushButtonBrowse1_clicked()
{
	mFileType = comboBoxFileType1->currentText();
	mFiles.clear();
	mBinDir = QFileDialog::getExistingDirectory(this,"Please select a directory");
	lineEditDirectory->setText(mBinDir);
}


//***************************************
void cCutSet::on_pushButtonCut1_clicked()
{
	cout << "Cutting......."<<endl;
	mFileType = comboBoxFileType1->currentText();
	try
	{
		if ((tableWidgetFileSets1->rowCount() > 0) && (tableWidgetFileSets2->rowCount()>0))
		{
			
			pushButtonBrowse1->setEnabled(false);
			pushButtonCut1->setEnabled(false);
			lineEditDirectory->setEnabled(false);
			int OriginalRow = tableWidgetFileSets1->currentRow();
			int BinRow = tableWidgetFileSets2->currentRow();
		
			cout << "Setting up mocking vals..."<<endl;
			//Values are not used....
			mCentMer =-1;
			mFiles.clear();
			mCount =0;
			mFileType=GDALFILE;
			mBinDir = lineEditDirectory->text();
		
			QTableWidgetItem *ID = static_cast<QTableWidgetItem*>(tableWidgetFileSets1->item(OriginalRow,0));
			QTableWidgetItem *Bin = static_cast<QTableWidgetItem*>(tableWidgetFileSets2->item(BinRow,0));
			mSource = (short int)ID->text().toDouble();
			mBin = (short int)Bin->text().toDouble();
			cout << "Setting up thread..."<<endl;
/*			CLoadThread->Set(mSource,  //Source Index
					mBin,  //Binary Index (Actually - Used in old stuff)
					mType,  //GeoType
					mCentMer, 
					mBinDir.toStdString(),  //Binary Directory (Actually - Used in old stuff)
					mFiles, // List of Files
					mBinDir, // Directory of Source Files
					mCount, // File Count
					mFileType, // QString File Type (DEM or Clutter)
					true); 
			cout << "Initialising..."<<endl;		
			Set(0,"Initialising.....");
			connect(CLoadThread,SIGNAL(Set(int,QString)),this,SLOT(Set(int,QString)));
			connect(CLoadThread,SIGNAL(Finished()),this,SLOT(Finished()));
			cout << "Starting Thread..."<<endl;
			CLoadThread->start();
*/
			
			bool Interpolate;
			short int FileRule=mIFileType;
			Interpolate = (1==FileRule);
			
			cLoadFile Rasters(mSource,mBin,mBinDir.toStdString());
			Rasters.CutCurrentSet(mSource,mBin,Interpolate,FileRule);
		}
	}
	catch(const exception &e) 
	{
		cout << e.what() << endl;
	}
	LoadData();
}

//*******************************************************
void cCutSet::Set(int tel,QString msgs)
{
	emit GiveFeedback(tel,msgs);
	LoadData();
}

//******************************************************
void cCutSet::Finished()
{
	disconnect(CLoadThread,SIGNAL(Set(int,QString)),this,SLOT(Set(int,QString)));
	disconnect(CLoadThread,SIGNAL(Finished()),this,SLOT(Finished()));
	
	mFiles.clear();
	pushButtonCut1->setEnabled(true);
	pushButtonBrowse1->setEnabled(true);
	lineEditDirectory->setEnabled(true);
	LoadData();
}


//******************************************************
void cCutSet::on_comboBoxFileType1_currentIndexChanged()
{
	mFileType = comboBoxFileType1->currentText();
	LoadData();
}

//*************************************************
void cCutSet::on_comboBoxFileFormat1_currentIndexChanged()
{
	LoadData();	
}

//*****************************************************
void cCutSet::on_comboBoxProjection1_currentIndexChanged()
{
	LoadData();
}

//******************************************************
void cCutSet::Setup()
{
	setupUi(this);
	CLoadThread = new cLoadThread(this);
	LoadData();
	lineEditDirectory->setText(gDb.GetSetting("OutputDir").c_str());
	LoadData();
}

//*****************************************************
void cCutSet::LoadData()
{
	disconnect(tableWidgetFileSets1,SIGNAL(currentCellChanged ( int , int , int , int )),this,SLOT(on_tableWidgetFileSets1_currentCellChanged ( int, int, int, int)));
	
	cout << comboBoxFileFormat1->currentText().toStdString() << endl;
	mFileType = comboBoxFileType1->currentText();
	while (tableWidgetFileSets1->rowCount() > 0)
	{
			tableWidgetFileSets1->removeRow(0);
	}
	string query = "SELECT * FROM filesets ";
	query += "WHERE derivedbinary=FALSE AND ";
	query += "filetype = '";
	query += comboBoxFileType1->currentText().toStdString();
	cout << comboBoxFileType1->currentText().toStdString() << endl;

	if (!strcmp("DEM", comboBoxFileType1->currentText().toStdString().c_str()))
		mIFileType = 1;
	else 	mIFileType = 2;
	
	query += "'";
	if (comboBoxFileFormat1->currentText() != "All")
		{
			query += " AND fileformat = '";
			query += comboBoxFileFormat1->currentText().toStdString();
			query += "'";
		}
		if (comboBoxProjection1->currentText() != "All")
		{
			query += " AND projection = '";
			query += comboBoxProjection1->currentText().toStdString();
			query += "'";
		}
		pqxx::result r;
		if (!gDb.PerformRawSql(query))
		{
			//\TODO: Error message
		}
		else
		{
			gDb.GetLastResult(r);
			for (int i = 0; i < r.size() ; i++)
			{
				int Col = 0;
				string Boolean;
				tableWidgetFileSets1->insertRow(i);
				QTableWidgetItem *ID = new QTableWidgetItem(r[i]["id"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,ID);
				QTableWidgetItem *Description = new QTableWidgetItem(r[i]["description"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,Description);
				QTableWidgetItem *Projection = new QTableWidgetItem(r[i]["projection"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,Projection);
				QTableWidgetItem *Res = new QTableWidgetItem(r[i]["resolution"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,Res);
				QTableWidgetItem *FileFormat = new QTableWidgetItem(r[i]["fileformat"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,FileFormat);
				if (r[i]["use"].c_str() == "t")
				{
					Boolean = "True";
				}
				else
				{
					Boolean = "False";
				}
				QTableWidgetItem *Use = new QTableWidgetItem(Boolean.c_str());
				tableWidgetFileSets1->setItem(i,Col++,Use);
				if (r[i]["derivedbinary"].c_str() == "t")
				{
					Boolean = "True";
				}
				else
				{
					Boolean = "False";
				}
				QTableWidgetItem *DerivedBinary = new QTableWidgetItem(Boolean.c_str());
				tableWidgetFileSets1->setItem(i,Col++,DerivedBinary);
				QTableWidgetItem *Classification = new QTableWidgetItem(r[i]["classgroup"].c_str());
				tableWidgetFileSets1->setItem(i,Col++,Classification);
			}
		}
	
	connect(tableWidgetFileSets1,SIGNAL(currentCellChanged ( int , int , int , int )),this,SLOT(on_tableWidgetFileSets1_currentCellChanged ( int, int, int, int)));
	// Selecting the first row
	if (tableWidgetFileSets1->rowCount() > 0)
	{
		tableWidgetFileSets1->setCurrentCell(0,0);
	}
	else
	{
		
	}
	mFileType = comboBoxFileType1->currentText();
	
}

//*************************************************************
void cCutSet::on_tableWidgetFileSets1_currentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	
	while (tableWidgetFileSets2->rowCount() > 0)
	{
			tableWidgetFileSets2->removeRow(0);
	}

	if (tableWidgetFileSets1->rowCount() > 0)
	{
		QTableWidgetItem *ID = static_cast<QTableWidgetItem*>(tableWidgetFileSets1->item(currentRow,0));
		if (ID != NULL) 
		{
			cout<<"NOT NULL"<<endl; 
			cout << "ID: "<< ID->text().toStdString()<<endl;
			string query = "SELECT * FROM filesets ";
			query += "WHERE derivedbinary=TRUE AND origin=";
			query+= ID->text().toStdString();
			//cout << query.c_str()<<endl;
			pqxx::result r;
			if (!gDb.PerformRawSql(query))
			{
			//\TODO: Error message
			}
			else
			{
				gDb.GetLastResult(r);
				for (int i = 0; i < r.size() ; i++)
				{
					int Col = 0;
					string Boolean;
					cout << "Writing new row....."<<endl;
					tableWidgetFileSets2->insertRow(i);
					QTableWidgetItem *ID = new QTableWidgetItem(r[i]["id"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,ID);
					QTableWidgetItem *Description = new QTableWidgetItem(r[i]["description"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,Description);
					QTableWidgetItem *Projection = new QTableWidgetItem(r[i]["projection"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,Projection);
					QTableWidgetItem *Res = new QTableWidgetItem(r[i]["resolution"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,Res);
					QTableWidgetItem *FileFormat = new QTableWidgetItem(r[i]["fileformat"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,FileFormat);
					if (r[i]["use"].c_str() == "t")
					{
						Boolean = "True";
					}
					else
					{
						Boolean = "False";
					}
					QTableWidgetItem *Use = new QTableWidgetItem(Boolean.c_str());
					tableWidgetFileSets2->setItem(i,Col++,Use);
					if (r[i]["derivedbinary"].c_str() == "t")
					{
						Boolean = "True";
					}
					else
					{
						Boolean = "False";
					}
					QTableWidgetItem *DerivedBinary = new QTableWidgetItem(Boolean.c_str());
					tableWidgetFileSets2->setItem(i,Col++,DerivedBinary);
					QTableWidgetItem *Classification = new QTableWidgetItem(r[i]["classgroup"].c_str());
					tableWidgetFileSets2->setItem(i,Col++,Classification);
				}
			}
		}
		if (tableWidgetFileSets2->rowCount() > 0)
		{
			mFileType = comboBoxFileType1->currentText();
			tableWidgetFileSets2->setCurrentCell(0,0);
			pushButtonCut1->setEnabled(true);
		}
		else
		{
			pushButtonCut1->setEnabled(false);
		}
	}
}

