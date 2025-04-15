CXX := g++
SRC := New\ Code/main.cpp
OUT := Assembler
INPUT_DIR := ./tests/input
OUTPUT_DIR := ./tests/output
EXPECTED_DIR := ./tests/expected

INPUT_FILES := $(notdir $(wildcard tests/input/*.txt))
TEST_CASES := $(basename $(INPUT_FILES))

all: test

.PHONY: all build execute test clean

build:
	$(CXX) -std=c++17 -o $(OUT) $(SRC)
	@echo "Build complete ✅. Executable: $(OUT)"

test: build
	@echo "Running tests..."
	$(eval FAILED=false)
	@$(foreach test_case,$(TEST_CASES), \
		$(eval input=$(INPUT_DIR)/input_$(test_case).txt) \
		$(eval expected_hex=$(EXPECTED_DIR)/expected_hex_$(test_case).txt) \
		$(eval expected_bin=$(EXPECTED_DIR)/expected_bin_$(test_case).txt) \
		$(eval output_hex=$(OUTPUT_DIR)/output_hex_$(test_case).txt) \
		$(eval output_bin=$(OUTPUT_DIR)/output_bin_$(test_case).txt) \
		@echo "Running test 🧪 $(test_case):"; \
		@-./$(OUT) $(input) $(output_hex) $(output_bin) || echo "❌ Execution failed for test $(test_case)"; \
		@echo "Comparing output with expected results..."; \
		@if cmp -s $(output_hex) $(expected_hex); then \
			echo "✅ Hex output matches expected results for test $(test_case)"; \
		else \
			echo "❌ Hex output does not match expected results for test $(test_case)"; \
			git diff --no-index --color $(output_hex) $(expected_hex); \
			FAILED=true; \
		fi; \
		@if [ -f $(expected_bin) ]; then \
			if cmp -s $(output_bin) $(expected_bin); then \
				echo "✅ Binary output matches expected results for test $(test_case)"; \
			else \
				echo "❌ Binary output does not match expected results for test $(test_case)"; \
				git diff --no-index --color $(output_bin) $(expected_bin); \
				FAILED=true; \
			fi; \
		else \
			echo "⚠️ No expected binary output for test $(test_case)"; \
		fi; \
		@echo "Test $(test_case) completed.\n"; \
	)

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