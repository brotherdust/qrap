 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadThread.cpp
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

#include "cLoadThread.h"

//*****************************************************************
cLoadThread::cLoadThread()
{

}

//**************************************************************
cLoadThread::cLoadThread(QObject *parent)
     : QThread(parent)
 {

     restart = false;
     abort = false;
 }

//**************************************************************
cLoadThread::~cLoadThread()
 {
     mutex.lock();
     abort = true;
     condition.wakeOne();
     mutex.unlock();

     wait();
 }


//***************************************************************
void cLoadThread::Set(unsigned Source,
		unsigned Bin,
		GeoType Type,
		int CentMer,
		string Dir,
		QStringList Files,
		QString SourceDir,
		int FileCount,
		QString FileType,
		bool cut,
		short int Rule,
		bool interpolate)
{
	try 
	{
		mSource = Source;
		mBin = Bin;
		mType = Type;
		mCentMer = CentMer;
		mDir = Dir;
		mFiles = Files;
		mSourceDir = SourceDir;
		mFileCount = FileCount;
		mFileType = FileType;
		mCut = cut;
		mInterpolate=interpolate;
		mFileType=Rule;
	}
	catch(const exception &e) 
	{
		cout << e.what() << endl;
	}
}

//****************************************************************
void cLoadThread::run()
{
	try 
	{
		printf("Directory:\t%s",mDir.c_str());
		cLoadFile Rasters(mSource,mBin,mDir);
		
		if (!mCut)
		{
			QStringList::Iterator it = mFiles.begin();
    			QString File;
	    		FileType Type;
	    		int k = 0;
	    		if (mFileType == "BIN" || mFileType == "BINFILE")
	    		{
	    			Type = BINFILE;
	    		}
	    		else if (mFileType == "GDAL" || mFileType == "GDALFILE")
	    		{
	    			Type = GDALFILE;
	    		}
	    		else
	    		{
	    			Type == GDALFILE;
	    		}
	   	 	while( it != mFiles.end() ) 
	    		{
	    			File = *it;
	    			
	    			printf("SourceDir: %s\nFile; %s\nType: %d\nmType: %d\n",mSourceDir.latin1(),File.latin1(),Type,mType);
	    			try
	    			{
					cout << "cLoadThread::run() loading raster: k = " << k << endl;
	    				Rasters.LoadFile(mSourceDir.latin1(),File.latin1(),Type,mType,"",mCentMer);
//bool cLoadFile::LoadFile(string SrcDirectory, string FileName,FileType filetype,GeoType Proj,	string Proj4String,int CentMer,	bool Hem)
					cout << "cLoadThread::run() DONE loading raster: k = " << k << endl;
	    				++it;
	    				k++;
	    			}
	    			catch (const exception &e1)
	    			{
	    				printf("Error trying to Load files:\n%s",e1.what());
	    				emit (mFileCount,e1.what());
	    			}
				emit Set(k,File);
	    		}
//	    		emit Set(mFileCount,"Writng to Database");
	    		try
	    		{
				cout << "cLoadThread::run() Writing to Database." << endl;
	    			Rasters.WriteDB();
				cout << "cLoadThread::run() DONE Writing to Database." << endl;
	    		}
	    		catch (const exception &e1)
	    		{
	    			printf("Error trying to Load files to db:\n%s",e1.what());
	    		}
			cout << "cLoadThread::run()  Emiting Filecount, Finished." << endl;
	    		emit Set(mFileCount,"Finished");
			cout << "cLoadThread::run()  Emiting  Finished()." << endl;
	    		emit Finished();
			cout << "cLoadThread::run()  DONE Emiting  Finished()." << endl;
		}
		else//cut set
		{
			cout << "cLoadThread::run()  Emiting  Set(0,''Cutting fileset'')" << endl;
			emit Set(0,"Cutting fileset");
			cout << "cLoadThread::run()  DONE Emiting  Set(0,''Cutting fileset'')" << endl;
			try
			{
				cout << "cLoadThread::run() cutting rasters" << endl;
				Rasters.CutCurrentSet(mSource,mBin,mFileRule,mInterpolate);
				cout << "cLoadThread::run() Done cutting rasters" << endl;
			}
			catch (const exception &e1)
			{
				printf("Error trying to Cut files:\n%s",e1.what());
			}
			cout << "cLoadThread::run()  Emiting 100, Finished." << endl;
	    		emit Set(100,"Finished");
			cout << "cLoadThread::run()  Emiting  Finished()." << endl;
	    		emit Finished();
			cout << "cLoadThread::run()  DONE Emiting  Finished() to Database." << endl;
		}
	}
	catch(const exception &e) 
	{
		cout << e.what() << endl;
		emit Set(100,e.what());
		emit Finished();
	}
}

