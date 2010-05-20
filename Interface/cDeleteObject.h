 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cDeleteObject.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : (Magdaleen Ballot) (magdaleen.ballot@up.ac.za) 
 *    Description : This is the interface class to delete GUI objects
 *
 **************************************************************************
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

#ifndef CDELETEOBJECT_H_
#define CDELETEOBJECT_H_

#include "ui_DeleteObject.h"
#include <stdio.h>
#include "../DataBase/Config.h"
#include "../Prediction/cGeoP.h"


using namespace std;
using namespace Qrap;

class cDeleteObject: public QDialog, private Ui::deleteDialog
{Q_OBJECT
public:
	cDeleteObject();
	cDeleteObject( QWidget* parent = 0, Qt::WFlags fl = 0);
	virtual ~cDeleteObject();
	
	bool SetParameters(QString Lat,QString Lon,bool site,double scale);
public slots:
    void on_btnOk_clicked();
    void on_btnCancel_clicked();
	
	
private:
	bool mSite;
	int id;
};

#endif /*CDELETEOBJECT_H_*/
