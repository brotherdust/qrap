 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : PreferencesDialog.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Roeland van Nieukerk (roeland.frans@gmail.com)
 *                : (Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Qt4 dialog widget that presents an interface that the 
 *                  user can use to change preferences to the user interface.
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

#include "PreferencesDialog.h"

using namespace std;
using namespace Qrap;

// The contructor
PreferencesDialog::PreferencesDialog (QWidget* parent) : QDialog(parent)
{
	setWindowModality(Qt::WindowModal);
	resize(696, 561);
	setWindowIcon(QIcon(QString::fromUtf8(":/images/logo_icon.png")));
	setWindowTitle(QApplication::translate("PreferencesDialog", "QRAP Preferences", 0, QApplication::UnicodeUTF8));
	
	// Setup the preferences dialog
	SetupUi();
}

//***********************************************************************************
// Write all the settings to the database
void PreferencesDialog::WriteSettings ()
{
	// Set the sensitivity
	if(receiverPower->isChecked())
	{
		gDb.SetSetting("sensitivity","dBm");
	} // if receiverPower
	else
	{
		if(receiverVoltage->isChecked())
		{
			gDb.SetSetting("sensitivity","dB\316\274V");
		} // if receiverVoltage
		else
		{
			if(receiverdBW->isChecked())
			{
				gDb.SetSetting("sensitivity","dBW");
			} // if receiverdBW
			else
			{
				gDb.SetSetting("sensitivity","false");
			} // else receiverdBW
		} // else receiverVoltage
	} // else receverPower
	
	// Set the power
	if(powerWattRadio->isChecked())
	{
		gDb.SetSetting("power","W");
	} // if powerWattRadio
	else
	{
		if(powerdBmRadio->isChecked())
		{
			gDb.SetSetting("power","dBm");
		} // if powerdBmRadio
		else
		{
			if(powerdBWRadio->isChecked())
			{
				gDb.SetSetting("power","dBW");
			} // if powerdBWRadio
			else
			{
				gDb.SetSetting("power","false");
			} // else powerdBWRadio
		} // else powerdBmRadio
	} // else powerWattRadio
	
	
	// Set the power
	if(eirpWattRadio->isChecked())
	{
		gDb.SetSetting("eirp","W");
	} // if eirpWattRadio
	else
	{
		if(eirpdBmRadio->isChecked())
		{
			gDb.SetSetting("eirp","dBm");
		} // if eirpdBmRadio
		else
		{
			if(eirpdBWRadio->isChecked())
			{
				gDb.SetSetting("eirp","dBW");
			} // if eirpdBWRadio
			else
			{
				gDb.SetSetting("eirp","false");
			} // else eirpdBWRadio
		} // else eirpdBmRadio
	} // else eirpWattRadio
	
		
	// Set the receiver input impedence
	if(fiftyOhm->isChecked())
	{
		gDb.SetSetting("impedence","50");
	} // if fiftyOhm
	else
	{
		if(seventyFiveOhm->isChecked())
		{
			gDb.SetSetting("impedence","75");
		} // if seventyFiveOhm
		else
		{
			gDb.SetSetting("impedence","false");
		} // else seventyFiveOhm
	} // else fiftyOhm
	
	// Set the prediction EIRP
	gDb.SetSetting("predictioneirp","explicit");
	if(explicitEIRP->isChecked())
	{
		gDb.SetSetting("predictioneirp","explicit");
	} // if explicitEIRP
	else
	{
		if(calculatedEIRP->isChecked())
		{
			gDb.SetSetting("predictioneirp","calculated");
		} // if calculatedEIRP
		else
		{
			gDb.SetSetting("predictioneirp","false");
		} // else calculatedEIRP
	} // else explicit EIRP
	
	// Set the location format
	if(DDMMSSRadio->isChecked())
	{
		gDb.SetSetting("location","DD:MM:SS");
	} // if DDMMSSRadio
	else
	{
		if(DDMMmmRadio->isChecked())
		{
			gDb.SetSetting("location","DD:MM.mm");
		} // if DDMMmmRadio
		else
		{
			if(DDddddRadio->isChecked())
			{
				gDb.SetSetting("location","DD.dddd");
			} // if DDddddRadio
			else
			{
				gDb.SetSetting("location","false");
			} // else DDddddRadio
		} // else DDMMmmRadio
	} // if DDMMSSRadio

	
	
	// Set the interface mode
	if(editNormalRadio->isChecked())
	{
		gDb.SetSetting("mode","normal");
	} // if editNormalRadio
	else
	{
		if(editDefaultsRadio->isChecked())
		{
			gDb.SetSetting("mode","defaults");
		} // if editDefaultsMode
		else
		{
			gDb.SetSetting("mode","false");
		} // else editDefaultsMode
	} // else editNormalRadio
	
	accept();
	
	double Double;
	char text[33];
	// Write the display units
	gDb.SetSetting("DisplayUnits",displayUnitsCombo->currentText().toStdString());
	
	// Write the default technology type
	gDb.SetSetting("TechType",technologyCombo->currentText().toStdString());
	
	// Write the RqSN
	Double = RqSNSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("RqSN",text);
	
	// Write the FadeMargin
	Double = fadeMarginSpin->value();
	gcvt(Double,10,text);
	gDb.SetSetting("FadeMargin",text);
	
	// Write the RxMin
	Double = rxMinSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("RxMin",text);
	
	// Write the RqCoci
	Double = rqCociSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("RqCiCo",text);
	
	// Write the RqCiad
	Double = rqCiadSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("RqCiad",text);
	
	// Write the RqEbNo
	Double = rqEbNoSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("RqEbNo",text);
	
	// Write the kFactorServer
	Double = kFactorServerSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("kFactorServer",text);
	
	// Write the kFactorInt
	Double = kFactorIntSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("kFactorInt",text);
	
	// Write the SiteSelectSense
	Double = siteSelectSenseSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("SiteSelectSense",text);
	
	// Write the PlotResolution
	Double = plotResolutionSpinBox->value();
	gcvt(Double,10,text);
	gDb.SetSetting("PlotResolution",text);
	
	// Write the BTLDir

	gDb.SetSetting("BTLDir",btlDirectoryEdit->text().toStdString());
	
	// Write the OutputDir
	gDb.SetSetting("OutputDir",outputDirectoryEdit->text().toStdString());
	
	// Write the UseClutter
	if(useClutterCheckBox->isChecked())
		gDb.SetSetting("UseClutter","true");
	else
		gDb.SetSetting("UseClutter","false");
	
	
	// Write the down link
	if(downLinkRadio->isChecked())
		gDb.SetSetting("DownLink","true");
	else
		if(upLinkRadio->isChecked())
			gDb.SetSetting("DownLink","false");
		else
			gDb.SetSetting("DownLink","false");
}


//***********************************************************************************
// Read all the settings from the database
void PreferencesDialog::ReadSettings ()
{
	string setting;
	
	setting = gDb.GetSetting("sensitivity");
	
	if(setting=="dBm")
	{
		receiverPower->setChecked(true);
	} // if dBm
	else
	{
		if(setting=="dB\316\274V")
		{
			receiverVoltage->setChecked(true);
		} // if dBuV
		else
		{
			if(setting=="dBW")
			{
				receiverdBW->setChecked(true);
			} // if dBW
			else
			{
				receiverPower->setChecked(true);
			} // else dBW
		} // else dBuV
	} // else dBm
	
	setting = gDb.GetSetting("power");
	
	if(setting=="W")
	{
		powerWattRadio->setChecked(true);
	} // if W
	else
	{
		if(setting=="dBm")
		{
			powerdBmRadio->setChecked(true);
		} // if dBm
		else
		{
			if(setting=="dBW")
			{
				powerdBWRadio->setChecked(true);
			} // if dBW
			else
			{
				powerWattRadio->setChecked(true);
			} // else dBW
		} // else dBm
	} // else W
	
	
	setting = gDb.GetSetting("eirp");
	
	if(setting=="W")
	{
		eirpWattRadio->setChecked(true);
	} // if W
	else
	{
		if(setting=="dBm")
		{
			eirpdBmRadio->setChecked(true);
		} // if dBm
		else
		{
			if(setting=="dBW")
			{
				eirpdBWRadio->setChecked(true);
			} // if dBW
			else
			{
				eirpWattRadio->setChecked(true);
			} // else dBW
		} // else dBm
	} // else W
	
	
	setting = gDb.GetSetting("impedence");
	
	if(setting=="50")
	{
		fiftyOhm->setChecked(true);
	} // if 50
	else
	{
		if(setting=="75")
		{
			seventyFiveOhm->setChecked(true);
		} // if 75
		else
		{
			fiftyOhm->setChecked(true);
		} // else 75
	} // else 50
	
	setting = gDb.GetSetting("predictioneirp");
	if(setting=="explicit")
	{
		explicitEIRP->setChecked(true);
	} // if explicit
	else
	{
		if(setting=="calculated")
		{
			calculatedEIRP->setChecked(true);
		} // if calculated
		else
		{
			explicitEIRP->setChecked(true);
		} // else calculated
	} // else explicit
	explicitEIRP->setChecked(true);
	calculatedEIRP->setChecked(false);
	
	setting = gDb.GetSetting("location");
	
	if(setting=="DD:MM:SS")
	{
		DDMMSSRadio->setChecked(true);
	} // if DD:MM:SS
	else
	{
		if(setting=="DD:MM.mm")
		{
			DDMMmmRadio->setChecked(true);
		} // if DD:MM.mm
		else
		{
			if(setting=="DD.dddd")
			{
				DDddddRadio->setChecked(true);
			} // if DD.dddd
			else
			{
				DDMMSSRadio->setChecked(true);
			} // else DD.dddd
		} // else DD:MM.mm
	} // else DD:MM:SS
	
	setting = gDb.GetSetting("mode");
	
	if(setting=="normal")
	{
		editNormalRadio->setChecked(true);
	} // if normal
	else
	{
		if(setting=="defaults")
		{
			editDefaultsRadio->setChecked(true);
		} // if defaults
		else
		{
			editNormalRadio->setChecked(true);
		} // else defaults
	} // if normal
	
	setting = gDb.GetSetting("DisplayUnits");
	if(setting!="")
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText(QString::fromStdString(setting),Qt::MatchCaseSensitive));
	else
		displayUnitsCombo->setCurrentIndex(displayUnitsCombo->findText("dBm",Qt::MatchCaseSensitive));
	
	setting = gDb.GetSetting("TechType");
	if(setting!="")
		technologyCombo->setCurrentIndex(technologyCombo->findText(QString::fromStdString(setting),Qt::MatchCaseSensitive));
	else
		technologyCombo->setCurrentIndex(technologyCombo->findText("0:NULL",Qt::MatchCaseSensitive));

	setting = gDb.GetSetting("RqSN");
	if(setting!="")
		RqSNSpinBox->setValue(atof(setting.c_str()));
	else
		RqSNSpinBox->setValue(8.00);
	
	setting = gDb.GetSetting("FadeMargin");
	if(setting!="")
		fadeMarginSpin->setValue(atof(setting.c_str()));
	else
		fadeMarginSpin->setValue(3.00);
	
	setting = gDb.GetSetting("RxMin");
	if(setting!="")
		rxMinSpinBox->setValue(atof(setting.c_str()));
	else
		rxMinSpinBox->setValue(-110.00);
	
	setting = gDb.GetSetting("RqCiCo");
	if(setting!="")
		rqCociSpinBox->setValue(atof(setting.c_str()));
	else
		rqCociSpinBox->setValue(9.00);
	
	setting = gDb.GetSetting("RqCiad");
	if(setting!="")
		rqCiadSpinBox->setValue(atof(setting.c_str()));
	else
		rqCiadSpinBox->setValue(-9.00);
	
	setting = gDb.GetSetting("RqEbNo");
	if(setting!="")
		rqEbNoSpinBox->setValue(atof(setting.c_str()));
	else
		rqEbNoSpinBox->setValue(8.00);
	
	setting = gDb.GetSetting("kFactorServer");
	if(setting!="")
		kFactorServerSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorServerSpinBox->setValue(1.00);
	
	setting = gDb.GetSetting("kFactorInt");
	if(setting!="")
		kFactorIntSpinBox->setValue(atof(setting.c_str()));
	else
		kFactorIntSpinBox->setValue(2.5);
	
	setting = gDb.GetSetting("UseClutter");
	if(setting=="true")
		useClutterCheckBox->setChecked(true);
	else
		useClutterCheckBox->setChecked(false);
	
	setting = gDb.GetSetting("BTLDir");
	if(setting!="")
		btlDirectoryEdit->setText(QString::fromStdString(setting));
	else
		btlDirectoryEdit->setText("/home/");
	
	setting = gDb.GetSetting("OutputDir");
	if(setting!="")
		outputDirectoryEdit->setText(QString::fromStdString(setting));
	else
		outputDirectoryEdit->setText("/home/");
	
	setting = gDb.GetSetting("SiteSelectSense");
	if(setting!="")
		siteSelectSenseSpinBox->setValue(atof(setting.c_str()));
	else
		siteSelectSenseSpinBox->setValue(150.00);
	
	setting = gDb.GetSetting("PlotResolution");
	if(setting!="")
		plotResolutionSpinBox->setValue(atof(setting.c_str()));
	else
		plotResolutionSpinBox->setValue(90.00);
	
	setting = gDb.GetSetting("DownLink");
	if(setting=="true")
		downLinkRadio->setChecked(true);
	else
		upLinkRadio->setChecked(true);
}


//*****************************************************************************
// Load the relevant widget
void PreferencesDialog::SelectionChanged (QTreeWidgetItem* item, int col)
{
	QString itemName = item->text(0);
	
	if(itemName == "System units")
	{
		preferencesStack->setCurrentIndex(0);
		return;
	}
	
	if(itemName == "Defaults")
	{
		preferencesStack->setCurrentIndex(1);
	}
	
	if(itemName == "Plots")
	{
		preferencesStack->setCurrentIndex(2);
	}
	if(itemName == "Plots")
	{
		preferencesStack->setCurrentIndex(2);
	}
	if (itemName =="Default colours for plots")
	{
		preferencesStack->setCurrentIndex(3);
	}
}

//***********************************************************************************
// Setup the preferences dialog
void PreferencesDialog::SetupUi ()
{
	buttonBox = new QDialogButtonBox(this);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setGeometry(QRect(520, 520, 171, 32));
	buttonBox->setOrientation(Qt::Horizontal);
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
	gridLayout = new QWidget(this);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	gridLayout->setGeometry(QRect(0, 10, 691, 491));
	gridLayout1 = new QGridLayout(gridLayout);
	gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
	gridLayout1->setContentsMargins(0, 0, 0, 0);

	// Create the preferences tree
	preferencesTree = new QTreeWidget(gridLayout);
	preferencesTree->setObjectName(QString::fromUtf8("preferencesTree"));
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(preferencesTree->sizePolicy().hasHeightForWidth());
	preferencesTree->setSizePolicy(sizePolicy);
	preferencesTree->setAutoFillBackground(false);
	preferencesTree->setFrameShape(QFrame::StyledPanel);
	preferencesTree->setFrameShadow(QFrame::Plain);
	preferencesTree->setColumnCount(1);
	
	QTreeWidgetItem* interface = new QTreeWidgetItem(0);
	QTreeWidgetItem* rap = new QTreeWidgetItem(0);
	interface->setText(0,"Interface");
	interface->setIcon(0,QIcon(":images/interface.png"));
	rap->setText(0,"RAP");
	rap->setIcon(0,QIcon(":images/logo_icon.png"));
	
//	QTreeWidgetItem* dropdowns = new QTreeWidgetItem(interface);
	QTreeWidgetItem* units = new QTreeWidgetItem(interface);
	QTreeWidgetItem* colours = new QTreeWidgetItem(interface);
//	dropdowns->setText(0,"Fixed drop down menus");
//	dropdowns->setIcon(0,QIcon(":images/dropdowns.png"));
	colours->setText(0,"Default colours for plots");
	colours->setIcon(0,QIcon(":images/Colour.png"));
	units->setText(0,"System units");
	units->setIcon(0,QIcon(":images/units.png"));
	
	QTreeWidgetItem* defaultSettings = new QTreeWidgetItem(rap);
	QTreeWidgetItem* plot = new QTreeWidgetItem(rap);
	defaultSettings->setText(0,"Defaults");
	defaultSettings->setIcon(0,QIcon(":images/defaults.png"));
	plot->setText(0,"Plots");
	plot->setIcon(0,QIcon(":images/plot.png"));
	
	preferencesTree->addTopLevelItem(interface);
	preferencesTree->addTopLevelItem(rap);
	preferencesTree->header()->setVisible(false);
	preferencesTree->expandAll();
	
	connect(preferencesTree,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(SelectionChanged(QTreeWidgetItem*,int)));

	gridLayout1->addWidget(preferencesTree, 0, 0, 1, 1);

	preferencesStack = new QStackedWidget(gridLayout);
	preferencesStack->setObjectName(QString::fromUtf8("preferencesStack"));
	QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
	sizePolicy1.setHorizontalStretch(0);
	sizePolicy1.setVerticalStretch(0);
	sizePolicy1.setHeightForWidth(preferencesStack->sizePolicy().hasHeightForWidth());
	preferencesStack->setSizePolicy(sizePolicy1);
	unitsPreferences = new QWidget();
	unitsPreferences->setObjectName(QString::fromUtf8("unitsPreferences"));
	sensitivityGroup = new QGroupBox(unitsPreferences);
	sensitivityGroup->setObjectName(QString::fromUtf8("sensitivityGroup"));
	sensitivityGroup->setGeometry(QRect(0, 30, 210, 100));
	sensitivityGroup->setAutoFillBackground(false);
	sensitivityGroup->setFlat(false);
	sensitivityGroup->setCheckable(false);
	receiverPower = new QRadioButton(sensitivityGroup);
	receiverPower->setObjectName(QString::fromUtf8("receiverPower"));
	receiverPower->setGeometry(QRect(10, 20, 190, 30));
	receiverVoltage = new QRadioButton(sensitivityGroup);
	receiverVoltage->setObjectName(QString::fromUtf8("receiverVoltage"));
	receiverVoltage->setGeometry(QRect(10, 60, 190, 30));
	receiverdBW = new QRadioButton(sensitivityGroup);
	receiverdBW->setObjectName(QString::fromUtf8("receiverdBW"));
	receiverdBW->setGeometry(QRect(10, 40, 190, 30));
	powerGroup = new QGroupBox(unitsPreferences);
	powerGroup->setObjectName(QString::fromUtf8("powerGroup"));
	powerGroup->setGeometry(QRect(220, 30, 210, 100));
	powerWattRadio = new QRadioButton(powerGroup);
	powerWattRadio->setObjectName(QString::fromUtf8("powerWattRadio"));
	powerWattRadio->setGeometry(QRect(10, 20, 190, 30));
	powerdBmRadio = new QRadioButton(powerGroup);
	powerdBmRadio->setObjectName(QString::fromUtf8("powerdBmRadio"));
	powerdBmRadio->setGeometry(QRect(10, 40, 60, 30));
	powerdBWRadio = new QRadioButton(powerGroup);
	powerdBWRadio->setObjectName(QString::fromUtf8("powerdBWRadio"));
	powerdBWRadio->setGeometry(QRect(10, 60, 60, 30));
	eirpGroup = new QGroupBox(unitsPreferences);
	eirpGroup->setObjectName(QString::fromUtf8("eirpGroup"));
	eirpGroup->setGeometry(QRect(0, 130, 210, 100));
	eirpWattRadio = new QRadioButton(eirpGroup);
	eirpWattRadio->setObjectName(QString::fromUtf8("eirpWattRadio"));
	eirpWattRadio->setGeometry(QRect(10, 20, 110, 30));
	eirpdBmRadio = new QRadioButton(eirpGroup);
	eirpdBmRadio->setObjectName(QString::fromUtf8("eirpdBmRadio"));
	eirpdBmRadio->setGeometry(QRect(10, 40, 110, 30));
	eirpdBWRadio = new QRadioButton(eirpGroup);
	eirpdBWRadio->setObjectName(QString::fromUtf8("eirpdBWRadio"));
	eirpdBWRadio->setGeometry(QRect(10, 60, 110, 30));
	unitsLabel = new QLabel(unitsPreferences);
	unitsLabel->setObjectName(QString::fromUtf8("unitsLabel"));
	unitsLabel->setGeometry(QRect(0, 0, 70, 30));
	QFont font;
	font.setPointSize(12);
	font.setBold(true);
	font.setUnderline(false);
	font.setWeight(75);
	font.setStrikeOut(false);
	font.setKerning(true);
	unitsLabel->setFont(font);
	unitsLabel->setTextFormat(Qt::RichText);
	unitsLabel->setPixmap(QPixmap());
	unitsHeaderLine = new QFrame(unitsPreferences);
	unitsHeaderLine->setObjectName(QString::fromUtf8("unitsHeaderLine"));
	unitsHeaderLine->setGeometry(QRect(0, 10, 411, 30));
	unitsHeaderLine->setFrameShadow(QFrame::Plain);
	unitsHeaderLine->setFrameShape(QFrame::HLine);
	positionFormatGroup = new QGroupBox(unitsPreferences);
	positionFormatGroup->setObjectName(QString::fromUtf8("positionFormatGroup"));
	positionFormatGroup->setGeometry(QRect(220, 130, 210, 100));
	DDMMSSRadio = new QRadioButton(positionFormatGroup);
	DDMMSSRadio->setObjectName(QString::fromUtf8("DDMMSSRadio"));
	DDMMSSRadio->setGeometry(QRect(10, 20, 120, 30));
	DDMMmmRadio = new QRadioButton(positionFormatGroup);
	DDMMmmRadio->setObjectName(QString::fromUtf8("DDMMmmRadio"));
	DDMMmmRadio->setGeometry(QRect(10, 40, 120, 30));
	DDddddRadio = new QRadioButton(positionFormatGroup);
	DDddddRadio->setObjectName(QString::fromUtf8("DDddddRadio"));
	DDddddRadio->setGeometry(QRect(10, 60, 120, 30));
	impedenceGroup = new QGroupBox(unitsPreferences);
	impedenceGroup->setObjectName(QString::fromUtf8("impedenceGroup"));
	impedenceGroup->setGeometry(QRect(0, 230, 430, 71));
	fiftyOhm = new QRadioButton(impedenceGroup);
	fiftyOhm->setObjectName(QString::fromUtf8("fiftyOhm"));
	fiftyOhm->setGeometry(QRect(10, 20, 420, 30));
	seventyFiveOhm = new QRadioButton(impedenceGroup);
	seventyFiveOhm->setObjectName(QString::fromUtf8("seventyFiveOhm"));
	seventyFiveOhm->setGeometry(QRect(10, 40, 420, 30));
	predictionEIRP = new QGroupBox(unitsPreferences);
	predictionEIRP->setObjectName(QString::fromUtf8("predictionEIRP"));
	predictionEIRP->setGeometry(QRect(0, 310, 430, 81));
	predictionEIRP->setEnabled(false);
	explicitEIRP = new QRadioButton(predictionEIRP);
	explicitEIRP->setObjectName(QString::fromUtf8("explicitEIRP"));
	explicitEIRP->setGeometry(QRect(10, 20, 420, 30));
	calculatedEIRP = new QRadioButton(predictionEIRP);
	calculatedEIRP->setObjectName(QString::fromUtf8("calculatedEIRP"));
	calculatedEIRP->setGeometry(QRect(10, 42, 420, 31));
	calculatedEIRP->setEnabled(false);
	preferencesStack->addWidget(unitsPreferences);
	rapPreferences = new QWidget();
	rapPreferences->setObjectName(QString::fromUtf8("rapPreferences"));
	defaultsLabel = new QLabel(rapPreferences);
	defaultsLabel->setObjectName(QString::fromUtf8("defaultsLabel"));
	defaultsLabel->setGeometry(QRect(0, 0, 74, 30));
	QFont font1;
	font1.setPointSize(11);
	font1.setBold(true);
	font1.setWeight(75);
	defaultsLabel->setFont(font1);
	defaultHeaderLine = new QFrame(rapPreferences);
	defaultHeaderLine->setObjectName(QString::fromUtf8("defaultHeaderLine"));
	defaultHeaderLine->setGeometry(QRect(0, 10, 421, 30));
	defaultHeaderLine->setFrameShadow(QFrame::Plain);
	defaultHeaderLine->setFrameShape(QFrame::HLine);
	displayModeGroup = new QGroupBox(rapPreferences);
	displayModeGroup->setObjectName(QString::fromUtf8("displayModeGroup"));
	displayModeGroup->setGeometry(QRect(0, 30, 421, 71));
	editNormalRadio = new QRadioButton(displayModeGroup);
	editNormalRadio->setObjectName(QString::fromUtf8("editNormalRadio"));
	editNormalRadio->setGeometry(QRect(10, 20, 141, 30));
	editDefaultsRadio = new QRadioButton(displayModeGroup);
	editDefaultsRadio->setObjectName(QString::fromUtf8("editDefaultsRadio"));
	editDefaultsRadio->setGeometry(QRect(10, 40, 111, 30));
	preferencesStack->addWidget(rapPreferences);
	plotPreferences = new QWidget();
	plotPreferences->setObjectName(QString::fromUtf8("plotPreferences"));
	plotPreferencesLabel = new QLabel(plotPreferences);
	plotPreferencesLabel->setObjectName(QString::fromUtf8("plotPreferencesLabel"));
	plotPreferencesLabel->setGeometry(QRect(0, 0, 74, 30));
	plotPreferencesLabel->setFont(font1);
	plotHeaderLine = new QFrame(plotPreferences);
	plotHeaderLine->setObjectName(QString::fromUtf8("plotHeaderLine"));
	plotHeaderLine->setGeometry(QRect(0, 10, 421, 30));
	plotHeaderLine->setFrameShadow(QFrame::Plain);
	plotHeaderLine->setFrameShape(QFrame::HLine);
	displayUnitsGroup = new QGroupBox(plotPreferences);
	displayUnitsGroup->setObjectName(QString::fromUtf8("displayUnitsGroup"));
	displayUnitsGroup->setGeometry(QRect(0, 30, 211, 61));
	displayUnitsCombo = new QComboBox(displayUnitsGroup);
	displayUnitsCombo->setObjectName(QString::fromUtf8("displayUnitsCombo"));
	displayUnitsCombo->setGeometry(QRect(100, 20, 101, 30));
	
	// Populate the display units
	displayUnitsCombo->addItem("dB");
	displayUnitsCombo->addItem("dBW");
	displayUnitsCombo->addItem("dBm");
	displayUnitsCombo->addItem("dBuV");
	displayUnitsCombo->addItem("dBuVm");
	displayUnitsCombo->addItem("dBWm2Hz");
	displayUnitsCombo->addItem("dBWm2");
	
	displayUnitsLabel = new QLabel(displayUnitsGroup);
	displayUnitsLabel->setObjectName(QString::fromUtf8("displayUnitsLabel"));
	displayUnitsLabel->setGeometry(QRect(10, 25, 91, 20));
	plotDefaultsGroup = new QGroupBox(plotPreferences);
	plotDefaultsGroup->setObjectName(QString::fromUtf8("plotDefaultsGroup"));
	plotDefaultsGroup->setGeometry(QRect(0, 100, 421, 251));
	gridLayout_2 = new QWidget(plotDefaultsGroup);
	gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
	gridLayout_2->setGeometry(QRect(10, 20, 400, 227));
	gridLayout2 = new QGridLayout(gridLayout_2);
	gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
	gridLayout2->setContentsMargins(0, 0, 0, 0);
	RqSNLabel = new QLabel(gridLayout_2);
	RqSNLabel->setObjectName(QString::fromUtf8("RqSNLabel"));
	QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy2.setHorizontalStretch(0);
	sizePolicy2.setVerticalStretch(0);
	sizePolicy2.setHeightForWidth(RqSNLabel->sizePolicy().hasHeightForWidth());
	RqSNLabel->setSizePolicy(sizePolicy2);
	RqSNLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(RqSNLabel, 0, 0, 1, 1);

	RqSNSpinBox = new QDoubleSpinBox(gridLayout_2);
	RqSNSpinBox->setObjectName(QString::fromUtf8("RqSNSpinBox"));
	sizePolicy2.setHeightForWidth(RqSNSpinBox->sizePolicy().hasHeightForWidth());
	RqSNSpinBox->setSizePolicy(sizePolicy2);
	RqSNSpinBox->setMinimum(-100);
	RqSNSpinBox->setMaximum(100);

	gridLayout2->addWidget(RqSNSpinBox, 0, 1, 1, 1);

	fadeMarginLabel = new QLabel(gridLayout_2);
	fadeMarginLabel->setObjectName(QString::fromUtf8("fadeMarginLabel"));
	fadeMarginLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(fadeMarginLabel, 1, 0, 1, 1);

	fadeMarginSpin = new QDoubleSpinBox(gridLayout_2);
	fadeMarginSpin->setObjectName(QString::fromUtf8("fadeMarginSpin"));
	fadeMarginSpin->setMinimum(-100);
	fadeMarginSpin->setMaximum(100);

	gridLayout2->addWidget(fadeMarginSpin, 1, 1, 1, 1);

	rxMinLabel = new QLabel(gridLayout_2);
	rxMinLabel->setObjectName(QString::fromUtf8("rxMinLabel"));
	rxMinLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(rxMinLabel, 2, 0, 1, 1);

	rxMinSpinBox = new QDoubleSpinBox(gridLayout_2);
	rxMinSpinBox->setObjectName(QString::fromUtf8("rxMinSpinBox"));
	rxMinSpinBox->setMinimum(-500);
	rxMinSpinBox->setMaximum(500);

	gridLayout2->addWidget(rxMinSpinBox, 2, 1, 1, 1);

	RqClCoLabel = new QLabel(gridLayout_2);
	RqClCoLabel->setObjectName(QString::fromUtf8("RqClCoLabel"));
	RqClCoLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(RqClCoLabel, 3, 0, 1, 1);

	rqCociSpinBox = new QDoubleSpinBox(gridLayout_2);
	rqCociSpinBox->setObjectName(QString::fromUtf8("rqCociSpinBox"));
	rqCociSpinBox->setMinimum(-100);
	rqCociSpinBox->setMaximum(100);

	gridLayout2->addWidget(rqCociSpinBox, 3, 1, 1, 1);

	RqCIadLabel = new QLabel(gridLayout_2);
	RqCIadLabel->setObjectName(QString::fromUtf8("RqCIadLabel"));
	RqCIadLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(RqCIadLabel, 4, 0, 1, 1);

	rqCiadSpinBox = new QDoubleSpinBox(gridLayout_2);
	rqCiadSpinBox->setObjectName(QString::fromUtf8("rqCiadSpinBox"));
	rqCiadSpinBox->setMinimum(-100);
	rqCiadSpinBox->setMaximum(100);

	gridLayout2->addWidget(rqCiadSpinBox, 4, 1, 1, 1);

	rqEbNoLabel = new QLabel(gridLayout_2);
	rqEbNoLabel->setObjectName(QString::fromUtf8("rqEbNoLabel"));
	rqEbNoLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(rqEbNoLabel, 5, 0, 1, 1);

	rqEbNoSpinBox = new QDoubleSpinBox(gridLayout_2);
	rqEbNoSpinBox->setObjectName(QString::fromUtf8("rqEbNoSpinBox"));
	rqEbNoSpinBox->setMinimum(-100);
	rqEbNoSpinBox->setMaximum(100);

	gridLayout2->addWidget(rqEbNoSpinBox, 5, 1, 1, 1);

	kFactorServerLabel = new QLabel(gridLayout_2);
	kFactorServerLabel->setObjectName(QString::fromUtf8("kFactorServerLabel"));
	kFactorServerLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(kFactorServerLabel, 0, 2, 1, 1);

	kFactorServerSpinBox = new QDoubleSpinBox(gridLayout_2);
	kFactorServerSpinBox->setObjectName(QString::fromUtf8("kFactorServerSpinBox"));
	sizePolicy2.setHeightForWidth(kFactorServerSpinBox->sizePolicy().hasHeightForWidth());
	kFactorServerSpinBox->setSizePolicy(sizePolicy2);
	kFactorServerSpinBox->setMinimum(-20);
	kFactorServerSpinBox->setMaximum(20);

	gridLayout2->addWidget(kFactorServerSpinBox, 0, 3, 1, 1);

	kFactorIntLabel = new QLabel(gridLayout_2);
	kFactorIntLabel->setObjectName(QString::fromUtf8("kFactorIntLabel"));
	kFactorIntLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(kFactorIntLabel, 1, 2, 1, 1);

	kFactorIntSpinBox = new QDoubleSpinBox(gridLayout_2);
	kFactorIntSpinBox->setObjectName(QString::fromUtf8("kFactorIntSpinBox"));
	kFactorIntSpinBox->setMinimum(-20);
	kFactorIntSpinBox->setMaximum(20);

	gridLayout2->addWidget(kFactorIntSpinBox, 1, 3, 1, 1);

	useClutterLabel = new QLabel(gridLayout_2);
	useClutterLabel->setObjectName(QString::fromUtf8("useClutterLabel"));
	useClutterLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(useClutterLabel, 2, 2, 1, 1);

	useClutterCheckBox = new QCheckBox(gridLayout_2);
	useClutterCheckBox->setObjectName(QString::fromUtf8("useClutterCheckBox"));
	sizePolicy2.setHeightForWidth(useClutterCheckBox->sizePolicy().hasHeightForWidth());
	useClutterCheckBox->setSizePolicy(sizePolicy2);
//	useClutterCheckBox->setEnabled(false);
//	useClutterLabel->setEnabled(false);

	gridLayout2->addWidget(useClutterCheckBox, 2, 3, 1, 1);

	siteSelectSenseLabel = new QLabel(gridLayout_2);
	siteSelectSenseLabel->setObjectName(QString::fromUtf8("siteSelectSenseLabel"));
	siteSelectSenseLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(siteSelectSenseLabel, 3, 2, 1, 1);

	siteSelectSenseSpinBox = new QDoubleSpinBox(gridLayout_2);
	siteSelectSenseSpinBox->setObjectName(QString::fromUtf8("siteSelectSenseSpinBox"));
	siteSelectSenseSpinBox->setMaximum(1e+06);

	gridLayout2->addWidget(siteSelectSenseSpinBox, 3, 3, 1, 1);

	downLinkLabel = new QLabel(gridLayout_2);
	downLinkLabel->setObjectName(QString::fromUtf8("downLinkLabel"));
	downLinkLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(downLinkLabel, 4, 2, 1, 1);

	upLinkLabel = new QLabel(gridLayout_2);
	upLinkLabel->setObjectName(QString::fromUtf8("upLinkLabel"));
	upLinkLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(upLinkLabel, 5, 2, 1, 1);

	downLinkRadio = new QRadioButton(gridLayout_2);
	downLinkRadio->setObjectName(QString::fromUtf8("downLinkRadio"));
	sizePolicy2.setHeightForWidth(downLinkRadio->sizePolicy().hasHeightForWidth());
	downLinkRadio->setSizePolicy(sizePolicy2);

	gridLayout2->addWidget(downLinkRadio, 4, 3, 1, 1);

	upLinkRadio = new QRadioButton(gridLayout_2);
	upLinkRadio->setObjectName(QString::fromUtf8("upLinkRadio"));
	sizePolicy2.setHeightForWidth(upLinkRadio->sizePolicy().hasHeightForWidth());
	upLinkRadio->setSizePolicy(sizePolicy2);

	gridLayout2->addWidget(upLinkRadio, 5, 3, 1, 1);

	plotResolutionLabel = new QLabel(gridLayout_2);
	plotResolutionLabel->setObjectName(QString::fromUtf8("plotResolutionLabel"));
	plotResolutionLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout2->addWidget(plotResolutionLabel, 6, 0, 1, 1);

	plotResolutionSpinBox = new QDoubleSpinBox(gridLayout_2);
	plotResolutionSpinBox->setObjectName(QString::fromUtf8("plotResolutionSpinBox"));
	plotResolutionSpinBox->setMaximum(1e+06);
	plotResolutionSpinBox->setMinimum(0);
	gridLayout2->addWidget(plotResolutionSpinBox, 6, 1, 1, 1);

	directoriesGroup = new QGroupBox(plotPreferences);
	directoriesGroup->setObjectName(QString::fromUtf8("directoriesGroup"));
	directoriesGroup->setGeometry(QRect(0, 360, 421, 91));
	gridLayout_3 = new QWidget(directoriesGroup);
	gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
	gridLayout_3->setGeometry(QRect(9, 20, 401, 62));
	gridLayout3 = new QGridLayout(gridLayout_3);
	gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
	gridLayout3->setContentsMargins(0, 0, 0, 0);
	btlDirectoryLabel = new QLabel(gridLayout_3);
	btlDirectoryLabel->setObjectName(QString::fromUtf8("btlDirectoryLabel"));
	btlDirectoryLabel->setLayoutDirection(Qt::RightToLeft);

	gridLayout3->addWidget(btlDirectoryLabel, 0, 0, 1, 1);

	btlDirectoryEdit = new QLineEdit(gridLayout_3);
	btlDirectoryEdit->setObjectName(QString::fromUtf8("btlDirectoryEdit"));

	gridLayout3->addWidget(btlDirectoryEdit, 0, 1, 1, 1);

	outputDirectoryLabel = new QLabel(gridLayout_3);
	outputDirectoryLabel->setObjectName(QString::fromUtf8("outputDirectoryLabel"));

	gridLayout3->addWidget(outputDirectoryLabel, 1, 0, 1, 1);

	outputDirectoryEdit = new QLineEdit(gridLayout_3);
	outputDirectoryEdit->setObjectName(QString::fromUtf8("outputDirectoryEdit"));

	gridLayout3->addWidget(outputDirectoryEdit, 1, 1, 1, 1);

	technologyGroup = new QGroupBox(plotPreferences);
	technologyGroup->setObjectName(QString::fromUtf8("technologyGroup"));
	technologyGroup->setGeometry(QRect(220, 30, 201, 61));
	technologyLabel = new QLabel(technologyGroup);
	technologyLabel->setObjectName(QString::fromUtf8("technologyLabel"));
	technologyLabel->setGeometry(QRect(10, 25, 71, 18));
	technologyCombo = new QComboBox(technologyGroup);
	technologyCombo->setObjectName(QString::fromUtf8("technologyCombo"));
	technologyCombo->setGeometry(QRect(90, 20, 101, 26));
	
	// Populate the technology combo box
	StringIntArray vals;
	StringIntArrayIterator iterator;
	
	// Get the techology types that are available
	gDb.GetFieldUiParams("radioinstallation","techkey",vals);
	
	technologyCombo->insertItem(0,"0:NULL");
	int j=1;
	
	// Poulate the combobox with the default data
	for( iterator=vals.begin() ; iterator!=vals.end() ; iterator++ )
	{
		QString temp = QString::number(iterator->first) + ":" + QString::fromStdString(iterator->second);
		technologyCombo->insertItem(j,temp,qVariantFromValue(iterator->first));
		j++;
	} // for
	
	preferencesStack->addWidget(plotPreferences);
	
	cColourManager *colour = new cColourManager(this);
	preferencesStack->addWidget(colour);

	gridLayout1->addWidget(preferencesStack, 0, 1, 1, 1);

	bottomLine = new QFrame(this);
	bottomLine->setObjectName(QString::fromUtf8("bottomLine"));
	bottomLine->setGeometry(QRect(0, 500, 691, 16));
	bottomLine->setFrameShadow(QFrame::Plain);
	bottomLine->setFrameShape(QFrame::HLine);
	
	RetranslateUi();
	ReadSettings();
	
	QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(WriteSettings()));
	QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
	preferencesStack->setCurrentIndex(0);
	
	QMetaObject::connectSlotsByName(this);
} // SetupUi


void PreferencesDialog::RetranslateUi ()
{
	setWindowTitle(QApplication::translate("PreferencesDialog", "QRAP Preferences", 0, QApplication::UnicodeUTF8));
	preferencesTree->headerItem()->setText(0, QApplication::translate("PreferencesDialog", "1", 0, QApplication::UnicodeUTF8));
	preferencesTree->setToolTip(QApplication::translate("PreferencesDialog", "Select preferences page", 0, QApplication::UnicodeUTF8));
	unitsPreferences->setToolTip(QString());
	sensitivityGroup->setToolTip(QApplication::translate("PreferencesDialog", "Set the receiver sensitivity units", 0, QApplication::UnicodeUTF8));
	sensitivityGroup->setTitle(QApplication::translate("PreferencesDialog", "Sensitivity", 0, QApplication::UnicodeUTF8));
	receiverPower->setText(QApplication::translate("PreferencesDialog", "Receiver Power in dBm", 0, QApplication::UnicodeUTF8));
	receiverVoltage->setText(QApplication::translate("PreferencesDialog", "Receiver Voltage in dB\316\274V", 0, QApplication::UnicodeUTF8));
	receiverdBW->setText(QApplication::translate("PreferencesDialog", "Receiver Power in dBW", 0, QApplication::UnicodeUTF8));
	powerGroup->setToolTip(QApplication::translate("PreferencesDialog", "Set the power units", 0, QApplication::UnicodeUTF8));
	powerGroup->setTitle(QApplication::translate("PreferencesDialog", "Power", 0, QApplication::UnicodeUTF8));
	powerWattRadio->setText(QApplication::translate("PreferencesDialog", "W", 0, QApplication::UnicodeUTF8));
	powerdBmRadio->setText(QApplication::translate("PreferencesDialog", "dBm", 0, QApplication::UnicodeUTF8));
	powerdBWRadio->setText(QApplication::translate("PreferencesDialog", "dBW", 0, QApplication::UnicodeUTF8));
	eirpGroup->setToolTip(QApplication::translate("PreferencesDialog", "Set the EIRP units", 0, QApplication::UnicodeUTF8));
	eirpGroup->setTitle(QApplication::translate("PreferencesDialog", "EIRP", 0, QApplication::UnicodeUTF8));
	eirpWattRadio->setText(QApplication::translate("PreferencesDialog", "W", 0, QApplication::UnicodeUTF8));
	eirpdBmRadio->setText(QApplication::translate("PreferencesDialog", "dBm", 0, QApplication::UnicodeUTF8));
	eirpdBWRadio->setText(QApplication::translate("PreferencesDialog", "dBW", 0, QApplication::UnicodeUTF8));
	unitsLabel->setText(QApplication::translate("PreferencesDialog", "Units", 0, QApplication::UnicodeUTF8));
	positionFormatGroup->setToolTip(QApplication::translate("PreferencesDialog", "Set the location format", 0, QApplication::UnicodeUTF8));
	positionFormatGroup->setTitle(QApplication::translate("PreferencesDialog", "Location Format", 0, QApplication::UnicodeUTF8));
	DDMMSSRadio->setToolTip(QApplication::translate("PreferencesDialog", "Enter the location in Degrees(DD):Minutes(MM):Seconds(SS) and direction X (N,S,E,W)", 0, QApplication::UnicodeUTF8));
	DDMMSSRadio->setText(QApplication::translate("PreferencesDialog", "DD:MM:SS X", 0, QApplication::UnicodeUTF8));
	DDMMmmRadio->setToolTip(QApplication::translate("PreferencesDialog", "Enter the location in Degrees(DD):Minutes(MM).Decimal minutes(mm) direction X (N,S,E,W)", 0, QApplication::UnicodeUTF8));
	DDMMmmRadio->setText(QApplication::translate("PreferencesDialog", "DD:MM.mm X", 0, QApplication::UnicodeUTF8));
	DDddddRadio->setToolTip(QApplication::translate("PreferencesDialog", "Enter the location in Degrees(DD).Decimals(dddd) and direction to be + or -", 0, QApplication::UnicodeUTF8));
	DDddddRadio->setText(QApplication::translate("PreferencesDialog", "\302\261DD.dddd", 0, QApplication::UnicodeUTF8));
	impedenceGroup->setTitle(QApplication::translate("PreferencesDialog", "Receiver Input Impedence", 0, QApplication::UnicodeUTF8));
	fiftyOhm->setToolTip(QApplication::translate("PreferencesDialog", "Used in most communication systems", 0, QApplication::UnicodeUTF8));
	fiftyOhm->setText(QApplication::translate("PreferencesDialog", "50\316\251 as used in most communication systems", 0, QApplication::UnicodeUTF8));
	seventyFiveOhm->setToolTip(QApplication::translate("PreferencesDialog", "Used in broadcasting systems", 0, QApplication::UnicodeUTF8));
	seventyFiveOhm->setText(QApplication::translate("PreferencesDialog", "75\316\251 as used in broadcasting", 0, QApplication::UnicodeUTF8));
	predictionEIRP->setTitle(QApplication::translate("PreferencesDialog", "EIRP value that will be used in predictions", 0, QApplication::UnicodeUTF8));
	explicitEIRP->setToolTip(QApplication::translate("PreferencesDialog", "Enter the value of the EIRP explicitly", 0, QApplication::UnicodeUTF8));
	explicitEIRP->setText(QApplication::translate("PreferencesDialog", "EIRP entered explicitly", 0, QApplication::UnicodeUTF8));
	calculatedEIRP->setToolTip(QApplication::translate("PreferencesDialog", "Calculate EIRP from transmitter power losses and antenna gain", 0, QApplication::UnicodeUTF8));
	calculatedEIRP->setText(QApplication::translate("PreferencesDialog", "Calculate EIRP", 0, QApplication::UnicodeUTF8));
	defaultsLabel->setText(QApplication::translate("PreferencesDialog", "Defaults", 0, QApplication::UnicodeUTF8));
	displayModeGroup->setTitle(QApplication::translate("PreferencesDialog", "Interface Mode", 0, QApplication::UnicodeUTF8));
	editNormalRadio->setToolTip(QApplication::translate("PreferencesDialog", "Use the interface in normal mode", 0, QApplication::UnicodeUTF8));
	editNormalRadio->setText(QApplication::translate("PreferencesDialog", "Edit Normally", 0, QApplication::UnicodeUTF8));
	editDefaultsRadio->setText(QApplication::translate("PreferencesDialog", "Edit Defaults", 0, QApplication::UnicodeUTF8));
	plotPreferences->setToolTip(QString());
	plotPreferencesLabel->setText(QApplication::translate("PreferencesDialog", "Plots", 0, QApplication::UnicodeUTF8));
	displayUnitsGroup->setTitle(QApplication::translate("PreferencesDialog", "Units", 0, QApplication::UnicodeUTF8));
	displayUnitsCombo->setToolTip(QApplication::translate("PreferencesDialog", "Output units of the prediction plots", 0, QApplication::UnicodeUTF8));
	displayUnitsLabel->setText(QApplication::translate("PreferencesDialog", "Display Units", 0, QApplication::UnicodeUTF8));
	plotDefaultsGroup->setTitle(QApplication::translate("PreferencesDialog", "Plot Defaults", 0, QApplication::UnicodeUTF8));
	RqSNLabel->setText(QApplication::translate("PreferencesDialog", "Required SN", 0, QApplication::UnicodeUTF8));
	RqSNSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Required signal to noise ratio", 0, QApplication::UnicodeUTF8));
	fadeMarginLabel->setText(QApplication::translate("PreferencesDialog", "Fade Margin", 0, QApplication::UnicodeUTF8));
	fadeMarginSpin->setToolTip(QApplication::translate("PreferencesDialog", "The fade margin", 0, QApplication::UnicodeUTF8));
	rxMinLabel->setText(QApplication::translate("PreferencesDialog", "Rx Minimum", 0, QApplication::UnicodeUTF8));
	rxMinSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Required minimum receiver level", 0, QApplication::UnicodeUTF8));
	RqClCoLabel->setText(QApplication::translate("PreferencesDialog", "Req coci", 0, QApplication::UnicodeUTF8));
	rqCociSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Required co-channel carrier to interference", 0, QApplication::UnicodeUTF8));
	RqCIadLabel->setText(QApplication::translate("PreferencesDialog", "Req CIad", 0, QApplication::UnicodeUTF8));
	rqCiadSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Required adjacent carrier to interference", 0, QApplication::UnicodeUTF8));
	rqEbNoLabel->setText(QApplication::translate("PreferencesDialog", "Req Ebno", 0, QApplication::UnicodeUTF8));
	rqEbNoSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Required engergy per bit to noise ratio", 0, QApplication::UnicodeUTF8));
	kFactorServerLabel->setText(QApplication::translate("PreferencesDialog", "kFactor Server", 0, QApplication::UnicodeUTF8));
	kFactorServerSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "kFactor for the server", 0, QApplication::UnicodeUTF8));
	kFactorIntLabel->setText(QApplication::translate("PreferencesDialog", "kFactor Int", 0, QApplication::UnicodeUTF8));
	kFactorIntSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "kFactor for interferers", 0, QApplication::UnicodeUTF8));
	useClutterLabel->setToolTip(QApplication::translate("PreferencesDialog", "Use clutter data in predictions", 0, QApplication::UnicodeUTF8));
	useClutterLabel->setText(QApplication::translate("PreferencesDialog", "Use clutter", 0, QApplication::UnicodeUTF8));
	useClutterCheckBox->setToolTip(QApplication::translate("PreferencesDialog", "Use clutter data in predictions", 0, QApplication::UnicodeUTF8));
	useClutterCheckBox->setText(QString());
	siteSelectSenseLabel->setText(QApplication::translate("PreferencesDialog", "Select Sense", 0, QApplication::UnicodeUTF8));
	siteSelectSenseSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Site selection sensitivity", 0, QApplication::UnicodeUTF8));
	downLinkLabel->setToolTip(QApplication::translate("PreferencesDialog", "Tower communicates with mobile", 0, QApplication::UnicodeUTF8));
	downLinkLabel->setText(QApplication::translate("PreferencesDialog", "Down link", 0, QApplication::UnicodeUTF8));
	upLinkLabel->setToolTip(QApplication::translate("PreferencesDialog", "Mobile communicates with tower", 0, QApplication::UnicodeUTF8));
	upLinkLabel->setText(QApplication::translate("PreferencesDialog", "Up link", 0, QApplication::UnicodeUTF8));
	downLinkRadio->setToolTip(QApplication::translate("PreferencesDialog", "Tower communicates with mobile", 0, QApplication::UnicodeUTF8));
	downLinkRadio->setText(QString());
	upLinkRadio->setToolTip(QApplication::translate("PreferencesDialog", "Mobile communicates with tower", 0, QApplication::UnicodeUTF8));
	upLinkRadio->setText(QString());
	plotResolutionLabel->setText(QApplication::translate("PreferencesDialog", "Plot Res", 0, QApplication::UnicodeUTF8));
	plotResolutionSpinBox->setToolTip(QApplication::translate("PreferencesDialog", "Plot resolution", 0, QApplication::UnicodeUTF8));
	directoriesGroup->setTitle(QApplication::translate("PreferencesDialog", "Plot Directories", 0, QApplication::UnicodeUTF8));
	btlDirectoryLabel->setText(QApplication::translate("PreferencesDialog", "BTL directory", 0, QApplication::UnicodeUTF8));
	btlDirectoryEdit->setToolTip(QApplication::translate("PreferencesDialog", "The directory where the BTL files will be saved", 0, QApplication::UnicodeUTF8));
	outputDirectoryLabel->setText(QApplication::translate("PreferencesDialog", "Ouput directory", 0, QApplication::UnicodeUTF8));
	outputDirectoryEdit->setToolTip(QApplication::translate("PreferencesDialog", "The directory where the prediction plots will be saved", 0, QApplication::UnicodeUTF8));
	technologyGroup->setTitle(QApplication::translate("PreferencesDialog", "Default Technology", 0, QApplication::UnicodeUTF8));
	technologyLabel->setText(QApplication::translate("PreferencesDialog", "Technology", 0, QApplication::UnicodeUTF8));

} // RetranslateUi
