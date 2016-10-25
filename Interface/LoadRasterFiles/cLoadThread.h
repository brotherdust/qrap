 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadThread.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot 
 *                : email: magdaleen.ballot@up.ac.za
 *    Description : This is used in cLoadFiles for monitoring progress
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
//\TODO: This whole LoadRasterFile bunch need to be cleaned up.

#ifndef CLOADTHREAD_H_
#define CLOADTHREAD_H_

#include <QtGui>
 #include <QMutex>
 #include <QSize>
 #include <QThread>
 #include <QWaitCondition>
 #include <QStringList>
 #include "../../Prediction/cLoadFile.h"
 
 using namespace Qrap;

class cLoadThread : public QThread
{ Q_OBJECT
public:
	cLoadThread();
	cLoadThread(QObject *parent = 0);
	virtual ~cLoadThread();
	
	void Set(unsigned Source,
				unsigned Bin,
					GeoType Type,
					int CentMer,
					string Dir,
					QStringList Files,
					QString SourceDir,
					int FileCount,
					QString FileType,
					bool cut = false,
					short int Rule=0,
					bool interpolate=true);
					
	
					
signals:
	void Set(int , QString );
	void Finished();
	
protected:
     void run();
     
private:
    	unsigned mSource;
	unsigned mBin;
	GeoType mType;
	int mCentMer;
	string mDir;
	QStringList mFiles;
	QString mSourceDir;
	int mFileCount;
	QString mFileType;
	bool mCut;
	bool mInterpolate;
	short int mFileRule;
	
	QMutex mutex;
     	QWaitCondition condition;
     	bool restart;
     	bool abort;

};

#endif /*CLOADTHREAD_H_*/
