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

#include <QString>
#include <QObject>
#include <QMouseEvent>
#include <QSettings>
#include <QCursor>
#include <QPixmap>
#include <QPointer>
#include <qgsmaptool.h>
#include <qgsmapcanvas.h>
#include <qgsmaptopixel.h> 
#include <qgsmaptoolemitpoint.h> 

//class QgsMapLayer;
//class QgsMapCanvas;
//class QMouseEvent;
//class QgsPoint;
//class QgsMapTool;


	class MouseEvents  : public QgsMapTool
	{ Q_OBJECT
	public:
		explicit MouseEvents(QgsMapCanvas* canvas);
		virtual ~MouseEvents();
	public slots:
    	//! Overridden mouse move event
    	virtual void canvasMoveEvent( QgsMapMouseEvent* e ) override;

    	//! Overridden mouse press event
    	virtual void canvasPressEvent( QgsMapMouseEvent* e ) override;

    	//! Overridden mouse release event
    	virtual void canvasReleaseEvent( QgsMapMouseEvent* e ) override;

    	//! called when map tool is being deactivated
    	virtual void deactivate() override;
	
	signals:
		void RightPoint(QgsPoint &Point);
		void LeftPoint(QgsPoint &Point);
		void MouseMove(QgsPoint &Point);
	
	
	private:
	//! pointer to map canvas
    	//QgsMapCanvas* mCanvas;
	QPointer<QgsMapCanvas> mCanvas;
    
   	 //! cursor used in map tool
    	//QCursor mCursor;
    
	};


#endif /*MOUSEEVENTS_H_*/ 
