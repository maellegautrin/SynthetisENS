GTKMMFLAGS = $(shell pkg-config gtkmm-3.0 --cflags --libs)

synthetisENS: main.o
	g++ obj/main.o -o synthetisENS $(GTKMMFLAGS)

main.o:
	g++ -c src/main.cpp -o obj/main.o $(GTKMMFLAGS)

.PHONY: clean
clean:
	rm -f obj/*.o synthetisENS
