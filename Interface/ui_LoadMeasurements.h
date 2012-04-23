/********************************************************************************
** Form generated from reading UI file 'LoadMeasurements.ui'
**
** Created: Thu Apr 19 19:59:29 2012
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
#include <QtGui/QDialogButtonBox>
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
    QWidget *layoutWidget;
    QHBoxLayout *layoutFileFormat;
    QLabel *lFileFormat;
    QComboBox *cBoxFileType;
    QWidget *layoutWidget1;
    QHBoxLayout *layoutCell;
    QLabel *lTxCell;
    QComboBox *cBoxCell;
    QLabel *lFiles;
    QLineEdit *lEditFileToLoad;
    QPushButton *pButtonBrowse;
    QPushButton *pButtonLoad;
    QWidget *layoutWidget2;
    QGridLayout *gridLayoutsdataLinks;
    QLabel *lMeasType;
    QComboBox *cBoxMeasType;
    QLabel *lMeasSource;
    QComboBox *cBoxMeasSource;
    QLabel *lPosSource;
    QComboBox *cBoxPosSource;
    QDialogButtonBox *buttonBox;
    QListWidget *listWidgetFiles;

    void setupUi(QDialog *LoadMeasurements)
    {
        if (LoadMeasurements->objectName().isEmpty())
            LoadMeasurements->setObjectName(QString::fromUtf8("LoadMeasurements"));
        LoadMeasurements->setWindowModality(Qt::NonModal);
        LoadMeasurements->resize(499, 416);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../../../QRapBusy/Coverage.png"), QSize(), QIcon::Normal, QIcon::Off);
        LoadMeasurements->setWindowIcon(icon);
        layoutWidget = new QWidget(LoadMeasurements);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 130, 481, 29));
        layoutFileFormat = new QHBoxLayout(layoutWidget);
        layoutFileFormat->setObjectName(QString::fromUtf8("layoutFileFormat"));
        layoutFileFormat->setContentsMargins(0, 0, 0, 0);
        lFileFormat = new QLabel(layoutWidget);
        lFileFormat->setObjectName(QString::fromUtf8("lFileFormat"));
        lFileFormat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layoutFileFormat->addWidget(lFileFormat);

        cBoxFileType = new QComboBox(layoutWidget);
        cBoxFileType->setObjectName(QString::fromUtf8("cBoxFileType"));

        layoutFileFormat->addWidget(cBoxFileType);

        layoutWidget1 = new QWidget(LoadMeasurements);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 170, 481, 29));
        layoutCell = new QHBoxLayout(layoutWidget1);
        layoutCell->setObjectName(QString::fromUtf8("layoutCell"));
        layoutCell->setContentsMargins(0, 0, 0, 0);
        lTxCell = new QLabel(layoutWidget1);
        lTxCell->setObjectName(QString::fromUtf8("lTxCell"));
        lTxCell->setEnabled(false);
        lTxCell->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        layoutCell->addWidget(lTxCell);

        cBoxCell = new QComboBox(layoutWidget1);
        cBoxCell->setObjectName(QString::fromUtf8("cBoxCell"));
        cBoxCell->setEnabled(false);

        layoutCell->addWidget(cBoxCell);

        lFiles = new QLabel(LoadMeasurements);
        lFiles->setObjectName(QString::fromUtf8("lFiles"));
        lFiles->setGeometry(QRect(10, 210, 101, 17));
        lEditFileToLoad = new QLineEdit(LoadMeasurements);
        lEditFileToLoad->setObjectName(QString::fromUtf8("lEditFileToLoad"));
        lEditFileToLoad->setGeometry(QRect(10, 230, 401, 27));
        pButtonBrowse = new QPushButton(LoadMeasurements);
        pButtonBrowse->setObjectName(QString::fromUtf8("pButtonBrowse"));
        pButtonBrowse->setGeometry(QRect(420, 230, 71, 27));
        pButtonLoad = new QPushButton(LoadMeasurements);
        pButtonLoad->setObjectName(QString::fromUtf8("pButtonLoad"));
        pButtonLoad->setGeometry(QRect(340, 330, 151, 27));
        layoutWidget2 = new QWidget(LoadMeasurements);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 20, 481, 95));
        gridLayoutsdataLinks = new QGridLayout(layoutWidget2);
        gridLayoutsdataLinks->setObjectName(QString::fromUtf8("gridLayoutsdataLinks"));
        gridLayoutsdataLinks->setContentsMargins(0, 0, 0, 0);
        lMeasType = new QLabel(layoutWidget2);
        lMeasType->setObjectName(QString::fromUtf8("lMeasType"));
        lMeasType->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lMeasType, 0, 0, 1, 1);

        cBoxMeasType = new QComboBox(layoutWidget2);
        cBoxMeasType->setObjectName(QString::fromUtf8("cBoxMeasType"));

        gridLayoutsdataLinks->addWidget(cBoxMeasType, 0, 1, 1, 1);

        lMeasSource = new QLabel(layoutWidget2);
        lMeasSource->setObjectName(QString::fromUtf8("lMeasSource"));
        lMeasSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lMeasSource, 1, 0, 1, 1);

        cBoxMeasSource = new QComboBox(layoutWidget2);
        cBoxMeasSource->setObjectName(QString::fromUtf8("cBoxMeasSource"));

        gridLayoutsdataLinks->addWidget(cBoxMeasSource, 1, 1, 1, 1);

        lPosSource = new QLabel(layoutWidget2);
        lPosSource->setObjectName(QString::fromUtf8("lPosSource"));
        lPosSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayoutsdataLinks->addWidget(lPosSource, 2, 0, 1, 1);

        cBoxPosSource = new QComboBox(layoutWidget2);
        cBoxPosSource->setObjectName(QString::fromUtf8("cBoxPosSource"));

        gridLayoutsdataLinks->addWidget(cBoxPosSource, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(LoadMeasurements);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(340, 370, 151, 27));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Close);
        listWidgetFiles = new QListWidget(LoadMeasurements);
        listWidgetFiles->setObjectName(QString::fromUtf8("listWidgetFiles"));
        listWidgetFiles->setGeometry(QRect(10, 270, 311, 131));

        retranslateUi(LoadMeasurements);

        QMetaObject::connectSlotsByName(LoadMeasurements);
    } // setupUi

    void retranslateUi(QDialog *LoadMeasurements)
    {
        LoadMeasurements->setWindowTitle(QApplication::translate("LoadMeasurements", "Load Measurements", 0, QApplication::UnicodeUTF8));
        lFileFormat->setText(QApplication::translate("LoadMeasurements", "File Format to import:", 0, QApplication::UnicodeUTF8));
        cBoxFileType->clear();
        cBoxFileType->insertItems(0, QStringList()
         << QApplication::translate("LoadMeasurements", "CW *.csv with Timestamp, Lat, Lon, and RSSI ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoadMeasurements", "CW space delimited Lat, Lon and RxLev", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoadMeasurements", "CW *.val file", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("LoadMeasurements", "GSM Drivetest", 0, QApplication::UnicodeUTF8)
        );
        lTxCell->setText(QApplication::translate("LoadMeasurements", "Transmitting Cell:", 0, QApplication::UnicodeUTF8));
        lFiles->setText(QApplication::translate("LoadMeasurements", "File(s) to load: ", 0, QApplication::UnicodeUTF8));
        pButtonBrowse->setText(QApplication::translate("LoadMeasurements", "Browse", 0, QApplication::UnicodeUTF8));
        pButtonLoad->setText(QApplication::translate("LoadMeasurements", "Load Measurements", 0, QApplication::UnicodeUTF8));
        lMeasType->setText(QApplication::translate("LoadMeasurements", "Measurement Type:", 0, QApplication::UnicodeUTF8));
        lMeasSource->setText(QApplication::translate("LoadMeasurements", "Measurement Source:", 0, QApplication::UnicodeUTF8));
        lPosSource->setText(QApplication::translate("LoadMeasurements", "Position Source:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LoadMeasurements: public Ui_LoadMeasurements {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADMEASUREMENTS_H
