#include <bits/stdc++.h>
#include <fstream>

using namespace std;

static const vector<char> hex_chars = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

// struct Instruction{
//     string opcode;
//     string register1;
//     string register2;
//     string register3;
//     string dataline;
// };

// function to remove heading and trailing whitespaces in a line
string strip(string &s){
    int i = 0, start, last;
    start = s.find_first_not_of(" ");
    last = s.find_last_not_of(" ");
    return (start == string::npos? "" : s.substr(start, last - start + 1));
}

// function to check appropriate number of commas in a line
bool check_comma(int comma_cnt, string hex_code){
    if (hex_code == "00" && comma_cnt) return false;
    else if ((hex_code >= "01" && hex_code <= "08" || hex_code >= "18" && hex_code <="1A") && comma_cnt != 3) return false;
    else if ((hex_code >= "09" && hex_code <= "0C" || hex_code >= "14" && hex_code <="17") && comma_cnt != 2) return false;
    else if ((hex_code >= "0D" && hex_code <= "13" || hex_code >= "1B") && comma_cnt != 1) return false;
    else return true;
}

// function to get the related hex value of the opcode passed
string opcode_hex(string opcode){
    if (opcode == "NOP") return "0000000";
    else if (opcode == "AND") return "01";
    else if (opcode == "OR") return "02";
    else if (opcode == "EXOR") return "03";
    else if (opcode == "ADD") return "04";
    else if (opcode == "ANDI") return "05";
    else if (opcode == "ORI") return "06";
    else if (opcode == "EXORI") return "07";
    else if (opcode == "ADDI") return "08";
    else if (opcode == "MOV") return "09";
    else if (opcode == "MOVI") return "0A";
    else if (opcode == "LOAD") return "0B";
    else if (opcode == "STORE") return "0C";
    else if (opcode == "JMP") return "0D";
    else if (opcode == "JMPZ") return "0E";
    else if (opcode == "JMPNZ") return "0F";
    else if (opcode == "JMPC") return "10";
    else if (opcode == "JMPNC") return "11";
    else if (opcode == "PUSH") return "12";
    else if (opcode == "POP") return "13";
    else if (opcode == "IN") return "14";
    else if (opcode == "OUT") return "15";
    else if (opcode == "LOADI") return "16";
    else if (opcode == "STOREI") return "17";
    else if (opcode == "SUB") return "18";
    else if (opcode == "SHIFTR") return "19";
    else if (opcode == "SHIFTL") return "1A";
    else if (opcode == "JMPPCRZ") return "1B";
    else if (opcode == "JMPPCRNZ") return "1C";
    else return "";
}

void parse(int line_num, int comma_cnt, int R_count, string line, ofstream &out_file){
    string temp = "", parsed_code = "";
    int line_idx = 0;

    // This section is to get hex value for the opcode
    while(line[line_idx] != ',' && line_idx < line.size()){
        temp += line[line_idx++];
    }

    temp = strip(temp);
    parsed_code += opcode_hex(temp);
    if (parsed_code.empty()){
        out_file << "Invalid OP Code at line number " << line_num << ". OP Code supplied: " << temp << ".\n";
        out_file << "Kindly check whether there is a typo or some other silly error!";
        return;
    }
    else if (!check_comma(comma_cnt, parsed_code.substr(0,2))){
        out_file << "Error: Invalid number of commas at line " << line_num;
        return;
    }
    else if (parsed_code == "0000000"){
        out_file << parsed_code;
        return;
    }
    else{
        out_file << parsed_code;
    }
    temp.clear();

    return;


    // This section is to get hex value for registers
    if (parsed_code <= "0C" || parsed_code >= "12" && parsed_code <= "1A"){

    }
    else{
        parsed_code += "000";
    }

}

int main(){
    ifstream assembly_code("asmcode.txt");
    ofstream hexfile("asm2hex.txt");

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

        parse(++line_num, count(line.begin(), line.end(), ','), count(line.begin(), line.end(), 'R'), line.substr(0, line.size() - 1), hexfile);
        hexfile << endl;

    }

    assembly_code.close();
    hexfile.close();

    return 0;
}