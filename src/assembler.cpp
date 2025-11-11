#include "assembler.h"
#include <iostream>
#include <cstddef> // For size_t
#include <cstdint>
#include <regex>
#include <map>
#include <sstream>
#include <algorithm>

#define INVALID_OPCODE 101
#define INVALID_DATALINE 102
#define REG_W_INVALID_REFERENCE 103
#define REG_RX_INVALID_REFERENCE 104
#define REG_RY_INVALID_REFERENCE 105
#define REG_W_OUT_OF_RANGE 106
#define REG_RX_OUT_OF_RANGE 107
#define REG_RY_OUT_OF_RANGE 108
#define JUMP_OUT_OF_RANGE 109

using namespace std;

bool ERR = 0;
// regular expression checks whether the string has valid hex_chars
// and checks whether the passed is string either 1 or 2 characters long
static const regex REG_DAT("^[0-9A-F]{1,2}$");
static const regex REG_REG("^R[0-9]{1,2}$");

static const char HEX_CHARS[] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const string BIN_STR[] = {
    "0000", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111"
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

static const size_t OP_TABLE_SIZE = sizeof(OP_TABLE) / sizeof(OP_TABLE[0]); // Should be 29 for now

// Helper functions

const Opcode* findOpcode(const string &name){
    for (size_t i = 0; i < OP_TABLE_SIZE; i++){
        if (OP_TABLE[i].opcode == name) return &OP_TABLE[i];
    }
    return nullptr;
}

void toUpper(string &s){
    for (char &c : s) {
        if (c >= 'a' && c <= 'z'){
            c = c - 32; // Convert to uppercase
        }
    }
}

string strip(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

bool isValidLabel(const string &s){
    size_t colonPos;
    string label;
    string stripped;
    
    stripped = strip(s);
    
    // Cannot contain semicolon;
    if (stripped.find(';') != string::npos) return false;

    // Must end with a colon
    if (stripped.back() != ':') return false;

    // Removing Spaces before a colon
    colonPos =  s.find_last_not_of(" \t", stripped.size() - 2);
    if (colonPos == string::npos) return false;
    
    label = stripped.substr(0, colonPos + 1);


    // First character should be an alphabet
    if (!isalpha(label[0])) return false;

    // Checking if remainig characters are alphanumeric or underscore;
    for (char c: label){
        if (!isalnum(c) && c != '_') return false;
    }

    return true;
}

bool isLabelRecorded(const string &s, const map<string, size_t> &labels){
    if (labels.find(s) == labels.end()) return false;
    return true;
}

/*
Function to check whether the values in Instruction are correct
Meaning of returned values
0: All good
101: Invalid Opcode
102: Invalid Dataline
103: Invalid Register Rw Reference
104: Invalid Register Rx Reference
105: Invalid Regsiter Ry Reference
106: Register Rw Out of Range
107: Register Rx Out of Range
108: Register Ry Out of Range
109: JUMP_OUT_OF_RANGE
*/


uint8_t instr_chk(Instruction &instr, const map<string, size_t> &labels){
    int temp;

    // Checking opcode
    if (!instr.opcode) return INVALID_OPCODE;
    
    if (instr.opcode->opcode == "STORE" || instr.opcode->opcode == "PUSH" || instr.opcode->opcode == "OUT"){ 
        instr.registers[1] = instr.registers[0];
        instr.registers[0] = "";
    } 
    else if( instr.opcode->opcode == "STOREI"){
        instr.registers[2] = instr.registers[1];
        instr.registers[1] = instr.registers[0];
        instr.registers[0] = "";
    }

    // Checking for registers
    for (uint8_t i = 0; i < 3; i++){
        if (instr.registers[i].empty()) instr.registers[i] = "0";
        else if (!regex_match(instr.registers[i], REG_REG)){
            if (i == 0) return REG_W_INVALID_REFERENCE;
            else if (i == 1) return REG_RX_INVALID_REFERENCE;
            else return REG_RY_INVALID_REFERENCE;
        }
        temp = stoi(instr.registers[i].substr(1, instr.registers[i].size() - 1));
        if (temp >= 16){
            if (i == 0) return REG_W_OUT_OF_RANGE;
            else if (i == 1) return REG_RX_OUT_OF_RANGE;
            else return REG_RY_OUT_OF_RANGE;
        }
        instr.registers[i] = string(1, HEX_CHARS[temp]);
    }
        
    // Checking valid dataline
    if (instr.dataline.empty()) return 0;
    else if (isValidLabel(instr.dataline + ":")) {
        // It's syntactically a label; now check if it's defined
        if (!isLabelRecorded(instr.dataline, labels)) return INVALID_DATALINE;
    
        temp = labels.at(instr.dataline);
        if (temp > 255) return JUMP_OUT_OF_RANGE;
        else if (temp == 0){
            instr.dataline = "00";
            return 0;
        }

        // Convert label address to hex string
        instr.dataline = "";
        while (temp > 0) {
            instr.dataline = HEX_CHARS[temp % 16] + instr.dataline;
            temp /= 16;
        }
    }
    else if (!regex_match(instr.dataline, REG_DAT)) return INVALID_DATALINE;

    if (instr.dataline.size() == 1) instr.dataline = "0" + instr.dataline;
    return 0;   
}



void parse(size_t line_num, const string &line, const map<string, size_t> &labels, ofstream &out_file) {
    Instruction instr;
    string word;
    stringstream ss(line);
    int param_num = 0;

    instr.reg_num = 0;
    instr.dataline = "";

    // Getting Opcode
    getline(ss, word, ',');
    instr.opcode = findOpcode(strip(word));
    
    // Getting rest of the instructions
    while (getline(ss, word, ',')){
        if (param_num >= 4){
            out_file << "Error: Invalid line at line number " << line_num << ".\n";
            out_file << "Too many parameters passed.\n";
        }
        word = strip(word);
        
        if (word.empty()){
            out_file << "Error: Invalid line at line number " << line_num << " and parameter number " << param_num << ".\n";
            out_file << "Passed value:  \"\".\n";
            ERR = true;
            return;
        }
        
        if (word[0] == 'R' && instr.reg_num >= 3){
            out_file << "Error: Too many register references at line number " << line_num << ".\n";
            out_file << "Total Register References: 4.\n";
            ERR = true;
            return;
        }
        else if (word[0] == 'R') instr.registers[instr.reg_num++] = word;
        else instr.dataline = word;
        param_num++;
    }

    switch (instr_chk(instr, labels)){
        case 0:
            // No error
            break;

        case INVALID_OPCODE:
            out_file << "Error (Code 101): Invalid opcode at line " << line_num << ".\n" 
            out_file << "Hint: Check for typos or undefined instruction mnemonic.\n";
            ERR = true;
            return;

        case INVALID_DATALINE:
            out_file << "Error (Code 102): Invalid or undefined data/label at line " << line_num << ".\n"
            out_file << "Hint: Ensure the immediate value is valid hex, or the label is defined earlier.\n";
            ERR = true;
            return;

        case REG_W_INVALID_REFERENCE:
            out_file << "Error (Code 103): Invalid destination register reference at line " << line_num << ".\n"
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            return;

        case REG_RX_INVALID_REFERENCE:
            out_file << "Error (Code 104): Invalid source register (Rx) reference at line " << line_num << ".\n"
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            return;

        case REG_RY_INVALID_REFERENCE:
            out_file << "Error (Code 105): Invalid second source register (Ry) reference at line " << line_num << ".\n"
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            return;

        case REG_W_OUT_OF_RANGE:
            out_file << "Error (Code 106): Destination register out of range at line " << line_num << ".\n"
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
        return;

        case REG_RX_OUT_OF_RANGE:
            out_file << "Error (Code 107): Source register (Rx) out of range at line " << line_num << ".\n"
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
            return;

        case REG_RY_OUT_OF_RANGE:
            out_file << "Error (Code 108): Second source register (Ry) out of range at line " << line_num << ".\n"
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
            return;

        case JUMP_OUT_OF_RANGE:
            out_file << "Error (Code 109): Jump target out of range at line " << line_num << ".\n"
            out_file << "Hint: Label address exceeds 255. Ensure label positions fit in 8-bit address space.\n";
            ERR = true;
            return;

        default:
            out_file << "Error: Unknown internal error at line " << line_num << ".\n";
            ERR = true;
            return;
    };



}

string hexBinConversion(char c) {
    for (uint8_t i = 0; i < 16; i++){
        if (HEX_CHARS[i] == c) return BIN_STR[i];
    }
    return "xxxx";
}
