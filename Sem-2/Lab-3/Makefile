CC := gcc
STD := -std=c99 -lm -Wall
DEBUG := -g3
OBJ_PATH := ./obj
SRC_PATH := ./src
BIN_PATH = ./bin
DATA_PATH = ./data
INC_PATH = ./include
SOURCES=tsp_v2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=hello

.PHONY: all v2

all: DIRECTORY v2

DIRECTORY: $(OBJ_PATH) $(BIN_PATH)

$(OBJ_PATH):
	$(if ifeq test -d "$(OBJ_PATH)" 0, @mkdir -p $(OBJ_PATH))

$(BIN_PATH):
	$(if ifeq test -d "$(BIN_PATH)" 0, @mkdir -p $(BIN_PATH))

$(DATA_PATH):
	$(if ifeq test -d "$(DATA_PATH)" 0, @mkdir -p $(DATA_PATH))

v2: $(OBJECTS)
	$(CC) $(STD) $(DEBUG) $(OBJ_PATH)/$< -o $(BIN_PATH)/$@

tsp_v2.o:
	$(CC) $(STD) $(DEBUG) -c $(SRC_PATH)/$*.c -o $(OBJ_PATH)/$@

clean:
	rm -rf $(OBJ_PATH) $(BIN_PATH)

restruct:
	make clean
	make all
