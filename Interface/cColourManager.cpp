 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cColourManager.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : (Magdaleen Ballot) 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This is to edit the colurs. Probably based on QGIS code
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

#include "cColourManager.h"

//***************************************************************************************
cColourManager::cColourManager(): QWidget()
{
	setupUi(this);
	//this->setModal(true);
}

//****************************************************************************************
cColourManager::cColourManager(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	setupUi(this);
	//this->setModal(true);
	connect(mColormapTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), 
		this, SLOT(handleColormapTreeWidgetDoubleClick(QTreeWidgetItem*,int)));
	QStringList headerLabels;
  	headerLabels << "Value";
  	headerLabels << "Color";
  	headerLabels << "Label";
  	mColormapTreeWidget->setHeaderLabels(headerLabels);
  	GetDefaults();
}

//****************************************************************************************
cColourManager::~cColourManager()
{
}

//****************************************************************************************
void cColourManager::on_btnCancel_clicked()
{
	close();
}

//***************************************************************************************
void cColourManager::on_btnOk_clicked()
{
	//if (defaultRadio->isChecked())
	{
		QString query = "DELETE FROM colourmanagement WHERE plottype ='";
		query += plotTypeCombo->currentText().latin1();
		query +="'";
		if (!gDb.PerformRawSql(query.latin1()))
		{
			cout <<"Error selecting colours" << endl;
		}
		//iterate through mColormapTreeWidget and set colormap info of layer
  
  		int r,g,b,a;
	      	int myTopLevelItemCount = mColormapTreeWidget->topLevelItemCount();
      		QTreeWidgetItem* myCurrentItem;
  
	      	for(int i = 0; i < myTopLevelItemCount; ++i)
      		{
        		myCurrentItem = mColormapTreeWidget->topLevelItem(i);
        		if(!myCurrentItem)
        		{
        	  		continue;
        		}
        		r = myCurrentItem->background(1).color().red();
        		g = myCurrentItem->background(1).color().green();
        		b = myCurrentItem->background(1).color().blue();
        		a = myCurrentItem->background(1).color().alpha();
        		//myCurrentItem->background(1).color().getRgb(r,g,b,a);
     			query = QString("INSERT INTO colourmanagement (r,g,b,a,val,label,plottype) ");
     			query = QString("%1 VALUES (%2, %3,%4,%5,%6,'%7','%8')").arg(query).arg(r).arg(g).arg(b).arg(a).
					arg(myCurrentItem->text(0)).arg(myCurrentItem->text(2)).arg( plotTypeCombo->currentText());
     			cout << r << " " << g << " " << b << " " << a << endl;
     			if (!gDb.PerformRawSql(query.latin1()))
			{
				cout <<"Error selecting colours" << endl;
			}	
      		}
	}
	/*else
	{
		QStringList::Iterator it = mFiles.begin();
    	QString File;
    
   
    	while( it != mFiles.end() ) 
    	{
    		File = *it++;
    		cout << "Calling Set Colour..." << endl;
    		SetColour(mSourceDir,File);
    	}
	}*/
	//accept();
}

//*************************************************************************************
void cColourManager::on_addFilesButton_clicked()
{
//	QDir MyDir;
//	mFiles.clear();
//	fileList->clear();
//	QStringList list = QFileDialog::getOpenFileNames(
//                    "",
//                    "",
//                    this,
//                    "open file dialog",
//                    "Choose files to load" );
//    if (list.size()>0)
//    {
//    QStringList::Iterator it = list.begin();
//    QString String = *it;
//    QString File;
//    mCount = 0;
//   mSourceDir = String.mid(0,String.lastIndexOf("/"));
//    while( it != list.end() ) 
//    {
//    	String = *it;
//    	File = String.mid(mSourceDir.length()+1);
//    	mFiles.append(File);
//    	QListWidgetItem *Item = new QListWidgetItem(File,fileList);
//    	mCount++;
//    	++it;
//    }
//    }
}

//***********************************************************************************************
void cColourManager::on_addButton_clicked()
{
	QTreeWidgetItem* newItem = new QTreeWidgetItem(mColormapTreeWidget);
      	newItem->setText(0,"0");
      	//newItem->setBackground(1, QBrush(*color_it));
      	newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
}

//***********************************************************************************************
void cColourManager::on_removeButton_clicked()
{
	QTreeWidgetItem* currentItem = mColormapTreeWidget->currentItem();
 	if(currentItem)
    	{
      		delete currentItem;
    	}
}

//**********************************************************************************************
void cColourManager::on_plotTypeCombo_currentIndexChanged()
{
	GetDefaults();
}

//**********************************************************************************************
void cColourManager::handleColormapTreeWidgetDoubleClick(QTreeWidgetItem* item, int column)
{
  	if(item)
    	{
      		if(column == 1)
		{
			//show color dialog
			QColor newColor = QColorDialog::getColor();
			if(newColor.isValid())
			{
	      			item->setBackground(1, QBrush(newColor));
			}
		}
	}
}

//***************************************************************************************
void cColourManager::GetDefaults()
{
	string query = "SELECT * FROM colourmanagement WHERE plottype ='";
	query += plotTypeCombo->currentText().latin1();
	query +="'";
	if (!gDb.PerformRawSql(query))
	{
		cout <<"Error selecting colours" << endl;
	}
	else
	{
		pqxx::result res;
		gDb.GetLastResult(res);
		mColormapTreeWidget->clear();
		
		int r,g,b,a;
		
  		for(int i =0; i < res.size(); i++)
    		{
	      		QTreeWidgetItem* newItem = new QTreeWidgetItem(mColormapTreeWidget);
      			newItem->setText(0, QString("%1").arg(res[i]["val"].c_str()));
      			newItem->setText(2, QString("%1").arg(res[i]["label"].c_str()));
      			r = (int)atof(res[i]["r"].c_str());
      			g = (int)atof(res[i]["g"].c_str());
      			b = (int)atof(res[i]["b"].c_str());
      			a = (int)atof(res[i]["a"].c_str());
      			QColor Col =  QColor(r,g,b,a);
      			newItem->setBackground(1, QBrush(Col));
      			newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
    		}
	}
}


//*************************************************************************************************
void cColourManager::SetColour(QString Dir, QString File,QString PlotType)
{
	Band Col;
	
	mColours.clear();
	mBands.clear();
	
	cout << "Setting up the Colour Bands" << endl;
	// Setting up the Colour Bands
	if (PlotType == "")
	{
		int myTopLevelItemCount = mColormapTreeWidget->topLevelItemCount();
      		QTreeWidgetItem* myCurrentItem;
      	
	      	for(int i = 0; i < myTopLevelItemCount; ++i)
	      	{
	        	myCurrentItem = mColormapTreeWidget->topLevelItem(i);
	        	if(!myCurrentItem)
	        	{
	          		continue;
	        	}
	        	Col.r = myCurrentItem->background(1).color().red();
	        	Col.g = myCurrentItem->background(1).color().green();
	        	Col.b = myCurrentItem->background(1).color().blue();
	        	Col.a = myCurrentItem->background(1).color().alpha();
	        	Col.val = (float)myCurrentItem->text(0).toDouble();
	        	mBands.push_back(Col);
	      	}
	}
	else
	{
		string query = "SELECT * FROM colourmanagement WHERE plottype ='";
		query += plotTypeCombo->currentText().latin1();
		query +="'";
		if (!gDb.PerformRawSql(query))
		{
			cout <<"Error selecting colours" << endl;
		}
		else
		{
			pqxx::result res;
			gDb.GetLastResult(res);
			mColormapTreeWidget->clear();
		
  			for(int i =0; i < res.size(); i++)
    			{
      				Col.val=atof(res[i]["val"].c_str());
      				Col.r = (int)atof(res[i]["r"].c_str());
	      			Col.g = (int)atof(res[i]["g"].c_str());
      				Col.b = (int)atof(res[i]["b"].c_str());
      				Col.a = (int)atof(res[i]["a"].c_str());
      				mBands.push_back(Col);   
    			}
		}
	}
	
	cout << "Setting up the Colours" << endl;
	// Setting up the individual colours
	int band = 0;
	while (band < mBands.size()-1)
	{
		int dif = (int)(mBands[band].val - mBands[band+1].val);
		int rdif =(int)ceil((mBands[band].r - mBands[band+1].r)/dif);
		int gdif =(int)ceil((mBands[band].g - mBands[band+1].g)/dif);
		int bdif =(int)ceil((mBands[band].b - mBands[band+1].b)/dif);
		int r = mBands[band].r;
		int g = mBands[band].g;
		int b = mBands[band].b;
		if (mBands[band].val < mBands[band+1].val)
		{
			for (float i = mBands[band].val; (i) <(mBands[band+1].val); i++)
			{
				if ((r+rdif)<0 || (r+rdif) > 255) rdif*=-1;
				if ((g+gdif)<0 || (g+gdif) > 255) gdif*=-1;
				if ((b+bdif)<0 || (b+bdif) > 255) bdif*=-1;
				r+=rdif;
				g+=gdif;
				b+=bdif;
				Col.val = i;
				Col.r=r;
				Col.g=g;
				Col.b=b;
				Col.a=mBands[band].a;
				mColours.push_back(Col);
				//cout << Col.val << "\t" << r << "\t"<< g << "\t"<< b << endl;
			}
		}
		else
		{
			for (float i = mBands[band+1].val; (i) <(mBands[band].val); i++)
			{
				if ((r+rdif)<0 || (r+rdif) > 255) rdif*=-1;
				if ((g+gdif)<0 || (g+gdif) > 255) gdif*=-1;
				if ((b+bdif)<0 || (b+bdif) > 255) bdif*=-1;
				r+=rdif;
				g+=gdif;
				b+=bdif;
				Col.val = i;
				Col.r=r;
				Col.g=g;
				Col.b=b;
				Col.a=mBands[band].a;
				mColours.push_back(Col);
				//cout << Col.val << "\t" << r << "\t"<< g << "\t"<< b << endl;
			}
		} 
		band++;
	}
	
	
	//Writing the file
	cout << "Writing file" << endl;
	GDALAllRegister(); 
	QString FN = Dir+"/" +File;
	GDALDataset *poSrcDS = 
       (GDALDataset *) GDALOpen( FN.latin1(), GA_ReadOnly );
    	GDALDataset *poDstDS;
	const char *pszFormat = "HFA";
	FN += "1";
	
	GDALDriver *poDriver;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
    	poDstDS = poDriver->CreateCopy( FN.latin1(), poSrcDS, FALSE, 
                                    NULL, NULL, NULL );
                            
	if( poDstDS == NULL )
	{
		string err = "Error reading file: ";
		err+=File.latin1();
		err+=" before PlotType. en of file reached";
		QRAP_ERROR(err.c_str());
		printf("We have a null");
		//\TODO:Error message
	}
	GDALColorTable *Colors = new GDALColorTable();
	for (int i = 0 ; i < mColours.size(); i ++)
	{
 		GDALColorEntry ColorBandMin;
 		ColorBandMin.c1 = mColours[i].r;
 		ColorBandMin.c2 = mColours[i].g;
 		ColorBandMin.c3 =mColours[i].b;
 		ColorBandMin.c4 = mColours[i].a;
 		//ColorBandMin.c4 = 255;
 		//GDALColorEntry ColorBandMax ={mBands[i+1].r,mBands[i+1].g,mBands[i+1].b,mBands[i+1].a};
 		Colors->SetColorEntry((int)mColours[i].val,&ColorBandMin);
 		cout << mColours[i].val << "\t" <<mColours[i].r << "\t"<< mColours[i].g << "\t"<< mColours[i].b << endl;
	}
	CPLErr lCrGdal;
	
	GDALRasterBand *poBand = poDstDS->GetRasterBand(1);
	
//	lCrGdal = poBand->SetNoDataValue(-9999);
//	if (lCrGdal == CE_Failure)
// 	{
// 		//\TODO: Error Message
// 		cout << "Error writing GDAL NoDataValue " << endl; 
// 	}
    
   // poBand->CreateColorRamp();
    	lCrGdal = poBand->SetColorTable(Colors);
	if (lCrGdal == CE_Failure)
 	{
 		//\TODO: Error Message
 		cout << "Error writing GDAL Colours " << endl; 
 	}
    	if (poDstDS != NULL)
 	{
 		delete poDstDS;
 		poDstDS = NULL;
 	}
 	if (poSrcDS != NULL)
 	{
 		delete poSrcDS;
 		poSrcDS = NULL;
 	}
 	GDALDestroyDriverManager();
 	
 	cout <<"Fine" << endl;
} 

//***************************************************************************************************
void cColourManager::on_mClassifyButton_clicked()
{
	//QTreeWidgetItem* newItem = new QTreeWidgetItem(mColormapTreeWidget);
    	//newItem->setText(0,"0");
    	//newItem->setBackground(1, QBrush(*color_it));
    	//newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
	
	int deltaColour = 255/(sboxNumberOfEntries_2->value()-1);
	int r = 255;
	int b = 0;
	double deltaValue = (maxEdit->text().toDouble()-minEdit->text().toDouble())/(sboxNumberOfEntries_2->value()-1);
	double Val = minEdit->text().toDouble();
	mColormapTreeWidget->clear();
	for (int i = 0 ; i < sboxNumberOfEntries_2->value() ; i++)
	{
		QTreeWidgetItem* newItem = new QTreeWidgetItem(mColormapTreeWidget);
    		newItem->setText(0,QString("%1").arg(Val));
    		Val += deltaValue;
    		QColor *Col = new QColor(r,0,b,255);
    		r -= deltaColour;
    		b += deltaColour;
    		newItem->setBackground(1, QBrush(*Col));
    		newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
	}
}
