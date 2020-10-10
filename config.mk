# game version
VERSION = 0.1.2

# Customize below to fit system

# Install paths
PREFIX := /usr/local
MANPREFIX := $(PREFIX)/share/man

ifeq ($(TARGET),w64)
CROSS_COMPILE = x86_64-w64-mingw32-
INCS := -I../glfw-3.3.2.bin.WIN64/include -I../glew/glew-2.2.0/include
LIBS := -Lglfw-3.3.2.bin.WIN64/lib-mingw-w64 -Lglew/glew-2.2.0/lib/Release/x64
LIBS += -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -lm -lglew32
LIBS += -lglfw3dll
CFLAGS += -DGLFW_DLL
LDFLAGS += --static -Wl,--no-undefined -static-libgcc
endif

ifeq ($(TARGET),x86)
PKG_CONFIG_PATH = /usr/lib32/pkgconfig/
CFLAGS += -m32
LDFLAGS += -m32
LDFLAGS += -L./libs32 -Wl,-rpath=./libs32 -ldl
else ifeq ($(TARGET),)
LDFLAGS += -Wl,-rpath=./libs -ldl
endif

ifneq ($(CROSS_COMPILE),)
CC      = $(CROSS_COMPILE)cc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf
OBJSIZE = $(CROSS_COMPILE)size
STRIP   = $(CROSS_COMPILE)strip
endif

ifeq ($(CC),emcc)
LDFLAGS += -s USE_GLFW=3 -s WASM=0 -s MIN_WEBGL_VERSION=2 -s FULL_ES3=1
PKG = emconfigure pkg-config
endif

PKG_CONFIG_PATH ?= /usr/lib/pkgconfig/
PKG ?= PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config
PKG ?= pkg-config

# Depencies includes and libs
INCS ?= $(shell $(PKG) --cflags glfw3 glew)
LIBS ?= $(shell $(PKG) --libs glfw3 glew)
LIBS += -lpthread -lm

# Flags
CFLAGS += -std=c99 -D_XOPEN_SOURCE=600 -D_POSIX_C_SOURCE=200112L
CFLAGS += -O2 -W
CFLAGS += $(INCS) -DVERSION=\"$(VERSION)\"

LDFLAGS += $(LIBS)
