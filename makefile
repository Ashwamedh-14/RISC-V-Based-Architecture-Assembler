# Compiler and Flags
CC := gcc
CXX := g++

CFLAGS := -std=c17 -static -static-libgcc -Wall -Wextra -Iinclude
CXXFLAGS := -std=c++17 -static-libstdc++ -static-libgcc -Wall -Wextra -Iinclude

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
	@echo "Running tests..."
	@sh -c '\
	FAILED=false; \
	for test_case in $(TEST_CASES); do \
		input="$(INPUT_DIR)/input_$$test_case.txt"; \
		expected_hex="$(EXPECTED_DIR)/expected_hex_$$test_case.txt"; \
		expected_bin="$(EXPECTED_DIR)/expected_bin_$$test_case.txt"; \
		output_hex="$(OUTPUT_DIR)/output_hex_$$test_case.txt"; \
		output_bin="$(OUTPUT_DIR)/output_bin_$$test_case.txt"; \
		dos2unix $$input $$expected_hex $$expected_bin 2>/dev/null || true; \
		echo "Running test 🧪 $$test_case:"; \
		./$(target) -i $$input -o $$output_hex -b $$output_bin || { \
			status=$$?; \
			if [ $$status -eq 8 ] && [ ! -f $$expected_bin ]; then \
				echo "✅ Test $$test_case passed (no expected binary output)."; \
			else \
				echo "❌ Test $$test_case failed with status code $$status"; \
				FAILED=true; \
			fi; \
		}; \
		echo "Comparing output with expected results..."; \
		if cmp -s $$output_hex $$expected_hex; then \
			echo "✅ Hex output matches expected results for test $$test_case"; \
		else \
			echo "❌ Hex output does not match expected results for test $$test_case"; \
			git diff --no-index --color $$output_hex $$expected_hex; \
			FAILED=true; \
		fi; \
		if [ -f $$expected_bin ]; then \
			if cmp -s $$output_bin $$expected_bin; then \
				echo "✅ Binary output matches expected results for test $$test_case"; \
			else \
				echo "❌ Binary output does not match expected results for test $$test_case"; \
				git diff --no-index --color $$output_bin $$expected_bin; \
				FAILED=true; \
			fi; \
		fi; \
		echo "Test $$test_case completed."; \
	done; \
	echo "All tests completed."; \
	if [ "$$FAILED" = "true" ]; then \
	    exit 1; \
	else \
	    echo "✅ All tests passed successfully!"; \
	fi'



clean:
	rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up. Removed executable and test output directory."

clean_hard:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(OUTPUT_DIR)
	@echo "Cleaned up. Removed all object files, executable, and test output directory."
