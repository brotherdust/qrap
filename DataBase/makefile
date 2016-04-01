CC = g++ -g -std=c++11 -std=gnu++11 -Wall -W -D_REENTRANT -c -I/usr/include -I/usr/include/postgresql -I/usr/include/gdal -I../Prediction -I/include -I/usr/include/qt4 -I/usr/include/qt4/Qt -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtCore

OBJECTS = cSettings.o cAlert.o cBase.o cDatabase.o Test.o cSiteEditTools.o cRaster.o cGeoP.o cBIN.o cGDAL.o cMemmngr.o
all: qraputil makefile

qraputil: $(OBJECTS) makefile 
	g++ -g -std=c++11 -std=gnu++11 -o qraputil $(OBJECTS) -L/usr/local/pgsql/lib -L/usr/lib/ -L/lib/-lpq -lpqxx -lgdal -lcrypto -lproj  -lssl -lPocoXML  -lPocoFoundation -lQtCore

Test.o: Config.h Test.cpp makefile
	$(CC) Test.cpp
	
cSettings.o: cSettings.h cSettings.cpp makefile
	$(CC) cSettings.cpp 
	
cAlert.o: cAlert.h cAlert.cpp makefile
	$(CC) cAlert.cpp 

cBase.o: cBase.h cBase.cpp makefile
	$(CC) cBase.cpp 
	
cDatabase.o: cDatabase.h cDatabase.cpp makefile
	$(CC) cDatabase.cpp 

cSiteEditTools.o: cSiteEditTools.h cSiteEditTools.cpp makefile
	$(CC) cSiteEditTools.cpp 

cRaster.o: ../Prediction/cRaster.h ../Prediction/cRaster.cpp makefile
	$(CC) ../Prediction/cRaster.cpp 

cMemmngr.o: ../Prediction/cMemmngr.h ../Prediction/cMemmngr.cpp makefile
	$(CC) ../Prediction/cMemmngr.cpp 

cGeoP.o: ../Prediction/cGeoP.h ../Prediction/cGeoP.cpp makefile
	$(CC) ../Prediction/cGeoP.cpp 

cGDAL.o: ../Prediction/cGDAL.h ../Prediction/cGDAL.cpp makefile
	$(CC) ../Prediction/cGDAL.cpp 

cBIN.o: ../Prediction/cBIN.h ../Prediction/cBIN.cpp makefile
	$(CC) ../Prediction/cBIN.cpp 
	
clean: 
	-rm -f *.o *~ *.out qraputil


