/********************************************************************************
** Form generated from reading ui file 'ConfirmMultiLink.ui'
**
** Created: Tue May 11 09:34:44 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIRMMULTILINK_H
#define UI_CONFIRMMULTILINK_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfirmMultiLink
{
public:
    QWidget *gridLayout_2;
    QGridLayout *gridlayAccept;
    QPushButton *pushOk;
    QPushButton *pushCancel;
    QTableWidget *tableInstallations;
    QLabel *lblTechnology;
    QComboBox *cbMinClear;
    QLabel *lblMaxPath;
    QComboBox *cbTechnology;
    QLabel *lblMinClear;
    QDoubleSpinBox *dsbMaxPath;
    QLabel *lblMaxDist;
    QDoubleSpinBox *dsbMaxDist;
    QLabel *lblProject;
    QComboBox *cbProject;
    QDoubleSpinBox *dsbKfactor;
    QLabel *lblKfactor;
    QCheckBox *checkBox;
    QLineEdit *lineEdit;
    QLabel *lblPath;

    void setupUi(QDialog *ConfirmMultiLink)
    {
        if (ConfirmMultiLink->objectName().isEmpty())
            ConfirmMultiLink->setObjectName(QString::fromUtf8("ConfirmMultiLink"));
        ConfirmMultiLink->resize(644, 397);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../MultiLink.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmMultiLink->setWindowIcon(icon);
        gridLayout_2 = new QWidget(ConfirmMultiLink);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(445, 353, 178, 36));
        gridlayAccept = new QGridLayout(gridLayout_2);
        gridlayAccept->setObjectName(QString::fromUtf8("gridlayAccept"));
        gridlayAccept->setContentsMargins(0, 0, 0, 0);
        pushOk = new QPushButton(gridLayout_2);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));

        gridlayAccept->addWidget(pushOk, 0, 0, 1, 1);

        pushCancel = new QPushButton(gridLayout_2);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        gridlayAccept->addWidget(pushCancel, 0, 1, 1, 1);

        tableInstallations = new QTableWidget(ConfirmMultiLink);
        if (tableInstallations->columnCount() < 3)
            tableInstallations->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableInstallations->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableInstallations->setObjectName(QString::fromUtf8("tableInstallations"));
        tableInstallations->setGeometry(QRect(10, 5, 361, 386));
        lblTechnology = new QLabel(ConfirmMultiLink);
        lblTechnology->setObjectName(QString::fromUtf8("lblTechnology"));
        lblTechnology->setGeometry(QRect(380, 35, 81, 27));
        cbMinClear = new QComboBox(ConfirmMultiLink);
        cbMinClear->setObjectName(QString::fromUtf8("cbMinClear"));
        cbMinClear->setGeometry(QRect(540, 175, 96, 27));
        lblMaxPath = new QLabel(ConfirmMultiLink);
        lblMaxPath->setObjectName(QString::fromUtf8("lblMaxPath"));
        lblMaxPath->setGeometry(QRect(380, 115, 171, 16));
        cbTechnology = new QComboBox(ConfirmMultiLink);
        cbTechnology->setObjectName(QString::fromUtf8("cbTechnology"));
        cbTechnology->setGeometry(QRect(480, 35, 156, 27));
        lblMinClear = new QLabel(ConfirmMultiLink);
        lblMinClear->setObjectName(QString::fromUtf8("lblMinClear"));
        lblMinClear->setGeometry(QRect(380, 175, 136, 27));
        dsbMaxPath = new QDoubleSpinBox(ConfirmMultiLink);
        dsbMaxPath->setObjectName(QString::fromUtf8("dsbMaxPath"));
        dsbMaxPath->setGeometry(QRect(550, 105, 86, 28));
        dsbMaxPath->setMaximum(300);
        lblMaxDist = new QLabel(ConfirmMultiLink);
        lblMaxDist->setObjectName(QString::fromUtf8("lblMaxDist"));
        lblMaxDist->setGeometry(QRect(380, 140, 171, 27));
        dsbMaxDist = new QDoubleSpinBox(ConfirmMultiLink);
        dsbMaxDist->setObjectName(QString::fromUtf8("dsbMaxDist"));
        dsbMaxDist->setGeometry(QRect(550, 140, 86, 28));
        dsbMaxDist->setMaximum(1000);
        lblProject = new QLabel(ConfirmMultiLink);
        lblProject->setObjectName(QString::fromUtf8("lblProject"));
        lblProject->setGeometry(QRect(380, 70, 76, 27));
        cbProject = new QComboBox(ConfirmMultiLink);
        cbProject->setObjectName(QString::fromUtf8("cbProject"));
        cbProject->setGeometry(QRect(480, 70, 156, 27));
        dsbKfactor = new QDoubleSpinBox(ConfirmMultiLink);
        dsbKfactor->setObjectName(QString::fromUtf8("dsbKfactor"));
        dsbKfactor->setGeometry(QRect(565, 210, 71, 28));
        dsbKfactor->setMaximum(10);
        dsbKfactor->setSingleStep(0.01);
        dsbKfactor->setValue(1);
        lblKfactor = new QLabel(ConfirmMultiLink);
        lblKfactor->setObjectName(QString::fromUtf8("lblKfactor"));
        lblKfactor->setGeometry(QRect(480, 210, 61, 27));
        checkBox = new QCheckBox(ConfirmMultiLink);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(380, 267, 221, 16));
        lineEdit = new QLineEdit(ConfirmMultiLink);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(385, 315, 246, 26));
        lblPath = new QLabel(ConfirmMultiLink);
        lblPath->setObjectName(QString::fromUtf8("lblPath"));
        lblPath->setGeometry(QRect(385, 290, 181, 18));

        retranslateUi(ConfirmMultiLink);

        QMetaObject::connectSlotsByName(ConfirmMultiLink);
    } // setupUi

    void retranslateUi(QDialog *ConfirmMultiLink)
    {
        ConfirmMultiLink->setWindowTitle(QApplication::translate("ConfirmMultiLink", "Multi Link Identification Dialog", 0, QApplication::UnicodeUTF8));
        pushOk->setText(QApplication::translate("ConfirmMultiLink", "OK", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("ConfirmMultiLink", "Cancel", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = tableInstallations->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("ConfirmMultiLink", "Site", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = tableInstallations->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("ConfirmMultiLink", "SiteID", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem2 = tableInstallations->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("ConfirmMultiLink", "Status", 0, QApplication::UnicodeUTF8));
        lblTechnology->setText(QApplication::translate("ConfirmMultiLink", "Technology: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbMinClear->setToolTip(QApplication::translate("ConfirmMultiLink", "0 - means Line of Sight; 60 means 60% of the 1st Fresnel Zone is clear; 100 means the entire 1st Fresnel Zone is clear.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblMaxPath->setText(QApplication::translate("ConfirmMultiLink", "Maximum Path Loss (dB) : ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbTechnology->setToolTip(QApplication::translate("ConfirmMultiLink", "This is the technology from which the parameters will be derived and from which the radio installations will generated. The technology must have a Default Site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblMinClear->setText(QApplication::translate("ConfirmMultiLink", "Minimum Clearance: ", 0, QApplication::UnicodeUTF8));
        lblMaxDist->setText(QApplication::translate("ConfirmMultiLink", "Maximum Distance (km) : ", 0, QApplication::UnicodeUTF8));
        lblProject->setText(QApplication::translate("ConfirmMultiLink", "Project: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbProject->setToolTip(QApplication::translate("ConfirmMultiLink", "The project to which the generated Radioinstallations should be assigned", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        dsbKfactor->setWhatsThis(QApplication::translate("ConfirmMultiLink", "This is used to indicate the refractivity gradient in the lower troposphere", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        lblKfactor->setText(QApplication::translate("ConfirmMultiLink", "k-Factor : ", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("ConfirmMultiLink", "Save working links pdf files?", 0, QApplication::UnicodeUTF8));
        lblPath->setText(QApplication::translate("ConfirmMultiLink", "Directory to save pdf files:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ConfirmMultiLink);
    } // retranslateUi

};

namespace Ui {
    class ConfirmMultiLink: public Ui_ConfirmMultiLink {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMMULTILINK_H
