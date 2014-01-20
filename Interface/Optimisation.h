 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : Optimisation.h
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Gabriel de Sousa (gdesousa26@gmail.com) 
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za) 
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

#ifndef OPTIMISATION_H
#define OPTIMISATION_H

#include "../DataBase/Config.h"
#include <qgspoint.h>
#include "../Prediction/cGeoP.h"
#include "../Prediction/cLink.h"
#include "../Prediction/PredStructs.h"
#include "cLinkAnalysis.h"
#include <string>
#include <QtCore/QList>
#include "ui_Optimisation.h"

#include<iostream>
#include <list>
#include <string>
#include <limits.h>
#include <ctime>

using namespace std;
using namespace Qrap;

	//struct for links
        struct linkDetails
        {
            int unservedReason; // 0=default, 1=pathloss/signal strength, 2=clearance, 3=both, 4=distance
            int transmitter;
            int receiver;
            int linkNumber;
            int rxheight;
            int txheight;
            int antID;
            int cabID;
            int comID;
            int splitID;
            bool saved;
            bool required;
            double cost;
            double clearance;
            double pathloss;
            double signalstrength;
            double weight;
            double distance;
        };
        
	//struct for sites
        struct siteDetails
        {
            bool existingMast;
            int minHeight;
            int maxHeight;
            bool served;
            int feasibleLinks;
            int savedLinks;
            int availableLinks;
        };

	class Optimisation: public QDialog, private Ui::Optimisation
	{Q_OBJECT
		
		public:
                        Optimisation();
                        Optimisation( QWidget* parent = 0, Qt::WFlags fl = 0 );
                        virtual ~Optimisation();
                        bool SetPoints(QList<QgsPoint> Points);
                        bool LoadAntennas();
                        bool LoadCables();
                        bool LoadConnectors();
                        

		public slots:
			void on_pushCancel_clicked();
			void on_pushOk_clicked();
			void checkBoxChanged(int state);
			void chkClearanceChanged(int state);
			void chkStrengthChanged(int state);
                        void chkSignalStrengthChanged(int state);
                        void chkSensitivityChanged(int state);
                        void chkPowerChanged(int state);

		private slots:

			
		private:
			void LoadDefaults();
                        void optimise();
                        int countLinks(int index, linkDetails links[], bool set);
                        void setLinkDetails(linkDetails links[]);
                        void saveLinks(int index, siteDetails sites[], linkDetails links[]);
                        void saveSingleLink(linkDetails links[]);
                        void removeSingleLink(linkDetails links[]);
                        void countMadeLinks(siteDetails sites[], linkDetails links[]);
                        void trim(linkDetails links[], siteDetails sites[]);
                        void updateLinkStatusPlanned(linkDetails links[]);
                        void updateLinkStatusExperimental(linkDetails links[]);
                        void saveChangedLinks(int siteNum, double minLinks, siteDetails site[], linkDetails links[], double signalLosses[], double combinerLosses[], double splitterLosses[], double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[]);
                        void round1(double minLinks, double signalLosses[], double combinerLosses[], double splitterLosses[], siteDetails sites[], linkDetails links[], int antennasUsed[], double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[]);
                        void round2(double minLinks, double signalLosses[], double combinerLosses[], double splitterLosses[], siteDetails sites[], linkDetails links[], int antennasUsed[], double antennaCosts[], double cableCosts[], double combinerCosts[], double splitterCosts[]);
                        bool LinkExists(int txradid, int rxradid);
                        bool isExperimental(int txradid, int rxradid);
                        void findVitalLinks(linkDetails links[], int size, siteDetails sites[], double signalLosses[], double combinerLosses[], double splitterLosses[]);
                        int InsertRadInst(int SiteID, int Sector=1, double Bearing=0.0, double Tilt=0.0);
                        void LoadCosts(double antennaCosts[], double cableCosts[], bool connectorList[], double combinerCosts[], double splitterCosts[], double signalLosses[], double combinerLosses[], double splitterLosses[], siteDetails sites[]);
                        double getPower(string Rate);
                        double getSensitivity(string Rate);
                        double getHeightCost(int Height);
                        string convertInt(int number);
                        int factorial(int x, int result = 1);
                        int getUnserved(siteDetails sites[]);

			QList<unsigned> mSiteList;
                        QList<unsigned> mAntennaList;
                        QList<unsigned> mCableList;
                        QList<unsigned> mConnectorList;
			double mMinClear;
			double mMaxPath;
			double mMaxDist;
			double mkFactor;
                        double mMinHeightN;
                        double mMaxHeightN;
                        double mPower;
                        double mSensitivity;
                        double mSignalStrength;
                        double mMinLinks;
                        string mRate;
                        int mTechKey;
			int mProject;
                        int currLinkNum;
                        int numLinks;
			string mTechnology;
                        string mProjectName;
                        string mMinNumLinks;
                        QString path;
                        bool SavePDF;
			tFixed mInst;
			QWidget* mParent;
			Qt::WFlags mFL;
                        
                        int size;
                        int numCombiners;
                        int numSplitters;
                        int allLinks;
                        int currAntID;
                        int currCabID;
                        int currComID;
                        int currSplitID;
                        double minCost;
                        double totalCost;
                        float currLinkCost;

                        time_t tstart, tend;

                        //Plotting variables
                        eOutputUnits Units;
                        cLink Link;
                        double PlotResolution;
                        double tempPlotRes;
                        string Down;
                        bool DownLink;
                        string Clutter;
                        bool UseClutter;
                        short int ClutterSource;
                        short int DEMsource;
                        QString Transmitter;
                        QString Receiver;
                        
                };
                
 
// Graph class used to check for full connectivity in the selected area
class Graph
{
    int allLinks;
    list<int> *adj;
    
public:
    Graph(int allLinks);
    void addEdge(int v, int w);
    bool isConnected(int source, int dest);
};

#endif
