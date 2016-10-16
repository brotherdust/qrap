 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cMouseEvents.h
 *    Copyright   : University of Pretoria
 *    Author      : Dirk van der Merwe 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This probably come from QGIS ... sorry maggie doesn't 
 *                  know true origin
 *
 **************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef MOUSEEVENTS_H_
#define MOUSEEVENTS_H_

#include <QCursor>
#include <QString>
#include <QObject>
#include <QMouseEvent>
#include <qgsmaptool.h>
#include <qgsmapcanvas.h>
#include <qgsmaptopixel.h> 
#include <qgsmaptoolemitpoint.h> 

class QgsMapLayer;
class QgsMapCanvas;
class QMouseEvent;
class QgsPoint;
class QgsMapTool;


	class MouseEvents  : public QgsMapToolEmitPoint
	{ Q_OBJECT
	public:
		MouseEvents(QgsMapCanvas* canvas);
		virtual ~MouseEvents();
	public slots:
		virtual void canvasReleaseEvent(QMouseEvent * e); 
		virtual void canvasMoveEvent(QMouseEvent * e);
	
	signals:
		void RightPoint(QgsPoint &Point);
		void LeftPoint(QgsPoint &Point);
		void MouseMove(QgsPoint &Point);
	
	
	private:
	//! pointer to map canvas
    	QgsMapCanvas* mCanvas;
    
   	 //! cursor used in map tool
    	QCursor mCursor;
    
	};


#endif /*MOUSEEVENTS_H_*/ 
