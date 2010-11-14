/********************************************************************************
** Form generated from reading UI file 'Filter.ui'
**
** Created: Sun Nov 14 14:21:42 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTER_H
#define UI_FILTER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Filter
{
public:
    QAction *actionOnApply;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout;
    QPushButton *pushButtonApplyFilter;
    QSpacerItem *spacerItem;
    QLabel *labelTechType;
    QComboBox *comboBoxTechType;
    QSpacerItem *spacerItem1;
    QLabel *labelStatus;
    QComboBox *comboBoxStatus;
    QSpacerItem *spacerItem2;
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
        Filter->resize(174, 551);
        actionOnApply = new QAction(Filter);
        actionOnApply->setObjectName(QString::fromUtf8("actionOnApply"));
        groupBox = new QGroupBox(Filter);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 161, 551));
        vboxLayout = new QVBoxLayout(groupBox);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        pushButtonApplyFilter = new QPushButton(groupBox);
        pushButtonApplyFilter->setObjectName(QString::fromUtf8("pushButtonApplyFilter"));

        vboxLayout->addWidget(pushButtonApplyFilter);

        spacerItem = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem);

        labelTechType = new QLabel(groupBox);
        labelTechType->setObjectName(QString::fromUtf8("labelTechType"));

        vboxLayout->addWidget(labelTechType);

        comboBoxTechType = new QComboBox(groupBox);
        comboBoxTechType->setObjectName(QString::fromUtf8("comboBoxTechType"));

        vboxLayout->addWidget(comboBoxTechType);

        spacerItem1 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem1);

        labelStatus = new QLabel(groupBox);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        labelStatus->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        vboxLayout->addWidget(labelStatus);

        comboBoxStatus = new QComboBox(groupBox);
        comboBoxStatus->setObjectName(QString::fromUtf8("comboBoxStatus"));

        vboxLayout->addWidget(comboBoxStatus);

        spacerItem2 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem2);

        labelProject = new QLabel(groupBox);
        labelProject->setObjectName(QString::fromUtf8("labelProject"));

        vboxLayout->addWidget(labelProject);

        comboBoxProject = new QComboBox(groupBox);
        comboBoxProject->setObjectName(QString::fromUtf8("comboBoxProject"));

        vboxLayout->addWidget(comboBoxProject);

        spacerItem3 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem3);

        labelFlagX = new QLabel(groupBox);
        labelFlagX->setObjectName(QString::fromUtf8("labelFlagX"));

        vboxLayout->addWidget(labelFlagX);

        comboBoxFlagX = new QComboBox(groupBox);
        comboBoxFlagX->setObjectName(QString::fromUtf8("comboBoxFlagX"));

        vboxLayout->addWidget(comboBoxFlagX);

        spacerItem4 = new QSpacerItem(139, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem4);

        labelFlagZ = new QLabel(groupBox);
        labelFlagZ->setObjectName(QString::fromUtf8("labelFlagZ"));

        vboxLayout->addWidget(labelFlagZ);

        comboBoxFlagZ = new QComboBox(groupBox);
        comboBoxFlagZ->setObjectName(QString::fromUtf8("comboBoxFlagZ"));

        vboxLayout->addWidget(comboBoxFlagZ);

        spacerItem5 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem5);

        labelAreaType = new QLabel(groupBox);
        labelAreaType->setObjectName(QString::fromUtf8("labelAreaType"));

        vboxLayout->addWidget(labelAreaType);

        comboBoxAreaType = new QComboBox(groupBox);
        comboBoxAreaType->setObjectName(QString::fromUtf8("comboBoxAreaType"));

        vboxLayout->addWidget(comboBoxAreaType);

        labelWhere = new QLabel(groupBox);
        labelWhere->setObjectName(QString::fromUtf8("labelWhere"));

        vboxLayout->addWidget(labelWhere);

        comboBoxField = new QComboBox(groupBox);
        comboBoxField->setObjectName(QString::fromUtf8("comboBoxField"));

        vboxLayout->addWidget(comboBoxField);

        labelArea = new QLabel(groupBox);
        labelArea->setObjectName(QString::fromUtf8("labelArea"));

        vboxLayout->addWidget(labelArea);

        comboBoxArea = new QComboBox(groupBox);
        comboBoxArea->setObjectName(QString::fromUtf8("comboBoxArea"));

        vboxLayout->addWidget(comboBoxArea);

        pushButtonCreateArea = new QPushButton(groupBox);
        pushButtonCreateArea->setObjectName(QString::fromUtf8("pushButtonCreateArea"));

        vboxLayout->addWidget(pushButtonCreateArea);


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
        labelStatus->setText(QApplication::translate("Filter", "Status:", 0, QApplication::UnicodeUTF8));
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

#endif // UI_FILTER_H
