 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cORT.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of perfectly useless *.ort files
 *                : Slots into cRaster
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


#include "cORT.h"
using namespace Qrap;

//******************************************************************************
cORT::cORT()
{
}

//*****************************************************************************
cORT::~cORT()
{
}

//******************************************************************************
bool cORT::openFile(Float2DArray &Raster,string Directory, 
						cGeoP &NW, cGeoP &SE, GeoType &Proj, projPJ &Proj4,
						uint &rows, uint &cols, double &ns_res, double &ew_res,
						float &MinH,float &MaxH, int &CentMer)
{
	Proj = WGS84GC;
	unsigned i ,j;
	string line, field;

	PointsArray InArray;
	DIR *FileDirectory;
	struct dirent *FileInfo;	
	string FileName;
	
	FileDirectory = opendir(Directory.c_str());
	FileInfo = readdir(FileDirectory);
	
	//read all the file content
	float MaxX=-MAXFLOAT, MaxY=-MAXFLOAT, MinX=MAXFLOAT, MinY=MAXFLOAT;
	double Delta;
	MinH = 9999; MaxH =-100;
	ns_res = MAXDOUBLE;
	ew_res = MAXDOUBLE;
	unsigned IsOrt;
	i=0;
	while (FileInfo!=NULL)
	{
		if (FileInfo->d_type == DT_REG)
		{
			FileName = Directory+'/';
			FileName += FileInfo->d_name;
			IsOrt=FileName.find(".ort");
			if (IsOrt==string::npos)
				IsOrt=FileName.find(".ORT");
			if (IsOrt!=string::npos)
			{
				ifstream in_file(FileName.c_str());
				if (!in_file)
					cout << "error opening " << FileInfo->d_name << endl;
				else
					cout << "Opened " << FileInfo->d_name << endl;
				tPoint newP;
				in_file >> newP.x;
				if (i>0)
					while (fabs(InArray[i-1].x-newP.x)>50000)
						in_file >> newP.x;
				in_file >> newP.y;
//				if (i>0)
//					while (fabs(InArray[i-1].y-newP.y)>50000)
//					{
//						newP.x = newP.y;
//						in_file >> newP.y;
//					}
				in_file >> newP.h;
//				if (i>0)
//					while (fabs(InArray[i-1].x-newP.x)>3000)
//					{
//						newP.x = newP.y;
//						newP.y = newP.h;
//						in_file >> newP.h;
//					}
				InArray.push_back(newP);
				i++;
				while (!in_file.eof())
				{
					tPoint newP;
					in_file >> newP.x;
					while (fabs(InArray[i-1].x-newP.x)>50000)
						in_file >> newP.x;
					in_file >> newP.y;
					while (fabs(InArray[i-1].y-newP.y)>50000)
					{
						newP.x = newP.y;
						in_file >> newP.y;
					}
					in_file >> newP.h;
					while (fabs(InArray[i-1].x-newP.x)>3000)
					{
						newP.x = newP.y;
						newP.y = newP.h;
						in_file >> newP.h;
					}

					if ((fabs(InArray[i-1].y-newP.y)>50000)||
						(fabs(InArray[i-1].x-newP.x)>50000)||
						(fabs(InArray[i-1].x-newP.x)>3000))
					{
						cout 	<< "y: " << InArray[i-1].y << "	x: " 
							<< InArray[i-1].x << "	h: " << InArray[i-1].h <<"	" << i << endl;
						cout 	<< "y: " << newP.y << "	x: " 
							<< newP.x << "	h: " << newP.h << "	"<< FileInfo->d_name << endl;
					}
					else
					{
						InArray.push_back(newP);

						if (MaxX < InArray[i].x) MaxX=InArray[i].x;
						if (MaxY < InArray[i].y) MaxY=InArray[i].y;
						if (MaxH < InArray[i].h) MaxH=InArray[i].h;
						if (MinX > InArray[i].x) MinX=InArray[i].x;
						if (MinY > InArray[i].y) MinY=InArray[i].y;
						if (MinH > InArray[i].h) MinH=InArray[i].h;
						Delta = fabs(InArray[i].y-InArray[i-1].y);
						if (Delta>0.1) ns_res = min(ns_res, Delta);
						Delta = fabs(InArray[i].x-InArray[i-1].x);
						if (Delta>0.1) ew_res = min(ew_res, Delta);
						i++;
					}
				}
				// Read all records for current file into map object
			}
		}
		FileInfo = readdir(FileDirectory);
	}
	closedir(FileDirectory);
	
	rows = (int)round((MaxY-MinY)/ns_res)+1;
	cols = (int)round((MaxX-MinX)/ew_res)+1;
	cout << rows << "  RxC  " << cols << endl;
	
	delete_Float2DArray(Raster);
	Raster = new_Float2DArray(rows,cols);
	
	for (i=0; i<rows; i++)
		for (j=0; j<cols; j++)
			Raster[i][j] = -9999;
	int row, col;
	for (i=0; i<InArray.size(); i++)
	{
//		cout << i<< "	y: " << InArray[i].y << "	x: " << InArray[i].x << endl;
		row = (int)round((-MinY+InArray[i].y)/ns_res);
		col = (int)round((MaxX-InArray[i].x)/ew_res);
		Raster[row][col] = InArray[i].h;
	}
	cout << "Y: "<< MaxY<< "	"<<MinY<< "	X: " << MaxX<< "	" <<MinX << endl;
		
	Proj4=NULL;
	
	NW.Set(-MinY,MaxX,Proj,CentMer);
	SE.Set(-MaxY,MinX,Proj,CentMer);
	NW.SetGeoType(DEG);
	NW.Display();
	SE.SetGeoType(DEG);
	SE.Display();
	InArray.clear();
	return true;
}
