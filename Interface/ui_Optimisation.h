/********************************************************************************
** Form generated from reading UI file 'OptimisationC18777.ui'
**
** Created: Thu Jan 9 00:28:52 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef OPTIMISATIONC18777_H
#define OPTIMISATIONC18777_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Optimisation
{
public:
    QWidget *gridLayout_2;
    QGridLayout *gridlayAccept;
    QPushButton *pushOk;
    QPushButton *pushCancel;
    QTableWidget *tableInstallations;
    QLabel *lblTechnology;
    QComboBox *cbMinClear;
    QLabel *lblMaxPath;
    QComboBox *cbTechnology;
    QLabel *lblMinClear;
    QDoubleSpinBox *dsbMaxPath;
    QDoubleSpinBox *dsbMaxDist;
    QLabel *lblProject;
    QComboBox *cbProject;
    QDoubleSpinBox *dsbKfactor;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QLabel *lblPath;
    QTableWidget *tableAntennas;
    QTableWidget *tableCables;
    QDoubleSpinBox *dsbMinMastheight;
    QLabel *lblMinMastHeight1;
    QLabel *lblMaxMastheight;
    QDoubleSpinBox *dsbMaxMastheight;
    QLabel *lblMaxMastheight2;
    QCheckBox *chkClearance;
    QCheckBox *chkStrength;
    QDoubleSpinBox *dsbPower;
    QCheckBox *chkPower;
    QLabel *lblPower;
    QLabel *lblSensitivity;
    QCheckBox *chkSensitivity;
    QDoubleSpinBox *dsbSensitivity;
    QLabel *lblRate;
    QComboBox *cbDatarate;
    QCheckBox *chkSignalStrength;
    QLabel *lblSignalStrength;
    QDoubleSpinBox *dsbSignalStrength;
    QComboBox *cbMinLinks;
    QTableWidget *tableConnectors;
    QLabel *lblNoMast_2;
    QLabel *lblNoMast_3;
    QLabel *lblNoMast_4;
    QLabel *lblNoMast_5;
    QFrame *frame;
    QLabel *lblNoMast;
    QFrame *frame_2;
    QFrame *frame_3;
    QLabel *lblMaxDist;
    QLabel *lblKfactor;
    QFrame *frame_4;
    QFrame *frame_5;
    QLabel *lblSettings;

    void setupUi(QDialog *Optimisation)
    {
        if (Optimisation->objectName().isEmpty())
            Optimisation->setObjectName(QString::fromUtf8("Optimisation"));
        Optimisation->resize(1502, 794);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../.designer/Optimisation.png"), QSize(), QIcon::Normal, QIcon::Off);
        Optimisation->setWindowIcon(icon);
        gridLayout_2 = new QWidget(Optimisation);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(1200, 730, 178, 36));
        gridlayAccept = new QGridLayout(gridLayout_2);
        gridlayAccept->setObjectName(QString::fromUtf8("gridlayAccept"));
        gridlayAccept->setContentsMargins(0, 0, 0, 0);
        pushOk = new QPushButton(gridLayout_2);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));

        gridlayAccept->addWidget(pushOk, 0, 0, 1, 1);

        pushCancel = new QPushButton(gridLayout_2);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        gridlayAccept->addWidget(pushCancel, 0, 1, 1, 1);

        tableInstallations = new QTableWidget(Optimisation);
        if (tableInstallations->columnCount() < 6)
            tableInstallations->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableInstallations->setObjectName(QString::fromUtf8("tableInstallations"));
        tableInstallations->setGeometry(QRect(420, 40, 651, 231));
        tableInstallations->setMaximumSize(QSize(651, 16777215));
        tableInstallations->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lblTechnology = new QLabel(Optimisation);
        lblTechnology->setObjectName(QString::fromUtf8("lblTechnology"));
        lblTechnology->setGeometry(QRect(70, 110, 81, 27));
        cbMinClear = new QComboBox(Optimisation);
        cbMinClear->setObjectName(QString::fromUtf8("cbMinClear"));
        cbMinClear->setGeometry(QRect(280, 705, 96, 27));
        lblMaxPath = new QLabel(Optimisation);
        lblMaxPath->setObjectName(QString::fromUtf8("lblMaxPath"));
        lblMaxPath->setGeometry(QRect(100, 645, 171, 16));
        cbTechnology = new QComboBox(Optimisation);
        cbTechnology->setObjectName(QString::fromUtf8("cbTechnology"));
        cbTechnology->setGeometry(QRect(170, 110, 156, 27));
        lblMinClear = new QLabel(Optimisation);
        lblMinClear->setObjectName(QString::fromUtf8("lblMinClear"));
        lblMinClear->setGeometry(QRect(100, 705, 161, 27));
        dsbMaxPath = new QDoubleSpinBox(Optimisation);
        dsbMaxPath->setObjectName(QString::fromUtf8("dsbMaxPath"));
        dsbMaxPath->setGeometry(QRect(290, 635, 86, 28));
        dsbMaxPath->setMaximum(300);
        dsbMaxDist = new QDoubleSpinBox(Optimisation);
        dsbMaxDist->setObjectName(QString::fromUtf8("dsbMaxDist"));
        dsbMaxDist->setGeometry(QRect(290, 670, 86, 28));
        dsbMaxDist->setMaximum(1000);
        lblProject = new QLabel(Optimisation);
        lblProject->setObjectName(QString::fromUtf8("lblProject"));
        lblProject->setGeometry(QRect(70, 190, 76, 27));
        cbProject = new QComboBox(Optimisation);
        cbProject->setObjectName(QString::fromUtf8("cbProject"));
        cbProject->setGeometry(QRect(170, 190, 156, 27));
        dsbKfactor = new QDoubleSpinBox(Optimisation);
        dsbKfactor->setObjectName(QString::fromUtf8("dsbKfactor"));
        dsbKfactor->setGeometry(QRect(305, 740, 71, 28));
        dsbKfactor->setMaximum(10);
        dsbKfactor->setSingleStep(0.01);
        dsbKfactor->setValue(1);
        checkBox = new QCheckBox(Optimisation);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(1155, 637, 221, 16));
        checkBox->setChecked(true);
        lineEdit = new QLineEdit(Optimisation);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(1160, 685, 246, 26));
        lblPath = new QLabel(Optimisation);
        lblPath->setObjectName(QString::fromUtf8("lblPath"));
        lblPath->setGeometry(QRect(1160, 660, 181, 18));
        tableAntennas = new QTableWidget(Optimisation);
        if (tableAntennas->columnCount() < 5)
            tableAntennas->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableAntennas->setHorizontalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableAntennas->setHorizontalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableAntennas->setHorizontalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        tableAntennas->setHorizontalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        tableAntennas->setHorizontalHeaderItem(4, __qtablewidgetitem10);
        tableAntennas->setObjectName(QString::fromUtf8("tableAntennas"));
        tableAntennas->setGeometry(QRect(10, 310, 521, 301));
        tableAntennas->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableCables = new QTableWidget(Optimisation);
        if (tableCables->columnCount() < 4)
            tableCables->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        tableCables->setHorizontalHeaderItem(0, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        tableCables->setHorizontalHeaderItem(1, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        tableCables->setHorizontalHeaderItem(2, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        tableCables->setHorizontalHeaderItem(3, __qtablewidgetitem14);
        tableCables->setObjectName(QString::fromUtf8("tableCables"));
        tableCables->setGeometry(QRect(540, 310, 421, 301));
        tableCables->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dsbMinMastheight = new QDoubleSpinBox(Optimisation);
        dsbMinMastheight->setObjectName(QString::fromUtf8("dsbMinMastheight"));
        dsbMinMastheight->setGeometry(QRect(1350, 85, 86, 28));
        dsbMinMastheight->setMaximum(300);
        lblMinMastHeight1 = new QLabel(Optimisation);
        lblMinMastHeight1->setObjectName(QString::fromUtf8("lblMinMastHeight1"));
        lblMinMastHeight1->setGeometry(QRect(1160, 90, 181, 16));
        lblMaxMastheight = new QLabel(Optimisation);
        lblMaxMastheight->setObjectName(QString::fromUtf8("lblMaxMastheight"));
        lblMaxMastheight->setGeometry(QRect(1160, 120, 181, 27));
        dsbMaxMastheight = new QDoubleSpinBox(Optimisation);
        dsbMaxMastheight->setObjectName(QString::fromUtf8("dsbMaxMastheight"));
        dsbMaxMastheight->setGeometry(QRect(1350, 120, 86, 28));
        dsbMaxMastheight->setMaximum(1000);
        lblMaxMastheight2 = new QLabel(Optimisation);
        lblMaxMastheight2->setObjectName(QString::fromUtf8("lblMaxMastheight2"));
        lblMaxMastheight2->setGeometry(QRect(1210, 170, 181, 27));
        chkClearance = new QCheckBox(Optimisation);
        chkClearance->setObjectName(QString::fromUtf8("chkClearance"));
        chkClearance->setGeometry(QRect(70, 730, 21, 22));
        chkClearance->setChecked(true);
        chkStrength = new QCheckBox(Optimisation);
        chkStrength->setObjectName(QString::fromUtf8("chkStrength"));
        chkStrength->setGeometry(QRect(70, 660, 21, 22));
        chkStrength->setChecked(true);
        dsbPower = new QDoubleSpinBox(Optimisation);
        dsbPower->setObjectName(QString::fromUtf8("dsbPower"));
        dsbPower->setGeometry(QRect(590, 670, 86, 28));
        dsbPower->setMaximum(300);
        chkPower = new QCheckBox(Optimisation);
        chkPower->setObjectName(QString::fromUtf8("chkPower"));
        chkPower->setGeometry(QRect(530, 640, 21, 22));
        chkPower->setChecked(true);
        lblPower = new QLabel(Optimisation);
        lblPower->setObjectName(QString::fromUtf8("lblPower"));
        lblPower->setGeometry(QRect(570, 640, 171, 16));
        lblSensitivity = new QLabel(Optimisation);
        lblSensitivity->setObjectName(QString::fromUtf8("lblSensitivity"));
        lblSensitivity->setGeometry(QRect(570, 710, 181, 16));
        chkSensitivity = new QCheckBox(Optimisation);
        chkSensitivity->setObjectName(QString::fromUtf8("chkSensitivity"));
        chkSensitivity->setGeometry(QRect(530, 710, 21, 22));
        chkSensitivity->setChecked(true);
        dsbSensitivity = new QDoubleSpinBox(Optimisation);
        dsbSensitivity->setObjectName(QString::fromUtf8("dsbSensitivity"));
        dsbSensitivity->setGeometry(QRect(590, 740, 86, 28));
        dsbSensitivity->setMinimum(-300);
        dsbSensitivity->setMaximum(300);
        lblRate = new QLabel(Optimisation);
        lblRate->setObjectName(QString::fromUtf8("lblRate"));
        lblRate->setGeometry(QRect(820, 710, 171, 16));
        cbDatarate = new QComboBox(Optimisation);
        cbDatarate->setObjectName(QString::fromUtf8("cbDatarate"));
        cbDatarate->setGeometry(QRect(850, 740, 96, 27));
        chkSignalStrength = new QCheckBox(Optimisation);
        chkSignalStrength->setObjectName(QString::fromUtf8("chkSignalStrength"));
        chkSignalStrength->setGeometry(QRect(780, 640, 21, 22));
        chkSignalStrength->setChecked(true);
        lblSignalStrength = new QLabel(Optimisation);
        lblSignalStrength->setObjectName(QString::fromUtf8("lblSignalStrength"));
        lblSignalStrength->setGeometry(QRect(820, 620, 181, 51));
        lblSignalStrength->setWordWrap(true);
        dsbSignalStrength = new QDoubleSpinBox(Optimisation);
        dsbSignalStrength->setObjectName(QString::fromUtf8("dsbSignalStrength"));
        dsbSignalStrength->setGeometry(QRect(850, 670, 86, 28));
        dsbSignalStrength->setMinimum(-300);
        dsbSignalStrength->setMaximum(300);
        cbMinLinks = new QComboBox(Optimisation);
        cbMinLinks->setObjectName(QString::fromUtf8("cbMinLinks"));
        cbMinLinks->setGeometry(QRect(1250, 220, 96, 27));
        tableConnectors = new QTableWidget(Optimisation);
        if (tableConnectors->columnCount() < 5)
            tableConnectors->setColumnCount(5);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        tableConnectors->setHorizontalHeaderItem(0, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        tableConnectors->setHorizontalHeaderItem(1, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        tableConnectors->setHorizontalHeaderItem(2, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        tableConnectors->setHorizontalHeaderItem(3, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        tableConnectors->setHorizontalHeaderItem(4, __qtablewidgetitem19);
        tableConnectors->setObjectName(QString::fromUtf8("tableConnectors"));
        tableConnectors->setGeometry(QRect(970, 310, 521, 301));
        tableConnectors->setEditTriggers(QAbstractItemView::NoEditTriggers);
        lblNoMast_2 = new QLabel(Optimisation);
        lblNoMast_2->setObjectName(QString::fromUtf8("lblNoMast_2"));
        lblNoMast_2->setGeometry(QRect(220, 270, 91, 41));
        lblNoMast_2->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        lblNoMast_3 = new QLabel(Optimisation);
        lblNoMast_3->setObjectName(QString::fromUtf8("lblNoMast_3"));
        lblNoMast_3->setGeometry(QRect(730, 270, 91, 41));
        lblNoMast_3->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        lblNoMast_4 = new QLabel(Optimisation);
        lblNoMast_4->setObjectName(QString::fromUtf8("lblNoMast_4"));
        lblNoMast_4->setGeometry(QRect(1160, 270, 191, 41));
        lblNoMast_4->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        lblNoMast_5 = new QLabel(Optimisation);
        lblNoMast_5->setObjectName(QString::fromUtf8("lblNoMast_5"));
        lblNoMast_5->setGeometry(QRect(730, 0, 91, 41));
        lblNoMast_5->setStyleSheet(QString::fromUtf8("font: 75 italic 16pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        frame = new QFrame(Optimisation);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(1120, 30, 341, 241));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        lblNoMast = new QLabel(frame);
        lblNoMast->setObjectName(QString::fromUtf8("lblNoMast"));
        lblNoMast->setGeometry(QRect(120, 10, 161, 31));
        lblNoMast->setStyleSheet(QString::fromUtf8("font: 75 italic 12pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        frame_2 = new QFrame(Optimisation);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(40, 30, 331, 241));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Plain);
        frame_3 = new QFrame(Optimisation);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(60, 620, 331, 161));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        lblMaxDist = new QLabel(frame_3);
        lblMaxDist->setObjectName(QString::fromUtf8("lblMaxDist"));
        lblMaxDist->setGeometry(QRect(40, 50, 171, 27));
        lblKfactor = new QLabel(frame_3);
        lblKfactor->setObjectName(QString::fromUtf8("lblKfactor"));
        lblKfactor->setGeometry(QRect(40, 120, 61, 27));
        frame_4 = new QFrame(Optimisation);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(490, 620, 531, 161));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        frame_5 = new QFrame(Optimisation);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setGeometry(QRect(1120, 620, 311, 161));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        lblSettings = new QLabel(Optimisation);
        lblSettings->setObjectName(QString::fromUtf8("lblSettings"));
        lblSettings->setGeometry(QRect(180, 40, 161, 31));
        lblSettings->setStyleSheet(QString::fromUtf8("font: 75 italic 12pt \"Ubuntu\";\n"
"text-decoration: underline;"));
        frame_4->raise();
        frame_5->raise();
        frame_3->raise();
        frame->raise();
        frame_2->raise();
        gridLayout_2->raise();
        tableInstallations->raise();
        lblTechnology->raise();
        cbMinClear->raise();
        lblMaxPath->raise();
        cbTechnology->raise();
        lblMinClear->raise();
        dsbMaxPath->raise();
        dsbMaxDist->raise();
        lblProject->raise();
        cbProject->raise();
        dsbKfactor->raise();
        checkBox->raise();
        lineEdit->raise();
        lblPath->raise();
        tableAntennas->raise();
        tableCables->raise();
        dsbMinMastheight->raise();
        lblMinMastHeight1->raise();
        lblMaxMastheight->raise();
        dsbMaxMastheight->raise();
        lblMaxMastheight2->raise();
        chkClearance->raise();
        chkStrength->raise();
        dsbPower->raise();
        chkPower->raise();
        lblPower->raise();
        lblSensitivity->raise();
        chkSensitivity->raise();
        dsbSensitivity->raise();
        lblRate->raise();
        cbDatarate->raise();
        chkSignalStrength->raise();
        lblSignalStrength->raise();
        dsbSignalStrength->raise();
        cbMinLinks->raise();
        tableConnectors->raise();
        lblNoMast_2->raise();
        lblNoMast_3->raise();
        lblNoMast_4->raise();
        lblNoMast_5->raise();
        lblSettings->raise();

        retranslateUi(Optimisation);

        QMetaObject::connectSlotsByName(Optimisation);
    } // setupUi

    void retranslateUi(QDialog *Optimisation)
    {
        Optimisation->setWindowTitle(QApplication::translate("Optimisation", "Q-Rap Link Optimisation Request", 0, QApplication::UnicodeUTF8));
        pushOk->setText(QApplication::translate("Optimisation", "OK", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("Optimisation", "Cancel", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableInstallations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Optimisation", "Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableInstallations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Optimisation", "SiteID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableInstallations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("Optimisation", "Status", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableInstallations->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("Optimisation", "Existing Mast", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableInstallations->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("Optimisation", "Min Height", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableInstallations->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("Optimisation", "Max Height", 0, QApplication::UnicodeUTF8));
        lblTechnology->setText(QApplication::translate("Optimisation", "Technology: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbMinClear->setToolTip(QApplication::translate("Optimisation", "0 - means Line of Sight; 60 means 60% of the 1st Fresnel Zone is clear; 100 means the entire 1st Fresnel Zone is clear.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblMaxPath->setText(QApplication::translate("Optimisation", "Maximum Path Loss (dB) : ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbTechnology->setToolTip(QApplication::translate("Optimisation", "This is the technology from which the parameters will be derived and from which the radio installations will generated. The technology must have a Default Site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblMinClear->setText(QApplication::translate("Optimisation", "Minimum Clearance (%): ", 0, QApplication::UnicodeUTF8));
        lblProject->setText(QApplication::translate("Optimisation", "Project: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbProject->setToolTip(QApplication::translate("Optimisation", "The project to which the generated Radioinstallations should be assigned", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        dsbKfactor->setWhatsThis(QApplication::translate("Optimisation", "This is used to indicate the refractivity gradient in the lower troposphere", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        checkBox->setText(QApplication::translate("Optimisation", "Save working links pdf files?", 0, QApplication::UnicodeUTF8));
        lblPath->setText(QApplication::translate("Optimisation", "Directory to save pdf files:", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableAntennas->horizontalHeaderItem(0);
        ___qtablewidgetitem6->setText(QApplication::translate("Optimisation", "Device Name", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableAntennas->horizontalHeaderItem(1);
        ___qtablewidgetitem7->setText(QApplication::translate("Optimisation", "AntennaID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableAntennas->horizontalHeaderItem(2);
        ___qtablewidgetitem8->setText(QApplication::translate("Optimisation", "Manufacturer", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem9 = tableAntennas->horizontalHeaderItem(3);
        ___qtablewidgetitem9->setText(QApplication::translate("Optimisation", "Cost", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem10 = tableAntennas->horizontalHeaderItem(4);
        ___qtablewidgetitem10->setText(QApplication::translate("Optimisation", "Gain", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem11 = tableCables->horizontalHeaderItem(0);
        ___qtablewidgetitem11->setText(QApplication::translate("Optimisation", "Cable Type", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem12 = tableCables->horizontalHeaderItem(1);
        ___qtablewidgetitem12->setText(QApplication::translate("Optimisation", "CableID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem13 = tableCables->horizontalHeaderItem(2);
        ___qtablewidgetitem13->setText(QApplication::translate("Optimisation", "Cost p/m", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem14 = tableCables->horizontalHeaderItem(3);
        ___qtablewidgetitem14->setText(QApplication::translate("Optimisation", "Loss p/m", 0, QApplication::UnicodeUTF8));
        lblMinMastHeight1->setText(QApplication::translate("Optimisation", "Minimum Mast Height (m):", 0, QApplication::UnicodeUTF8));
        lblMaxMastheight->setText(QApplication::translate("Optimisation", "Maximum Mast Height (m): ", 0, QApplication::UnicodeUTF8));
        lblMaxMastheight2->setText(QApplication::translate("Optimisation", "Specify min links per site:", 0, QApplication::UnicodeUTF8));
        chkClearance->setText(QString());
        chkStrength->setText(QString());
        chkPower->setText(QString());
        lblPower->setText(QApplication::translate("Optimisation", "Specify txpower (dBm):", 0, QApplication::UnicodeUTF8));
        lblSensitivity->setText(QApplication::translate("Optimisation", "Specify rxsensitivity (dBm):", 0, QApplication::UnicodeUTF8));
        chkSensitivity->setText(QString());
        lblRate->setText(QApplication::translate("Optimisation", "Specify data rate (Mbps):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbDatarate->setToolTip(QApplication::translate("Optimisation", "<html><head/><body><p>The data rate for the system that has an associated transmitter power and receiver sensitivity if none is specified.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        chkSignalStrength->setText(QString());
        lblSignalStrength->setText(QApplication::translate("Optimisation", "Specify minimum required signal strength (dBm):", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbMinLinks->setToolTip(QApplication::translate("Optimisation", "<html><head/><body><p>The number of links that the optimisation algorithm will aim to form per site</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        QTableWidgetItem *___qtablewidgetitem15 = tableConnectors->horizontalHeaderItem(0);
        ___qtablewidgetitem15->setText(QApplication::translate("Optimisation", "Description", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem16 = tableConnectors->horizontalHeaderItem(1);
        ___qtablewidgetitem16->setText(QApplication::translate("Optimisation", "Comb/Split ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem17 = tableConnectors->horizontalHeaderItem(2);
        ___qtablewidgetitem17->setText(QApplication::translate("Optimisation", "Comb/Split", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem18 = tableConnectors->horizontalHeaderItem(3);
        ___qtablewidgetitem18->setText(QApplication::translate("Optimisation", "Cost", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem19 = tableConnectors->horizontalHeaderItem(4);
        ___qtablewidgetitem19->setText(QApplication::translate("Optimisation", "Losses", 0, QApplication::UnicodeUTF8));
        lblNoMast_2->setText(QApplication::translate("Optimisation", "Antennas", 0, QApplication::UnicodeUTF8));
        lblNoMast_3->setText(QApplication::translate("Optimisation", "Cables", 0, QApplication::UnicodeUTF8));
        lblNoMast_4->setText(QApplication::translate("Optimisation", "Combiners/Splitters", 0, QApplication::UnicodeUTF8));
        lblNoMast_5->setText(QApplication::translate("Optimisation", "Sites", 0, QApplication::UnicodeUTF8));
        lblNoMast->setText(QApplication::translate("Optimisation", "No Existing Mast", 0, QApplication::UnicodeUTF8));
        lblMaxDist->setText(QApplication::translate("Optimisation", "Maximum Distance (km) : ", 0, QApplication::UnicodeUTF8));
        lblKfactor->setText(QApplication::translate("Optimisation", "k-Factor : ", 0, QApplication::UnicodeUTF8));
        lblSettings->setText(QApplication::translate("Optimisation", "Settings", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Optimisation: public Ui_Optimisation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // OPTIMISATIONC18777_H
