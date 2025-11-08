#include "assembler.h"
#include <iostream>
#include <cstddef> // For size_t
#include <cstdint>
#include <regex>
#include <map>
#include <sstream>
#include <algorithm>

#define INVALID_OPCODE 1
#define INVALID_DATALINE 2
#define REG_W_INVALID_REFERENCE 3
#define REG_RX_INVALID_REFERENCE 4
#define REG_RY_INVALID_REFERENCE 5
#define REG_W_OUT_OF_RANGE 6
#define REG_RX_OUT_OF_RANGE 7
#define REG_RY_OUT_OF_RANGE 8

using namespace std;

bool ERR = 0;

// regular expression checks whether the string has valid hex_chars
// and checks whether the passed is string either 1 or 2 characters long
static const regex reg_dat("^[0-9A-F]{1,2}$");
static const regex reg_reg("^R[0-9]{1,2}$");

static const char hex_chars[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const Opcode op_table[29] = {
    {"NOP", "00", 0},
    {"AND", "01", 4},
    {""}
};


/*
Function to check whether the values in Instruction are correct
Meaning of returned values
0: All good
1: Invalid Opcode
2: Invalid Dataline
3: Invalid Register Rw Reference
4: Invalid Register Rx Reference
5: Invalid Regsiter Ry Reference
6: Register Rw Out of Range
7: Register Rx Out of Range
8: Register Ry Out of Range
*/
uint8_t instr_chk(Instruction &instr){
}

void parse(size_t line_num, const string &line, ofstream &out_file) {
}

string hexBinConversion(char c) {
    if (hex_to_bin_map.find(c) != hex_to_bin_map.end()) {
        return hex_to_bin_map.at(c);
    } else {
        return "xxxx"; // Default value for invalid hex character
    }
}
