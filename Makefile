SYNTFLAGS = -Iinclude

GTKMMFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
GTKMMLIBS = $(shell pkg-config gtkmm-3.0 --libs)

STKDEF = -D__LINUX_ALSA__ # Forcing ALSA compilation
STKFLAGS = -I/usr/include/stk
STKLIBS = -lstk

DEF = $(STKDEF)
FLAGS = $(SYNTFLAGS) $(GTKMMFLAGS) $(STKFLAGS)
LIBS = $(GTKMMLIBS) $(STKLIBS)

OBJ = main.o component.o signal.o

synthetisENS: $(OBJ)
	g++ obj/main.o obj/component.o obj/signal.o -o synthetisENS $(LIBS)

main.o: src/main.cpp
	mkdir -p obj
	g++ -c src/main.cpp -o obj/main.o $(DEF) $(FLAGS)

component.o: src/component.cpp
	g++ -c src/component.cpp -o obj/component.o $(DEF) $(FLAGS)

signal.o: src/signal.cpp
	g++ -c src/signal.cpp -o obj/signal.o $(DEF) $(FLAGS)

.PHONY: clean
clean:
	rm -rf obj/ synthetisENS
