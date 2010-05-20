/********************************************************************************
** Form generated from reading ui file 'CutSet.ui'
**
** Created: Tue Jul 22 23:54:41 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CUTSET_H
#define UI_CUTSET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_CutSet
{
public:
    QGroupBox *groupBox;
    QTableWidget *tableWidgetFileSets1;
    QWidget *horizontalLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label;
    QComboBox *comboBoxFileType1;
    QLabel *label_2;
    QComboBox *comboBoxFileFormat1;
    QLabel *label_3;
    QComboBox *comboBoxProjection1;
    QGroupBox *groupBox_2;
    QTableWidget *tableWidgetFileSets2;
    QWidget *horizontalLayout_2;
    QHBoxLayout *hboxLayout1;
    QLabel *label_4;
    QLineEdit *lineEditDirectory;
    QPushButton *pushButtonBrowse1;
    QPushButton *pushButtonCut1;

    void setupUi(QWidget *CutSet)
    {
    if (CutSet->objectName().isEmpty())
        CutSet->setObjectName(QString::fromUtf8("CutSet"));
    CutSet->resize(696, 343);
    groupBox = new QGroupBox(CutSet);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 0, 681, 171));
    tableWidgetFileSets1 = new QTableWidget(groupBox);
    tableWidgetFileSets1->setObjectName(QString::fromUtf8("tableWidgetFileSets1"));
    tableWidgetFileSets1->setGeometry(QRect(10, 50, 661, 116));
    tableWidgetFileSets1->setAutoFillBackground(true);
    tableWidgetFileSets1->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetFileSets1->setWordWrap(false);
    horizontalLayout = new QWidget(groupBox);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setGeometry(QRect(10, 20, 656, 29));
    hboxLayout = new QHBoxLayout(horizontalLayout);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(horizontalLayout);
    label->setObjectName(QString::fromUtf8("label"));

    hboxLayout->addWidget(label);

    comboBoxFileType1 = new QComboBox(horizontalLayout);
    comboBoxFileType1->setObjectName(QString::fromUtf8("comboBoxFileType1"));

    hboxLayout->addWidget(comboBoxFileType1);

    label_2 = new QLabel(horizontalLayout);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    hboxLayout->addWidget(label_2);

    comboBoxFileFormat1 = new QComboBox(horizontalLayout);
    comboBoxFileFormat1->setObjectName(QString::fromUtf8("comboBoxFileFormat1"));

    hboxLayout->addWidget(comboBoxFileFormat1);

    label_3 = new QLabel(horizontalLayout);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    hboxLayout->addWidget(label_3);

    comboBoxProjection1 = new QComboBox(horizontalLayout);
    comboBoxProjection1->setObjectName(QString::fromUtf8("comboBoxProjection1"));

    hboxLayout->addWidget(comboBoxProjection1);

    groupBox_2 = new QGroupBox(CutSet);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 170, 681, 136));
    tableWidgetFileSets2 = new QTableWidget(groupBox_2);
    tableWidgetFileSets2->setObjectName(QString::fromUtf8("tableWidgetFileSets2"));
    tableWidgetFileSets2->setGeometry(QRect(10, 20, 661, 111));
    tableWidgetFileSets2->setAutoFillBackground(true);
    tableWidgetFileSets2->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetFileSets2->setWordWrap(false);
    horizontalLayout_2 = new QWidget(CutSet);
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    horizontalLayout_2->setGeometry(QRect(20, 310, 661, 30));
    hboxLayout1 = new QHBoxLayout(horizontalLayout_2);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    label_4 = new QLabel(horizontalLayout_2);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    hboxLayout1->addWidget(label_4);

    lineEditDirectory = new QLineEdit(horizontalLayout_2);
    lineEditDirectory->setObjectName(QString::fromUtf8("lineEditDirectory"));

    hboxLayout1->addWidget(lineEditDirectory);

    pushButtonBrowse1 = new QPushButton(horizontalLayout_2);
    pushButtonBrowse1->setObjectName(QString::fromUtf8("pushButtonBrowse1"));

    hboxLayout1->addWidget(pushButtonBrowse1);

    pushButtonCut1 = new QPushButton(horizontalLayout_2);
    pushButtonCut1->setObjectName(QString::fromUtf8("pushButtonCut1"));

    hboxLayout1->addWidget(pushButtonCut1);


    retranslateUi(CutSet);

    comboBoxFileFormat1->setCurrentIndex(0);
    comboBoxProjection1->setCurrentIndex(0);


    QMetaObject::connectSlotsByName(CutSet);
    } // setupUi

    void retranslateUi(QWidget *CutSet)
    {
    CutSet->setWindowTitle(QApplication::translate("CutSet", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CutSet", "Fileset to be cut", 0, QApplication::UnicodeUTF8));
    if (tableWidgetFileSets1->columnCount() < 8)
        tableWidgetFileSets1->setColumnCount(8);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("CutSet", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("CutSet", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("CutSet", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(2, __colItem2);

    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    __colItem3->setText(QApplication::translate("CutSet", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(3, __colItem3);

    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    __colItem4->setText(QApplication::translate("CutSet", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(4, __colItem4);

    QTableWidgetItem *__colItem5 = new QTableWidgetItem();
    __colItem5->setText(QApplication::translate("CutSet", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(5, __colItem5);

    QTableWidgetItem *__colItem6 = new QTableWidgetItem();
    __colItem6->setText(QApplication::translate("CutSet", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(6, __colItem6);

    QTableWidgetItem *__colItem7 = new QTableWidgetItem();
    __colItem7->setText(QApplication::translate("CutSet", "Classification", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets1->setHorizontalHeaderItem(7, __colItem7);
    label->setText(QApplication::translate("CutSet", "File Type", 0, QApplication::UnicodeUTF8));
    comboBoxFileType1->clear();
    comboBoxFileType1->insertItems(0, QStringList()
     << QApplication::translate("CutSet", "DEM", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "Clutter", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "Other", 0, QApplication::UnicodeUTF8)
    );
    label_2->setText(QApplication::translate("CutSet", "File Format", 0, QApplication::UnicodeUTF8));
    comboBoxFileFormat1->clear();
    comboBoxFileFormat1->insertItems(0, QStringList()
     << QApplication::translate("CutSet", "All", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "GDALFILE", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "ORTFILE", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("CutSet", "Projection:", 0, QApplication::UnicodeUTF8));
    comboBoxProjection1->clear();
    comboBoxProjection1->insertItems(0, QStringList()
     << QApplication::translate("CutSet", "All", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "DEG", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "WGS84GC", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CutSet", "UTM", 0, QApplication::UnicodeUTF8)
    );
    groupBox_2->setTitle(QApplication::translate("CutSet", "Fileset containing cut data", 0, QApplication::UnicodeUTF8));
    if (tableWidgetFileSets2->columnCount() < 8)
        tableWidgetFileSets2->setColumnCount(8);

    QTableWidgetItem *__colItem8 = new QTableWidgetItem();
    __colItem8->setText(QApplication::translate("CutSet", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(0, __colItem8);

    QTableWidgetItem *__colItem9 = new QTableWidgetItem();
    __colItem9->setText(QApplication::translate("CutSet", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(1, __colItem9);

    QTableWidgetItem *__colItem10 = new QTableWidgetItem();
    __colItem10->setText(QApplication::translate("CutSet", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(2, __colItem10);

    QTableWidgetItem *__colItem11 = new QTableWidgetItem();
    __colItem11->setText(QApplication::translate("CutSet", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(3, __colItem11);

    QTableWidgetItem *__colItem12 = new QTableWidgetItem();
    __colItem12->setText(QApplication::translate("CutSet", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(4, __colItem12);

    QTableWidgetItem *__colItem13 = new QTableWidgetItem();
    __colItem13->setText(QApplication::translate("CutSet", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(5, __colItem13);

    QTableWidgetItem *__colItem14 = new QTableWidgetItem();
    __colItem14->setText(QApplication::translate("CutSet", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(6, __colItem14);

    QTableWidgetItem *__colItem15 = new QTableWidgetItem();
    __colItem15->setText(QApplication::translate("CutSet", "Classification", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets2->setHorizontalHeaderItem(7, __colItem15);
    label_4->setText(QApplication::translate("CutSet", "Save cut files in directory:", 0, QApplication::UnicodeUTF8));
    pushButtonBrowse1->setText(QApplication::translate("CutSet", "Browse", 0, QApplication::UnicodeUTF8));
    pushButtonCut1->setText(QApplication::translate("CutSet", "Cut", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(CutSet);
    } // retranslateUi

};

namespace Ui {
    class CutSet: public Ui_CutSet {};
} // namespace Ui

#endif // UI_CUTSET_H
