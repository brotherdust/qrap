/********************************************************************************
** Form generated from reading ui file 'ConfirmLink.ui'
**
** Created: Wed Apr 21 12:08:39 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIRMLINK_H
#define UI_CONFIRMLINK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmLink
{
public:
    QWidget *horizontalLayout;
    QHBoxLayout *Out;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    QLineEdit *nameEdit;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QDoubleSpinBox *plotResolutionSpinBox;
    QDoubleSpinBox *kFactorIntSpinBox;
    QLabel *label_3;
    QComboBox *displayUnitsCombo;
    QLabel *label_5;
    QDoubleSpinBox *frequencySpinBox;
    QLabel *label_6;
    QLabel *label_21;
    QWidget *layoutWidget;
    QHBoxLayout *UpDown;
    QCheckBox *useClutterCheckBox;
    QRadioButton *downLinkRadio;
    QRadioButton *upLinkRadio;
    QTableWidget *TxIDTable;
    QTableWidget *RxIDTable;

    void setupUi(QDialog *ConfirmLink)
    {
        if (ConfirmLink->objectName().isEmpty())
            ConfirmLink->setObjectName(QString::fromUtf8("ConfirmLink"));
        ConfirmLink->resize(579, 390);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmLink->setWindowIcon(icon);
        horizontalLayout = new QWidget(ConfirmLink);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setGeometry(QRect(380, 350, 181, 31));
        Out = new QHBoxLayout(horizontalLayout);
        Out->setObjectName(QString::fromUtf8("Out"));
        Out->setContentsMargins(0, 0, 0, 0);
        btnOk = new QPushButton(horizontalLayout);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        Out->addWidget(btnOk);

        btnCancel = new QPushButton(horizontalLayout);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        Out->addWidget(btnCancel);

        nameEdit = new QLineEdit(ConfirmLink);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setGeometry(QRect(375, 145, 196, 31));
        label = new QLabel(ConfirmLink);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(15, 15, 203, 18));
        label_4 = new QLabel(ConfirmLink);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(380, 125, 81, 20));
        label_2 = new QLabel(ConfirmLink);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 185, 185, 18));
        plotResolutionSpinBox = new QDoubleSpinBox(ConfirmLink);
        plotResolutionSpinBox->setObjectName(QString::fromUtf8("plotResolutionSpinBox"));
        plotResolutionSpinBox->setGeometry(QRect(480, 305, 91, 26));
        plotResolutionSpinBox->setMaximum(3000);
        kFactorIntSpinBox = new QDoubleSpinBox(ConfirmLink);
        kFactorIntSpinBox->setObjectName(QString::fromUtf8("kFactorIntSpinBox"));
        kFactorIntSpinBox->setGeometry(QRect(480, 270, 91, 26));
        kFactorIntSpinBox->setMinimum(-10000);
        kFactorIntSpinBox->setMaximum(3000);
        kFactorIntSpinBox->setValue(1);
        label_3 = new QLabel(ConfirmLink);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(375, 275, 56, 18));
        displayUnitsCombo = new QComboBox(ConfirmLink);
        displayUnitsCombo->setObjectName(QString::fromUtf8("displayUnitsCombo"));
        displayUnitsCombo->setGeometry(QRect(480, 200, 91, 27));
        label_5 = new QLabel(ConfirmLink);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(375, 205, 86, 18));
        frequencySpinBox = new QDoubleSpinBox(ConfirmLink);
        frequencySpinBox->setObjectName(QString::fromUtf8("frequencySpinBox"));
        frequencySpinBox->setGeometry(QRect(480, 235, 91, 26));
        frequencySpinBox->setMaximum(30000);
        frequencySpinBox->setSingleStep(10);
        frequencySpinBox->setValue(2400);
        label_6 = new QLabel(ConfirmLink);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(375, 240, 69, 18));
        label_21 = new QLabel(ConfirmLink);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(375, 310, 101, 18));
        layoutWidget = new QWidget(ConfirmLink);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(24, 354, 292, 25));
        UpDown = new QHBoxLayout(layoutWidget);
        UpDown->setObjectName(QString::fromUtf8("UpDown"));
        UpDown->setContentsMargins(0, 0, 0, 0);
        useClutterCheckBox = new QCheckBox(layoutWidget);
        useClutterCheckBox->setObjectName(QString::fromUtf8("useClutterCheckBox"));
        useClutterCheckBox->setEnabled(false);

        UpDown->addWidget(useClutterCheckBox);

        downLinkRadio = new QRadioButton(layoutWidget);
        downLinkRadio->setObjectName(QString::fromUtf8("downLinkRadio"));
        downLinkRadio->setChecked(true);

        UpDown->addWidget(downLinkRadio);

        upLinkRadio = new QRadioButton(layoutWidget);
        upLinkRadio->setObjectName(QString::fromUtf8("upLinkRadio"));

        UpDown->addWidget(upLinkRadio);

        TxIDTable = new QTableWidget(ConfirmLink);
        if (TxIDTable->columnCount() < 3)
            TxIDTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        TxIDTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        TxIDTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        TxIDTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        TxIDTable->setObjectName(QString::fromUtf8("TxIDTable"));
        TxIDTable->setGeometry(QRect(15, 40, 346, 136));
        RxIDTable = new QTableWidget(ConfirmLink);
        if (RxIDTable->columnCount() < 3)
            RxIDTable->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        RxIDTable->setHorizontalHeaderItem(0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        RxIDTable->setHorizontalHeaderItem(1, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        RxIDTable->setHorizontalHeaderItem(2, __qtablewidgetitem5);
        RxIDTable->setObjectName(QString::fromUtf8("RxIDTable"));
        RxIDTable->setGeometry(QRect(15, 210, 346, 131));
        nameEdit->raise();
        label->raise();
        label_4->raise();
        label_2->raise();
        plotResolutionSpinBox->raise();
        kFactorIntSpinBox->raise();
        label_3->raise();
        displayUnitsCombo->raise();
        label_5->raise();
        frequencySpinBox->raise();
        label_6->raise();
        label_21->raise();
        layoutWidget->raise();
        TxIDTable->raise();
        RxIDTable->raise();
        horizontalLayout->raise();

        retranslateUi(ConfirmLink);

        displayUnitsCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfirmLink);
    } // setupUi

    void retranslateUi(QDialog *ConfirmLink)
    {
        ConfirmLink->setWindowTitle(QApplication::translate("ConfirmLink", "Confirm Link Analysis", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("ConfirmLink", "Ok", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("ConfirmLink", "Cancel", 0, QApplication::UnicodeUTF8));
        nameEdit->setText(QApplication::translate("ConfirmLink", "Link", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfirmLink", "Transmitting Radio Installation:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ConfirmLink", "Link Name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConfirmLink", "Receiving Radio Installation:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ConfirmLink", "k-Factor:", 0, QApplication::UnicodeUTF8));
        displayUnitsCombo->clear();
        displayUnitsCombo->insertItems(0, QStringList()
         << QApplication::translate("ConfirmLink", "dBm", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dBuV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dBW", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dBuVm", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dBWm2Hz", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ConfirmLink", "dBWm2", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("ConfirmLink", "Output Units:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("ConfirmLink", "Frequency:", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("ConfirmLink", "Plot Resolution:", 0, QApplication::UnicodeUTF8));
        useClutterCheckBox->setText(QApplication::translate("ConfirmLink", "Use Clutter ", 0, QApplication::UnicodeUTF8));
        downLinkRadio->setText(QApplication::translate("ConfirmLink", "Down Link", 0, QApplication::UnicodeUTF8));
        upLinkRadio->setText(QApplication::translate("ConfirmLink", "Up Link", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = TxIDTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfirmLink", "Tx Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = TxIDTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfirmLink", "Technology", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = TxIDTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ConfirmLink", "Rad Inst", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = RxIDTable->horizontalHeaderItem(0);
        ___qtablewidgetitem3->setText(QApplication::translate("ConfirmLink", "Rx Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = RxIDTable->horizontalHeaderItem(1);
        ___qtablewidgetitem4->setText(QApplication::translate("ConfirmLink", "Technology", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = RxIDTable->horizontalHeaderItem(2);
        ___qtablewidgetitem5->setText(QApplication::translate("ConfirmLink", "Rad Inst", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ConfirmLink);
    } // retranslateUi

};

namespace Ui {
    class ConfirmLink: public Ui_ConfirmLink {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMLINK_H
