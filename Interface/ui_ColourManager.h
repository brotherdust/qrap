/********************************************************************************
** Form generated from reading UI file 'ColourManager.ui'
**
** Created: Sat Nov 13 14:57:59 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLOURMANAGER_H
#define UI_COLOURMANAGER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColourManager
{
public:
    QWidget *horizontalLayout_3;
    QHBoxLayout *hboxLayout;
    QPushButton *btnOk;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout1;
    QLabel *mNumberOfEntriesLabel_3;
    QSpinBox *sboxNumberOfEntries_2;
    QLabel *label;
    QComboBox *plotTypeCombo;
    QWidget *layoutWidget1;
    QHBoxLayout *hboxLayout2;
    QPushButton *mClassifyButton;
    QHBoxLayout *hboxLayout3;
    QLabel *label_3;
    QLineEdit *minEdit;
    QLabel *label_4;
    QLineEdit *maxEdit;
    QPushButton *addButton;
    QPushButton *removeButton;
    QTreeWidget *mColormapTreeWidget;

    void setupUi(QWidget *ColourManager)
    {
        if (ColourManager->objectName().isEmpty())
            ColourManager->setObjectName(QString::fromUtf8("ColourManager"));
        ColourManager->resize(437, 505);
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        ColourManager->setWindowIcon(icon);
        horizontalLayout_3 = new QWidget(ColourManager);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setGeometry(QRect(320, 460, 101, 29));
        hboxLayout = new QHBoxLayout(horizontalLayout_3);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        btnOk = new QPushButton(horizontalLayout_3);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        hboxLayout->addWidget(btnOk);

        layoutWidget = new QWidget(ColourManager);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 40, 411, 29));
        hboxLayout1 = new QHBoxLayout(layoutWidget);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        mNumberOfEntriesLabel_3 = new QLabel(layoutWidget);
        mNumberOfEntriesLabel_3->setObjectName(QString::fromUtf8("mNumberOfEntriesLabel_3"));
        mNumberOfEntriesLabel_3->setEnabled(true);

        hboxLayout1->addWidget(mNumberOfEntriesLabel_3);

        sboxNumberOfEntries_2 = new QSpinBox(layoutWidget);
        sboxNumberOfEntries_2->setObjectName(QString::fromUtf8("sboxNumberOfEntries_2"));
        sboxNumberOfEntries_2->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sboxNumberOfEntries_2->sizePolicy().hasHeightForWidth());
        sboxNumberOfEntries_2->setSizePolicy(sizePolicy);
        sboxNumberOfEntries_2->setMaximum(10000);

        hboxLayout1->addWidget(sboxNumberOfEntries_2);

        label = new QLabel(ColourManager);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 12, 166, 26));
        plotTypeCombo = new QComboBox(ColourManager);
        plotTypeCombo->setObjectName(QString::fromUtf8("plotTypeCombo"));
        plotTypeCombo->setGeometry(QRect(140, 10, 281, 26));
        layoutWidget1 = new QWidget(ColourManager);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 80, 411, 34));
        hboxLayout2 = new QHBoxLayout(layoutWidget1);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        mClassifyButton = new QPushButton(layoutWidget1);
        mClassifyButton->setObjectName(QString::fromUtf8("mClassifyButton"));
        mClassifyButton->setEnabled(true);

        hboxLayout2->addWidget(mClassifyButton);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        hboxLayout3->addWidget(label_3);

        minEdit = new QLineEdit(layoutWidget1);
        minEdit->setObjectName(QString::fromUtf8("minEdit"));

        hboxLayout3->addWidget(minEdit);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hboxLayout3->addWidget(label_4);

        maxEdit = new QLineEdit(layoutWidget1);
        maxEdit->setObjectName(QString::fromUtf8("maxEdit"));

        hboxLayout3->addWidget(maxEdit);


        hboxLayout2->addLayout(hboxLayout3);

        addButton = new QPushButton(layoutWidget1);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setIcon(icon);

        hboxLayout2->addWidget(addButton);

        removeButton = new QPushButton(layoutWidget1);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setIcon(icon);

        hboxLayout2->addWidget(removeButton);

        mColormapTreeWidget = new QTreeWidget(ColourManager);
        mColormapTreeWidget->setObjectName(QString::fromUtf8("mColormapTreeWidget"));
        mColormapTreeWidget->setGeometry(QRect(10, 120, 411, 331));
        mColormapTreeWidget->setColumnCount(3);

        retranslateUi(ColourManager);

        plotTypeCombo->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ColourManager);
    } // setupUi

    void retranslateUi(QWidget *ColourManager)
    {
        ColourManager->setWindowTitle(QApplication::translate("ColourManager", "Colour Manager", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("ColourManager", "Update ", 0, QApplication::UnicodeUTF8));
        mNumberOfEntriesLabel_3->setText(QApplication::translate("ColourManager", "Number of entries:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ColourManager", "Prediction:", 0, QApplication::UnicodeUTF8));
        plotTypeCombo->clear();
        plotTypeCombo->insertItems(0, QStringList()
         << QApplication::translate("ColourManager", "Digital Elevation Model", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Coverage", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Carrier to Co-channel Interference Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Carrier to Adjacent-channel Interf Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Interfered Areas", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Signal to Noise Ratio", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Primary Server", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Secondary Server", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Number of Servers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Primary Co-channel Interferers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Primary Adjacent-channel Interferers", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("ColourManager", "Number of Interferers", 0, QApplication::UnicodeUTF8)
        );
        mClassifyButton->setText(QApplication::translate("ColourManager", "Classify", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ColourManager", "Min:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("ColourManager", "Max:", 0, QApplication::UnicodeUTF8));
        addButton->setText(QString());
        removeButton->setText(QString());
        QTreeWidgetItem *___qtreewidgetitem = mColormapTreeWidget->headerItem();
        ___qtreewidgetitem->setText(2, QApplication::translate("ColourManager", "Label", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(1, QApplication::translate("ColourManager", "Colour", 0, QApplication::UnicodeUTF8));
        ___qtreewidgetitem->setText(0, QApplication::translate("ColourManager", "Value", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ColourManager: public Ui_ColourManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLOURMANAGER_H
