/********************************************************************************
** Form generated from reading ui file 'LoadFiles.ui'
**
** Created: Tue Jun 17 13:12:30 2008
**      by: Qt User Interface Compiler version 4.3.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_LOADFILES_H
#define UI_LOADFILES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QWidget>

class Ui_LoadFiles
{
public:

    void setupUi(QWidget *LoadFiles)
    {
    if (LoadFiles->objectName().isEmpty())
        LoadFiles->setObjectName(QString::fromUtf8("LoadFiles"));
    LoadFiles->resize(517, 300);

    retranslateUi(LoadFiles);

    QMetaObject::connectSlotsByName(LoadFiles);
    } // setupUi

    void retranslateUi(QWidget *LoadFiles)
    {
    LoadFiles->setWindowTitle(QApplication::translate("LoadFiles", "Form", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(LoadFiles);
    } // retranslateUi

};

namespace Ui {
    class LoadFiles: public Ui_LoadFiles {};
} // namespace Ui

#endif // UI_LOADFILES_H
