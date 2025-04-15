CXX := g++
SRC := New\ Code/main.cpp
OUT := Assembler
INPUT_DIR := ./tests/inputs
OUTPUT_DIR := ./tests/outputs
EXPECTED_DIR := ./tests/expected

TEST_CASES := 1 2 3 4 5

all: test

.PHONY: all build test clean

dependencies:
	@echo "Downloading Dependencies..."
	sudo apt-get install -y dos2unix
	@echo "Dependencies installed."

build: dependencies
	@echo "Building the project..."
	$(CXX) -std=c++17 -o $(OUT) $(SRC)
	@echo "Build complete ✅. Executable: $(OUT)"

setup: build
	@echo "Setting all files in Directories to Unix Standard, if saved in Windows Standard"
	dos2unix $(INPUT_DIR)/*.txt
	dos2unix $(EXPECTED_DIR)/*.txt
	@echo "Done"
	@echo "Checking is the output directory exists..."
	@if [ ! -d "$(OUTPUT_DIR)" ]; then \
		echo "Output directory does not exist. Creating..."; \
		mkdir -p $(OUTPUT_DIR); \
		echo "Output directory created."; \
	else \
		echo "Output directory already exists."; \
	fi

test: setup
	@echo "Running tests..."
	@FAILED=false; \
	for test_case in $(TEST_CASES); do \
		input="$(INPUT_DIR)/input_$$test_case.txt"; \
		expected_hex="$(EXPECTED_DIR)/expected_hex_$$test_case.txt"; \
		expected_bin="$(EXPECTED_DIR)/expected_bin_$$test_case.txt"; \
		output_hex="$(OUTPUT_DIR)/output_hex_$$test_case.txt"; \
		output_bin="$(OUTPUT_DIR)/output_bin_$$test_case.txt"; \
		echo "Running test 🧪 $$test_case:"; \
		./$(OUT) $$input $$output_hex $$output_bin || { \
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
	rm -f $(OUT)
	rm -rf $(OUTPUT_DIR)
	@echo "Cleaned up. Removed executable and test output directory."