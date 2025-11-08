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
static const regex REG_DAT("^[0-9A-F]{1,2}$");
static const regex REG_REG("^R[0-9]{1,2}$");

static const char HEX_CHARS[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const Opcode OP_TABLE[] = {
    {0x00, "NOP", "00"},
    {0x0f, "AND", "01"},
    {0x0f, "OR", "02"},
    {0x0f, "EXOR", "03"},
    {0x0f, "ADD", "04"},
    {0x1b, "ANDI", "05"},
    {0x1b, "ORI", "06"},
    {0x1b, "EXORI", "07"},
    {0x1b, "ADDI", "08"},
    {0x0a, "MOV", "09"},
    {0x16, "MOVI", "0A"},
    {0x16, "LOAD", "0B"},
    {0x16, "STORE", "0C"},
    {0x11, "JMP", "0D"},
    {0x11, "JMPZ", "0E"},
    {0x11, "JMPNZ", "0F"},
    {0x11, "JMPC", "10"},
    {0x11, "JMPNC", "11"},
    {0x05, "PUSH", "12"},
    {0x05, "POP", "13"},
    {0x16, "IN", "14"},
    {0x16, "OUT", "15"},
    {0x0a, "LOADI", "16"},
    {0x0a, "STOREI", "17"},
    {0x0f, "SUB", "18"},
    {0x0f, "SHIFTR", "19"},
    {0x0f, "SHIFTL", "1A"},
    {0x11, "JMPPCRZ", "1B"},
    {0x11, "JMPPCRNZ", "1C"}
};

static const size_t OP_TABLE_SIZE = sizeof(op_table) / sizeof(op_table[0]); // Should be 29 for now

// Helper functions
const Opcode* findOpcode(const string &name){
    for (size_t i = 0; i < OP_TABLE_SIZE; i++){
        if (OP_TABLE[i].opcode == name) return &OP_TABLE[i];
    }
    return nullptr;
}


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
