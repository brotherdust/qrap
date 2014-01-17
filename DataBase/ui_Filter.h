/********************************************************************************
** Form generated from reading UI file 'FilterwZ9053.ui'
**
** Created: Sun Sep 22 19:03:35 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef FILTERWZ9053_H
#define FILTERWZ9053_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Filter
{
public:
    QAction *actionOnApply;
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QPushButton *pushButtonApplyFilter;
    QSpacerItem *spacerItem;
    QLabel *labelTechType;
    QComboBox *comboBoxTechType;
    QSpacerItem *spacerItem1;
    QLabel *labelStatus;
    QComboBox *comboBoxStatus;
    QSpacerItem *spacerItem2;
    QLabel *labelLinkStatus;
    QComboBox *comboBoxLinkStatus;
    QSpacerItem *spacer;
    QLabel *labelProject;
    QComboBox *comboBoxProject;
    QSpacerItem *spacerItem3;
    QLabel *labelFlagX;
    QComboBox *comboBoxFlagX;
    QSpacerItem *spacerItem4;
    QLabel *labelFlagZ;
    QComboBox *comboBoxFlagZ;
    QSpacerItem *spacerItem5;
    QLabel *labelAreaType;
    QComboBox *comboBoxAreaType;
    QLabel *labelWhere;
    QComboBox *comboBoxField;
    QLabel *labelArea;
    QComboBox *comboBoxArea;
    QPushButton *pushButtonCreateArea;

    void setupUi(QWidget *Filter)
    {
        if (Filter->objectName().isEmpty())
            Filter->setObjectName(QString::fromUtf8("Filter"));
        Filter->resize(196, 671);
        actionOnApply = new QAction(Filter);
        actionOnApply->setObjectName(QString::fromUtf8("actionOnApply"));
        gridLayout = new QGridLayout(Filter);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox = new QGroupBox(Filter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        pushButtonApplyFilter = new QPushButton(groupBox);
        pushButtonApplyFilter->setObjectName(QString::fromUtf8("pushButtonApplyFilter"));

        gridLayout_2->addWidget(pushButtonApplyFilter, 0, 0, 1, 1);

        spacerItem = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem, 1, 0, 1, 1);

        labelTechType = new QLabel(groupBox);
        labelTechType->setObjectName(QString::fromUtf8("labelTechType"));

        gridLayout_2->addWidget(labelTechType, 2, 0, 1, 1);

        comboBoxTechType = new QComboBox(groupBox);
        comboBoxTechType->setObjectName(QString::fromUtf8("comboBoxTechType"));

        gridLayout_2->addWidget(comboBoxTechType, 3, 0, 1, 1);

        spacerItem1 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem1, 4, 0, 1, 1);

        labelStatus = new QLabel(groupBox);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        labelStatus->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        gridLayout_2->addWidget(labelStatus, 5, 0, 1, 1);

        comboBoxStatus = new QComboBox(groupBox);
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));

        gridLayout_2->addWidget(comboBoxStatus, 6, 0, 1, 1);

        spacerItem2 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem2, 7, 0, 1, 1);

        labelLinkStatus = new QLabel(groupBox);
        labelLinkStatus->setObjectName(QString::fromUtf8("labelLinkStatus"));
        labelLinkStatus->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        gridLayout_2->addWidget(labelLinkStatus, 8, 0, 1, 1);

        comboBoxLinkStatus = new QComboBox(groupBox);
        comboBoxLinkStatus->setObjectName(QString::fromUtf8("comboBoxLinkStatus"));

        gridLayout_2->addWidget(comboBoxLinkStatus, 9, 0, 1, 1);

        spacer = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacer, 10, 0, 1, 1);

        labelProject = new QLabel(groupBox);
        labelProject->setObjectName(QString::fromUtf8("labelProject"));

        gridLayout_2->addWidget(labelProject, 11, 0, 1, 1);

        comboBoxProject = new QComboBox(groupBox);
        comboBoxProject->setObjectName(QString::fromUtf8("comboBoxProject"));

        gridLayout_2->addWidget(comboBoxProject, 12, 0, 1, 1);

        spacerItem3 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem3, 13, 0, 1, 1);

        labelFlagX = new QLabel(groupBox);
        labelFlagX->setObjectName(QString::fromUtf8("labelFlagX"));

        gridLayout_2->addWidget(labelFlagX, 14, 0, 1, 1);

        comboBoxFlagX = new QComboBox(groupBox);
        comboBoxFlagX->setObjectName(QString::fromUtf8("comboBoxFlagX"));

        gridLayout_2->addWidget(comboBoxFlagX, 15, 0, 1, 1);

        spacerItem4 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem4, 16, 0, 1, 1);

        labelFlagZ = new QLabel(groupBox);
        labelFlagZ->setObjectName(QString::fromUtf8("labelFlagZ"));

        gridLayout_2->addWidget(labelFlagZ, 17, 0, 1, 1);

        comboBoxFlagZ = new QComboBox(groupBox);
        comboBoxFlagZ->setObjectName(QString::fromUtf8("comboBoxFlagZ"));

        gridLayout_2->addWidget(comboBoxFlagZ, 18, 0, 1, 1);

        spacerItem5 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(spacerItem5, 19, 0, 1, 1);

        labelAreaType = new QLabel(groupBox);
        labelAreaType->setObjectName(QString::fromUtf8("labelAreaType"));

        gridLayout_2->addWidget(labelAreaType, 20, 0, 1, 1);

        comboBoxAreaType = new QComboBox(groupBox);
        comboBoxAreaType->setObjectName(QString::fromUtf8("comboBoxAreaType"));

        gridLayout_2->addWidget(comboBoxAreaType, 21, 0, 1, 1);

        labelWhere = new QLabel(groupBox);
        labelWhere->setObjectName(QString::fromUtf8("labelWhere"));

        gridLayout_2->addWidget(labelWhere, 22, 0, 1, 1);

        comboBoxField = new QComboBox(groupBox);
        comboBoxField->setObjectName(QString::fromUtf8("comboBoxField"));

        gridLayout_2->addWidget(comboBoxField, 23, 0, 1, 1);

        labelArea = new QLabel(groupBox);
        labelArea->setObjectName(QString::fromUtf8("labelArea"));

        gridLayout_2->addWidget(labelArea, 24, 0, 1, 1);

        comboBoxArea = new QComboBox(groupBox);
        comboBoxArea->setObjectName(QString::fromUtf8("comboBoxArea"));

        gridLayout_2->addWidget(comboBoxArea, 25, 0, 1, 1);

        pushButtonCreateArea = new QPushButton(groupBox);
        pushButtonCreateArea->setObjectName(QString::fromUtf8("pushButtonCreateArea"));

        gridLayout_2->addWidget(pushButtonCreateArea, 26, 0, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(Filter);

        QMetaObject::connectSlotsByName(Filter);
    } // setupUi

    void retranslateUi(QWidget *Filter)
    {
        Filter->setWindowTitle(QApplication::translate("Filter", "Form", 0, QApplication::UnicodeUTF8));
        actionOnApply->setText(QApplication::translate("Filter", "onApply", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Filter", "Filter:", 0, QApplication::UnicodeUTF8));
        pushButtonApplyFilter->setText(QApplication::translate("Filter", "Apply filter changes", 0, QApplication::UnicodeUTF8));
        labelTechType->setText(QApplication::translate("Filter", "Technolgy Type:", 0, QApplication::UnicodeUTF8));
        labelStatus->setText(QApplication::translate("Filter", "Site Status:", 0, QApplication::UnicodeUTF8));
        labelLinkStatus->setText(QApplication::translate("Filter", "Link Status:", 0, QApplication::UnicodeUTF8));
        labelProject->setText(QApplication::translate("Filter", "Project:", 0, QApplication::UnicodeUTF8));
        labelFlagX->setText(QApplication::translate("Filter", "Flag X:", 0, QApplication::UnicodeUTF8));
        labelFlagZ->setText(QApplication::translate("Filter", "Flag Z:", 0, QApplication::UnicodeUTF8));
        labelAreaType->setText(QApplication::translate("Filter", "Area Type:", 0, QApplication::UnicodeUTF8));
        labelWhere->setText(QApplication::translate("Filter", "where the field:", 0, QApplication::UnicodeUTF8));
        labelArea->setText(QApplication::translate("Filter", "is Area:", 0, QApplication::UnicodeUTF8));
        pushButtonCreateArea->setText(QApplication::translate("Filter", "Create Custom Area", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Filter: public Ui_Filter {};
} // namespace Ui

QT_END_NAMESPACE

#endif // FILTERWZ9053_H
