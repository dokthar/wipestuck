# See LICENSE file for copyright and license details.
.POSIX:

include ../config.mk

HDR = engine.h util.h math.h mesh.h
SRC = main.c engine.c util.c math.c
OBJ = $(SRC:.c=.o)
BIN = game.a

all: $(BIN)

$(OBJ): $(HDR)

$(BIN): $(OBJ)
	$(AR) rc $@ $(OBJ)

.PHONY: all
