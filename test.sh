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
if [[ $# -eq 1 ]]; then
    echo "${BLU}No arguements passed!"
    echo "Using default values${RST}"

elif [[ $# -gt 5 ]]; then
    echo "❌ ${RED}Too many arguements passed"
    echo "Expected arguements in the following format${RST}"
    echo "./test.sh <ASSEMBLER> <INPUT_DIR> <OUTPUT_DIR> <EXPECTED_DIR>"
    echo "${RED}Exiting...${RST}"
    exit 1

else
    for ((i = 1; i < $#; i++)); do
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

# Test cases
echo -e "${BLU}Starting Test Cases:${RST}\n"
case_num=1
flag=0x00   # Flag to note whether hex and binary are required

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

    echo "${BLU}Case:${RST} $case_num"
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
        flag=((flag | 0x02))

    elif [[ ! -f "$exp_bin" ]]; then
        echo "${BLU}Expected Hex code at:${RST} $exp_hex"
        echo "${BLU}Output Hex code at: ${RST} $out_hex"
        echo "${YLW}NO binary file expected!!!${RST}"
        flag=((flag | 0x01))
    
    else
        echo "${BLU}Expected Hex code at:${RST} $exp_hex"
        echo "${BLU}Output Hex code at: ${RST} $out_hex"
        echo "${BLU}Expected Binary code at:${RST} $exp_bin"
        echo "${BLU}Output Binary code at: ${RST} $out_bin"
    fi
    
    # Executing the program
    $ASSEMBLER -i "$input_file" -o "$out_hex" -b "$out_bin" -f "$out_fmt"
    signal=$?
    echo "${BLU}The Assembler returned exit code:${RST} $signal"
    echo -e "${BLU}Kindly refer to the README file for knowledge on what each code means${RST}\n"

    if  ((flag & 0x01)) && [[ -f "$out_bin" ]]; then
        echo "❌ ${RED} Test case failed!!!"
        echo "No binary file was expected, yet was generated!!!${RST}"
        flag=((flag | 0x90))

    elif ((flag & 0x02)) && [[ -f "$out_hex" && -f "$out_bin"]]; then
        echo "❌ ${RED} Test case failed!!!"
        echo "No binary or hex file were expected, yet were generated!!!${RST}"
        flag=((flag | 0x90))
    fi




done
