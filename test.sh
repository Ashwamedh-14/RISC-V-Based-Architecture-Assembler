#!/bin/bash

# We will take arguements to the shell according to the folllowing syntax
# test.sh <assmebler> <test_input_dir> <test_output_dir> <test_expected_dir>
#
# All the arguements are optional, and default values will be used if not passed.
# ORDER IS IMPORTANT !!!

# Colour Macro
eval "$(printf 'RED=\"\e[31m\"; GRN=\"\e[32m\"; YLW=\"\e[33m\"; BLU=\"\e[34m\"; RST=\"\e[0m\"')"

# Setting defaults
ASSEMBLER="./bin/Assembler"
INPUT_DIR="./tests/inputs"
EXPECTED_DIR="./tests/expected"
OUTPUT_DIR="./tests/output"

# Validating any command line args
if [[ $# -eq 0 ]]; then
    echo "${BLU}No arguements passed!"
    echo "Using default values${RST}"

elif [[ $# -gt 4 ]]; then
    echo "❌ ${RED}Too many arguements passed"
    echo "Expected arguements in the following format${RST}"
    echo "./test.sh <ASSEMBLER> <INPUT_DIR> <OUTPUT_DIR> <EXPECTED_DIR>"
    echo "${RED}Exiting...${RST}"
    exit 1

else
    for ((i = 1; i <= $#; i++)); do
        arg=${!i}
        if [[ i -eq 1 ]]; then
            echo "${BLU}Passed Assembler:${RST} $arg"
            ASSEMBLER=$arg
        
        elif [[ i -eq 2 ]]; then
            echo "${BLU}Passed Input Test Cases Directory:${RST} $arg"
            INPUT_DIR=$arg

        elif [[ i -eq 3 ]]; then
            echo "${BLU}Passed Output Test Cases Directory:${RST} $arg"
            OUTPUT_DIR=$arg
        
        elif [[ i -eq 4 ]]; then
            echo "${BLU}Passed Expected Test Result Directory:${RST} $arg"
            EXPECTED_DIR=$arg
        fi

    done
fi
    
# Setting sub directory structure
EXPECTED_HEX="$EXPECTED_DIR/hex"
EXPECTED_BIN="$EXPECTED_DIR/bin_f"
EXPECTED_FORMAT="$EXPECTED_DIR/format"

OUTPUT_HEX="$OUTPUT_DIR/hex"
OUTPUT_BIN="$OUTPUT_DIR/bin_f"
OUTPUT_FORMAT="$OUTPUT_DIR/format"


# Validating
if [[ ! (-f "$ASSEMBLER" && -x "$ASSEMBLER") ]]; then
    echo -e "❌ $ASSEMBLER ${RED}not found, or was not executable${RST}"
    exit 2
elif [[ ! -d "$INPUT_DIR" ]]; then
    echo -e "❌ ${RED}Could not find the directory:${RST} $INPUT_DIR"
    exit 2
elif [[ ! -d "$EXPECTED_DIR" ]]; then
    echo "❌ ${RED}Could not find the directory:${RST} $EXPECTED_DIR"
    exit 2
elif [[ ! -d "$EXPECTED_FORMAT" ]]; then
    echo "❌ ${RED}Could not find the directory:${RST} $EXPECTED_FORMAT"
    echo "${RED}Kindly obtain the Expected Formats from the repo...${RST}"
    exit 2
elif [[ ! -d "$EXPECTED_HEX" ]]; then
    echo "❌ ${RED}Could not find the directory:${RST} $EXPECTED_HEX"
    echo "${RED}Kindly obtain the Expected Hexes from the repo...${RST}"
    exit 2
elif [[ ! -d "$EXPECTED_BIN" ]]; then
    echo "❌ ${RED}Could not find the the directory:${RST} $EXPECTED_BIN"
    echo "${RED}Kindly obtain the Expected Binaries from the repo...${RST}"
    exit 2
else
    echo -e "✅ ${GRN}Assembler file and Directories validated${RST}\n"
fi


# Generated output directories
echo "${BLU}Making Output Directories, if does not exist"
echo "Making${RST} $OUTPUT_DIR"
mkdir -p $OUTPUT_DIR

echo "${BLU}Making${RST} $OUTPUT_FORMAT"
mkdir -p "$OUTPUT_FORMAT"

echo "${BLU}Making${RST} $OUTPUT_HEX"
mkdir -p "$OUTPUT_HEX"

echo "${BLU}Making${RST} $OUTPUT_BIN"
mkdir -p "$OUTPUT_BIN"

echo -e "✅ ${GRN}All output directories made${RST}\n\n"

echo "${YLW} AT THIS POINT WE ASSUME YOU HAVE:" 
echo "1. ALL THE INPUT TEST CASES IN THE CORRECT FORMAT"
echo "2. ALL THE DIRECTORIES CORRECTLY STRUCTURED"
echo "3. ASSMEBLER CORRECTLY IN PLACE AND EXECUTABLE"
echo -e "4. ALL THE EXPECTED FORMAT, HEX, and BIN files are in their place${RST}\n\n"

sleep 1

# Test cases
echo -e "${BLU}Starting Test Cases:${RST}\n"
case_num=1

# Flag to note whether hex and binary are required
# if 0th bit is set, then no binary file to generate
# if 1st bit is set, then no binary and hex file to set
# if 4th bit is set, binary or hex file was generated when they were not expected
# if 6th bit is set, this test case has failed
# if 7th bit is set, some test cases have failed
flag=0x00   



for input_file in "$INPUT_DIR"/input_*.txt; do
    name=$(basename $input_file .txt | sed 's/input_//')

    # Expected Result Files
    exp_fmt="$EXPECTED_FORMAT/$name.txt"
    exp_hex="$EXPECTED_HEX/$name.txt"
    exp_bin="$EXPECTED_BIN/$name.txt"

    # Output Files
    out_fmt="$OUTPUT_FORMAT/$name.txt"
    out_hex="$OUTPUT_HEX/$name.txt"
    out_bin="$OUTPUT_BIN/$name.txt"

    echo "${BLU}Test 🧪🧪 Case:${RST} $case_num"
    echo "${BLU}Input_file:${RST} $name"
    
    if [[ ! -f "$exp_fmt" ]]; then
        echo "${RED}ERROR: Could not find the expected format file for given input file!!!"
        echo "Aborting...${RST}"
        exit 3
    fi
    
    echo "${BLU}Expected file format at:${RST} $exp_fmt"
    echo "${BLU}Output file format at:${RST} $out_fmt"

    if [[ ! -f "$exp_hex" ]]; then
        echo "${YLW}NO hex file expected!!!"
        echo "NO binary file expected!!!${RST}"
        ((flag |= 0x02))

    elif [[ ! -f "$exp_bin" ]]; then
        echo "${BLU}Expected Hex code at:${RST} $exp_hex"
        echo "${BLU}Output Hex code at: ${RST} $out_hex"
        echo "${YLW}NO binary file expected!!!${RST}"
        ((flag |= 0x01))
    
    else
        echo "${BLU}Expected Hex code at:${RST} $exp_hex"
        echo "${BLU}Output Hex code at: ${RST} $out_hex"
        echo "${BLU}Expected Binary code at:${RST} $exp_bin"
        echo "${BLU}Output Binary code at: ${RST} $out_bin"
    fi
    
    # Executing the program
    "$ASSEMBLER" -i "$input_file" -o "$out_hex" -b "$out_bin" -f "$out_fmt" > /dev/null
    signal=$?
    echo "${BLU}The Assembler returned exit code:${RST} $signal"
    echo -e "${BLU}Kindly refer to the README file for knowledge on what each code means${RST}\n"
    
    if [[ $signal -ne 8 && $signal -ne 0 ]]; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "Something internally wrong happened with the Assembler!!!"
        echo "Check the returned error code printed above"
        echo "Aborting...${RST}"
        exit 4

    elif  ((flag & 0x01)) && [[ -f "$out_bin" ]]; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "No binary file was expected, yet was generated!!!${RST}"
        ((flag |= 0xd0))

    elif ((flag & 0x02)) && [[ -f "$out_hex" && -f "$out_bin" ]]; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "No binary or hex file were expected, yet were generated!!!${RST}"
        ((flag |= 0xd0))
    
    elif ! diff -q "$exp_fmt" "$out_fmt" > /dev/null; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "Format files don't match!!!${RST}"
        diff "$exp_fmt" "$out_fmt"
        ((flag |= 0xc0))

    elif ! ((flag & 0x03)) && ! diff -q "$exp_hex" "$out_hex" > /dev/null; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "Hex files do not match!!!${RST}"
        diff "$exp_hex" "$out_hex"
        ((flag |= 0xc0))

    elif ! ((flag & 0x03)) && ! diff -q "$exp_bin" "$out_bin" > /dev/null; then
        echo "❌ ${RED} Test 🧪🧪 case failed!!!"
        echo "Binary files do not match!!!${RST}"
        diff "$exp_bin" "$out_bin"
        ((flag |= 0xc0))
    else
        echo "✅ ${GRN} Test 🧪🧪 case passed successfully!!!${RST}"
    fi
    
    echo -e "\n\n"
    ((case_num++))
    ((flag &= 0x80))
    
    sleep 1

done

if ((flag & 0x80)); then
    echo "❌❌❌❌ ${RED}Some cases Failed${RST} 🫠🫠🫠"
    exit 5
else
    echo "✅✅✅✅ ${GRN}All cases passed${RST} 🥳🥳🥳"
fi
exit 0
