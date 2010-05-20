/********************************************************************************
** Form generated from reading ui file 'LinkAnalysis.ui'
**
** Created: Wed May 12 20:45:02 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
    QDialogButtonBox *buttonBox;
    QwtPlot *qwtLinkPlot;
    QWidget *gridLayout;
    QGridLayout *TxInfo;
    QLabel *Lat;
    QLabel *Lon;
    QLabel *Bearing;
    QLabel *RecomB;
    QLabel *Tilt;
    QLabel *RecomTilt;
    QLabel *lblTxLat;
    QLabel *lblTxLon;
    QLabel *lblTxBearing;
    QLabel *lblTxBearingR;
    QLabel *lblTxTilt;
    QLabel *lblTxTiltR;
    QLabel *Elevation;
    QLabel *AntHeight;
    QLabel *lblTxAntHeight;
    QLabel *lblTxElevation;
    QLabel *lblTxName;
    QWidget *gridLayout_2;
    QGridLayout *RxInfo;
    QLabel *Lat_2;
    QLabel *Lon_2;
    QLabel *BearR;
    QLabel *lblRxTilt_2;
    QLabel *RecomTilt_2;
    QLabel *lblRxLat;
    QLabel *lblRxElevation;
    QLabel *lblRxBearing;
    QLabel *lblRxBearingR;
    QLabel *lblRxTilt;
    QLabel *lblRxTiltR;
    QLabel *Elevation_2;
    QLabel *Bearing_2;
    QLabel *lblRxAntHeight;
    QLabel *AntHeight_2;
    QLabel *lblRxLon;
    QLabel *lblRxName;
    QWidget *gridLayout_3;
    QGridLayout *LinkInfo;
    QLabel *label_32;
    QLabel *label_34;
    QLabel *label_35;
    QLabel *label_36;
    QLabel *lblkfactor;
    QLabel *lblClearance;
    QLabel *lblLength;
    QLabel *lblFrequency;
    QLabel *label_37;
    QLabel *lblPathLoss;
    QLabel *label_31;
    QLabel *lblRxPower;
    QPushButton *btnSave;
    QPushButton *btnPDF;
    QwtPlot *qwtBTLPlot;
    QPushButton *btnRedo;
    QPushButton *btnAlign;

    void setupUi(QDialog *LinkAnalysis)
    {
        if (LinkAnalysis->objectName().isEmpty())
            LinkAnalysis->setObjectName(QString::fromUtf8("LinkAnalysis"));
        LinkAnalysis->resize(836, 732);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../.designer/backup"), QSize(), QIcon::Normal, QIcon::Off);
        LinkAnalysis->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(LinkAnalysis);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(530, 690, 166, 32));
        buttonBox->setContextMenuPolicy(Qt::DefaultContextMenu);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        qwtLinkPlot = new QwtPlot(LinkAnalysis);
        qwtLinkPlot->setObjectName(QString::fromUtf8("qwtLinkPlot"));
        qwtLinkPlot->setGeometry(QRect(20, 25, 806, 311));
        qwtLinkPlot->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QWidget(LinkAnalysis);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setGeometry(QRect(25, 355, 246, 171));
        TxInfo = new QGridLayout(gridLayout);
        TxInfo->setObjectName(QString::fromUtf8("TxInfo"));
        TxInfo->setVerticalSpacing(6);
        TxInfo->setContentsMargins(0, 0, 0, 0);
        Lat = new QLabel(gridLayout);
        Lat->setObjectName(QString::fromUtf8("Lat"));
        QFont font;
        font.setBold(false);
        font.setItalic(true);
        font.setWeight(50);
        Lat->setFont(font);

        TxInfo->addWidget(Lat, 1, 0, 1, 1);

        Lon = new QLabel(gridLayout);
        Lon->setObjectName(QString::fromUtf8("Lon"));
        Lon->setFont(font);

        TxInfo->addWidget(Lon, 2, 0, 1, 1);

        Bearing = new QLabel(gridLayout);
        Bearing->setObjectName(QString::fromUtf8("Bearing"));
        Bearing->setFont(font);

        TxInfo->addWidget(Bearing, 5, 0, 1, 1);

        RecomB = new QLabel(gridLayout);
        RecomB->setObjectName(QString::fromUtf8("RecomB"));
        QFont font1;
        font1.setPointSize(9);
        RecomB->setFont(font1);
        RecomB->setLayoutDirection(Qt::LeftToRight);
        RecomB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TxInfo->addWidget(RecomB, 6, 0, 1, 1);

        Tilt = new QLabel(gridLayout);
        Tilt->setObjectName(QString::fromUtf8("Tilt"));
        Tilt->setFont(font);

        TxInfo->addWidget(Tilt, 7, 0, 1, 1);

        RecomTilt = new QLabel(gridLayout);
        RecomTilt->setObjectName(QString::fromUtf8("RecomTilt"));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setWeight(50);
        RecomTilt->setFont(font2);
        RecomTilt->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        TxInfo->addWidget(RecomTilt, 8, 0, 1, 1);

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

        Elevation = new QLabel(gridLayout);
        Elevation->setObjectName(QString::fromUtf8("Elevation"));
        Elevation->setFont(font);

        TxInfo->addWidget(Elevation, 3, 0, 1, 1);

        AntHeight = new QLabel(gridLayout);
        AntHeight->setObjectName(QString::fromUtf8("AntHeight"));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Sans Serif"));
        font3.setBold(false);
        font3.setItalic(true);
        font3.setWeight(50);
        AntHeight->setFont(font3);

        TxInfo->addWidget(AntHeight, 4, 0, 1, 1);

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

        gridLayout_2 = new QWidget(LinkAnalysis);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(580, 350, 246, 176));
        RxInfo = new QGridLayout(gridLayout_2);
        RxInfo->setObjectName(QString::fromUtf8("RxInfo"));
        RxInfo->setContentsMargins(0, 0, 0, 0);
        Lat_2 = new QLabel(gridLayout_2);
        Lat_2->setObjectName(QString::fromUtf8("Lat_2"));
        Lat_2->setFont(font);

        RxInfo->addWidget(Lat_2, 1, 0, 1, 1);

        Lon_2 = new QLabel(gridLayout_2);
        Lon_2->setObjectName(QString::fromUtf8("Lon_2"));
        Lon_2->setFont(font);

        RxInfo->addWidget(Lon_2, 2, 0, 1, 1);

        BearR = new QLabel(gridLayout_2);
        BearR->setObjectName(QString::fromUtf8("BearR"));
        BearR->setFont(font1);
        BearR->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        RxInfo->addWidget(BearR, 6, 0, 1, 1);

        lblRxTilt_2 = new QLabel(gridLayout_2);
        lblRxTilt_2->setObjectName(QString::fromUtf8("lblRxTilt_2"));
        lblRxTilt_2->setFont(font);

        RxInfo->addWidget(lblRxTilt_2, 7, 0, 1, 1);

        RecomTilt_2 = new QLabel(gridLayout_2);
        RecomTilt_2->setObjectName(QString::fromUtf8("RecomTilt_2"));
        RecomTilt_2->setFont(font1);
        RecomTilt_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        RxInfo->addWidget(RecomTilt_2, 8, 0, 1, 1);

        lblRxLat = new QLabel(gridLayout_2);
        lblRxLat->setObjectName(QString::fromUtf8("lblRxLat"));

        RxInfo->addWidget(lblRxLat, 1, 1, 1, 1);

        lblRxElevation = new QLabel(gridLayout_2);
        lblRxElevation->setObjectName(QString::fromUtf8("lblRxElevation"));

        RxInfo->addWidget(lblRxElevation, 3, 1, 1, 1);

        lblRxBearing = new QLabel(gridLayout_2);
        lblRxBearing->setObjectName(QString::fromUtf8("lblRxBearing"));

        RxInfo->addWidget(lblRxBearing, 5, 1, 1, 1);

        lblRxBearingR = new QLabel(gridLayout_2);
        lblRxBearingR->setObjectName(QString::fromUtf8("lblRxBearingR"));

        RxInfo->addWidget(lblRxBearingR, 6, 1, 1, 1);

        lblRxTilt = new QLabel(gridLayout_2);
        lblRxTilt->setObjectName(QString::fromUtf8("lblRxTilt"));

        RxInfo->addWidget(lblRxTilt, 7, 1, 1, 1);

        lblRxTiltR = new QLabel(gridLayout_2);
        lblRxTiltR->setObjectName(QString::fromUtf8("lblRxTiltR"));

        RxInfo->addWidget(lblRxTiltR, 8, 1, 1, 1);

        Elevation_2 = new QLabel(gridLayout_2);
        Elevation_2->setObjectName(QString::fromUtf8("Elevation_2"));
        Elevation_2->setFont(font);

        RxInfo->addWidget(Elevation_2, 3, 0, 1, 1);

        Bearing_2 = new QLabel(gridLayout_2);
        Bearing_2->setObjectName(QString::fromUtf8("Bearing_2"));
        Bearing_2->setFont(font);

        RxInfo->addWidget(Bearing_2, 5, 0, 1, 1);

        lblRxAntHeight = new QLabel(gridLayout_2);
        lblRxAntHeight->setObjectName(QString::fromUtf8("lblRxAntHeight"));

        RxInfo->addWidget(lblRxAntHeight, 4, 1, 1, 1);

        AntHeight_2 = new QLabel(gridLayout_2);
        AntHeight_2->setObjectName(QString::fromUtf8("AntHeight_2"));
        AntHeight_2->setFont(font3);

        RxInfo->addWidget(AntHeight_2, 4, 0, 1, 1);

        lblRxLon = new QLabel(gridLayout_2);
        lblRxLon->setObjectName(QString::fromUtf8("lblRxLon"));

        RxInfo->addWidget(lblRxLon, 2, 1, 1, 1);

        lblRxName = new QLabel(gridLayout_2);
        lblRxName->setObjectName(QString::fromUtf8("lblRxName"));
        lblRxName->setFont(font4);

        RxInfo->addWidget(lblRxName, 0, 0, 1, 2);

        gridLayout_3 = new QWidget(LinkAnalysis);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setGeometry(QRect(280, 375, 291, 151));
        LinkInfo = new QGridLayout(gridLayout_3);
        LinkInfo->setSpacing(7);
        LinkInfo->setObjectName(QString::fromUtf8("LinkInfo"));
        LinkInfo->setContentsMargins(0, 0, 0, 0);
        label_32 = new QLabel(gridLayout_3);
        label_32->setObjectName(QString::fromUtf8("label_32"));
        label_32->setFont(font);

        LinkInfo->addWidget(label_32, 2, 0, 1, 1);

        label_34 = new QLabel(gridLayout_3);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setFont(font);

        LinkInfo->addWidget(label_34, 3, 0, 1, 1);

        label_35 = new QLabel(gridLayout_3);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setFont(font);

        LinkInfo->addWidget(label_35, 4, 0, 1, 1);

        label_36 = new QLabel(gridLayout_3);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setFont(font);

        LinkInfo->addWidget(label_36, 5, 0, 1, 1);

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

        label_37 = new QLabel(gridLayout_3);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setFont(font);

        LinkInfo->addWidget(label_37, 1, 0, 1, 1);

        lblPathLoss = new QLabel(gridLayout_3);
        lblPathLoss->setObjectName(QString::fromUtf8("lblPathLoss"));

        LinkInfo->addWidget(lblPathLoss, 1, 1, 1, 1);

        label_31 = new QLabel(gridLayout_3);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setFont(font);

        LinkInfo->addWidget(label_31, 0, 0, 1, 1);

        lblRxPower = new QLabel(gridLayout_3);
        lblRxPower->setObjectName(QString::fromUtf8("lblRxPower"));

        LinkInfo->addWidget(lblRxPower, 0, 1, 1, 1);

        btnSave = new QPushButton(LinkAnalysis);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));
        btnSave->setGeometry(QRect(430, 690, 91, 32));
        btnSave->setIcon(icon);
        btnPDF = new QPushButton(LinkAnalysis);
        btnPDF->setObjectName(QString::fromUtf8("btnPDF"));
        btnPDF->setGeometry(QRect(190, 690, 101, 32));
        btnPDF->setIcon(icon);
        qwtBTLPlot = new QwtPlot(LinkAnalysis);
        qwtBTLPlot->setObjectName(QString::fromUtf8("qwtBTLPlot"));
        qwtBTLPlot->setGeometry(QRect(20, 545, 811, 136));
        btnRedo = new QPushButton(LinkAnalysis);
        btnRedo->setObjectName(QString::fromUtf8("btnRedo"));
        btnRedo->setGeometry(QRect(300, 690, 121, 32));
        btnRedo->setIcon(icon);
        btnAlign = new QPushButton(LinkAnalysis);
        btnAlign->setObjectName(QString::fromUtf8("btnAlign"));
        btnAlign->setGeometry(QRect(300, 690, 121, 31));
        btnAlign->setIcon(icon);

        retranslateUi(LinkAnalysis);
        QObject::connect(buttonBox, SIGNAL(accepted()), LinkAnalysis, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LinkAnalysis, SLOT(reject()));

        QMetaObject::connectSlotsByName(LinkAnalysis);
    } // setupUi

    void retranslateUi(QDialog *LinkAnalysis)
    {
        LinkAnalysis->setWindowTitle(QApplication::translate("LinkAnalysis", "Link Analysis", 0, QApplication::UnicodeUTF8));
        Lat->setText(QApplication::translate("LinkAnalysis", "Latitude:", 0, QApplication::UnicodeUTF8));
        Lon->setText(QApplication::translate("LinkAnalysis", "Longitude:", 0, QApplication::UnicodeUTF8));
        Bearing->setText(QApplication::translate("LinkAnalysis", "Bearing:", 0, QApplication::UnicodeUTF8));
        RecomB->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        Tilt->setText(QApplication::translate("LinkAnalysis", "Tilt:", 0, QApplication::UnicodeUTF8));
        RecomTilt->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        lblTxLat->setText(QString());
        lblTxLon->setText(QString());
        lblTxBearing->setText(QString());
        lblTxBearingR->setText(QString());
        lblTxTilt->setText(QString());
        lblTxTiltR->setText(QString());
        Elevation->setText(QApplication::translate("LinkAnalysis", "Elevation (m):", 0, QApplication::UnicodeUTF8));
        AntHeight->setText(QApplication::translate("LinkAnalysis", "Antenna Height (m):", 0, QApplication::UnicodeUTF8));
        lblTxAntHeight->setText(QString());
        lblTxElevation->setText(QString());
        lblTxName->setText(QApplication::translate("LinkAnalysis", "Name", 0, QApplication::UnicodeUTF8));
        Lat_2->setText(QApplication::translate("LinkAnalysis", "Latitude:", 0, QApplication::UnicodeUTF8));
        Lon_2->setText(QApplication::translate("LinkAnalysis", "Longitude:", 0, QApplication::UnicodeUTF8));
        BearR->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        lblRxTilt_2->setText(QApplication::translate("LinkAnalysis", "Tilt:", 0, QApplication::UnicodeUTF8));
        RecomTilt_2->setText(QApplication::translate("LinkAnalysis", "  (Recommended)", 0, QApplication::UnicodeUTF8));
        lblRxLat->setText(QString());
        lblRxElevation->setText(QString());
        lblRxBearing->setText(QString());
        lblRxBearingR->setText(QString());
        lblRxTilt->setText(QString());
        lblRxTiltR->setText(QString());
        Elevation_2->setText(QApplication::translate("LinkAnalysis", "Elevation (m):", 0, QApplication::UnicodeUTF8));
        Bearing_2->setText(QApplication::translate("LinkAnalysis", "Bearing:", 0, QApplication::UnicodeUTF8));
        lblRxAntHeight->setText(QString());
        AntHeight_2->setText(QApplication::translate("LinkAnalysis", "Antenna Height (m):", 0, QApplication::UnicodeUTF8));
        lblRxLon->setText(QString());
        lblRxName->setText(QApplication::translate("LinkAnalysis", "Name", 0, QApplication::UnicodeUTF8));
        label_32->setText(QApplication::translate("LinkAnalysis", "k-Factor:", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("LinkAnalysis", "Fresnel Clearance (%):", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("LinkAnalysis", "Path Length (km):", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("LinkAnalysis", "Frequency (MHz):", 0, QApplication::UnicodeUTF8));
        lblkfactor->setText(QString());
        lblClearance->setText(QString());
        lblLength->setText(QString());
        lblFrequency->setText(QString());
        label_37->setText(QApplication::translate("LinkAnalysis", "Pathloss (dB):", 0, QApplication::UnicodeUTF8));
        lblPathLoss->setText(QString());
        label_31->setText(QApplication::translate("LinkAnalysis", "Received Power (dBm):", 0, QApplication::UnicodeUTF8));
        lblRxPower->setText(QString());
#ifndef QT_NO_TOOLTIP
        btnSave->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnSave->setText(QApplication::translate("LinkAnalysis", "Save", 0, QApplication::UnicodeUTF8));
        btnPDF->setText(QApplication::translate("LinkAnalysis", "Export to PDF", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnRedo->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnRedo->setText(QApplication::translate("LinkAnalysis", "Recalculate", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnAlign->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        btnAlign->setText(QApplication::translate("LinkAnalysis", " Align Antennae", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(LinkAnalysis);
    } // retranslateUi

};

namespace Ui {
    class LinkAnalysis: public Ui_LinkAnalysis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINKANALYSIS_H
