/********************************************************************************
** Form generated from reading ui file 'ImportUpdateFile.ui'
**
** Created: Sun Jul 27 19:39:22 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IMPORTUPDATEFILE_H
#define UI_IMPORTUPDATEFILE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

class Ui_ImportUpdateFileDialog
{
public:
    QDialogButtonBox *buttonBox;
    QPushButton *fileSelectionPushButton;
    QLineEdit *fileNameLineEdit;
    QLabel *label;
    QPushButton *importFilePushButton;

    void setupUi(QDialog *ImportUpdateFileDialog)
    {
    if (ImportUpdateFileDialog->objectName().isEmpty())
        ImportUpdateFileDialog->setObjectName(QString::fromUtf8("ImportUpdateFileDialog"));
    ImportUpdateFileDialog->resize(475, 133);
    ImportUpdateFileDialog->setMinimumSize(QSize(475, 133));
    ImportUpdateFileDialog->setMaximumSize(QSize(475, 133));
    buttonBox = new QDialogButtonBox(ImportUpdateFileDialog);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(130, 90, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);
    fileSelectionPushButton = new QPushButton(ImportUpdateFileDialog);
    fileSelectionPushButton->setObjectName(QString::fromUtf8("fileSelectionPushButton"));
    fileSelectionPushButton->setGeometry(QRect(390, 10, 80, 27));
    fileNameLineEdit = new QLineEdit(ImportUpdateFileDialog);
    fileNameLineEdit->setObjectName(QString::fromUtf8("fileNameLineEdit"));
    fileNameLineEdit->setGeometry(QRect(80, 10, 291, 27));
    label = new QLabel(ImportUpdateFileDialog);
    label->setObjectName(QString::fromUtf8("label"));
    label->setGeometry(QRect(10, 10, 91, 18));
    importFilePushButton = new QPushButton(ImportUpdateFileDialog);
    importFilePushButton->setObjectName(QString::fromUtf8("importFilePushButton"));
    importFilePushButton->setGeometry(QRect(170, 50, 161, 28));

    retranslateUi(ImportUpdateFileDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), ImportUpdateFileDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), ImportUpdateFileDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(ImportUpdateFileDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportUpdateFileDialog)
    {
    ImportUpdateFileDialog->setWindowTitle(QApplication::translate("ImportUpdateFileDialog", "Import Update File", 0, QApplication::UnicodeUTF8));
    fileSelectionPushButton->setText(QApplication::translate("ImportUpdateFileDialog", "...", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("ImportUpdateFileDialog", "File Name", 0, QApplication::UnicodeUTF8));
    importFilePushButton->setText(QApplication::translate("ImportUpdateFileDialog", "Import File", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ImportUpdateFileDialog);
    } // retranslateUi

};

namespace Ui {
    class ImportUpdateFileDialog: public Ui_ImportUpdateFileDialog {};
} // namespace Ui

#endif // UI_IMPORTUPDATEFILE_H
