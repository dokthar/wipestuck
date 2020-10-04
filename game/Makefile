# See LICENSE file for copyright and license details.
.POSIX:

include ../config.mk

HDR = engine.h util.h math.h camera.h mesh.h
SRC = engine.c util.c math.c camera.c
SRC += main.c
OBJ = $(SRC:.c=.o)
BIN = game.a

all: $(BIN)

$(OBJ): $(HDR)

$(BIN): $(OBJ)
	$(AR) rc $@ $(OBJ)

clean:
	rm -f $(BIN) $(OBJ)

.PHONY: all clean
