OBJS = convert.o convertp.o tokenizer.o
PROGNAME = convert
INCLUDE = -I /usr/include/pqxx

$(PROGNAME): $(OBJS)
	g++ -g -O0 $(OBJS) -o $(PROGNAME) -lpqxx

%.o: %.cpp
	g++ -c $< $(INCLUDE)

clean:
	rm -f $(PROGNAME) $(OBJS) 
	mv -f *~ tmp\

