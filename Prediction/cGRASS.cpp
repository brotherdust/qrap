 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGRASS.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of grass file formats. 
 *                : Can slot into cRaster. Not really used
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

#include "cGRASS.h"
using namespace Qrap;
cGRASS::cGRASS()
{
}

cGRASS::~cGRASS()
{
	G_close_cell(fd);
}


int cGRASS::openFile(Float2DArray &Raster,string Directory, string FileName, 
						cGeoP &NW, cGeoP &SE, GeoType &Proj, projPJ &Proj4,
						uint &rows, uint &cols, double &ns_res, double &ew_res,
						float &min,	float &max,int CentMer)
{
//	int ret;
//	string Dir = Directory + "/cellhd";
//	cGDAL *MyRaster = new cGDAL();
//	ret = MyRaster->openFile(Raster, Dir, FileName, NW,SE, Proj,Proj4,rows, cols, ns_res, ew_res,min,max);
//	delete MyRaster;
	breakDir(Directory);
	G_set_error_routine ( &error_routine );
	G_set_gisrc_mode(1);
	char temp[255];
	strcpy(temp,Location.c_str());
	G_setenv("LOCATION_NAME",temp);
	strcpy(temp,GDBase.c_str());
	G_setenv("GISDBASE",temp);
	strcpy(temp,MapSet.c_str());	
	G_setenv("MAPSET",temp);
	 G_set_gisrc_mode(1);
		G_gisinit("QRap");
		
	char FN[255];
	strcpy(FN,FileName.c_str());
	char MS[255];
	strcpy(MS,MapSet.c_str());
//	G_get_window (&cellhd);
	G_get_cellhd(FN,MS,&cellhd);
	cout << cellhd.north <<endl;
	CentMer = 6*cellhd.zone-180;
	cols = cellhd.cols;
	rows = cellhd.rows;
	ns_res = cellhd.ns_res;
	ew_res = cellhd.ew_res;
	mFormat = cellhd.format;
	mProjection = cellhd.proj;
	Proj = determineProj(mProjection);
	cout << mProjection << endl;
	NW.Set(cellhd.north,cellhd.west,Proj,CentMer);
	SE.Set(cellhd.south,cellhd.east,Proj,CentMer);
	
	
	fd = G_open_cell_old (FN, MS);
	delete_Float2DArray(Raster);
	Raster = new_Float2DArray(rows,cols);
    if (fd < 0)
        return FALSE;
    
    void *ptr, *rraster;
	//char cell_buf[300];

	rraster=G_allocate_raster_buf(FCELL_TYPE);

	for (uint row = 0; row < rows; row++)
	{
		G_percent(row, rows, 2);

		if (G_get_raster_row(fd, rraster, row,FCELL_TYPE) < 0)
		   return FALSE;
		ptr = rraster;
		FCELL c;
		G_set_null_value(rraster, 0, FCELL_TYPE);
		
		for (uint col = 0; col < cols; col++, 
			   ptr = G_incr_void_ptr(ptr, G_raster_size(FCELL_TYPE))) 
		{
			c = *((FCELL *) ptr);
//    		if(G_is_f_null_value(&c))
//    		{
//       			c =0;
//    		}
			Raster[row][col] = c;
		}
	}
	G_close_cell(fd);  	
	return true;
	//return ret;
}

int cGRASS::writeFile(Float2DArray &Raster, string Directory, 
					string FileName,  cGeoP NW, cGeoP SE, uint rows, 
					uint cols, GeoType proj,
				double NSRes,
				double EWRes)
{
	mDir = Directory;
	mFile = FileName;
	mNW = NW;
	mSE = SE;
	mCols = cols;
	mRows = rows;
	mProj = proj;
	mFormat = 1;
	mNSRes = NSRes;
	mEWRes = EWRes;
	mProjection = determineProj(mProj);
	 G_set_error_routine ( &error_routine );
		
	if (!generateRasterHeadder()) 
	{
		printf("Headder Error");
		return FALSE;
	}
	float GRASSMAX = MAXFLOAT;
	char FN[255];
	strcpy(FN,mFile.c_str());
	//G_set_window(&cellhd);
	//printf("Set Window");
	fd = G_open_cell_new(FN);
	if (!(fd))
		return false;
	printf("Opened File");
	G_put_cellhd(FN, &cellhd);
	printf("Created Cell Head");
	buf = G_allocate_f_raster_buf();
	FCELL iras;
	if (!fd)
	{
		cout << "error creating: " << mFile << endl;
		return FALSE;
	}
	else
	{
		cout << mFile << " has been created" << endl;
	}
	for (uint i=0;i<mRows;i++) 
	{
    	for (uint j=0;j<mCols;j++)
    	{
    		if ( (Raster[i][j] >= -GRASSMAX)  && (Raster[i][j] < GRASSMAX ))
			{
         		//if (Raster[i][j] >= 0 )
           			iras = FCELL(Raster[i][j]);
         		//else
           		//	iras = FCELL(Raster[i][j]-0.5);
			}
			else
			{
				iras = 0;
			}
			buf[j] =iras;
	  	}
	  	if (!G_put_f_raster_row(fd, buf))
	  	{
	  		printf("WRITING GRASS ERROR");
	  		return FALSE;
	  	}
  	}

	G_close_cell(fd);
	G_free(buf);  	
  	Rainbow();
	printf("Finished Writning GRASS File");
	return TRUE;
}

GeoType cGRASS::determineProj(int Type)
{
	switch (Type) 
	{
		case PROJECTION_UTM: return UTM;
		case PROJECTION_SP: return WGS84GC;
		case PROJECTION_LL: return DEG;
		case PROJECTION_OTHER: return NDEF;
		default: return DEG;
	}
}

int cGRASS::determineProj(GeoType Type)
{
	switch (Type) 
	{
		case UTM: return PROJECTION_UTM;
		case WGS84GC: return PROJECTION_SP;
		case DEG: return PROJECTION_LL;
		case NDEF: return PROJECTION_OTHER;
	}
	return true;
}

void cGRASS::breakDir(string Dir)
{
	StrVec Vec;
	Vec.clear();
	char dir[255];
	strcpy(dir,Dir.c_str());
	char * t;
	string temp;
	printf ("Splitting string \"%s\" into tokens:\n",dir);
	t = strtok (dir,"/");
	while (t != NULL)
	{
		temp = t;
		Vec.push_back(temp);
  		t = strtok (NULL, "/");
	}
	MapSet = Vec[Vec.size()-1];
	Location = Vec[Vec.size()-2];
	GDBase = "";
	for (uint i = 0; i < Vec.size()-2; i++)
	{
		GDBase = GDBase + "/" + Vec[i];
	}
//	printf(MapSet);
//	printf(Location);
//	printf(GDBase);
}

int cGRASS::error_routine ( char *msg, int fatal) 
{
    std::cerr << "error_routine (fatal = " << fatal << "): " << msg << std::endl;
    return 1;
}

void cGRASS::computeResolutions()
{
	double North,South,East,West;
	bool bSouth;
	int CentMer;
	GeoType TempProj;
	mNW.Get(North,West,TempProj,CentMer,bSouth);
	mSE.Get(South,East,TempProj,CentMer,bSouth);
	mNSRes =fabs(North - South)/mRows;
	mEWRes = fabs(East-West)/mCols;
}

int cGRASS::generateRasterHeadder()
{
	if (mRows == 0 || mCols ==0)
	{
		printf("No rows and cols");
		return FALSE;
	}
	double North,South,East,West;
	bool bSouth;
	int CentMer;
	GeoType TempProj;
	mNW.Get(North,West,TempProj,CentMer,bSouth);
	mSE.Get(South,East,TempProj,CentMer,bSouth);
	mNW.SetGeoType(mProj,CentMer);
	mSE.SetGeoType(mProj,CentMer);
	mNW.Get(North,West,TempProj,CentMer,bSouth);
	mSE.Get(South,East,TempProj,CentMer,bSouth);
	if (North <= South)
	{
		printf("North must be larger than South");
		return FALSE;
	}
	if (West == East)
	{
		printf("No Lats");
		return FALSE;
	}
	//computeResolutions();
//	FILE *fp;
//	string File = mDir+"/cellhd/"+mFile;
//	if((fp=fopen(File.c_str(),"w"))==NULL)
//	{
//		
//	}
//	
//	mProjection = determineProj(mProj);
//	fprintf(fp,"proj:\t\t%d\n",mProjection);
//	fprintf(fp,"zone:\t\t%d\n",(int)ceil((180+CentMer)/6));
//	fprintf(fp,"north:\t\t%f\n",North);
//	fprintf(fp,"south:\t\t%f\n",South);
//	fprintf(fp,"east:\t\t%f\n",East);
//	fprintf(fp,"west:\t\t%f\n",West);
//	fprintf(fp,"cols:\t\t%d\n",mCols);
//	fprintf(fp,"rows:\t\t%d\n",mRows);
//	fprintf(fp,"e-w resol:\t%f\n",mEWRes);
//	fprintf(fp,"n-s resol:\t%f\n",mNSRes);
//	fprintf(fp,"format:\t\t%d\n",1);
//	fprintf(fp,"compressed:\t%d\n",1);
//	fprintf(fp,"top:\t\t%d\n",1);
//	fprintf(fp,"bottom:\t\t%d\n",0);
//	fprintf(fp,"cols3:\t\t%d\n",mCols);
//	fprintf(fp,"rows3:\t\t%d\n",mRows);
//	fprintf(fp,"e-w resol3:\t%f\n",mEWRes);
//	fprintf(fp,"n-s resol3:\t%f\n",mNSRes);
//	fprintf(fp,"t-b resol:\t%d\n",1);
//	fclose(fp);
//	printf("Wrote CellHD Maulally");
//	if (mProj == UTM)
//	{
//		North =fabs(North);
//		South = fabs(South);
//	}
	cellhd.rows = mRows;
	cellhd.rows3 = mRows;
	cellhd.cols =mCols;
	cellhd.cols3 = mCols;
	cellhd.north = North;
	cellhd.ns_res = mNSRes;
	cellhd.ns_res3 = mNSRes;
	cellhd.south = South;
	cellhd.west = West;
	cellhd.ew_res = mEWRes;
	cellhd.ew_res3 = mEWRes;
	cellhd.east = East;
	cellhd.top = 1.;
	cellhd.bottom = 0.;
	cellhd.tb_res = 1;
	cellhd.depths =1;
	cellhd.format = mFormat;
	cellhd.proj = mProjection;
	cellhd.zone = (int)ceil((180+CentMer)/6);
	cellhd.compressed = 1;
	cellhd.rows = mRows;
	cellhd.cols = mCols;	
	printf("Start Initialising");
	breakDir(mDir);
	G_set_gisrc_mode(1);
//	GDBase = "/home/gecko/GRASS";
//	Location = "QRap";
//	MapSet = "Predictions";
	char temp[256];
	strcpy(temp,Location.c_str());
	G_setenv("LOCATION_NAME",temp);
	strcpy(temp,GDBase.c_str());
	G_setenv("GISDBASE",temp);
	strcpy(temp,MapSet.c_str());	
	G_setenv("MAPSET",temp);
	 printf("Fin EnvVar");
	  G_set_gisrc_mode ( 1 ); 
	 G_gisinit("QRap");
	printf("Fin QRap");
	G_set_window(&cellhd);
	printf("Fin Set Window");
	G_adjust_Cell_head(&cellhd, true, true);
	printf("Fin Cell Head");
	
	return TRUE;	
}

void cGRASS::Rainbow()
{
	float Min = -179;
	float Max = 50;
	int dif4 = 0;
	int dif2 = 0;
	int R = 0,G=0,B=0;
	string name;
	char tempName[255];
	
	G_init_colors (&colors);
	G_init_cats ((FCELL)0, "", &cats);
    G_set_raster_cats_title("AltGisRap",&cats);

	dif4 = (int)floor(255/fabs(Max-Min)*4);
	dif2 = (int)floor(255/fabs(Max-Min)*2);
	if (dif4 == 0) dif4 = 2;
	if (dif2 == 0) dif2 = 1;
	//cout << dif4 << "\t" << dif2 << endl;
	R=255;
	G=0; 
	B=0;
	int count =(int)( Max - Min);
//	cout << "Max:\t" << Max << endl;
//	cout << "Min:\t" << Min << endl;
//	cout << "Count:\t" << count << endl;
//	cout<< "Count/4:\t" << count/4 << endl;
//	cout << "Count/2:\t"<< count/2 << endl;
//	cout << "3*Count/4:\t"<< 3*count/4 << endl;
//	cout << "Max - Count/4:\t"<< (Max - count/4) << endl;
//	cout << "Max - Count/2:\t"<< (Max - count/2) << endl;
//	cout << "Max - 3*Count/4:\t"<< (Max - (3)*count/4) << endl;
	for (int k = (int)Min; k <= (int)Max; k++)
	{
		if (k <= (Min + count/4))
		{ 
			R-=dif2;
			G+=dif4;
			B=0;
//			cout << "1/4" << endl;
			if (R<1 || R > 254) dif2 = -dif2;
			if (G<1 || G > 254) dif4 = -dif4;
		}
		if(k<=(Min + count/2) && k > (Min + count/4))
		{
			R-=dif2;
			G=255;
			B+=dif4;
//			cout << "2/4" << endl;
			if (R<1 || R > 254) dif2 = -dif2;
			if (B<1 || B > 254) dif4 = -dif4;
		} 
		if (k<=(Min + (3)*count/4) && k > (Min + count/2))
		{
			R+=dif2;
			G=255;
			B=255;
//			cout << "3/4" << endl;
			if (R<1 || R > 254) dif2 = -dif2;
		}
		if (k > (Min + (3)*count/4))
		{
			R+=dif2;
			G-=dif4;
			B=255;
//			cout << "4/4" << endl;
			if (R<1 || R > 254) dif2 = -dif2;
			if (G<1 || G > 254) dif4 = -dif4;
		}
		G_set_color(k,R,G,B,&colors);
		name = (k) + "dBm";
		strcpy(tempName,name.c_str());
		G_set_cat(((CELL)k),  tempName ,       &cats);
//		cout << "k: " << k << "\t";		
//		cout << "R: " << R << "\t";
//		cout << "G: " << G << "\t";
//		cout << "B: " << B << endl;
	}
	G_set_color((Min-1),255,255,255,&colors); 
	char MS[255];
	char FN[255];
	strcpy( MS , MapSet.c_str());
	strcpy( FN , mFile.c_str());
	G_write_colors(FN,MS,&colors);
	G_write_cats(FN,&cats);	
}

