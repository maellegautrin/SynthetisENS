SYNTFLAGS = -Iinclude

GTKMMFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
GTKMMLIBS = $(shell pkg-config gtkmm-3.0 --libs)

STKDEF = -D__LINUX_ALSA__ # Forcing ALSA compilation
STKFLAGS = -I/usr/include/stk
STKLIBS = -lstk
SNDLIB= -lsndfile

DEF = $(STKDEF)
FLAGS = $(GTKMMFLAGS) $(STKFLAGS) $(SYNTFLAGS)
LIBS = $(GTKMMLIBS) $(STKLIBS) $(SNDLIB)

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Liste des fichiers source
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# Nom de l'exécutable
TARGET = synthetisENS

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	g++ -c $< -o $@ $(DEF) $(FLAGS)
clean:
	rm -rf $(OBJ_DIR) $(TARGET) output.wav
