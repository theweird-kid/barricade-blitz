CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -g -O2 -Iinclude -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS= -Llib -limgui_shared -lSDL2 -lSDL2_image -lSDL2_mixer -Wl,-rpath=./lib

SRCDIR := src
BINDIR := bin
IMGUI_DIR := imgui

SRC = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/**/*.cc)

all:
	@mkdir -p $(BINDIR)
	@$(CXX) $(CXXFLAGS) -o $(BINDIR)/main $(SRC) $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/main

.PHONY: all clean
