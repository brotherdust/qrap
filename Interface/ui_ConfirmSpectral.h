/********************************************************************************
** Form generated from reading ui file 'ConfirmSpectral.ui'
**
** Created: Thu Apr 15 21:24:32 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIRMSPECTRAL_H
#define UI_CONFIRMSPECTRAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmSpectral
{
public:
    QWidget *gridLayout;
    QGridLayout *gridLayout1;
    QLabel *label;
    QComboBox *comboRx;
    QLabel *label_2;
    QDoubleSpinBox *doubleFreqRes;
    QWidget *gridLayout_2;
    QGridLayout *gridLayout2;
    QPushButton *pushCancel;
    QPushButton *pushOk;
    QTableWidget *tableInstallations;

    void setupUi(QDialog *ConfirmSpectral)
    {
        if (ConfirmSpectral->objectName().isEmpty())
            ConfirmSpectral->setObjectName(QString::fromUtf8("ConfirmSpectral"));
        ConfirmSpectral->resize(921, 350);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QRap/Spectral.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmSpectral->setWindowIcon(icon);
        gridLayout = new QWidget(ConfirmSpectral);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setGeometry(QRect(20, 270, 866, 31));
        gridLayout1 = new QGridLayout(gridLayout);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayout);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout1->addWidget(label, 0, 0, 1, 1);

        comboRx = new QComboBox(gridLayout);
        comboRx->setObjectName(QString::fromUtf8("comboRx"));

        gridLayout1->addWidget(comboRx, 0, 1, 1, 1);

        label_2 = new QLabel(gridLayout);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout1->addWidget(label_2, 0, 2, 1, 1);

        doubleFreqRes = new QDoubleSpinBox(gridLayout);
        doubleFreqRes->setObjectName(QString::fromUtf8("doubleFreqRes"));

        gridLayout1->addWidget(doubleFreqRes, 0, 3, 1, 1);

        gridLayout_2 = new QWidget(ConfirmSpectral);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(710, 310, 178, 31));
        gridLayout2 = new QGridLayout(gridLayout_2);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        gridLayout2->setContentsMargins(0, 0, 0, 0);
        pushCancel = new QPushButton(gridLayout_2);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        gridLayout2->addWidget(pushCancel, 0, 1, 1, 1);

        pushOk = new QPushButton(gridLayout_2);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));

        gridLayout2->addWidget(pushOk, 0, 0, 1, 1);

        tableInstallations = new QTableWidget(ConfirmSpectral);
        if (tableInstallations->columnCount() < 9)
            tableInstallations->setColumnCount(9);
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
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(7, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(8, __qtablewidgetitem8);
        tableInstallations->setObjectName(QString::fromUtf8("tableInstallations"));
        tableInstallations->setGeometry(QRect(5, 10, 911, 251));

        retranslateUi(ConfirmSpectral);

        QMetaObject::connectSlotsByName(ConfirmSpectral);
    } // setupUi

    void retranslateUi(QDialog *ConfirmSpectral)
    {
        ConfirmSpectral->setWindowTitle(QApplication::translate("ConfirmSpectral", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfirmSpectral", "Select Reciever Station :", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConfirmSpectral", "Frequency Resolution (kHz) : ", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("ConfirmSpectral", "Cancel", 0, QApplication::UnicodeUTF8));
        pushOk->setText(QApplication::translate("ConfirmSpectral", "OK", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableInstallations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfirmSpectral", "Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableInstallations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfirmSpectral", "Sector", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableInstallations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ConfirmSpectral", "Radio Inst ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableInstallations->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ConfirmSpectral", "New Column", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableInstallations->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("ConfirmSpectral", "Project", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableInstallations->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("ConfirmSpectral", "Technology", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableInstallations->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("ConfirmSpectral", "FlagZ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableInstallations->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("ConfirmSpectral", "Status", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableInstallations->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("ConfirmSpectral", "Radius", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ConfirmSpectral);
    } // retranslateUi

};

namespace Ui {
    class ConfirmSpectral: public Ui_ConfirmSpectral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMSPECTRAL_H
