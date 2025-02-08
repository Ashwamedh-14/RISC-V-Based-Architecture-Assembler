#include <bits/stdc++.h>
#include <fstream>
#include <Sstream>

using namespace std;

static const vector<char> hex_chars = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static const map<string,string> op_code = {
    {"NOP", "0000000"},
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
        if (s[i] >= 97 || s[i] <= 122) s[i] = s[i] - 32;
    }
}

// function to remove heading and trailing whitespaces in a line
string strip(string &s){
    int i = 0, start, last;
    start = s.find_first_not_of(" ");
    last = s.find_last_not_of(" ");
    return (start == string::npos? "" : s.substr(start, last - start + 1));
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
    regex reg_reg("^[0-9]{1,2}");
    int temp;
    if (op_code.find(instr.opcode) == op_code.end()) return 1;    // Checks valid opcode
    for (int i = 0; i < instr.reg_num; i++){
        if (instr.registers[i].empty()) break;
        else if (!regex_match(instr.registers[i].substr(1,instr.registers[i].size() - 1), reg_reg)) return i + 3;
        temp = stoi(instr.registers[i].substr(1, instr.registers[i].size() - 1));
        if (temp >= 16) return i + 6;
    }
    if (!instr.dataline.empty() && !regex_match(instr.dataline, reg_dat)) return 2;
    return 0;
}

void parse(int line_num, string line, ofstream &out_file){
    Instruction instr;
    stringstream ss(line);
    string temp = "", parsed_code = "";
    int line_idx = 0;

    instr.reg_num = 0;
    instr.dataline = "";

    // This section is to get the opcode
    getline(ss, temp, ',');
    instr.opcode = strip(temp);
    line_idx++;

    // This section is to get rest of the Instruction
    while (getline(ss, temp, ',')){
        temp = strip(temp);
        if (temp.empty()){
            out_file << "Error: Invalid line at line " << line_num << ".\n";
            out_file << "Passed Value: \"\"";
            return;
        }

        if (temp[0] == 'R'){
            if (temp.size() == 2 || temp.size() == 3){
                if (instr.reg_num < 3){
                    instr.registers[instr.reg_num] = temp;
                    instr.reg_num++;
                }
                else{
                    out_file << "Error: Too many registers at line number " << line_num << ".\n";
                    return;
                }
            }
            else{
                out_file << "Error: Invalid Register at line " << line_num << ".";
                return;
            }
        }
        else if(temp.size() == 2){
            instr.dataline = temp;
        }
        else{
            out_file << "Error: Invalid Instruction at line number " << line_num << ".\n";
            out_file << "Passed Value: " << temp;
            return;
        }
        line_idx++;
    }
    if (line_idx >= 5){
        out_file << "Error: Too many instructions at line number: " << line_num << ".\n";
        out_file << "Number of instructions found: " << line_idx;
        return;
    }

    if (op_code.find(instr.opcode) == op_code.end()){
        out_file << "Error: Invalid Opcode at line number " << line_num << ".\n";
        out_file << "Passed Opcode: " << instr.opcode;
        return;
    }

    if (instr.opcode == "NOP"){
        if (instr.reg_num || !instr.dataline.empty()){
            out_file << "Error: Invalid reference to registers or dataline on line number: " << line_num << ".\n";
            out_file << "Opcode: " << instr.opcode;
            return;
        }
    }

}

int main(int argc, char **argv){
    string input = "asmcode.txt";
    string output = "hexcode.txt";

    ifstream assembly_code(input);
    ofstream hexfile(output);

    int line_num = 0;

    if (!assembly_code.is_open()){
        cout << "Error: File asmcode.txt not found" << endl;
        return 1;
    }

    string line;

    while(getline(assembly_code,line)){
        // Convert assembly code to hex
        // and write to hexfile
        line = strip(line);
        toUpper(line);

        if (!line.size() || (line[0] == ';' && line.size() == 1)) {
            line_num++;
            continue;
        }

        else if (line.size() < 4){
            hexfile << "Error: Invalid line at line " << ++line_num << endl;
            continue;
        }
        
        else if (line[line.size() - 1] != ';'){
            hexfile << "Error: Missing semicolon at line " << ++line_num << endl;
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