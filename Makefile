GTKMMFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
GTKMMLIBS = $(shell pkg-config gtkmm-3.0 --libs)

STKFLAGS = -I/usr/include/stk
STKLIBS = -lstk

FLAGS = $(GTKMMFLAGS) $(STKFLAGS)
LIBS = $(GTKMMLIBS) $(STKLIBS)

synthetisENS: main.o
	g++ obj/main.o -o synthetisENS $(LIBS)

main.o:
	g++ -c src/main.cpp -o obj/main.o $(FLAGS)

.PHONY: clean
clean:
	rm -f obj/*.o synthetisENS
