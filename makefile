# Versioning
VERSION ?= dev

# Compiler and Flags
CC := gcc
CXX := g++

CFLAGS := -std=c17 -Wall -Wextra -flto
CXXFLAGS := -std=c++20 -Wall -Wextra -flto
CPPFLAGS := -DASSEMBLER_VERSION=\"$(VERSION)\" -Iinclude


# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Directories for Tests
INPUT_DIR := ./tests/inputs
EXPECTED_DIR := ./tests/expected
OUTPUT_DIR := ./tests/output
test_file := ./test.sh

# Source and Object Files
SOURCE_CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
SOURCE_C_FILES := $(wildcard $(SRC_DIR)/*.c)

OBJECTS_CPP := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCE_CPP_FILES))
OBJECTS_C := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCE_C_FILES))
OBJECTS := $(OBJECTS_CPP) $(OBJECTS_C)

# Final Executable
target := $(BIN_DIR)/Assembler

.PHONY: all clean test clean_hard preprocess windows macos

all: $(target)

# Create obj and bin folders if they don't exist

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(OUTPUT_DIR):
	@mkdir -p $(OUTPUT_DIR)

# Rule to compile each .cpp file into .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Rule to compile each .c file into .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Rule to link obj files into final binary : linux
$(target): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(target)
	@echo "Build complete. Executable: $(target)"
	@echo "To run the build, use the command: ./$(target)"

# Rule to link obj files into final binary : windows
windows: $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -static -static-libgcc -static-libstdc++ $(OBJECTS) -o $(target)_x64
	@echo "Windows build complete. Executable: $(target)_x64"
	@echo "To run the Windows build, use the command: ./$(target)_x64"

# Rule to link obj files into final binary : macos
macos: $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(target)_macos
	@echo "macOS build complete. Executable: $(target)_macos"

# Rule to run tests
test: $(target) $(INPUT_DIR) $(OUTPUT_DIR) $(EXPECTED_DIR) $(test_file)
	$(test_file) $(target) $(INPUT_DIR) $(OUTPUT_DIR) $(EXPECTED_DIR)

clean:
	rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up. Removed executable and test output directory."

clean_hard:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OUTPUT_DIR)
	@echo "Cleaned up. Removed all object files, executable, and test output directory."
