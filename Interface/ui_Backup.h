/********************************************************************************
** Form generated from reading UI file 'Backup.ui'
**
** Created: Wed May 9 18:24:06 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BACKUP_H
#define UI_BACKUP_H

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

class Ui_BackupDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *backupLineEdit;
    QPushButton *backupPushButton;
    QLabel *label;
    QPushButton *backupChooseFilePushButton;

    void setupUi(QDialog *BackupDialog)
    {
        if (BackupDialog->objectName().isEmpty())
            BackupDialog->setObjectName(QString::fromUtf8("BackupDialog"));
        BackupDialog->setWindowModality(Qt::ApplicationModal);
        BackupDialog->resize(513, 122);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BackupDialog->sizePolicy().hasHeightForWidth());
        BackupDialog->setSizePolicy(sizePolicy);
        BackupDialog->setMinimumSize(QSize(513, 0));
        BackupDialog->setMaximumSize(QSize(513, 122));
        BackupDialog->setModal(true);
        buttonBox = new QDialogButtonBox(BackupDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(280, 70, 221, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        backupLineEdit = new QLineEdit(BackupDialog);
        backupLineEdit->setObjectName(QString::fromUtf8("backupLineEdit"));
        backupLineEdit->setGeometry(QRect(80, 10, 321, 27));
        backupLineEdit->setReadOnly(true);
        backupPushButton = new QPushButton(BackupDialog);
        backupPushButton->setObjectName(QString::fromUtf8("backupPushButton"));
        backupPushButton->setGeometry(QRect(10, 50, 151, 28));
        label = new QLabel(BackupDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 57, 18));
        backupChooseFilePushButton = new QPushButton(BackupDialog);
        backupChooseFilePushButton->setObjectName(QString::fromUtf8("backupChooseFilePushButton"));
        backupChooseFilePushButton->setGeometry(QRect(410, 10, 80, 27));

        retranslateUi(BackupDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), BackupDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BackupDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(BackupDialog);
    } // setupUi

    void retranslateUi(QDialog *BackupDialog)
    {
        BackupDialog->setWindowTitle(QApplication::translate("BackupDialog", "Q-Rap Backup ", 0, QApplication::UnicodeUTF8));
        backupLineEdit->setText(QApplication::translate("BackupDialog", "/", 0, QApplication::UnicodeUTF8));
        backupPushButton->setText(QApplication::translate("BackupDialog", "Backup", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("BackupDialog", "Backup", 0, QApplication::UnicodeUTF8));
        backupChooseFilePushButton->setText(QApplication::translate("BackupDialog", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BackupDialog: public Ui_BackupDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BACKUP_H
