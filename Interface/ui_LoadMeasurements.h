/********************************************************************************
** Form generated from reading UI file 'LoadMeasurements.ui'
**
** Created: Tue May 1 20:22:53 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADMEASUREMENTS_H
#define UI_LOADMEASUREMENTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoadMeasurements
{
public:
    QWidget *layoutWidgetFF;
    QHBoxLayout *layoutFileFormat;
    QLabel *lFileFormat;
    QComboBox *cBoxFileType;
    QLabel *lFiles;
    QLineEdit *lEditFileToLoad;
    QPushButton *pButtonBrowse;
    QPushButton *pButtonLoad;
    QWidget *layoutWidgetDouble;
    QGridLayout *gridLayoutsdataLinks;
    QLabel *lMeasType;
    QComboBox *cBoxMeasType;
    QLabel *lMeasSource;
    QComboBox *cBoxMeasSource;
    QLabel *lPosSource;
    QComboBox *cBoxPosSource;
    QListWidget *listWidgetFiles;
    QPushButton *pButtonClose;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *lTxCell;
    QComboBox *cBoxCell;
    QLabel *lFrequency;
    QDoubleSpinBox *dSpinBoxFreq;
    QLabel *lSensitivity;
    QDoubleSpinBox *dSpinBoxSensitivity;
    QLabel *lMobileHeight;
    QDoubleSpinBox *dSpinBoxMobileH;

    void setupUi(QDialog *LoadMeasurements)
    {
        if (LoadMeasurements->objectName().isEmpty())
            LoadMeasurements->setObjectName(QString::fromUtf8("LoadMeasurements"));
        LoadMeasurements->setWindowModality(Qt::NonModal);
        LoadMeasurements->resize(501, 541);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../../QRapBusy/Coverage.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoadMeasurements->setWindowIcon(icon);
        layoutWidgetFF = new QWidget(LoadMeasurements);
        layoutWidgetFF->setObjectName(QString::fromUtf8("layoutWidgetFF"));
        layoutWidgetFF->setGeometry(QRect(10, 130, 481, 29));
        layoutFileFormat = new QHBoxLayout(layoutWidgetFF);
        layoutFileFormat->setObjectName(QString::fromUtf8("layoutFileFormat"));
        layoutFileFormat->setContentsMargins(0, 0, 0, 0);
        lFileFormat = new QLabel(layoutWidgetFF);
        lFileFormat->setObjectName(QString::fromUtf8("lFileFormat"));
        lFileFormat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layoutFileFormat->addWidget(lFileFormat);

        cBoxFileType = new QComboBox(layoutWidgetFF);
        cBoxFileType->setObjectName(QString::fromUtf8("cBoxFileType"));

        layoutFileFormat->addWidget(cBoxFileType);

        lFiles = new QLabel(LoadMeasurements);
        lFiles->setObjectName(QString::fromUtf8("lFiles"));
        lFiles->setGeometry(QRect(10, 310, 101, 17));
        lEditFileToLoad = new QLineEdit(LoadMeasurements);
        lEditFileToLoad->setObjectName(QString::fromUtf8("lEditFileToLoad"));
        lEditFileToLoad->setGeometry(QRect(10, 330, 391, 27));
        pButtonBrowse = new QPushButton(LoadMeasurements);
        pButtonBrowse->setObjectName(QString::fromUtf8("pButtonBrowse"));
        pButtonBrowse->setGeometry(QRect(410, 330, 81, 27));
        pButtonLoad = new QPushButton(LoadMeasurements);
        pButtonLoad->setObjectName(QString::fromUtf8("pButtonLoad"));
        pButtonLoad->setGeometry(QRect(340, 470, 151, 27));
        layoutWidgetDouble = new QWidget(LoadMeasurements);
        layoutWidgetDouble->setObjectName(QString::fromUtf8("layoutWidgetDouble"));
        layoutWidgetDouble->setGeometry(QRect(10, 20, 481, 95));
        gridLayoutsdataLinks = new QGridLayout(layoutWidgetDouble);
        gridLayoutsdataLinks->setObjectName(QString::fromUtf8("gridLayoutsdataLinks"));
        gridLayoutsdataLinks->setContentsMargins(0, 0, 0, 0);
        lMeasType = new QLabel(layoutWidgetDouble);
        lMeasType->setObjectName(QString::fromUtf8("lMeasType"));
        lMeasType->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lMeasType, 0, 0, 1, 1);

        cBoxMeasType = new QComboBox(layoutWidgetDouble);
        cBoxMeasType->setObjectName(QString::fromUtf8("cBoxMeasType"));

        gridLayoutsdataLinks->addWidget(cBoxMeasType, 0, 1, 1, 1);

        lMeasSource = new QLabel(layoutWidgetDouble);
        lMeasSource->setObjectName(QString::fromUtf8("lMeasSource"));
        lMeasSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lMeasSource, 1, 0, 1, 1);

        cBoxMeasSource = new QComboBox(layoutWidgetDouble);
        cBoxMeasSource->setObjectName(QString::fromUtf8("cBoxMeasSource"));

        gridLayoutsdataLinks->addWidget(cBoxMeasSource, 1, 1, 1, 1);

        lPosSource = new QLabel(layoutWidgetDouble);
        lPosSource->setObjectName(QString::fromUtf8("lPosSource"));
        lPosSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lPosSource, 2, 0, 1, 1);

        cBoxPosSource = new QComboBox(layoutWidgetDouble);
        cBoxPosSource->setObjectName(QString::fromUtf8("cBoxPosSource"));

        gridLayoutsdataLinks->addWidget(cBoxPosSource, 2, 1, 1, 1);

        listWidgetFiles = new QListWidget(LoadMeasurements);
        listWidgetFiles->setObjectName(QString::fromUtf8("listWidgetFiles"));
        listWidgetFiles->setGeometry(QRect(10, 370, 321, 161));
        pButtonClose = new QPushButton(LoadMeasurements);
        pButtonClose->setObjectName(QString::fromUtf8("pButtonClose"));
        pButtonClose->setGeometry(QRect(340, 500, 151, 27));
        layoutWidget = new QWidget(LoadMeasurements);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 170, 271, 128));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lTxCell = new QLabel(layoutWidget);
        lTxCell->setObjectName(QString::fromUtf8("lTxCell"));
        lTxCell->setEnabled(true);
        lTxCell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lTxCell, 0, 0, 1, 1);

        cBoxCell = new QComboBox(layoutWidget);
        cBoxCell->setObjectName(QString::fromUtf8("cBoxCell"));
        cBoxCell->setEnabled(true);

        gridLayout->addWidget(cBoxCell, 0, 1, 1, 1);

        lFrequency = new QLabel(layoutWidget);
        lFrequency->setObjectName(QString::fromUtf8("lFrequency"));
        lFrequency->setEnabled(true);
        lFrequency->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lFrequency, 1, 0, 1, 1);

        dSpinBoxFreq = new QDoubleSpinBox(layoutWidget);
        dSpinBoxFreq->setObjectName(QString::fromUtf8("dSpinBoxFreq"));
        dSpinBoxFreq->setEnabled(true);
        dSpinBoxFreq->setMinimum(30);
        dSpinBoxFreq->setMaximum(23000);
        dSpinBoxFreq->setValue(900);

        gridLayout->addWidget(dSpinBoxFreq, 1, 1, 1, 1);

        lSensitivity = new QLabel(layoutWidget);
        lSensitivity->setObjectName(QString::fromUtf8("lSensitivity"));
        lSensitivity->setEnabled(true);
        lSensitivity->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lSensitivity, 2, 0, 1, 1);

        dSpinBoxSensitivity = new QDoubleSpinBox(layoutWidget);
        dSpinBoxSensitivity->setObjectName(QString::fromUtf8("dSpinBoxSensitivity"));
        dSpinBoxSensitivity->setEnabled(true);
        dSpinBoxSensitivity->setMinimum(-150);
        dSpinBoxSensitivity->setMaximum(0);
        dSpinBoxSensitivity->setValue(-110);

        gridLayout->addWidget(dSpinBoxSensitivity, 2, 1, 1, 1);

        lMobileHeight = new QLabel(layoutWidget);
        lMobileHeight->setObjectName(QString::fromUtf8("lMobileHeight"));
        lMobileHeight->setEnabled(true);
        lMobileHeight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lMobileHeight, 3, 0, 1, 1);

        dSpinBoxMobileH = new QDoubleSpinBox(layoutWidget);
        dSpinBoxMobileH->setObjectName(QString::fromUtf8("dSpinBoxMobileH"));
        dSpinBoxMobileH->setEnabled(true);
        dSpinBoxMobileH->setMinimum(0.5);
        dSpinBoxMobileH->setMaximum(30);
        dSpinBoxMobileH->setValue(1.3);

        gridLayout->addWidget(dSpinBoxMobileH, 3, 1, 1, 1);


        retranslateUi(LoadMeasurements);

        QMetaObject::connectSlotsByName(LoadMeasurements);
    } // setupUi

    void retranslateUi(QDialog *LoadMeasurements)
    {
        LoadMeasurements->setWindowTitle(QApplication::translate("LoadMeasurements", "Load Measurements", 0, QApplication::UnicodeUTF8));
        lFileFormat->setText(QApplication::translate("LoadMeasurements", "File Format to import:", 0, QApplication::UnicodeUTF8));
        cBoxFileType->clear();
        cBoxFileType->insertItems(0, QStringList()
         << QApplication::translate("LoadMeasurements", "GSM Drivetest", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoadMeasurements", "CW space delimited Lat, Lon and RxLev", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoadMeasurements", "CW *.csv with Timestamp, Lat, Lon, and RSSI ", 0, QApplication::UnicodeUTF8)
        );
        lFiles->setText(QApplication::translate("LoadMeasurements", "File(s) to load: ", 0, QApplication::UnicodeUTF8));
        pButtonBrowse->setText(QApplication::translate("LoadMeasurements", "Browse...", 0, QApplication::UnicodeUTF8));
        pButtonLoad->setText(QApplication::translate("LoadMeasurements", "Load Measurements", 0, QApplication::UnicodeUTF8));
        lMeasType->setText(QApplication::translate("LoadMeasurements", "Measurement Type:", 0, QApplication::UnicodeUTF8));
        lMeasSource->setText(QApplication::translate("LoadMeasurements", "Measurement Source:", 0, QApplication::UnicodeUTF8));
        lPosSource->setText(QApplication::translate("LoadMeasurements", "Position Source:", 0, QApplication::UnicodeUTF8));
        pButtonClose->setText(QApplication::translate("LoadMeasurements", "Close / Cancel", 0, QApplication::UnicodeUTF8));
        lTxCell->setText(QApplication::translate("LoadMeasurements", "Transmitting Cell:", 0, QApplication::UnicodeUTF8));
        lFrequency->setText(QApplication::translate("LoadMeasurements", "Transmitting Frequency:", 0, QApplication::UnicodeUTF8));
        lSensitivity->setText(QApplication::translate("LoadMeasurements", "Sensitivity:", 0, QApplication::UnicodeUTF8));
        lMobileHeight->setText(QApplication::translate("LoadMeasurements", "    Mobile Height:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoadMeasurements: public Ui_LoadMeasurements {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADMEASUREMENTS_H
