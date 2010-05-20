 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cOrderArray.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This interface class is used setting the order in which 
 *                : rasterfiles should be used
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

#include "cOrderArray.h"
//***********************************************
cOrderArray::cOrderArray():QWidget()
{
	Setup();
}

//*********************************************
cOrderArray::cOrderArray(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	Setup();
}
//********************************************
cOrderArray::~cOrderArray()
{
}

//*******************************************
void cOrderArray::on_pushButtonAdd_clicked()
{
	try
	{
		int UseRow =tableWidgetFileSets4->rowCount();
		int Row =tableWidgetFileSets3->currentRow(); 
		CastRow(tableWidgetFileSets3,tableWidgetFileSets4,Row,UseRow);
		WriteDB();
	}
	catch (const exception &e)
	{
		cout << e.what() << endl;
	}
}

//******************************************
void cOrderArray::on_pushButtonRemove_clicked()
{
	try
	{
		int UseRow =tableWidgetFileSets3->rowCount();
		int Row =tableWidgetFileSets4->currentRow(); 
		CastRow(tableWidgetFileSets4,tableWidgetFileSets3,Row,UseRow);
		WriteDB();
	}
	catch (const exception &e)
	{
		cout << e.what() << endl;
	}
}

//*******************************************
void cOrderArray::on_comboBoxFileType4_currentIndexChanged()
{
	LoadTables();
}


//*****************************************
void cOrderArray::Setup()
{
	setupUi(this);
	LoadTables();
}

//*****************************************
void cOrderArray::LoadTables()
{
	while (tableWidgetFileSets3->rowCount() > 0)
	{
		tableWidgetFileSets3->removeRow(0);
	}
	while (tableWidgetFileSets4->rowCount() > 0)
	{
		tableWidgetFileSets4->removeRow(0);
	}
	string query = "SELECT * FROM filesets WHERE filetype='";
	query+=comboBoxFileType4->currentText().toStdString();
	query+="'";
	gDb.PerformRawSql(query);
	
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
			tableWidgetFileSets3->insertRow(i);
			QTableWidgetItem *ID = new QTableWidgetItem(r[i]["id"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,ID);
			QTableWidgetItem *Description = new QTableWidgetItem(r[i]["description"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,Description);
			QTableWidgetItem *Projection = new QTableWidgetItem(r[i]["projection"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,Projection);
			QTableWidgetItem *Res = new QTableWidgetItem(r[i]["resolution"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,Res);
			QTableWidgetItem *FileFormat = new QTableWidgetItem(r[i]["fileformat"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,FileFormat);
			if (r[i]["use"].c_str() == "t")
			{
				Boolean = "True";
			}
			else
			{
				Boolean = "False";
			}
			QTableWidgetItem *Use = new QTableWidgetItem(Boolean.c_str());
			tableWidgetFileSets3->setItem(i,Col++,Use);
			if (r[i]["derivedbinary"].c_str() == "t")
			{
				Boolean = "True";
			}
			else
			{
				Boolean = "False";
			}
			QTableWidgetItem *DerivedBinary = new QTableWidgetItem(Boolean.c_str());
			tableWidgetFileSets3->setItem(i,Col++,DerivedBinary);
			QTableWidgetItem *Classification = new QTableWidgetItem(r[i]["classgroup"].c_str());
			tableWidgetFileSets3->setItem(i,Col++,Classification);
		}
	}
	QStringList Filesets;
	QString Result;
	query = "SELECT orderarray FROM filesetsused WHERE type ='";
	query+=comboBoxFileType4->currentText().toStdString();
	query+="'";
	if (!gDb.PerformRawSql(query))
	{
		//QMessageBox::information(this, "QRap", "Error finding the DEM order array!");
	}
	gDb.GetLastResult(r);
	if (r.size() < 1)
	{
		//QMessageBox::information(this, "QRap", "There are no DEM filesets declared!");
		Result = "";
	}
	else
	{
		Result = QString ("%1").arg(r[0]["orderarray"].c_str());
		Result = Result.mid(1,Result.length()-2);
		Filesets=Result.split(",");
	}
	QStringListIterator i(Filesets);
	int UseRow=0;
	while (i.hasNext())
 	{			
 		QString FS = i.next();
 		int Row = 0;
 		bool found = false;
 		while (Row < tableWidgetFileSets3->rowCount() && !found)
 		{
 			QTableWidgetItem *ID = static_cast<QTableWidgetItem*>(tableWidgetFileSets3->item(Row,0));
 			if (FS == ID->text())
 			{
 				CastRow(tableWidgetFileSets3,tableWidgetFileSets4,Row,UseRow);
 				UseRow++;
 				found = true;
 			}
 			Row++;
 		}
 	}
 	if (tableWidgetFileSets4->rowCount() >0) tableWidgetFileSets4->setCurrentCell(0,0);
 	if (tableWidgetFileSets3->rowCount() >0) tableWidgetFileSets3->setCurrentCell(0,0);
 	EnableButtons();
}


//*****************************************************************
void cOrderArray::CastRow(QTableWidget* from,QTableWidget* to,int fromRow, int toRow)
{
	to->insertRow(toRow);
 				
 	for (int i = 0;i<8;i++)
 	{
 		QTableWidgetItem *It = static_cast<QTableWidgetItem*>(from->item(fromRow,i));
 		QTableWidgetItem *Is = new QTableWidgetItem(It->text());
 		to->setItem(toRow,i, Is);
 	}
 	from->removeRow(fromRow);
 	
 	if (from->rowCount() > 0)
 	{
 		if (fromRow-1 > 0)
 		{
 			from->setCurrentCell(fromRow-1,0);
 		}
 		else
 		{
 			from->setCurrentCell(0,0);
 		}
 	}
 	
 	if (to->rowCount() > 0)
 	{
 		to->setCurrentCell(toRow,0);
 	}
 	EnableButtons();
 	
}

//************************************************************
void cOrderArray::on_pushButtonUp_clicked()
{
	try
	{
		int Row =tableWidgetFileSets4->currentRow(); 
		CastRow(tableWidgetFileSets4,tableWidgetFileSets4,Row+1,Row-1);
		WriteDB();
	}
	catch (const exception &e)
	{
		cout << e.what() << endl;
	}
}
	
	
//****************************************************
void cOrderArray::on_pushButtonDown_clicked()
{
	try
	{
		int Row =tableWidgetFileSets4->currentRow(); 
		CastRow(tableWidgetFileSets4,tableWidgetFileSets4,Row,Row+2);
		Row+=1;
		tableWidgetFileSets4->setCurrentCell(Row,0);
		WriteDB();
	}
	catch (const exception &e)
	{
		cout << e.what() << endl;
	}
}

//**************************************************
void cOrderArray::EnableButtons()
{
	
	if (tableWidgetFileSets3->rowCount()>0)
 	{
 		pushButtonAdd->setEnabled(true);
 	}
 	else
 	{
 		pushButtonAdd->setEnabled(false);
 	}
 	
 	if (tableWidgetFileSets4->rowCount()>0)
 	{
 		pushButtonRemove->setEnabled(true);
 	}
 	else
 	{
 		pushButtonRemove->setEnabled(false);
 	}
}

//***************************************************************
void cOrderArray::on_tableWidgetFileSets4_itemSelectionChanged()
{
	if (tableWidgetFileSets4->rowCount()>1)
 		{
 			if (tableWidgetFileSets4->currentRow() != 0)
 			{
 				pushButtonUp->setEnabled(true);
 			}
 			else
 			{
 				pushButtonUp->setEnabled(false);
 			}
 			if ((tableWidgetFileSets4->currentRow() != tableWidgetFileSets4->rowCount()-1))
 			{
 				pushButtonDown->setEnabled(true);
 			}
 			else
 			{
 				pushButtonDown->setEnabled(false);
 			}
 		}
 		else
 		{
 			pushButtonUp->setEnabled(false);
 			pushButtonDown->setEnabled(false);
 		}
}

//**************************************************************
void cOrderArray::WriteDB()
{
	string query;
	for (int i = 0;i<tableWidgetFileSets3->rowCount();i++)
	{
		QTableWidgetItem *Id = static_cast<QTableWidgetItem*>(tableWidgetFileSets3->item(i,0));
		query = "UPDATE filesets set use = FALSE WHERE id=";
		query += Id->text().toStdString();
		cout << query << endl;
		gDb.PerformRawSql(query);
	}
	
	for (int i = 0;i<tableWidgetFileSets4->rowCount();i++)
	{
		QTableWidgetItem *Id = static_cast<QTableWidgetItem*>(tableWidgetFileSets4->item(i,0));
		query = "UPDATE filesets set use = TRUE WHERE id=";
		query += Id->text().toStdString();
		cout << query << endl;
		gDb.PerformRawSql(query);
	}
	
	query = "UPDATE filesetsused SET orderarray = '{";
	for (int i = 0;i<tableWidgetFileSets4->rowCount();i++)
	{
		QTableWidgetItem *Id = static_cast<QTableWidgetItem*>(tableWidgetFileSets4->item(i,0));
		cout << i << "\t"<<Id->text().toStdString() << endl;
		query += Id->text().toStdString();
		Id=NULL;
		if (i <tableWidgetFileSets4->rowCount()-1)
		{
			query+= ",";
		}
	}
	query+="}' WHERE type = '";
	query+=comboBoxFileType4->currentText().toStdString();
	query+="'";
	cout << query << endl;
	gDb.PerformRawSql(query);
	
	
}
