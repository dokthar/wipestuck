# game version
VERSION = 0.0

# Customize below to fit system

# Install paths
PREFIX := /usr/local
MANPREFIX := $(PREFIX)/share/man

# Depencies includes and libs
INCS := `pkg-config --cflags glfw3 glew`
LIBS := `pkg-config --libs glfw3 glew` -lm
#LIBS := -lGLEW -lX11 -lGL -lm

# Flags
CFLAGS  = -std=c99 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
CFLAGS += -g -W -lpthread -ldl -lm
CFLAGS += $(INCS) -DVERSION=\"$(VERSION)\"

LDFLAGS += $(LIBS)

CC_HTML = /usr/lib/emscripten/emcc
