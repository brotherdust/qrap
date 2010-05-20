 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : server2widget.h
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

#ifndef _SERVER2WIDGET_H_
#define _SERVER2WIDGET_H_

#include "server2widgetbase.h"

class server2Widget : public server2WidgetBase
{
    Q_OBJECT

public:
    server2Widget(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~server2Widget();
    /*$PUBLIC_FUNCTIONS$*/

public slots:
    /*$PUBLIC_SLOTS$*/
    virtual void button_clicked();
    virtual void stopServerSlot();
    virtual void startServerSlot();

protected:
    /*$PROTECTED_FUNCTIONS$*/
	bool started;
	FServer* fserver;

protected slots:
    /*$PROTECTED_SLOTS$*/

};

#endif

