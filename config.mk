# game version
VERSION = 0.1.1

# Customize below to fit system

# Install paths
PREFIX := /usr/local
MANPREFIX := $(PREFIX)/share/man

ifeq ($(TARGET),x86)
PKG_CONFIG_PATH = /usr/lib32/pkgconfig/
CFLAGS := -m32
LDFLAGS := -m32
LDFLAGS += -L./libs32 -Wl,-rpath=./libs32
else
LDFLAGS := -Wl,-rpath=./libs
endif

PKG_CONFIG_PATH ?= /usr/lib/pkgconfig/
$(info $(PKG_CONFIG_PATH))
PKG := PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config
# Depencies includes and libs
INCS := $(shell $(PKG) --cflags glfw3 glew)
LIBS := $(shell $(PKG) --libs glfw3 glew) -l:libm.so

# Flags
CFLAGS += -std=c99 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
CFLAGS += -O2 -W -lpthread -ldl
CFLAGS += $(INCS) -DVERSION=\"$(VERSION)\"

LDFLAGS += $(LIBS)

CC_HTML = /usr/lib/emscripten/emcc
