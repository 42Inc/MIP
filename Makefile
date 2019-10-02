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
OBJ = $(OBJ_DIR)/coder.o $(OBJ_DIR)/decoder.o $(OBJ_DIR)/bwt.o $(OBJ_DIR)/main.o
OUTERS := *.code *.decode
.PHONY: all bins clean rebuild
all: bins

bins: $(DIRS) $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(BIN_DIR)/mip $(LIB) $(INCLUDES)
	@test -e ${PWD}/bin/mtf || (chmod +x ${PWD}/src/mtf.rb && ln -s ${PWD}/src/mtf.rb ${PWD}/bin/mtf)
	@test -e ${PWD}/bin/bwt || (chmod +x ${PWD}/src/bwt.rb && ln -s ${PWD}/src/bwt.rb ${PWD}/bin/bwt)

$(OBJ_DIR):
	$(if ifeq test -d "$(OBJ_DIR)" 0, @mkdir -p $(OBJ_DIR))

$(BIN_DIR):
	$(if ifeq test -d "$(BIN_DIR)" 0, @mkdir -p $(BIN_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(OFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	$(QUIET)rm -rfv $(DIRS)
	$(QUIET)rm -rfv ${OUTERS}

rebuild:
	make clean ; make
