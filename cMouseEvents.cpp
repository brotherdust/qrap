 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cMouseEvents.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This probably come from QGIS ... sorry Maggie doesn't 
 *                  know true origin
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
#include "cMouseEvents.h"

MouseEvents::MouseEvents(QgsMapCanvas* canvas)
  : QgsMapToolEmitPoint(canvas)
{
	//Debug("Mouse");
	mCanvas = canvas;
	mCanvas->setCursor(Qt::CrossCursor);
}

//********************************************************************************************
MouseEvents::~MouseEvents()
{

}


//**********************************************************************************************
void MouseEvents::canvasReleaseEvent(QMouseEvent * e)
{
	//Debug("Button Pressed");
	std::cout << " MouseEvents::canvasReleaseEvent enter " << endl << endl;
	QgsPoint point = mCanvas->getCoordinateTransform()->toMapCoordinates(e->x(), e->y());

  	if(e->button() == Qt::RightButton && (e->button() & Qt::LeftButton) == 0) // restart
  	{
		std::cout << " MouseEvents::canvasReleaseEvent right " << endl << endl;
    		emit RightPoint(point);
  	} 
  	else if (e->button()== Qt::LeftButton)
  	{
		std::cout << " MouseEvents::canvasReleaseEvent left " << endl << endl;
    		emit LeftPoint(point);
  	}
}

//**********************************************************************************************
void MouseEvents::canvasMoveEvent(QMouseEvent * e)
{
    	QgsPoint point = mCanvas->getCoordinateTransform()->toMapCoordinates(e->pos().x(), e->pos().y());
   	emit MouseMove(point);
}



