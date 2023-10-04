GTKMMFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
GTKMMLIBS = $(shell pkg-config gtkmm-3.0 --libs)

STKDEF = -D__LINUX_ALSA__ # Forcing ALSA compilation
STKFLAGS = -I/usr/include/stk
STKLIBS = -lstk

DEF = $(STKDEF)
FLAGS = $(GTKMMFLAGS) $(STKFLAGS)
LIBS = $(GTKMMLIBS) $(STKLIBS)

synthetisENS: main.o
	g++ obj/main.o -o synthetisENS $(LIBS)

main.o:
	mkdir -p obj
	g++ -c src/main.cpp -o obj/main.o $(DEF) $(FLAGS)

.PHONY: clean
clean:
	rm -rf obj/ synthetisENS
