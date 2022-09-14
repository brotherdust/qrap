/********************************************************************************
** Form generated from reading UI file 'CreateUpdateFile.ui'
**
** Created: Wed May 9 18:25:51 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEUPDATEFILE_H
#define UI_CREATEUPDATEFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_CreateUpdateFileDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *updateFileLineEdit;
    QPushButton *selectFileNamePushButton;
    QLabel *label;
    QPushButton *createUpdateFilePushButton;
    QCheckBox *fullUpdateFileCheckBox;

    void setupUi(QDialog *CreateUpdateFileDialog)
    {
        if (CreateUpdateFileDialog->objectName().isEmpty())
            CreateUpdateFileDialog->setObjectName(QString::fromUtf8("CreateUpdateFileDialog"));
        CreateUpdateFileDialog->resize(520, 87);
        CreateUpdateFileDialog->setMinimumSize(QSize(400, 50));
        CreateUpdateFileDialog->setMaximumSize(QSize(1000, 200));
        buttonBox = new QDialogButtonBox(CreateUpdateFileDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(430, 50, 81, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);
        updateFileLineEdit = new QLineEdit(CreateUpdateFileDialog);
        updateFileLineEdit->setObjectName(QString::fromUtf8("updateFileLineEdit"));
        updateFileLineEdit->setGeometry(QRect(80, 10, 341, 27));
        selectFileNamePushButton = new QPushButton(CreateUpdateFileDialog);
        selectFileNamePushButton->setObjectName(QString::fromUtf8("selectFileNamePushButton"));
        selectFileNamePushButton->setGeometry(QRect(430, 10, 80, 27));
        label = new QLabel(CreateUpdateFileDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 81, 18));
        createUpdateFilePushButton = new QPushButton(CreateUpdateFileDialog);
        createUpdateFilePushButton->setObjectName(QString::fromUtf8("createUpdateFilePushButton"));
        createUpdateFilePushButton->setGeometry(QRect(160, 50, 161, 27));
        fullUpdateFileCheckBox = new QCheckBox(CreateUpdateFileDialog);
        fullUpdateFileCheckBox->setObjectName(QString::fromUtf8("fullUpdateFileCheckBox"));
        fullUpdateFileCheckBox->setGeometry(QRect(10, 50, 181, 23));

        retranslateUi(CreateUpdateFileDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), CreateUpdateFileDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CreateUpdateFileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(CreateUpdateFileDialog);
    } // setupUi

    void retranslateUi(QDialog *CreateUpdateFileDialog)
    {
        CreateUpdateFileDialog->setWindowTitle(QApplication::translate("CreateUpdateFileDialog", "Q-Rap Create Update File", 0, QApplication::UnicodeUTF8));
        selectFileNamePushButton->setText(QApplication::translate("CreateUpdateFileDialog", "...", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CreateUpdateFileDialog", "File Name:", 0, QApplication::UnicodeUTF8));
        createUpdateFilePushButton->setText(QApplication::translate("CreateUpdateFileDialog", "Create Update File", 0, QApplication::UnicodeUTF8));
        fullUpdateFileCheckBox->setText(QApplication::translate("CreateUpdateFileDialog", "Full update file", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CreateUpdateFileDialog: public Ui_CreateUpdateFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEUPDATEFILE_H
