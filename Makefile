CXX=g++
CXXFLAGS=-std=c++20 -Wall -Wextra -g -O2 -I/usr/include/SDL2 -Iinclude/imgui
LDFLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRCDIR := src
BINDIR := bin
IMGUI_DIR := imgui

SRC = $(wildcard $(SRCDIR)/*.cc $(SRCDIR)/**/*.cc) \
    $(IMGUI_DIR)/imgui.cpp \
    $(IMGUI_DIR)/imgui_draw.cpp \
    $(IMGUI_DIR)/imgui_tables.cpp \
    $(IMGUI_DIR)/imgui_widgets.cpp \
    $(IMGUI_DIR)/imgui_impl_sdl2.cpp \
    $(IMGUI_DIR)/imgui_impl_sdlrenderer2.cpp


all:
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/main $(SRC) $(LDFLAGS)

clean:
	rm -rf $(BINDIR)/main

.PHONY: all clean
