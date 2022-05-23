CC = gcc
AR = ar
AR_FLAGS = rsv
OBJ_DIR = ./obj
CFLAGS  = -Wall -g -std=c99
LIBS    = -lreadline
INC = -Iinc -I/usr/local

SRC = $(shell find ./src -type f -name '*.c')
OBJ = $(patsubst %.c, %.o, $(SRC))
BIN = cordoba

# Default target
.PHONY: all

all: fld $(BIN)

fld:
	@mkdir -p bin
	@mkdir -p obj

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ $(LIBS) -o bin/$@ 

%.o: %.c
	@echo $<
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

.PHONY: clean
clean:
	@rm -fr src/*.o
	@rm -fr bin/$(BIN)
