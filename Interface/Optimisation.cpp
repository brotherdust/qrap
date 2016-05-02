/*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Optimisation.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Gabriel de Sousa (gdesousa26@gmail.com) 
 *		  : Magdaleen Ballot (magdaleen.ballot@up.ac.za) 
 *    Description : Implementation for link optimisation for a selected area 
 *		    of sites in terms of monetary cost
 *
 ***************************************************************************
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

#include <qt4/QtGui/qabstractbutton.h>
#include <qt4/QtCore/qstring.h>
#include <QMessageBox>
#include <cfloat>
#include <algorithm>

#include "Optimisation.h"

using namespace std;


//*********************************************************************************************
// Default Constructor
//*********************************************************************************************

Optimisation::Optimisation() : QDialog()
{
    setupUi(this);
    this->setModal(true);
    LoadDefaults();
}

//*********************************************************************************************
// Constructor
//*********************************************************************************************
Optimisation::Optimisation(QWidget* parent, Qt::WFlags fl)
: QDialog(parent, fl)
{
    setupUi(this);
    this->setModal(true);
    LoadDefaults();
    mFL = fl;
    mParent = parent;
    
    tstart = time(0);
}

//*********************************************************************************************
// Destructor
//*********************************************************************************************
Optimisation::~Optimisation() 
{
 
}

///*********************************************************************************************
// Loads the default values and the database information onto the GUI display
//*********************************************************************************************
void Optimisation::LoadDefaults()
{
    int j = 0;
    string query;
    QString err = "";
    pqxx::result r;

    pushOk->setText("Ok");
    pushCancel->setText("Cancel");
    pushOk->setEnabled(true);
    pushCancel->setEnabled(true);

    checkBox->setCheckState(Qt::Unchecked);
    chkClearance->setCheckState(Qt::Checked);
    chkStrength->setCheckState(Qt::Checked);
    chkPower->setCheckState(Qt::Unchecked);
    chkSignalStrength->setCheckState(Qt::Unchecked);
    chkSensitivity->setCheckState(Qt::Unchecked);

    QString Dir = (QString) (gDb.GetSetting("OutputDir").c_str());
    lineEdit->setText(Dir);

    connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(checkBoxChanged(int)));
    connect(chkClearance, SIGNAL(stateChanged(int)), this, SLOT(chkClearanceChanged(int)));
    connect(chkStrength, SIGNAL(stateChanged(int)), this, SLOT(chkStrengthChanged(int)));
    connect(chkSignalStrength, SIGNAL(stateChanged(int)), this, SLOT(chkSignalStrengthChanged(int)));
    connect(chkPower, SIGNAL(stateChanged(int)), this, SLOT(chkPowerChanged(int)));
    connect(chkSensitivity, SIGNAL(stateChanged(int)), this, SLOT(chkSensitivityChanged(int)));


	// for the selected technology
    query = "SELECT id,technologytype from technology;";
    if ( !gDb.PerformRawSql(query) )
    {
        err = "Database Select for technology in Optimisation::Optimisation() failed";
        QRAP_WARN(err.toStdString().c_str());
        QMessageBox::information(this, "QRap", err);
    }
    else
    {
        gDb.GetLastResult(r);
        if ( r.size() > 0 )
        {
            for ( int i = 0; i < r.size(); i++ )
            {
                cbTechnology->insertItem(j, r[i]["technologytype"].c_str());
                j++;
            }
        }
        else
        {
            if ( j == 0 ) cbTechnology->insertItem(0, "0:NULL");
            err = "You need to define at least one technology with a default site-installation";
            QRAP_WARN(err.toStdString().c_str());
            QMessageBox::information(this, "QRap", err);
        }
    }

	//for the selected project
    query = "SELECT id,projectname from projects;";
    if ( !gDb.PerformRawSql(query) )
    {
        err = "Database Select for projects in Optimisation::Optimisation() failed";
        cout << err.toStdString().c_str() << " Query: " << query;
        QRAP_WARN(err.toStdString().c_str());
    }
    else
    {
        j = 0;
        gDb.GetLastResult(r);
        if ( r.size() > 0 )
        {
            for ( int i = 0; i < r.size(); i++ )
            {
                cbProject->insertItem(j, r[i]["projectname"].c_str());
                j++;
            }
        }
        else
        {
            if ( j == 0 ) cbTechnology->insertItem(0, "0:NULL");
            err = "Project list is empty";
            QRAP_WARN(err.toStdString().c_str());
        }
    }
    cbMinClear->insertItem(0,"0");
    cbMinClear->insertItem(1,"60");
    cbMinClear->insertItem(2, "100");
    dsbMaxPath->setValue(130.0);
    dsbMaxDist->setValue(20.0);

    string query2;
    pqxx::result DataRates;

    query2 = "SELECT data_rate";
    query2 += " FROM receiver_capabilities;";

    if ( !gDb.PerformRawSql(query2) )
    {
        cout << "Database Select on receiver capabilities table failed in Optimisation::LoadDefaults" << endl;
        string err = "Database Select on receiver capabilities table failed";
        QRAP_WARN(err.c_str());
    }
    gDb.GetLastResult(DataRates);

    for ( int i = 0; i < DataRates.size(); i++ )
    {
        cbDatarate->insertItem(i, DataRates[i]["data_rate"].c_str());
    }

    cbMinLinks->insertItem(0, "1");
    cbMinLinks->insertItem(1, "2");
    cbMinLinks->insertItem(2, "3");
    cbMinLinks->insertItem(3, "4");

    dsbKfactor->setValue(1.01);
    dsbMinMastheight->setValue(3.0);
    dsbMaxMastheight->setValue(20.0);
    
    dsbPower->setValue(20);
    dsbSensitivity->setValue(-96);
    dsbSignalStrength->setValue(-60);
    
    dsbSignalStrength->setVisible(false);
    dsbSensitivity->setVisible(false);
    dsbPower->setVisible(false);
}


//*********************************************************************************************
// Checkbox toggle action for save links PDF
//*********************************************************************************************
void Optimisation::checkBoxChanged(int state)
{
    if ( state == 2 )
    {
        lineEdit->setVisible(true);
        lineEdit->setEnabled(true);
    }
    else if ( state == 0 )
    {
        lineEdit->setVisible(false);
        lineEdit->setEnabled(false);
    }
}

//*********************************************************************************************
// Checkbox toggle action for min clearance
//*********************************************************************************************
void Optimisation::chkClearanceChanged(int state)
{
    if ( state == 2 )
    {
        cbMinClear->setVisible(true);
        dsbKfactor->setVisible(true);
    }
    else if ( state == 0 )
    {
        cbMinClear->setVisible(false);
        dsbKfactor->setVisible(false);
    }
}

//*********************************************************************************************
// Checkbox toggle action for distance and path loss
//*********************************************************************************************
void Optimisation::chkStrengthChanged(int state)
{
    if ( state == 2 )
    {
        dsbMaxPath->setVisible(true);
        dsbMaxDist->setVisible(true);
    }
    else if ( state == 0 )
    {
        dsbMaxPath->setVisible(false);
        dsbMaxDist->setVisible(false);
    }
}

//*********************************************************************************************
// Checkbox toggle action for signal strength
//*********************************************************************************************
void Optimisation::chkSignalStrengthChanged(int state)
{
    if ( state == 2 )
        dsbSignalStrength->setVisible(true);
    else if ( state == 0 )
        dsbSignalStrength->setVisible(false);
}

//*********************************************************************************************
// Checkbox toggle action for txpower
//*********************************************************************************************
void Optimisation::chkPowerChanged(int state)
{
    if ( state == 2 )
        dsbPower->setVisible(true);
    else if ( state == 0 )
        dsbPower->setVisible(false);
}

//*********************************************************************************************
// Checkbox toggle action for rxsensitivity
//*********************************************************************************************
void Optimisation::chkSensitivityChanged(int state)
{
    if ( state == 2 )
        dsbSensitivity->setVisible(true);
    else if ( state == 0 )
        dsbSensitivity->setVisible(false);
}

//*********************************************************************************************
// Check to see if there are any sites in the selected polygon area and load their information
//*********************************************************************************************
bool Optimisation::SetPoints(QList<QgsPoint> Points)
{
    string err = "";
    char *text = new char[33];
    if ( Points.size() > 1 )
    {
        double North = Points[0].y();
        double South = Points[0].y();
        double East = Points[0].x();
        double West = Points[0].x();
        string query, query2;
        pqxx::result SitesIn;
        pqxx::result MastInfo;
        
        query2 = "SELECT siteid, existing_mast, min_mast_height, max_mast_height";
        query2 += " FROM sitedescription;";

        if ( !gDb.PerformRawSql(query2) )
        {
            cout << "Database Select on site description table failed in Optimisation::SetPoints" << endl;
            string err = "Database Select on site description table failed";
            QRAP_WARN(err.c_str());
            delete [] text;
            return false;
        }
        gDb.GetLastResult(MastInfo);

        query = "SELECT id, sitename, ST_AsText(location) AS location, status";
        query += " FROM site";
        query += " WHERE location ";
        query += "@ ST_GeomFromText('POLYGON((";
        for ( int i = 0; i < Points.size(); i++ )
        {
            North = max(North, Points[i].y());
            South = min(South, Points[i].y());
            East = max(East, Points[i].x());
            West = min(West, Points[i].x());
            //			sprintf (text, "%f", Points[i].x());
            //			cout << text << endl;
            gcvt(Points[i].x(), 10, text);
            query += text;
            query += " ";
            //			sprintf (text, "%f", Points[i].y());
            gcvt(Points[i].y(), 10, text);
            query += text;
            query += ",";
        }
        gcvt(Points[0].x(), 10, text);
        query += text;
        query += " ";
        gcvt(Points[0].y(), 10, text);
        query += text;
        query += "))',4326);";

        if ( !gDb.PerformRawSql(query) )
        {
            cout << "Database Select on sites table failed in Optimisation::SetPoints" << endl;
            string err = "Database Select on sites table failed";
            QRAP_WARN(err.c_str());
            delete [] text;
            return false;
        }
        gDb.GetLastResult(SitesIn);

        int siteNum;
        if ( SitesIn.size() > 0 )
        {
            tableInstallations->setRowCount(SitesIn.size());
            for ( int i = 0; i < SitesIn.size(); i++ )
            {
                siteNum = -1;
                    
                for ( int j = 0; j < MastInfo.size(); j++ )
                {
                    if ( strcmp(MastInfo[j]["siteid"].c_str(),SitesIn[i]["id"].c_str()) == 0 )
                    {
                        siteNum = j;
                        break;
                    }
                }
                    
                QCheckBox *Enabled = new QCheckBox(SitesIn[i]["sitename"].c_str());
                Enabled->setCheckState(Qt::Checked);
                tableInstallations->setCellWidget(i, 0, Enabled);
                tableInstallations->setItem(i, 1, new QTableWidgetItem(SitesIn[i]["id"].c_str()));
                tableInstallations->setItem(i, 2, new QTableWidgetItem(SitesIn[i]["status"].c_str()));
                if ( siteNum == -1 )
                {
                    tableInstallations->setItem(i, 3, new QTableWidgetItem("False"));
                    tableInstallations->setItem(i, 4, new QTableWidgetItem(0));
                    tableInstallations->setItem(i, 5, new QTableWidgetItem(0));
                }
                else
                {
                    if(strcmp(MastInfo[siteNum]["existing_mast"].c_str(),"t") == 0)
                        tableInstallations->setItem(i, 3, new QTableWidgetItem("True"));
                    else
                        tableInstallations->setItem(i, 3, new QTableWidgetItem("False"));
                    
                    if(strcmp(MastInfo[siteNum]["min_mast_height"].c_str(),"0") == 0)
                        tableInstallations->setItem(i, 4, new QTableWidgetItem(0));
                    else
                        tableInstallations->setItem(i, 4, new QTableWidgetItem(MastInfo[siteNum]["min_mast_height"].c_str()));
                    
                    if(strcmp(MastInfo[siteNum]["max_mast_height"].c_str(),"0") == 0)
                        tableInstallations->setItem(i, 5, new QTableWidgetItem(0));
                    else
                        tableInstallations->setItem(i, 5, new QTableWidgetItem(MastInfo[siteNum]["max_mast_height"].c_str()));
                }
            }
        }
        else
        {
            string err = "No Sites in selected area! ";
            //QRAP_WARN(err.c_str());
            QMessageBox::warning(this, "QRap", err.c_str());
            delete [] text;
            return false;
        }
        delete [] text;
        return true;
    }
    delete [] text;
    return false;
}

//*********************************************************************************************
// Load the cables from the database
//*********************************************************************************************
bool Optimisation::LoadCables()
{
    string err = "";
    string query;
    pqxx::result Cables;

    query = "SELECT id, cabletype, cost_per_metre, signal_loss_per_metre";
    query += " FROM cable_type;";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on cable_type table failed in Optimisation::LoadCables" << endl;
        string err = "Database Select on cable_type table failed";
        QRAP_WARN(err.c_str());
        return false;
    }
    gDb.GetLastResult(Cables);
    if ( Cables.size() > 0 )
    {
        tableCables->setRowCount(Cables.size());
        for ( int i = 0; i < Cables.size(); i++ )
        {
            QCheckBox *Enabled = new QCheckBox(Cables[i]["cabletype"].c_str());
            Enabled->setCheckState(Qt::Checked);
            tableCables->setCellWidget(i, 0, Enabled);
            tableCables->setItem(i, 1, new QTableWidgetItem(Cables[i]["id"].c_str()));
            tableCables->setItem(i, 2, new QTableWidgetItem(Cables[i]["cost_per_metre"].c_str()));
            tableCables->setItem(i, 3, new QTableWidgetItem(Cables[i]["signal_loss_per_metre"].c_str()));
        }
    }
    else
    {
        string err = "No Cable types stored in the database ";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
        return false;
    }
    return true;
}

//*********************************************************************************************
// Load the connectors from the database
//*********************************************************************************************
bool Optimisation::LoadConnectors()
{
    string err = "";
    string query;
    pqxx::result Connectors;

    query = "SELECT id, combiner_splitter_type, cost, losses, description";
    query += " FROM combiner_splitter_type;";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on combiner_splitter_type table failed in Optimisation::LoadConnectors" << endl;
        string err = "Database Select on combiner_splitter_type table failed";
        QRAP_WARN(err.c_str());
        return false;
    }
    gDb.GetLastResult(Connectors);
    if ( Connectors.size() > 0 )
    {
        tableConnectors->setRowCount(Connectors.size());
        for ( int i = 0; i < Connectors.size(); i++ )
        {
            QCheckBox *Enabled = new QCheckBox(Connectors[i]["description"].c_str());
            Enabled->setCheckState(Qt::Checked);
            tableConnectors->setCellWidget(i, 0, Enabled);
            tableConnectors->setItem(i, 1, new QTableWidgetItem(Connectors[i]["id"].c_str()));
            tableConnectors->setItem(i, 2, new QTableWidgetItem(Connectors[i]["combiner_splitter_type"].c_str()));
            tableConnectors->setItem(i, 3, new QTableWidgetItem(Connectors[i]["cost"].c_str()));
            tableConnectors->setItem(i, 4, new QTableWidgetItem(Connectors[i]["losses"].c_str()));
        }
    }
    else
    {
        string err = "No Combiner/Splitter types stored in the database ";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
        return false;
    }
    return true;
}

//*********************************************************************************************
// Load the antennas from the database
//*********************************************************************************************
bool Optimisation::LoadAntennas()
{
    string err = "";
    string query;
    pqxx::result Antennas;

    query = "SELECT antennapattern.id, antennadevice.devicename, antennadevice.manufacturer, antennadevice.cost, antennapattern.gain";
    query += " FROM antennapattern cross join  antennadevice";
    query += " WHERE antennadevice.id=antennapattern.antdevicekey;";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on antennadevice table failed in Optimisation::LoadAntennas" << endl;
        string err = "Database Select on antenna table failed";
        QRAP_WARN(err.c_str());
        return false;
    }
    gDb.GetLastResult(Antennas);
    if ( Antennas.size() > 0 )
    {
        tableAntennas->setRowCount(Antennas.size());
        for ( int i = 0; i < Antennas.size(); i++ )
        {
            QCheckBox *Enabled = new QCheckBox(Antennas[i]["devicename"].c_str());
            Enabled->setCheckState(Qt::Checked);
            tableAntennas->setCellWidget(i, 0, Enabled);
            tableAntennas->setItem(i, 1, new QTableWidgetItem(Antennas[i]["id"].c_str()));
            tableAntennas->setItem(i, 2, new QTableWidgetItem(Antennas[i]["manufacturer"].c_str()));
            tableAntennas->setItem(i, 3, new QTableWidgetItem(Antennas[i]["cost"].c_str()));
            tableAntennas->setItem(i, 4, new QTableWidgetItem(Antennas[i]["gain"].c_str()));
        }
    }
    else
    {
        string err = "No Antennas stored in the database ";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
        return false;
    }
    return true;
}

//*********************************************************************************************
// Cancel button clicked on GUI
//*********************************************************************************************
void Optimisation::on_pushCancel_clicked()
{
    close();
}

//*********************************************************************************************
// OK button clicked on GUI, call optimise function
//*********************************************************************************************
void Optimisation::on_pushOk_clicked()
{
    // Save working links directory
    SavePDF = false;
    path = lineEdit->text();
    path += "/";
    QString file = path;
    if ( checkBox->isChecked() )
        SavePDF = true;

    // Max Path Loss + Maximum link distance
    if ( chkStrength->isChecked() )
    {
        mMaxPath = dsbMaxPath->value();
        mMaxDist = dsbMaxDist->value();
    }
    else
    {
        mMaxPath = 99999999;
        mMaxDist = 99999999;
    }

    // Min Clearance + kFactor
    if ( chkClearance->isChecked() )
    {
        mMinClear = atof(cbMinClear->currentText().latin1()) / 100.0;
        mkFactor = dsbKfactor->value();
    }
    else
    {
        mMinClear = -999999;
        mkFactor = 1.33;
    }

    //Technology + Project + Min Links
    mTechnology = cbTechnology->currentText().latin1();
    mProjectName = cbProject->currentText().latin1();

    mMinNumLinks = cbMinLinks->currentText().latin1();
    if ( mMinNumLinks == "1" )
        mMinLinks = 1;
    else if ( mMinNumLinks == "2" )
        mMinLinks = 2;
    else if ( mMinNumLinks == "3" )
        mMinLinks = 3;
    else
        mMinLinks = 4;

    //No existing mast boundary values
    mMinHeightN = dsbMinMastheight->value();
    mMaxHeightN = dsbMaxMastheight->value();

    //Power + Sensitivity + Data Rate
    mRate = cbDatarate->currentText().latin1();

    if ( chkPower->isChecked() )
        mPower = dsbPower->value();
    else
        mPower = getPower(mRate);

    if ( chkSensitivity->isChecked() )
        mSensitivity = dsbSensitivity->value();
    else
        mSensitivity = getSensitivity(mRate);

    if ( chkSignalStrength->isChecked() )
        mSignalStrength = dsbSignalStrength->value();
    else
        mSignalStrength = -9999999;
    
    int site = 0;
    int ant = 0;
    int cab = 0;
    int comb = 0;
    int split = 0;
    
    bool Success = true;

    //Get selected sites
    for ( int i = 0; i < tableInstallations->rowCount(); i++ )
    {
        void *Wid = tableInstallations->cellWidget(i, 0);
        QCheckBox *get = (QCheckBox*) Wid;
        if ( get->isChecked() )
        {
            site++;
            mSiteList.push_back((unsigned) tableInstallations->item(i, 1)->text().toDouble());
        }
    }
    
    if(site < 2)
    {
        Success = false;
        string err = "Not enough sites selected";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
    }

    //Get selected antennas
    for ( int i = 0; i < tableAntennas->rowCount(); i++ )
    {
        void *Wid = tableAntennas->cellWidget(i, 0);
        QCheckBox *get = (QCheckBox*) Wid;
        if ( get->isChecked() )
        {
            ant++;
            mAntennaList.push_back((unsigned) tableAntennas->item(i, 1)->text().toDouble());
        }
    }
    
    if(ant == 0)
    {
        Success = false;
        string err = "No antennas selected";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
    }

    //Get selected cables
    for ( int i = 0; i < tableCables->rowCount(); i++ )
    {
        void *Wid = tableCables->cellWidget(i, 0);
        QCheckBox *get = (QCheckBox*) Wid;
        if ( get->isChecked() )
        {
            cab++;
            mCableList.push_back((unsigned) tableCables->item(i, 1)->text().toDouble());
        }
    }
    
    if(cab == 0)
    {
        Success = false;
        string err = "No cables selected";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
    }
    
    //Get selected connectors
    for ( int i = 0; i < tableConnectors->rowCount(); i++ )
    {
        void *Wid = tableConnectors->cellWidget(i, 0);
        QCheckBox *get = (QCheckBox*) Wid;
        if ( get->isChecked() )
        {
            if(tableConnectors->item(i, 2)->text() == "Combiner")
                comb++;
            else
                split++;
            mConnectorList.push_back((unsigned) tableConnectors->item(i, 1)->text().toDouble());
        }
    }

    if(comb == 0)
    {
        Success = false;
        string err = "No combiners selected";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
    }

    if(split == 0)
    {
        Success = false;
        string err = "No splitters selected";
        QRAP_WARN(err.c_str());
        QMessageBox::warning(this, "QRap", err.c_str());
    }    
    
    bool heightsExist = true;
    
    for (int t = mMinHeightN ; t < mMaxHeightN+1 ; t++)
    {
        if(getHeightCost(t) == -1.0)
            heightsExist = false;
    }
    
    if (!heightsExist)
    {
        string err = "All costs do not exist for specified height range.";
        //QRAP_WARN(err.c_str());
        Success = false;
        QMessageBox::warning(this, "QRap", err.c_str());
    }
    
    pqxx::result r;

    // Check for a default radio installation for the specified technology
    string query = "SELECT technology.id as techkey, eirp, ";
    query += " radioinstallation.txpower,radioinstallation.txlosses,";
    query += " radioinstallation.rxlosses,radioinstallation.rxsensitivity,btlfreq, ";
    query += "txantpatternkey,txbearing,txmechtilt,txantennaheight, ";
    query += "rxantpatternkey,rxbearing,rxmechtilt,rxantennaheight ";
    query += "FROM radioinstallation cross join site cross join technology ";
    query += "WHERE radioinstallation.siteid=site.id and defaultsite=site.id ";
    query += "and technologytype ='";
    query += mTechnology;
    query += "';";
    mInst.sSiteID = 0;

    if ( !gDb.PerformRawSql(query) )
    {
        Success = false;
        QString err = "There seems to be an error with the query to get the default";
        err += "radio installation  for the technology.";
        cout << "Query Failed: " << query.c_str() << endl;
        QRAP_ERROR(err.toStdString().c_str());
        QMessageBox::information(this, "QRap", err);
        close();
    } // if
    else
    {
        gDb.GetLastResult(r);

        // store all the values of the default site for the specified technology in mInst
        int size = r.size();
        if ( size > 0 )
        {
            mTechKey = atoi(r[0]["techkey"].c_str());
            mInst.sEIRP = atof(r[0]["eirp"].c_str());
            mInst.sTxPower = mPower;
            mInst.sTxSysLoss = atof(r[0]["txlosses"].c_str());
            mInst.sRxSysLoss = atof(r[0]["rxlosses"].c_str());
            mInst.sRxSens = mSensitivity;
            mInst.sTxPatternKey = atoi(r[0]["txantpatternkey"].c_str());
            mInst.sTxAzimuth = atof(r[0]["txbearing"].c_str());
            mInst.sTxMechTilt = atof(r[0]["txmechtilt"].c_str());
            mInst.sRxPatternKey = atoi(r[0]["rxantpatternkey"].c_str());
            mInst.sRxAzimuth = atof(r[0]["rxbearing"].c_str());
            mInst.sRxMechTilt = atof(r[0]["rxmechtilt"].c_str());
            mInst.sTxHeight = atof(r[0]["txantennaheight"].c_str());
            mInst.sRxHeight = atof(r[0]["rxantennaheight"].c_str());
            mInst.sFrequency = atof(r[0]["btlfreq"].c_str());
        }
        else
        {
            QString err = "There seems to be no default site installation defined for the technology. ";
            QRAP_WARN(err.toStdString().c_str());
            cout << "Query Empty: " << query.c_str() << endl;
            QMessageBox::information(this, "QRap", err);
            Success = false;
            close();
        }
    }         

    // get the id of the specified project
    query = "SELECT id from projects where projectname = '";
    query += cbProject->currentText().latin1();
    query += "'";
    if ( !gDb.PerformRawSql(query) )
    {
        string err = "There seems to be an error with the query to get the project";
        err += query;
        mProject = 0;
        QRAP_WARN(err.c_str());
    } 
    else
    {
        gDb.GetLastResult(r);

        int size = r.size();
        if ( size > 0 )
            mProject = atoi(r[0]["id"].c_str());
        else mProject = 0;
    }

    if(Success)
    {
        pushOk->setEnabled(false);
        pushCancel->setEnabled(false);
        pushOk->setText("busy");
        pushCancel->setText("busy");
        optimise();
    }
}

//*********************************************************************************************
// Initialisation of all variables with values from GUI and call optimisation functions
//*********************************************************************************************
void Optimisation::optimise()
{
    // Initialise plot settings
    Units = dBm;
    PlotResolution = atof(gDb.GetSetting("PlotResolution").c_str());
    tempPlotRes = PlotResolution;
    Down = gDb.GetSetting("Downlink");

    if ( Down == "false" )
        DownLink = false;
    else DownLink = true;

    Clutter = gDb.GetSetting("UseClutter");
    UseClutter = false;
    if ( Clutter == "true" )
        UseClutter = true;

    ClutterSource = atoi(gDb.GetSetting("ClutterSource").c_str());
    DEMsource = atoi(gDb.GetSetting("DEMsource").c_str());
    Transmitter = "Transmitter";
    Receiver = "Receiver";

    size = mSiteList.size();
    double antennaCosts[mAntennaList.size()];
    double cableCosts[mCableList.size()];
    double signalLosses[mCableList.size()];
    int antennasUsed[mAntennaList.size()];

    numLinks = 0; 
    int num = size - 1;
    while ( num > 0 )
    {
        numLinks += num;
        num--;
    }

    linkDetails links[numLinks];
    siteDetails sites[size];
    
    numCombiners = 0;
    numSplitters = 0;
    bool connectorList[mConnectorList.size()];
    string query = "";
    pqxx::result Connectors;
    
    for ( int i = 0; i < mConnectorList.size(); i++ )
    {
        QString str3 = QString::number(mConnectorList[i]);
        query = "SELECT combiner_splitter_type FROM combiner_splitter_type WHERE id = ";
        query += str3.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(Connectors);
        
        if(strcmp(Connectors[0]["combiner_splitter_type"].c_str(),"Combiner") == 0)
        {
            numCombiners++;
            connectorList[i] = false;
        }
        else
        {
            numSplitters++;
            connectorList[i] = true;
        }
    }
    
    double combinerLosses[numCombiners];
    double splitterLosses[numSplitters];
    double combinerCosts[numCombiners];
    double splitterCosts[numSplitters];

    LoadCosts(antennaCosts, cableCosts, connectorList, combinerCosts, splitterCosts, signalLosses, combinerLosses, splitterLosses, sites);
    
    currAntID = 0;
    currCabID = 0;
    currComID = 0;
    currSplitID = 0;
    totalCost = 0;
    currLinkCost = 0;
    minCost = 0;

    for ( int n = 0; n < mAntennaList.size(); n++ )
    {
        antennasUsed[n] = 0;
    }

    for ( int n = 0; n < size; n++ )
    {
        sites[n].served = false;
        sites[n].feasibleLinks = 0;
        sites[n].savedLinks = 0;
        sites[n].availableLinks = 0;
    }

    currLinkCost = FLT_MAX;

    //get cheapest antenna/cable pair
    for ( int z = 0; z < mAntennaList.size(); z++ )
    {
        if ( antennasUsed[z] == 0 )
        {
            for ( int v = 0; v < mCableList.size(); v++ )
            {
                for( int y = 0 ; y < numCombiners ; y++)
                {
                    for( int u = 0 ; u < numSplitters ; u++)
                    {
                        if ( (antennaCosts[z] + cableCosts[v] + combinerCosts[y] + splitterCosts[u]) < currLinkCost )
                        {
                            currLinkCost = antennaCosts[z] + cableCosts[v] + combinerCosts[y] + splitterCosts[u];
                            currAntID = z;
                            currCabID = v;
                            currComID = y;
                            currSplitID = u;
                            minCost = combinerCosts[y] + splitterCosts[u] + 2*antennaCosts[z] + 2*cableCosts[v]*(mMinHeightN+5) + 2*getHeightCost(mMinHeightN);
                        }
                    }
                }
            }
        }
    }

    mInst.sTxPatternKey = mAntennaList[currAntID];
    mInst.sRxPatternKey = mAntennaList[currAntID];
    
    for (int j = 0 ; j < numLinks ; j++)
    {
            links[j].unservedReason = -1;
            links[j].transmitter = -1;
            links[j].receiver = -1;
            links[j].linkNumber = -1;
            links[j].rxheight = 0;
            links[j].txheight = 0;
            links[j].antID = currAntID;
            links[j].cabID = currCabID;
            links[j].comID = currComID;
            links[j].splitID = currSplitID;
            links[j].saved = false;
            links[j].required = false;
            links[j].cost = 0.0;
            links[j].clearance = 0.0;
            links[j].pathloss = 0.0;
            links[j].signalstrength = 0.0;
            links[j].distance = 0.0;
            links[j].weight = 0.0;
    } 
    
        updateLinkStatusExperimental(links);
    
        round1(mMinLinks, signalLosses, combinerLosses, splitterLosses, sites, links, antennasUsed, antennaCosts, cableCosts, combinerCosts, splitterCosts);
	round2(mMinLinks, signalLosses, combinerLosses, splitterLosses, sites, links, antennasUsed, antennaCosts, cableCosts, combinerCosts, splitterCosts);

        if(getUnserved(sites) > 0)
        {
            if(mMinLinks == 2)
            {
                for (int z = 0 ; z < size ; z++)
                {
                    if(sites[z].feasibleLinks >= 1)
                        saveChangedLinks(z, 1, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                }
            }
            else if (mMinLinks == 3)
            {
                for (int z = 0 ; z < size ; z++)
                {
                    if(sites[z].feasibleLinks >= 2)
                        saveChangedLinks(z, 2, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                }

                if(getUnserved(sites) > 0)
                {
                    for (int z = 0 ; z < size ; z++)
                    {
                        if(sites[z].feasibleLinks >= 1)
                            saveChangedLinks(z, 1, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                    }      
                }
            }
            else if (mMinLinks == 4)
            {
                for (int z = 0 ; z < size ; z++)
                {
                    if(sites[z].feasibleLinks >= 3)
                        saveChangedLinks(z, 3, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                }

                if(getUnserved(sites) > 0)
                {
                    for (int z = 0 ; z < size ; z++)
                    {
                        if(sites[z].feasibleLinks >= 2)
                            saveChangedLinks(z, 2, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                    }      
                }

                if(getUnserved(sites) > 0)
                {
                    for (int z = 0 ; z < size ; z++)
                    {
                        if(sites[z].feasibleLinks >= 1)
                            saveChangedLinks(z, 1, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                    }      
                }
            }
        }

        if(getUnserved(sites) > 0)
        {
            cout << "No solution for remaining unserved sites" << endl;
        }

        findVitalLinks(links, size, sites, signalLosses, combinerLosses, splitterLosses);

        trim(links, sites);

        for (int z = 0 ; z < size ; z++)
            cout << "Site: " << z << " Served: " << sites[z].served << " Available: " << sites[z].feasibleLinks << " Saved: " << sites[z].savedLinks << endl;

        for (int z = 0 ; z < numLinks ; z++)
            cout << "Link: " << z << " T: " << links[z].transmitter << " R: " << links[z].receiver << " Reason: " << links[z].unservedReason << "  " << links[z].saved << " Wei: " << links[z].weight << " Req: " << links[z].required << " Cost: " << links[z].cost << " PL: " << links[z].pathloss <<  " SS: " << links[z].signalstrength <<  " Dist: " << links[z].distance <<  " Clear: " << links[z].clearance << " Ser: " << sites[links[z].transmitter].served << endl;

        updateLinkStatusPlanned(links);
    
    tend = time(0);
    cout << "Time: " << difftime(tend, tstart) << endl;

    accept();
    close();
}


//*********************************************************************************************
// Insert a new radio installation into the database for a new link
//*********************************************************************************************
int Optimisation::InsertRadInst(int SiteID, int Sector, double Bearing, double Tilt)
{
    string query = "";
    char * text;
    text = new char[33];

    query = "INSERT INTO radioinstallation ";
    query += "(lastmodified, machineid, siteid, sector,techkey,eirp,txpower,";
    query += "txlosses,txantennaheight,txantpatternkey,txbearing,txmechtilt,";
    query += "rxlosses,rxantennaheight,rxantpatternkey,rxbearing,rxmechtilt,";
    query += "rxsensitivity, project) VALUES (now(),";
    gcvt(gDb.globalMachineID, 8, text);
    query += text;
    query += ", ";
    gcvt(SiteID, 20, text);
    query += text;
    query += ", ";
    gcvt(Sector, 8, text);
    query += text;
    query += ", ";
    gcvt(mTechKey, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sEIRP, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sTxPower, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sTxSysLoss, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sTxHeight, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sTxPatternKey, 8, text);
    query += text;
    query += ", ";
    gcvt(Bearing, 8, text);
    query += text;
    query += ", ";
    gcvt(Tilt, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sRxSysLoss, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sRxHeight, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sRxPatternKey, 8, text);
    query += text;
    query += ", ";
    gcvt(Bearing, 8, text);
    query += text;
    query += ", ";
    gcvt(Tilt, 8, text);
    query += text;
    query += ", ";
    gcvt(mInst.sRxSens, 8, text);
    query += text;
    query += ", ";
    gcvt(mProject, 8, text);
    if ( 0 == mProject )
        strcpy(text, "null");
    query += text;
    query += ")";


    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Insert Radioinstallation in Optimisation::InsertRadInst failed" << endl;
        delete [] text;
        return 0;
    }

    int LastID = 0;
    pqxx::result r;
    query = "select currval('radioinstallation_id_seq');";
    if ( !gDb.PerformRawSql(query) )
    {
        string err = "There seems to be an error getting the last ID of the radioinstallation table";
        err += query;
        QRAP_WARN(err.c_str());
    } // if
    else
    {
        gDb.GetLastResult(r);

        int size = r.size();
        if ( size > 0 )
            LastID = atoi(r[0][0].c_str());
    }
    delete [] text;
    return LastID;
}

//*********************************************************************************************
// Read in a txpower using the datarate specified by the user on the GUI
//*********************************************************************************************
double Optimisation::getPower(string Rate)
{
    string err = "";
    string query;
    double power = 20.0;
    pqxx::result Power;

    query = "SELECT txpower";
    query += " FROM receiver_capabilities ";
    query += "WHERE data_rate = ";
    query += Rate;
    query += ";";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on receiver capabilities table failed in Optimisation::getPower" << endl;
        string err = "Database Select on receiver capabilities table failed";
        QRAP_WARN(err.c_str());
    }
    gDb.GetLastResult(Power);
    if ( Power.size() > 0 )
    {
        power = atoi(Power[0]["txpower"].c_str());
    }
    else
    {
        string err = "No receiver capabilities stored in the database ";
        QRAP_WARN(err.c_str());
    }
    return power;
}

//*********************************************************************************************
// Read in a rxsensitivity using the datarate specified by the user on the GUI
//*********************************************************************************************
double Optimisation::getSensitivity(string Rate)
{
    string err = "";
    string query;
    double sensitivity = -96.0;
    pqxx::result Sens;

    query = "SELECT rxsensitivity";
    query += " FROM receiver_capabilities ";
    query += "WHERE data_rate = ";
    query += Rate;
    query += ";";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on receiver capabilities table failed in Optimisation::getSensitivity" << endl;
        string err = "Database Select on receiver capabilities table failed";
        QRAP_WARN(err.c_str());
    }
    gDb.GetLastResult(Sens);
    if ( Sens.size() > 0 )
    {
        sensitivity = atof(Sens[0]["rxsensitivity"].c_str());
    }
    else
    {
        string err = "No receiver capabilities stored in the database ";
        QRAP_WARN(err.c_str());
    }
    return sensitivity;
}

//*********************************************************************************************
// Convert a integer to a string
//*********************************************************************************************
string Optimisation::convertInt(int number)
{
   stringstream sTemp;
   sTemp << number;
   return sTemp.str();
}


//*********************************************************************************************
// Read in a heightcost from the database for a specified height
//*********************************************************************************************
double Optimisation::getHeightCost(int Height)
{
    string err = "";
    string query;
    double HeightCost = -1.0;
    pqxx::result costing;
    
    string str = convertInt(Height);

    query = "SELECT cost";
    query += " FROM height_costs ";
    query += "WHERE height = ";
    query += str;
    query += ";";

    if ( !gDb.PerformRawSql(query) )
    {
        cout << "Database Select on height costs table failed in Optimisation::getHeightCost" << endl;
        string err = "Database Select on height costs table failed";
        QRAP_WARN(err.c_str());
    }
    gDb.GetLastResult(costing);
    if ( costing.size() > 0 )
    {
        HeightCost = atoi(costing[0]["cost"].c_str());
    }

    return HeightCost;
}

//*********************************************************************************************
// Update the status of links to 'planned' in the database
//*********************************************************************************************
void Optimisation::updateLinkStatusPlanned(linkDetails links[])
{
    
    for (int p = 0 ; p < numLinks ; p++)
    {       
        if(links[p].saved == true)
        {
            string query = "";
            string query2 = "";
            string query3 = "";
            string query4 = "";
            pqxx::result Rad;
            pqxx::result Site1;
            pqxx::result Site2;

            bool site1found;
            bool site2found;

            query = "SELECT txinst, rxinst, id ";
            query += "FROM links;";
            gDb.PerformRawSql(query);
            gDb.GetLastResult(Rad);

            if ( Rad.size() > 0 )
            {
                for ( int k = 0; k < Rad.size(); k++ )
                {
                    site1found = false;
                    site2found = false;

                    query2 = "SELECT radioinstallation.siteid ";
                    query2 += "FROM radioinstallation ";
                    query2 += "WHERE radioinstallation.id = ";
                    query2 += Rad[k]["txinst"].c_str();
                    query2 += ";";
                    gDb.PerformRawSql(query2);
                    gDb.GetLastResult(Site1);

                    if ( Site1.size() > 0 )
                    {
                            if (atoi(Site1[0]["siteid"].c_str()) == mSiteList[links[p].transmitter])
                            {
                                site1found = true;
                            }
                    }

                    query3 = "SELECT radioinstallation.siteid ";
                    query3 += "FROM radioinstallation ";
                    query3 += "WHERE radioinstallation.id = ";
                    query3 += Rad[k]["rxinst"].c_str();
                    query3 += ";";
                    gDb.PerformRawSql(query3);
                    gDb.GetLastResult(Site2);

                    if ( Site2.size() > 0 )
                    {
                            if (atoi(Site2[0]["siteid"].c_str()) == mSiteList[links[p].receiver])
                            {
                                site2found = true;
                            }
                    }

                    if ( site1found && site2found )
                    {
                        query4 = "UPDATE links SET status = 'Planned' ";
                        query4 += "WHERE id = ";
                        query4 += Rad[k]["id"].c_str();
                        query4 += ";";
                        gDb.PerformRawSql(query4);
                    }
                }
            }
        }
    }
    
    string err = "The total cost estimate for the proposed optimised link structure is R" + convertInt(totalCost);
    QMessageBox::information(this, "QRap", err.c_str());
}

//*********************************************************************************************
// Delete the existing links from the database
//*********************************************************************************************
void Optimisation::updateLinkStatusExperimental(linkDetails links[])
{
    string query = "";
    string query2 = "";
    string query3 = "";
    string query4 = "";
    string query5 = "";
    string query6 = "";
    pqxx::result Rad;
    pqxx::result Site1;
    pqxx::result Site2;

    bool site1found;
    bool site2found;
    int site1id;
    int site2id;

    query = "SELECT id, txinst, rxinst ";
    query += "FROM links;";
    gDb.PerformRawSql(query);
    gDb.GetLastResult(Rad);
    if ( Rad.size() > 0 )
    {
        for ( int k = 0; k < Rad.size(); k++ )
        {
            site1found = false;
            site2found = false;
            site1id = -1;
            site2id = -1;

            query2 = "SELECT radioinstallation.siteid ";
            query2 += "FROM radioinstallation ";
            query2 += "WHERE radioinstallation.id = ";
            query2 += Rad[k]["txinst"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(Site1);

            if ( Site1.size() > 0 )
            {
                for (int m = 0 ; m < size ; m++)
                {
                    if (atoi(Site1[0]["siteid"].c_str()) == mSiteList[m])
                    {
                        site1found = true;
                        site1id = m;
                        break;
                    }
                }
            }

            query3 = "SELECT radioinstallation.siteid ";
            query3 += "FROM radioinstallation ";
            query3 += "WHERE radioinstallation.id = ";
            query3 += Rad[k]["rxinst"].c_str();
            query3 += ";";
            gDb.PerformRawSql(query3);
            gDb.GetLastResult(Site2);

            if ( Site2.size() > 0 )
            {
                for (int m = 0 ; m < size ; m++)
                {
                    if (atoi(Site2[0]["siteid"].c_str()) == mSiteList[m])
                    {
                        site2found = true;
                        site2id = m;
                        break;
                    }
                }
            }

            if ( site1found && site2found )
            {
                        query4 = "DELETE FROM links ";
                        query4 += "WHERE id = ";
                        query4 += Rad[k]["id"].c_str();
                        query4 += ";";
                        gDb.PerformRawSql(query4);
                        
                        query5 = "DELETE FROM radioinstallation ";
                        query5 += "WHERE id = ";
                        query5 += Rad[k]["rxinst"].c_str();
                        query5 += ";";
                        gDb.PerformRawSql(query5);
                        
                        query6 = "DELETE FROM radioinstallation ";
                        query6 += "WHERE id = ";
                        query6 += Rad[k]["txinst"].c_str();
                        query6 += ";";
                        gDb.PerformRawSql(query6);
            }
        }
    }   
}

//*********************************************************************************************
// Update the transmitter and receiver of the links as well as their unserved reasons
//*********************************************************************************************
void Optimisation::setLinkDetails(linkDetails links[])
{
    double PathLoss, MinClearance, SignalStrength;
    bool Work = true;
    bool Signal = false;
    tempPlotRes = PlotResolution;
    Link.SetLink(Units, DownLink, mInst.sFrequency, mkFactor, tempPlotRes,
            DEMsource, ClutterSource, UseClutter, 0, 0, true);

    if ( Link.SetTxSite(mSiteList[links[currLinkNum].transmitter], mInst) )
    {
        if ( Link.SetRxSite(mSiteList[links[currLinkNum].receiver], mInst) )
        {
            if ( Link.DoLink(true, mMaxDist * 1000) )
            {
                MinClearance = Link.GetMinClearance();
                links[currLinkNum].clearance = MinClearance;
                PathLoss = Link.GetPathLoss();
                links[currLinkNum].pathloss = PathLoss;
                SignalStrength = Link.GetRxLev();
                links[currLinkNum].signalstrength = SignalStrength;
                links[currLinkNum].distance = Link.GetDistance();
                if ( ((SignalStrength > -120)&&(SignalStrength < -60)) || (SignalStrength >= mSignalStrength) )
                    Signal = true;
                Work = ((MinClearance >= mMinClear)&&(PathLoss <= mMaxPath)&&(Signal));
                if ( Work )
                {
                    links[currLinkNum].unservedReason = 0;
                }
                else
                {
                    if ( (MinClearance < mMinClear)&&((PathLoss > mMaxPath) || (!Signal)) )
                    {
                        links[currLinkNum].unservedReason = 3;
                    }
                    else
                    {
                        if ( MinClearance < mMinClear )
                            links[currLinkNum].unservedReason = 2;
                        else
                            links[currLinkNum].unservedReason = 1;
                    }
                }
                Link.ReSetPlotRes(PlotResolution);
            }
            else links[currLinkNum].unservedReason = 4;
        }
    }
}

//*********************************************************************************************
// Used in round 1 to count the number of feasible links for a site using the cheapest link
//*********************************************************************************************
int Optimisation::countLinks(int index, linkDetails links[], bool set)
{
    double PathLoss, MinClearance, SignalStrength;
    bool Work = true;
    bool Signal = false;
    tempPlotRes = PlotResolution;
    Link.SetLink(Units, DownLink, mInst.sFrequency, mkFactor, tempPlotRes, DEMsource, ClutterSource, UseClutter, 0, 0, true);

    int linkCount = 0;

    if ( Link.SetTxSite(mSiteList[index], mInst) )
    {
        for ( int j = 0; j < size; j++ )
        {
            if ( j != index )
            {
                if ( Link.SetRxSite(mSiteList[j], mInst) )
                {
                    if ( (j > index)&&(set) )
                    {
                        links[currLinkNum].receiver = j;
                        links[currLinkNum].transmitter = index;
                        links[currLinkNum].linkNumber = currLinkNum;                     
                        links[currLinkNum].rxheight = mInst.sRxHeight;
                        links[currLinkNum].txheight = mInst.sTxHeight;
                    }
                    if ( Link.DoLink(true, mMaxDist * 1000) )
                    {
                        MinClearance = Link.GetMinClearance();
                        links[currLinkNum].clearance = MinClearance;
                        PathLoss = Link.GetPathLoss();
                        links[currLinkNum].pathloss = PathLoss;
                        SignalStrength = Link.GetRxLev();
                        links[currLinkNum].signalstrength = SignalStrength;
                        links[currLinkNum].distance = Link.GetDistance();
                        if ( ((SignalStrength > -120)&&(SignalStrength < -60)) || (SignalStrength >= mSignalStrength) )
                            Signal = true;
                        Work = ((MinClearance >= mMinClear)&&(PathLoss <= mMaxPath)&&(Signal));
                        if ( Work )
                        {
                            cout << index << " x " << j << " of " << size << endl;
                            linkCount++;
                            if ( (j > index)&&(set) )
                            {
                                links[currLinkNum].unservedReason = 0;
                            }
                        }
                        else
                        {
                            if ( (j > index)&&(set) )
                            {
                                if ( (MinClearance < mMinClear)&&((PathLoss > mMaxPath) || (!Signal)) )
                                {
                                    links[currLinkNum].unservedReason = 3;
                                }
                                else
                                {
                                    if ( MinClearance < mMinClear )
                                        links[currLinkNum].unservedReason = 2;
                                    else
                                        links[currLinkNum].unservedReason = 1;
                                }
                            }
                        }
                        Link.ReSetPlotRes(PlotResolution);
                    }
                    else
                    {
                       links[currLinkNum].unservedReason = 4; 
                    }
                    
                    if ( (j > index)&&(set) )
                    {
                        currLinkNum++;
                    }
                }
            }
        }
    }

    return linkCount;
}

//*********************************************************************************************
// Used in round 1 to save all feasible links for a site using the cheapest link
//*********************************************************************************************
void Optimisation::saveLinks(int index, siteDetails sites[], linkDetails links[])
{
    double PathLoss, MinClearance, SignalStrength;
    bool Work = true;
    bool Signal = false;
    int TxRadID, RxRadID;
    char * temp = new char[33];
    string LinkName = "Link";
    QString Linkname = "Link";
    int LinkID = 0;
    tempPlotRes = PlotResolution;
    Link.SetLink(Units, DownLink, mInst.sFrequency, mkFactor, tempPlotRes, DEMsource, ClutterSource, UseClutter, 0, 0, true);
    
    int currIndex = 0;
    for (int h = 0 ; h < index ; h++)
    {
        currIndex += sites[h].availableLinks;
    }

    
    if ( Link.SetTxSite(mSiteList[index], mInst) )
    {
        for ( int m = index+1; m < size; m++ )
        {
                if ( Link.SetRxSite(mSiteList[m], mInst) )
                {
                    if ( Link.DoLink(true, mMaxDist * 1000) )
                    {
                        MinClearance = Link.GetMinClearance();
                        PathLoss = Link.GetPathLoss();
                        mInst.sEIRP = Link.GetEIRP();
                        SignalStrength = Link.GetRxLev();
                        if ( ((SignalStrength > -120)&&(SignalStrength < -60)) || (SignalStrength >= mSignalStrength) )
                            Signal = true;
                        Work = ((MinClearance >= mMinClear)&&(PathLoss <= mMaxPath)&&(Signal));
                        if ( Work )
                        {
                            if ((!LinkExists(mSiteList[index], mSiteList[m]))||((LinkExists(mSiteList[index], mSiteList[m]))&&(isExperimental(mSiteList[index], mSiteList[m]))))
                            {
                                TxRadID = InsertRadInst(mSiteList[index], m, Link.GetTxBearing(), Link.GetTxTilt());
                                RxRadID = InsertRadInst(mSiteList[m], index, Link.GetRxBearing(), Link.GetRxTilt());

                                Link.SetTxInst(TxRadID);
                                Link.SetRxInst(RxRadID);
                                LinkName = "Link";
                                LinkID = 0;
                                
                                links[currIndex].rxheight = mInst.sRxHeight;
                                links[currIndex].txheight = mInst.sTxHeight;
                                links[currIndex].antID = currAntID;
                                links[currIndex].cabID = currCabID; 
                                links[currIndex].comID = currComID;
                                links[currIndex].splitID = currSplitID;
                                links[currIndex].cost = minCost;
                                totalCost += links[currIndex].cost;
                                links[currIndex].saved = true;
                                sites[index].savedLinks++;
                                sites[m].savedLinks++;
                                
											Link.DoLink(false);
                                Link.SaveLink(LinkName, LinkID);
                                
                                cout << "Saved link between " << index << " and " << m << endl;
                                if ( SavePDF )
                                {
                                    cLinkAnalysis * LinkDisplay = new cLinkAnalysis(mParent, mFL);
                                    Linkname = LinkName.c_str();
                                    Transmitter = "Site:";
                                    gcvt(mSiteList[index], 8, temp);
                                    Transmitter += temp;
                                    Transmitter += "_Radio:";
                                    gcvt(TxRadID, 8, temp);
                                    Transmitter += temp;
                                    gcvt(mSiteList[m], 8, temp);
                                    Receiver = "Site:";
                                    Receiver += temp;
                                    Receiver += "_Radio:";
                                    gcvt(RxRadID, 8, temp);
                                    Receiver += temp;
                                    tempPlotRes = PlotResolution;
                                    LinkDisplay->DoAndSetUpDisplay(Units, DownLink,
                                            mInst.sFrequency,
                                            mkFactor, tempPlotRes,
                                            DEMsource, ClutterSource,
                                            UseClutter, TxRadID, RxRadID,
                                            Linkname, Transmitter,
                                            Receiver, LinkID);
                                    LinkDisplay->SetPath(path);
                                    LinkDisplay->SetAutoName(true);
                                    LinkDisplay->MakePDF();
                                    LinkDisplay->OKClose();
                                    LinkDisplay->exec();
                                    delete LinkDisplay;
                                }
                            }                           
                        }
                        else
                        {
                        }
                        Link.ReSetPlotRes(PlotResolution);
                    }//if (Link.DoLink(true,mMaxDist*1000))
                    else
                    {
                    }
                }//if (Link.SetRxSite(mSiteList[m],mInst))
                else
                {
                }
            //}
               currIndex++;
        } //for (m=k+1; m<size; m++)
    }//if (Link.SetTxSite(mSiteList[k],mInst))
    else
    {
     
    }

    delete [] temp;
}

//*********************************************************************************************
// Used in round 2 to save a particular feasible link for a site using an adjusted link
//*********************************************************************************************
void Optimisation::saveSingleLink(linkDetails links[])
{
    double PathLoss, MinClearance, SignalStrength;
    bool Work = true;
    bool Signal = false;
    int TxRadID, RxRadID;
    char * temp = new char[33];
    string LinkName = "Link";
    QString Linkname = "Link";
    int LinkID = 0;
    tempPlotRes = PlotResolution;
    Link.SetLink(Units, DownLink, mInst.sFrequency, mkFactor, tempPlotRes, DEMsource, ClutterSource, UseClutter, 0, 0, true);

    if ( Link.SetTxSite(mSiteList[links[currLinkNum].transmitter], mInst) )
    {
        if ( Link.SetRxSite(mSiteList[links[currLinkNum].receiver], mInst) )
        {
            if ( Link.DoLink(true, mMaxDist * 1000) )
            {
                MinClearance = Link.GetMinClearance();
                links[currLinkNum].clearance = MinClearance;
                PathLoss = Link.GetPathLoss();
                links[currLinkNum].pathloss = PathLoss;
                mInst.sEIRP = Link.GetEIRP();
                SignalStrength = Link.GetRxLev();
                links[currLinkNum].signalstrength = SignalStrength;
                links[currLinkNum].distance = Link.GetDistance();
                if ( ((SignalStrength > -120)&&(SignalStrength < -60)) || (SignalStrength >= mSignalStrength) )
                    Signal = true;
                Work = ((MinClearance >= mMinClear)&&(PathLoss <= mMaxPath)&&(Signal));
                if ( Work )
                {
                    if ((!LinkExists(mSiteList[links[currLinkNum].transmitter], mSiteList[links[currLinkNum].receiver]))||((LinkExists(mSiteList[links[currLinkNum].transmitter], mSiteList[links[currLinkNum].receiver]))&&(isExperimental(mSiteList[links[currLinkNum].transmitter], mSiteList[links[currLinkNum].receiver]))))
                    {
                        TxRadID = InsertRadInst(mSiteList[links[currLinkNum].transmitter], links[currLinkNum].receiver, Link.GetTxBearing(), Link.GetTxTilt());
                        RxRadID = InsertRadInst(mSiteList[links[currLinkNum].receiver], links[currLinkNum].transmitter, Link.GetRxBearing(), Link.GetRxTilt());

                        cout << "Saved link between " << links[currLinkNum].transmitter << " and " << links[currLinkNum].receiver << endl;
                        Link.SetTxInst(TxRadID);
                        Link.SetRxInst(RxRadID);
                        links[currLinkNum].distance = Link.GetDistance();
                        LinkName = "Link";
                        LinkID = 0;
								Link.DoLink(false);
                        Link.SaveLink(LinkName, LinkID);
                        if ( SavePDF )
                        {
                            cLinkAnalysis * LinkDisplay = new cLinkAnalysis(mParent, mFL);
                            Linkname = LinkName.c_str();
                            Transmitter = "Site:";
                            gcvt(mSiteList[links[currLinkNum].transmitter], 8, temp);
                            Transmitter += temp;
                            Transmitter += "_Radio:";
                            gcvt(TxRadID, 8, temp);
                            Transmitter += temp;
                            gcvt(mSiteList[links[currLinkNum].receiver], 8, temp);
                            Receiver = "Site:";
                            Receiver += temp;
                            Receiver += "_Radio:";
                            gcvt(RxRadID, 8, temp);
                            Receiver += temp;
                            tempPlotRes = PlotResolution;
                            LinkDisplay->DoAndSetUpDisplay(Units, DownLink,
                                    mInst.sFrequency,
                                    mkFactor, tempPlotRes,
                                    DEMsource, ClutterSource,
                                    UseClutter, TxRadID, RxRadID,
                                    Linkname, Transmitter,
                                    Receiver, LinkID);
                            LinkDisplay->SetPath(path);
                            LinkDisplay->SetAutoName(true);
                            LinkDisplay->MakePDF();
                            LinkDisplay->OKClose();
                            LinkDisplay->exec();
                            delete LinkDisplay;
                        }
                    }
                    cout << mSiteList[links[currLinkNum].transmitter] << " to " << mSiteList[links[currLinkNum].receiver] << " saved." << endl;
                }//if (Work)
                else
                {
                    cout << mSiteList[links[currLinkNum].transmitter] << " to " << mSiteList[links[currLinkNum].receiver] << " does not work." << endl;
                }
                Link.ReSetPlotRes(PlotResolution);
            }//if (Link.DoLink(true,mMaxDist*1000))
            else
            {
                cout << mSiteList[links[currLinkNum].transmitter] << " to " << mSiteList[links[currLinkNum].receiver] << " skipped." << endl;
            }
        }//if (Link.SetRxSite(mSiteList[m],mInst))
        else
        {
            cout << "Problem with site: " << mSiteList[links[currLinkNum].receiver] << endl;
        }
    }//if (Link.SetTxSite(mSiteList[k],mInst))
    else
    {
        cout << "Problem with site: " << mSiteList[links[currLinkNum].transmitter] << endl;
    }

    delete [] temp;
}

//*********************************************************************************************
// Load all the default costs and losses from the database for all peripherals
//*********************************************************************************************
void Optimisation::LoadCosts(double antennaCosts[], double cableCosts[], bool connectorList[], double combinerCosts[], double splitterCosts[], double signalLosses[], double combinerLosses[], double splitterLosses[], siteDetails sites[])
{
    string query = "";
    pqxx::result Antennas;
    pqxx::result Cables;
    pqxx::result Sites;
    pqxx::result Connectors;

    for ( int i = 0; i < mAntennaList.size(); i++ )
    {
        QString str = QString::number(mAntennaList[i]);
        query = "SELECT antennapattern.antdevicekey, antennadevice.cost ";
        query += "FROM antennapattern cross join  antennadevice ";
        query += "WHERE antennadevice.id = antennapattern.antdevicekey AND antennapattern.id = ";
        query += str.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(Antennas);
        antennaCosts[i] = atof(Antennas[0]["cost"].c_str());
    }

    for ( int i = 0; i < mCableList.size(); i++ )
    {
        QString str2 = QString::number(mCableList[i]);
        query = "SELECT cost_per_metre, signal_loss_per_metre FROM cable_type WHERE id = ";
        query += str2.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(Cables);
        cableCosts[i] = atof(Cables[0]["cost_per_metre"].c_str());
        signalLosses[i] = atof(Cables[0]["signal_loss_per_metre"].c_str());
    }
       
    for ( int h = 0; h < mConnectorList.size(); h++ )
    {
        int counter = 0;
        if(connectorList[h] == false)
        {
            QString str3 = QString::number(mConnectorList[h]);
            query = "SELECT cost,losses FROM combiner_splitter_type WHERE id = ";
            query += str3.toStdString();
            query += ";";
            gDb.PerformRawSql(query);
            gDb.GetLastResult(Connectors);
            combinerCosts[counter] = atof(Connectors[0]["cost"].c_str());
            combinerLosses[counter] = atof(Connectors[0]["losses"].c_str());
        }
    }
    
    for ( int j = 0; j < mConnectorList.size(); j++ )
    {
        int counter = 0;
        if(connectorList[j] == true)
        {
            QString str3 = QString::number(mConnectorList[j]);
            query = "SELECT cost,losses FROM combiner_splitter_type WHERE id = ";
            query += str3.toStdString();
            query += ";";
            gDb.PerformRawSql(query);
            gDb.GetLastResult(Connectors);
            splitterCosts[counter] = atof(Connectors[0]["cost"].c_str());
            splitterLosses[counter] = atof(Connectors[0]["losses"].c_str());
            counter++;
        }
    }

    for ( int i = 0; i < mSiteList.size(); i++ )
    {
        QString str4 = QString::number(mSiteList[i]);
        query = "SELECT existing_mast, min_mast_height, max_mast_height FROM sitedescription WHERE siteid = ";
        query += str4.toStdString();
        query += ";";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(Sites);
        if ( Sites.size() > 0 )
        {
            if(strcmp(Sites[0]["existing_mast"].c_str(),"True"))
                sites[i].existingMast = true;
            else
                sites[i].existingMast = false;
            sites[i].minHeight = atoi(Sites[0]["min_mast_height"].c_str());
            sites[i].maxHeight = atoi(Sites[0]["max_mast_height"].c_str());
        }
        else
        {
            sites[i].existingMast = false;
            sites[i].minHeight = 0;
            sites[i].maxHeight = 0;
        }
    }
}

//*********************************************************************************************
// Check if a particular link exists in the database
//*********************************************************************************************
bool Optimisation::LinkExists(int txsiteid, int rxsiteid)
{
    string query = "";
    string query2 = "";
    string query3 = "";
    pqxx::result Rad;
    pqxx::result Site1;
    pqxx::result Site2;

    bool linkExists = false;
    bool site1found;
    bool site2found;

    query = "SELECT txinst, rxinst ";
    query += "FROM links;";
    gDb.PerformRawSql(query);
    gDb.GetLastResult(Rad);
    if ( Rad.size() > 0 )
    {
        for ( int k = 0; k < Rad.size(); k++ )
        {
            site1found = false;
            site2found = false;

            query2 = "SELECT radioinstallation.siteid ";
            query2 += "FROM radioinstallation ";
            query2 += "WHERE radioinstallation.id = ";
            query2 += Rad[k]["txinst"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(Site1);

            if ( Site1.size() > 0 )
            {
                if ( (atoi(Site1[0]["siteid"].c_str()) == txsiteid) || (atoi(Site1[0]["siteid"].c_str()) == rxsiteid) )
                    site1found = true;
            }

            query3 = "SELECT radioinstallation.siteid ";
            query3 += "FROM radioinstallation ";
            query3 += "WHERE radioinstallation.id = ";
            query3 += Rad[k]["rxinst"].c_str();
            query3 += ";";
            gDb.PerformRawSql(query3);
            gDb.GetLastResult(Site2);

            if ( Site2.size() > 0 )
            {
                if ( (atoi(Site2[0]["siteid"].c_str()) == txsiteid) || (atoi(Site2[0]["siteid"].c_str()) == rxsiteid) )
                    site2found = true;
            }

            if ( site1found && site2found )
            {
                linkExists = true;
                break;
            }
        }
    }

    return linkExists;
}

//*********************************************************************************************
// Check if a link exists and is an experimental link in the database
//*********************************************************************************************
bool Optimisation::isExperimental(int txsiteid, int rxsiteid)
{
    string query = "";
    string query2 = "";
    string query3 = "";
    pqxx::result Rad;
    pqxx::result Site1;
    pqxx::result Site2;

    bool linkExists = false;
    bool site1found;
    bool site2found;

    query = "SELECT txinst, rxinst ";
    query += "FROM links";
    query += " WHERE status='Experimental'";
    gDb.PerformRawSql(query);
    gDb.GetLastResult(Rad);
    if ( Rad.size() > 0 )
    {
        for ( int k = 0; k < Rad.size(); k++ )
        {
            site1found = false;
            site2found = false;

            query2 = "SELECT radioinstallation.siteid ";
            query2 += "FROM radioinstallation ";
            query2 += "WHERE radioinstallation.id = ";
            query2 += Rad[k]["txinst"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(Site1);

            if ( Site1.size() > 0 )
            {
                if ( (atoi(Site1[0]["siteid"].c_str()) == txsiteid) || (atoi(Site1[0]["siteid"].c_str()) == rxsiteid) )
                    site1found = true;
            }

            query3 = "SELECT radioinstallation.siteid ";
            query3 += "FROM radioinstallation ";
            query3 += "WHERE radioinstallation.id = ";
            query3 += Rad[k]["rxinst"].c_str();
            query3 += ";";
            gDb.PerformRawSql(query3);
            gDb.GetLastResult(Site2);

            if ( Site2.size() > 0 )
            {
                if ( (atoi(Site2[0]["siteid"].c_str()) == txsiteid) || (atoi(Site2[0]["siteid"].c_str()) == rxsiteid) )
                    site2found = true;
            }

            if ( site1found && site2found )
            {
                linkExists = true;
                break;
            }
        }
    }

    return linkExists;
}

//*********************************************************************************************
// Return the number of unserved sites in the current setup
//*********************************************************************************************
int Optimisation::getUnserved(siteDetails sites[])
{
    int count = 0;
    
    for (int j = 0 ; j < size ; j++)
    {
        if(sites[j].savedLinks < mMinLinks)
            count++;
    }
    return count;
}

//*********************************************************************************************
// Perform round 1, where the cheapest link is assigned to try and serve sites
//*********************************************************************************************
void Optimisation::round1(double minLinks, double signalLosses[], double combinerLosses[], double splitterLosses[], siteDetails sites[], 
        linkDetails links[], int antennasUsed[],  double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[])
{
    currLinkNum = 0;

    for ( int i = 0; i < size; i++ )
    {
        if(sites[i].served == false)
        {
            if ( sites[i].existingMast == true )
            {
                mInst.sRxHeight = sites[i].minHeight;
                mInst.sTxHeight = sites[i].minHeight;
            }
            else
            {
                mInst.sRxHeight = mMinHeightN;
                mInst.sTxHeight = mMinHeightN;
            }

            mInst.sTxSysLoss = ((mInst.sTxHeight + 5) * signalLosses[currCabID]) + combinerLosses[currComID];
            mInst.sRxSysLoss = ((mInst.sRxHeight + 5) * signalLosses[currCabID]) + splitterLosses[currSplitID];       

            sites[i].feasibleLinks = countLinks(i, links, true);
            sites[i].availableLinks = size - i - 1;
            
            if ( sites[i].feasibleLinks >= minLinks )
            {
                sites[i].served = true;
                saveLinks(i, sites, links);
            }
        }
    }

    antennasUsed[currAntID] = 1;
    cout << "Total Cost: " << totalCost << endl;
    cout << "Unserved after round 1: " << getUnserved(sites) << endl;
}

//*********************************************************************************************
// Perform round 2, where the links that couldn't get made in round 1 are adjusted is assigned 
// to try and make them feasible to save and serve more sites
//*********************************************************************************************
void Optimisation::round2(double minLinks, double signalLosses[], double combinerLosses[],
        double splitterLosses[], siteDetails sites[], linkDetails links[], int antennasUsed[], 
        double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[])
{
    bool lastLink;
    bool revisit[size];
    
    for(int p = 0 ; p < size ; p++)
        revisit[p] = false;
    
    currLinkNum = 0;
    
    for ( int z = 0; z < size; z++ )
    {
            cout << endl << "Serving" << z << "  " << mSiteList[z] << endl;
            bool incremented;
            
            for ( int g = 0 ; g < sites[z].availableLinks; g++ )
            {
                incremented = false;
                lastLink = false;
                currLinkCost = 0;
                cout << currLinkNum << " : " << links[currLinkNum].unservedReason << endl;
                if ( links[currLinkNum].unservedReason == 2 )
                {
                    int heightRange;
                    int currHeight;
                    
                    if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                    {
                        int low, high;
                        
                        if(sites[z].maxHeight < sites[links[currLinkNum].receiver].maxHeight)
                          high = sites[z].maxHeight;
                        else
                          high = sites[links[currLinkNum].receiver].maxHeight;  
                        
                        if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                          low = sites[z].minHeight;
                        else
                          low = sites[links[currLinkNum].receiver].minHeight; 
                            
                        heightRange = high - low;
                        currHeight = low;
                        mInst.sTxHeight = low;
                    }
                    else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                    {                            
                        heightRange = sites[z].maxHeight - sites[z].minHeight;
                        currHeight = sites[z].minHeight;
                        mInst.sTxHeight = sites[z].minHeight;
                    }
                    else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                    {                            
                        heightRange = sites[links[currLinkNum].receiver].maxHeight - sites[links[currLinkNum].receiver].minHeight;
                        currHeight = sites[links[currLinkNum].receiver].minHeight;
                        mInst.sTxHeight = sites[links[currLinkNum].receiver].minHeight;
                    }
                    else
                    {
                        heightRange = mMaxHeightN - mMinHeightN;
                        currHeight = mMinHeightN;
                        mInst.sTxHeight = mMinHeightN;
                    }
                    
                    for ( int d = 0; d < heightRange; d++ )
                    {
                        mInst.sRxHeight = currHeight;

                        setLinkDetails(links);

                        if ( links[currLinkNum].unservedReason == 0 )
                        {
                            links[currLinkNum].rxheight = currHeight;
                            
                            incremented = true;
                            revisit[links[currLinkNum].receiver] = true;
                            
                            if (mInst.sTxHeight > links[currLinkNum].txheight)
                                links[currLinkNum].txheight = mInst.sTxHeight;
                            
                            links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                            links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                            links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);
                            
                            cout << "Changed height1 to " << currHeight << endl;
                            sites[z].feasibleLinks++;
                            sites[links[currLinkNum].receiver].feasibleLinks++;
                            break;
                        }
                        currHeight++;
                    }
                    
                    if (!incremented)
                    {                      
                           if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                           {
                               int low, high;

                               if(sites[z].maxHeight < sites[links[currLinkNum].receiver].maxHeight)
                                 high = sites[z].maxHeight;
                               else
                                 high = sites[links[currLinkNum].receiver].maxHeight;  

                               if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                                 low = sites[z].minHeight;
                               else
                                 low = sites[links[currLinkNum].receiver].minHeight; 

                               heightRange = high - low;
                               currHeight = low;
                               mInst.sRxHeight = low;
                           }
                           else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                           {                            
                               heightRange = sites[z].maxHeight - sites[z].minHeight;
                               currHeight = sites[z].minHeight;
                               mInst.sRxHeight = sites[z].minHeight;
                           }
                           else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                           {                            
                               heightRange = sites[links[currLinkNum].receiver].maxHeight - sites[links[currLinkNum].receiver].minHeight;
                               currHeight = sites[links[currLinkNum].receiver].minHeight;
                               mInst.sRxHeight = sites[links[currLinkNum].receiver].minHeight;
                           }
                           else
                           {
                               heightRange = mMaxHeightN - mMinHeightN;
                               currHeight = mMinHeightN;
                               mInst.sRxHeight = mMinHeightN;
                           }

                        for ( int d = 0; d < heightRange; d++ )
                        {
                            mInst.sTxHeight = currHeight;

                            setLinkDetails(links);

                            if ( links[currLinkNum].unservedReason == 0 )
                            {
                                links[currLinkNum].rxheight = mInst.sRxHeight;
                                
                                incremented = true;
                                
                                if (currHeight > links[currLinkNum].txheight)
                                    links[currLinkNum].txheight = currHeight;
                                
                                links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);

                                cout << "Changed height2 to " << currHeight << endl;
                                sites[z].feasibleLinks++;
                                sites[links[currLinkNum].receiver].feasibleLinks++;

                                break;
                            }
                            currHeight++;
                        }
                    }
                    
                    if (!incremented)
                    {
                        int currTxHeight;
                        int currRxHeight;
                        
                        if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                         {
                             int low; 

                             if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                               low = sites[z].minHeight;
                             else
                               low = sites[links[currLinkNum].receiver].minHeight; 

                             currTxHeight = low;
                         }
                         else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                         {                            
                             currTxHeight = sites[z].minHeight;
                         }
                         else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                         {                            
                             currTxHeight = sites[links[currLinkNum].receiver].minHeight;
                         }
                         else
                         {
                             currTxHeight = mMinHeightN;
                         }                        

                        
                        for ( int c = 0 ; c < heightRange ; c++)
                        {
                            mInst.sTxHeight = currTxHeight;
                            
                            if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                             {
                                 int low; 

                                 if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                                   low = sites[z].minHeight;
                                 else
                                   low = sites[links[currLinkNum].receiver].minHeight; 

                                 currRxHeight = low;
                             }
                             else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                             {                            
                                 currRxHeight = sites[z].minHeight;
                             }
                             else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                             {                            
                                 currRxHeight = sites[links[currLinkNum].receiver].minHeight;
                             }
                             else
                             {
                                 currRxHeight = mMinHeightN;
                             } 

                            for ( int d = 0; d < heightRange; d++ )
                            {
                                    mInst.sRxHeight = currRxHeight;
   
                                    setLinkDetails(links);
                                    
                                    if ( links[currLinkNum].unservedReason == 0 )
                                    {
                                        cout << "Changed height3 to " << currRxHeight << endl;
                                        sites[z].feasibleLinks++;
                                        sites[links[currLinkNum].receiver].feasibleLinks++;

                                        links[currLinkNum].rxheight = currRxHeight;
                                        
                                        revisit[links[currLinkNum].receiver] = true;
                                        incremented = true;

                                        if (currTxHeight > links[currLinkNum].txheight)
                                            links[currLinkNum].txheight = currTxHeight;

                                        links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                        links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                        links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);
                                        
                                        break;
                                    }
                                    currRxHeight++;
                            }// for d
                            if (incremented)
                                break;
                            currTxHeight++;
                        }//for c
                    }
                }//if (links[g].unservedReason == 2)
                else if ( links[currLinkNum].unservedReason == 1 )
                {
                        while ( lastLink == false )
                        {
                            currLinkCost = FLT_MAX;

                            for ( int c = 0; c < mAntennaList.size(); c++ )
                                {
                                    if ( antennasUsed[c] == 0 )
                                    {
                                        for ( int v = 0; v < mCableList.size(); v++ )
                                        {
                                            for( int y = 0 ; y < numCombiners ; y++)
                                            {
                                                for( int u = 0 ; u < numSplitters ; u++)
                                                {
                                                    if ( (antennaCosts[c] + cableCosts[v] + combinerCosts[y] + splitterCosts[u]) < currLinkCost )
                                                    {
                                                        currLinkCost = antennaCosts[c] + cableCosts[v] + combinerCosts[y] + splitterCosts[u];
                                                        currAntID = c;
                                                        currCabID = v;
                                                        currComID = y;
                                                        currSplitID = u;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                            antennasUsed[currAntID] = 1;

                            int lastCounter = 0;

                            //check if all antennas have been tried
                            for ( int h = 0; h < mAntennaList.size(); h++ )
                            {
                                if ( antennasUsed[h] == 0 )
                                    lastCounter++;
                            }

                            if ( lastCounter == 0 )
                                lastLink = true;
                            
                            if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                            {
                                int low;
  
                                if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                                  low = sites[z].minHeight;
                                else
                                  low = sites[links[currLinkNum].receiver].minHeight; 

                                mInst.sRxHeight = low;
                                mInst.sTxHeight = low;
                            }
                            else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                            {   
                                mInst.sRxHeight = sites[z].minHeight;
                                mInst.sTxHeight = sites[z].minHeight;
                            }
                            else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                            {   
                                mInst.sRxHeight = sites[links[currLinkNum].receiver].minHeight;
                                mInst.sTxHeight = sites[links[currLinkNum].receiver].minHeight;
                            }
                            else
                            {
                                mInst.sRxHeight = mMinHeightN;
                                mInst.sTxHeight = mMinHeightN;
                            }

                            setLinkDetails(links);
                            
                            if ( links[currLinkNum].unservedReason == 0 )
                            {
                                cout << "Changed antenna " << endl;
                                sites[z].feasibleLinks++;
                                sites[links[currLinkNum].receiver].feasibleLinks++;
                                
                                links[currLinkNum].antID = currAntID;
                                links[currLinkNum].cabID = currCabID;
                                links[currLinkNum].comID = currComID;
                                links[currLinkNum].splitID = currSplitID;
                                links[currLinkNum].rxheight = mInst.sRxHeight;
                                links[currLinkNum].txheight = mInst.sTxHeight;
                                                               
                                links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);

                                break;
                            }                            
                        }//while(lastLink == 0)

                }//else if (links[currLinkNum].unservedReason == 1)
                else if ( links[currLinkNum].unservedReason == 3 )
                {
                    int heightRange;
                    int currHeight;
                    
                    if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                    {
                        int low, high;
                        
                        if(sites[z].maxHeight < sites[links[currLinkNum].receiver].maxHeight)
                          high = sites[z].maxHeight;
                        else
                          high = sites[links[currLinkNum].receiver].maxHeight;  
                        
                        if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                          low = sites[z].minHeight;
                        else
                          low = sites[links[currLinkNum].receiver].minHeight; 
                            
                        heightRange = high - low;
                        currHeight = low;
                        mInst.sTxHeight = low;
                    }
                    else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                    {                            
                        heightRange = sites[z].maxHeight - sites[z].minHeight;
                        currHeight = sites[z].minHeight;
                        mInst.sTxHeight = sites[z].minHeight;
                    }
                    else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                    {                            
                        heightRange = sites[links[currLinkNum].receiver].maxHeight - sites[links[currLinkNum].receiver].minHeight;
                        currHeight = sites[links[currLinkNum].receiver].minHeight;
                        mInst.sTxHeight = sites[links[currLinkNum].receiver].minHeight;
                    }
                    else
                    {
                        heightRange = mMaxHeightN - mMinHeightN;
                        currHeight = mMinHeightN;
                        mInst.sTxHeight = mMinHeightN;
                    }
                    
                    for ( int d = 0; d < heightRange; d++ )
                    {
                        mInst.sRxHeight = currHeight;

                        setLinkDetails(links);

                        if ( links[currLinkNum].unservedReason == 0 )
                        {
                            links[currLinkNum].rxheight = currHeight;
                            
                            incremented = true;
                            revisit[links[currLinkNum].receiver] = true;
                            
                            if (mInst.sTxHeight > links[currLinkNum].txheight)
                                links[currLinkNum].txheight = mInst.sTxHeight;
                            
                            links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                            links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                            links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);
                            
                            cout << "Changed height1 to " << currHeight << endl;
                            sites[z].feasibleLinks++;
                            sites[links[currLinkNum].receiver].feasibleLinks++;
                            break;
                        }
                        currHeight++;
                    }
                    
                    if (!incremented)
                    {                      
                           if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                           {
                               int low, high;

                               if(sites[z].maxHeight < sites[links[currLinkNum].receiver].maxHeight)
                                 high = sites[z].maxHeight;
                               else
                                 high = sites[links[currLinkNum].receiver].maxHeight;  

                               if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                                 low = sites[z].minHeight;
                               else
                                 low = sites[links[currLinkNum].receiver].minHeight; 

                               heightRange = high - low;
                               currHeight = low;
                               mInst.sRxHeight = low;
                           }
                           else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                           {                            
                               heightRange = sites[z].maxHeight - sites[z].minHeight;
                               currHeight = sites[z].minHeight;
                               mInst.sRxHeight = sites[z].minHeight;
                           }
                           else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                           {                            
                               heightRange = sites[links[currLinkNum].receiver].maxHeight - sites[links[currLinkNum].receiver].minHeight;
                               currHeight = sites[links[currLinkNum].receiver].minHeight;
                               mInst.sRxHeight = sites[links[currLinkNum].receiver].minHeight;
                           }
                           else
                           {
                               heightRange = mMaxHeightN - mMinHeightN;
                               currHeight = mMinHeightN;
                               mInst.sRxHeight = mMinHeightN;
                           }

                        for ( int d = 0; d < heightRange; d++ )
                        {
                            mInst.sTxHeight = currHeight;

                            setLinkDetails(links);

                            if ( links[currLinkNum].unservedReason == 0 )
                            {
                                links[currLinkNum].rxheight = mInst.sRxHeight;
                                
                                incremented = true;
                                
                                if (currHeight > links[currLinkNum].txheight)
                                    links[currLinkNum].txheight = currHeight;
                                
                                links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);

                                cout << "Changed height2 to " << currHeight << endl;
                                sites[z].feasibleLinks++;
                                sites[links[currLinkNum].receiver].feasibleLinks++;

                                break;
                            }
                            currHeight++;
                        }
                    }
                    
                    if (!incremented)
                    {
                        int currTxHeight;
                        int currRxHeight;
                        
                        if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                         {
                             int low; 

                             if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                               low = sites[z].minHeight;
                             else
                               low = sites[links[currLinkNum].receiver].minHeight; 

                             currTxHeight = low;
                         }
                         else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                         {                            
                             currTxHeight = sites[z].minHeight;
                         }
                         else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                         {                            
                             currTxHeight = sites[links[currLinkNum].receiver].minHeight;
                         }
                         else
                         {
                             currTxHeight = mMinHeightN;
                         }                        

                        
                        for ( int c = 0 ; c < heightRange ; c++)
                        {
                            mInst.sTxHeight = currTxHeight;
                            
                            if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                             {
                                 int low; 

                                 if(sites[z].minHeight > sites[links[currLinkNum].receiver].minHeight)
                                   low = sites[z].minHeight;
                                 else
                                   low = sites[links[currLinkNum].receiver].minHeight; 

                                 currRxHeight = low;
                             }
                             else if (( sites[z].existingMast == true )&&( sites[links[currLinkNum].receiver].existingMast == false ))
                             {                            
                                 currRxHeight = sites[z].minHeight;
                             }
                             else if (( sites[z].existingMast == false )&&( sites[links[currLinkNum].receiver].existingMast == true ))
                             {                            
                                 currRxHeight = sites[links[currLinkNum].receiver].minHeight;
                             }
                             else
                             {
                                 currRxHeight = mMinHeightN;
                             } 

                            for ( int d = 0; d < heightRange; d++ )
                            {
                                    mInst.sRxHeight = currRxHeight;
   
                                    setLinkDetails(links);
                                    
                                    if ( links[currLinkNum].unservedReason == 0 )
                                    {
                                        cout << "Changed height3 to " << currRxHeight << endl;
                                        sites[z].feasibleLinks++;
                                        sites[links[currLinkNum].receiver].feasibleLinks++;

                                        links[currLinkNum].rxheight = currRxHeight;
                                        
                                        revisit[links[currLinkNum].receiver] = true;
                                        incremented = true;

                                        if (currTxHeight > links[currLinkNum].txheight)
                                            links[currLinkNum].txheight = currTxHeight;

                                        links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                        links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                        links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);
                                        
                                        break;
                                    }
                                    currRxHeight++;
                            }// for d
                            if (incremented)
                                break;
                            currTxHeight++;
                        }//for c
                    }
                    
                    if (!incremented)
                    {
                        while ( lastLink == false )
                        {
                                currLinkCost = FLT_MAX;

                                for ( int c = 0; c < mAntennaList.size(); c++ )
                                {
                                    if ( antennasUsed[c] == 0 )
                                    {
                                        for ( int v = 0; v < mCableList.size(); v++ )
                                        {
                                            for( int y = 0 ; y < numCombiners ; y++)
                                            {
                                                for( int u = 0 ; u < numSplitters ; u++)
                                                {
                                                    if ( (antennaCosts[c] + cableCosts[v] + combinerCosts[y] + splitterCosts[u]) < currLinkCost )
                                                    {
                                                        currLinkCost = antennaCosts[c] + cableCosts[v] + combinerCosts[y] + splitterCosts[u];
                                                        currAntID = c;
                                                        currCabID = v;
                                                        currComID = y;
                                                        currSplitID = u;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                            antennasUsed[currAntID] = 1;

                            int lastCounter = 0;

                            //check if all antennas have been tried
                            for ( int h = 0; h < mAntennaList.size(); h++ )
                            {
                                if ( antennasUsed[h] == 0 )
                                    lastCounter++;
                            }
                            
                            if ( lastCounter == 0 )
                                lastLink = true;

                            if ( sites[z].existingMast == true )
                            {
                                mInst.sRxHeight = sites[z].minHeight;
                                mInst.sTxHeight = sites[z].minHeight;
                            }
                            else
                            {
                                mInst.sRxHeight = mMinHeightN;
                                mInst.sTxHeight = mMinHeightN;
                            }

                            setLinkDetails(links);
                            
                            if ( links[currLinkNum].unservedReason == 0 )
                            {
                                cout << "Changed antenna " << endl;
                                sites[z].feasibleLinks++;
                                sites[links[currLinkNum].receiver].feasibleLinks++;
                                
                                links[currLinkNum].antID = currAntID;
                                links[currLinkNum].cabID = currCabID;
                                links[currLinkNum].comID = currComID;
                                links[currLinkNum].splitID = currSplitID;
                                links[currLinkNum].rxheight = mInst.sRxHeight;
                                links[currLinkNum].txheight = mInst.sTxHeight;
                                
                                links[currLinkNum].cost = 2*antennaCosts[links[currLinkNum].antID]+cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].rxheight+5);
                                links[currLinkNum].cost += cableCosts[links[currLinkNum].cabID]*(links[currLinkNum].txheight+5) + combinerCosts[links[currLinkNum].comID]+ splitterCosts[links[currLinkNum].splitID];
                                links[currLinkNum].cost += getHeightCost(links[currLinkNum].rxheight) + getHeightCost(links[currLinkNum].txheight);

                                break;
                            }                                  
                        }//while(lastLink == 0)
                    }//if(!incremented)
                }
                currLinkNum++;
            }//for (int g = 0 ; g < availableLinks[z] ; g++)
            
            cout << "Counter " << sites[z].feasibleLinks << " Made " << sites[z].savedLinks << endl;
                    
            if ( sites[z].feasibleLinks >= minLinks )
            {
                int temp = currLinkNum;
                saveChangedLinks(z, minLinks, sites, links, signalLosses, combinerLosses, splitterLosses, antennaCosts, cableCosts, combinerCosts, splitterCosts);
                currLinkNum = temp;
            }            
    }//for (int z = 0 ; z < size ; z++)

    cout << "Total Cost: " << totalCost << endl;
    cout << "Unserved after round 2: " << getUnserved(sites) << endl;
}

//*********************************************************************************************
// Count the number of links made for a particular site
//*********************************************************************************************
void Optimisation::countMadeLinks(siteDetails sites[], linkDetails links[])
{
    string query = "";
    string query2 = "";
    string query3 = "";
    pqxx::result Rad;
    pqxx::result Site1;
    pqxx::result Site2;

    bool site1found;
    bool site2found;
    int site1id;
    int site2id;

    query = "SELECT txinst, rxinst ";
    query += "FROM links;";
    gDb.PerformRawSql(query);
    gDb.GetLastResult(Rad);
    if ( Rad.size() > 0 )
    {
        for ( int k = 0; k < Rad.size(); k++ )
        {
            site1found = false;
            site2found = false;
            site1id = -1;
            site2id = -1;

            query2 = "SELECT radioinstallation.siteid ";
            query2 += "FROM radioinstallation ";
            query2 += "WHERE radioinstallation.id = ";
            query2 += Rad[k]["txinst"].c_str();
            query2 += ";";
            gDb.PerformRawSql(query2);
            gDb.GetLastResult(Site1);

            if ( Site1.size() > 0 )
            {
                for (int m = 0 ; m < size ; m++)
                {
                    if (atoi(Site1[0]["siteid"].c_str()) == mSiteList[m])
                    {
                        site1found = true;
                        site1id = m;
                        break;
                    }
                }
            }

            query3 = "SELECT radioinstallation.siteid ";
            query3 += "FROM radioinstallation ";
            query3 += "WHERE radioinstallation.id = ";
            query3 += Rad[k]["rxinst"].c_str();
            query3 += ";";
            gDb.PerformRawSql(query3);
            gDb.GetLastResult(Site2);

            if ( Site2.size() > 0 )
            {
                for (int m = 0 ; m < size ; m++)
                {
                    if (atoi(Site2[0]["siteid"].c_str()) == mSiteList[m])
                    {
                        site2found = true;
                        site2id = m;
                        break;
                    }
                }
            }

            if ( site1found && site2found )
            {
                sites[site1id].savedLinks++;
                sites[site2id].savedLinks++;
                
                int linkNumber = 0;
                
                for (int y = 0 ; y < site1id ; y++)
                {
                    linkNumber += sites[y].availableLinks;
                }
                
                linkNumber += abs(site2id - site1id) -1;
                
                links[linkNumber].saved = true;
                links[linkNumber].transmitter = site1id;
                links[linkNumber].receiver = site2id;
                links[linkNumber].linkNumber = linkNumber;
                links[linkNumber].unservedReason = 0;
            }
        }
    }
}
    
//*********************************************************************************************
// Used in round 3 to save links that have already been made feasible but not yet saved
//*********************************************************************************************
void Optimisation::saveChangedLinks(int siteNum, double minLinks, siteDetails sites[], linkDetails links[], double signalLosses[], double combinerLosses[], double splitterLosses[], double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[])
{
        int index = 0;
        int numLinksToSave = minLinks-sites[siteNum].savedLinks;
        for (int b = 0 ; b < numLinksToSave; b++)
        {
            int currIndex = 0;
            for (int h = 0 ; h < siteNum ; h++)
            {
                currIndex += sites[h].availableLinks;
            }

            cout << "currIndex " << currIndex << " Available " << sites[siteNum].availableLinks << endl;

            double lowestCost = FLT_MAX;
            index = -1;

            for (int h = 0 ; h < sites[siteNum].availableLinks ; h++)
            {
                if ((links[currIndex].unservedReason == 0)&&(links[currIndex].saved == false))
                {
                    links[currIndex].cost = 2*antennaCosts[links[currIndex].antID]+cableCosts[links[currIndex].cabID]*(links[currIndex].rxheight+5);
                    links[currIndex].cost += cableCosts[links[currIndex].cabID]*(links[currIndex].txheight+5) + combinerCosts[links[currIndex].comID]+ splitterCosts[links[currIndex].splitID];
                    links[currIndex].cost += getHeightCost(links[currIndex].rxheight) + getHeightCost(links[currIndex].txheight);
                    cout << currIndex << " costs " << links[currIndex].cost << endl;
                    if (links[currIndex].cost < lowestCost)
                    {
                        lowestCost = links[currIndex].cost;
                        index = currIndex;
                    }
                }
                currIndex++;
            }

            cout << "Index " << index << endl;

            if(index != -1)
            {
                currLinkNum = index;
                mInst.sTxHeight = links[index].txheight;
                mInst.sRxHeight = links[index].rxheight;

                mInst.sTxSysLoss = ((mInst.sTxHeight + 5) * signalLosses[links[index].cabID]) + combinerLosses[links[index].comID];
                mInst.sRxSysLoss = ((mInst.sRxHeight + 5) * signalLosses[links[index].cabID]) + splitterLosses[links[index].splitID];

                mInst.sTxPatternKey = mAntennaList[links[index].antID];
                mInst.sRxPatternKey = mAntennaList[links[index].antID];

                saveSingleLink(links);
                links[index].saved = true;
                sites[siteNum].savedLinks++;
                sites[links[index].receiver].savedLinks++;
                totalCost += links[index].cost;
                cout << "Saved link between " << siteNum << " and " << links[index].receiver << endl;
            }
            else
            {
                lowestCost = FLT_MAX;
                index = -1;

                for(int h = 0 ; h < numLinks ; h++)
                {
                    if (links[h].receiver == siteNum)
                    {
                        if((links[h].unservedReason == 0)&&(links[h].saved == false))
                        {
                            if (links[h].cost <= lowestCost)
                            {
                                lowestCost = links[h].cost;
                                index = h;
                            }
                        }
                    }
                }

                if(index != -1)
                {
                    currLinkNum = index;
                    mInst.sTxHeight = links[index].txheight;
                    mInst.sRxHeight = links[index].rxheight;

                    mInst.sTxSysLoss = ((mInst.sTxHeight + 5) * signalLosses[links[index].cabID]) + combinerLosses[links[index].comID];
                    mInst.sRxSysLoss = ((mInst.sRxHeight + 5) * signalLosses[links[index].cabID]) + splitterLosses[links[index].splitID];

                    mInst.sTxPatternKey = mAntennaList[links[index].antID];
                    mInst.sRxPatternKey = mAntennaList[links[index].antID];

                    saveSingleLink(links);
                    links[index].saved = true;
                    sites[siteNum].savedLinks++;
                    sites[links[index].transmitter].savedLinks++;
                    totalCost += links[index].cost;
                    cout << "Saved link between " << siteNum << " and " << links[index].transmitter << endl;
                }
            }
            
            if(sites[links[index].transmitter].savedLinks >= minLinks)
            {
                sites[siteNum].served = true;
                cout << links[index].transmitter << " now served" << endl;
            }

            if(sites[links[index].receiver].savedLinks >= minLinks)
            {
                sites[links[index].receiver].served = true;
                cout << links[index].receiver << " now served" << endl;
            }
        }
}

//*********************************************************************************************
// Trim the unnecessary links made during the rest of the algorithm by means of a monetary cost
// function and a link quality weighting function
//*********************************************************************************************
void Optimisation::trim(linkDetails links[], siteDetails sites[])
{
    int alreadyChecked[numLinks];
    double minWeight = DBL_MIN;
    int minIndex = -1;
    
    double minClear = DBL_MAX;
    double maxClear = DBL_MIN;
    double rangeClear = 0.0;
    double minPath = DBL_MAX;
    double maxPath = DBL_MIN;
    double rangePath = 0.0;
    double minSignalStrength = DBL_MAX;
    double maxSignalStrength = DBL_MIN;
    double rangeSignalStrength = 0.0;
    double minDistance = DBL_MAX;
    double maxDistance = DBL_MIN;
    double rangeDistance = 0.0;
    double maxVisited[numLinks];
    
    for (int u = 0 ; u < numLinks ; u++)
    {
        maxVisited[u] = 0;
    }
    
    int y = 1;
    
    while (y == 1)
    {
            int maxCost = INT_MIN;
            int maxInt = -1;
            for (int v = 0 ; v < numLinks ; v++)
            {
                 if ((links[v].cost != minCost)&&(links[v].saved == true))
                 {
                     if((links[v].cost > maxCost)&&(maxVisited[v] == 0))
                     {
                         maxInt = v;
                         maxCost = links[v].cost;
                     }
                 }
            }
            
            if (maxInt == -1)
                y = 2;
            else
            {
                maxVisited[maxInt] = 1;
                Graph g(size);

                for ( int s = 0 ; s < numLinks ; s++ )
                {
                    if ((links[s].saved == true)&&(s != maxInt))
                    {
                        g.addEdge(links[s].transmitter,links[s].receiver);
                        g.addEdge(links[s].receiver,links[s].transmitter);
                    }
                }

                bool allConnected = true;

                for(int y = 1 ; y < size ; y++)
                {
                    if((!g.isConnected(0,y))&&(!g.isConnected(y,0)))
                    {
                        allConnected = false;
                    }
                }

                if (allConnected == false)
                    links[maxInt].required = true;
                else
                    links[maxInt].required = false;

                if(links[maxInt].required == false)
                {
                    if((sites[links[maxInt].transmitter].savedLinks-1 >= mMinLinks)&&(sites[links[maxInt].receiver].savedLinks-1 >= mMinLinks))
                    {
                        currLinkNum = links[maxInt].linkNumber;
                        //removeSingleLink(links);
                        links[currLinkNum].saved = false;
                        sites[links[currLinkNum].transmitter].savedLinks--;
                        sites[links[currLinkNum].receiver].savedLinks--;
                        totalCost -= links[currLinkNum].cost;
                        cout << "Deleted extra link " << links[currLinkNum].transmitter << " x " << links[currLinkNum].receiver << endl;
                    }
                }
            }
    }  
    
    for (int h = 0 ; h < numLinks ; h++)
    {   
        if( links[h].clearance < minClear)
            minClear = links[h].clearance;

        if( links[h].clearance > maxClear)
            maxClear = links[h].clearance;

        if( links[h].pathloss < minPath)
            minPath = links[h].pathloss;

        if( links[h].pathloss > maxPath)
            maxPath = links[h].pathloss;

        if( links[h].signalstrength < minSignalStrength)
            minSignalStrength = links[h].signalstrength;

        if( links[h].signalstrength > maxSignalStrength)
            maxSignalStrength = links[h].signalstrength;

        if( links[h].distance < minDistance)
            minDistance = links[h].distance;

        if( links[h].distance > maxDistance)
            maxDistance = links[h].distance;
    }

    rangeClear = maxClear - minClear;
    rangePath = maxPath - minPath;
    rangeSignalStrength = maxSignalStrength - minSignalStrength;
    rangeDistance = maxDistance - minDistance;

    for (int h = 0 ; h < numLinks ; h++)
    {
        double clear = ((links[h].clearance-minClear)/rangeClear)*100;
        double path = ((maxPath-links[h].pathloss)/rangePath)*100;
        double signalStrength = ((links[h].signalstrength-minSignalStrength)/rangeSignalStrength)*100;
        double distance = ((maxDistance-links[h].distance)/rangeDistance)*100;

        links[h].weight = clear*0.3 + path*0.3 + signalStrength*0.3 + distance*0.1;
        alreadyChecked[h] = 0;
    }
    
    int equalLinksCount = 0; 
    
    for (int h = 0 ; h < numLinks ; h++)
    {  
        if ((links[h].cost == minCost)&&(links[h].saved == true))
            equalLinksCount++;
    }
    

    if(equalLinksCount > 11)
    {
        int x = 1;

        while (x == 1)
        {
            minIndex = -1;
            minWeight = DBL_MAX;

            for (int h = 0 ; h < numLinks ; h++)
            {
                if ((links[h].saved == true)&&(alreadyChecked[h] == 0)&&(links[h].cost == minCost))
                {
                    if (links[h].weight < minWeight)
                    {
                        minWeight = links[h].weight;
                        minIndex = h;
                    }
                }
            }

            if(minIndex == -1)
            {
                x = 2;
            }
            else
            {       
                alreadyChecked[minIndex] = 1;

                Graph g(size);

                for ( int s = 0 ; s < numLinks ; s++ )
                {
                    if ((links[s].saved == true)&&(s != minIndex))
                    {
                        g.addEdge(links[s].transmitter,links[s].receiver);
                        g.addEdge(links[s].receiver,links[s].transmitter);
                    }
                }

                bool allConnected = true;

                for(int y = 1 ; y < size ; y++)
                {
                    if((!g.isConnected(0,y))&&(!g.isConnected(y,0)))
                    {
                        allConnected = false;
                    }
                }

                g.addEdge(links[minIndex].transmitter,links[minIndex].receiver);
                g.addEdge(links[minIndex].receiver,links[minIndex].transmitter);

                bool wholeGraphConnected = true;

                for(int y = 1 ; y < size ; y++)
                {
                    if((!g.isConnected(0,y))&&(!g.isConnected(y,0)))
                    {
                        wholeGraphConnected = false;
                    }
                }

                if ((allConnected == false)&&(wholeGraphConnected == true))
                    links[minIndex].required = true;
                else
                    links[minIndex].required = false;

                if (links[minIndex].required == false)
                {
                    if((sites[links[minIndex].transmitter].savedLinks-1 >= mMinLinks)&&(sites[links[minIndex].receiver].savedLinks-1 >= mMinLinks))
                    {
                        currLinkNum = links[minIndex].linkNumber;
                        //removeSingleLink(links);
                        links[currLinkNum].saved = false;
                        sites[links[currLinkNum].transmitter].savedLinks--;
                        sites[links[currLinkNum].receiver].savedLinks--;
                        totalCost -= links[currLinkNum].cost;
                        cout << "Deleted extra link " << links[currLinkNum].transmitter << " x " << links[currLinkNum].receiver << endl;
                    }
                }
            }
        } 
    }
    else if(equalLinksCount > 1)
    {

        int trimPermutations = 0; 
        linkDetails linksTemp[numLinks];
        siteDetails sitesTemp[size];
        linkDetails linksMin[numLinks];
        siteDetails sitesMin[size];
        
         double minavg = DBL_MAX;
         int minsavedCount = 0;
         double minsum = 0.0;
        
        trimPermutations = factorial(equalLinksCount);
        
        int permNumbers[equalLinksCount];
        int visitOrder[equalLinksCount];
        
        for (int g = 0 ; g < equalLinksCount ; g++)
        {
                permNumbers[g] = g;
                visitOrder[g] = g;
        }    
        
        int linksLeft;
        int visited[numLinks];
        double totalCostTemp = 0;
        double totalCostMin = 0;
        
        int minNumLinksLeft = INT_MAX;
        int smallest = -1;
        
        for (int k = 0 ; k < trimPermutations ; k++)
        {
            bool carryOn = false;
            
            if (equalLinksCount > 9)
            {
                if (k % trimPermutations/3600 == 0)
                    carryOn = true;
            }
            else
                carryOn = true;
            
            if(carryOn)
            {
                linksLeft = equalLinksCount;

                totalCostTemp = totalCost;

                for ( int n = 0; n < size; n++ )
                {
                    sitesTemp[n].existingMast = sites[n].existingMast;
                    sitesTemp[n].minHeight = sites[n].minHeight;
                    sitesTemp[n].maxHeight = sites[n].maxHeight;
                    sitesTemp[n].served = sites[n].served;
                    sitesTemp[n].feasibleLinks = sites[n].feasibleLinks;
                    sitesTemp[n].savedLinks = sites[n].savedLinks;
                    sitesTemp[n].availableLinks = sites[n].availableLinks;
                }

                for (int j = 0 ; j < numLinks ; j++)
                {
                        linksTemp[j].unservedReason = links[j].unservedReason;
                        linksTemp[j].transmitter = links[j].transmitter;
                        linksTemp[j].receiver = links[j].receiver;
                        linksTemp[j].linkNumber = links[j].linkNumber;
                        linksTemp[j].rxheight = links[j].rxheight;
                        linksTemp[j].txheight = links[j].txheight;
                        linksTemp[j].antID = links[j].antID;
                        linksTemp[j].cabID = links[j].cabID;
                        linksTemp[j].comID = links[j].comID;
                        linksTemp[j].splitID = links[j].splitID;
                        linksTemp[j].saved = links[j].saved;
                        linksTemp[j].required = links[j].required;
                        linksTemp[j].cost = links[j].cost;
                        linksTemp[j].clearance = links[j].clearance;
                        linksTemp[j].pathloss = links[j].pathloss;
                        linksTemp[j].signalstrength = links[j].signalstrength;
                        linksTemp[j].distance = links[j].distance;
                        linksTemp[j].weight = links[j].weight;
                } 

                for (int l = 0 ; l < numLinks ; l++)
                { 
                    if ((linksTemp[l].cost == minCost)&&(linksTemp[l].saved == true))
                        visited[l] = 0;
                    else
                        visited[l] = 1;
                }

                int visitLinks[equalLinksCount];
                int indexCount = 0;
                
                for( int d = 0 ; d < numLinks ; d++)
                {
                    if(visited[d] == 0)
                    {
                        visitLinks[indexCount] = linksTemp[d].linkNumber;
                        indexCount++;
                    }
                }

                for (int v = 0 ; v < equalLinksCount ; v++)
                {
                        int nextIndex = 0;
                        int random = visitOrder[v];

                        for (int a = 0 ; a < numLinks ; a++)
                        {
                            if(linksTemp[a].linkNumber == visitLinks[random])
                                nextIndex = a;
                        }

                        Graph g(size);

                        for ( int s = 0 ; s < numLinks ; s++ )
                        {
                            if ((linksTemp[s].saved == true)&&(s != nextIndex))
                            {
                                g.addEdge(linksTemp[s].transmitter,linksTemp[s].receiver);
                                g.addEdge(linksTemp[s].receiver,linksTemp[s].transmitter);
                            }
                        }

                        bool allConnected = true;

                        for(int y = 1 ; y < size ; y++)
                        {
                            if((!g.isConnected(0,y))&&(!g.isConnected(y,0)))
                            {
                                allConnected = false;
                            }
                        }

                        g.addEdge(linksTemp[nextIndex].transmitter,linksTemp[nextIndex].receiver);
                        g.addEdge(linksTemp[nextIndex].receiver,linksTemp[nextIndex].transmitter);

                        bool wholeGraphConnected = true;

                        for(int y = 1 ; y < size ; y++)
                        {
                            if((!g.isConnected(0,y))&&(!g.isConnected(y,0)))
                            {
                                wholeGraphConnected = false;
                            }
                        }

                        if ((allConnected == false)&&(wholeGraphConnected == true))
                            linksTemp[nextIndex].required = true;
                        else
                            linksTemp[nextIndex].required = false;

                        if (linksTemp[nextIndex].required == false)
                        {
                            if((sitesTemp[linksTemp[nextIndex].transmitter].savedLinks-1 >= mMinLinks)&&(sitesTemp[linksTemp[nextIndex].receiver].savedLinks-1 >= mMinLinks))
                            {
                                currLinkNum = linksTemp[nextIndex].linkNumber;
                                //removeSingleLink(links);
                                linksLeft--;
                                linksTemp[currLinkNum].saved = false;
                                sitesTemp[linksTemp[currLinkNum].transmitter].savedLinks--;
                                sitesTemp[linksTemp[currLinkNum].receiver].savedLinks--;
                                totalCostTemp -= linksTemp[currLinkNum].cost;
                            }
                        }
                }

                if (linksLeft < minNumLinksLeft)
                {
                    smallest = k;
                    minNumLinksLeft = linksLeft;

                    for ( int n = 0; n < size; n++ )
                    {
                        sitesMin[n].existingMast = sitesTemp[n].existingMast;
                        sitesMin[n].minHeight = sitesTemp[n].minHeight;
                        sitesMin[n].maxHeight = sitesTemp[n].maxHeight;
                        sitesMin[n].served = sitesTemp[n].served;
                        sitesMin[n].feasibleLinks = sitesTemp[n].feasibleLinks;
                        sitesMin[n].savedLinks = sitesTemp[n].savedLinks;
                        sitesMin[n].availableLinks = sitesTemp[n].availableLinks;
                    }

                    for (int j = 0 ; j < numLinks ; j++)
                    {
                            linksMin[j].unservedReason = linksTemp[j].unservedReason;
                            linksMin[j].transmitter = linksTemp[j].transmitter;
                            linksMin[j].receiver = linksTemp[j].receiver;
                            linksMin[j].linkNumber = linksTemp[j].linkNumber;
                            linksMin[j].rxheight = linksTemp[j].rxheight;
                            linksMin[j].txheight = linksTemp[j].txheight;
                            linksMin[j].antID = linksTemp[j].antID;
                            linksMin[j].cabID = linksTemp[j].cabID;
                            linksMin[j].comID = linksTemp[j].comID;
                            linksMin[j].splitID = linksTemp[j].splitID;
                            linksMin[j].saved = linksTemp[j].saved;
                            linksMin[j].required = linksTemp[j].required;
                            linksMin[j].cost = linksTemp[j].cost;
                            linksMin[j].clearance = linksTemp[j].clearance;
                            linksMin[j].pathloss = linksTemp[j].pathloss;
                            linksMin[j].signalstrength = linksTemp[j].signalstrength;
                            linksMin[j].distance = linksTemp[j].distance;
                            linksMin[j].weight = linksTemp[j].weight;
                    } 
                    totalCostMin = totalCostTemp;

                    minsum = 0;
                    minsavedCount = 0;

                    for (int u = 0 ; u < numLinks ; u++)
                    {
                        if(linksTemp[u].saved == true)
                        {
                            minsum += linksTemp[u].weight;
                            minsavedCount++;
                        }
                    }

                    minavg = minsum/minsavedCount;
                }
                else if (linksLeft == minNumLinksLeft)
                {
                    double avg = 0.0;
                    int savedCount = 0;
                    double sum = 0.0;

                    for (int u = 0 ; u < numLinks ; u++)
                    {
                        if(linksTemp[u].saved == true)
                        {
                            sum += linksTemp[u].weight;
                            savedCount++;
                        }
                    }

                    avg = sum/savedCount;

                    if(avg > minavg)
                    {
                        smallest = k;
                        minNumLinksLeft = linksLeft;

                        for ( int n = 0; n < size; n++ )
                        {
                            sitesMin[n].existingMast = sitesTemp[n].existingMast;
                            sitesMin[n].minHeight = sitesTemp[n].minHeight;
                            sitesMin[n].maxHeight = sitesTemp[n].maxHeight;
                            sitesMin[n].served = sitesTemp[n].served;
                            sitesMin[n].feasibleLinks = sitesTemp[n].feasibleLinks;
                            sitesMin[n].savedLinks = sitesTemp[n].savedLinks;
                            sitesMin[n].availableLinks = sitesTemp[n].availableLinks;
                        }

                        for (int j = 0 ; j < numLinks ; j++)
                        {
                                linksMin[j].unservedReason = linksTemp[j].unservedReason;
                                linksMin[j].transmitter = linksTemp[j].transmitter;
                                linksMin[j].receiver = linksTemp[j].receiver;
                                linksMin[j].linkNumber = linksTemp[j].linkNumber;
                                linksMin[j].rxheight = linksTemp[j].rxheight;
                                linksMin[j].txheight = linksTemp[j].txheight;
                                linksMin[j].antID = linksTemp[j].antID;
                                linksMin[j].cabID = linksTemp[j].cabID;
                                linksMin[j].comID = linksTemp[j].comID;
                                linksMin[j].splitID = linksTemp[j].splitID;
                                linksMin[j].saved = linksTemp[j].saved;
                                linksMin[j].required = linksTemp[j].required;
                                linksMin[j].cost = linksTemp[j].cost;
                                linksMin[j].clearance = linksTemp[j].clearance;
                                linksMin[j].pathloss = linksTemp[j].pathloss;
                                linksMin[j].signalstrength = linksTemp[j].signalstrength;
                                linksMin[j].distance = linksTemp[j].distance;
                                linksMin[j].weight = linksTemp[j].weight;
                        } 
                        totalCostMin = totalCostTemp;
                        minavg = avg;
                    }
                }
            }
            
            if(next_permutation(permNumbers,permNumbers+equalLinksCount))
            {
                    for (int g = 0 ; g < equalLinksCount ; g++)
                    {
                            visitOrder[g] = permNumbers[g];
                    }           
            }
        }
        
        if (smallest != -1)
        {
                for ( int n = 0; n < size; n++ )
                {
                    sites[n].existingMast = sitesMin[n].existingMast;
                    sites[n].minHeight = sitesMin[n].minHeight;
                    sites[n].maxHeight = sitesMin[n].maxHeight;
                    sites[n].served = sitesMin[n].served;
                    sites[n].feasibleLinks = sitesMin[n].feasibleLinks;
                    sites[n].savedLinks = sitesMin[n].savedLinks;
                    sites[n].availableLinks = sitesMin[n].availableLinks;
                }

                for (int j = 0 ; j < numLinks ; j++)
                {
                        links[j].unservedReason = linksMin[j].unservedReason;
                        links[j].transmitter = linksMin[j].transmitter;
                        links[j].receiver = linksMin[j].receiver;
                        links[j].linkNumber = linksMin[j].linkNumber;
                        links[j].rxheight = linksMin[j].rxheight;
                        links[j].txheight = linksMin[j].txheight;
                        links[j].antID = linksMin[j].antID;
                        links[j].cabID = linksMin[j].cabID;
                        links[j].comID = linksMin[j].comID;
                        links[j].splitID = linksMin[j].splitID;
                        links[j].saved = linksMin[j].saved;
                        links[j].required = linksMin[j].required;
                        links[j].cost = linksMin[j].cost;
                        links[j].clearance = linksMin[j].clearance;
                        links[j].pathloss = linksMin[j].pathloss;
                        links[j].signalstrength = linksMin[j].signalstrength;
                        links[j].distance = linksMin[j].distance;
                        links[j].weight = linksMin[j].weight;
                } 
                totalCost = totalCostMin;
        }
        
    }
}

//*********************************************************************************************
// Return the factorial of a number
//*********************************************************************************************
int Optimisation::factorial(int x, int result)
{
    if (x == 1)
        return result;
    else
        return factorial(x-1, x*result);
}

//*********************************************************************************************
// Can be used to delete a particular link from the database
//*********************************************************************************************
void Optimisation::removeSingleLink(linkDetails links[])
{
        string query = "";
        string query2 = "";
        string query3 = "";
        pqxx::result Rad;
        pqxx::result Site1;
        pqxx::result Site2;

        bool site1found;
        bool site2found;

        query = "SELECT id, txinst, rxinst ";
        query += "FROM links;";
        gDb.PerformRawSql(query);
        gDb.GetLastResult(Rad);
        if ( Rad.size() > 0 )
        {
            for ( int k = 0; k < Rad.size(); k++ )
            {
                site1found = false;
                site2found = false;

                query2 = "SELECT radioinstallation.siteid ";
                query2 += "FROM radioinstallation ";
                query2 += "WHERE radioinstallation.id = ";
                query2 += Rad[k]["txinst"].c_str();
                query2 += ";";
                gDb.PerformRawSql(query2);
                gDb.GetLastResult(Site1);

                if ( Site1.size() > 0 )
                {
                    if ( (atoi(Site1[0]["siteid"].c_str()) == mSiteList[links[currLinkNum].transmitter]) || (atoi(Site1[0]["siteid"].c_str()) == mSiteList[links[currLinkNum].receiver]) )
                        site1found = true;
                }

                query3 = "SELECT radioinstallation.siteid ";
                query3 += "FROM radioinstallation ";
                query3 += "WHERE radioinstallation.id = ";
                query3 += Rad[k]["rxinst"].c_str();
                query3 += ";";
                gDb.PerformRawSql(query3);
                gDb.GetLastResult(Site2);

                if ( Site2.size() > 0 )
                {
                    if ( (atoi(Site2[0]["siteid"].c_str()) == mSiteList[links[currLinkNum].transmitter]) || (atoi(Site2[0]["siteid"].c_str()) == mSiteList[links[currLinkNum].receiver]) )
                        site2found = true;
                }

                if ( site1found && site2found )
                {
                    string query4;

                    query4 = "SELECT id, rxinst, txinst from links where id =";
                    query4 += Rad[k]["id"].c_str();
                    
                    if (gDb.PerformRawSql(query4))
                    {
                            pqxx::result RI;
                            gDb.GetLastResult(RI);
                            query4 = "DELETE FROM radioinstallation where id in (";
                            query4 += RI[0]["txinst"].c_str();
                            query4 += ",";
                            query4 += RI[0]["rxinst"].c_str();
                            query4 += ");";
                            gDb.PerformRawSql(query4);
                    }
                    query4 = "DELETE FROM links where id = ";
                    query4 += Rad[k]["id"].c_str();
                    
                    gDb.PerformRawSql(query4);                   
                    break;
                }
            }
        }    
}

//*********************************************************************************************
// Find crucial links to make the area of sites fully connected between served sites
//*********************************************************************************************
void Optimisation::findVitalLinks(linkDetails links[], int size, siteDetails sites[], double signalLosses[], double combinerLosses[], double splitterLosses[])
{
    allLinks = 0;
    for (int k = 0 ; k < numLinks ; k++)
    {
        if(links[k].unservedReason == 0)
                allLinks++;
    }
      
    bool allConnected2;
    int linksToAdd[numLinks];
    
    for(int w = 0 ; w < numLinks ; w++)
    {
        linksToAdd[w] = -1;
    }
    
        for( int s = 0 ; s < numLinks ; s++)
        {   
            if((links[s].unservedReason == 0)&&(links[s].saved == false))
            {                
                
                Graph g2(allLinks);
                allConnected2 = true;

                for( int t = 0 ; t < numLinks ; t++)
                {
                     if(links[t].saved == true)
                     {
                         g2.addEdge(links[t].transmitter,links[t].receiver);
                         g2.addEdge(links[t].receiver,links[t].transmitter);
                     }
                }
                
                bool allConnected = true;

                for(int y = 1 ; y < size ; y++)
                {
                    if((!g2.isConnected(0,y))&&(!g2.isConnected(y,0)))
                    {
                        allConnected = false;
                    }
                }
                
                g2.addEdge(links[s].transmitter,links[s].receiver);
                g2.addEdge(links[s].receiver,links[s].transmitter);

                for(int y = 1 ; y < size ; y++)
                {
                    if((!g2.isConnected(0,y))&&(!g2.isConnected(y,0)))
                    {
                        allConnected2 = false;
                        break;
                    }
                }

                if((allConnected2 == true)&&(allConnected == false))
                {
                    linksToAdd[s] = s;
                }
            }
        }  
    
    for (int w = 0 ; w < numLinks ; w++)
    {
        if(linksToAdd[w] != -1)
        {
            currLinkNum = linksToAdd[w];
            mInst.sTxHeight = links[currLinkNum].txheight;
            mInst.sRxHeight = links[currLinkNum].rxheight;

            mInst.sTxSysLoss = ((mInst.sTxHeight + 5) * signalLosses[links[currLinkNum].cabID]) + combinerLosses[links[currLinkNum].comID];
            mInst.sRxSysLoss = ((mInst.sRxHeight + 5) * signalLosses[links[currLinkNum].cabID]) + splitterLosses[links[currLinkNum].splitID];

            mInst.sTxPatternKey = mAntennaList[links[currLinkNum].antID];
            mInst.sRxPatternKey = mAntennaList[links[currLinkNum].antID];

            saveSingleLink(links);
            links[currLinkNum].saved = true;
            links[currLinkNum].required = true;
            sites[links[currLinkNum].transmitter].savedLinks++;
            sites[links[currLinkNum].receiver].savedLinks++;
            totalCost += links[currLinkNum].cost;
            cout << "Saved extra link " << links[currLinkNum].transmitter << " x " << links[currLinkNum].receiver << endl;
        }
    }
    
}
 

//*********************************************************************************************
// Graph Class used for checking for full connectivity in the selected area
//*********************************************************************************************

//*********************************************************************************************
// Constructor
//*********************************************************************************************
Graph::Graph(int allLinks)
{
    this->allLinks = allLinks;
    adj = new list<int>[allLinks];
}
 
//*********************************************************************************************
// Add an edge to the graph from a source to a destination node
//*********************************************************************************************
void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

//*********************************************************************************************
// Check if a source node is able to connect to a destination node
//*********************************************************************************************
bool Graph::isConnected(int source, int dest)
{
    if (source == dest)
      return true;
 
    bool *visited = new bool[allLinks];
    for (int i = 0; i < allLinks; i++)
        visited[i] = false;
    list<int> queue;
 
    visited[source] = true;
    queue.push_back(source);

    list<int>::iterator i;
 
    while (!queue.empty())
    {
        source = queue.front();
        queue.pop_front();

//loop through all the adjacencies of the source node to find one to the destination
        for (i = adj[source].begin(); i != adj[source].end(); ++i)
        {
            if (*i == dest)
                return true;

            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }
    return false;
}

