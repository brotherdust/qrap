

/****************************************************************************
** Form interface generated from reading ui file './server2widgetbase.ui'
**
** Created: Thu Jul 24 14:30:27 2008
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SERVER2WIDGETBASE_H
#define SERVER2WIDGETBASE_H

#include <qvariant.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;
class QLineEdit;

class server2WidgetBase : public QWidget
{
    Q_OBJECT

public:
    server2WidgetBase( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~server2WidgetBase();

    QLabel* textLabel1;
    QLabel* textLabel1_2;
    QLabel* textLabel2_2;
    QLabel* textLabel3;
    QLabel* textLabel4;
    QPushButton* stopServerPushButton;
    QPushButton* startServerButton;
    QLabel* textLabel2;
    QLineEdit* portNumberLineEdit;
    QLineEdit* workingDirectoryLineEdit;
    QLineEdit* serverUserNameLineEdit;
    QLineEdit* serverPasswordLineEdit;
    QLineEdit* serverHostnameLineEdit;
    QLineEdit* serverDBNameLineEdit;
    QLabel* textLabel1_3;

public slots:
    virtual void button_clicked();
    virtual void startServerSlot();
    virtual void stopServerSlot();

protected:

protected slots:
    virtual void languageChange();

};

#endif // SERVER2WIDGETBASE_H
