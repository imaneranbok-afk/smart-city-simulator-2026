#**************************************************************************************************
# Raylib Makefile for Windows/MSYS2 MinGW
#**************************************************************************************************

.PHONY: all clean

# Project name
PROJECT_NAME ?= smart_city

# Build mode: DEBUG or RELEASE
BUILD_MODE ?= DEBUG

# Raylib path (adjust to your installation)
RAYLIB_PATH ?= /c/raylib/raylib-5.5_win64_mingw-w64

# Compiler
CC = g++

# Include paths
INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/include -I$(RAYLIB_PATH)/src/external

# Compiler flags
CFLAGS = -Wall -std=c++14 -D_DEFAULT_SOURCE -Wno-missing-braces
ifeq ($(BUILD_MODE),DEBUG)
    CFLAGS += -g -O0
else
    CFLAGS += -O2
endif

# Library paths
LDFLAGS = -L$(RAYLIB_PATH)/lib

# Libraries to link
LDLIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Source and object directories
SRC_DIR = src
OBJ_DIR = obj

# Source files and corresponding object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: $(PROJECT_NAME).exe

# Build project
$(PROJECT_NAME).exe: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE_PATHS) $(LDFLAGS) $(LDLIBS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_PATHS)

# Clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(PROJECT_NAME).exe
	@echo "Cleaning done"

