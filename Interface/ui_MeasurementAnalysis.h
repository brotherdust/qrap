/********************************************************************************
** Form generated from reading UI file 'MeasurementAnalysis.ui'
**
** Created: Thu May 10 10:46:09 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEASUREMENTANALYSIS_H
#define UI_MEASUREMENTANALYSIS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeasurementAnalysis
{
public:
    QWidget *gridLayout_2;
    QGridLayout *gridlayAccept;
    QPushButton *pushCancel;
    QPushButton *pushOk;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *lblMeasurementTy;
    QComboBox *cBoxMeasType;
    QLabel *lblPositionSource;
    QComboBox *cBoxPosSource;
    QLabel *lblMeasSource;
    QComboBox *cBoxMeasSource;
    QLabel *lblMeanError;
    QLabel *ValueMeanError;
    QLabel *lblStdDev;
    QLabel *ValueStdDev;
    QLabel *llblCorrCoef;
    QLabel *ValueCorrCoef;

    void setupUi(QDialog *MeasurementAnalysis)
    {
        if (MeasurementAnalysis->objectName().isEmpty())
            MeasurementAnalysis->setObjectName(QString::fromUtf8("MeasurementAnalysis"));
        MeasurementAnalysis->resize(382, 246);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../MultiLink.png"), QSize(), QIcon::Normal, QIcon::Off);
        MeasurementAnalysis->setWindowIcon(icon);
        gridLayout_2 = new QWidget(MeasurementAnalysis);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(190, 210, 178, 31));
        gridlayAccept = new QGridLayout(gridLayout_2);
        gridlayAccept->setObjectName(QString::fromUtf8("gridlayAccept"));
        gridlayAccept->setContentsMargins(0, 0, 0, 0);
        pushCancel = new QPushButton(gridLayout_2);
        pushCancel->setObjectName(QString::fromUtf8("pushCancel"));

        gridlayAccept->addWidget(pushCancel, 0, 1, 1, 1);

        pushOk = new QPushButton(gridLayout_2);
        pushOk->setObjectName(QString::fromUtf8("pushOk"));

        gridlayAccept->addWidget(pushOk, 0, 0, 1, 1);

        layoutWidget = new QWidget(MeasurementAnalysis);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 12, 361, 191));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lblMeasurementTy = new QLabel(layoutWidget);
        lblMeasurementTy->setObjectName(QString::fromUtf8("lblMeasurementTy"));
        lblMeasurementTy->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblMeasurementTy, 0, 0, 1, 1);

        cBoxMeasType = new QComboBox(layoutWidget);
        cBoxMeasType->setObjectName(QString::fromUtf8("cBoxMeasType"));

        gridLayout->addWidget(cBoxMeasType, 0, 1, 1, 1);

        lblPositionSource = new QLabel(layoutWidget);
        lblPositionSource->setObjectName(QString::fromUtf8("lblPositionSource"));
        lblPositionSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblPositionSource, 1, 0, 1, 1);

        cBoxPosSource = new QComboBox(layoutWidget);
        cBoxPosSource->setObjectName(QString::fromUtf8("cBoxPosSource"));

        gridLayout->addWidget(cBoxPosSource, 1, 1, 1, 1);

        lblMeasSource = new QLabel(layoutWidget);
        lblMeasSource->setObjectName(QString::fromUtf8("lblMeasSource"));
        lblMeasSource->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblMeasSource, 2, 0, 1, 1);

        cBoxMeasSource = new QComboBox(layoutWidget);
        cBoxMeasSource->setObjectName(QString::fromUtf8("cBoxMeasSource"));

        gridLayout->addWidget(cBoxMeasSource, 2, 1, 1, 1);

        lblMeanError = new QLabel(layoutWidget);
        lblMeanError->setObjectName(QString::fromUtf8("lblMeanError"));
        lblMeanError->setEnabled(false);
        lblMeanError->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblMeanError, 3, 0, 1, 1);

        ValueMeanError = new QLabel(layoutWidget);
        ValueMeanError->setObjectName(QString::fromUtf8("ValueMeanError"));
        ValueMeanError->setEnabled(false);

        gridLayout->addWidget(ValueMeanError, 3, 1, 1, 1);

        lblStdDev = new QLabel(layoutWidget);
        lblStdDev->setObjectName(QString::fromUtf8("lblStdDev"));
        lblStdDev->setEnabled(false);
        lblStdDev->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(lblStdDev, 4, 0, 1, 1);

        ValueStdDev = new QLabel(layoutWidget);
        ValueStdDev->setObjectName(QString::fromUtf8("ValueStdDev"));
        ValueStdDev->setEnabled(false);

        gridLayout->addWidget(ValueStdDev, 4, 1, 1, 1);

        llblCorrCoef = new QLabel(layoutWidget);
        llblCorrCoef->setObjectName(QString::fromUtf8("llblCorrCoef"));
        llblCorrCoef->setEnabled(false);
        llblCorrCoef->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(llblCorrCoef, 5, 0, 1, 1);

        ValueCorrCoef = new QLabel(layoutWidget);
        ValueCorrCoef->setObjectName(QString::fromUtf8("ValueCorrCoef"));
        ValueCorrCoef->setEnabled(false);

        gridLayout->addWidget(ValueCorrCoef, 5, 1, 1, 1);


        retranslateUi(MeasurementAnalysis);

        QMetaObject::connectSlotsByName(MeasurementAnalysis);
    } // setupUi

    void retranslateUi(QDialog *MeasurementAnalysis)
    {
        MeasurementAnalysis->setWindowTitle(QApplication::translate("MeasurementAnalysis", "Q-Rap Measurement Analysis Request", 0, QApplication::UnicodeUTF8));
        pushCancel->setText(QApplication::translate("MeasurementAnalysis", "Cancel", 0, QApplication::UnicodeUTF8));
        pushOk->setText(QApplication::translate("MeasurementAnalysis", "OK", 0, QApplication::UnicodeUTF8));
        lblMeasurementTy->setText(QApplication::translate("MeasurementAnalysis", "Measurement Type: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cBoxMeasType->setToolTip(QApplication::translate("MeasurementAnalysis", "This is the technology from which the parameters will be derived and from which the radio installations will generated. The technology must have a Default Site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblPositionSource->setText(QApplication::translate("MeasurementAnalysis", "Position Source: ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cBoxPosSource->setToolTip(QApplication::translate("MeasurementAnalysis", "The project to which the generated Radioinstallations should be assigned", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblMeasSource->setText(QApplication::translate("MeasurementAnalysis", "Measurement Source:", 0, QApplication::UnicodeUTF8));
        lblMeanError->setText(QApplication::translate("MeasurementAnalysis", "Mean Error:", 0, QApplication::UnicodeUTF8));
        ValueMeanError->setText(QApplication::translate("MeasurementAnalysis", "Value Mean Error", 0, QApplication::UnicodeUTF8));
        lblStdDev->setText(QApplication::translate("MeasurementAnalysis", "Standard Deviation:", 0, QApplication::UnicodeUTF8));
        ValueStdDev->setText(QApplication::translate("MeasurementAnalysis", "Value StdDev", 0, QApplication::UnicodeUTF8));
        llblCorrCoef->setText(QApplication::translate("MeasurementAnalysis", "Correlation Coefficient:", 0, QApplication::UnicodeUTF8));
        ValueCorrCoef->setText(QApplication::translate("MeasurementAnalysis", "Value CorrCoef", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MeasurementAnalysis: public Ui_MeasurementAnalysis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEASUREMENTANALYSIS_H
