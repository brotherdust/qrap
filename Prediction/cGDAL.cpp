 /*
 *    QRAP Project
 * 
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cGDAL.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Dirk van der Merwe
 *                : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : Class enabling handling of general file formats that
 *                : can be handled by gdal. Slots into cRaster
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

#include "cGDAL.h"
using namespace Qrap; 
 
//********************************************************************
cGDAL::cGDAL()
{
	GDALAllRegister(); 
}

//***************************************************************************
cGDAL::~cGDAL()
{
}

//***************************************************************************
bool cGDAL::openFile(Float2DArray &Raster,string Directory, string FileName, 
			cGeoP &NW, cGeoP &SE, GeoType &Proj, projPJ &Proj4,
			uint &rows, uint &cols, double &ns_res, double &ew_res,
			float &min,	float &max)
{
	Rows = 0;
	Cols = 0;
	string FN = Directory+"/" +FileName;
	m_file_name = FN;
//	cout << " In cGDAL::openFile(...) before GDALDataset constructor." << endl;
	GDALDataset *poDataset;
//	cout << " In cGDAL::openFile(...) before GDALOpen ..." << endl;
	poDataset = (GDALDataset *) GDALOpen( m_file_name.c_str(), GA_ReadOnly );
	if( poDataset == NULL )
	{
		string err = "Error reading file: ";
		err+=FileName;
		err+=" before PlotType. en of file reached";
		QRAP_ERROR(err.c_str());
		printf("We have a null");
		//\TODO:Error message
		return false;
	}
//	printf("The GDAL file is open");
	/* Getting the meta data */

/*	printf( "Driver: %s/%s\n",
    	poDataset->GetDriver()->GetDescription(),
    	poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
//	printf( "Size is %dx%dx%d\n",
    	poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
    	poDataset->GetRasterCount() );
*/	
	if( poDataset->GetProjectionRef() != NULL ) 
	{
		poSRS = OGRSpatialReference(poDataset->GetProjectionRef());
		char *pszProjWKT[] = {NULL};

		poSRS.exportToProj4( pszProjWKT ); 

		if ((Proj4=pj_init(sizeof(pszProjWKT)/sizeof(char*),pszProjWKT)))
		{
//			printf("Initialised Proj4: %s\n",pj_get_def(Proj4,0));	
			Proj =NDEF;
		}
		else
		{
			//printf("Error in: %s",pszProjWKT);
			
			char *pszProjWKT1[] = {"proj=latlong"};
			if ((Proj4=pj_init(sizeof(pszProjWKT1)/sizeof(char*),pszProjWKT1)))
			{
//				printf("Initialised Proj4: %s\n",pj_get_def(Proj4,0));	
				Proj = DEG;
			}
			else printf("Error in: %s",*pszProjWKT);
//			CSLDestroy( pszProjWKT1 );
		}
//		CSLDestroy( pszProjWKT );
	}
	else Proj = DEG;

//	cout << " In cGDAL::openFile(...) before Getting the raster band ..." << endl;
	/* Getting the raster band */
	poBand = poDataset->GetRasterBand( 1 );
	poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
//	printf( "Block=%dx%d Type=%s, ColorInterp=%s\n", nBlockXSize, nBlockYSize,
//			GDALGetDataTypeName(poBand->GetRasterDataType()),
//			GDALGetColorInterpretationName(poBand->GetColorInterpretation()) );
	adfMinMax[0] = poBand->GetMinimum( &bGotMin );
	adfMinMax[1] = poBand->GetMaximum( &bGotMax );
	if( ! (bGotMin && bGotMax) )
		GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);
//	printf( "Min=%.3fd, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
	min = adfMinMax[0];
	max = adfMinMax[1];
//	if( poBand->GetOverviewCount() > 0 )
//		printf( "Band has %d overviews.\n", poBand->GetOverviewCount() );
//	if( poBand->GetColorTable() != NULL )
//		printf( "Band has a color table with %d entries.\n",poBand->GetColorTable()->GetColorEntryCount() );
	if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
	{
//		printf( "Origin = (%.6f,%.6f)\n",adfGeoTransform[0], adfGeoTransform[3] );
        	Rows = poBand->GetYSize();
		Cols = poBand->GetXSize();
		rows = Rows;
		cols = Cols;
//		printf( "Pixel Size = (%.6f,%.6f)\n", adfGeoTransform[1], adfGeoTransform[5] );
        	ns_res = adfGeoTransform[5];
        	ew_res = adfGeoTransform[1];
        	NW.Set(adfGeoTransform[3],adfGeoTransform[0],Proj,-1);
        	SE.Set((adfGeoTransform[3]+ns_res*rows),(adfGeoTransform[0]+ew_res*cols),Proj,-1);
//        	printf("Rotation (0 = north is up): %d\n",(int)adfGeoTransform[2]);
	}
	
	if (Rows <1 || Cols < 1)
	{
		printf("Error\nRows:%d\tCols%d\n",Rows,Cols); 
		//\TODO:Error message
		return false;
	}
//	cout << Raster << endl;
    	delete_Float2DArray(Raster);
//    	cout << Raster << endl;
	Raster = new_Float2DArray(Rows,Cols);
//	cout << Raster << endl;

	float *pafScanline;
	pafScanline = new float[Cols];
	if (adfGeoTransform[2]==0)
	{
		for (unsigned row = 0;row < Rows;row++)
		{
			//X = Cols ; Y = Rows
			poBand->RasterIO( GF_Read, 0, row, Cols, 1,
								pafScanline, Cols, 1, GDT_Float32, 0, 0 );
  			for (unsigned col = 0; col < Cols ; col++)
     			Raster[row][col] = pafScanline[col];
		}
	}
	else
	{
		for (unsigned row = Rows-1; row <= 0;row--)
		{
			poBand->RasterIO( GF_Read, 0, 0, row, Cols,
            	      			pafScanline, Cols, 1, GDT_Float32, 0, 0 );
  			for (unsigned col = 0; col < Cols ; col++)
     			Raster[row][col] = pafScanline[col];
		}
	}
	delete [] pafScanline;
    	ns_res = fabs(ns_res); 	// MS added fabs 2008-04-16
    	ew_res = fabs(ew_res);	// MS added fabs 2008-04-16

    	if( poDataset != NULL )
        	GDALClose( (GDALDatasetH) poDataset );
//	printf("Read GDAL\n");

	return true;
}

//**************************************************************************
bool cGDAL::writeFile(Float2DArray &Raster,
				string Directory, 
				string FileName, 
				cGeoP &NW, 
				GeoType &InProj,
				GeoType &OutProj, 
				projPJ &InProj4,
				projPJ &OutProj4,
				uint &rows, 
				uint &cols,
				int CentMer,
				string type,
				double NSRes,
				double EWRes)
{
	GDALDataset *poDataset=NULL;
	const char *pszFormat = type.c_str();
	GDALDriver *poDriver=NULL;
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	char **papzOptions = NULL;
	string extGDAL;
	GDALDataType PxType =  GDT_Float32;
	string File = Directory + "/" + FileName;
	poDataset = poDriver->Create( File.c_str(), cols, rows, 1, 
					PxType, papzOptions );
    	if (poDataset == NULL)
    	{
    		//\TODO: Error Message
    		cout << "Error creating GDAL File" << endl;
    		return false;
   	}
    
	poBand = poDataset->GetRasterBand(1);
	char **papszMetadata;
	
	papszMetadata = poDataset->GetMetadata(NULL);
	CSLSetNameValue( papszMetadata,"Generated Map","QRAP");
	
	double North, West;
	int TempCentMer;
	bool South=NW.Hemisphere();
	GeoType Temp;
	NW.SetGeoType(OutProj,CentMer); // Transform
	NW.Get(North,West,Temp,TempCentMer,South);
	if ((South) && (NSRes > 0))
		NSRes = - NSRes;
	else if ((!(South)) && (NSRes > 0))
		NSRes = -NSRes;

//	cout << "before GetGeoTrasform" << endl;
//	cout << pj_get_def(OutProj4,0) << endl;
	double *adfGeoTransform;
	adfGeoTransform = new double[6];
	adfGeoTransform[0]=West;
	adfGeoTransform[1]=EWRes;
	adfGeoTransform[2]=0.0;
	adfGeoTransform[3]=North;
	adfGeoTransform[4]=0.0;
	adfGeoTransform[5]=NSRes;
	poDataset->SetGeoTransform( adfGeoTransform );

	OGRSpatialReference oSRS;
	
	//oSRS.importFromProj4( pj_get_def(OutProj4,0) ); // Transform
	oSRS.importFromProj4( ReturnProj4(OutProj,CentMer).c_str() ); // Transform
	//cout << pj_get_def(OutProj4,0) << endl;
	char *pszSRS_WKT = NULL;;
	oSRS.exportToWkt( &pszSRS_WKT );
	poDataset->SetProjection( pszSRS_WKT );
	CPLFree( pszSRS_WKT );
	CPLErr lCrGdal;

//	cout << "Converting Raster to single array: "<<endl;
	unsigned i,j,k,l;
	l = rows*cols;
	float *abyRaster;
	abyRaster = new float[l];

	for (i = 0,k=0; i < rows*cols ; i+=cols,k++)
		for (j = 0; j < cols ; j++)
		{
			abyRaster[j+i] = Raster[k][j];
		}
//	cout << "Done with Conversion" <<endl;

	lCrGdal = poBand->RasterIO( GF_Write, 0, 0, cols, rows,
                  abyRaster, cols, rows, PxType, 0, 0 );
	
	if (lCrGdal == CE_Failure)
 	{
 		//\TODO: Error Message
 		cout << "Error writing GDAL File " << endl; 
 		return false;
 	}
 	
// 	GDALColorTable *Colors = new GDALColorTable();
// 	GDALColorEntry ColorBandMin ={255,255,255,0};
// 	GDALColorEntry ColorBandMax ={0,0,0,0};
 	//Colors->CreateColorRamp(-179,ColorBandMin,50,ColorBandMax);
 	
 	
 	if (poDataset != NULL)
 	{
 		delete poDataset;
 		poDataset = NULL;
 	}
 	GDALDestroyDriverManager();
 	delete [] adfGeoTransform;
	delete [] abyRaster;
// 	cout <<"Fine" << endl;
	return true;
}

//******************************************************************
string cGDAL::ReturnProj4(GeoType Type,int CentMer, bool Hem)
{
	char centmer[100];
		char southstr[100];
		string Proj4 = "";
		switch (Type) 
		{
			case DEG:
				Proj4 = "+latlong +ellps=WGS84 +datum=WGS84 +no_defs";
//				static char *DEGparms[] = {	"proj=latlong",	"ellps=WGS84",	
//											"datum=WGS84",	"no_defs"};
//				Proj = pj_init(sizeof(DEGparms)/sizeof(char*),DEGparms);
				break;
			case WGS84GC:
				sprintf(centmer,"+proj=tmerc +ellps=WGS84 +lon_0=%d +units=m +no_defs",CentMer);
				Proj4 = centmer;
//				sprintf(centmer,"lon_0=%d",PointCM);
//				static char *WGS84GCparms[] = {"proj=tmerc","ellps=WGS84",
//												centmer,"units=m","no_defs"};
//				Proj = pj_init(sizeof(WGS84GCparms)/sizeof(char*),WGS84GCparms);
				break;
			case UTM:
				if (Hem) {
					sprintf(southstr,"south");
					}
					else {
					sprintf(southstr,"north");
					}
				sprintf(centmer,"+proj=utm +zone=%d +%s",(int)ceil((180+CentMer)/6),southstr);
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
		return Proj4;
}
