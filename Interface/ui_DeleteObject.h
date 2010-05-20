/********************************************************************************
** Form generated from reading ui file 'DeleteObject.ui'
**
** Created: Sat Oct 3 12:14:09 2009
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DELETEOBJECT_H
#define UI_DELETEOBJECT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_deleteDialog
{
public:
    QLabel *label;
    QWidget *horizontalLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *btnCancel;
    QPushButton *btnOk;

    void setupUi(QDialog *deleteDialog)
    {
        if (deleteDialog->objectName().isEmpty())
            deleteDialog->setObjectName(QString::fromUtf8("deleteDialog"));
        deleteDialog->resize(400, 94);
        const QIcon icon = QIcon(QString::fromUtf8(":/images/logo_icon.png"));
        deleteDialog->setWindowIcon(icon);
        label = new QLabel(deleteDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 371, 18));
        horizontalLayout = new QWidget(deleteDialog);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setGeometry(QRect(79, 50, 311, 31));
        hboxLayout = new QHBoxLayout(horizontalLayout);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        btnCancel = new QPushButton(horizontalLayout);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        hboxLayout->addWidget(btnCancel);

        btnOk = new QPushButton(horizontalLayout);
        btnOk->setObjectName(QString::fromUtf8("btnOk"));

        hboxLayout->addWidget(btnOk);


        retranslateUi(deleteDialog);

        QMetaObject::connectSlotsByName(deleteDialog);
    } // setupUi

    void retranslateUi(QDialog *deleteDialog)
    {
        deleteDialog->setWindowTitle(QApplication::translate("deleteDialog", "Delete an Object", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("deleteDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("deleteDialog", "Cancel", 0, QApplication::UnicodeUTF8));
        btnOk->setText(QApplication::translate("deleteDialog", "Delete", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(deleteDialog);
    } // retranslateUi

};

namespace Ui {
    class deleteDialog: public Ui_deleteDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETEOBJECT_H
