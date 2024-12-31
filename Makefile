CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -g -O2 -I/usr/include/SDL2
LDFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRCDIR := src
BINDIR := bin

SRC = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/**/*.cc)

all:
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/main $(SRC) $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/main

.PHONY: all clean
