CC := gcc
STD := -std=c99
DEBUG := -g3
OBJ_PATH := ./obj
SRC_PATH := ./src
BIN_PATH = ./bin
DATA_PATH = ./data
INC_PATH = ./include
SOURCES=$(wildcard $(SRC_PATH)/*.c)
OBJECTS=$(patsubst $(SRC_PATH)%, $(OBJ_PATH)%, $(SOURCES:.c=.o))

.PHONY: all cmpl

all: DIRECTORY cmpl

DIRECTORY: $(OBJ_PATH) $(BIN_PATH)

$(OBJ_PATH):
	$(if ifeq test -d "$(OBJ_PATH)" 0, @mkdir -p $(OBJ_PATH))

$(BIN_PATH):
	$(if ifeq test -d "$(BIN_PATH)" 0, @mkdir -p $(BIN_PATH))

$(DATA_PATH):
	$(if ifeq test -d "$(DATA_PATH)" 0, @mkdir -p $(DATA_PATH))

cmpl: $(OBJECTS)
	$(CC) $(STD) $(DEBUG) $< -o $(BIN_PATH)/$@ -lm

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) $(STD) $(DEBUG) -c $^ -o $@ -lm

clean:
	rm -rf $(OBJ_PATH) $(BIN_PATH)

restruct:
	make clean
	make all
