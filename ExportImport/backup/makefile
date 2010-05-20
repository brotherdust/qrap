OBJS = backup.o restore.o
BACKUPPROGNAME = backup
RESTOREPROGNAME = restore
INCLUDE = -I /usr/include/pqxx

$(PROGNAME),$(RESTOREPROGNAME) : $(OBJS) testbackup.o testrestore.o
	g++ -g -O0 $(OBJS) testbackup.o -o $(BACKUPPROGNAME) -lpqxx
	g++ -g -O0 $(OBJS) testrestore.o -o $(RESTOREPROGNAME) -lpqxx

%.o: %.cpp
	g++ -c $< $(INCLUDE)

clean:
	rm -f $(PROGNAME) $(OBJS) 
	mv -f *~ tmp\

