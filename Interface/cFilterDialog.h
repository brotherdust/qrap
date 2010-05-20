 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cFilterDialog.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe (magdaleen.ballot@up.ac.za) 
 *    Description : This is the interface class to create Custom Filter Areas
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

#ifndef CFILTERDIALOG_H_
#define CFILTERDIALOG_H_

#include "ui_FilterDialog.h"
#include "../DataBase/Config.h"
#include <stdio.h>
#include <qgspoint.h>
#include "../DataBase/MainWindow.h"

using namespace std;
namespace Qrap
{

	class cFilterDialog:public QDialog, private Ui::FilterDialog
	{Q_OBJECT
		public:
			cFilterDialog();
			cFilterDialog( QWidget* parent = 0, Qt::WFlags fl = 0 );
			virtual ~cFilterDialog();
			
			void SetPoints(QList<QgsPoint> Points);
		
		protected:
     		void accept();
			
		private:
			void Setup();
			QList<QgsPoint> mPoints;
			
		
	};
}
#endif /*CFILTERDIALOG_H_*/
