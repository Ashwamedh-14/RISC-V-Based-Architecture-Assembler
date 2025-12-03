# Compiler and Flags
CC := gcc
CXX := g++

CFLAGS := -std=c17 -Wall -Wextra -Iinclude
CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Directories for Tests
INPUT_DIR := ./tests/inputs
EXPECTED_DIR := ./tests/expected
OUTPUT_DIR := ./tests/outputs

# Defining Testcases
TEST_CASES := 1 2 3 4 5

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
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile each .c file into .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to link obj files into final binary : linux
$(target): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(target)
	@echo "Build complete. Executable: $(target)"
	@echo "To run the build, use the command: ./$(target)"
# Rule to link obj files into final binary : windows
windows: $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(target)_x64
	@echo "Windows build complete. Executable: $(target)_x64"
	@echo "To run the Windows build, use the command: ./$(target)_x64"


# Pre-processing rule to convert line endings from CRLF to LF
# This is useful for Windows users to ensure consistent line endings
preprocess:
	@echo "Preprocessing test files (dos2unix)..."
	@find $(INPUT_DIR) $(EXPECTED_DIR) -type f -name '*.txt' -exec dos2unix {} + 2>/dev/null || true
	@echo "✅ Preprocessing complete."

# Rule to run tests
test: $(target) $(OUTPUT_DIR) preprocess
	@echo "==============================="
	@echo "        Running Tests"
	@echo "==============================="

	@FAILED=false; \
	for input_file in $(INPUT_DIR)/input_*.txt; do \
	    case=$$(basename "$$input_file" .txt | sed 's/input_//'); \
	    echo ""; \
	    echo "🧪 Running test case: $$case"; \
	    \
	    input="$(INPUT_DIR)/input_$$case.txt"; \
	    out_hex="$(OUTPUT_DIR)/hex/$$case.txt"; \
	    out_bin="$(OUTPUT_DIR)/bin/$$case.txt"; \
	    exp_hex="$(EXPECTED_DIR)/hex/$$case.txt"; \
	    exp_bin="$(EXPECTED_DIR)/bin_f/$$case.txt"; \
	    \
	    : "Normalize Windows line endings"; \
	    dos2unix $$input $$exp_hex $$exp_bin 2>/dev/null || true; \
	    \
	    : "Run assembler"; \
	    if ! ./$(target) -i $$input -o $$out_hex -b $$out_bin; then \
	        status=$$?; \
	        if [ $$status -eq 8 ] && [ ! -f $$exp_bin ]; then \
	            echo "   ✔ Status OK: Binary output intentionally skipped (code 8)"; \
	        else \
	            echo "   ❌ Assembler returned error code $$status"; \
	            FAILED=true; \
	            continue; \
	        fi; \
	    fi; \
	    \
	    echo "   🔍 Comparing hex output..."; \
	    if cmp -s $$out_hex $$exp_hex; then \
	        echo "   ✔ Hex OK"; \
	    else \
	        echo "   ❌ Hex mismatch!"; \
	        git diff --no-index --color $$out_hex $$exp_hex; \
	        FAILED=true; \
	    fi; \
	    \
	    echo "   🔍 Comparing binary output..."; \
	    if [ -f $$exp_bin ]; then \
	        if cmp -s $$out_bin $$exp_bin; then \
	            echo "   ✔ Bin OK"; \
	        else \
	            echo "   ❌ Bin mismatch!"; \
	            git diff --no-index --color $$out_bin $$exp_bin; \
	            FAILED=true; \
	        fi; \
	    else \
	        echo "   ↪ No expected binary output — skipping."; \
	    fi; \
	    \
	    echo "   ✓ Completed test $$case"; \
	done; \
	echo ""; \
	echo "==============================="; \
	echo "        Test Summary"; \
	echo "==============================="; \
	if [ "$$FAILED" = "true" ]; then \
	    echo "❌ Some tests FAILED"; \
	    exit 1; \
	else \
	    echo "✅ All tests passed successfully!"; \
	fi



clean:
	rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up. Removed executable and test output directory."

clean_hard:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OUTPUT_DIR)
	@echo "Cleaned up. Removed all object files, executable, and test output directory."
