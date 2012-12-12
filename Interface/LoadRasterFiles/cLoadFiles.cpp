 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadFiles.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for importing files
 *                : into a raster file set.
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


#include "cLoadFiles.h"
//***********************************************
cLoadFiles::cLoadFiles():QWidget()
{
	Setup();
}

//*********************************************
cLoadFiles::cLoadFiles(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	Setup();
}
//********************************************
cLoadFiles::~cLoadFiles()
{
}

//********************************************
void cLoadFiles::on_pushButtonBrowse_clicked()
{
	pushButtonImport->setEnabled(true);
	int currentRow = tableWidgetFileSets->currentRow();
	QTableWidgetItem *FileType = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(currentRow,3));
	if (FileType != NULL)
	{
		if (FileType->text() != "ORTFILE")
		{
			QDir MyDir;
			mFiles.clear();
			listWidgetFiles->clear();
			QStringList list = QFileDialog::getOpenFileNames("","", this,
                  							  "open file dialog",
                    							"Choose files to load" );
    	
    			if (list.size() > 0)
    			{
    				pushButtonImport->setEnabled(true);
    				QStringList::Iterator it = list.begin();
    				QString String = *it;
    				QString File;
    				mCount = 0;
   				mSourceDir = String.mid(0,String.lastIndexOf("/"));
				labelDirectory->setText(mSourceDir);
    				while( it != list.end() ) 
    				{
    					String = *it;
    					File = String.mid(mSourceDir.length()+1);
    					mFiles.append(File);
    					QListWidgetItem *Item = new QListWidgetItem(File,listWidgetFiles);
    					mCount++;
    					++it;
    				}
    			}
    			else
    			{
    				pushButtonImport->setEnabled(false);
    			}
		}
		else
		{
			QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 labelDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    			if (dir.length() >0)
    			{
    				labelDirectory->setText(dir);
    				if (lineEditORTDirectory->text().length() > 0)
    				{
    					pushButtonImport->setEnabled(true);
    				}
    				else
    				{
    					pushButtonImport->setEnabled(false);
    				}
    			}
    			else
    			{
    				pushButtonImport->setEnabled(false);
    			}	
		}
	}
}

//********************************************
void cLoadFiles::on_pushButtonORTBrowse_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 labelDirectory->text(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dir.length() >0)
    {
    	lineEditORTDirectory->setText(dir);
    	if (labelDirectory->text().length() > 0)
    	{
    		pushButtonImport->setEnabled(true);
    	}
    	else
    	{
    		pushButtonImport->setEnabled(false);
    	}
    }
    else
    {
    	pushButtonImport->setEnabled(false);
    }	
    
}

//***************************************
void cLoadFiles::on_pushButtonImport_clicked()
{
	int currentRow = tableWidgetFileSets->currentRow();
	QTableWidgetItem *FileType = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(currentRow,3));
	if (FileType != NULL)
	{
		if (tableWidgetFileSets->rowCount() > 0)
		{
			pushButtonImport->setEnabled(false);
			pushButtonBrowse->setEnabled(false);
			int Row = tableWidgetFileSets->currentRow();
			QTableWidgetItem *ID = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(Row,0));
			QTableWidgetItem *Bin = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(Row,5));
			QTableWidgetItem *Projection = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(Row,6));
			QTableWidgetItem *FileFormat = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(Row,3));
			if (Bin->text() == "True")
			{
				mBin=(short int)ID->text().toDouble();
				mSource = mBin-1;
			}
			else
			{
				mSource=(short int)ID->text().toDouble();
				mBin = mSource+1;
			}
			if (FileFormat->text() != "")
			{
				mFileType = FileFormat->text();
			}
			else
			{
				mFileType = "GDALFILE";
			}
			if (Projection->text() == "DEG")
			{
				mType = DEG;
			}
			else
			{
				if (Projection->text() == "UTM")
				{
					mType = UTM;
				}
				else
				{
					if (Projection->text() == "WGS84GC")
					{
						mType = WGS84GC;
					}
					else
					{
						mType = DEG;
					}
				}
			}
		
			mCentMer = -1; //\TODO: If CentMer has to be selected by User! GUI doesn't support this.
			if (FileType->text() !="ORTFILE")
			{
		
				CLoadThread->Set(mSource,  //Source Index
						mBin,  //Binary Index (Actually - Used in old stuff)
						mType,  //GeoType
						mCentMer, 
						mSourceDir.toStdString(),  //Binary Directory (Actually - Used in old stuff)
						mFiles, // List of Files
						mSourceDir, // Directory of Source Files
						mCount, // File Count
						mFileType); // QString File Type (GDAL<BIN)
						
				Set(0,"Initialising.....");
				connect(CLoadThread,SIGNAL(Set(int,QString)),this,SLOT(Set(int,QString)));
				connect(CLoadThread,SIGNAL(Finished()),this,SLOT(Finished()));
				CLoadThread->start();
			}
			else //ort files
			{
				cLoadFile Files;
				cout << "Na cLoadfile Constructer" << endl;
				try
				{
					string Source = labelDirectory->text().toStdString();
					string ORTDir = lineEditORTDirectory->text().toStdString();
					string File =  lineEditORTFile->text().toStdString();
					unsigned FileSet = (unsigned)mSource;
					int CentMer = spinBoxORTCentralMeridian->value();
					printf("ORTDir: %s\nBIN: %s\nFile: %s\nFileSet:%d\nCentMer:%d\n",
						Source.c_str(),ORTDir.c_str(),File.c_str(),FileSet,CentMer);
					Files.LoadOrt(Source,
							ORTDir, 
							 File,
							 FileSet, 
							 CentMer);
				}
				catch (const exception &e)
				{
					cout << "Loading ORT Files..." << endl;
					cout << e.what() << endl;
				}
				
				QMessageBox::information(this, "QRap","Finished loading Directory");
				pushButtonImport->setEnabled(true);
			}
		}
	}
}

//*******************************************************
void cLoadFiles::Set(int tel,QString msgs)
{
	int Persentage = (tel*100)/mCount;
	cout << "tel: "<<tel<< "\tmCount:"<<mCount<< "\tPer:"<< Persentage<<endl;
	emit GiveFeedback(Persentage,msgs);
}

//******************************************************
void cLoadFiles::Finished()
{
	int Persentage = 100;
	cout << "tel: "<<mCount<< "\tmCount:"<<mCount<< "\tPer:"<< Persentage<<endl;
	QString msgs = "Files loaded.";

	emit GiveFeedback(Persentage,msgs);
	mFiles.clear();
	listWidgetFiles->clear();

	disconnect(CLoadThread,SIGNAL(Set(int,QString)),this,SLOT(Set(int,QString)));
	disconnect(CLoadThread,SIGNAL(Finished()),this,SLOT(Finished()));

	pushButtonBrowse->setEnabled(true);
}


//******************************************************
void cLoadFiles::on_comboBoxFileType_currentIndexChanged()
{
	LoadData();
}

//*************************************************
void cLoadFiles::on_comboBoxFileFormat_currentIndexChanged()
{
	LoadData();
}

//*****************************************************
void cLoadFiles::on_comboBoxProjection_currentIndexChanged()
{
	LoadData();
}

//******************************************************
void cLoadFiles::Setup()
{
	setupUi(this);
	CLoadThread = new cLoadThread(this);
	LoadData();
}

//*****************************************************
void cLoadFiles::LoadData()
{
	while (tableWidgetFileSets->rowCount() > 0)
	{
		tableWidgetFileSets->removeRow(0);
	}
	string query = "SELECT * FROM filesets ";
	query += "WHERE ";
	query += "filetype = '";
	query += comboBoxFileType->currentText().toStdString();
	query += "'";
	if (comboBoxFileFormat->currentText() != "All")
	{
		query += " AND fileformat = '";
		query += comboBoxFileFormat->currentText().toStdString();
		query += "'";
	}
	if (comboBoxProjection->currentText() != "All")
	{
		query += " AND projection = '";
		query += comboBoxProjection->currentText().toStdString();
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
			tableWidgetFileSets->insertRow(i);
			QTableWidgetItem *ID = new QTableWidgetItem(r[i]["id"].c_str());
			tableWidgetFileSets->setItem(i,Col++,ID);
			QTableWidgetItem *Description = new QTableWidgetItem(r[i]["description"].c_str());
			tableWidgetFileSets->setItem(i,Col++,Description);
			QTableWidgetItem *Res = new QTableWidgetItem(r[i]["resolution"].c_str());
			tableWidgetFileSets->setItem(i,Col++,Res);
			QTableWidgetItem *FileFormat = new QTableWidgetItem(r[i]["fileformat"].c_str());
			tableWidgetFileSets->setItem(i,Col++,FileFormat);
			
			Boolean = r[i]["use"].c_str();
			if (Boolean == "t")
			{
				Boolean = "True";
			}
			else
			{
				Boolean = "False";
			}
			QTableWidgetItem *Use = new QTableWidgetItem(Boolean.c_str());
			tableWidgetFileSets->setItem(i,Col++,Use);
			
			Boolean = r[i]["derivedbinary"].c_str();
			if (Boolean == "t")
			{
				Boolean = "True";
			}
			else
			{
				Boolean = "False";
			}
			
			QTableWidgetItem *DerivedBinary = new QTableWidgetItem(Boolean.c_str());
			tableWidgetFileSets->setItem(i,Col++,DerivedBinary);
			QTableWidgetItem *Projection = new QTableWidgetItem(r[i]["projection"].c_str());
			tableWidgetFileSets->setItem(i,Col++,Projection);
			QTableWidgetItem *Classification = new QTableWidgetItem(r[i]["classgroup"].c_str());
			tableWidgetFileSets->setItem(i,Col++,Classification);
			//mGeoType.push_back((GeoType)(int)atof(r[i]["projection"].c_str()));
		}
	}
	
	// Selecting the first row
	if (tableWidgetFileSets->rowCount() > 0)
	{
		tableWidgetFileSets->setCurrentCell(0,0);
		pushButtonBrowse->setEnabled(true);
	}
	else
	{
		pushButtonBrowse->setEnabled(false);
		pushButtonImport->setEnabled(false);
	}
}

//************************************************************
void cLoadFiles::on_tableWidgetFileSets_currentCellChanged ( int currentRow, int currentColumn, int previousRow, int previousColumn )
{
	
//	disconnect(tableWidgetFileSets,SIGNAL(currentCellChanged ( int , int , int , int )),this,SLOT(on_tableWidgetFileSets1_currentCellChanged ( int, int, int, int)));
//	
	if (tableWidgetFileSets->rowCount() > 0)
	{
		QTableWidgetItem *FileType = static_cast<QTableWidgetItem*>(tableWidgetFileSets->item(currentRow,3));
		if (FileType != NULL)
		{
			if (FileType->text() == "ORTFILE")
			{
				widgetORT->setVisible(true);
				widgetORT->setEnabled(true);	
				listWidgetFiles->setVisible(false);	
				
			}
			else
			{
				widgetORT->setVisible(false);
				widgetORT->setEnabled(false);		
				listWidgetFiles->setVisible(true);
			}
		}
	}
//	
//	connect(tableWidgetFileSets,SIGNAL(currentCellChanged ( int , int , int , int )),this,SLOT(on_tableWidgetFileSets1_currentCellChanged ( int, int, int, int)));	
}

