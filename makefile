# Compiler and Flags
CXX := g++
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

# Source and Object Files
SOURCEs := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCEs))

# Final Executable
target := $(BIN_DIR)/Assembler

.PHONY: setup all clean

all: $(target)

setup:
	@echo "Setting up the environment"
	@echo "Updating and Upgrading the Environment"
	@sudo apt update && sudo apt upgrade -y
	@echo "Installing required packages"
	@sudo apt install -y g++ git

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

# Rule to link obj files into final binary
$(target): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(target)


# Rule to run tests
test: $(target) $(OUTPUT_DIR)
	@echo "Running tests..."
	@FAILED=false; \
	for test_case in $(TEST_CASES); do \
		input="$(INPUT_DIR)/input_$$test_case.txt"; \
		expected_hex="$(EXPECTED_DIR)/expected_hex_$$test_case.txt"; \
		expected_bin="$(EXPECTED_DIR)/expected_bin_$$test_case.txt"; \
		output_hex="$(OUTPUT_DIR)/output_hex_$$test_case.txt"; \
		output_bin="$(OUTPUT_DIR)/output_bin_$$test_case.txt"; \
		echo "Running test 🧪 $$test_case:"; \
		./$(target) $$input $$output_hex $$output_bin || { \
			status=$$?; \
			if [ $$status -eq 9 ] && [ ! -f $$expected_bin ]; then \
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

	@echo "All tests completed."

	@if [ "$(FAILED)" = "true" ]; then \
	    exit 1; \
	else \
	    echo "✅ All tests passed successfully!"; \
	fi;



clean:
	rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up. Removed executable and test output directory."