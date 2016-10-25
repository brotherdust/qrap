 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : cLoadFile.cpp
 *    Copyright   : (c) University of Pretoria
 *    Author      : Magdaleen Ballot (magdaleen.ballot@up.ac.za)
 *    Description : The actual method used to do raster file-loading 
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

#include "cLoadFile.h"
using namespace Qrap;

//*****************************************************************************
cLoadFile::cLoadFile()
{
	mMetaData.clear();
	mGlobalCounter = 0;
	mSourceFileSet = 0;
	mBinaryFileSet = 0;
	mBinaryDirectory = "";
}

//********************************************************************************
cLoadFile::cLoadFile(unsigned SourceFileSet, unsigned BinaryFileSet, string BinaryDirectory)
{
	mSourceFileSet = SourceFileSet;
	mBinaryFileSet = BinaryFileSet;
	mBinaryDirectory = BinaryDirectory;
	mMetaData.clear();
	mGlobalCounter = 0;
}

//**************************************************************************
cLoadFile::~cLoadFile()
{
}

//**************************************************************************
bool cLoadFile::LoadFile(string SrcDirectory, 
			string FileName,
			FileType filetype,
			GeoType Proj,
			string Proj4String,
			int CentMer,
			bool Hem)
{
	StructMetaData MapHeader;
	cRaster INRaster(SrcDirectory, FileName, filetype, Proj, Proj4String, CentMer,Hem);

	cGeoP NW,SE;
	FileType Type;
	unsigned Rows,Cols;
	double NSres,EWres;
	int MapCentMer;
	double degNorth, degSouth, degEast, degWest;
	INRaster.GetHeader(NW, SE, MapCentMer, Rows, Cols, NSres, EWres, Proj, Proj4String, Type);
	if (Proj != DEG)
	{
//		cout << "In cLoadFile::LoadFile ... (Proj != DEG) " << endl;
		NW.SetGeoType(DEG);
		SE.SetGeoType(DEG);
	}
	NW.Get(degNorth,degWest);
	SE.Get(degSouth,degEast);
//	NW.Display();
//	SE.Display();
	MapHeader.North = degNorth;
	MapHeader.South = degSouth;
	MapHeader.East = degEast;
	MapHeader.West = degWest;
	MapHeader.CentralMeridian = MapCentMer;
	MapHeader.Resolution = INRaster.GetRes();
	MapHeader.Proj = Proj;
	MapHeader.Proj4 = Proj4String;
	MapHeader.Location = SrcDirectory;
	MapHeader.FileName = FileName;
	MapHeader.FileSet = mSourceFileSet;
	MapHeader.Filetype = Type;
	mMetaData.push_back(MapHeader);
//	cout << "Uit Loadfile" << endl; 
	return true;
}

//************************************************************************
bool cLoadFile:: WriteDB()
{
	
	StringMap Vals;
	char *North=new char[33];
	char *South=new char[33];
	char *East=new char[33];
	char *West=new char[33];
	char *CentM=new char[33];
	char *text=new char[33];
	string query;
	cout <<  "Number of Maps being saved in Database: " << mMetaData.size() << endl;
	for (unsigned i = 0; i < mMetaData.size();i++)
	{
		cout << "Count Write: " << i << endl;
		gcvt(mMetaData[i].CentralMeridian,8,CentM);

		gcvt(mMetaData[i].North,10,North);
		gcvt(mMetaData[i].South,10,South);
		gcvt(mMetaData[i].East,10,East);
		gcvt(mMetaData[i].West,10,West);
		query ="INSERT INTO sourcefiles (lastmodified, machineid, filesetkey,";
		query +=" filename,location, centmer,areasquare) VALUES (now(), ";
		gcvt(gDb.globalMachineID,10,text);
		query +=text; 
		query += ", ";
		gcvt(mMetaData[i].FileSet,10,text);
		query +=text; 
		query += ", ";
		query += "'" + mMetaData[i].FileName + "', ";
		query += "'" + mMetaData[i].Location + "', ";
		query +=CentM; 
		query += ", ";	
		query +="BOX'((";
		query += North; 
		query += "," ;
		query += West; 
		query +="),("; 
		query += South; 
		query += ","; 
		query += East; 
		query +="))');";
/*		query += "POLYGON((" + North + " " + West +\
							"," + North + " " + East +\
							"," + South + " " + East +\
							"," + South + " " + West +\
							"," + North + " " + West +"))',4326))";*/
		cout << "query: " << query << endl;
		if ((!gDb.PerformRawSql(query)))
		{
			cout << "gDb failed Count Write: " << i << endl;
			cout << "Database Update on filesets table failed"<< endl;
			return false;
		}
			
	}
	delete [] North;
	delete [] South;
	delete [] East;
	delete [] West;
	delete [] CentM;
	delete [] text;
	mMetaData.clear();
	return true;
}

//************************************************************************
bool cLoadFile::LoadTextList(string Directory, string List, FileType filetype,
							GeoType Proj, string Proj4String, 
							int CentMer, bool Hem)
{
	ifstream fp;
	char *st = new char[255];
	string str;
	string Extension;
	string fileList = Directory + "/" + List;
	fp.open(fileList.c_str(),ios::in);
	if (!fp.rdbuf()->is_open())
	{
		cout << "could not open " << fileList << endl;
		return 0;
	}
	while (!fp.eof())
	// for the entire filelist
	{
		fp.getline(st,255);
		str = st;
		if (str.length() > 3)
		{
			Extension = str.substr(str.length()-3);
			if (Extension == "HED" || Extension == "hed")
			{
				fp.getline(st,255);
			}
			printf("%s\n",st);
			LoadFile(Directory,st,filetype,Proj,Proj4String,CentMer,Hem);
		}
	}
	delete [] st;
	fp.close();
	WriteDB();
	return true;
}

//***************************************************************
bool cLoadFile::CutCurrentSet(	unsigned OriginalFileSet,
                                unsigned ResultFileSet, bool Interpolate,
                                short int inFileType)
{
	cout << " In cLoadFile::CutCurrentSet(..). OriginalFileSet=" 
		<< OriginalFileSet << "	ResultFileSet=" << ResultFileSet <<
		"	inFileType=" << inFileType << "	Interpolate=";

	if (Interpolate)
		cout << "True";
	else cout << "False";
	cout << endl;

	string Type, Oproj4, Rproj4, projection;
	MetaData OldMetaData;
	FileType Ofiletype, Rfiletype;
	GeoType OrigProj, ResultProj;
	double OrigRes,ResultRes;
	unsigned i,j;
	pqxx::result r;
	string query;
	char *OFSID, *RFSID;
	OFSID = new char[33];
	RFSID = new char[33];
	gcvt(OriginalFileSet,9,OFSID);
	query = "SELECT resolution, fileformat, projection, proj4string ";
	query += "FROM filesets WHERE ID=";
	query += OFSID;
	query += ";";

	cout << "cLoadFile::CutCurrentSet query: " << query << endl;
	if(!gDb.PerformRawSql(query))
	{
		string err = "Database Select for Original FileSet failed. Query: ";
		err += query;
		cout << err << endl;
//		QRAP_WARN(err.c_str());
		return false;
	}
	else
	{
		cout << "Voor get result original file set query" << endl;
		gDb.GetLastResult(r);
		
		cout << "Got result original file set query. r.size()="<< r.size() << endl;
		if(r.size()>0)
		{
			OrigRes = atof(r[0]["resolution"].c_str());
			Type = r[0]["fileformat"].c_str();
			if 	(Type=="BINFILE") 	Ofiletype=BINFILE;
			else if (Type=="NESFILE") 	Ofiletype=NESFILE;
			else if (Type=="GRASSFILE")	Ofiletype=GRASSFILE;
			else if (Type=="GDALFILE")	Ofiletype=GDALFILE;
			else if (Type=="ORTFILE")	Ofiletype=ORTFILE;
			Oproj4 = 	r[0]["proj4string"].c_str();
			projection 	= 	r[0]["projection"].c_str();
			if (projection=="DEG") 				OrigProj=DEG;
			else if (projection=="WGS84GC") 	OrigProj=WGS84GC;
			else if (projection=="UTM")			OrigProj=UTM;
			else OrigProj=NDEF;
		} // if r.size()
		else
		{
			string err = "Database Select for Original FileSet is empty. Query: ";
			err += query;
			cout << err << endl;
//			QRAP_WARN(err.c_str());
			return false;
		}
	}

	cout << "Na original file set query." << endl;
	gcvt(ResultFileSet,9,RFSID);
	query = "SELECT resolution, fileformat, projection, proj4string ";
	query += "FROM filesets WHERE ID=";
	query += RFSID;
	query += ";";
	
	cout << "cLoadFile::CutCurrentSet query: " << query << endl; 	
	if(!gDb.PerformRawSql(query))
	{
		string err = "Database Select for Result FileSet failed. Query: ";
		err += query;
		cout << err << endl;
//		QRAP_WARN(err.c_str());
		return false;
	}
	else
	{
		gDb.GetLastResult(r);
		
		if(r.size()!=0)
		{
			ResultRes = atof(r[0]["resolution"].c_str());
			Type = r[0]["fileformat"].c_str();
			if 	(Type=="BINFILE") 	Rfiletype=BINFILE;
			else if (Type=="NESFILE") 	Rfiletype=NESFILE;
			else if (Type=="GRASSFILE")	Rfiletype=GRASSFILE;
			else if (Type=="GDALFILE")	Rfiletype=GDALFILE;
			else if (Type=="ORTFILE")	Rfiletype=ORTFILE;
			Rproj4 = 	r[0]["proj4string"].c_str();
			projection = 	r[0]["projection"].c_str();
			if (projection=="DEG") 		ResultProj=DEG;
			else if (projection=="WGS84GC") ResultProj=WGS84GC;
			else if (projection=="UTM")	ResultProj=UTM;
			else 				ResultProj=NDEF;
		} // if r.size()
		else
		{
			string err = "Database Select for Result FileSet is empty. Query: ";
			err += query;
			cout << err << endl;
//			QRAP_WARN(err.c_str());
			return false;
		}
	}
	cout << "Na result file set query." << endl;
	
	StructMetaData MapHeader;
	double maxN=-90.0, minW=360.0, minS=90.0, maxE=0.0; 
	
	query = "SELECT filename, location, centmer, ";
	query += " NE[0] as N, NE[1] as E, SW[0] as S, SW[1] as W ";
	query += " FROM (SELECT filename, location, centmer, ";
	query += " areasquare[0] as NE, areasquare[1] as SW";
	query += " FROM sourcefiles WHERE filesetkey=";
	query += OFSID;
	query += ") as tussen;";

	
	cout << "cLoadFile::CutCurrentSet query: " << query << endl;
	if(!gDb.PerformRawSql(query))
	{
		string err = "Database Select to get Original Files info failed. Query: ";
		err += query;
		cout << err << endl;
//		QRAP_WARN(err.c_str());
		return false;
	}
	else
	{
		cout  <<"cLoadFile::CutCurrentSet. Getting content of original file set. r.size() = " << r.size() << endl; 
		gDb.GetLastResult(r);
		
		if (r.size()==0)
		{
			string err = "Database Select to get Original Files is empty. Query: ";
			err += query;
			cout << err << endl;
//			QRAP_WARN(err.c_str());
			return false;	
		}
		
		for (i=0; i< r.size(); i++)
		{
			cout  <<"cLoadFile::CutCurrentSet. Preparing OldMetaData  i=" << i << endl;
			MapHeader.North = atof(r[i]["N"].c_str());
			MapHeader.South = atof(r[i]["S"].c_str());
			MapHeader.East = atof(r[i]["E"].c_str());
			MapHeader.West = atof(r[i]["W"].c_str());
			if (maxN < MapHeader.North) maxN = MapHeader.North;
			if (minW > MapHeader.West) 	minW = MapHeader.West;
			if (minS > MapHeader.South) minS = MapHeader.South;
			if (maxE < MapHeader.East) 	maxE = MapHeader.East;
			MapHeader.CentralMeridian = atoi(r[i]["centmer"].c_str());
			MapHeader.Resolution=OrigRes;
			MapHeader.Proj=OrigProj;
			MapHeader.Proj4=Oproj4;
			MapHeader.Location = r[i]["location"].c_str();
			MapHeader.FileName = r[i]["filename"].c_str();
			MapHeader.FileSet=OriginalFileSet;
			MapHeader.Filetype=Ofiletype;
			OldMetaData.push_back(MapHeader);
		} // for i to r.size()
	}

	delete [] OFSID;
	delete [] RFSID; 

	cout << "Read Current Database" << endl;
	bool swap=false;
	for (i=0; i<OldMetaData.size(); i++)
	{
		for (j=i; j<OldMetaData.size(); j++)
		{
			swap = ((OldMetaData[j].North>OldMetaData[i].North) ||
					(((OldMetaData[j].North==OldMetaData[i].North) &&
					  (OldMetaData[j].West<OldMetaData[i].West))));
			if (swap)
			{
				MapHeader = CopyMapHed(OldMetaData[i]);
				OldMetaData[i] = CopyMapHed(OldMetaData[j]);
				OldMetaData[j] = CopyMapHed(MapHeader);
			}
		}
	}
	
	cout << "Done ordering old meta data " << endl;
	unsigned Rows, Cols, Size; // (Estimated) Size of map in pixels
	double SizeEW, SizeNS; // Size of map in degrees
	
	SizeEW = 0.5; // Size of map in degrees
	SizeNS = 0.5; 
	Rows = (unsigned) (SizeNS*100000 / OrigRes);
	Cols = (unsigned) (SizeEW*100000 / OrigRes);
	Size = Rows*Cols;
	while (Size> 100000000) 
	{
		if (Cols>Rows)	SizeEW/=2.0;
		else			SizeNS/=2.0;
		Rows = (unsigned)(SizeNS*100000 / OrigRes);
		Cols = (unsigned)(SizeEW*100000 / OrigRes);
		Size = Rows*Cols;	
	}

	unsigned NumMapRows, NumMapCols;
	NumMapRows = (unsigned)((maxN-minS)/SizeNS +1.0);
	NumMapCols = (unsigned)((maxE-minW)/SizeEW +1.0);

	cout << "Before float MapLoaded" << endl;
	Float2DArray MapLoaded = new_Float2DArray(NumMapRows,NumMapCols);
	for (i=0; i<NumMapRows; i++)
		for (j=0; j<NumMapCols; j++)
			MapLoaded[i][j]=0.0;

	cRasterFileHandler RastersSet;
	bool RValue;
//        RastersSet.DirectChangeSetToUse(OriginalFileSet);
        RastersSet.SetRasterFileRules(inFileType);
        if (!Interpolate) RastersSet.SetSampleMethod(1);
	StructMetaData NewHeader;
	cGeoP NW, SE, NE, SW, Mid, inNW, inSE, inSW, inNE;
	Float2DArray NewRaster = new_Float2DArray(2,2);
	unsigned nRows, nCols;
	int nCentMer;
	double EWres, NSres, NewEWRes, NewNSRes, NewRes,res, Ndist, Sdist, Wdist, Edist;
	double gN,gS,gE,gW, inN, inS, inW, inE,S,E,N,W, dummy;
	bool Hem=true;
	bool ValidMap;
	double minValue=0.0, maxValue=1000.0;
	double tempNSres,tempEWres;
	char *temp;
	temp = new char[33]; 
	string nProj4String;
	GeoType nProj;
	FileType FType;
	
	int row=0, col=0;
	minW = SizeEW*(int)round(minW/SizeEW); // start at the closest "whole" SizeEW 
	maxN = SizeNS*(int)round(maxN/SizeNS); // start at the closest whole SizeNS 
	for (i=0; i<OldMetaData.size(); i++)
	{
		{// these seemingly unrequired brackets are to limit the scope of INRaster and save some memory.
			cRaster INRaster;
			INRaster.ReadFile(	OldMetaData[i].Location, 
						OldMetaData[i].FileName,
						OldMetaData[i].Filetype,
						OldMetaData[i].Proj,
						OldMetaData[i].Proj4,
						OldMetaData[i].CentralMeridian,
							Hem,minValue,maxValue);
			// note that previous estimation of Rows and Cols are now overriden with info of laoded map
			INRaster.GetHeader(	inNW, inSE, OldMetaData[i].CentralMeridian, 
						Rows, Cols, NSres, EWres, 
						OldMetaData[i].Proj, 
						OldMetaData[i].Proj4, 
						MapHeader.Filetype);
		} // these seemingly unrequired brackets are to limit the scope of INRaster and save some memory.
		
		cout << "###################################################################" << endl;
		cout << " Loaded: " << OldMetaData[i].FileName << endl;
		cout << " Rows: " << Rows << "	Cols: " << Cols << endl;
		cout << "###################################################################" << endl;		

		inNW.SetGeoType(OldMetaData[i].Proj, OldMetaData[i].CentralMeridian);
		inSE.SetGeoType(OldMetaData[i].Proj, OldMetaData[i].CentralMeridian);
		inNW.Get(inN, inW, OldMetaData[i].Proj, OldMetaData[i].CentralMeridian, Hem);
		inSE.Get(inS, inE, OldMetaData[i].Proj, OldMetaData[i].CentralMeridian, Hem);
		inNE.Set(inN, inE, OldMetaData[i].Proj, OldMetaData[i].CentralMeridian);
		inSW.Set(inS, inW, OldMetaData[i].Proj, OldMetaData[i].CentralMeridian);
		inNW.Display();
		inNE.Display();
		inSW.Display();
		inSE.Display();
		Wdist = inNW.Distance(inSW);
		Edist = inNE.Distance(inSE);
		Ndist = inNW.Distance(inNE);
		Sdist = inSW.Distance(inSE);
		NewNSRes = min(Wdist,Edist)/Cols; //resolution 
		NewEWRes = min(Ndist,Sdist)/Rows; //resolution
		NewRes = min(NewNSRes,NewEWRes);	
		if (NewRes>ResultRes)
		{
			NewRes = ResultRes;
			NewNSRes = ResultRes;
			NewEWRes = ResultRes;
		}
		if ((ResultProj==DEG)&&(OrigProj==DEG)&&(OrigRes<=ResultRes*1.1))
		{
			NewRes = OrigRes;
			NewNSRes = NewRes;
			NewEWRes = NewRes;
		}
			
		row = (int)round((maxN - OldMetaData[i].North)/SizeNS);
		while ((maxN-row*SizeNS>OldMetaData[i].South)&&(row<NumMapRows))
		{
			col = (int)round((OldMetaData[i].West-minW)/SizeEW);
			cout << "row: " << row << "	col: " << col << endl;
			while ((minW+col*SizeEW<OldMetaData[i].East)&&(col<NumMapCols))
			{
				cout << "row: " << row << "	col: " << col << endl;
				if (MapLoaded[row][col]<1.0)
				{
					N = maxN - row*SizeNS;
					S = maxN - (row+1)*SizeNS;
					E = minW + (col+1)*SizeEW;
					W = minW + col*SizeEW;
					NW.Set(N,W,DEG);
					SE.Set(S,E,DEG);
					NE.Set(N,E,DEG);
					SW.Set(S,W,DEG);
					Wdist= NW.Distance(SW);
					Edist = NE.Distance(SE);
					Ndist = NW.Distance(NE);
					Sdist = SW.Distance(SE);			
					Mid.Set((N+S)/2.0,(E+W)/2.0,DEG);
					nCentMer = Mid.DefaultCentMer(ResultProj);
					res = NewRes;
					NW.FromHere(Mid, res*round(max(Wdist,Edist)/2/res+1.5),0);
					SE.FromHere(Mid, res*round(max(Wdist,Edist)/2/res+1.5),180);
					NW.Get(gN,dummy);
					SE.Get(gS,dummy);
					NW.FromHere(Mid, res*round(max(Ndist,Sdist)/2/res+1.5),270);
					SE.FromHere(Mid, res*round(max(Ndist,Sdist)/2/res+1.5),90);
					NW.Get(dummy,gW);
					SE.Get(dummy,gE);
					NW.Set(gN,gW,DEG);
					SE.Set(gS,gE,DEG);
					cout << "Small map coordinates:"<< endl;
					NW.Display();
					SE.Display();
					char *BinName;
					BinName = new char[100];
					ValidMap = RastersSet.GetForDEM(NW,SE,res,nRows,nCols,NewRaster,ResultProj);

					if (ValidMap)
					{
						if (ResultProj==DEG)
							res*=cDegResT;
						sprintf(BinName,"%s%d%d%d.BIN",
								OldMetaData[i].FileName.c_str(),
								row,col,mGlobalCounter++);
						printf("Creating File: %s\n",BinName);
						cRaster NewOutPutMap;
						NewHeader.North = N;
						NewHeader.South = S;
						NewHeader.East = E;
						NewHeader.West = W;
						gcvt(nCentMer,8,temp);
						switch (ResultProj)
						{
							case DEG:	nProj4String= "+latlong +ellps=WGS84 +datum=WGS84 +no_defs"; 
									break;
							case WGS84GC: 	nProj4String = "+proj=tmerc +ellps=WGS84 +lon_0=";
									nProj4String +=temp;
									break;
							case UTM:	nProj4String = "+proj=utm +zone=";
									nProj4String +=temp;
									if (Hem) nProj4String += " +south";
									else  nProj4String += " +north";
									break;
							default:	nProj4String= "+latlong +ellps=WGS84 +datum=WGS84 +no_defs";  
						}
						
						NewOutPutMap.WriteFile(NewRaster,NW,SE,nRows,nCols,res,res,
								mBinaryDirectory,BinName,BIN,ResultProj,ResultProj,
								nCentMer, nCentMer,nProj4String);
						NewOutPutMap.Display();
						NW.Set(N,W,DEG);
						SE.Set(S,E,DEG);
						NewOutPutMap.GetHeader(NW, SE, nCentMer, nRows, nCols, 
									tempNSres, tempEWres, nProj, nProj4String, FType);

						NewHeader.CentralMeridian = nCentMer;
						NewHeader.Resolution = NewOutPutMap.GetRes();
						NewHeader.Proj = ResultProj;
						NewHeader.Proj4 = nProj4String;
						NewHeader.Location = mBinaryDirectory;
						NewHeader.FileName = BinName;
						NewHeader.FileSet = ResultFileSet;
						NewHeader.Filetype = BINFILE;
						mMetaData.push_back(NewHeader);
						MapLoaded[row][col]=10;
					}
					else
						printf("This is a NULL map - discarded\n");
					delete [] BinName;
				}//if not already loaded
				col++;
			}//While cols
			row ++;
		}//while row
		cout << "Finished File"<<endl;
		RValue=WriteDB();
	}//go through all files
	delete_Float2DArray(NewRaster);
	OldMetaData.clear();
	delete [] temp;
	delete_Float2DArray(MapLoaded);
	return RValue;
}

//***************************************************************
bool cLoadFile::LoadOrt(string sourceDirectory, string resultDirectory, 
						string resultFileName, unsigned ResultFileSet, int CentMer)
{
	StructMetaData MapHeader;
	double Min=0.0, Max=0.0, NSres=50.0, EWres=50.0;
	bool South=true;
	cGeoP NW, SE;
	unsigned Rows=2, Cols=2;
	Float2DArray Data;
	char *temp;
	temp = new char[33]; 
	gcvt(CentMer,8,temp);
	MapHeader.CentralMeridian = CentMer;
	MapHeader.Proj = WGS84GC;
	MapHeader.FileName = resultFileName;
	MapHeader.FileSet = ResultFileSet;
	MapHeader.Filetype = BINFILE;
	MapHeader.Location = resultDirectory;
	
	cRaster Output(sourceDirectory, resultFileName, ORTFILE, 
					MapHeader.Proj, MapHeader.Proj4, CentMer, South, Min, Max); 

	Data = new_Float2DArray(Rows, Cols);
	Output.GetMap(Data,Rows,Cols);
	Output.GetHeader(NW, SE, CentMer, Rows, Cols, NSres, EWres, 
					MapHeader.Proj, MapHeader.Proj4, MapHeader.Filetype);
	NW.SetGeoType(DEG);
	SE.SetGeoType(DEG);
	NW.Get(MapHeader.North, MapHeader.West);
	SE.Get(MapHeader.South, MapHeader.East);
	MapHeader.Proj4 = "+proj=tmerc +ellps=WGS84 +lon_0=";
	MapHeader.Proj4 +=temp;
	MapHeader.Resolution=Output.GetRes();
	Output.WriteFile(Data, NW, SE, Rows, Cols, NSres, EWres,
					MapHeader.Location, MapHeader.FileName,	BIN,
					MapHeader.Proj,MapHeader.Proj,CentMer,CentMer);
	mMetaData.push_back(MapHeader);
	delete [] temp;
	delete_Float2DArray(Data);
	return WriteDB();
}
