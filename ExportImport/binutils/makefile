OBJS = fixtxd.o tokenizer.o
PROGNAME = fixtxd

%.o: %.cpp
	g++ -c $<

$(PROGNAME): $(OBJS)
	g++ $(OBJS) -o $(PROGNAME)

