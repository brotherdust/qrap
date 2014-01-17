CC = g++ -g -Wall -pedantic -c -I /usr/include/postgresql 

OBJECTS = cgeneric.o cPosition.o cPosSet.o cGeoP.o cantpatn.o memmngr.o cImportMMR.o cTPoint.o cMeas.o CellList.o main.o

all: Position makefile

Position: $(OBJECTS) makefile
	g++ -g -o Position $(OBJECTS) -L/usr/local/pgsql/lib -lpq

main.o: main.cpp makefile
	$(CC) main.cpp

cgeneric.o:  cgeneric.h cgeneric.cpp makefile
	$(CC) cgeneric.cpp
	
cGeoP.o:  cGeoP.h cGeoP.cpp makefile
	$(CC) cGeoP.cpp

cImportMMR.o:  cImportMMR.h cImportMMR.cpp makefile
	$(CC) cImportMMR.cpp

cMeas.o:  cMeas.h cMeas.cpp makefile
	$(CC) cMeas.cpp

cTPoint.o:  cTPoint.h cTPoint.cpp makefile
	$(CC) cTPoint.cpp
	
CellList.o:  CellList.h CellList.cpp makefile
	$(CC) CellList.cpp

cPosSet.o:  cPosSet.h cPosSet.cpp makefile
	$(CC) cPosSet.cpp

cantpatn.o:  cantpatn.h cantpatn.cpp makefile
	$(CC) cantpatn.cpp

memmngr.o:  memmngr.h memmngr.cpp makefile
	$(CC) memmngr.cpp

cPosition.o:  cPosition.h cPosition.cpp makefile
	$(CC) cPosition.cpp

clean: 
	-rm -f *.o *~ *.out main
