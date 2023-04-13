CC = g++
CPPFLAGS = -std=c++11 -c -g
LD = g++
LDFLAGS = -std=c++11
FINAL_TARGET = pacman

QT_DIR = ../qtbase-small-5.15.2-x64
FMOD_DIR = ../FMOD
INCLUDE_DIR = -I$(QT_DIR)/include -I$(FMOD_DIR)/include  -Iinclude
LIB_DIR = -L$(QT_DIR)/lib -L$(FMOD_DIR)/lib
LIBS = -lfmodex -lQt5Widgets -lQt5Gui -lQt5Core 

OBJECTS_DIR = obj
SRC_DIR = src

default: $(FINAL_TARGET)

$(FINAL_TARGET): $(OBJECTS_DIR)/jeu.o $(OBJECTS_DIR)/main.o $(OBJECTS_DIR)/pacmanwindow.o $(OBJECTS_DIR)/soundgenerator.o
	$(LD) $+ -o $@ $(LDFLAGS) $(LIB_DIR) $(LIBS)

$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS_DIR)/*.o $(FINAL_TARGET)
