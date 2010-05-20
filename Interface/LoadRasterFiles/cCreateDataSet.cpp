 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cCreateDataSet.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used for creating a new 
 *                : raster file set. Slots into cLoadRasterFiles
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

#include "cCreateDataSet.h"
//***********************************************
cCreateDataSet::cCreateDataSet():QWidget()
{
	Setup();
	LoadData();
}

//*********************************************
cCreateDataSet::cCreateDataSet(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	Setup();
	LoadData();
}
//********************************************
cCreateDataSet::~cCreateDataSet()
{
}


//*********************************************
void cCreateDataSet::Setup()
{
		setupUi(this);
		LoadData();
		checkBoxDerivedBinary->setCheckState(Qt::Unchecked);
		tableWidgetFileSets5->setEnabled(false);
		tableWidgetFileSets5->setVisible(false);
		pushButtonCreate->setEnabled(false);
}

//*********************************************************
void cCreateDataSet::LoadData()
{
	while (tableWidgetFileSets5->rowCount() > 0)
	{
			tableWidgetFileSets5->removeRow(0);
	}
	
	string query = "SELECT * FROM filesets ";
	query += "WHERE ";
	query += "derivedbinary = FALSE";
	
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
			tableWidgetFileSets5->insertRow(i);
			QTableWidgetItem *ID = new QTableWidgetItem(r[i]["id"].c_str());
			tableWidgetFileSets5->setItem(i,Col++,ID);
			QTableWidgetItem *Description = new QTableWidgetItem(r[i]["description"].c_str());
			tableWidgetFileSets5->setItem(i,Col++,Description);
			QTableWidgetItem *Res = new QTableWidgetItem(r[i]["resolution"].c_str());
			tableWidgetFileSets5->setItem(i,Col++,Res);
			QTableWidgetItem *FileFormat = new QTableWidgetItem(r[i]["fileformat"].c_str());
			tableWidgetFileSets5->setItem(i,Col++,FileFormat);
			Boolean =  r[i]["use"].c_str();
			if (Boolean == "t")
			{
				Boolean = "True";
			}
			else
			{
				Boolean = "False";
			}
			QTableWidgetItem *Use = new QTableWidgetItem(Boolean.c_str());
			tableWidgetFileSets5->setItem(i,Col++,Use);
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
			tableWidgetFileSets5->setItem(i,Col++,DerivedBinary);
			QTableWidgetItem *Projection = new QTableWidgetItem(r[i]["projection"].c_str());
			tableWidgetFileSets5->setItem(i,Col++,Projection);
		}
	}
		
	// Selecting the first row
	if (tableWidgetFileSets5->rowCount() > 0)
	{
		tableWidgetFileSets5->setCurrentCell(0,0);
	}
	else
	{
		
	}
}


//**************************************************************
void cCreateDataSet::on_checkBoxDerivedBinary_stateChanged(int state)
{
	if (state)
	{
		tableWidgetFileSets5->setEnabled(true);
		tableWidgetFileSets5->setVisible(true);
		LoadBin();
	}
	else
	{
		tableWidgetFileSets5->setEnabled(false);
		tableWidgetFileSets5->setVisible(false);
	}
	LoadData();
}

//**************************************************************
void cCreateDataSet::on_lineEditDescription_textChanged(QString txt)
{
	if (txt.length() > 0)
	{
		pushButtonCreate->setEnabled(true);
	}
	else
	{
		pushButtonCreate->setEnabled(false);
	}
}


//***********************************************************
void cCreateDataSet::on_pushButtonCreate_clicked()
{
	pushButtonCreate->setEnabled(false);
	
	string query = "INSERT INTO filesets (lastmodified, description,";
	query += "filetype,resolution,date,use,fileformat,";
	query += "projection,derivedbinary";
	if (checkBoxDerivedBinary->checkState() == Qt::Unchecked)
		query += ") VALUES (";
	else query += ",origin) VALUES (";
	query +=  "now(),'";
	query += lineEditDescription->text().toStdString();
	query += "','";
	query += comboBoxFileType5->currentText().toStdString();
	query += "',";
	query += QString("%1").arg(spinBoxResolution->value()).toStdString();
	query += ",now(),TRUE,'";
	query += comboBoxFileFormat5->currentText().toStdString();
	query += "','";
	query += comboBoxProjection5->currentText().toStdString();
	query += "',";
	
	if (checkBoxDerivedBinary->checkState() == Qt::Unchecked)
	{
		query += "FALSE";
	}
	else
	{
		query += "TRUE,";
		int Row = tableWidgetFileSets5->currentRow();
		QTableWidgetItem *Id = static_cast<QTableWidgetItem*>(tableWidgetFileSets5->item(Row,0));
		query += Id->text().toStdString();
	}
	
	query += ")";
	try
	{
		gDb.PerformRawSql(query);
		QMessageBox::information(this, "QRap","FileSet Successfully Added" );
		LoadData();
	}
	catch (const exception &e) 
	{
		cout << e.what() <<endl;
		QMessageBox::information(this, "QRap",e.what() );
	}
	LoadData();
}

//***********************************************************
void cCreateDataSet::LoadBin()
{
	int Row = tableWidgetFileSets5->currentRow();
	QTableWidgetItem *Res = static_cast<QTableWidgetItem*>(tableWidgetFileSets5->item(Row,2));
	spinBoxResolution->setValue((int)Res->text().toDouble());
	comboBoxFileFormat5->setCurrentIndex(comboBoxFileFormat5->findText("BINFILE"));
	comboBoxProjection5->setCurrentIndex(comboBoxProjection5->findText("WGS84GC"));
}


