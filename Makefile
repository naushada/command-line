CC = gcc

CFLAGS  = -Wall -g
LIBS    = -lreadline
INC = -Iinc

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
	@echo $^
	$(CC) $(CFLAGS) $^ -o bin/$@ $(LIBS)

%.o: %.c
	@echo $<
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

.PHONY: clean
clean:
	@rm -fr src/*.o
	@rm -fr bin/$(BIN)
