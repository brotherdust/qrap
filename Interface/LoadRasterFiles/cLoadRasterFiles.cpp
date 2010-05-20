 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadRasterFiles.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This base interface class is used managing the rasterfiles
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

#include "cLoadRasterFiles.h"
//********************************************************
cLoadRasterFiles::cLoadRasterFiles(): QDialog()
{
	Setup();
}

//********************************************************
cLoadRasterFiles::cLoadRasterFiles(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
	Setup();
}

//********************************************************
cLoadRasterFiles::~cLoadRasterFiles()
{
}

//********************************************************
void cLoadRasterFiles::on_treeWidget_currentItemChanged()
{
	QTreeWidgetItem *myItem = treeWidget->currentItem();
	if (myItem->text(0) == "Load Files into the Database")
	{
		CreateDataSet->LoadData();
		CutSet->LoadData();
		LoadFiles->LoadData();
		LoadFiles->show();
		//progressBar->show();
		CutSet->hide();
		CreateDataSet->hide();
		OrderArray->hide();
	}		
	else if (myItem->text(0) == "Cut a File Set")
	{
		CreateDataSet->LoadData();
		CutSet->LoadData();
		LoadFiles->LoadData();
		LoadFiles->hide();
		CutSet->show();
		//progressBar->show();
		CreateDataSet->hide();
		OrderArray->hide();
	}
	else if (myItem->text(0) == "Set File Set Usage Order")
	{
		CreateDataSet->LoadData();
		CutSet->LoadData();
		LoadFiles->LoadData();
		LoadFiles->hide();
		CutSet->hide();
		CreateDataSet->hide();
		//progressBar->hide();
		OrderArray->show();
	}
	else if (myItem->text(0) == "Create a new File Set")
	{
		CreateDataSet->LoadData();
		CutSet->LoadData();
		LoadFiles->LoadData();
		LoadFiles->hide();
		CutSet->hide();
		CreateDataSet->show();
		OrderArray->hide();
		//progressBar->hide();
	}
}

//********************************************************
void cLoadRasterFiles::Setup()
{
	setupUi(this);
	LoadFiles = new cLoadFiles(widgetContainer);
	connect(LoadFiles,SIGNAL(GiveFeedback(int,QString)),this,SLOT(GiveFeedback(int,QString)));
	CutSet = new cCutSet(widgetContainer);
	CreateDataSet = new cCreateDataSet(widgetContainer);
	OrderArray = new cOrderArray(widgetContainer);
		
	QTreeWidgetItem* Item = treeWidget->itemAt(0,0);
	treeWidget->setCurrentItem(Item);
	string query = "SELECT * FROM filesetsused WHERE type = 'DEM'";
	pqxx::result result;
	if (!gDb.PerformRawSql(query))
	{
		string err = "Error reading from filesetsused...";
		cout << err.c_str() << endl;
		QRAP_WARN(err.c_str());
	}
	else
	{
		gDb.GetLastResult(result);
		if (result.size() < 1)
		{
			query = "INSERT INTO filesetsused (id,lastmodified,type) VALUES";
			query += "(1,now(),'DEM')";
			if (!gDb.PerformRawSql(query))
			{
				string err = "Error inserting DEM into filesetsused...";
				cout << err.c_str() << endl;
				QRAP_WARN(err.c_str());
			}
		}
	}
	query = "SELECT * FROM filesetsused WHERE type = 'Clutter'";
	if (!gDb.PerformRawSql(query))
	{
		string err = "Error reading from filesetsused...";
		cout << err.c_str() << endl;
		QRAP_WARN(err.c_str());
	}
	else
	{
		gDb.GetLastResult(result);
		if (result.size() < 1)
		{
			query = "INSERT INTO filesetsused (id,lastmodified,type) VALUES";
			query += "(2,now(),'Clutter')";
			if (!gDb.PerformRawSql(query))
			{
				string err = "Error inserting Clutter into filesetsused...";
				cout << err.c_str() << endl;
				QRAP_WARN(err.c_str());
			}
		}
	}
}

//**************************************************************
void cLoadRasterFiles::GiveFeedback(int Per,QString Mes)
{
	if (Per != 100)
	{
		progressBar->show();
		labelFeedback->show();
		progressBar->setValue(Per);
		labelFeedback->setText(Mes);
		cout << "!!!!!!!!!!!!!!!!!!!!!!+++++++++" <<endl;
		cout << "Set Value: " << Per << endl;
	}
	else
	{
		//progressBar->hide();
		//labelFeedback->hide();
	}
}

