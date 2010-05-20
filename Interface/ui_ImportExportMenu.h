/********************************************************************************
** Form generated from reading ui file 'ImportExportMenu.ui'
**
** Created: Sun Jul 27 23:49:05 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_IMPORTEXPORTMENU_H
#define UI_IMPORTEXPORTMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

class Ui_ImportExportMenuDialog
{
public:
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    QPushButton *createBackupFilePushButton;
    QPushButton *restoreBackupPushButton;
    QPushButton *createExportFilePushButton;
    QPushButton *importFilePushButton;
    QPushButton *updateToServerPushButton;
    QPushButton *runServerPushButton;
    QPushButton *helpPushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ImportExportMenuDialog)
    {
    if (ImportExportMenuDialog->objectName().isEmpty())
        ImportExportMenuDialog->setObjectName(QString::fromUtf8("ImportExportMenuDialog"));
    ImportExportMenuDialog->setWindowModality(Qt::ApplicationModal);
    ImportExportMenuDialog->resize(163, 320);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(ImportExportMenuDialog->sizePolicy().hasHeightForWidth());
    ImportExportMenuDialog->setSizePolicy(sizePolicy);
    ImportExportMenuDialog->setMinimumSize(QSize(163, 320));
    ImportExportMenuDialog->setMaximumSize(QSize(163, 320));
    ImportExportMenuDialog->setModal(true);
    widget = new QWidget(ImportExportMenuDialog);
    widget->setObjectName(QString::fromUtf8("widget"));
    widget->setGeometry(QRect(10, 10, 135, 291));
    vboxLayout = new QVBoxLayout(widget);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    createBackupFilePushButton = new QPushButton(widget);
    createBackupFilePushButton->setObjectName(QString::fromUtf8("createBackupFilePushButton"));

    vboxLayout->addWidget(createBackupFilePushButton);

    restoreBackupPushButton = new QPushButton(widget);
    restoreBackupPushButton->setObjectName(QString::fromUtf8("restoreBackupPushButton"));

    vboxLayout->addWidget(restoreBackupPushButton);

    createExportFilePushButton = new QPushButton(widget);
    createExportFilePushButton->setObjectName(QString::fromUtf8("createExportFilePushButton"));

    vboxLayout->addWidget(createExportFilePushButton);

    importFilePushButton = new QPushButton(widget);
    importFilePushButton->setObjectName(QString::fromUtf8("importFilePushButton"));

    vboxLayout->addWidget(importFilePushButton);

    updateToServerPushButton = new QPushButton(widget);
    updateToServerPushButton->setObjectName(QString::fromUtf8("updateToServerPushButton"));

    vboxLayout->addWidget(updateToServerPushButton);

    runServerPushButton = new QPushButton(widget);
    runServerPushButton->setObjectName(QString::fromUtf8("runServerPushButton"));

    vboxLayout->addWidget(runServerPushButton);

    helpPushButton = new QPushButton(widget);
    helpPushButton->setObjectName(QString::fromUtf8("helpPushButton"));

    vboxLayout->addWidget(helpPushButton);

    buttonBox = new QDialogButtonBox(widget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok);

    vboxLayout->addWidget(buttonBox);


    retranslateUi(ImportExportMenuDialog);
    QObject::connect(buttonBox, SIGNAL(accepted()), ImportExportMenuDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), ImportExportMenuDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(ImportExportMenuDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportExportMenuDialog)
    {
    ImportExportMenuDialog->setWindowTitle(QApplication::translate("ImportExportMenuDialog", "Import Export Actions", 0, QApplication::UnicodeUTF8));
    createBackupFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Create Backup File...", 0, QApplication::UnicodeUTF8));
    restoreBackupPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Restore from Backup...", 0, QApplication::UnicodeUTF8));
    createExportFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Create Export File...", 0, QApplication::UnicodeUTF8));
    importFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Import File...", 0, QApplication::UnicodeUTF8));
    updateToServerPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Update To Server...", 0, QApplication::UnicodeUTF8));
    runServerPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Run Server...", 0, QApplication::UnicodeUTF8));
    helpPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Help...", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ImportExportMenuDialog);
    } // retranslateUi

};

namespace Ui {
    class ImportExportMenuDialog: public Ui_ImportExportMenuDialog {};
} // namespace Ui

#endif // UI_IMPORTEXPORTMENU_H
