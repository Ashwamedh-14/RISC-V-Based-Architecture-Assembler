#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <string>
#include <getopt.h>

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
3: Invalid Register 1 Reference
4: Invalid Register 2 Reference
5: Invalid Regsiter 3 Reference
6: Register 1 Out of Range
7: Register 2 Out of Range
8: Register 3 Out of Range
*/
int instr_chk(Instruction &instr){

    // regular expression checks whether the string has valid hex_chars
    // and checks whether the passed is string either 1 or 2 characters long
    regex reg_dat("^[0-9A-F]{1,2}$");
    regex reg_reg("^R[0-9]{1,2}$");
    int temp;
    if (op_code.find(instr.opcode) == op_code.end()) return 1;    // Checks valid opcode

    for (int i = 0; i < 3; i++){                      // Checks valid register
        if (instr.registers[i].empty()) {
           instr.registers[i] = "0";
           continue;
        }
        else if (!regex_match(instr.registers[i], reg_reg)) return i + 3;
        temp = stoi(instr.registers[i].substr(1, instr.registers[i].size() - 1));
        if (temp >= 16) return i + 6;
        instr.registers[i] = hex_chars[temp];
    }

    // Checks valid dataline
    if (!instr.dataline.empty() && !regex_match(instr.dataline, reg_dat)) return 2;
    else if (instr.dataline.empty()) instr.dataline = "00";
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
                } else {
                    out_file << "Error: Too many registers at line number " << line_num << "";
                    return;
                }
            } else {
                out_file << "Error: Invalid Register at line " << line_num << ".\n";
                out_file << "Register Passed: " << temp;
                return;
            }
        } else if (temp.size() == 2) {
            instr.dataline = temp;
        } else {
            out_file << "Error: Invalid Instruction at line number " << line_num << ".\n";
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
        cout << "Instruction is correct\n";
        break;

        case 1:
        out_file << "Error: Invalid Opcode at line " << line_num << ".\n";
        out_file << "Opcode: " << instr.opcode;
        return;
        break;

        case 2:
        out_file << "Error: Invalid Dataline at line " << line_num << ".\n";
        out_file << "Dataline: " << instr.dataline;
        return;
        break;

        case 3:
        case 4:
        case 5:
        out_file << "Error: Invalid Register Reference at line " << line_num << ".\n";
        out_file << "Register Referenced for R" << line_idx - 2 << ": " << instr.registers[line_idx - 3];
        return;
        break;

        case 6:
        case 7:
        case 8:
        out_file << "Error: Register R" << line_idx - 6 << " out of Range at line " << line_num << ".\n";
        out_file << "Register Value: " << instr.registers[line_idx - 6];
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
    else cout << "Number of registers valid for the given opcode\n";

    // Verifying whether the dataline is valid for the given opcode
    if (instr.dataline == "00" && (op_code.at(instr.opcode) >= "05" && op_code.at(instr.opcode) <= "08" || op_code.at(instr.opcode) >= "0A" && op_code.at(instr.opcode) <= "11" || op_code.at(instr.opcode) == "14" || op_code.at(instr.opcode) == "15" || op_code.at(instr.opcode) == "1B" || op_code.at(instr.opcode) == "1C")){
        out_file << "Error: Missing Dataline for OPCode " << instr.opcode << " at line " << line_num << '.';
        return;
    }
    else if (instr.dataline != "00" && (op_code.at(instr.opcode) >= "00" && op_code.at(instr.opcode) <= "04" || op_code.at(instr.opcode) == "09" || op_code.at(instr.opcode) == "12" || op_code.at(instr.opcode) == "13" || op_code.at(instr.opcode) >= "16" && op_code.at(instr.opcode) <= "1A")){
        out_file << "Error: Dataline not required for OPCode " << instr.opcode << " at line " << line_num << '.';
        return;
    }
    else cout << "Dataline valid for the given opcode\n";

    // Debugging Instruction
    cout << "Opcode: " << instr.opcode << ",\n";
    cout << "Register Numbers: " << instr.reg_num << ",\n";
    cout << "Reg1: " << instr.registers[0] << ",\n";
    cout << "Reg2: " << instr.registers[1] << ",\n";
    cout << "Reg3: " << instr.registers[2] << ",\n";
    cout << "Dataline: " << instr.dataline << "\n\n";

    // Construncting the hex code
    if (instr.opcode == "STORE" || instr.opcode == "PUSH" || instr.opcode == "OUT" || instr.opcode == "STOREI"){
        out_file << op_code.at(instr.opcode) << instr.registers[2] << instr.registers[0] << instr.registers[1] << instr.dataline;
    }
    else{
        out_file << op_code.at(instr.opcode) << instr.registers[0] << instr.registers[1] << instr.registers[2] << instr.dataline;
    }
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

        

        // Debug: Print the line being processed
        cout << "Processing line " << line_num << ": " << line << "\n";

        if (!line.size() || (line[0] == ';' && line.size() == 1)) {
            line_num++;
            continue;
        }

        else if (line.size() < 4){
            hexfile << "Error: Invalid line at line " << ++line_num << "\n";
            continue;
        }

        else if (count(line.begin(), line.end(), ';') > 1){
            hexfile << "Error: Too many semicolon on line " << ++line_num << ". Ensure only 1 semi-colon at the end of each line.\n";
            continue;
        }
        
        else if (line[line.size() - 1] != ';'){
            hexfile << "Error: Missing semicolon at line " << ++line_num << "\n";
            continue;
        }

        parse(++line_num, line.substr(0, line.size() - 1), hexfile);
        hexfile << '\n';
    }
    hexfile << endl;

    assembly_code.close();
    hexfile.close();

    return 0;
}