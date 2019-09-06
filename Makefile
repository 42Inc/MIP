CC = gcc
QUIET = @
CFLAGS = -g3 -O0 -Wall
OFLAGS = -Wall
LIB = -lm

SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
INC_DIR = ./include
INCLUDES = -I $(INC_DIR)
DIRS = $(BIN_DIR) $(OBJ_DIR)
OBJ = $(OBJ_DIR)/coder.o $(OBJ_DIR)/decoder.o $(OBJ_DIR)/main.o

.PHONY: all bins clean rebuild
all: bins

bins: $(DIRS) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(BIN_DIR)/mip $(LIB) $(INCLUDES)

$(OBJ_DIR):
	$(if ifeq test -d "$(OBJ_DIR)" 0, @mkdir -p $(OBJ_DIR))

$(BIN_DIR):
	$(if ifeq test -d "$(BIN_DIR)" 0, @mkdir -p $(BIN_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(OFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	$(QUIET)rm -rfv $(DIRS)

rebuild:
	make clean ; make
