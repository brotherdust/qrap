/********************************************************************************
** Form generated from reading ui file 'Site.ui'
**
** Created: Sat May 1 08:44:25 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SITE_H
#define UI_SITE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Site
{
public:
    QWidget *gridLayout_2;
    QGridLayout *EditLayout;
    QLabel *lblSiteName;
    QLineEdit *txtSite;
    QLabel *lblLatitude;
    QLineEdit *txtLatitude;
    QLabel *lblLongitude;
    QLineEdit *txtLongitude;
    QLabel *lblGroundHeight;
    QLineEdit *txtGroundHeight;
    QLabel *lblStatus;
    QComboBox *comboBoxStatus;
    QPushButton *btnPlaceSite;
    QPushButton *btnDefaultInst;
    QPushButton *btnCancel;
    QPushButton *btnEditInstallation;
    QPushButton *btnMove;

    void setupUi(QDialog *Site)
    {
        if (Site->objectName().isEmpty())
            Site->setObjectName(QString::fromUtf8("Site"));
        Site->resize(319, 241);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/QRap/DataBase/images/logo_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        Site->setWindowIcon(icon);
        gridLayout_2 = new QWidget(Site);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setGeometry(QRect(9, 9, 306, 161));
        EditLayout = new QGridLayout(gridLayout_2);
        EditLayout->setObjectName(QString::fromUtf8("EditLayout"));
        EditLayout->setContentsMargins(0, 0, 0, 0);
        lblSiteName = new QLabel(gridLayout_2);
        lblSiteName->setObjectName(QString::fromUtf8("lblSiteName"));

        EditLayout->addWidget(lblSiteName, 0, 0, 1, 1);

        txtSite = new QLineEdit(gridLayout_2);
        txtSite->setObjectName(QString::fromUtf8("txtSite"));

        EditLayout->addWidget(txtSite, 0, 1, 1, 1);

        lblLatitude = new QLabel(gridLayout_2);
        lblLatitude->setObjectName(QString::fromUtf8("lblLatitude"));

        EditLayout->addWidget(lblLatitude, 1, 0, 1, 1);

        txtLatitude = new QLineEdit(gridLayout_2);
        txtLatitude->setObjectName(QString::fromUtf8("txtLatitude"));

        EditLayout->addWidget(txtLatitude, 1, 1, 1, 1);

        lblLongitude = new QLabel(gridLayout_2);
        lblLongitude->setObjectName(QString::fromUtf8("lblLongitude"));

        EditLayout->addWidget(lblLongitude, 2, 0, 1, 1);

        txtLongitude = new QLineEdit(gridLayout_2);
        txtLongitude->setObjectName(QString::fromUtf8("txtLongitude"));

        EditLayout->addWidget(txtLongitude, 2, 1, 1, 1);

        lblGroundHeight = new QLabel(gridLayout_2);
        lblGroundHeight->setObjectName(QString::fromUtf8("lblGroundHeight"));

        EditLayout->addWidget(lblGroundHeight, 3, 0, 1, 1);

        txtGroundHeight = new QLineEdit(gridLayout_2);
        txtGroundHeight->setObjectName(QString::fromUtf8("txtGroundHeight"));

        EditLayout->addWidget(txtGroundHeight, 3, 1, 1, 1);

        lblStatus = new QLabel(gridLayout_2);
        lblStatus->setObjectName(QString::fromUtf8("lblStatus"));

        EditLayout->addWidget(lblStatus, 4, 0, 1, 1);

        comboBoxStatus = new QComboBox(gridLayout_2);
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));

        EditLayout->addWidget(comboBoxStatus, 4, 1, 1, 1);

        btnPlaceSite = new QPushButton(Site);
        btnPlaceSite->setObjectName(QString::fromUtf8("btnPlaceSite"));
        btnPlaceSite->setGeometry(QRect(5, 175, 85, 28));
        btnDefaultInst = new QPushButton(Site);
        btnDefaultInst->setObjectName(QString::fromUtf8("btnDefaultInst"));
        btnDefaultInst->setGeometry(QRect(145, 210, 172, 28));
        btnCancel = new QPushButton(Site);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(230, 175, 85, 28));
        btnEditInstallation = new QPushButton(Site);
        btnEditInstallation->setObjectName(QString::fromUtf8("btnEditInstallation"));
        btnEditInstallation->setGeometry(QRect(5, 210, 137, 28));
        btnMove = new QPushButton(Site);
        btnMove->setObjectName(QString::fromUtf8("btnMove"));
        btnMove->setGeometry(QRect(95, 175, 111, 28));

        retranslateUi(Site);

        QMetaObject::connectSlotsByName(Site);
    } // setupUi

    void retranslateUi(QDialog *Site)
    {
        Site->setWindowTitle(QApplication::translate("Site", "Place a Site", 0, QApplication::UnicodeUTF8));
        lblSiteName->setText(QApplication::translate("Site", "Site Name:", 0, QApplication::UnicodeUTF8));
        lblLatitude->setText(QApplication::translate("Site", "Latitude:", 0, QApplication::UnicodeUTF8));
        lblLongitude->setText(QApplication::translate("Site", "Longitude:", 0, QApplication::UnicodeUTF8));
        lblGroundHeight->setText(QApplication::translate("Site", "Groud Height:", 0, QApplication::UnicodeUTF8));
        lblStatus->setText(QApplication::translate("Site", "Status:", 0, QApplication::UnicodeUTF8));
        btnPlaceSite->setText(QApplication::translate("Site", "Just a Site", 0, QApplication::UnicodeUTF8));
        btnDefaultInst->setText(QApplication::translate("Site", "Add Default Installation", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("Site", "Cancel", 0, QApplication::UnicodeUTF8));
        btnEditInstallation->setText(QApplication::translate("Site", "Edit Installation(s)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        btnMove->setToolTip(QApplication::translate("Site", "To move the site to specific point on the map, click here, than click on the position. The values of the new position will appear combo boxes, but will only change after updating or committing the site.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        btnMove->setText(QApplication::translate("Site", "Move Site to ...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Site);
    } // retranslateUi

};

namespace Ui {
    class Site: public Ui_Site {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SITE_H
