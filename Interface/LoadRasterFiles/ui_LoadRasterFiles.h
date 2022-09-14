/********************************************************************************
** Form generated from reading ui file 'LoadRasterFiles.ui'
**
** Created: Wed Jul 23 01:10:08 2008
**      by: Qt User Interface Compiler version 4.3.4
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOADRASTERFILES_H
#define UI_LOADRASTERFILES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QTreeWidget>

class Ui_LoadRasterFiles
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *labelFeedback;
    QProgressBar *progressBar;
    QTreeWidget *treeWidget;
    QGroupBox *widgetContainer;

    void setupUi(QDialog *LoadRasterFiles)
    {
    if (LoadRasterFiles->objectName().isEmpty())
        LoadRasterFiles->setObjectName(QString::fromUtf8("LoadRasterFiles"));
    LoadRasterFiles->setWindowModality(Qt::NonModal);
    LoadRasterFiles->resize(1051, 489);
    LoadRasterFiles->setWindowIcon(QIcon(QString::fromUtf8(":/images/logo_icon.png")));
    buttonBox = new QDialogButtonBox(LoadRasterFiles);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(700, 450, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);
    labelFeedback = new QLabel(LoadRasterFiles);
    labelFeedback->setObjectName(QString::fromUtf8("labelFeedback"));
    labelFeedback->setGeometry(QRect(250, 410, 471, 18));
    progressBar = new QProgressBar(LoadRasterFiles);
    progressBar->setObjectName(QString::fromUtf8("progressBar"));
    progressBar->setGeometry(QRect(10, 460, 231, 23));
    progressBar->setValue(0);
    treeWidget = new QTreeWidget(LoadRasterFiles);
    treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
    treeWidget->setGeometry(QRect(15, 5, 216, 441));
    widgetContainer = new QGroupBox(LoadRasterFiles);
    widgetContainer->setObjectName(QString::fromUtf8("widgetContainer"));
    widgetContainer->setGeometry(QRect(235, 5, 811, 441));

    retranslateUi(LoadRasterFiles);
    QObject::connect(buttonBox, SIGNAL(accepted()), LoadRasterFiles, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), LoadRasterFiles, SLOT(reject()));

    QMetaObject::connectSlotsByName(LoadRasterFiles);
    } // setupUi

    void retranslateUi(QDialog *LoadRasterFiles)
    {
    LoadRasterFiles->setWindowTitle(QApplication::translate("LoadRasterFiles", "Load Raster Files", 0, QApplication::UnicodeUTF8));
    labelFeedback->setText(QString());
    treeWidget->headerItem()->setText(0, QApplication::translate("LoadRasterFiles", "Raster File Operations", 0, QApplication::UnicodeUTF8));
    treeWidget->clear();

    QTreeWidgetItem *__item = new QTreeWidgetItem(treeWidget);
    __item->setText(0, QApplication::translate("LoadRasterFiles", "Load Files into the Database", 0, QApplication::UnicodeUTF8));
    __item->setIcon(0, QIcon(QString::fromUtf8(":/images/add.png")));

    QTreeWidgetItem *__item1 = new QTreeWidgetItem(treeWidget);
    __item1->setText(0, QApplication::translate("LoadRasterFiles", "Cut a File Set", 0, QApplication::UnicodeUTF8));
    __item1->setIcon(0, QIcon(QString::fromUtf8(":/images/cut.png")));

    QTreeWidgetItem *__item2 = new QTreeWidgetItem(treeWidget);
    __item2->setText(0, QApplication::translate("LoadRasterFiles", "Set File Set Usage Order", 0, QApplication::UnicodeUTF8));
    __item2->setIcon(0, QIcon(QString::fromUtf8(":/images/dbgrun.png")));

    QTreeWidgetItem *__item3 = new QTreeWidgetItem(treeWidget);
    __item3->setText(0, QApplication::translate("LoadRasterFiles", "Create a new File Set", 0, QApplication::UnicodeUTF8));
    __item3->setIcon(0, QIcon(QString::fromUtf8(":/images/new.png")));
    widgetContainer->setTitle(QString());
    Q_UNUSED(LoadRasterFiles);
    } // retranslateUi

};

namespace Ui {
    class LoadRasterFiles: public Ui_LoadRasterFiles {};
} // namespace Ui

#endif // UI_LOADRASTERFILES_H
