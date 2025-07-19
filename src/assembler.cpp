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

static const char hex_chars[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

static const map<char, string> hex_to_bin_map = {
    {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
    {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
    {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
    {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
};

static const map<string,string> op_code = {
    {"NOP", "00"},
    {"AND", "01"},
    {"OR", "02"},
    {"EXOR", "03"},
    {"ADD", "04"},
    {"ANDI", "05"},
    {"ORI", "06"},
    {"EXORI", "07"},
    {"ADDI", "08"},
    {"MOV", "09"},
    {"MOVI", "0A"},
    {"LOAD", "0B"},
    {"STORE", "0C"},
    {"JMP", "0D"},
    {"JMPZ", "0E"},
    {"JMPNZ", "0F"},
    {"JMPC", "10"},
    {"JMPNC", "11"},
    {"PUSH", "12"},
    {"POP", "13"},
    {"IN", "14"},
    {"OUT", "15"},
    {"LOADI", "16"},
    {"STOREI", "17"},
    {"SUB", "18"},
    {"SHIFTR", "19"},
    {"SHIFTL", "1A"},
    {"JMPPCRZ", "1B"},
    {"JMPPCRNZ", "1C"}
};

// Function to convert lowercase letters to uppercase
void toUpper(string &s){
    for (char &c : s) {
        if (c >= 'a' && c <= 'z'){
            c = c - 32; // Convert to uppercase
        }
    }
}

// Function to remove leading and trailing whitespaces from a string
string strip(const string &s) {
    size_t start = s.find_first_not_of(" ");
    size_t end = s.find_last_not_of(" ");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
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

    // regular expression checks whether the string has valid hex_chars
    // and checks whether the passed is string either 1 or 2 characters long
    regex reg_dat("^[0-9A-F]{1,2}$");
    regex reg_reg("^R[0-9]{1,2}$");
    int temp;
    if (op_code.find(instr.opcode) == op_code.end()) return INVALID_OPCODE;    // Checks valid opcode

    if (instr.opcode == "STORE" || instr.opcode == "PUSH" || instr.opcode == "OUT"){
        instr.registers[1] = instr.registers[0];
        instr.registers[0] = "";
    }
    else if( instr.opcode == "STOREI"){
        instr.registers[2] = instr.registers[1];
        instr.registers[1] = instr.registers[0];
        instr.registers[0] = "";
    }

    for (uint8_t i = 0; i < 3; i++){                      // Checks valid register
        if (instr.registers[i].empty()) {
           instr.registers[i] = "0";
           continue;
        }

        else if (!regex_match(instr.registers[i], reg_reg)){
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
        instr.registers[i] = hex_chars[temp];

    }

    // Checks valid dataline
    if (!instr.dataline.empty() && !regex_match(instr.dataline, reg_dat)) return INVALID_DATALINE;
    else if (instr.dataline.size() == 1) instr.dataline = "0" + instr.dataline;
    return 0;
}

void parse(size_t line_num, const string &line, ofstream &out_file) {
    Instruction instr;
    stringstream ss(line);
    string temp;
    int line_idx = 0;

    instr.reg_num = 0;
    instr.dataline = "";

    // Get the opcode
    getline(ss, temp, ',');
    instr.opcode = strip(temp);
    line_idx++;

    // Get the rest of the instruction
    while (getline(ss, temp, ',')) {
        temp = strip(temp);
        if (temp.empty()) {
            out_file << "Error: Invalid line at line " << line_num << ".\n";
            out_file << "Passed Value: \"\".\n";
            ERR = true;
            return;
        }

        if (temp[0] == 'R') {
            if (instr.reg_num < 3) {
                    instr.registers[instr.reg_num++] = temp;
            } 
            else {
                out_file << "Error: Too many registers at line number " << line_num << "\n";
                ERR = true;
                return;
            }
        } 
        
        else if (temp.size() == 2 || temp.size() == 1) {
            instr.dataline = temp;
        } 
        
        else {
            out_file << "Error: Invalid Dataline at line number " << line_num << ".\n";
            out_file << "Passed Value: " << temp << "\n";
            ERR = true;
            return;
        }
        line_idx++;
    }

    if (line_idx >= 5) {
        out_file << "Error: Too many instructions at line number: " << line_num << ".\n";
        out_file << "Number of instructions found: " << line_idx << "\n";
        ERR = true;
        return;
    }

    line_idx = instr_chk(instr);    // line_idx checks for error codes now. I know it is confusing, but it is what it is.

    switch(line_idx){
        case 0:
        break;

        case INVALID_OPCODE:
        out_file << "Error: Invalid Opcode at line " << line_num << ".\n";
        out_file << "Opcode: " << instr.opcode  << "\n";
        ERR = true;
        return;
        break;

        case INVALID_DATALINE:
        out_file << "Error: Invalid Dataline at line " << line_num << ".\n";
        out_file << "Dataline: " << instr.dataline  << "\n";
        ERR = true;
        return;
        break;

        case REG_W_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Rw at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[0] << "\n";
        ERR = true;
        return;
        break;

        case REG_RX_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Rx at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[1] << "\n";
        ERR = true;
        return;
        break;

        case REG_RY_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Ry at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[2] << "\n";
        ERR = true;
        return;
        break;

        case REG_W_OUT_OF_RANGE:
        out_file << "Error: Register Rw out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[0] << "\n";
        ERR = true;
        return;
        break;

        case REG_RX_OUT_OF_RANGE:
        out_file << "Error: Register Rx out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[1] << "\n";
        ERR = true;
        return;
        break;

        case REG_RY_OUT_OF_RANGE:
        out_file << "Error: Register Ry out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[2] << "\n";
        ERR = true;
        return;
        break;
    };
    
    // Verifying whether the number of registers is valid for the given opcode
    if (instr.reg_num && (op_code.at(instr.opcode) == "00" || (op_code.at(instr.opcode) >= "0D" && op_code.at(instr.opcode) <= "11") || op_code.at(instr.opcode) == "1B" || op_code.at(instr.opcode) == "1C")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 0, Found: " << instr.reg_num << "\n";
        ERR = true;
        return;
    }
    else if (instr.reg_num != 1 && ((op_code.at(instr.opcode) >= "0A" && op_code.at(instr.opcode) <= "0C") || (op_code.at(instr.opcode) >= "12" && op_code.at(instr.opcode) <= "15"))){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 1, Found: " << instr.reg_num << "\n";
        ERR = true;
        return;
    }
    else if (instr.reg_num != 2 && ((op_code.at(instr.opcode) >= "05" && op_code.at(instr.opcode) <= "09") || op_code.at(instr.opcode) == "16" || op_code.at(instr.opcode) == "17")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 2, Found: " << instr.reg_num << "\n";
        ERR = true;
        return;
    }
    else if (instr.reg_num != 3 && ((op_code.at(instr.opcode) >= "01" && op_code.at(instr.opcode) <= "04") || (op_code.at(instr.opcode) >= "18" && op_code.at(instr.opcode) <= "1A"))){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 3, Found: " << instr.reg_num << "\n";
        ERR = true;
        return;
    }

    // Verifying whether the dataline is valid for the given opcode
    if (instr.dataline.empty() && ((op_code.at(instr.opcode) >= "05" && op_code.at(instr.opcode) <= "08") || (op_code.at(instr.opcode) >= "0A" && op_code.at(instr.opcode) <= "11") || op_code.at(instr.opcode) == "14" || op_code.at(instr.opcode) == "15" || op_code.at(instr.opcode) == "1B" || op_code.at(instr.opcode) == "1C")){
        out_file << "Error: Missing Dataline for OPCode " << instr.opcode << " at line " << line_num << '.' << "\n";
        ERR = true;
        return;
    }
    else if (!instr.dataline.empty() && ((op_code.at(instr.opcode) >= "00" && op_code.at(instr.opcode) <= "04") || op_code.at(instr.opcode) == "09" || op_code.at(instr.opcode) == "12" || op_code.at(instr.opcode) == "13" || (op_code.at(instr.opcode) >= "16" && op_code.at(instr.opcode) <= "1A"))){
        out_file << "Error: Dataline not required for OPCode " << instr.opcode << " at line " << line_num << '.' << "\n";
        ERR = true;
        return;
    }
    
    if (instr.dataline.empty()) instr.dataline = "00";

    // Construncting the hex code
    out_file << op_code.at(instr.opcode) << instr.registers[0] << instr.registers[1] << instr.registers[2] << instr.dataline << "\n";
    return;
}

string hexBinConversion(char c) {
    if (hex_to_bin_map.find(c) != hex_to_bin_map.end()) {
        return hex_to_bin_map.at(c);
    } else {
        return "xxxx"; // Default value for invalid hex character
    }
}
