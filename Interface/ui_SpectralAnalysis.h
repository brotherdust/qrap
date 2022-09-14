/********************************************************************************
** Form generated from reading UI file 'SpectralAnalysis.ui'
**
** Created: Wed May 9 18:29:00 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SPECTRALANALYSIS_H
#define UI_SPECTRALANALYSIS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_SpectralAnalysis
{
public:
    QwtPlot *qwtPlot;
    QPushButton *pushClose;
    QPushButton *pushSave;
    QLineEdit *lineFilename;
    QLabel *lblFilename;

    void setupUi(QDialog *SpectralAnalysis)
    {
        if (SpectralAnalysis->objectName().isEmpty())
            SpectralAnalysis->setObjectName(QString::fromUtf8("SpectralAnalysis"));
        SpectralAnalysis->resize(800, 600);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QRap/Spectral.png"), QSize(), QIcon::Normal, QIcon::Off);
        SpectralAnalysis->setWindowIcon(icon);
        qwtPlot = new QwtPlot(SpectralAnalysis);
        qwtPlot->setObjectName(QString::fromUtf8("qwtPlot"));
        qwtPlot->setGeometry(QRect(10, 10, 781, 521));
        pushClose = new QPushButton(SpectralAnalysis);
        pushClose->setObjectName(QString::fromUtf8("pushClose"));
        pushClose->setGeometry(QRect(700, 560, 83, 27));
        pushClose->setAutoDefault(false);
        pushSave = new QPushButton(SpectralAnalysis);
        pushSave->setObjectName(QString::fromUtf8("pushSave"));
        pushSave->setGeometry(QRect(610, 560, 83, 27));
        pushSave->setDefault(true);
        lineFilename = new QLineEdit(SpectralAnalysis);
        lineFilename->setObjectName(QString::fromUtf8("lineFilename"));
        lineFilename->setGeometry(QRect(400, 560, 201, 23));
        lblFilename = new QLabel(SpectralAnalysis);
        lblFilename->setObjectName(QString::fromUtf8("lblFilename"));
        lblFilename->setGeometry(QRect(330, 560, 63, 18));

        retranslateUi(SpectralAnalysis);

        QMetaObject::connectSlotsByName(SpectralAnalysis);
    } // setupUi

    void retranslateUi(QDialog *SpectralAnalysis)
    {
        SpectralAnalysis->setWindowTitle(QApplication::translate("SpectralAnalysis", "Spectral Power Flux", 0, QApplication::UnicodeUTF8));
        pushClose->setText(QApplication::translate("SpectralAnalysis", "Close", 0, QApplication::UnicodeUTF8));
        pushSave->setText(QApplication::translate("SpectralAnalysis", "Save", 0, QApplication::UnicodeUTF8));
        lineFilename->setText(QApplication::translate("SpectralAnalysis", "flux.csv", 0, QApplication::UnicodeUTF8));
        lblFilename->setText(QApplication::translate("SpectralAnalysis", "Filename :", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SpectralAnalysis: public Ui_SpectralAnalysis {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SPECTRALANALYSIS_H
