# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

OBJ = game.a
BIN = drone

all: $(BIN)

web: $(BIN).html

game/$(OBJ):
	$(MAKE) -C game/ all

$(BIN): game/$(OBJ)
	$(CC) $(CFLAGS) -o $@ game/$(OBJ) $(LDFLAGS) $(LDLIBS)

$(BIN).html: game/$(OBJ)
	$(CC_HTML) $(CFLAGS) -o $@ game/$(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	$(MAKE) -C game/ clean
	rm -f $(BIN)

.PHONY: all web clean game/$(OBJ)
# /usr/lib/emscripten/emcc -o drone.html game/*.c -lGLEW -lGL -lm -s USE_GLFW=3 -s LLD_REPORT_UNDEFINED
