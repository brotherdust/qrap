/********************************************************************************
** Form generated from reading UI file 'ConfirmSpectral.ui'
**
** Created: Tue Jun 1 22:15:46 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
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
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmSpectral
{
public:
    QTableWidget *tableInstallations;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *lblRx;
    QComboBox *comboRx;
    QLabel *lblkFactor;
    QDoubleSpinBox *doublekFactor;
    QLabel *lblFreqRes;
    QDoubleSpinBox *doubleFreqRes;
    QGridLayout *gridLayout;
    QPushButton *pushCancel;
    QPushButton *pushOk;

    void setupUi(QDialog *ConfirmSpectral)
    {
        if (ConfirmSpectral->objectName().isEmpty())
            ConfirmSpectral->setObjectName(QString::fromUtf8("ConfirmSpectral"));
        ConfirmSpectral->resize(1009, 371);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QRap/Spectral.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmSpectral->setWindowIcon(icon);
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
        tableInstallations->setGeometry(QRect(5, 10, 991, 311));
        tableInstallations->setMinimumSize(QSize(911, 0));
        layoutWidget = new QWidget(ConfirmSpectral);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 330, 981, 33));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lblRx = new QLabel(layoutWidget);
        lblRx->setObjectName(QString::fromUtf8("lblRx"));
        lblRx->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblRx);

        comboRx = new QComboBox(layoutWidget);
        comboRx->setObjectName(QString::fromUtf8("comboRx"));

        horizontalLayout->addWidget(comboRx);

        lblkFactor = new QLabel(layoutWidget);
        lblkFactor->setObjectName(QString::fromUtf8("lblkFactor"));
        lblkFactor->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblkFactor);

        doublekFactor = new QDoubleSpinBox(layoutWidget);
        doublekFactor->setObjectName(QString::fromUtf8("doublekFactor"));
        doublekFactor->setMinimum(-2);
        doublekFactor->setMaximum(20);
        doublekFactor->setValue(1.33);

        horizontalLayout->addWidget(doublekFactor);

        lblFreqRes = new QLabel(layoutWidget);
        lblFreqRes->setObjectName(QString::fromUtf8("lblFreqRes"));
        lblFreqRes->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(lblFreqRes);

        doubleFreqRes = new QDoubleSpinBox(layoutWidget);
        doubleFreqRes->setObjectName(QString::fromUtf8("doubleFreqRes"));
        doubleFreqRes->setMinimum(0.01);
        doubleFreqRes->setMaximum(1000);
        doubleFreqRes->setValue(10);

        horizontalLayout->addWidget(doubleFreqRes);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushCancel = new QPushButton(layoutWidget);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        gridLayout->addWidget(pushCancel, 0, 1, 1, 1);

        pushOk = new QPushButton(layoutWidget);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));

        gridLayout->addWidget(pushOk, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(ConfirmSpectral);

        QMetaObject::connectSlotsByName(ConfirmSpectral);
    } // setupUi

    void retranslateUi(QDialog *ConfirmSpectral)
    {
        ConfirmSpectral->setWindowTitle(QApplication::translate("ConfirmSpectral", "Spectral Interference Analysis", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableInstallations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfirmSpectral", "Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableInstallations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfirmSpectral", "Sector", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableInstallations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ConfirmSpectral", "Radio Inst ID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem3 = tableInstallations->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("ConfirmSpectral", "Technology", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem4 = tableInstallations->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QApplication::translate("ConfirmSpectral", "Project", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem5 = tableInstallations->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QApplication::translate("ConfirmSpectral", "FlagX", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem6 = tableInstallations->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QApplication::translate("ConfirmSpectral", "FlagZ", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem7 = tableInstallations->horizontalHeaderItem(7);
        ___qtablewidgetitem7->setText(QApplication::translate("ConfirmSpectral", "Status", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem8 = tableInstallations->horizontalHeaderItem(8);
        ___qtablewidgetitem8->setText(QApplication::translate("ConfirmSpectral", "Radius", 0, QApplication::UnicodeUTF8));
        lblRx->setText(QApplication::translate("ConfirmSpectral", "Select Reciever Station :", 0, QApplication::UnicodeUTF8));
        lblkFactor->setText(QApplication::translate("ConfirmSpectral", "k-Factor:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        doublekFactor->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        lblFreqRes->setText(QApplication::translate("ConfirmSpectral", "Frequency Resolution (kHz) : ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        doubleFreqRes->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        pushCancel->setText(QApplication::translate("ConfirmSpectral", "Cancel", 0, QApplication::UnicodeUTF8));
        pushOk->setText(QApplication::translate("ConfirmSpectral", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfirmSpectral: public Ui_ConfirmSpectral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMSPECTRAL_H
