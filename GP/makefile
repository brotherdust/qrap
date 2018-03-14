CC = g++ -g -std=c++11 -std=gnu++11 -Wall -W -D_REENTRANT -c -I/include -I/usr/include/qt4 -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore -I/usr/include -I/usr/include/postgresql -I/usr/include/gdal -I../Prediction -I../DataBase  -I/usr/include/eigen3 -I/usr/local/ -I/usr/include/c++/5.4.0/

OBJECTS =  cMemmngr.o cGeoP.o cAntpatn.o cClutter.o cPthlssp.o cProfile.o cRasterFileHandler.o cGDAL.o cBIN.o cRaster.o cSettings.o cAlert.o cBase.o cDatabase.o cSiteEditTools.o cMeasAnalysisCalc.o cGPpropModel.o cGPFunctions.o main.o

all: GP makefile

GP: $(OBJECTS) makefile
	c++ -g -o GP $(OBJECTS) -L/usr/local/pgsql/lib  -L/usr/lib/ -L/lib/-lpq -lpqxx -lgdal -lproj  -lssl -lQtCore -ldoublefann -lfann -lPocoXML  -lPocoFoundation -lpthread

main.o: main.cpp makefile
	$(CC) main.cpp

cSettings.o: ../DataBase/cSettings.h ../DataBase/cSettings.cpp makefile
	$(CC) ../DataBase/cSettings.cpp 
	
cAlert.o: ../DataBase/cAlert.h ../DataBase/cAlert.cpp makefile
	$(CC) ../DataBase/cAlert.cpp 

cBase.o: ../DataBase/cBase.h ../DataBase/cBase.cpp makefile
	$(CC) ../DataBase/cBase.cpp 
	
cDatabase.o: ../DataBase/Types.h ../DataBase/cDatabase.h ../DataBase/cDatabase.cpp makefile
	$(CC) ../DataBase/cDatabase.cpp 

cSiteEditTools.o: ../DataBase/cSiteEditTools.h ../DataBase/cSiteEditTools.cpp makefile
	$(CC) ../DataBase/cSiteEditTools.cpp 

cPthlssp.o: ../Prediction/cPthlssp.h ../Prediction/cPthlssp.cpp makefile
	$(CC) ../Prediction/cPthlssp.cpp 

cProfile.o: ../Prediction/cProfile.h ../Prediction/cProfile.cpp makefile
	$(CC) ../Prediction/cProfile.cpp 

cAntpatn.o: ../Prediction/cAntpatn.h ../Prediction/cAntpatn.cpp makefile
	$(CC) ../Prediction/cAntpatn.cpp 

cClutter.o: ../Prediction/cClutter.h ../Prediction/cClutter.cpp makefile
	$(CC) ../Prediction/cClutter.cpp 

cRaster.o: ../Prediction/cRaster.h ../Prediction/cRaster.cpp makefile
	$(CC) ../Prediction/cRaster.cpp

cRasterFileHandler.o: ../Prediction/cRasterFileHandler.h ../Prediction/cRasterFileHandler.cpp makefile
	$(CC) ../Prediction/cRasterFileHandler.cpp  

cMemmngr.o: ../Prediction/cMemmngr.h ../Prediction/cMemmngr.cpp makefile
	$(CC) ../Prediction/cMemmngr.cpp 

cGeoP.o: ../Prediction/cGeoP.h ../Prediction/cGeoP.cpp makefile
	$(CC) ../Prediction/cGeoP.cpp 

cGDAL.o: ../Prediction/cGDAL.h ../Prediction/cGDAL.cpp makefile
	$(CC) ../Prediction/cGDAL.cpp 

cBIN.o: ../Prediction/cBIN.h ../Prediction/cBIN.cpp makefile
	$(CC) ../Prediction/cBIN.cpp 

cMeasAnalysisCalc.o: ../Prediction/cMeasAnalysisCalc.h ../Prediction/cMeasAnalysisCalc.cpp makefile
	$(CC) ../Prediction/cMeasAnalysisCalc.cpp

cGPpropModel.o: SafeQueue.h ThreadPool.h cGPpropModel.h cGPpropModel.cpp makefile
	$(CC) cGPpropModel.cpp	

cGPFunctions.o:  cGPFunctions.h cGPFunctions.cpp makefile
	$(CC) cGPFunctions.cpp

clean: 
	-rm -f *.o *~ *.out main


