/********************************************************************************
** Form generated from reading UI file 'ConfirmMultiLink.ui'
**
** Created: Thu May 10 14:09:51 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMMULTILINK_H
#define UI_CONFIRMMULTILINK_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

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
        ConfirmMultiLink->resize(798, 407);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../MultiLink.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfirmMultiLink->setWindowIcon(icon);
        gridLayout_2 = new QWidget(ConfirmMultiLink);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(600, 360, 178, 36));
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
        tableInstallations->setGeometry(QRect(5, 12, 521, 386));
        lblTechnology = new QLabel(ConfirmMultiLink);
        lblTechnology->setObjectName(QString::fromUtf8("lblTechnology"));
        lblTechnology->setGeometry(QRect(535, 42, 81, 27));
        cbMinClear = new QComboBox(ConfirmMultiLink);
        cbMinClear->setObjectName(QString::fromUtf8("cbMinClear"));
        cbMinClear->setGeometry(QRect(695, 182, 96, 27));
        lblMaxPath = new QLabel(ConfirmMultiLink);
        lblMaxPath->setObjectName(QString::fromUtf8("lblMaxPath"));
        lblMaxPath->setGeometry(QRect(535, 122, 171, 16));
        cbTechnology = new QComboBox(ConfirmMultiLink);
        cbTechnology->setObjectName(QString::fromUtf8("cbTechnology"));
        cbTechnology->setGeometry(QRect(635, 42, 156, 27));
        lblMinClear = new QLabel(ConfirmMultiLink);
        lblMinClear->setObjectName(QString::fromUtf8("lblMinClear"));
        lblMinClear->setGeometry(QRect(535, 182, 136, 27));
        dsbMaxPath = new QDoubleSpinBox(ConfirmMultiLink);
        dsbMaxPath->setObjectName(QString::fromUtf8("dsbMaxPath"));
        dsbMaxPath->setGeometry(QRect(705, 112, 86, 28));
        dsbMaxPath->setMaximum(300);
        lblMaxDist = new QLabel(ConfirmMultiLink);
        lblMaxDist->setObjectName(QString::fromUtf8("lblMaxDist"));
        lblMaxDist->setGeometry(QRect(535, 147, 171, 27));
        dsbMaxDist = new QDoubleSpinBox(ConfirmMultiLink);
        dsbMaxDist->setObjectName(QString::fromUtf8("dsbMaxDist"));
        dsbMaxDist->setGeometry(QRect(705, 147, 86, 28));
        dsbMaxDist->setMaximum(1000);
        lblProject = new QLabel(ConfirmMultiLink);
        lblProject->setObjectName(QString::fromUtf8("lblProject"));
        lblProject->setGeometry(QRect(535, 77, 76, 27));
        cbProject = new QComboBox(ConfirmMultiLink);
        cbProject->setObjectName(QString::fromUtf8("cbProject"));
        cbProject->setGeometry(QRect(635, 77, 156, 27));
        dsbKfactor = new QDoubleSpinBox(ConfirmMultiLink);
        dsbKfactor->setObjectName(QString::fromUtf8("dsbKfactor"));
        dsbKfactor->setGeometry(QRect(720, 217, 71, 28));
        dsbKfactor->setMaximum(10);
        dsbKfactor->setSingleStep(0.01);
        dsbKfactor->setValue(1);
        lblKfactor = new QLabel(ConfirmMultiLink);
        lblKfactor->setObjectName(QString::fromUtf8("lblKfactor"));
        lblKfactor->setGeometry(QRect(635, 217, 61, 27));
        checkBox = new QCheckBox(ConfirmMultiLink);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(535, 274, 221, 16));
        checkBox->setChecked(true);
        lineEdit = new QLineEdit(ConfirmMultiLink);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(540, 322, 246, 26));
        lblPath = new QLabel(ConfirmMultiLink);
        lblPath->setObjectName(QString::fromUtf8("lblPath"));
        lblPath->setGeometry(QRect(540, 297, 181, 18));

        retranslateUi(ConfirmMultiLink);

        QMetaObject::connectSlotsByName(ConfirmMultiLink);
    } // setupUi

    void retranslateUi(QDialog *ConfirmMultiLink)
    {
        ConfirmMultiLink->setWindowTitle(QApplication::translate("ConfirmMultiLink", "Q-Rap Multi Link Identification Request", 0, QApplication::UnicodeUTF8));
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
    } // retranslateUi

};

namespace Ui {
    class ConfirmMultiLink: public Ui_ConfirmMultiLink {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMMULTILINK_H
