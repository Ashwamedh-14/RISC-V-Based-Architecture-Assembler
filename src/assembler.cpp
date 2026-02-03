#include "assembler.h"
#include <iostream>
#include <cstddef> // For size_t
#include <cstdint>
#include <map>
#include <sstream>

// Defining Error Codes
#define INVALID_LINE 100
#define INVALID_OPCODE 101
#define INVALID_DATALINE 102
#define REG_W_INVALID_REFERENCE 103
#define REG_RX_INVALID_REFERENCE 104
#define REG_RY_INVALID_REFERENCE 105
#define REG_W_OUT_OF_RANGE 106
#define REG_RX_OUT_OF_RANGE 107
#define REG_RY_OUT_OF_RANGE 108
#define JUMP_OUT_OF_RANGE 109
#define INVALID_PARAM_NUM 110
#define INVALID_REG_NUM 111
#define INVALID_DAT_REF 112
#define INVALID_LABEL_REF 113
#define INVALID_LABEL_USE 114
#define INVALID_INPUT_PORT 115
#define INVALID_OUTPUT_PORT 116

using namespace std;

// Error variable
bool ERR = 0;

// regular expression checks
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

static const string INPUT_PORTS[] = {"F1", "F2", "F3", "F4"};

static const string OUTPUT_PORTS[] = {"F8", "F9", "FA", "FB"};


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
static const size_t INPUT_PORT_NUMBERS = sizeof(INPUT_PORTS) / sizeof(INPUT_PORTS[0]);
static const size_t OUTPUT_PORT_NUMBERS = sizeof(OUTPUT_PORTS) / sizeof(OUTPUT_PORTS[0]);

// Helper functions
static bool validHexDAT(const string &s){
    if (s.empty() || s.size() > 2) return false;
    for (char c: s) if ((c < 'A' || c > 'F') && (c < '0' || c > '9')) return false;
    return true;
}

static bool validReg(const string &s){
    if (s.size() < 2) return false;
    else if (s[0] != 'R') return false;
    for (char c: s.substr(1)) if (c < '0' || c > '9') return false;
    return true;
}

static bool validLabelName(const string &s){
    if (s.size() < 1) return false;
    else if ((s[0] < 'A' || s[0] > 'Z') && s[0] != '_') return false;
    for (char c: s.substr(1)) if ((c < '0' || c >'9') && (c < 'A' || c > 'Z') && c != '_') return false;
    return true;
}

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

// Function to check whether a string is present in the given array
// If yes, returns the index, if no returns -1
int isPresent(const string &s, const string *array, size_t array_size){
    for (size_t i = 0; i < array_size; i++){
        if (array[i] == s) return i;
    }
    return -1;
}

string strip(const string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

// This function removes comments, makes the line uppercase, and returns it removing leading and trailing whitespaces or tabs.
string sanitizeLine(const string &s){
    string uncommented = s;
    size_t comment = s.find("//");              // Comments can be specified by starting the comment with '//'
    if (comment != string::npos) uncommented = s.substr(0, comment);
    toUpper(uncommented);
    return strip(uncommented);
}

/*
 * This function returns the following codes
 * 0: All good
 * 1: Empty label
 * 2: Label ends with a semi-colon
 * 3: Label does not end with a colon
 * 4: Label is a valid OPCODE
 * 5: Label is not a valid label name
*/
uint8_t isValidLabel(const string &s){
    string label;
    string stripped;
    
    stripped = strip(s);
    
    // Conditions
    if (stripped.empty()) return 1;                             // Should not be empty
    else if (stripped.find(';') != string::npos) return 2;      // Should not end with a semi-colon              
    else if (stripped[stripped.size() - 1] != ':') return 3;    // Should end with a colon

    // If the above conditions are passed, we remove the colon, and strip the passed label again
    label = strip(stripped.substr(0, stripped.size() - 1));
    
    // return false if it is a valid OPCODE
    if (findOpcode(label)) return 4;
    else if (!validLabelName(label))return 5;

    return 0;

}

bool isLabelRecorded(const string &s, const map<string, size_t> &labels){
    if (labels.find(s) == labels.end()) return false;
    return true;
}

string hexBinConversion(char c) {
    for (uint8_t i = 0; i < 16; i++){
        if (HEX_CHARS[i] == c) return BIN_STR[i];
    }
    return "xxxx";
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
    

    // These are allowed in the function since they just re-arrage the regs to their correct position according to the Opcode
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
        if (instr.registers[i].empty()) {
            instr.registers[i] = "0";
            continue;
        }
        else if (!validReg(instr.registers[i])){
            if (i == 0) return REG_W_INVALID_REFERENCE;
            else if (i == 1) return REG_RX_INVALID_REFERENCE;
            else return REG_RY_INVALID_REFERENCE;
        }
        temp = stoi(instr.registers[i].substr(1, instr.registers[i].size()));
        if (temp >= 16){
            if (i == 0) return REG_W_OUT_OF_RANGE;
            else if (i == 1) return REG_RX_OUT_OF_RANGE;
            else return REG_RY_OUT_OF_RANGE;
        }
        instr.registers[i] = string(1, HEX_CHARS[temp]);
    }
        
    // Checking valid dataline
    if (instr.dataline.empty()) return 0;
    else if (!validLabelName(instr.dataline) && !validHexDAT(instr.dataline)) return INVALID_DATALINE;
    else if (validLabelName(instr.dataline) && !isLabelRecorded(instr.dataline, labels) && !validHexDAT(instr.dataline)) return INVALID_LABEL_REF;
    else if (validLabelName(instr.dataline) && isLabelRecorded(instr.dataline, labels)){
        // Now we know that the dataline is having a label and it is recorded, we convert the address of the label to the dataline in hex
        temp = labels.at(instr.dataline);
        if (!temp) instr.dataline = "00";
        else if (temp > 255 || temp < 0) return JUMP_OUT_OF_RANGE;
        else{
            instr.dataline = "";
            while (temp > 0){
                instr.dataline += HEX_CHARS[temp % 16];
                temp /= 16;
            }
        }
    }
    

    if (instr.dataline.size() == 1) instr.dataline = "0" + instr.dataline;
    return 0;   
}


// Main Parsing Logic
uint8_t parse(size_t line_num, const string &line, const string block_label, const map<string, size_t> &labels, ofstream &out_file) {
    Instruction instr;
    string word;
    string wrong_code;
    int error_num = 0;
    int param_num = 0;

    instr.reg_num = 0;
    instr.dataline = "";
   
    stringstream ss(line);
    // Getting Opcode
    getline(ss, word, ',');
    instr.opcode = findOpcode(strip(word));

    if (!instr.opcode) wrong_code = word;
    
    // Getting rest of the instructions
    while (getline(ss, word, ',')){
        if (param_num >= 4){
            if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
            out_file << "Error (Code 100): Bad line at line number " << line_num << ".\n";
            out_file << "Too many parameters passed.\n";
            ERR = true;
            return INVALID_LINE;
        }
        word = strip(word);
        
        if (word.empty()){
            if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
            out_file << "Error (Code 100): Bad line at line number " << line_num << " and parameter number " << param_num << ".\n";
            out_file << "Passed value:  \"\".\n";
            ERR = true;
            return INVALID_LINE;
        }

        else if (word[0] == 'R' && !validReg(word)) instr.dataline = word;
        
        else if (word[0] == 'R' && instr.reg_num >= 3){
            if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
            out_file << "Error (Code 111): Too many register references at line number " << line_num << ".\n";
            out_file << "Maximum Register References allowed: 3.\n";
            ERR = true;
            return INVALID_REG_NUM;
        }
        else if (word[0] == 'R') instr.registers[instr.reg_num++] = word;
        else instr.dataline = word;
        param_num++;
    }

    error_num = instr_chk(instr, labels);
    
    // checking to see if there is any error, to get a default message for all switch cases with error;
    if (error_num && !block_label.empty()) out_file << "In block: " << block_label << ".\n";

    switch (error_num){
        case 0:
            // No error
            break;
   
        case INVALID_OPCODE:
            out_file << "Error (Code 101): Invalid opcode: " << wrong_code << ", at line " << line_num << ".\n";
            out_file << "Hint: Check for typos or undefined instruction mnemonic.\n";
            ERR = true;
            break;

        case INVALID_DATALINE:
            out_file << "Error (Code 102): Invalid or undefined data/label: " << instr.dataline << ", at line " << line_num << ".\n";
            out_file << "Hint: Ensure the immediate value is valid hex, or the label is defined earlier.\n";
            ERR = true;
            break;

        case REG_W_INVALID_REFERENCE:
            out_file << "Error (Code 103): Invalid destination register (Rw) reference: " << instr.registers[0] << ", at line " << line_num << ".\n";
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            break;

        case REG_RX_INVALID_REFERENCE:
            out_file << "Error (Code 104): Invalid source register (Rx) reference: " << instr.registers[1] << ", at line " << line_num << ".\n";
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            break;

        case REG_RY_INVALID_REFERENCE:
            out_file << "Error (Code 105): Invalid second source register (Ry) reference: " << instr.registers[2] << ", at line " << line_num << ".\n";
            out_file << "Hint: Register names must be in the form R0–R15.\n";
            ERR = true;
            break;

        case REG_W_OUT_OF_RANGE:
            out_file << "Error (Code 106): Destination register (Rw) out of range at line " << line_num << ".\n";
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
            break;

        case REG_RX_OUT_OF_RANGE:
            out_file << "Error (Code 107): Source register (Rx) out of range at line " << line_num << ".\n";
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
            break;

        case REG_RY_OUT_OF_RANGE:
            out_file << "Error (Code 108): Second source register (Ry) out of range at line " << line_num << ".\n";
            out_file << "Hint: Only registers R0–R15 are valid.\n";
            ERR = true;
            break;

        case JUMP_OUT_OF_RANGE:
            out_file << "Error (Code 109): Jump target out of range at line " << line_num << ".\n";
            out_file << "Hint: Label address exceeds 255. Ensure label positions fit in 8-bit number size.\n";
            ERR = true;
            break;

        case INVALID_LABEL_REF:
            out_file << "Error (Code 113): Referenced Label: " << instr.dataline << " at line " << line_num << " not found.\n";
            out_file << "The error could either be due to invalid label name, or no label of same name was found.\n";
            ERR = true;
            break;

        case INVALID_LABEL_USE:
            out_file << "Error (Code 114): Inavlid use of label with opcode " << instr.opcode->opcode << ", at line " << line_num  << ".\n";
            out_file << "The error is because labels are explicitly only to be used with `jmp`, or similar statements.\n";
            ERR = true;
            break;

        default:
            out_file << "Error (Code 100): Bad line at line number " << line_num << ".\n";
            ERR = true;
            return INVALID_LINE;
    };

    if (error_num) return error_num;

    else if (param_num != (instr.opcode->instr_num & 0x03)){       // Checking the 2 LSB bits for expected number of parameters
        if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
        out_file << "Error (Code 110): Invalid number of parameters for OPCODE: " << instr.opcode->opcode << ", at line number " << line_num << ".\n";
        out_file << "Expected number of parameters: " << (instr.opcode->instr_num & 0x03) << ", ";
        out_file << "Received: " << param_num << ".\n";
        ERR = true;
        return INVALID_PARAM_NUM;
    }
    else if (instr.reg_num != ((instr.opcode->instr_num >> 2) & 0x03)) { // Shifting 2 bits and checking the 2 LSB bits for expected number of register references
        if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
        out_file << "Error (Code 111): Invalid number of registers referenced for OPCODE: " << instr.opcode->opcode << ", at line number " << line_num << ".\n";
        out_file << "Expected number of refereced registers: " << ((instr.opcode->instr_num >> 2) & 0x03) << ", ";
        out_file << "Received: " << instr.reg_num << ".\n";
        ERR = true;
        return INVALID_REG_NUM;
    }
    else if ((instr.opcode->instr_num & 0x10) && instr.dataline.empty()){
        if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
        out_file << "Error (Code 112): Expected Data on Dataline for OPCODE: " << instr.opcode->opcode << ", at line number " << line_num << " ";
        out_file << "But non was passed.\n";
        ERR = true;
        return INVALID_DAT_REF;
    }
    else if (instr.opcode->opcode == "IN" && isPresent(instr.dataline, INPUT_PORTS, INPUT_PORT_NUMBERS) == -1){
        if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
        out_file << "Error (Code 115): Invalid input port at line number " << line_num << ".\n";
        out_file << "Passed port: " << instr.dataline << ".\n";
        ERR = true;
        return INVALID_INPUT_PORT;
    }
    else if (instr.opcode->opcode == "OUT" && isPresent(instr.dataline, OUTPUT_PORTS, OUTPUT_PORT_NUMBERS) == -1){
        if (!block_label.empty()) out_file << "In block: " << block_label << ".\n";
        out_file << "Error (Code 116): Invalid output port at line number " << line_num << ".\n";
        out_file << "Passed port: " << instr.dataline << ".\n";
        ERR = true;
        return INVALID_OUTPUT_PORT;
    }

    // If after all these checkes, dataline is still empty, it means the dataline is not used, so we write "00" to it
    else if (instr.dataline.empty()) instr.dataline = "00";

    out_file << instr.opcode->hex << instr.registers[0] << instr.registers[1] << instr.registers[2] << instr.dataline << "\n";
    
    return 0;

}
