/********************************************************************************
** Form generated from reading UI file 'LoadFiles.ui'
**
** Created: Wed Apr 18 12:44:27 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOADFILES_H
#define UI_LOADFILES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

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
    QPushButton *pushButtonBrowse;
    QSpacerItem *spacerItem;
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
    QLineEdit *labelDirectory;

    void setupUi(QWidget *LoadFiles)
    {
        if (LoadFiles->objectName().isEmpty())
            LoadFiles->setObjectName(QString::fromUtf8("LoadFiles"));
        LoadFiles->resize(722, 366);
        tableWidgetFileSets = new QTableWidget(LoadFiles);
        if (tableWidgetFileSets->columnCount() < 8)
            tableWidgetFileSets->setColumnCount(8);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableWidgetFileSets->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        tableWidgetFileSets->setObjectName(QString::fromUtf8("tableWidgetFileSets"));
        tableWidgetFileSets->setGeometry(QRect(10, 40, 701, 131));
        tableWidgetFileSets->setAutoFillBackground(true);
        tableWidgetFileSets->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidgetFileSets->setWordWrap(false);
        listWidgetFiles = new QListWidget(LoadFiles);
        listWidgetFiles->setObjectName(QString::fromUtf8("listWidgetFiles"));
        listWidgetFiles->setGeometry(QRect(510, 200, 181, 121));
        horizontalLayout = new QWidget(LoadFiles);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setGeometry(QRect(9, 10, 701, 29));
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
        label_6->setGeometry(QRect(510, 180, 161, 18));
        pushButtonImport = new QPushButton(LoadFiles);
        pushButtonImport->setObjectName(QString::fromUtf8("pushButtonImport"));
        pushButtonImport->setEnabled(false);
        pushButtonImport->setGeometry(QRect(580, 330, 111, 27));
        gridLayout = new QWidget(LoadFiles);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setGeometry(QRect(10, 180, 441, 181));
        gridLayout1 = new QGridLayout(gridLayout);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(gridLayout);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout1->addWidget(label_4, 0, 0, 1, 1);

        pushButtonBrowse = new QPushButton(gridLayout);
        pushButtonBrowse->setObjectName(QString::fromUtf8("pushButtonBrowse"));

        gridLayout1->addWidget(pushButtonBrowse, 0, 2, 1, 2);

        spacerItem = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacerItem, 1, 3, 1, 1);

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

        labelDirectory = new QLineEdit(gridLayout);
        labelDirectory->setObjectName(QString::fromUtf8("labelDirectory"));
        labelDirectory->setReadOnly(true);

        gridLayout1->addWidget(labelDirectory, 0, 1, 1, 1);

        tableWidgetFileSets->raise();
        listWidgetFiles->raise();
        horizontalLayout->raise();
        label_6->raise();
        pushButtonImport->raise();
        gridLayout->raise();
        labelDirectory->raise();

        retranslateUi(LoadFiles);

        comboBoxFileFormat->setCurrentIndex(1);
        comboBoxProjection->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LoadFiles);
    } // setupUi

    void retranslateUi(QWidget *LoadFiles)
    {
        LoadFiles->setWindowTitle(QApplication::translate("LoadFiles", "Form", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetFileSets->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("LoadFiles", "ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetFileSets->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("LoadFiles", "Description", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetFileSets->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("LoadFiles", "Resolution", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidgetFileSets->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("LoadFiles", "File Format", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidgetFileSets->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("LoadFiles", "Use", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidgetFileSets->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("LoadFiles", "Cutted Set", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidgetFileSets->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("LoadFiles", "Projection", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidgetFileSets->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("LoadFiles", "Classification", 0, QApplication::UnicodeUTF8));
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
        pushButtonBrowse->setText(QApplication::translate("LoadFiles", "Browse", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("LoadFiles", "GroupBox", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("LoadFiles", "Binary Directory:", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("LoadFiles", "File:", 0, QApplication::UnicodeUTF8));
        lineEditORTFile->setText(QApplication::translate("LoadFiles", "FileName", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("LoadFiles", "Central Meridian:", 0, QApplication::UnicodeUTF8));
        pushButtonORTBrowse->setText(QApplication::translate("LoadFiles", "Browse", 0, QApplication::UnicodeUTF8));
        labelDirectory->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoadFiles: public Ui_LoadFiles {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOADFILES_H
