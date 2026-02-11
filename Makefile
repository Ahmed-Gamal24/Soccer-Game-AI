# Simple Makefile for Soccer Game AI
CXX := g++
CXXFLAGS := -g -Wall -std=c++17
INCLUDES := -I$(CURDIR)/include
LIBS := -lSDL2 -lSDL2_image

SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE := $(BUILD_DIR)/main

.PHONY: all run clean rebuild help

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $@
	@echo "✓ Build complete: $@"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(BUILD_DIR)

rebuild: clean all

help:
	@echo "make         - Build the project"
	@echo "make run     - Build and run"
	@echo "make rebuild - Clean and rebuild"
	@echo "make clean   - Remove build artifacts"
