/********************************************************************************
** Form generated from reading ui file 'LoadFiles.ui'
**
** Created: Tue Jul 8 16:41:21 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOADFILES_H
#define UI_LOADFILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_LoadFiles
{
public:
    QTableWidget *tableWidgetFileSets;
    QListWidget *listWidgetFiles;
    QWidget *horizontalLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QComboBox *comboBoxFileType;
    QLabel *label_2;
    QComboBox *comboBoxFileFormat;
    QLabel *label_3;
    QComboBox *comboBoxProjection;
    QLabel *label_6;
    QPushButton *pushButtonImport;
    QWidget *gridLayout;
    QGridLayout *gridLayout1;
    QLabel *label_4;
    QLineEdit *labelDirectory;
    QPushButton *pushButtonBrowse;
    QWidget *widgetORT;
    QGroupBox *groupBox;
    QWidget *gridLayout_2;
    QGridLayout *gridLayout2;
    QLabel *label_5;
    QLabel *label_7;
    QLineEdit *lineEditORTDirectory;
    QLineEdit *lineEditORTFile;
    QLabel *label_8;
    QSpinBox *spinBoxORTCentralMeridian;
    QPushButton *pushButtonORTBrowse;
    QSpacerItem *spacerItem;

    void setupUi(QWidget *LoadFiles)
    {
    if (LoadFiles->objectName().isEmpty())
        LoadFiles->setObjectName(QString::fromUtf8("LoadFiles"));
    LoadFiles->resize(645, 366);
    tableWidgetFileSets = new QTableWidget(LoadFiles);
    tableWidgetFileSets->setObjectName(QString::fromUtf8("tableWidgetFileSets"));
    tableWidgetFileSets->setGeometry(QRect(10, 40, 631, 131));
    tableWidgetFileSets->setAutoFillBackground(true);
    tableWidgetFileSets->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetFileSets->setWordWrap(false);
    listWidgetFiles = new QListWidget(LoadFiles);
    listWidgetFiles->setObjectName(QString::fromUtf8("listWidgetFiles"));
    listWidgetFiles->setGeometry(QRect(10, 200, 181, 121));
    horizontalLayout = new QWidget(LoadFiles);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setGeometry(QRect(9, 10, 631, 28));
    hboxLayout = new QHBoxLayout(horizontalLayout);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(horizontalLayout);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    comboBoxFileType = new QComboBox(horizontalLayout);
    comboBoxFileType->setObjectName(QString::fromUtf8("comboBoxFileType"));

    hboxLayout->addWidget(comboBoxFileType);

    label_2 = new QLabel(horizontalLayout);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout->addWidget(label_2);

    comboBoxFileFormat = new QComboBox(horizontalLayout);
    comboBoxFileFormat->setObjectName(QString::fromUtf8("comboBoxFileFormat"));

    hboxLayout->addWidget(comboBoxFileFormat);

    label_3 = new QLabel(horizontalLayout);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    hboxLayout->addWidget(label_3);

    comboBoxProjection = new QComboBox(horizontalLayout);
    comboBoxProjection->setObjectName(QString::fromUtf8("comboBoxProjection"));

    hboxLayout->addWidget(comboBoxProjection);

    label_6 = new QLabel(LoadFiles);
    label_6->setObjectName(QString::fromUtf8("label_6"));
    label_6->setGeometry(QRect(10, 180, 161, 18));
    pushButtonImport = new QPushButton(LoadFiles);
    pushButtonImport->setObjectName(QString::fromUtf8("pushButtonImport"));
    pushButtonImport->setEnabled(false);
    pushButtonImport->setGeometry(QRect(80, 330, 111, 27));
    gridLayout = new QWidget(LoadFiles);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setGeometry(QRect(200, 180, 441, 181));
    gridLayout1 = new QGridLayout(gridLayout);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    label_4 = new QLabel(gridLayout);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout1->addWidget(label_4, 0, 0, 1, 1);

    labelDirectory = new QLineEdit(gridLayout);
    labelDirectory->setObjectName(QString::fromUtf8("labelDirectory"));
    labelDirectory->setReadOnly(true);

    gridLayout1->addWidget(labelDirectory, 0, 1, 1, 1);

    pushButtonBrowse = new QPushButton(gridLayout);
    pushButtonBrowse->setObjectName(QString::fromUtf8("pushButtonBrowse"));

    gridLayout1->addWidget(pushButtonBrowse, 0, 2, 1, 2);

    widgetORT = new QWidget(gridLayout);
    widgetORT->setObjectName(QString::fromUtf8("widgetORT"));
    groupBox = new QGroupBox(widgetORT);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 0, 381, 91));
    gridLayout_2 = new QWidget(groupBox);
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    gridLayout_2->setGeometry(QRect(10, 20, 361, 62));
    gridLayout2 = new QGridLayout(gridLayout_2);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    gridLayout2->setContentsMargins(0, 0, 0, 0);
    label_5 = new QLabel(gridLayout_2);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout2->addWidget(label_5, 0, 0, 1, 2);

    label_7 = new QLabel(gridLayout_2);
    label_7->setObjectName(QString::fromUtf8("label_7"));

    gridLayout2->addWidget(label_7, 1, 0, 1, 1);

    lineEditORTDirectory = new QLineEdit(gridLayout_2);
    lineEditORTDirectory->setObjectName(QString::fromUtf8("lineEditORTDirectory"));

    gridLayout2->addWidget(lineEditORTDirectory, 0, 2, 1, 2);

    lineEditORTFile = new QLineEdit(gridLayout_2);
    lineEditORTFile->setObjectName(QString::fromUtf8("lineEditORTFile"));

    gridLayout2->addWidget(lineEditORTFile, 1, 1, 1, 2);

    label_8 = new QLabel(gridLayout_2);
    label_8->setObjectName(QString::fromUtf8("label_8"));

    gridLayout2->addWidget(label_8, 1, 3, 1, 1);

    spinBoxORTCentralMeridian = new QSpinBox(gridLayout_2);
    spinBoxORTCentralMeridian->setObjectName(QString::fromUtf8("spinBoxORTCentralMeridian"));
    spinBoxORTCentralMeridian->setMinimum(-180);
    spinBoxORTCentralMeridian->setMaximum(180);
    spinBoxORTCentralMeridian->setValue(29);

    gridLayout2->addWidget(spinBoxORTCentralMeridian, 1, 4, 1, 1);

    pushButtonORTBrowse = new QPushButton(gridLayout_2);
    pushButtonORTBrowse->setObjectName(QString::fromUtf8("pushButtonORTBrowse"));

    gridLayout2->addWidget(pushButtonORTBrowse, 0, 4, 1, 1);


    gridLayout1->addWidget(widgetORT, 1, 0, 1, 3);

    spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    gridLayout1->addItem(spacerItem, 1, 3, 1, 1);


    retranslateUi(LoadFiles);

    comboBoxFileFormat->setCurrentIndex(1);
    comboBoxProjection->setCurrentIndex(1);


    QMetaObject::connectSlotsByName(LoadFiles);
    } // setupUi

    void retranslateUi(QWidget *LoadFiles)
    {
    LoadFiles->setWindowTitle(QApplication::translate("LoadFiles", "Form", 0, QApplication::UnicodeUTF8));
    if (tableWidgetFileSets->columnCount() < 8)
        tableWidgetFileSets->setColumnCount(8);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("LoadFiles", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("LoadFiles", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("LoadFiles", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(2, __colItem2);

    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    __colItem3->setText(QApplication::translate("LoadFiles", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(3, __colItem3);

    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    __colItem4->setText(QApplication::translate("LoadFiles", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(4, __colItem4);

    QTableWidgetItem *__colItem5 = new QTableWidgetItem();
    __colItem5->setText(QApplication::translate("LoadFiles", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(5, __colItem5);

    QTableWidgetItem *__colItem6 = new QTableWidgetItem();
    __colItem6->setText(QApplication::translate("LoadFiles", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(6, __colItem6);

    QTableWidgetItem *__colItem7 = new QTableWidgetItem();
    __colItem7->setText(QApplication::translate("LoadFiles", "Classification", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets->setHorizontalHeaderItem(7, __colItem7);
    label->setText(QApplication::translate("LoadFiles", "File Type", 0, QApplication::UnicodeUTF8));
    comboBoxFileType->clear();
    comboBoxFileType->insertItems(0, QStringList()
     << QApplication::translate("LoadFiles", "DEM", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "Clutter", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "Other", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("LoadFiles", "File Format", 0, QApplication::UnicodeUTF8));
    comboBoxFileFormat->clear();
    comboBoxFileFormat->insertItems(0, QStringList()
     << QApplication::translate("LoadFiles", "All", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "GDALFILE", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "BINFILE", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "ORTFILE", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("LoadFiles", "Projection:", 0, QApplication::UnicodeUTF8));
    comboBoxProjection->clear();
    comboBoxProjection->insertItems(0, QStringList()
     << QApplication::translate("LoadFiles", "All", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "DEG", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "WGS84GC", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("LoadFiles", "UTM", 0, QApplication::UnicodeUTF8)
    );
    label_6->setText(QApplication::translate("LoadFiles", "Files to be imported:", 0, QApplication::UnicodeUTF8));
    pushButtonImport->setText(QApplication::translate("LoadFiles", "Import", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("LoadFiles", "Directory:", 0, QApplication::UnicodeUTF8));
    labelDirectory->setText(QString());
    pushButtonBrowse->setText(QApplication::translate("LoadFiles", "Browse", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("LoadFiles", "GroupBox", 0, QApplication::UnicodeUTF8));
    label_5->setText(QApplication::translate("LoadFiles", "Binary Directory:", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("LoadFiles", "File:", 0, QApplication::UnicodeUTF8));
    lineEditORTFile->setText(QApplication::translate("LoadFiles", "FileName", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("LoadFiles", "Central Meridian:", 0, QApplication::UnicodeUTF8));
    pushButtonORTBrowse->setText(QApplication::translate("LoadFiles", "Browse", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(LoadFiles);
    } // retranslateUi

};

namespace Ui {
    class LoadFiles: public Ui_LoadFiles {};
} // namespace Ui

#endif // UI_LOADFILES_H
