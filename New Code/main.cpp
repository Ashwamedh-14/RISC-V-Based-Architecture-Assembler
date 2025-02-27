/*
Author: Ashwamedh-14
Date Started: 17 January 2025
Software: Assembly to Hex Converter, i.e, Assembler
Description: 
This program converts assembly code to hex code.
The program reads the assembly code from a file named "asmcode.txt" and writes the hex code to a file named "hexcode.txt".
The assembly code should be written in the following format:
1. Each line should contain only one instruction.
2. The instruction should be written in the following format:
    <OPCODE>, <REGISTER1>, <REGISTER2>, <REGISTER3>, <DATALINE>;
    where:
    a. OPCODE: The operation code of the instruction. It should be written in uppercase.
    b. REGISTER1, REGISTER2, REGISTER3: The registers used in the instruction. The registers should be written in the format "R<register number>". The register number should be written in decimal.
    c. DATALINE: The data to be used in the instruction. It should be written in hexadecimal.
3. The instruction should be followed by a semicolon.

*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <string>

#define INVALID_OPCODE 1
#define INVALID_DATALINE 2
#define REG_W_INVALID_REFERENCE 3
#define REG_RX_INVALID_REFERENCE 4
#define REG_RY_INVALID_REFERENCE 5
#define REG_W_OUT_OF_RANGE 6
#define REG_RX_OUT_OF_RANGE 7
#define REG_RY_OUT_OF_RANGE 8

using namespace std;

static const vector<char> hex_chars = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
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

struct Instruction{
    string opcode;
    vector<string> registers = {"","",""};
    int reg_num;
    string dataline;
};

// Convert alphabets in a string to uppercase
void toUpper(string &s){
    for(int i = 0; i < s.size(); i++){
        if (s[i] >= 97 && s[i] <= 122) s[i] = s[i] - 32;
    }
}

// function to remove heading and trailing whitespaces in a line
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
int instr_chk(Instruction &instr){

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

    for (unsigned short int i = 0; i < 3; i++){                      // Checks valid register
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

void parse(int line_num, const string &line, ofstream &out_file) {
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
            out_file << "Passed Value: \"\".";
            return;
        }

        if (temp[0] == 'R') {
            if (temp.size() == 2 || temp.size() == 3) {
                if (instr.reg_num < 3) {
                    instr.registers[instr.reg_num++] = temp;
                } 
                else {
                    out_file << "Error: Too many registers at line number " << line_num << "";
                    return;
                }
            } 
            else {
                out_file << "Error: Invalid Register at line " << line_num << ".\n";
                out_file << "Register Passed: " << temp;
                return;
            }
        } 
        
        else if (temp.size() == 2 || temp.size() == 1) {
            instr.dataline = temp;
        } 
        
        else {
            out_file << "Error: Invalid Dataline at line number " << line_num << ".\n";
            out_file << "Passed Value: " << temp;
            return;
        }
        line_idx++;
    }

    if (line_idx >= 5) {
        out_file << "Error: Too many instructions at line number: " << line_num << ".\n";
        out_file << "Number of instructions found: " << line_idx;
        return;
    }

    line_idx = instr_chk(instr);

    switch(line_idx){
        case 0:
        break;

        case INVALID_OPCODE:
        out_file << "Error: Invalid Opcode at line " << line_num << ".\n";
        out_file << "Opcode: " << instr.opcode;
        return;
        break;

        case INVALID_DATALINE:
        out_file << "Error: Invalid Dataline at line " << line_num << ".\n";
        out_file << "Dataline: " << instr.dataline;
        return;
        break;

        case REG_W_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Rw at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[0];
        return;
        break;

        case REG_RX_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Rx at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[1];
        return;
        break;

        case REG_RY_INVALID_REFERENCE:
        out_file << "Error: Invalid Register Reference for Ry at line " << line_num << ".\n";
        out_file << "Register Referenced: " << instr.registers[line_idx - 3];
        return;
        break;

        case REG_W_OUT_OF_RANGE:
        out_file << "Error: Register Rw out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[0];
        return;
        break;

        case REG_RX_OUT_OF_RANGE:
        out_file << "Error: Register Rx out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[1];
        return;
        break;

        case REG_RY_OUT_OF_RANGE:
        out_file << "Error: Register Ry out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[line_idx - 6];
        return;
        break;
    };
    
    // Verifying whether the number of registers is valid for the given opcode
    if (instr.reg_num && (op_code.at(instr.opcode) == "00" || op_code.at(instr.opcode) >= "0D" && op_code.at(instr.opcode) <= "11" || op_code.at(instr.opcode) == "1B" || op_code.at(instr.opcode) == "1C")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 0, Found: " << instr.reg_num;
        return;
    }
    else if (instr.reg_num != 1 && (op_code.at(instr.opcode) >= "0A" && op_code.at(instr.opcode) <= "0C" || op_code.at(instr.opcode) >= "12" && op_code.at(instr.opcode) <= "15")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 1, Found: " << instr.reg_num;
        return;
    }
    else if (instr.reg_num != 2 && (op_code.at(instr.opcode) >= "05" && op_code.at(instr.opcode) <= "09" || op_code.at(instr.opcode) == "16" || op_code.at(instr.opcode) == "17")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 2, Found: " << instr.reg_num;
        return;
    }
    else if (instr.reg_num != 3 && (op_code.at(instr.opcode) >= "01" && op_code.at(instr.opcode) <= "04" || op_code.at(instr.opcode) >= "18" && op_code.at(instr.opcode) <= "1A")){
        out_file << "Error: Invalid number of registers for OPCode " << instr.opcode << " at line " << line_num << ".\n";
        out_file << "Number of Registers Expected: 3, Found: " << instr.reg_num;
        return;
    }

    // Verifying whether the dataline is valid for the given opcode
    if (instr.dataline.empty() && (op_code.at(instr.opcode) >= "05" && op_code.at(instr.opcode) <= "08" || op_code.at(instr.opcode) >= "0A" && op_code.at(instr.opcode) <= "11" || op_code.at(instr.opcode) == "14" || op_code.at(instr.opcode) == "15" || op_code.at(instr.opcode) == "1B" || op_code.at(instr.opcode) == "1C")){
        out_file << "Error: Missing Dataline for OPCode " << instr.opcode << " at line " << line_num << '.';
        return;
    }
    else if (!instr.dataline.empty() && (op_code.at(instr.opcode) >= "00" && op_code.at(instr.opcode) <= "04" || op_code.at(instr.opcode) == "09" || op_code.at(instr.opcode) == "12" || op_code.at(instr.opcode) == "13" || op_code.at(instr.opcode) >= "16" && op_code.at(instr.opcode) <= "1A")){
        out_file << "Error: Dataline not required for OPCode " << instr.opcode << " at line " << line_num << '.';
        return;
    }
    
    if (instr.dataline.empty()) instr.dataline = "00";

    // Construncting the hex code
    out_file << op_code.at(instr.opcode) << instr.registers[0] << instr.registers[1] << instr.registers[2] << instr.dataline;
    return;
}

int main(int argc, char **argv){
    string input = "asmcode.txt";
    string output = "hexcode.txt";

    ifstream assembly_code(input);
    
    int line_num = 0;
    
    if (!assembly_code.is_open()){
        cout << "Error: File " << input << " not found" << endl;
        return 1;
    }
    
    ofstream hexfile(output);
    string line;

    hexfile << "v2.0 raw\n";

    while(getline(assembly_code,line)){
        // Convert assembly code to hex
        // and write to hexfile
        line = strip(line);
        toUpper(line);

        if (!line.size()){
            line_num++;
            hexfile << "NOP\n";
            continue;
        }

        else if (count(line.begin(), line.end(), ';') == 0){
            hexfile << "Error: Missing semicolon at line " << ++line_num << "\n";
            continue;
        }

        line = line.substr(0, line.find_first_of(';'));

        if (!line.size()) {
            line_num++;
            hexfile << "NOP\n";
            continue;
        }

        else if (line.size() < 3){
            hexfile << "Error: Invalid line at line " << ++line_num << "\n";
            continue;
        }
        

        parse(++line_num, line, hexfile);
        hexfile << '\n';
    }
    hexfile << endl;

    assembly_code.close();
    hexfile.close();

    return 0;
}