CC = g++
CPPFLAGS = -std=c++11 -c -g
LD = g++
LDFLAGS = -std=c++11
FINAL_TARGET = pacman

QT_DIR = ../qtbase-small-5.15.2-x64
INCLUDE_DIR = -I$(QT_DIR)/include -Iinclude
LIB_DIR = -L$(QT_DIR)/lib
LIBS = -lQt5Widgets -lQt5Gui -lQt5Core

OBJECTS_DIR = objects
SRC_DIR = src

default: $(FINAL_TARGET)

$(FINAL_TARGET): $(OBJECTS_DIR)/jeu.o $(OBJECTS_DIR)/main.o $(OBJECTS_DIR)/pacmanwindow.o
	$(LD) $+ -o $@ $(LDFLAGS) $(LIB_DIR) $(LIBS)

$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(INCLUDE_DIR)/*.h) | $(OBJECTS_DIR)
	$(CC) $(CPPFLAGS) $(INCLUDE_DIR) -c $< -o $@

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	rm -rf $(OBJECTS_DIR) $(FINAL_TARGET)
