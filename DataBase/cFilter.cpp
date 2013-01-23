 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cFilter.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *                : Dirk van der Merwe
 *    Description : Implemetation of Filter Functionality
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

#include "cFilter.h"

//******************************************************************
cFilter::cFilter(): QWidget()
{
	Setup();
	mChangeSite=false;
	mChangeInsts=false;
}

//**********************************************
cFilter::cFilter(QWidget* parent, Qt::WFlags fl)
: QWidget(parent, fl)
{
	Setup();
	connect(this, SIGNAL(Area()), parent->parentWidget(), SLOT(Area()));
	connect(this, SIGNAL(FilterChanged()),parent->parentWidget(),  SLOT(FilterChanged()));
	mChangeSite=false;
	mChangeInsts=false;
}

//***********************************************
cFilter::~cFilter()
{
}

//***********************************************
void cFilter::Setup()
{
	setupUi(this);
	Initialise = true;
	LoadCombos();
	LoadDefaults();
	mChangeInsts=true;
	mChangeSite=true;
	CreateViews();
	pushButtonApplyFilter->setEnabled(false);
	Initialise = false;
}

//***********************************************
void cFilter::LoadCombos()
{
	QString ComboText;
	cDatabase::FieldUiType uiType;
	StringIntArray vals;
	StringIntArrayIterator iterator;
	string setting;
	
	comboBoxTechType->addItem(QString("All"));
	int j=1;
	cout << " In cFilter::LoadCombos() voor Technology " << endl;
	string table="radioinstallation";
	string field="techkey";
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);
	comboBoxProject->addItem(QString("All"));
	j=1;
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		cout << iterator->second << endl;
		comboBoxTechType->addItem(temp);
		j++;
	} // for

	table="site";
	field="status";
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);
	comboBoxStatus->addItem(QString("All"));
	j=1;
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		comboBoxStatus->addItem(temp);
		j++;
	} // for

	table="radioinstallation";
	field="project";
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);
	comboBoxProject->addItem(QString("All"));
	j=1;
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		comboBoxProject->addItem(temp);
		j++;
	} // for
	
	table="radioinstallation";
	field="flagx";
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);
	comboBoxFlagX->addItem(QString("All"));
	j=1;
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		comboBoxFlagX->addItem(temp);
		j++;
	} // for
	
	table="radioinstallation";
	field="flagz";
	uiType = gDb.GetFieldUiType(table,field);
	gDb.GetFieldUiParams(table,field,vals);
	comboBoxFlagZ->addItem(QString("All"));
	j=1;
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::fromStdString(iterator->second);
		comboBoxFlagZ->addItem(temp);
		j++;
	} // for

	string query = "SELECT f_table_name FROM geometry_columns";
	query += " where type='POLYGON' or type='MULTIPOLYGON';";
	pqxx::result r;
	comboBoxAreaType->addItem(QString("Not Used"));
	if (!gDb.PerformRawSql(query))
	{
		comboBoxAreaType->setEnabled(false);
		comboBoxField->setEnabled(false);//
		comboBoxArea->setEnabled(false);
		cout << "Database Select for Polygon geometries failed"<< endl;
		QMessageBox::information(this, "QRap", "Database Select for Polygon geometries failed");
	}
	else
	{
		comboBoxAreaType->setEnabled(true);
		comboBoxField->setEnabled(true);
		comboBoxArea->setEnabled(true);
		gDb.GetLastResult(r);
		if (r.size()==0)
		{
			comboBoxAreaType->setEnabled(false);
			comboBoxField->setEnabled(false);
			comboBoxArea->setEnabled(false);
			cout << "Database Select for Polygon geometries is empty"<< endl;
			QMessageBox::information(this, "QRap", "Database Select for Polygon geometries is empty");			
		}
		else
		{
			for (int i = 0; i < r.size(); i++)
			{
				ComboText = QString(r[i]["f_table_name"].c_str());
				if (ComboText.size()>1)
					comboBoxAreaType->addItem(ComboText);
			}
		}
	}

	comboBoxField->addItem(QString("Not Used"));
	comboBoxArea->addItem(QString("All"));
}

//**************************************************************************
void cFilter::ChangeAlterButton()
{
	pushButtonApplyFilter->setEnabled(true);
}

//********************************************************
void cFilter::LoadDefaults()
{
	string setting;
	int i, num;

	setting = gDb.GetSetting("FilterTechType");
	if(setting!="")
		comboBoxTechType->setCurrentIndex(comboBoxTechType->findText(QString::fromStdString(setting)));
	else comboBoxTechType->setCurrentIndex(comboBoxTechType->findText("All"));
	
	setting = gDb.GetSetting("FilterStatus");
	if(setting!="")
		comboBoxStatus->setCurrentIndex(comboBoxStatus->findText(QString::fromStdString(setting)));
	else comboBoxStatus->setCurrentIndex(comboBoxStatus->findText("All"));
		
	setting = gDb.GetSetting("FilterProject");
	if(setting!="")
		comboBoxProject->setCurrentIndex(comboBoxProject->findText(QString::fromStdString(setting)));
	else comboBoxProject->setCurrentIndex(comboBoxProject->findText("All"));
		
	setting = gDb.GetSetting("FilterFlagX");
	if(setting!="")
		comboBoxFlagX->setCurrentIndex(comboBoxFlagX->findText(QString::fromStdString(setting)));
	else comboBoxFlagX->setCurrentIndex(comboBoxFlagX->findText("All"));
		
	setting = gDb.GetSetting("FilterFlagZ");
	if(setting!="")
		comboBoxFlagZ->setCurrentIndex(comboBoxFlagZ->findText(QString::fromStdString(setting)));
	else comboBoxFlagZ->setCurrentIndex(comboBoxFlagZ->findText("All"));
	
	QString ComboText;
	string query, table, column;
	pqxx::result r;
	table = setting = gDb.GetSetting("FilterAreaType");
	if ((setting!="")&&(setting!="Not Used"))
		comboBoxAreaType->setCurrentIndex(comboBoxAreaType->findText(QString::fromStdString(setting)));
	else comboBoxAreaType->setCurrentIndex(comboBoxAreaType->findText("Not Used"));
}

//***************************************************
void cFilter::on_comboBoxTechType_currentIndexChanged(int index)
{
	if (!Initialise)
	{
		string NewValue = comboBoxTechType->currentText().toStdString();
		gDb.SetSetting("FilterTechType",NewValue);
		cout << "FilterTechType" << "\t\t" << NewValue << endl;

		if ((NewValue!="")&&(NewValue!="All"))
		{
			string query;
			query = "create view antennapattern_view as select distinct antennapattern.* ";
			query += " from antennapattern cross join technology where techkey=technology.id ";
			query += " and technologytype='"+NewValue+"';";

			if (gDb.ViewExists("antennapattern_view"))
				gDb.RemoveView("antennapattern_view");
	
			if (!gDb.PerformRawSql(query))
			{	
				cout << "Error creating radioinstallation_view in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error creating radioinstallation_view in cFilter::CreateViews");
			}
	
			query = "create view equipmenttype_view as select distinct equipmenttype.* ";
			query += " from equipmenttype cross join technology where techkey=technology.id ";
			query += " and technologytype= '"+NewValue+"';";	

			if (gDb.ViewExists("equipmenttype_view"))
				gDb.RemoveView("equipmenttype_view");
	
			if (!gDb.PerformRawSql(query))
			{
				cout << "Error creating equipmenttype_view in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error creating equipmenttype_view in cFilter::CreateViews");
			}
		}
		pushButtonApplyFilter->setEnabled(true);
		mChangeInsts=true;
	}
}

//***************************************************
void cFilter::on_comboBoxStatus_currentIndexChanged(int index)
{
	if (!Initialise)
	{
		QString NewValue = comboBoxStatus->currentText();
		gDb.SetSetting("FilterStatus",NewValue.toStdString());
		cout << "FilterStatus" << "\t\t" << NewValue.toStdString() << endl;
		pushButtonApplyFilter->setEnabled(true);
		mChangeSite=true;
	}
}

//***************************************************
void cFilter::on_comboBoxProject_currentIndexChanged(int index)
{
	if (!Initialise)
	{
		QString NewValue = comboBoxProject->currentText();
		gDb.SetSetting("FilterProject",NewValue.toStdString());
		cout << "FilterProject" << "\t\t" << NewValue.toStdString() << endl;
		pushButtonApplyFilter->setEnabled(true);
		mChangeInsts=true;
	}
}
	
//**************************************************
void cFilter::on_comboBoxFlagX_currentIndexChanged(int index)
{
	if (!Initialise)
	{
		QString NewValue = comboBoxFlagX->currentText();
		gDb.SetSetting("FilterFlagX",NewValue.toStdString());
		cout << "FilterFlagx" << "\t\t" << NewValue.toStdString() << endl;
		pushButtonApplyFilter->setEnabled(true);
		mChangeInsts=true;
	}
}

//**************************************************
void cFilter::on_comboBoxFlagZ_currentIndexChanged(int index)
{
	if (!Initialise)
	{
		QString NewValue = comboBoxFlagZ->currentText();
		gDb.SetSetting("FilterFlagZ",NewValue.toStdString());
		mChangeInsts=true;
		cout << "FilterFlagZ" << "\t\t" << NewValue.toStdString() << endl;
		pushButtonApplyFilter->setEnabled(true);
	}
}

//**************************************************
void cFilter::on_comboBoxAreaType_currentIndexChanged(int index)
{
	string setting;
	QString ComboText;
	string query, table;
	pqxx::result r;
	int i, num;

	QString NewValue = comboBoxAreaType->currentText();
	table = NewValue.toStdString();
	if (!Initialise) gDb.SetSetting("FilterAreaType",table);
	cout << "FilterAreaType" << "\t\t" << table << endl;
	if ((table!="")&&(table!="Not Used"))
	{
		query = "select column_name from information_schema.columns where table_name = '";
		query += table +"' and column_name<>'the_geom' and column_name<>'gid';";
		if (!gDb.PerformRawSql(query))
		{	
			comboBoxField->setEnabled(false);
			comboBoxArea->setEnabled(false);
			cout << "Database Select for column in table containing polygon geometry failed"<< endl;
			QMessageBox::information(this, "QRap", 
				"Database Select for column in table containing polygon geometry failed");
		}
		else
		{
			comboBoxField->setEnabled(true);
			comboBoxArea->setEnabled(true);
			gDb.GetLastResult(r);
			if (r.size()==0)
			{
				comboBoxField->setEnabled(false);
				comboBoxArea->setEnabled(false);
				cout << "Database Select for columns in table containing polygon geometry is empty"<< endl;
				QMessageBox::information(this, "QRap", 
					"Database Select for columns in table containing polygon geometry is empty");			
			}
			else
			{	
				if (!Initialise)
					comboBoxField->setCurrentIndex(comboBoxField->findText("Not Used"));

				num = comboBoxField->count();
				for (i=num-1; i>0; i--)
					comboBoxField->removeItem(i);	
				for (i = 0; i < r.size(); i++)
				{
					ComboText = QString(r[i]["column_name"].c_str());
					if (ComboText.size()>1)
						comboBoxField->addItem(ComboText);
				}
				setting = gDb.GetSetting("FilterAreaField");
				if ((setting!="")&&(setting!="Not Used"))
					comboBoxField->setCurrentIndex(comboBoxField->findText(QString::fromStdString(setting)));	
				else comboBoxField->setCurrentIndex(comboBoxField->findText("Not Used"));
			}	
		}
	}
	else comboBoxField->setCurrentIndex(comboBoxField->findText("Not Used"));
}

//**************************************************
void cFilter::on_comboBoxField_currentIndexChanged(int index)
{
	string setting;
	QString ComboText;
	string query, table, column;
	pqxx::result r;
	int i, num;

	QString NewValue = comboBoxField->currentText();
	table = gDb.GetSetting("FilterAreaType");
	column= NewValue.toStdString();
	if (!Initialise) gDb.SetSetting("FilterAreaField",column);
	cout << "FilterAreaField" << "\t\t" << column << endl;
	if ((NewValue!="")&&(NewValue!="Not Used"))
	{
		query = " select distinct \"" +column +"\" from \"" + table +"\";"; 
		if (!gDb.PerformRawSql(query))
		{	
			comboBoxArea->setEnabled(false);
			cout << "Database Select for areas failed"<< endl;
			QMessageBox::information(this, "QRap", "Database Select for areas failed");
		}
		else
		{	
			cout << "Database Select for areas succeeded"<< endl;
			comboBoxArea->setEnabled(true);
			gDb.GetLastResult(r);
			if (r.size()==0)
			{
				comboBoxArea->setEnabled(false);
//				cout << "Database Select for areas is empty"<< endl;
//				QMessageBox::information(this, "QRap", "Database Select for areas is empty");			
			}
			else
			{
				cout << "Database Select for areas succeeded and is not empty"<< endl;
				if (!Initialise)
					comboBoxArea->setCurrentIndex(comboBoxArea->findText("All"));
				num = comboBoxArea->count();
				for (i=num-1; i>0; i--)
					comboBoxArea->removeItem(i);
				cout << "Database Select for areas succeeded and is not empty 2"<< endl;	
				for (i = 0; i < r.size(); i++)
				{
					ComboText = QString(r[i][0].c_str());
					if (ComboText.size()>1)
						comboBoxArea->addItem(ComboText);
				}
				cout << "Database Select for areas succeeded and is not empty 3"<< endl;
				setting = gDb.GetSetting("FilterArea");
				if ((setting!="")&&(setting!="All"))
					comboBoxArea->setCurrentIndex(comboBoxArea->findText(QString::fromStdString(setting)));
				else comboBoxArea->setCurrentIndex(comboBoxArea->findText("All"));
			}
		}
	}
	else comboBoxArea->setCurrentIndex(comboBoxArea->findText("All"));

}

//**************************************************
void cFilter::on_comboBoxArea_currentIndexChanged(int index)
{
	QString NewValue = comboBoxArea->currentText();
	if (!Initialise) 
	{	
		gDb.SetSetting("FilterArea",NewValue.toStdString());
		cout << "FilterArea" << "\t\t" << NewValue.toStdString() << endl;
		pushButtonApplyFilter->setEnabled(true);
		mChangeSite=true;
	}
}

//*******************************************************
void cFilter::on_pushButtonCreateArea_clicked()
{
	gDb.SetSetting("FilterAreaType","customareafilter");
	gDb.SetSetting("FilterAreaField","areaname");
	LoadDefaults();
	pushButtonApplyFilter->setEnabled(true);
	emit Area();
}

//****************************************************************
void cFilter::on_pushButtonApplyFilter_clicked()
{
	CreateViews();
	pushButtonApplyFilter->setEnabled(false);
	emit FilterChanged();
}
//*****************************************************************
void cFilter::CreateViews() 
{
	string status="All", table="Not Used", column="Not Used";
	string area="All", project="All", flagX="All", flagZ="All", techtype="All";
	string whereclause ="";
	string query = "", queryIN="", queryB="";


	if ((mChangeInsts)||(mChangeSite))
	{

		status = comboBoxStatus->currentText().toStdString();
		table = comboBoxAreaType->currentText().toStdString();
		column= comboBoxField->currentText().toStdString();
		area = comboBoxArea->currentText().toStdString();
		project = comboBoxProject->currentText().toStdString();
		flagX = comboBoxFlagX->currentText().toStdString();
		flagZ = comboBoxFlagZ->currentText().toStdString();
		techtype = comboBoxTechType->currentText().toStdString();

		if ((status!="")&&(status!="All"))
			whereclause+=" status='"+status+"'";
		if ((table!="")&&(table!="Not Used")&&(column!="")&&
			(column!="Not Used")&&(area!="")&&(area!="All"))
		{
			if (whereclause!="")
				whereclause += " and ";
			whereclause += "\""+table+"\".\""+column+"\"='"+area;
			whereclause += "' and location @ the_geom ";
			query += " cross join \""+table+"\" ";

		}
		if (((status!="")&&(status!="All"))||((table!="")&&(table!="Not Used")&&
				(column!="")&&(column!="Not Used")&&(area!="")&&(area!="All")))
			query +=" where "+whereclause +";";
		
		queryB = "create view site_view as select distinct site.* from site "+query; 
		if (gDb.ViewExists("site_view"))
		{
			string drop = "drop view site_view cascade";
			if (!gDb.PerformRawSql(drop))
			{
				cout << "Error dropping site_view in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error dropping site_view in cFilter::CreateViews");
			}
		}
		cout << queryB << endl;
		if (!gDb.PerformRawSql(queryB))
		{
			cout << "Error creating site_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating site_view in cFilter::CreateViews");
		}

		queryB = "create view site_view_list as select distinct site.* from site "+query; 
		if (gDb.ViewExists("site_view_list"))
		{
			string drop = "drop view site_view_list cascade";
			if (!gDb.PerformRawSql(drop))
			{
				cout << "Error dropping site_view_list in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error dropping site_view_list in cFilter::CreateViews");
			}
		}
		if (!gDb.PerformRawSql(queryB))
		{
			cout << "Error creating site_view_list in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating site_view_list in cFilter::CreateViews");
		}

		queryB = "create view site_view_only as select distinct site.* from site "+query; 
		if (gDb.ViewExists("site_view_only"))
		{
			string drop = "drop view site_view_only";
			if (!gDb.PerformRawSql(drop))
			{
				cout << "Error dropping site_view_only in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error dropping site_view_only in cFilter::CreateViews");
			}
		}
		if (!gDb.PerformRawSql(queryB))
		{
			cout << "Error creating site_view_only in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating site_view_only in cFilter::CreateViews");
		}

		bool radinvolved=false;
		query ="";
		whereclause=" siteid=site_view_list.id ";
		if ((techtype!="")&&(techtype!="All"))
		{
			radinvolved=true;
			query +=" cross join technology ";
			whereclause+=" and (techkey=technology.id and technologytype='"+techtype + "') " ;
//			Alter parameter selection
			string navraag;

			if (gDb.ViewExists("cellparameters_view"))
				gDb.RemoveView("cellparameters_view");
			navraag = "create view cellparameters_view as select cellparameters.* from cellparameters cross join technology";
			navraag += " where (techkey=technology.id and technologytype='"+techtype + "'); " ;

			gDb.PerformRawSql(navraag);
		}
		if ((project!="")&&(project!="All"))
		{
			radinvolved=true;
			query +=" cross join projects ";
			whereclause+=" and ((project=projects.id and projectname='"+project + "') " ;
//			whereclause+=" or (project is NULL))";
			whereclause+=" )";
		}
		if ((flagX!="")&&(flagX!="All"))
		{
			radinvolved=true;
			query +=" cross join flagx ";
			whereclause+=" and ((flagx=flagx.id and shortname='"+flagX + "') " ;
//			whereclause+=" or (flagx is NULL))";
			whereclause+=" )";
		}
		if ((flagZ!="")&&(flagZ!="All"))
		{
			radinvolved=true;
			query +=" cross join flagz ";
			whereclause+=" and ((flagz=flagz.id and shortname='"+flagZ + "') " ;
//			whereclause+=" or (flagz is NULL))";
			whereclause+=" )";
		}
		queryIN+=whereclause+");";

		cout << "whereclause=" << whereclause << endl;
		if (radinvolved)
		{
			queryIN = " create view site_view_only as select distinct ";
			queryIN += " site_view_list.id, site_view_list.lastmodified, ";
			queryIN += " site_view_list.machineid, site_view_list.sitename,"; 
			queryIN += " site_view_list.status, site_view_list.groundheight, site_view_list.location ";
			queryIN += " from site_view_list cross join radioinstallation ";
			queryIN += query;
			queryIN +=" where "+whereclause;
//			queryIN += " union select * from site_view_list ";
//			queryIN +=" where site_view_list.id not in (select distinct siteid from radioinstallation)";
			queryIN += ";";
	
			if (gDb.ViewExists("site_view_only"))
				gDb.RemoveView("site_view_only");
		
			if (!gDb.PerformRawSql(queryIN))
			{
				cout << "Error creating site_view_only in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error creating site_view_only in cFilter::CreateViews");
			}	


			queryIN = " create view site_view as select distinct ";
			queryIN += " site_view_list.id, site_view_list.lastmodified, ";
			queryIN += " site_view_list.machineid, site_view_list.sitename,"; 
			queryIN += " site_view_list.status, site_view_list.groundheight, site_view_list.location ";
			queryIN += " from site_view_list cross join radioinstallation ";
			queryIN += query;
			queryIN +=" where "+whereclause;
//			queryIN += " union select * from site_view_list ";
//			queryIN +=" where site_view_list.id not in (select distinct siteid from radioinstallation)";
			queryIN += ";";
			if (gDb.ViewExists("site_view"))
			{
				string drop = "drop view site_view cascade";
				if (!gDb.PerformRawSql(drop))
				{
					cout << "Error dropping site_view in cFilter::CreateViews" << endl;
					QRAP_ERROR("Error dropping site_view in cFilter::CreateViews");
				}
			}
			
			if (!gDb.PerformRawSql(queryIN))
			{
				cout << "Error creating site_view in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error creating site_view in cFilter::CreateViews");
			}		
		}
	 
		queryIN = " create view radioinstallation_view as select distinct site_view_list.sitename as sitename, ";
		queryIN += " sitename||sector as sectorname, ";
		queryIN +="radioinstallation.* from radioinstallation cross join site_view_list ";
		if (radinvolved) 
		{
			queryIN += query;
			queryIN +="where radioinstallation.siteid=site_view_list.id and ";
			queryIN += whereclause+";";
		}
		else queryIN += "where radioinstallation.siteid=site_view_list.id; ";;

		cout << "RadInst queryIN = " << queryIN << endl; 
		if (gDb.ViewExists("radioinstallation_view"))
		{
			string drop = "drop view radioinstallation_view cascade";
			if (!gDb.PerformRawSql(drop))
			{
				cout << "Error dropping radioinstallation_view in cFilter::CreateViews" << endl;
				QRAP_ERROR("Error dropping radioinstallation_view in cFilter::CreateViews");
			}
		}
		
		if (!gDb.PerformRawSql(queryIN))
		{
			cout << "Error creating radioinstallation_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating radioinstallation_view in cFilter::CreateViews");
		}
		
		//change cells view
		query = "create view cell_view as select distinct sectorname, cell.id||sectorname as cellname,";
		query += " cell.* from cell cross join radioinstallation_view";
		query += " where cell.risector = radioinstallation_view.id; ";
		
		if (gDb.ViewExists("cell_view"))
			gDb.RemoveView("cell_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating cell_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating cell_view in cFilter::CreateViews");
		}
		
		//change frequency allocation view
		query = "create view frequencyallocationlist_view as ";
		query+= "select distinct frequencyallocationlist.id as id,cellname,";
		query += " frequencyallocationlist.ci as ci, ";
		query += " frequencyallocationlist.carrier as carrier, ";
		query += " frequencyallocationlist.channel as channel ";
		query += " from frequencyallocationlist, cell_view";
		query += " where frequencyallocationlist.ci=cell_view.id;";
		
		if (gDb.ViewExists("frequencyallocationlist_view"))
			gDb.RemoveView("frequencyallocationlist_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating frequencyallocationlist_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating frequencyallocationlist_view in cFilter::CreateViews");
		}
		
		//change parametersettings view
		query = "create view parametersettings_view as ";
		query+= "select distinct parametersettings.id as id,cellname,";
		query += " parametersettings.ci as ci, ";
		query += " parameterid, parametername as cell_parameter, setting ";
		query += " from parametersettings, cell_view, cellparameters_view ";
		query += " where parametersettings.ci=cell_view.id and ";
		query += " parametersettings.parameterid=cellparameters_view.id;";
		
		if (gDb.ViewExists("parametersettings_view"))
			gDb.RemoveView("parametersettings_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating parametersettings_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating parametersettings_view in cFilter::CreateViews");
		}

		//change antenna_view
		query = " create view antenna_view as select sitename,";
		query += " antenna.* from antenna, radioinstallation_view"; 
		query += " where antenna.rikey=radioinstallation_view.id;";
	
		if (gDb.ViewExists("antenna_view"))
			gDb.RemoveView("antenna_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating antenna_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating antenna_view in cFilter::CreateViews");
		}
						
		//change equipment view
		query = "create view equipment_view as select equipment.id as id, sitename, ";
		query += " equipment.serialnumber as serialnumber, ";
		query += " equipment.rikey as rikey, ";
		query += " equipment.equipkey as equipkey, ";
		query += " equipment.installationdate as installationdate ";
		query += " from equipment, radioinstallation_view "; 
		query += " where equipment.rikey=radioinstallation_view.id;";
/*		query += " union ";
		query += " select equipment.id as id, ";
		query += " null as sitename, ";
		query += " equipment.serialnumber as serialnumber,";
		query += " equipment.rikey as rikey,";
		query += " equipment.equipkey as equipkey,";
		query += " equipment.installationdate as installationdate";
		query += " from equipment where equipment.rikey is null;";
*/		
		if (gDb.ViewExists("equipment_view"))
			gDb.RemoveView("equipment_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating equipment_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating equipment_view in cFilter::CreateViews");
		}

		//change links view
		query = "create view links_view as select linkname, ";
		query+= "site1.sitename||rad1.sector as txsite, txinst, ";
		query+= "rad1.txbearing as txbearing, ";
		query+= "site2.sitename||rad2.sector as rxsite, rxinst, ";
		query+= "rad2.txbearing as rxbearing, ";
		query+= "ST_Distance(site1.location, site2.location, true) as Distance, ";
		query+= "minclearance as clear, ";
		query+= "frequency, pathloss, kfactor, line ";
		query+= "from links cross join radioinstallation as rad1 cross join site_view_only as site1 ";
	 	query+= "cross join radioinstallation as rad2 cross join site_view_only as site2 ";
	 	query+= "where rad1.id=txinst ";
		query+= "and rxinst=rad2.id ";
		query+= "and rad1.siteid=site1.id ";
		query+= "and rad2.siteid=site2.id;";
		
		if (gDb.ViewExists("links_view"))
			gDb.RemoveView("links_view");
		
		if (!gDb.PerformRawSql(query))
		{
			cout << "Error creating links_view in cFilter::CreateViews" << endl;
			QRAP_ERROR("Error creating links_view in cFilter::CreateViews");
		}

		radinvolved=false;
					
	}
	
	mChangeInsts=false;
	mChangeSite=false;
	
}


