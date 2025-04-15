CXX := g++
SRC := New\ Code/main.cpp
OUT := Assembler
INPUT_DIR := ./tests/inputs
OUTPUT_DIR := ./tests/outputs
EXPECTED_DIR := ./tests/expected

TEST_CASES := 1 2 3 4 5

all: test

.PHONY: all build test clean

build:
	$(CXX) -std=c++17 -o $(OUT) $(SRC)
	@echo "Build complete ✅. Executable: $(OUT)"

setup: build
	@echo "Setting all files in Directories to Unix Standard, if saved in Windows Standard"
	dos2unix $(INPUT_DIR)/*.txt
	dos2unix $(EXPECTED_DIR)/*.txt
	@echo "Done"

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
		./$(OUT) $$input $$output_hex $$output_bin; \
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
		else \
			echo "⚠️ No expected binary output for test $$test_case"; \
		fi; \
		echo "Test $$test_case completed."; \
	done; \
	if [ "$$FAILED" = true ]; then \
		exit 1; \
	else \
		echo "✅ All tests passed successfully!"; \
	fi;

	@echo "All tests completed."

	@if [ "$(FAILED)" = "true" ]; then \
	    exit 1; \
	else \
	    echo "✅ All tests passed successfully!"; \
	fi;



clean:
	rm -f $(OUT)
	rm -f $(OUTPUT_DIR)/*.txt
	@echo "Cleaned up. Removed executable and test output."