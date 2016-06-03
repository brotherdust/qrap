CC = g++ -g -Wall -W -D_REENTRANT -c -std=c++0x -std=gnu++0x -I/usr/include  -I/usr/include/postgresql -I/usr/include/gdal -I../Prediction -I/include -I/usr/include/qt4  -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/eigen3 -I/usr/include/c++/4.8.2/ -I/usr/local/include -I../libcmaes/ -I../libcmaes/src/

OBJECTS =  cTrafficDist.o cMemmngr.o cGeoP.o cAntpatn.o cLoadFile.o cClutter.o cPthlssp.o cProfile.o cLink.o cBTLPredict.o cCoveragePredict.o cPlotTask.o cRasterFileHandler.o cGDAL.o cBIN.o cRaster.o cSettings.o cAlert.o cBase.o cDatabase.o cSiteEditTools.o cMeasAnalysisCalc.o cMeasImportSpace.o cMeasImportCSV.o  cTrainPosNetDistAngle.o cPosEstimation.o cMeas.o cCellList.o cTPoint.o cTrainAntPattern.o main.o

all: rap makefile

#alles wat in main geinclude is
#main.o:   cgeneric.o main.cpp makefile
#	$(CC) main.cpp 

rap: $(OBJECTS) makefile
	c++ -g -o rap $(OBJECTS) -L/usr/local/pgsql/lib  -L/usr/lib/ -L/lib/-lpq -lpqxx -lgdal -lproj  -lssl -lPocoXML  -lPocoFoundation -lQtCore -ldoublefann -lfann -L../libcmaes/src/.libs/ -lcmaes


main.o: ../DataBase/Config.h main.cpp makefile
	$(CC) main.cpp

cMemmngr.o:  cMemmngr.h cMemmngr.cpp makefile
	$(CC) cMemmngr.cpp

cGeoP.o: cGeoP.h cGeoP.cpp makefile
	$(CC) cGeoP.cpp

cTPoint.o: cTPoint.h cTPoint.cpp makefile
	$(CC) cTPoint.cpp

cMeas.o: cMeas.h cMeas.cpp makefile
	$(CC) cMeas.cpp

cCellList.o: cCellList.h cCellList.cpp makefile
	$(CC) cCellList.cpp

cTrainAntPattern.o: cTrainAntPattern.h cTrainAntPattern.cpp makefile
	$(CC) cTrainAntPattern.cpp
	
#cMeasImportOldTEMS.o: cMeasImportOldTEMS.h cMeasImportOldTEMS.cpp makefile
#	$(CC) cMeasImportOldTEMS.cpp

cAntpatn.o: cAntpatn.h cMemmngr.o cAntpatn.cpp makefile
	$(CC) cAntpatn.cpp

cBTLPredict.o: cBTLPredict.h cMemmngr.o cGeoP.o cBTLPredict.cpp makefile
	$(CC) cBTLPredict.cpp

cCoveragePredict.o: cCoveragePredict.h cGeoP.o cAntpatn.o cMemmngr.o cCoveragePredict.cpp makefile
	$(CC) cCoveragePredict.cpp

cPlotTask.o: cPlotTask.h ../DataBase/Config.h cDatabase.o cRasterFileHandler.o cCoveragePredict.o cBTLPredict.o cGeoP.o cMemmngr.o cPlotTask.cpp makefile
	$(CC) cPlotTask.cpp

cRasterFileHandler.o: cRasterFileHandler.h ../DataBase/Config.h cDatabase.o cMemmngr.o cGeoP.o cRasterFileHandler.cpp makefile
	$(CC) cRasterFileHandler.cpp
	
cProfile.o:  cProfile.h cGeoP.o cProfile.cpp makefile 
	$(CC) cProfile.cpp
	
cLink.o:  cLink.h cClutter.o cLink.cpp makefile  
	$(CC) cLink.cpp

cClutter.o:  cClutter.h ../DataBase/Config.h cClutter.cpp makefile
	$(CC) cClutter.cpp

cMeasAnalysisCalc.o:  cMeasAnalysisCalc.h cMeasAnalysisCalc.cpp makefile
	$(CC) cMeasAnalysisCalc.cpp

cTrafficDist.o:  cTrafficDist.h cTrafficDist.cpp makefile
	$(CC) cTrafficDist.cpp

cMeasImportSpace.o:  cMeasImportSpace.h cMeasImportSpace.cpp makefile
	$(CC) cMeasImportSpace.cpp

cMeasImportCSV.o:  cMeasImportCSV.h cMeasImportCSV.cpp makefile
	$(CC) cMeasImportCSV.cpp

cPthlssp.o:  cPthlssp.h cClutter.o cMemmngr.o cGeoP.o cProfile.o cPthlssp.cpp makefile
	$(CC) cPthlssp.cpp

cGDAL.o: cGDAL.h cMemmngr.o cGDAL.cpp makefile
	$(CC) cGDAL.cpp

#cTrainPosNet.o: cTrainPosNet.h cGeoP.o ../DataBase/Config.h cTrainPosNet.cpp makefile
#	$(CC) cTrainPosNet.cpp

cTrainPosNetDistAngle.o: cTrainPosNetDistAngle.h cGeoP.o ../DataBase/Config.h cTrainPosNetDistAngle.cpp makefile
	$(CC) cTrainPosNetDistAngle.cpp

cPosEstimation.o: cPosEstimation.h cGeoP.o cAntpatn.o ../DataBase/Config.h cPosEstimation.cpp makefile
	$(CC) cPosEstimation.cpp

cSiteEditTools.o: ../DataBase/cSiteEditTools.h ../DataBase/cSiteEditTools.cpp makefile
	$(CC) ../DataBase/cSiteEditTools.cpp 

cLoadFile.o: cLoadFile.h cLoadFile.cpp makefile
	$(CC) cLoadFile.cpp

cORT.o: cORT.h cMemmngr.o cORT.cpp makefile
	$(CC) cORT.cpp

cBIN.o: cBIN.h cMemmngr.o cBIN.cpp makefile
	$(CC) cBIN.cpp

cSettings.o: ../DataBase/cSettings.h ../DataBase/cSettings.cpp makefile
	$(CC) ../DataBase/cSettings.cpp 
	
cAlert.o: ../DataBase/cAlert.h ../DataBase/cAlert.cpp makefile
	$(CC) ../DataBase/cAlert.cpp 

cBase.o: ../DataBase/cBase.h ../DataBase/cBase.cpp makefile
	$(CC) ../DataBase/cBase.cpp 
	
cDatabase.o: ../DataBase/cDatabase.h ../DataBase/cDatabase.cpp makefile
	$(CC) ../DataBase/cDatabase.cpp 

cRaster.o: cRaster.h ../DataBase/Config.h cGDAL.o cBIN.o cMemmngr.o cRaster.cpp makefile
	$(CC) cRaster.cpp
	
clean: 
	-rm -f *.o *~ *.out main


