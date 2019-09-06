CC = gcc
QUIET = @
CFLAGS = -g3 -O0 -Wall
OFLAGS = -c -Wall
LIB = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INCLUDES = -I $(SRC_DIR)
DIRS = $(BIN_DIR) $(OBJ_DIR)
OBJ = $(OBJ_DIR)/coder.o $(OBJ_DIR)/decoder.o

.PHONY: all bins objs clean rebuild
all: bins

bins: $(DIRS) objs
	$(CC) $(OBJ) $(CFLAGS) -o $(BIN_DIR)/coder $(LIB)
	$(CC) $(OBJ) $(CFLAGS) -o $(BIN_DIR)/decoder $(LIB)

$(DIRS):
	mkdir $(DIRS)

objs:
	$(CC) $(OFLAGS) $(SRC_DIR)/coder.c -o $(OBJ_DIR)/coder.o $(INCLUDES)
	$(CC) $(OFLAGS) $(SRC_DIR)/decoder.c -o $(OBJ_DIR)/decoder.o $(INCLUDES)

clean:
	$(QUIET)rm -rfv $(DIRS)

rebuild:
	make clean ; make
