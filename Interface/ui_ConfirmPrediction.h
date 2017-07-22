/********************************************************************************
** Form generated from reading UI file 'ConfirmPrediction.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMPREDICTION_H
#define UI_CONFIRMPREDICTION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmPrediction
{
public:
    QTableWidget *tableWidget;
    QWidget *gridLayout;
    QGridLayout *PlotInput;
    QLabel *RqRxlbl;
    QDoubleSpinBox *rxMinSpinBox;
    QLabel *AngleReslbl;
    QLabel *plotReslbl;
    QLabel *kFactorIlbl;
    QDoubleSpinBox *plotResolutionSpinBox;
    QDoubleSpinBox *kFactorIntSpinBox;
    QDoubleSpinBox *kFactorServerSpinBox;
    QLabel *kFactorSlbl;
    QDoubleSpinBox *minAngleResSpinBox;
    QLabel *RqCCIRlbl;
    QLabel *RqCAIRlbl;
    QDoubleSpinBox *rqCociSpinBox;
    QDoubleSpinBox *rqCiadSpinBox;
    QLabel *RqEbNolbl;
    QDoubleSpinBox *rqEbNoSpinBox;
    QLabel *FadeMlbl;
    QDoubleSpinBox *fadeMarginSpin;
    QLabel *Nolbl;
    QDoubleSpinBox *noiseLevelSpinBox;
    QDoubleSpinBox *RqSNSpinBox;
    QLabel *RqSNlbl;
    QLabel *mobilelbl;
    QComboBox *mobileCombo;
    QLabel *Dirlbl;
    QLabel *outputDirectoryEdit;
    QLabel *filelbl;
    QLineEdit *outputFileNameEdit;
    QComboBox *outputTypeCombo;
    QLabel *FileTypelbl;
    QLabel *plotlbl;
    QComboBox *plotTypeCombo;
    QLabel *unitlbl;
    QComboBox *displayUnitsCombo;
    QCheckBox *useClutterCheckBox;
    QHBoxLayout *UpDownLink;
    QRadioButton *downLinkRadio;
    QRadioButton *upLinkRadio;
    QWidget *horizontalLayout;
    QHBoxLayout *hboxLayout;
    QPushButton *pushButtonPrint;
    QPushButton *btnDone;
    QPushButton *btnCancel;
    QPushButton *btnDo;
    QWidget *horizontalLayout_2;
    QHBoxLayout *hboxLayout1;
    QRadioButton *radioArea;
    QRadioButton *radioRadius;

    void setupUi(QDialog *ConfirmPrediction)
    {
        if (ConfirmPrediction->objectName().isEmpty())
            ConfirmPrediction->setObjectName(QString::fromUtf8("ConfirmPrediction"));
        ConfirmPrediction->resize(1011, 607);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConfirmPrediction->sizePolicy().hasHeightForWidth());
        ConfirmPrediction->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmPrediction->setWindowIcon(icon);
        tableWidget = new QTableWidget(ConfirmPrediction);
        if (tableWidget->columnCount() < 9)
            tableWidget->setColumnCount(9);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 10, 991, 241));
        tableWidget->setSortingEnabled(true);
        gridLayout = new QWidget(ConfirmPrediction);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setGeometry(QRect(10, 260, 1011, 305));
        PlotInput = new QGridLayout(gridLayout);
        PlotInput->setObjectName(QString::fromUtf8("PlotInput"));
        PlotInput->setContentsMargins(0, 0, 0, 0);
        RqRxlbl = new QLabel(gridLayout);
        RqRxlbl->setObjectName(QString::fromUtf8("RqRxlbl"));

        PlotInput->addWidget(RqRxlbl, 5, 0, 1, 1);

        rxMinSpinBox = new QDoubleSpinBox(gridLayout);
        rxMinSpinBox->setObjectName(QString::fromUtf8("rxMinSpinBox"));
        rxMinSpinBox->setMinimum(-10000);
        rxMinSpinBox->setMaximum(10000);

        PlotInput->addWidget(rxMinSpinBox, 5, 1, 1, 1);

        AngleReslbl = new QLabel(gridLayout);
        AngleReslbl->setObjectName(QString::fromUtf8("AngleReslbl"));

        PlotInput->addWidget(AngleReslbl, 8, 2, 1, 1);

        plotReslbl = new QLabel(gridLayout);
        plotReslbl->setObjectName(QString::fromUtf8("plotReslbl"));

        PlotInput->addWidget(plotReslbl, 7, 2, 1, 1);

        kFactorIlbl = new QLabel(gridLayout);
        kFactorIlbl->setObjectName(QString::fromUtf8("kFactorIlbl"));

        PlotInput->addWidget(kFactorIlbl, 6, 2, 1, 1);

        plotResolutionSpinBox = new QDoubleSpinBox(gridLayout);
        plotResolutionSpinBox->setObjectName(QString::fromUtf8("plotResolutionSpinBox"));
        plotResolutionSpinBox->setMaximum(10000);

        PlotInput->addWidget(plotResolutionSpinBox, 7, 3, 1, 1);

        kFactorIntSpinBox = new QDoubleSpinBox(gridLayout);
        kFactorIntSpinBox->setObjectName(QString::fromUtf8("kFactorIntSpinBox"));
        kFactorIntSpinBox->setMinimum(-10000);
        kFactorIntSpinBox->setMaximum(10000);

        PlotInput->addWidget(kFactorIntSpinBox, 6, 3, 1, 1);

        kFactorServerSpinBox = new QDoubleSpinBox(gridLayout);
        kFactorServerSpinBox->setObjectName(QString::fromUtf8("kFactorServerSpinBox"));
        kFactorServerSpinBox->setMinimum(-10000);
        kFactorServerSpinBox->setMaximum(10000);

        PlotInput->addWidget(kFactorServerSpinBox, 5, 3, 1, 1);

        kFactorSlbl = new QLabel(gridLayout);
        kFactorSlbl->setObjectName(QString::fromUtf8("kFactorSlbl"));

        PlotInput->addWidget(kFactorSlbl, 5, 2, 1, 1);

        minAngleResSpinBox = new QDoubleSpinBox(gridLayout);
        minAngleResSpinBox->setObjectName(QString::fromUtf8("minAngleResSpinBox"));

        PlotInput->addWidget(minAngleResSpinBox, 8, 3, 1, 1);

        RqCCIRlbl = new QLabel(gridLayout);
        RqCCIRlbl->setObjectName(QString::fromUtf8("RqCCIRlbl"));

        PlotInput->addWidget(RqCCIRlbl, 6, 0, 1, 1);

        RqCAIRlbl = new QLabel(gridLayout);
        RqCAIRlbl->setObjectName(QString::fromUtf8("RqCAIRlbl"));

        PlotInput->addWidget(RqCAIRlbl, 7, 0, 1, 1);

        rqCociSpinBox = new QDoubleSpinBox(gridLayout);
        rqCociSpinBox->setObjectName(QString::fromUtf8("rqCociSpinBox"));
        rqCociSpinBox->setMinimum(-10000);
        rqCociSpinBox->setMaximum(10000);

        PlotInput->addWidget(rqCociSpinBox, 6, 1, 1, 1);

        rqCiadSpinBox = new QDoubleSpinBox(gridLayout);
        rqCiadSpinBox->setObjectName(QString::fromUtf8("rqCiadSpinBox"));
        rqCiadSpinBox->setMinimum(-10000);
        rqCiadSpinBox->setMaximum(10000);

        PlotInput->addWidget(rqCiadSpinBox, 7, 1, 1, 1);

        RqEbNolbl = new QLabel(gridLayout);
        RqEbNolbl->setObjectName(QString::fromUtf8("RqEbNolbl"));

        PlotInput->addWidget(RqEbNolbl, 8, 0, 1, 1);

        rqEbNoSpinBox = new QDoubleSpinBox(gridLayout);
        rqEbNoSpinBox->setObjectName(QString::fromUtf8("rqEbNoSpinBox"));
        rqEbNoSpinBox->setMinimum(-10000);
        rqEbNoSpinBox->setMaximum(10000);

        PlotInput->addWidget(rqEbNoSpinBox, 8, 1, 1, 1);

        FadeMlbl = new QLabel(gridLayout);
        FadeMlbl->setObjectName(QString::fromUtf8("FadeMlbl"));

        PlotInput->addWidget(FadeMlbl, 2, 2, 1, 1);

        fadeMarginSpin = new QDoubleSpinBox(gridLayout);
        fadeMarginSpin->setObjectName(QString::fromUtf8("fadeMarginSpin"));
        fadeMarginSpin->setMinimum(-10000);
        fadeMarginSpin->setMaximum(10000);

        PlotInput->addWidget(fadeMarginSpin, 2, 3, 1, 1);

        Nolbl = new QLabel(gridLayout);
        Nolbl->setObjectName(QString::fromUtf8("Nolbl"));

        PlotInput->addWidget(Nolbl, 3, 2, 1, 1);

        noiseLevelSpinBox = new QDoubleSpinBox(gridLayout);
        noiseLevelSpinBox->setObjectName(QString::fromUtf8("noiseLevelSpinBox"));
        noiseLevelSpinBox->setMinimum(-10000);
        noiseLevelSpinBox->setMaximum(10000);

        PlotInput->addWidget(noiseLevelSpinBox, 3, 3, 1, 1);

        RqSNSpinBox = new QDoubleSpinBox(gridLayout);
        RqSNSpinBox->setObjectName(QString::fromUtf8("RqSNSpinBox"));
        RqSNSpinBox->setMinimum(-10000);
        RqSNSpinBox->setMaximum(10000);

        PlotInput->addWidget(RqSNSpinBox, 4, 3, 1, 1);

        RqSNlbl = new QLabel(gridLayout);
        RqSNlbl->setObjectName(QString::fromUtf8("RqSNlbl"));

        PlotInput->addWidget(RqSNlbl, 4, 2, 1, 1);

        mobilelbl = new QLabel(gridLayout);
        mobilelbl->setObjectName(QString::fromUtf8("mobilelbl"));

        PlotInput->addWidget(mobilelbl, 4, 0, 1, 1);

        mobileCombo = new QComboBox(gridLayout);
        mobileCombo->setObjectName(QString::fromUtf8("mobileCombo"));

        PlotInput->addWidget(mobileCombo, 4, 1, 1, 1);

        Dirlbl = new QLabel(gridLayout);
        Dirlbl->setObjectName(QString::fromUtf8("Dirlbl"));

        PlotInput->addWidget(Dirlbl, 3, 0, 1, 1);

        outputDirectoryEdit = new QLabel(gridLayout);
        outputDirectoryEdit->setObjectName(QString::fromUtf8("outputDirectoryEdit"));

        PlotInput->addWidget(outputDirectoryEdit, 3, 1, 1, 1);

        filelbl = new QLabel(gridLayout);
        filelbl->setObjectName(QString::fromUtf8("filelbl"));

        PlotInput->addWidget(filelbl, 2, 0, 1, 1);

        outputFileNameEdit = new QLineEdit(gridLayout);
        outputFileNameEdit->setObjectName(QString::fromUtf8("outputFileNameEdit"));

        PlotInput->addWidget(outputFileNameEdit, 2, 1, 1, 1);

        outputTypeCombo = new QComboBox(gridLayout);
        outputTypeCombo->setObjectName(QString::fromUtf8("outputTypeCombo"));
        outputTypeCombo->setEnabled(false);

        PlotInput->addWidget(outputTypeCombo, 1, 1, 1, 1);

        FileTypelbl = new QLabel(gridLayout);
        FileTypelbl->setObjectName(QString::fromUtf8("FileTypelbl"));

        PlotInput->addWidget(FileTypelbl, 1, 0, 1, 1);

        plotlbl = new QLabel(gridLayout);
        plotlbl->setObjectName(QString::fromUtf8("plotlbl"));

        PlotInput->addWidget(plotlbl, 0, 0, 1, 1);

        plotTypeCombo = new QComboBox(gridLayout);
        plotTypeCombo->setObjectName(QString::fromUtf8("plotTypeCombo"));

        PlotInput->addWidget(plotTypeCombo, 0, 1, 1, 1);

        unitlbl = new QLabel(gridLayout);
        unitlbl->setObjectName(QString::fromUtf8("unitlbl"));

        PlotInput->addWidget(unitlbl, 1, 2, 1, 1);

        displayUnitsCombo = new QComboBox(gridLayout);
        displayUnitsCombo->setObjectName(QString::fromUtf8("displayUnitsCombo"));

        PlotInput->addWidget(displayUnitsCombo, 1, 3, 1, 1);

        useClutterCheckBox = new QCheckBox(gridLayout);
        useClutterCheckBox->setObjectName(QString::fromUtf8("useClutterCheckBox"));
        useClutterCheckBox->setEnabled(true);

        PlotInput->addWidget(useClutterCheckBox, 0, 3, 1, 1);

        UpDownLink = new QHBoxLayout();
        UpDownLink->setObjectName(QString::fromUtf8("UpDownLink"));
        downLinkRadio = new QRadioButton(gridLayout);
        downLinkRadio->setObjectName(QString::fromUtf8("downLinkRadio"));
        downLinkRadio->setChecked(true);

        UpDownLink->addWidget(downLinkRadio);

        upLinkRadio = new QRadioButton(gridLayout);
        upLinkRadio->setObjectName(QString::fromUtf8("upLinkRadio"));

        UpDownLink->addWidget(upLinkRadio);


        PlotInput->addLayout(UpDownLink, 0, 2, 1, 1);

        horizontalLayout = new QWidget(ConfirmPrediction);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setGeometry(QRect(630, 570, 373, 31));
        hboxLayout = new QHBoxLayout(horizontalLayout);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        pushButtonPrint = new QPushButton(horizontalLayout);
        pushButtonPrint->setObjectName(QString::fromUtf8("pushButtonPrint"));

        hboxLayout->addWidget(pushButtonPrint);

        btnDone = new QPushButton(horizontalLayout);
        btnDone->setObjectName(QString::fromUtf8("btnDone"));

        hboxLayout->addWidget(btnDone);

        btnCancel = new QPushButton(horizontalLayout);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        hboxLayout->addWidget(btnCancel);

        btnDo = new QPushButton(horizontalLayout);
        btnDo->setObjectName(QString::fromUtf8("btnDo"));

        hboxLayout->addWidget(btnDo);

        horizontalLayout_2 = new QWidget(ConfirmPrediction);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setGeometry(QRect(10, 570, 496, 25));
        hboxLayout1 = new QHBoxLayout(horizontalLayout_2);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        radioArea = new QRadioButton(horizontalLayout_2);
        radioArea->setObjectName(QString::fromUtf8("radioArea"));

        hboxLayout1->addWidget(radioArea);

        radioRadius = new QRadioButton(horizontalLayout_2);
        radioRadius->setObjectName(QString::fromUtf8("radioRadius"));
        radioRadius->setChecked(true);

        hboxLayout1->addWidget(radioRadius);


        retranslateUi(ConfirmPrediction);

        outputTypeCombo->setCurrentIndex(0);
        plotTypeCombo->setCurrentIndex(0);
        displayUnitsCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfirmPrediction);
    } // setupUi

    void retranslateUi(QDialog *ConfirmPrediction)
    {
        ConfirmPrediction->setWindowTitle(QApplication::translate("ConfirmPrediction", "Q-Rap Prediction Request Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfirmPrediction", "Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfirmPrediction", "Sector", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ConfirmPrediction", "Radius", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ConfirmPrediction", "Technology", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("ConfirmPrediction", "Radio Inst ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("ConfirmPrediction", "Project", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("ConfirmPrediction", "Flag X", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("ConfirmPrediction", "Flag Z", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("ConfirmPrediction", "Status", 0, QApplication::UnicodeUTF8));
        RqRxlbl->setText(QApplication::translate("ConfirmPrediction", "Minimum Required Received Signal:", 0, QApplication::UnicodeUTF8));
        AngleReslbl->setText(QApplication::translate("ConfirmPrediction", "Minimum Angle Resolution", 0, QApplication::UnicodeUTF8));
        plotReslbl->setText(QApplication::translate("ConfirmPrediction", "Plot Resolution:", 0, QApplication::UnicodeUTF8));
        kFactorIlbl->setText(QApplication::translate("ConfirmPrediction", "k-Factor for Interferers:", 0, QApplication::UnicodeUTF8));
        kFactorSlbl->setText(QApplication::translate("ConfirmPrediction", "k-Factor for Servers:", 0, QApplication::UnicodeUTF8));
        RqCCIRlbl->setText(QApplication::translate("ConfirmPrediction", "Required Carrier to Co-Channel Interference Ratio:", 0, QApplication::UnicodeUTF8));
        RqCAIRlbl->setText(QApplication::translate("ConfirmPrediction", "Required Carrier to Adjacent-Channel Interferece Ratio:", 0, QApplication::UnicodeUTF8));
        RqEbNolbl->setText(QApplication::translate("ConfirmPrediction", "Required Energy per Bit to Noise Ratio:", 0, QApplication::UnicodeUTF8));
        FadeMlbl->setText(QApplication::translate("ConfirmPrediction", "Fade Margin:", 0, QApplication::UnicodeUTF8));
        Nolbl->setText(QApplication::translate("ConfirmPrediction", "Noise Level:", 0, QApplication::UnicodeUTF8));
        RqSNlbl->setText(QApplication::translate("ConfirmPrediction", "Required Signal to Noise Raio:", 0, QApplication::UnicodeUTF8));
        mobilelbl->setText(QApplication::translate("ConfirmPrediction", "Mobile Radio Used:", 0, QApplication::UnicodeUTF8));
        Dirlbl->setText(QApplication::translate("ConfirmPrediction", "Output Directory:", 0, QApplication::UnicodeUTF8));
        outputDirectoryEdit->setText(QString());
        filelbl->setText(QApplication::translate("ConfirmPrediction", "Output File Name:", 0, QApplication::UnicodeUTF8));
        outputFileNameEdit->setText(QApplication::translate("ConfirmPrediction", "RapOutput", 0, QApplication::UnicodeUTF8));
        outputTypeCombo->clear();
        outputTypeCombo->insertItems(0, QStringList()
         << QApplication::translate("ConfirmPrediction", "Edras Image Forma (*.img)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "GRASS", 0, QApplication::UnicodeUTF8)
        );
        FileTypelbl->setText(QApplication::translate("ConfirmPrediction", "File Output Type:", 0, QApplication::UnicodeUTF8));
        plotlbl->setText(QApplication::translate("ConfirmPrediction", "Prediction Type / Plot Type:", 0, QApplication::UnicodeUTF8));
        plotTypeCombo->clear();
        plotTypeCombo->insertItems(0, QStringList()
         << QApplication::translate("ConfirmPrediction", "Coverage", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Primary Server", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Secondary Server", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Number of Servers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Carrier to Co-channel Interference Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Carrier to Adjacent-channel Interf Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Interfered Areas", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Number of Interferers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Primary Co-channel Interferers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Primary Adjacent-channel Interferers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Signal to Noise Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Digital Elevation Model", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Update Cell Centroids", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "Estimate Traffic Distribution", 0, QApplication::UnicodeUTF8)
        );
        unitlbl->setText(QApplication::translate("ConfirmPrediction", "Output Units:", 0, QApplication::UnicodeUTF8));
        displayUnitsCombo->clear();
        displayUnitsCombo->insertItems(0, QStringList()
         << QApplication::translate("ConfirmPrediction", "dB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBW", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBm", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBuV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBuVm", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBWm2Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmPrediction", "dBWm2", 0, QApplication::UnicodeUTF8)
        );
        useClutterCheckBox->setText(QApplication::translate("ConfirmPrediction", "Use Clutter ", 0, QApplication::UnicodeUTF8));
        downLinkRadio->setText(QApplication::translate("ConfirmPrediction", "Down Link", 0, QApplication::UnicodeUTF8));
        upLinkRadio->setText(QApplication::translate("ConfirmPrediction", "Up Link", 0, QApplication::UnicodeUTF8));
        pushButtonPrint->setText(QApplication::translate("ConfirmPrediction", "Print", 0, QApplication::UnicodeUTF8));
        btnDone->setText(QApplication::translate("ConfirmPrediction", "Done", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("ConfirmPrediction", "Cancel", 0, QApplication::UnicodeUTF8));
        btnDo->setText(QApplication::translate("ConfirmPrediction", "Do", 0, QApplication::UnicodeUTF8));
        radioArea->setText(QApplication::translate("ConfirmPrediction", "Predict selected area", 0, QApplication::UnicodeUTF8));
        radioRadius->setText(QApplication::translate("ConfirmPrediction", "Predict selected area with radius included", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfirmPrediction: public Ui_ConfirmPrediction {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMPREDICTION_H
