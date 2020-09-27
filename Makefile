# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

OBJ = game.a
BIN = drone

all: $(BIN)

game/$(OBJ):
	$(MAKE) -C game/ all

$(BIN): game/$(OBJ)
	$(CC) $(CFLAGS) -o $@ game/$(OBJ) $(LDFLAGS) $(LDLIBS)

.PHONY: all game/$(OBJ)
