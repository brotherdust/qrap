/********************************************************************************
** Form generated from reading UI file 'ImportExportMenu.ui'
**
** Created: Wed May 9 18:27:03 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTEXPORTMENU_H
#define UI_IMPORTEXPORTMENU_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImportExportMenuDialog
{
public:
    QWidget *layoutWidget;
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
        ImportExportMenuDialog->resize(190, 320);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImportExportMenuDialog->sizePolicy().hasHeightForWidth());
        ImportExportMenuDialog->setSizePolicy(sizePolicy);
        ImportExportMenuDialog->setMinimumSize(QSize(164, 320));
        ImportExportMenuDialog->setMaximumSize(QSize(190, 320));
        ImportExportMenuDialog->setModal(true);
        layoutWidget = new QWidget(ImportExportMenuDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 172, 291));
        vboxLayout = new QVBoxLayout(layoutWidget);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        createBackupFilePushButton = new QPushButton(layoutWidget);
        createBackupFilePushButton->setObjectName(QString::fromUtf8("createBackupFilePushButton"));

        vboxLayout->addWidget(createBackupFilePushButton);

        restoreBackupPushButton = new QPushButton(layoutWidget);
        restoreBackupPushButton->setObjectName(QString::fromUtf8("restoreBackupPushButton"));

        vboxLayout->addWidget(restoreBackupPushButton);

        createExportFilePushButton = new QPushButton(layoutWidget);
        createExportFilePushButton->setObjectName(QString::fromUtf8("createExportFilePushButton"));

        vboxLayout->addWidget(createExportFilePushButton);

        importFilePushButton = new QPushButton(layoutWidget);
        importFilePushButton->setObjectName(QString::fromUtf8("importFilePushButton"));

        vboxLayout->addWidget(importFilePushButton);

        updateToServerPushButton = new QPushButton(layoutWidget);
        updateToServerPushButton->setObjectName(QString::fromUtf8("updateToServerPushButton"));

        vboxLayout->addWidget(updateToServerPushButton);

        runServerPushButton = new QPushButton(layoutWidget);
        runServerPushButton->setObjectName(QString::fromUtf8("runServerPushButton"));

        vboxLayout->addWidget(runServerPushButton);

        helpPushButton = new QPushButton(layoutWidget);
        helpPushButton->setObjectName(QString::fromUtf8("helpPushButton"));

        vboxLayout->addWidget(helpPushButton);

        buttonBox = new QDialogButtonBox(layoutWidget);
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
        ImportExportMenuDialog->setWindowTitle(QApplication::translate("ImportExportMenuDialog", "Q-Rap Import Export Actions", 0, QApplication::UnicodeUTF8));
        createBackupFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Create Backup File...", 0, QApplication::UnicodeUTF8));
        restoreBackupPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Restore from Backup...", 0, QApplication::UnicodeUTF8));
        createExportFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Create Export File...", 0, QApplication::UnicodeUTF8));
        importFilePushButton->setText(QApplication::translate("ImportExportMenuDialog", "Import File...", 0, QApplication::UnicodeUTF8));
        updateToServerPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Update To Server...", 0, QApplication::UnicodeUTF8));
        runServerPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Run Server...", 0, QApplication::UnicodeUTF8));
        helpPushButton->setText(QApplication::translate("ImportExportMenuDialog", "Help...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImportExportMenuDialog: public Ui_ImportExportMenuDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTEXPORTMENU_H
