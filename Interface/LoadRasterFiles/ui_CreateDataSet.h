/********************************************************************************
** Form generated from reading ui file 'CreateDataSet.ui'
**
** Created: Tue Jul 8 10:48:46 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CREATEDATASET_H
#define UI_CREATEDATASET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

class Ui_CreateDataSet
{
public:
    QGroupBox *groupBox;
    QWidget *gridLayout;
    QGridLayout *gridLayout1;
    QLabel *label;
    QLineEdit *lineEditDescription;
    QLabel *label_2;
    QComboBox *comboBoxFileType5;
    QLabel *label_3;
    QSpinBox *spinBoxResolution;
    QCheckBox *checkBoxDerivedBinary;
    QLabel *label_4;
    QComboBox *comboBoxFileFormat5;
    QLabel *label_5;
    QComboBox *comboBoxProjection5;
    QPushButton *pushButtonCreate;
    QGroupBox *groupBox_2;
    QTableWidget *tableWidgetFileSets5;

    void setupUi(QWidget *CreateDataSet)
    {
    if (CreateDataSet->objectName().isEmpty())
        CreateDataSet->setObjectName(QString::fromUtf8("CreateDataSet"));
    CreateDataSet->resize(646, 301);
    groupBox = new QGroupBox(CreateDataSet);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(10, 0, 621, 121));
    gridLayout = new QWidget(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setGeometry(QRect(10, 20, 601, 95));
    gridLayout1 = new QGridLayout(gridLayout);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    gridLayout1->setContentsMargins(0, 0, 0, 0);
    label = new QLabel(gridLayout);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout1->addWidget(label, 0, 0, 1, 1);

    lineEditDescription = new QLineEdit(gridLayout);
    lineEditDescription->setObjectName(QString::fromUtf8("lineEditDescription"));

    gridLayout1->addWidget(lineEditDescription, 0, 1, 1, 5);

    label_2 = new QLabel(gridLayout);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    gridLayout1->addWidget(label_2, 1, 0, 1, 1);

    comboBoxFileType5 = new QComboBox(gridLayout);
    comboBoxFileType5->setObjectName(QString::fromUtf8("comboBoxFileType5"));

    gridLayout1->addWidget(comboBoxFileType5, 1, 1, 1, 1);

    label_3 = new QLabel(gridLayout);
    label_3->setObjectName(QString::fromUtf8("label_3"));

    gridLayout1->addWidget(label_3, 1, 2, 1, 1);

    spinBoxResolution = new QSpinBox(gridLayout);
    spinBoxResolution->setObjectName(QString::fromUtf8("spinBoxResolution"));
    spinBoxResolution->setMaximum(100000);
    spinBoxResolution->setValue(90);

    gridLayout1->addWidget(spinBoxResolution, 1, 3, 1, 1);

    checkBoxDerivedBinary = new QCheckBox(gridLayout);
    checkBoxDerivedBinary->setObjectName(QString::fromUtf8("checkBoxDerivedBinary"));
    checkBoxDerivedBinary->setChecked(false);

    gridLayout1->addWidget(checkBoxDerivedBinary, 1, 4, 1, 1);

    label_4 = new QLabel(gridLayout);
    label_4->setObjectName(QString::fromUtf8("label_4"));

    gridLayout1->addWidget(label_4, 2, 0, 1, 1);

    comboBoxFileFormat5 = new QComboBox(gridLayout);
    comboBoxFileFormat5->setObjectName(QString::fromUtf8("comboBoxFileFormat5"));

    gridLayout1->addWidget(comboBoxFileFormat5, 2, 1, 1, 1);

    label_5 = new QLabel(gridLayout);
    label_5->setObjectName(QString::fromUtf8("label_5"));

    gridLayout1->addWidget(label_5, 2, 2, 1, 1);

    comboBoxProjection5 = new QComboBox(gridLayout);
    comboBoxProjection5->setObjectName(QString::fromUtf8("comboBoxProjection5"));

    gridLayout1->addWidget(comboBoxProjection5, 2, 3, 1, 1);

    pushButtonCreate = new QPushButton(gridLayout);
    pushButtonCreate->setObjectName(QString::fromUtf8("pushButtonCreate"));

    gridLayout1->addWidget(pushButtonCreate, 2, 5, 1, 1);

    groupBox_2 = new QGroupBox(CreateDataSet);
    groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
    groupBox_2->setGeometry(QRect(10, 130, 621, 161));
    tableWidgetFileSets5 = new QTableWidget(groupBox_2);
    tableWidgetFileSets5->setObjectName(QString::fromUtf8("tableWidgetFileSets5"));
    tableWidgetFileSets5->setGeometry(QRect(10, 20, 601, 131));
    tableWidgetFileSets5->setSelectionMode(QAbstractItemView::SingleSelection);
    tableWidgetFileSets5->setSelectionBehavior(QAbstractItemView::SelectRows);

    retranslateUi(CreateDataSet);

    QMetaObject::connectSlotsByName(CreateDataSet);
    } // setupUi

    void retranslateUi(QWidget *CreateDataSet)
    {
    CreateDataSet->setWindowTitle(QApplication::translate("CreateDataSet", "Form", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("CreateDataSet", "Create a new fileset:", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("CreateDataSet", "Description:", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("CreateDataSet", "File Type:", 0, QApplication::UnicodeUTF8));
    comboBoxFileType5->clear();
    comboBoxFileType5->insertItems(0, QStringList()
     << QApplication::translate("CreateDataSet", "DEM", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "Clutter", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "Other", 0, QApplication::UnicodeUTF8)
    );
    label_3->setText(QApplication::translate("CreateDataSet", "Resolution:", 0, QApplication::UnicodeUTF8));
    checkBoxDerivedBinary->setText(QApplication::translate("CreateDataSet", "Derived Binary", 0, QApplication::UnicodeUTF8));
    label_4->setText(QApplication::translate("CreateDataSet", "File Format", 0, QApplication::UnicodeUTF8));
    comboBoxFileFormat5->clear();
    comboBoxFileFormat5->insertItems(0, QStringList()
     << QApplication::translate("CreateDataSet", "GDALFILE", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "BINFILE", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "ORTFILE", 0, QApplication::UnicodeUTF8)
    );
    label_5->setText(QApplication::translate("CreateDataSet", "Projection:", 0, QApplication::UnicodeUTF8));
    comboBoxProjection5->clear();
    comboBoxProjection5->insertItems(0, QStringList()
     << QApplication::translate("CreateDataSet", "DEG", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "WGS84GC", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("CreateDataSet", "UTM", 0, QApplication::UnicodeUTF8)
    );
    pushButtonCreate->setText(QApplication::translate("CreateDataSet", "Create", 0, QApplication::UnicodeUTF8));
    groupBox_2->setTitle(QApplication::translate("CreateDataSet", "Link to an exsisting fileset:", 0, QApplication::UnicodeUTF8));
    if (tableWidgetFileSets5->columnCount() < 7)
        tableWidgetFileSets5->setColumnCount(7);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("CreateDataSet", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("CreateDataSet", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("CreateDataSet", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(2, __colItem2);

    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    __colItem3->setText(QApplication::translate("CreateDataSet", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(3, __colItem3);

    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    __colItem4->setText(QApplication::translate("CreateDataSet", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(4, __colItem4);

    QTableWidgetItem *__colItem5 = new QTableWidgetItem();
    __colItem5->setText(QApplication::translate("CreateDataSet", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(5, __colItem5);

    QTableWidgetItem *__colItem6 = new QTableWidgetItem();
    __colItem6->setText(QApplication::translate("CreateDataSet", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets5->setHorizontalHeaderItem(6, __colItem6);
    Q_UNUSED(CreateDataSet);
    } // retranslateUi

};

namespace Ui {
    class CreateDataSet: public Ui_CreateDataSet {};
} // namespace Ui

#endif // UI_CREATEDATASET_H
