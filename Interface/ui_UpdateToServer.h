/********************************************************************************
** Form generated from reading UI file 'UpdateToServer.ui'
**
** Created: Wed May 9 18:29:22 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPDATETOSERVER_H
#define UI_UPDATETOSERVER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_UpdateToServerDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLabel *label_2;
    QPushButton *sendServerUpdateButton;
    QLineEdit *serverHostNameLineEdit;
    QLineEdit *serverPortNumberLineEdit;
    QPushButton *updateAllPushButton;
    QLabel *label_3;
    QLineEdit *backupFileNameLineEdit;
    QToolButton *backupSelectFileName;
    QCheckBox *filterUpdatesByDateCheckBox;
    QLabel *label_4;
    QLineEdit *dateLineEdit;

    void setupUi(QDialog *UpdateToServerDialog)
    {
        if (UpdateToServerDialog->objectName().isEmpty())
            UpdateToServerDialog->setObjectName(QString::fromUtf8("UpdateToServerDialog"));
        UpdateToServerDialog->resize(453, 200);
        UpdateToServerDialog->setMinimumSize(QSize(453, 200));
        UpdateToServerDialog->setMaximumSize(QSize(453, 200));
        buttonBox = new QDialogButtonBox(UpdateToServerDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 160, 431, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        label = new QLabel(UpdateToServerDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 111, 18));
        label_2 = new QLabel(UpdateToServerDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 50, 111, 18));
        sendServerUpdateButton = new QPushButton(UpdateToServerDialog);
        sendServerUpdateButton->setObjectName(QString::fromUtf8("sendServerUpdateButton"));
        sendServerUpdateButton->setGeometry(QRect(10, 160, 141, 28));
        serverHostNameLineEdit = new QLineEdit(UpdateToServerDialog);
        serverHostNameLineEdit->setObjectName(QString::fromUtf8("serverHostNameLineEdit"));
        serverHostNameLineEdit->setGeometry(QRect(132, 10, 311, 24));
        serverPortNumberLineEdit = new QLineEdit(UpdateToServerDialog);
        serverPortNumberLineEdit->setObjectName(QString::fromUtf8("serverPortNumberLineEdit"));
        serverPortNumberLineEdit->setGeometry(QRect(132, 40, 311, 24));
        updateAllPushButton = new QPushButton(UpdateToServerDialog);
        updateAllPushButton->setObjectName(QString::fromUtf8("updateAllPushButton"));
        updateAllPushButton->setGeometry(QRect(160, 160, 141, 28));
        label_3 = new QLabel(UpdateToServerDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 80, 111, 18));
        backupFileNameLineEdit = new QLineEdit(UpdateToServerDialog);
        backupFileNameLineEdit->setObjectName(QString::fromUtf8("backupFileNameLineEdit"));
        backupFileNameLineEdit->setGeometry(QRect(130, 71, 251, 24));
        backupSelectFileName = new QToolButton(UpdateToServerDialog);
        backupSelectFileName->setObjectName(QString::fromUtf8("backupSelectFileName"));
        backupSelectFileName->setGeometry(QRect(390, 70, 51, 27));
        filterUpdatesByDateCheckBox = new QCheckBox(UpdateToServerDialog);
        filterUpdatesByDateCheckBox->setObjectName(QString::fromUtf8("filterUpdatesByDateCheckBox"));
        filterUpdatesByDateCheckBox->setGeometry(QRect(10, 110, 141, 23));
        label_4 = new QLabel(UpdateToServerDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(160, 114, 54, 18));
        dateLineEdit = new QLineEdit(UpdateToServerDialog);
        dateLineEdit->setObjectName(QString::fromUtf8("dateLineEdit"));
        dateLineEdit->setGeometry(QRect(210, 110, 113, 24));

        retranslateUi(UpdateToServerDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UpdateToServerDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UpdateToServerDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UpdateToServerDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateToServerDialog)
    {
        UpdateToServerDialog->setWindowTitle(QApplication::translate("UpdateToServerDialog", "Update To Server", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("UpdateToServerDialog", "Server Hostname:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("UpdateToServerDialog", "Server Port Number:", 0, QApplication::UnicodeUTF8));
        sendServerUpdateButton->setText(QApplication::translate("UpdateToServerDialog", "Update To Server", 0, QApplication::UnicodeUTF8));
        serverHostNameLineEdit->setText(QApplication::translate("UpdateToServerDialog", "localhost", 0, QApplication::UnicodeUTF8));
        serverPortNumberLineEdit->setText(QApplication::translate("UpdateToServerDialog", "10000", 0, QApplication::UnicodeUTF8));
        updateAllPushButton->setText(QApplication::translate("UpdateToServerDialog", "Get Host Identifier", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("UpdateToServerDialog", "Backup File Name:", 0, QApplication::UnicodeUTF8));
        backupFileNameLineEdit->setText(QApplication::translate("UpdateToServerDialog", "/home/backup.txd", 0, QApplication::UnicodeUTF8));
        backupSelectFileName->setText(QApplication::translate("UpdateToServerDialog", "...", 0, QApplication::UnicodeUTF8));
        filterUpdatesByDateCheckBox->setText(QApplication::translate("UpdateToServerDialog", "Filter Updates by Date", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("UpdateToServerDialog", "Date", 0, QApplication::UnicodeUTF8));
        dateLineEdit->setText(QApplication::translate("UpdateToServerDialog", "2008-04-01", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UpdateToServerDialog: public Ui_UpdateToServerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPDATETOSERVER_H
