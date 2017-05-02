 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cRaster.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Represents a raster of points e.g. DEM raster file
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

#include "cRaster.h"
using namespace Qrap;


//************************************************************************
cRaster::cRaster()
{
	mRows = 2;
	mCols = 2;
	mProjType = DEG;
	mNSres = 1.0;
	mEWres = 1.0;
	mUsed = false;
	mSouth = true;
	mProj4=nullptr;
	mRaster = new_Float2DArray(mRows,mCols);	
	mDirectory = "qrap/Data/SRTM/";
	mFilename = "raster.bin";
	mAdfGeoTransform = new double[6];
//	cout << "Default constructor Raster" << endl;
}
 
//*************************************************************************
cRaster::cRaster(string Directory, 
				string FileName,
				FileType filetype,
				GeoType Proj,
				string Proj4String,
				int CentMer,
				bool Hem,
				float Min,
				float Max)
{
	mAdfGeoTransform = new double[6];
	mRows = 2;
	mCols = 2;
	mProjType = Proj;
	mCentMer = CentMer;
	mSouth = Hem;
	mMin = Min;
	mMax = Max;
	mProj4=nullptr;
	cout << "In CRaster Non-default constructor ... FileName = " << FileName.c_str() << endl;

	mNSres = 1.0;
	mEWres = 1.0;
	mDirectory = Directory;
	mFilename = FileName;
//	cout << "In cRaster non-default constructor, before new_Float2DArray" << endl;
	mRaster = new_Float2DArray(mRows,mCols);
//	cout << "In cRaster non-default constructor, before ReadFile" << endl;	
	if ((Proj4String != "")&&(nullptr==mProj4))
	{
//		cout << "In CRaster Non-default constructor ... Proj4Sting empty " << endl;
		mProj4=pj_init_plus(Proj4String.c_str());
	}
	else if ((mProjType != NDEF)&&(nullptr==mProj4))
	{
//		cout << "In CRaster Non-default constructor ... calling ReturnProj4 " << endl;
		ReturnProj4(mProjType,mCentMer,mSouth,mProj4);
		if (nullptr!=mProj4) Proj4String = pj_get_def(mProj4, 0);
	}
	

	ReadFile(Directory, FileName,filetype,mProjType,
							Proj4String,mCentMer,mSouth,
							mMin,mMax);

//	cout << "In CRaster Non-default constructor ... after ReadFile assignment." << endl;

	if (mSouth)
	{
		mNW.SetGeoType(mProjType,mCentMer);
		mNW.Get(mMapLat,mMapLon,mMapType,mMapCM,Hem);
	}
	else
	{
		mNW.SetGeoType(mProjType,mCentMer);
		mNW.Get(mMapLat,mMapLon,mMapType,mMapCM,Hem);
	}


	cout << "In CRaster Non-default constructor ... Geotype = " << mProjType << endl;
	cout << "mCentMer = " << mCentMer << endl;
	if (nullptr!=mProj4) Proj4String = pj_get_def(mProj4, 0);
	cout << "Proj4string = " << Proj4String << endl;
	if (mSouth) cout << " South" << endl;
	else cout << " North" << endl;

	cout << "Leaving cRaster non-default Constructer: mFilename = " << mFilename << endl;


}

//************************************************************************
cRaster::~cRaster()
{
//	cout << "cRaster Destructor" << endl;
	delete [] mAdfGeoTransform;
	delete_Float2DArray(mRaster); 
	if (nullptr!=mProj4)
		pj_free(mProj4);
}

//****************************************************************************
bool cRaster::WriteFile(Float2DArray &Raster,
				cGeoP NW,
				cGeoP SE, 
				unsigned Rows,
				unsigned Cols,
				double NSres,
				double EWres,
				string Directory, 
				string FileName,
				OutPut filetype,
				GeoType OriginalProj,
				GeoType ResultProj,
				int OriginalCentMer,
				int ResultCentMer,
				string Proj4String )
{
	double oN,oS,oE,oW,rN,rS,rE,rW;
	bool South=true;
	bool msgs=false;
	mNW = NW;
	mSE = SE;
	South = mNW.Hemisphere();
	if (OriginalProj!=ResultProj)
	{
		mNW.SetGeoType(OriginalProj,OriginalCentMer);
		mSE.SetGeoType(OriginalProj,OriginalCentMer);
		mNW.Get(oN,oW,OriginalProj,OriginalCentMer,South);
		mSE.Get(oS,oE,OriginalProj,OriginalCentMer,South);
	}
	mDirectory = Directory;
	mFilename = FileName;
	mProjType = ResultProj;
	mCentMer = ResultCentMer;
	mNW.SetGeoType(mProjType,mCentMer);
	mSE.SetGeoType(mProjType,mCentMer);
	mNW.Get(rN,rW,mProjType,mCentMer,South);
	mSE.Get(rS,rE,mProjType,mCentMer,South);

	if (OriginalProj!=ResultProj)
	{
		mNSres = NSres*fabs((rN-rS)/(oN-oS));
		mEWres = EWres*fabs((rE-rW)/(oE-oW));
		mRows = Rows;
		mCols = Cols;
	}
	else
	{
		mNSres = NSres;
		mEWres = EWres;
		mRows = Rows;
		mCols = Cols;
	}
	
	if (mProjType != NDEF && Proj4String != "")
		mProj4=pj_init_plus(Proj4String.c_str());
	else if (mProjType != NDEF && Proj4String == "")
		ReturnProj4(mProjType,mCentMer,South,mProj4);
	if (filetype == BIN)
	{
		mFileType = BINFILE;
	 	cBIN MyRaster;
		msgs = MyRaster.writeFile(Raster,Directory, FileName, 
						mNW, mSE, ResultProj, mProj4,
						Rows, Cols, ResultCentMer,mNSres,mEWres);
	}
/*	else if (filetype == GRASS) 
	{
		mFileType = GRASSFILE;
		cGRASS MyRaster;
		int msgs = MyRaster.writeFile(Raster,Directory, FileName, 
						mNW, mSE, Rows, Cols,mProjType,mNSres,mEWres);
		if (!msgs)
		{
			string err = "Error reading file: ";
			err+=FileName;
			QRAP_ERROR(err.c_str());
		}
	}
*/
	else 
	{
		mFileType = GDALFILE;
		string TypeString;
		switch (filetype)
		{
			case GEOTIFF:		TypeString = "GTiff";		break;
			case VRT:		TypeString = "VRT";		break;
			case NITF:		TypeString = "NITF";		break;
			case HFA:		TypeString = "HFA";		break;
			case ELAS:		TypeString = "ELAS";		break;
			case AAIGRID:		TypeString = "AAIGrid";		break;
			case MEM:		TypeString = "MEM";		break;
			case GIF:		TypeString = "GIF";		break;
			case XPM:		TypeString = "XPM";		break;
			case BMP:		TypeString = "BMP";		break;
			case PCIDSK:		TypeString = "PCIDSK";		break;
			case PCRASTER:		TypeString = "PCRaster";	break;
			case ILWIS:		TypeString = "ILWIS";		break;
			case GMT:		TypeString = "GMT";		break;
			case NETCDF:		TypeString = "netCDF";		break;
			case HDF4IMAGE:		TypeString = "HDF4Image";	break;
			case ENDR:		TypeString = "ENdr";		break;
			case PAUX:		TypeString = "PAux";		break;
			case MFF:		TypeString = "MFF";		break;
			case MFF2:		TypeString = "MFF2";		break;
			case BT:		TypeString = "BT";		break;
			case IDA:		TypeString = "IDA";		break;
			case JPEG2000:		TypeString = "JPEG2000";	break;
			case RMF:		TypeString = "RMF";		break;
			case RST:		TypeString = "RST";		break;
			default:		TypeString = "HFA";		break;
		}
		cGDAL MyRaster;
		msgs = MyRaster.writeFile(Raster,Directory, FileName, 
						mNW,OriginalProj,ResultProj, mProj4,mProj4,
						Rows, Cols, ResultCentMer,TypeString,mNSres,mEWres);
				
	}
	if (!msgs)
	{
		string err = "Error reading file: ";
		err+=FileName;
		QRAP_ERROR(err.c_str());
	}
	return msgs;  
}


//*********************************************************************************
bool cRaster::ReadFile(string Directory, 
				string FileName,
				FileType filetype,
				GeoType Proj,
				string Proj4String,
				int CentMer,
				bool Hem,
				float Min,
				float Max)
{
	mDirectory = Directory;
	mFilename = FileName;
	bool msgs=false;
	mFileType = filetype;
	mCentMer = CentMer;
	mSouth = Hem;
	mMin = Min;
	mMax = Max;
	char * Proj4char;
	Proj4char = new char[100];

	cout << "In cRaster::ReadFile ... Proj4String.c_str() = " << Proj4String.c_str() << endl;

	strcpy(Proj4char,Proj4String.c_str());
//	if (Proj != mProjType)
	if (nullptr==mProj4)
	{
		if (mProjType != NDEF && Proj4String != "")
		{
			mProj4=pj_init_plus(Proj4char);
		}
		else if (mProjType != NDEF && Proj4String == "")
		{
			ReturnProj4(mProjType,mCentMer,mSouth,mProj4);
		}
	}
	delete [] Proj4char;


	if (filetype == BINFILE) // binary file
	{
		cout << endl << "In bool cRaster::ReadFile(...) filetype == BINFILE. First Try" << endl;
		cBIN MyRaster;
		msgs = MyRaster.openFile(mRaster, Directory, FileName, mNW,mSE, 
				mProjType, mProj4, mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer);
		mProjType=WGS84GC;
	}
	else if (filetype == GDALFILE) // GDAL file 
	{
		cout << endl << "In bool cRaster::ReadFile(...) filetype == GDALFILE . First try." << endl;
		cGDAL MyRaster;
		msgs = MyRaster.openFile(mRaster,Directory, FileName, mNW,mSE, 
				mProjType,mProj4,mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer);
	}
/*	else if (filetype == GRASSFILE) // GDAL file 
	{
		cGRASS MyRaster;
		msgs = MyRaster.openFile(mRaster,Directory, FileName, mNW,mSE, 
				mProjType,mProj4,mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer);
	}

	else if (filetype == ORTFILE) // ORT file
	{
		cout << "In bool cRaster::ReadFile( ... filetype == ORTFILE " << endl;
		cORT MyRaster; 
		msgs = MyRaster.openFile(mRaster,Directory, mNW,mSE, 
						mProjType,mProj4,mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer);
		mProjType=WGS84GC;
	}
*/
	if (!msgs)
	{
		cout << endl <<"In bool cRaster::ReadFile(...) !msgs. Initial try failed. " << endl;
		cGDAL MyGDALRaster;
		msgs = (MyGDALRaster.openFile(mRaster,Directory, FileName, mNW,mSE, 
			mProjType,mProj4, mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer));

		if (msgs) 
		{
			mSouth = mNW.Hemisphere();
			mFileType = GDALFILE;
			cout << endl << "In bool cRaster::ReadFile(...) !msgs  GDALFILE succeeded after defaulting to GDAL" << endl;
		}
		else
		{
			cBIN MyBINRaster;
			msgs = (MyBINRaster.openFile(mRaster, Directory, FileName, mNW,mSE, 
					mProjType,mProj4,mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer));
			if (msgs)
			{
				mFileType = BINFILE;
				mProjType = WGS84GC;
				mSouth = mNW.Hemisphere();
				cout << "In bool cRaster::ReadFile(...) !msgs attempting BINFILE after GDAL failed." << endl;
			}
/*			else
			{
				cORT MyORTRaster;
				msgs =(MyORTRaster.openFile(mRaster, Directory, mNW,mSE, 
						mProjType,mProj4,mRows, mCols, mNSres, mEWres,mMin,mMax,mCentMer));
				
				if (msgs)
				{
					mFileType = ORTFILE;
					mProjType = WGS84GC;
					mSouth = true;
					cout << "In bool cRaster::ReadFile( ... !msgs  ORTFILE " << endl;
				}
*/
			else 
			{
				cout << " Leaving cRaster::ReadFile(....) Could not load file. Returning False" << endl; 
				return false;
			}
			
		}
	}

	if (msgs)
	{	
		if (CentMer != -1 && CentMer != mCentMer)
			mCentMer = CentMer;
		if (Min > mMin)	mMin = Min;
		if (Max < mMax)	mMax = Max;
		mSouth = mNW.Hemisphere();
		if (Proj != NDEF && Proj != mProjType)
		{
			mProjType = Proj;
			mNW.SetGeoType(mProjType,mCentMer);
			mSE.SetGeoType(mProjType,mCentMer);
		}
		if (nullptr==mProj4)
		{
			mNW.SetGeoType(mProjType,mCentMer);
			mSE.SetGeoType(mProjType,mCentMer);
			ReturnProj4(mProjType,mCentMer,mSouth,mProj4);
		}
		GetRes();
		cout << "Leaving cRaster::ReadFile(...). File loaded." << endl;		
		return true;
	}
	cout << " Leaving cRaster::ReadFile(....). returning False as last resort" << endl; 
	return false;
}


//******************************************************************************
int cRaster::GetHeader(cGeoP &NW, 	cGeoP &SE, 
					int &CentMer,
					uint &Rows, 
					uint &Cols,
					double &NSres,
					double &EWres,
					GeoType &Proj,
					string &Proj4,
					FileType &Type) 
{
	NW = mNW;
	SE = mSE;
	CentMer = mCentMer;
	Rows = mRows;
	Cols = mCols;
	GetRes();
	NSres=mNSres;
	EWres=mEWres;
	Proj = mProjType;
	if (mProj4 != nullptr)
	{
		Proj4 = pj_get_def(mProj4, 0);
//		cout << "In cRaster::GetHeader: Proj4 = " << Proj4 << endl << endl;
	}
	else
		Proj4 = "";
	Type = mFileType;
	return TRUE;
}

//*******************************************************************************
double cRaster::GetRes() // return res in meters
{
	double res = 0;
	double EWRes;
	double NSRes;
	double North,South,East,West;
	if (mEWres == 0 || mNSres ==0)
	{
		if (mProjType == DEG)
		{
			mNW.Get(North,West);
			mSE.Get(South,East);
			mNSres = fabs((North - South)/mRows);
			mEWres = fabs((East - West)/mCols);
		}
		else
		{
			int CM;
			GeoType GT;
			bool S;
			mNW.Get(North,West,GT,CM,S);
			mSE.Get(South,East,GT,CM,S);
			cGeoP NE(North,East,GT,CM);
			cGeoP SW(South,West,GT,CM);
			mNSres = max(mNW.Distance(SW)/mRows,NE.Distance(mSE)/mRows);
			mEWres = max(mNW.Distance(NE)/mCols,SW.Distance(mSE)/mCols);
		}
	}
	if (mProjType==DEG)
	{
		mNW.Get(North,West);
		mSE.Get(South,East);
		cGeoP NE(North,East,DEG);
		cGeoP SW(South,West,DEG);
		NSRes = mNSres*mNW.Distance(SW)/fabs(North - South);
		EWRes = mEWres*max(mNW.Distance(NE),SW.Distance(mSE))/fabs(East-West);		
	}
	else
	{
		EWRes = mEWres;
		NSRes = mNSres;		
	}
	res = min(EWRes,NSRes);
	return res;
}

//*******************************************************************************
float cRaster::GetValue(cGeoP &Point, int Interpolation)
{
	if ((mRows != 0)&& (mCols != 0)) 
	{
		bool Hem;
		mUsed = true;
		int row,col;
		double drow, dcol;
		GeoType PointType;
		double PointLat,PointLon;
		int PointCM;
		double dlat, dlon;     // 'decimal' row and col
					// distance between point of interest
	                               // and closest point to NW
		double h11, h12, h21, h22, h1dash, h2dash;   // heights at intermediary points (see diagram)
		double value;                           // height at point of interest

//		cout << " In cRaster::GetValue: mCentMer = " << mCentMer << endl;
//		cout << " In cRaster::GetValue: mProjType = " << mProjType << endl;
//		cout << " In cRaster::GetValue: mRows = " << mRows << endl;
//		cout << " In cRaster::GetValue: mCols = " << mCols << endl;
		// \TODO the following two line are only for debugging and need to be taken out
//		Point.SetGeoType(DEG); 
//		Point.Display();
		Point.SetGeoType(mProjType,mCentMer);
//		cout << " In cRaster::GetValue: after Point.SetGeoType" << endl;		
		Point.Get(PointLat,PointLon,PointType,PointCM,Hem);
//		cout << " In cRaster::GetValue: after Point.Get" << endl;
		drow = (mMapLat-PointLat)/mNSres;
		if (mProjType!=WGS84GC)
			dcol = (PointLon - mMapLon)/mEWres;
		else
			dcol = (mMapLon - PointLon)/mEWres;

//		cout << " In cRaster::GetValue: Before Interpolation" << endl;
//		cout << " In cRaster::GetValue: drow = " << drow << "	dcol = " << dcol << endl;
		if (Interpolation==2)
		{
			// interpolate between the surrounding 4 points
			//
			//                  h1'
			//     h11 X--------x----X h12        ^
			//         |        ^    |            |
			//         |    dlat|    |            |
			//         |        |    |            | m_res
			//         |        v    |            |
			//         |<------>o    |            |
			//         |   dlon      |            |
			//     h21 X--------x----X h22        v
			//                  h2'

			col = floor(dcol);
			row = floor(drow);
			dlat = mMapLat - (double)row*mNSres - PointLat;

			if (mProjType!=WGS84GC)
				dlon = PointLon - mMapLon - (double)col*mEWres;
			else
				dlon = mMapLon - PointLon - (double)col*mEWres;

//			cout << " In cRaster::GetValue: row = " << row << "	col = " << col << endl;
			// get heights at each of surrounding points
			if ((row>=0)&&(row+1<mRows)&&(col>=0)&&(col+1<mCols))
			{
				h11 = mRaster[row][col];
				h21 = mRaster[row+1][col];
				h12 = mRaster[row][col+1];
				h22 = mRaster[row+1][col+1];
/*				if (row+1 < mRows)   		
					h21 = mRaster[row+1][col];
				else h21 = h11;
//				else h21 = OUTOFRASTER;
				if (col+1 < mCols)
					h12 = mRaster[row][col+1];
				else h12 = h11;
//				else h12 = OUTOFRASTER;
			 	if ((row+1 < mRows) && (col+1<mCols))
			 		h22 = mRaster[row+1][col+1];
			 	else if (row+1 < mRows)	h22 = h21;
			 	else if (col+1 < mCols) h22 = h12;
			 	else h22 = h11;
//				else h22 = OUTOFRASTER;
*/
			}
			else return OUTOFRASTER;
			
			// 1. interpolate in a E-W direction 
			h1dash = h11 + (dlon/mEWres)*(h12-h11);
			h2dash = h21 + (dlon/mEWres)*(h22-h21);


			// 2. interpolate in a N-S direction 
			value = h1dash + (dlat/mNSres)*(h2dash-h1dash);

			if ((h11!=OUTOFRASTER)&&(h12!=OUTOFRASTER)
					&&(h21!=OUTOFRASTER)&&(h22!=OUTOFRASTER))
			{
				if ((value <-500)||(value>8880))
				{
				   		row = round(drow);
						col = round(dcol);
	   					if ((row>=0)&&(row<mRows)&&(col>=0)&&(col<mCols))
							return mRaster[row][col];
						else
							return OUTOFRASTER;
				}
				else return(value);
			}
		}
//		cout << "cRaster::GetValue .... amper einde" << endl; 
		row = round(drow);
		col = round(dcol);
//		cout << " In cRaster::GetValue: After Interpololation " << endl;
//		cout << " In cRaster::GetValue: row = " << row << "	col = " << col << endl;
	   	if ((row>=0)&&(row<mRows)&&(col>=0)&&(col<mCols))
			return mRaster[row][col];
		else
			return OUTOFRASTER;
	}
	else return OUTOFRASTER;
}

//*******************************************************************************
bool cRaster::IsIn(cGeoP point)
{
	if (point.Between(mNW,mSE))
	{
		double test = GetValue(point,2);
		if ((test>-500)&&(test<8880))
			return true;
		else return false;
	}
	else return false;
}

//*****************************************************************************
bool cRaster::ReturnProj4(GeoType PointType,
				int PointCM,
				bool South, 
				projPJ &Proj)
{
//		cout << "In cRaster::ReturnProj4. PointType = " << PointType << endl;
		if (Proj != nullptr)
		{
//			cout << "In cRaster::ReturnProj4. freeing pj "<< endl;
			pj_free(Proj);
//			cout << "In cRaster::ReturnProj4. pj freed "<< endl;
		}
		char *centmer;
		centmer = new char[120];
		char *southstr;
		southstr = new char [120];
		string Proj4;

//		cout << "In cRaster::ReturnProj4. Voor switch " << endl;
		switch (PointType) 
		{
			case DEG:
				Proj4 = "+latlong +ellps=WGS84 +datum=WGS84 +no_defs";
//				static char *DEGparms[] = {	"proj=latlong",	"ellps=WGS84",	
//											"datum=WGS84",	"no_defs"};
//				Proj = pj_init(sizeof(DEGparms)/sizeof(char*),DEGparms);
				break;
			case WGS84GC:
				sprintf(centmer,"+proj=tmerc +ellps=WGS84 +lon_0=%d +units=m +no_defs",PointCM);
				Proj4 = centmer;
//				sprintf(centmer,"lon_0=%d",PointCM);
//				static char *WGS84GCparms[] = {"proj=tmerc","ellps=WGS84",
//												centmer,"units=m","no_defs"};
//				Proj = pj_init(sizeof(WGS84GCparms)/sizeof(char*),WGS84GCparms);
				break;
			case UTM:
				if (South) 	
					sprintf(southstr,"south");
				else 
					sprintf(southstr,"north");
				sprintf(centmer,"+proj=utm +zone=%d +%s",(int)ceil((180+PointCM)/6),southstr);
				Proj4 = centmer;
//				sprintf(centmer,"zone=%d",(int)ceil((180+PointCM)/6));
//				if (South)
//					sprintf(southstr,"south");
//				else 
//					sprintf(southstr,"north");
//				static char *UTMparms[] = {	"proj=utm",	centmer,southstr };
//				Proj = pj_init(sizeof(UTMparms)/sizeof(char*),UTMparms);
				break;
			case NDEF:
				Proj4 = "+proj=latlong";

//				static char *NDEFparms[] = {"proj=latlong"	};
//				Proj = pj_init(sizeof(NDEFparms)/sizeof(char*),NDEFparms);
				break;
		}
//		Proj = nullptr;
		cout << " In cRaster::ReturnProj, Proj4 = " << Proj4 << endl;
		Proj = pj_init_plus(Proj4.c_str());
		
		delete [] centmer;
		delete [] southstr;
//		cout << " Leaving cRaster::ReturnProj" << endl;
		return true;
}

//*****************************************************************************
void cRaster:: Display()
{
	double North,South,East,West;
	int CentMer;
	bool Hem;
	GeoType GType;
	mNW.Get(North,West,GType,CentMer,Hem);
	mSE.Get(South,East,GType,CentMer,Hem);
	printf("####CRaster Header####\n");
	printf("North: %f\nSouth: %f\n East: %f\n West: %f\n",North,South,East,West);
	if (mSouth) printf("Southern Hemesphere\n");
		else printf("Northen Hemesphere\n");
	printf("Central Meridian: %d\n",mCentMer);
	printf("Rows: %d\t Cols: \%d\n",mRows,mCols);
	printf("Resolution: %f\n",GetRes());
	printf("Proj4: %s\n",pj_get_def(mProj4,0));
	printf("FileType: %d\n",mFileType);
	printf("GeoType: %d\n",mProjType);
	printf("******************************\n"); 
}

//************************************************************************
void cRaster::GetMap(Float2DArray &Raster, unsigned &Rows, unsigned &Cols)
{
	Rows = mRows;
	Cols = mCols;
	delete_Float2DArray(Raster);
	Raster = new_Float2DArray(mRows,mCols);
	copy_Float2DArray(Raster,mRaster,Rows,Cols);
}

//**************************************************************************
int cRaster::GetSize()
{
	return max(mRows,mCols);
}
