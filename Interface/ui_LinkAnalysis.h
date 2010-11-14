/********************************************************************************
** Form generated from reading UI file 'LinkAnalysis.ui'
**
** Created: Sun Nov 14 14:10:24 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINKANALYSIS_H
#define UI_LINKANALYSIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_LinkAnalysis
{
public:
    QwtPlot *qwtLinkPlot;
    QWidget *gridLayout;
    QGridLayout *TxInfo;
    QLabel *TxLat;
    QLabel *TxLon;
    QLabel *TxBearing;
    QLabel *TxRecomB;
    QLabel *TxTilt;
    QLabel *TxRecomTilt;
    QLabel *lblTxLat;
    QLabel *lblTxLon;
    QLabel *lblTxBearing;
    QLabel *lblTxBearingR;
    QLabel *lblTxTilt;
    QLabel *lblTxTiltR;
    QLabel *TxElevation;
    QLabel *TxAntHeight;
    QLabel *lblTxAntHeight;
    QLabel *lblTxElevation;
    QLabel *lblTxName;
    QWidget *gridLayout_3;
    QGridLayout *LinkInfo;
    QLabel *lblkFactor;
    QLabel *lblFresnel;
    QLabel *lblPath;
    QLabel *lblFreq;
    QLabel *lblkfactor;
    QLabel *lblClearance;
    QLabel *lblLength;
    QLabel *lblFrequency;
    QLabel *lblLoss;
    QLabel *lblPathLoss;
    QLabel *lblRxLev;
    QLabel *lblRxPower;
    QPushButton *btnSave;
    QwtPlot *qwtBTLPlot;
    QWidget *gridLayout_2;
    QGridLayout *RxInfo;
    QLabel *RxLat;
    QLabel *RxLon;
    QLabel *RxRecomB;
    QLabel *RxRecomTilt_2;
    QLabel *RxRecomTilt;
    QLabel *lblRxLat;
    QLabel *lblRxElevation;
    QLabel *lblRxBearing;
    QLabel *lblRxBearingR;
    QLabel *lblRxTilt;
    QLabel *lblRxTiltR;
    QLabel *RxElevation;
    QLabel *RxBearing;
    QLabel *lblRxAntHeight;
    QLabel *RxAntHeight;
    QLabel *lblRxLon;
    QLabel *lblRxName;
    QPushButton *btnPDF;
    QPushButton *btnAlign;
    QPushButton *btnRedo;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LinkAnalysis)
    {
        if (LinkAnalysis->objectName().isEmpty())
            LinkAnalysis->setObjectName(QString::fromUtf8("LinkAnalysis"));
        LinkAnalysis->setWindowModality(Qt::WindowModal);
        LinkAnalysis->resize(978, 653);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LinkAnalysis->sizePolicy().hasHeightForWidth());
        LinkAnalysis->setSizePolicy(sizePolicy);
        LinkAnalysis->setMinimumSize(QSize(100, 100));
        LinkAnalysis->setSizeIncrement(QSize(5, 5));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../Link.png"), QSize(), QIcon::Normal, QIcon::Off);
        LinkAnalysis->setWindowIcon(icon);
        qwtLinkPlot = new QwtPlot(LinkAnalysis);
        qwtLinkPlot->setObjectName(QString::fromUtf8("qwtLinkPlot"));
        qwtLinkPlot->setGeometry(QRect(20, 25, 931, 281));
        qwtLinkPlot->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QWidget(LinkAnalysis);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setGeometry(QRect(20, 310, 241, 203));
        TxInfo = new QGridLayout(gridLayout);
        TxInfo->setObjectName(QString::fromUtf8("TxInfo"));
        TxInfo->setVerticalSpacing(6);
        TxInfo->setContentsMargins(0, 0, 0, 0);
        TxLat = new QLabel(gridLayout);
        TxLat->setObjectName(QString::fromUtf8("TxLat"));
        QFont font;
        font.setBold(false);
        font.setItalic(true);
        font.setWeight(50);
        TxLat->setFont(font);

        TxInfo->addWidget(TxLat, 1, 0, 1, 1);

        TxLon = new QLabel(gridLayout);
        TxLon->setObjectName(QString::fromUtf8("TxLon"));
        TxLon->setFont(font);

        TxInfo->addWidget(TxLon, 2, 0, 1, 1);

        TxBearing = new QLabel(gridLayout);
        TxBearing->setObjectName(QString::fromUtf8("TxBearing"));
        TxBearing->setFont(font);

        TxInfo->addWidget(TxBearing, 5, 0, 1, 1);

        TxRecomB = new QLabel(gridLayout);
        TxRecomB->setObjectName(QString::fromUtf8("TxRecomB"));
        QFont font1;
        font1.setPointSize(9);
        TxRecomB->setFont(font1);
        TxRecomB->setLayoutDirection(Qt::LeftToRight);
        TxRecomB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TxInfo->addWidget(TxRecomB, 6, 0, 1, 1);

        TxTilt = new QLabel(gridLayout);
        TxTilt->setObjectName(QString::fromUtf8("TxTilt"));
        TxTilt->setFont(font);

        TxInfo->addWidget(TxTilt, 7, 0, 1, 1);

        TxRecomTilt = new QLabel(gridLayout);
        TxRecomTilt->setObjectName(QString::fromUtf8("TxRecomTilt"));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setWeight(50);
        TxRecomTilt->setFont(font2);
        TxRecomTilt->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TxInfo->addWidget(TxRecomTilt, 8, 0, 1, 1);

        lblTxLat = new QLabel(gridLayout);
        lblTxLat->setObjectName(QString::fromUtf8("lblTxLat"));

        TxInfo->addWidget(lblTxLat, 1, 1, 1, 1);

        lblTxLon = new QLabel(gridLayout);
        lblTxLon->setObjectName(QString::fromUtf8("lblTxLon"));

        TxInfo->addWidget(lblTxLon, 2, 1, 1, 1);

        lblTxBearing = new QLabel(gridLayout);
        lblTxBearing->setObjectName(QString::fromUtf8("lblTxBearing"));

        TxInfo->addWidget(lblTxBearing, 5, 1, 1, 1);

        lblTxBearingR = new QLabel(gridLayout);
        lblTxBearingR->setObjectName(QString::fromUtf8("lblTxBearingR"));

        TxInfo->addWidget(lblTxBearingR, 6, 1, 1, 1);

        lblTxTilt = new QLabel(gridLayout);
        lblTxTilt->setObjectName(QString::fromUtf8("lblTxTilt"));

        TxInfo->addWidget(lblTxTilt, 7, 1, 1, 1);

        lblTxTiltR = new QLabel(gridLayout);
        lblTxTiltR->setObjectName(QString::fromUtf8("lblTxTiltR"));

        TxInfo->addWidget(lblTxTiltR, 8, 1, 1, 1);

        TxElevation = new QLabel(gridLayout);
        TxElevation->setObjectName(QString::fromUtf8("TxElevation"));
        TxElevation->setFont(font);

        TxInfo->addWidget(TxElevation, 3, 0, 1, 1);

        TxAntHeight = new QLabel(gridLayout);
        TxAntHeight->setObjectName(QString::fromUtf8("TxAntHeight"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Sans Serif"));
        font3.setBold(false);
        font3.setItalic(true);
        font3.setWeight(50);
        TxAntHeight->setFont(font3);

        TxInfo->addWidget(TxAntHeight, 4, 0, 1, 1);

        lblTxAntHeight = new QLabel(gridLayout);
        lblTxAntHeight->setObjectName(QString::fromUtf8("lblTxAntHeight"));

        TxInfo->addWidget(lblTxAntHeight, 4, 1, 1, 1);

        lblTxElevation = new QLabel(gridLayout);
        lblTxElevation->setObjectName(QString::fromUtf8("lblTxElevation"));

        TxInfo->addWidget(lblTxElevation, 3, 1, 1, 1);

        lblTxName = new QLabel(gridLayout);
        lblTxName->setObjectName(QString::fromUtf8("lblTxName"));
        QFont font4;
        font4.setFamily(QString::fromUtf8("Sans Serif"));
        font4.setPointSize(12);
        font4.setBold(true);
        font4.setItalic(false);
        font4.setUnderline(true);
        font4.setWeight(75);
        lblTxName->setFont(font4);

        TxInfo->addWidget(lblTxName, 0, 0, 1, 2);

        gridLayout_3 = new QWidget(LinkAnalysis);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setGeometry(QRect(280, 340, 291, 171));
        LinkInfo = new QGridLayout(gridLayout_3);
        LinkInfo->setSpacing(7);
        LinkInfo->setObjectName(QString::fromUtf8("LinkInfo"));
        LinkInfo->setContentsMargins(0, 0, 0, 0);
        lblkFactor = new QLabel(gridLayout_3);
        lblkFactor->setObjectName(QString::fromUtf8("lblkFactor"));
        lblkFactor->setFont(font);

        LinkInfo->addWidget(lblkFactor, 2, 0, 1, 1);

        lblFresnel = new QLabel(gridLayout_3);
        lblFresnel->setObjectName(QString::fromUtf8("lblFresnel"));
        lblFresnel->setFont(font);

        LinkInfo->addWidget(lblFresnel, 3, 0, 1, 1);

        lblPath = new QLabel(gridLayout_3);
        lblPath->setObjectName(QString::fromUtf8("lblPath"));
        lblPath->setFont(font);

        LinkInfo->addWidget(lblPath, 4, 0, 1, 1);

        lblFreq = new QLabel(gridLayout_3);
        lblFreq->setObjectName(QString::fromUtf8("lblFreq"));
        lblFreq->setFont(font);

        LinkInfo->addWidget(lblFreq, 5, 0, 1, 1);

        lblkfactor = new QLabel(gridLayout_3);
        lblkfactor->setObjectName(QString::fromUtf8("lblkfactor"));

        LinkInfo->addWidget(lblkfactor, 2, 1, 1, 1);

        lblClearance = new QLabel(gridLayout_3);
        lblClearance->setObjectName(QString::fromUtf8("lblClearance"));

        LinkInfo->addWidget(lblClearance, 3, 1, 1, 1);

        lblLength = new QLabel(gridLayout_3);
        lblLength->setObjectName(QString::fromUtf8("lblLength"));

        LinkInfo->addWidget(lblLength, 4, 1, 1, 1);

        lblFrequency = new QLabel(gridLayout_3);
        lblFrequency->setObjectName(QString::fromUtf8("lblFrequency"));

        LinkInfo->addWidget(lblFrequency, 5, 1, 1, 1);

        lblLoss = new QLabel(gridLayout_3);
        lblLoss->setObjectName(QString::fromUtf8("lblLoss"));
        lblLoss->setFont(font);

        LinkInfo->addWidget(lblLoss, 1, 0, 1, 1);

        lblPathLoss = new QLabel(gridLayout_3);
        lblPathLoss->setObjectName(QString::fromUtf8("lblPathLoss"));

        LinkInfo->addWidget(lblPathLoss, 1, 1, 1, 1);

        lblRxLev = new QLabel(gridLayout_3);
        lblRxLev->setObjectName(QString::fromUtf8("lblRxLev"));
        lblRxLev->setFont(font);

        LinkInfo->addWidget(lblRxLev, 0, 0, 1, 1);

        lblRxPower = new QLabel(gridLayout_3);
        lblRxPower->setObjectName(QString::fromUtf8("lblRxPower"));

        LinkInfo->addWidget(lblRxPower, 0, 1, 1, 1);

        btnSave = new QPushButton(LinkAnalysis);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setGeometry(QRect(850, 360, 111, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../../.designer/backup"), QSize(), QIcon::Normal, QIcon::Off);
        btnSave->setIcon(icon1);
        qwtBTLPlot = new QwtPlot(LinkAnalysis);
        qwtBTLPlot->setObjectName(QString::fromUtf8("qwtBTLPlot"));
        qwtBTLPlot->setGeometry(QRect(20, 520, 941, 131));
        gridLayout_2 = new QWidget(LinkAnalysis);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(590, 310, 246, 203));
        RxInfo = new QGridLayout(gridLayout_2);
        RxInfo->setObjectName(QString::fromUtf8("RxInfo"));
        RxInfo->setContentsMargins(0, 0, 0, 0);
        RxLat = new QLabel(gridLayout_2);
        RxLat->setObjectName(QString::fromUtf8("RxLat"));
        RxLat->setFont(font);

        RxInfo->addWidget(RxLat, 3, 0, 1, 1);

        RxLon = new QLabel(gridLayout_2);
        RxLon->setObjectName(QString::fromUtf8("RxLon"));
        RxLon->setFont(font);

        RxInfo->addWidget(RxLon, 4, 0, 1, 1);

        RxRecomB = new QLabel(gridLayout_2);
        RxRecomB->setObjectName(QString::fromUtf8("RxRecomB"));
        RxRecomB->setFont(font1);
        RxRecomB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        RxInfo->addWidget(RxRecomB, 8, 0, 1, 1);

        RxRecomTilt_2 = new QLabel(gridLayout_2);
        RxRecomTilt_2->setObjectName(QString::fromUtf8("RxRecomTilt_2"));
        RxRecomTilt_2->setFont(font);

        RxInfo->addWidget(RxRecomTilt_2, 9, 0, 1, 1);

        RxRecomTilt = new QLabel(gridLayout_2);
        RxRecomTilt->setObjectName(QString::fromUtf8("RxRecomTilt"));
        RxRecomTilt->setFont(font1);
        RxRecomTilt->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        RxInfo->addWidget(RxRecomTilt, 10, 0, 1, 1);

        lblRxLat = new QLabel(gridLayout_2);
        lblRxLat->setObjectName(QString::fromUtf8("lblRxLat"));

        RxInfo->addWidget(lblRxLat, 3, 1, 1, 1);

        lblRxElevation = new QLabel(gridLayout_2);
        lblRxElevation->setObjectName(QString::fromUtf8("lblRxElevation"));

        RxInfo->addWidget(lblRxElevation, 5, 1, 1, 1);

        lblRxBearing = new QLabel(gridLayout_2);
        lblRxBearing->setObjectName(QString::fromUtf8("lblRxBearing"));

        RxInfo->addWidget(lblRxBearing, 7, 1, 1, 1);

        lblRxBearingR = new QLabel(gridLayout_2);
        lblRxBearingR->setObjectName(QString::fromUtf8("lblRxBearingR"));

        RxInfo->addWidget(lblRxBearingR, 8, 1, 1, 1);

        lblRxTilt = new QLabel(gridLayout_2);
        lblRxTilt->setObjectName(QString::fromUtf8("lblRxTilt"));

        RxInfo->addWidget(lblRxTilt, 9, 1, 1, 1);

        lblRxTiltR = new QLabel(gridLayout_2);
        lblRxTiltR->setObjectName(QString::fromUtf8("lblRxTiltR"));

        RxInfo->addWidget(lblRxTiltR, 10, 1, 1, 1);

        RxElevation = new QLabel(gridLayout_2);
        RxElevation->setObjectName(QString::fromUtf8("RxElevation"));
        RxElevation->setFont(font);

        RxInfo->addWidget(RxElevation, 5, 0, 1, 1);

        RxBearing = new QLabel(gridLayout_2);
        RxBearing->setObjectName(QString::fromUtf8("RxBearing"));
        RxBearing->setFont(font);

        RxInfo->addWidget(RxBearing, 7, 0, 1, 1);

        lblRxAntHeight = new QLabel(gridLayout_2);
        lblRxAntHeight->setObjectName(QString::fromUtf8("lblRxAntHeight"));

        RxInfo->addWidget(lblRxAntHeight, 6, 1, 1, 1);

        RxAntHeight = new QLabel(gridLayout_2);
        RxAntHeight->setObjectName(QString::fromUtf8("RxAntHeight"));
        RxAntHeight->setFont(font3);

        RxInfo->addWidget(RxAntHeight, 6, 0, 1, 1);

        lblRxLon = new QLabel(gridLayout_2);
        lblRxLon->setObjectName(QString::fromUtf8("lblRxLon"));

        RxInfo->addWidget(lblRxLon, 4, 1, 1, 1);

        lblRxName = new QLabel(gridLayout_2);
        lblRxName->setObjectName(QString::fromUtf8("lblRxName"));
        lblRxName->setFont(font4);

        RxInfo->addWidget(lblRxName, 0, 0, 1, 2);

        btnPDF = new QPushButton(LinkAnalysis);
        btnPDF->setObjectName(QString::fromUtf8("btnPDF"));
        btnPDF->setGeometry(QRect(850, 400, 111, 32));
        btnPDF->setIcon(icon1);
        btnAlign = new QPushButton(LinkAnalysis);
        btnAlign->setObjectName(QString::fromUtf8("btnAlign"));
        btnAlign->setGeometry(QRect(850, 470, 111, 31));
        btnAlign->setIcon(icon1);
        btnRedo = new QPushButton(LinkAnalysis);
        btnRedo->setObjectName(QString::fromUtf8("btnRedo"));
        btnRedo->setGeometry(QRect(850, 470, 111, 32));
        btnRedo->setIcon(icon1);
        buttonBox = new QDialogButtonBox(LinkAnalysis);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(850, 310, 111, 51));
        buttonBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(LinkAnalysis);
        QObject::connect(buttonBox, SIGNAL(accepted()), LinkAnalysis, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LinkAnalysis, SLOT(reject()));

        QMetaObject::connectSlotsByName(LinkAnalysis);
    } // setupUi

    void retranslateUi(QDialog *LinkAnalysis)
    {
        LinkAnalysis->setWindowTitle(QApplication::translate("LinkAnalysis", "Link Analysis", 0, QApplication::UnicodeUTF8));
        TxLat->setText(QApplication::translate("LinkAnalysis", "Latitude:", 0, QApplication::UnicodeUTF8));
        TxLon->setText(QApplication::translate("LinkAnalysis", "Longitude:", 0, QApplication::UnicodeUTF8));
        TxBearing->setText(QApplication::translate("LinkAnalysis", "Bearing:", 0, QApplication::UnicodeUTF8));
        TxRecomB->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        TxTilt->setText(QApplication::translate("LinkAnalysis", "Tilt:", 0, QApplication::UnicodeUTF8));
        TxRecomTilt->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        lblTxLat->setText(QString());
        lblTxLon->setText(QString());
        lblTxBearing->setText(QString());
        lblTxBearingR->setText(QString());
        lblTxTilt->setText(QString());
        lblTxTiltR->setText(QString());
        TxElevation->setText(QApplication::translate("LinkAnalysis", "Elevation (m):", 0, QApplication::UnicodeUTF8));
        TxAntHeight->setText(QApplication::translate("LinkAnalysis", "Antenna Height (m):", 0, QApplication::UnicodeUTF8));
        lblTxAntHeight->setText(QString());
        lblTxElevation->setText(QString());
        lblTxName->setText(QApplication::translate("LinkAnalysis", "Name", 0, QApplication::UnicodeUTF8));
        lblkFactor->setText(QApplication::translate("LinkAnalysis", "k-Factor:", 0, QApplication::UnicodeUTF8));
        lblFresnel->setText(QApplication::translate("LinkAnalysis", "Fresnel Clearance (%):", 0, QApplication::UnicodeUTF8));
        lblPath->setText(QApplication::translate("LinkAnalysis", "Path Length (km):", 0, QApplication::UnicodeUTF8));
        lblFreq->setText(QApplication::translate("LinkAnalysis", "Frequency (MHz):", 0, QApplication::UnicodeUTF8));
        lblkfactor->setText(QString());
        lblClearance->setText(QString());
        lblLength->setText(QString());
        lblFrequency->setText(QString());
        lblLoss->setText(QApplication::translate("LinkAnalysis", "Pathloss (dB):", 0, QApplication::UnicodeUTF8));
        lblPathLoss->setText(QString());
        lblRxLev->setText(QApplication::translate("LinkAnalysis", "Received Power (dBm):", 0, QApplication::UnicodeUTF8));
        lblRxPower->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnSave->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnSave->setText(QApplication::translate("LinkAnalysis", "Save", 0, QApplication::UnicodeUTF8));
        RxLat->setText(QApplication::translate("LinkAnalysis", "Latitude:", 0, QApplication::UnicodeUTF8));
        RxLon->setText(QApplication::translate("LinkAnalysis", "Longitude:", 0, QApplication::UnicodeUTF8));
        RxRecomB->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        RxRecomTilt_2->setText(QApplication::translate("LinkAnalysis", "Tilt:", 0, QApplication::UnicodeUTF8));
        RxRecomTilt->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        lblRxLat->setText(QString());
        lblRxElevation->setText(QString());
        lblRxBearing->setText(QString());
        lblRxBearingR->setText(QString());
        lblRxTilt->setText(QString());
        lblRxTiltR->setText(QString());
        RxElevation->setText(QApplication::translate("LinkAnalysis", "Elevation (m):", 0, QApplication::UnicodeUTF8));
        RxBearing->setText(QApplication::translate("LinkAnalysis", "Bearing:", 0, QApplication::UnicodeUTF8));
        lblRxAntHeight->setText(QString());
        RxAntHeight->setText(QApplication::translate("LinkAnalysis", "Antenna Height (m):", 0, QApplication::UnicodeUTF8));
        lblRxLon->setText(QString());
        lblRxName->setText(QApplication::translate("LinkAnalysis", "Name", 0, QApplication::UnicodeUTF8));
        btnPDF->setText(QApplication::translate("LinkAnalysis", "Export to PDF", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnAlign->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnAlign->setText(QApplication::translate("LinkAnalysis", " Align Antennae", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnRedo->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnRedo->setText(QApplication::translate("LinkAnalysis", "Recalculate", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LinkAnalysis: public Ui_LinkAnalysis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINKANALYSIS_H
