/********************************************************************************
** Form generated from reading ui file 'FilterDialog.ui'
**
** Created: Tue Jul 15 07:51:54 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FILTERDIALOG_H
#define UI_FILTERDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

class Ui_FilterDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEditName;

    void setupUi(QDialog *FilterDialog)
    {
    if (FilterDialog->objectName().isEmpty())
        FilterDialog->setObjectName(QString::fromUtf8("FilterDialog"));
    FilterDialog->resize(400, 160);
    buttonBox = new QDialogButtonBox(FilterDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(40, 120, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    label = new QLabel(FilterDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(30, 50, 91, 18));
    lineEditName = new QLineEdit(FilterDialog);
    lineEditName->setObjectName(QString::fromUtf8("lineEditName"));
    lineEditName->setGeometry(QRect(110, 40, 261, 27));

    retranslateUi(FilterDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), FilterDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), FilterDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(FilterDialog);
    } // setupUi

    void retranslateUi(QDialog *FilterDialog)
    {
    FilterDialog->setWindowTitle(QApplication::translate("FilterDialog", "New Area Filter", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("FilterDialog", "Area Name:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(FilterDialog);
    } // retranslateUi

};

namespace Ui {
    class FilterDialog: public Ui_FilterDialog {};
} // namespace Ui

#endif // UI_FILTERDIALOG_H
