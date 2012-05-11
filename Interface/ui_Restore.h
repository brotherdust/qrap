/********************************************************************************
** Form generated from reading UI file 'Restore.ui'
**
** Created: Wed May 9 18:28:33 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTORE_H
#define UI_RESTORE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RestoreDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *restoreLineEdit;
    QLabel *label;
    QPushButton *restoreChooseFilePushButton;
    QPushButton *restorePushButton;

    void setupUi(QDialog *RestoreDialog)
    {
        if (RestoreDialog->objectName().isEmpty())
            RestoreDialog->setObjectName(QString::fromUtf8("RestoreDialog"));
        RestoreDialog->resize(500, 141);
        RestoreDialog->setMinimumSize(QSize(500, 141));
        RestoreDialog->setMaximumSize(QSize(500, 141));
        buttonBox = new QDialogButtonBox(RestoreDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(140, 90, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        restoreLineEdit = new QLineEdit(RestoreDialog);
        restoreLineEdit->setObjectName(QString::fromUtf8("restoreLineEdit"));
        restoreLineEdit->setGeometry(QRect(70, 10, 311, 24));
        label = new QLabel(RestoreDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 13, 81, 18));
        restoreChooseFilePushButton = new QPushButton(RestoreDialog);
        restoreChooseFilePushButton->setObjectName(QString::fromUtf8("restoreChooseFilePushButton"));
        restoreChooseFilePushButton->setGeometry(QRect(400, 10, 75, 28));
        restorePushButton = new QPushButton(RestoreDialog);
        restorePushButton->setObjectName(QString::fromUtf8("restorePushButton"));
        restorePushButton->setGeometry(QRect(10, 50, 111, 28));

        retranslateUi(RestoreDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), RestoreDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), RestoreDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(RestoreDialog);
    } // setupUi

    void retranslateUi(QDialog *RestoreDialog)
    {
        RestoreDialog->setWindowTitle(QApplication::translate("RestoreDialog", "Restore", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RestoreDialog", "FileName:", 0, QApplication::UnicodeUTF8));
        restoreChooseFilePushButton->setText(QApplication::translate("RestoreDialog", "...", 0, QApplication::UnicodeUTF8));
        restorePushButton->setText(QApplication::translate("RestoreDialog", "Restore", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RestoreDialog: public Ui_RestoreDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTORE_H
