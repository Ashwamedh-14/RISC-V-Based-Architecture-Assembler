CXX := g++
SRC := New\ Code/main.cpp
OUT := Assembler
TEST_INPUT := tests/input.txt
TEST_OUTPUT_HEX := tests/output_hex.txt
TEST_EXPECTED_HEX := tests/expected_hex.txt
TEST_OUTPUT_BIN := tests/output_bin.txt

all: test

.PHONY: all build execute test clean

build:
	$(CXX) -std=c++17 -o $(OUT) $(SRC)
	@echo "Build complete ✅. Executable: $(OUT)"

execute: build
	@echo "Executing $(OUT) with test input..."
	@./$(OUT) $(TEST_INPUT) $(TEST_OUTPUT_HEX) $(TEST_OUTPUT_BIN)
	@echo "Execution complete. Output files generated."

test: execute
	@echo "Running tests..."
	@echo "Input:"
	@cat $(TEST_INPUT)
	@echo "Output of Hexadecimal:"
	@cat $(TEST_OUTPUT_HEX)
	@echo "Expected:"
	@cat $(TEST_EXPECTED_HEX)
	@echo "Comparing output..."
	@if cmp -s $(TEST_OUTPUT_HEX) $(TEST_EXPECTED_HEX); then \
		echo "Test for Hexadecimal passed! ✅"; \
	else \
		echo "Test for Hexadecimal failed! ❌"; \
		git diff --no-index --color $(TEST_OUTPUT_HEX) $(TEST_EXPECTED_HEX) || true; \
		exit 1; \
	fi

clean:
	rm -f $(OUT) $(TEST_OUTPUT_HEX)
	@echo "Cleaned up. Removed executable and test output."