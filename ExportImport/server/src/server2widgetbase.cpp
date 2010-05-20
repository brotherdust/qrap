 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : server2widgetbase.cpp
 *    Copyright   : (c) University of Pretoria
 *    Authors     : Christo (magdaleen.ballot@up.ac.za) 
 *    Description : Not really ready to be used
 *
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#include <kdialog.h>
#include <klocale.h>
/****************************************************************************
** Form implementation generated from reading ui file './server2widgetbase.ui'
**
** Created: Thu Jul 24 14:30:29 2008
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "server2widgetbase.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

/*
 *  Constructs a server2WidgetBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
server2WidgetBase::server2WidgetBase( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "server2widgetbase" );
    setMinimumSize( QSize( 400, 350 ) );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setGeometry( QRect( 0, 30, 130, 31 ) );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );
    textLabel1_2->setGeometry( QRect( 0, 120, 121, 20 ) );

    textLabel2_2 = new QLabel( this, "textLabel2_2" );
    textLabel2_2->setGeometry( QRect( 0, 160, 110, 20 ) );

    textLabel3 = new QLabel( this, "textLabel3" );
    textLabel3->setGeometry( QRect( 0, 200, 113, 20 ) );

    textLabel4 = new QLabel( this, "textLabel4" );
    textLabel4->setGeometry( QRect( 0, 240, 71, 20 ) );

    stopServerPushButton = new QPushButton( this, "stopServerPushButton" );
    stopServerPushButton->setGeometry( QRect( 180, 290, 151, 41 ) );

    startServerButton = new QPushButton( this, "startServerButton" );
    startServerButton->setGeometry( QRect( 10, 290, 131, 41 ) );

    textLabel2 = new QLabel( this, "textLabel2" );
    textLabel2->setGeometry( QRect( 0, 80, 101, 21 ) );

    portNumberLineEdit = new QLineEdit( this, "portNumberLineEdit" );
    portNumberLineEdit->setGeometry( QRect( 130, 70, 240, 31 ) );

    workingDirectoryLineEdit = new QLineEdit( this, "workingDirectoryLineEdit" );
    workingDirectoryLineEdit->setGeometry( QRect( 130, 30, 240, 31 ) );

    serverUserNameLineEdit = new QLineEdit( this, "serverUserNameLineEdit" );
    serverUserNameLineEdit->setGeometry( QRect( 130, 120, 240, 24 ) );

    serverPasswordLineEdit = new QLineEdit( this, "serverPasswordLineEdit" );
    serverPasswordLineEdit->setGeometry( QRect( 130, 160, 240, 24 ) );

    serverHostnameLineEdit = new QLineEdit( this, "serverHostnameLineEdit" );
    serverHostnameLineEdit->setGeometry( QRect( 130, 200, 240, 24 ) );

    serverDBNameLineEdit = new QLineEdit( this, "serverDBNameLineEdit" );
    serverDBNameLineEdit->setGeometry( QRect( 130, 240, 240, 24 ) );

    textLabel1_3 = new QLabel( this, "textLabel1_3" );
    textLabel1_3->setGeometry( QRect( 10, 340, 360, 60 ) );
    textLabel1_3->setAlignment( int( QLabel::WordBreak | QLabel::AlignVCenter ) );
    languageChange();
    resize( QSize(431, 405).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( startServerButton, SIGNAL( clicked() ), this, SLOT( startServerSlot() ) );
    connect( stopServerPushButton, SIGNAL( clicked() ), this, SLOT( stopServerSlot() ) );

    // tab order
    setTabOrder( workingDirectoryLineEdit, portNumberLineEdit );
    setTabOrder( portNumberLineEdit, serverUserNameLineEdit );
    setTabOrder( serverUserNameLineEdit, serverPasswordLineEdit );
    setTabOrder( serverPasswordLineEdit, serverHostnameLineEdit );
    setTabOrder( serverHostnameLineEdit, serverDBNameLineEdit );
    setTabOrder( serverDBNameLineEdit, startServerButton );
    setTabOrder( startServerButton, stopServerPushButton );
}

/*
 *  Destroys the object and frees any allocated resources
 */
server2WidgetBase::~server2WidgetBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void server2WidgetBase::languageChange()
{
    setCaption( tr2i18n( "server2widgetbase" ) );
    textLabel1->setText( tr2i18n( "Working Directory" ) );
    textLabel1_2->setText( tr2i18n( "Server User Name" ) );
    textLabel2_2->setText( tr2i18n( "Server password" ) );
    textLabel3->setText( tr2i18n( "Server hostname" ) );
    textLabel4->setText( tr2i18n( "DB name" ) );
    stopServerPushButton->setText( tr2i18n( "St&op Server" ) );
    stopServerPushButton->setAccel( QKeySequence( tr2i18n( "Alt+O" ) ) );
    startServerButton->setText( tr2i18n( "St&art Server..." ) );
    startServerButton->setAccel( QKeySequence( tr2i18n( "Alt+A" ) ) );
    textLabel2->setText( tr2i18n( "Port Number" ) );
    portNumberLineEdit->setText( tr2i18n( "20000" ) );
    workingDirectoryLineEdit->setText( tr2i18n( "/home/christo/Documents/qrap/" ) );
    serverUserNameLineEdit->setText( tr2i18n( "postgres" ) );
    serverPasswordLineEdit->setText( tr2i18n( "postqrap" ) );
    serverHostnameLineEdit->setText( tr2i18n( "localhost" ) );
    serverDBNameLineEdit->setText( tr2i18n( "qrap" ) );
    textLabel1_3->setText( tr2i18n( "NB: Please ensure that the database user has read and write priviliges for the working directory." ) );
}

void server2WidgetBase::button_clicked()
{
    qWarning( "server2WidgetBase::button_clicked(): Not implemented yet" );
}

void server2WidgetBase::startServerSlot()
{
    qWarning( "server2WidgetBase::startServerSlot(): Not implemented yet" );
}

void server2WidgetBase::stopServerSlot()
{
    qWarning( "server2WidgetBase::stopServerSlot(): Not implemented yet" );
}

#include "server2widgetbase.moc"
