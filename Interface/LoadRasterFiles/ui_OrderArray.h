/********************************************************************************
** Form generated from reading ui file 'OrderArray.ui'
**
** Created: Tue Jul 22 23:53:57 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ORDERARRAY_H
#define UI_ORDERARRAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

class Ui_OrderArray
{
public:
    QComboBox *comboBoxFileType4;
    QLabel *label;
    QPushButton *pushButtonAdd;
    QPushButton *pushButtonRemove;
    QPushButton *pushButtonUp;
    QPushButton *pushButtonDown;
    QTableWidget *tableWidgetFileSets4;
    QTableWidget *tableWidgetFileSets3;

    void setupUi(QWidget *OrderArray)
    {
    if (OrderArray->objectName().isEmpty())
        OrderArray->setObjectName(QString::fromUtf8("OrderArray"));
    OrderArray->resize(768, 375);
    comboBoxFileType4 = new QComboBox(OrderArray);
    comboBoxFileType4->setObjectName(QString::fromUtf8("comboBoxFileType4"));
    comboBoxFileType4->setGeometry(QRect(70, 10, 121, 27));
    label = new QLabel(OrderArray);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(15, 15, 54, 18));
    pushButtonAdd = new QPushButton(OrderArray);
    pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
     pushButtonAdd->setIcon(QIcon(QString::fromUtf8(":/images/down.png")));
    pushButtonAdd->setGeometry(QRect(255, 185, 80, 28));
    pushButtonRemove = new QPushButton(OrderArray);
    pushButtonRemove->setObjectName(QString::fromUtf8("pushButtonRemove"));
	pushButtonRemove->setIcon(QIcon(QString::fromUtf8(":/images/up.png")));
    pushButtonRemove->setGeometry(QRect(340, 185, 80, 28));
    pushButtonUp = new QPushButton(OrderArray);
    pushButtonUp->setObjectName(QString::fromUtf8("pushButtonUp"));
    pushButtonUp->setGeometry(QRect(680, 245, 80, 32));
    pushButtonUp->setIcon(QIcon(QString::fromUtf8(":/images/add.png")));
    pushButtonDown = new QPushButton(OrderArray);
    pushButtonDown->setObjectName(QString::fromUtf8("pushButtonDown"));
    pushButtonDown->setGeometry(QRect(680, 325, 80, 32));
    pushButtonDown->setIcon(QIcon(QString::fromUtf8(":/images/remove.png")));
    tableWidgetFileSets4 = new QTableWidget(OrderArray);
    tableWidgetFileSets4->setObjectName(QString::fromUtf8("tableWidgetFileSets4"));
    tableWidgetFileSets4->setGeometry(QRect(0, 220, 671, 151));
    tableWidgetFileSets4->setAutoFillBackground(true);
    tableWidgetFileSets4->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetFileSets4->setWordWrap(false);
    tableWidgetFileSets3 = new QTableWidget(OrderArray);
    tableWidgetFileSets3->setObjectName(QString::fromUtf8("tableWidgetFileSets3"));
    tableWidgetFileSets3->setGeometry(QRect(0, 45, 671, 136));
    tableWidgetFileSets3->setAutoFillBackground(true);
    tableWidgetFileSets3->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidgetFileSets3->setWordWrap(false);

    retranslateUi(OrderArray);

    QMetaObject::connectSlotsByName(OrderArray);
    } // setupUi

    void retranslateUi(QWidget *OrderArray)
    {
    OrderArray->setWindowTitle(QApplication::translate("OrderArray", "Form", 0, QApplication::UnicodeUTF8));
    comboBoxFileType4->clear();
    comboBoxFileType4->insertItems(0, QStringList()
     << QApplication::translate("OrderArray", "DEM", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("OrderArray", "Clutter", 0, QApplication::UnicodeUTF8)
    );
    label->setText(QApplication::translate("OrderArray", "Filetype:", 0, QApplication::UnicodeUTF8));
    pushButtonAdd->setText(QApplication::translate("OrderArray", "Add ", 0, QApplication::UnicodeUTF8));
    pushButtonRemove->setText(QApplication::translate("OrderArray", "Remove", 0, QApplication::UnicodeUTF8));
    pushButtonUp->setText(QApplication::translate("OrderArray", "Up", 0, QApplication::UnicodeUTF8));
    pushButtonDown->setText(QApplication::translate("OrderArray", "Down", 0, QApplication::UnicodeUTF8));
    if (tableWidgetFileSets4->columnCount() < 8)
        tableWidgetFileSets4->setColumnCount(8);

    QTableWidgetItem *__colItem = new QTableWidgetItem();
    __colItem->setText(QApplication::translate("OrderArray", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(0, __colItem);

    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    __colItem1->setText(QApplication::translate("OrderArray", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(1, __colItem1);

    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    __colItem2->setText(QApplication::translate("OrderArray", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(2, __colItem2);

    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    __colItem3->setText(QApplication::translate("OrderArray", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(3, __colItem3);

    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    __colItem4->setText(QApplication::translate("OrderArray", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(4, __colItem4);

    QTableWidgetItem *__colItem5 = new QTableWidgetItem();
    __colItem5->setText(QApplication::translate("OrderArray", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(5, __colItem5);

    QTableWidgetItem *__colItem6 = new QTableWidgetItem();
    __colItem6->setText(QApplication::translate("OrderArray", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(6, __colItem6);

    QTableWidgetItem *__colItem7 = new QTableWidgetItem();
    __colItem7->setText(QApplication::translate("OrderArray", "Classification", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets4->setHorizontalHeaderItem(7, __colItem7);
    if (tableWidgetFileSets3->columnCount() < 8)
        tableWidgetFileSets3->setColumnCount(8);

    QTableWidgetItem *__colItem8 = new QTableWidgetItem();
    __colItem8->setText(QApplication::translate("OrderArray", "ID", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(0, __colItem8);

    QTableWidgetItem *__colItem9 = new QTableWidgetItem();
    __colItem9->setText(QApplication::translate("OrderArray", "Description", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(1, __colItem9);

    QTableWidgetItem *__colItem10 = new QTableWidgetItem();
    __colItem10->setText(QApplication::translate("OrderArray", "Projection", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(2, __colItem10);

    QTableWidgetItem *__colItem11 = new QTableWidgetItem();
    __colItem11->setText(QApplication::translate("OrderArray", "Resolution", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(3, __colItem11);

    QTableWidgetItem *__colItem12 = new QTableWidgetItem();
    __colItem12->setText(QApplication::translate("OrderArray", "File Format", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(4, __colItem12);

    QTableWidgetItem *__colItem13 = new QTableWidgetItem();
    __colItem13->setText(QApplication::translate("OrderArray", "Use", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(5, __colItem13);

    QTableWidgetItem *__colItem14 = new QTableWidgetItem();
    __colItem14->setText(QApplication::translate("OrderArray", "Cutted Set", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(6, __colItem14);

    QTableWidgetItem *__colItem15 = new QTableWidgetItem();
    __colItem15->setText(QApplication::translate("OrderArray", "Classification", 0, QApplication::UnicodeUTF8));
    tableWidgetFileSets3->setHorizontalHeaderItem(7, __colItem15);
    Q_UNUSED(OrderArray);
    } // retranslateUi

};

namespace Ui {
    class OrderArray: public Ui_OrderArray {};
} // namespace Ui

#endif // UI_ORDERARRAY_H
