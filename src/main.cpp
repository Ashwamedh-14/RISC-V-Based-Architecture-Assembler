/*
Author: Ashwamedh-14
Date Started: 17 January 2025
Software: Assembly to Hex Converter, i.e, Assembler
Description: 
This program converts assembly code to hex code.
The program reads the assembly code from a file named "asmcode.txt" and writes the hex code to a file named "hexcode.txt".
It then also converts the hex code to binary code and write to a file named "bin.txt".
People can specify the files to read or written to by the use of command line arguements:

./Assembler <input_file>.txt <output_file>.txt <binary_file>.txt

The assembly code should be written in the following format:
1. Each line should contain only one instruction.
2. The instruction should be written in the following format:
    <OPCODE>, <REGISTER1>, <REGISTER2>, <REGISTER3>, <DATALINE>;
    where:
    a. OPCODE: The operation code of the instruction. It should be written in uppercase.
    b. REGISTER1, REGISTER2, REGISTER3: The registers used in the instruction. The registers should be written in the format "R<register number>". The register number should be written in decimal.
    c. DATALINE: The data to be used in the instruction. It should be written in hexadecimal. Can be PORT or MEM Address too
3. The instruction should be followed by a semicolon.
*/

#include "assembler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <string>

using namespace std;

int main(int argc, char **argv){
    string input = "asmcode.txt";     // Default input file
    string output = "hexcode.txt";    // Default output file
    string binary = "bin.txt";

    // Check for command line arguements
    if (argc > 1) input = argv[1];       // If one arguement is passed then it is the input file
    if (argc > 2) output = argv[2];      // If two arguements are passed then the second arguement is the output file
    if (argc > 3) binary = argv[3];     // If three arguements are passed then the third arguement is the binary file

    // Ensure I/O files have .txt extension
    if (input.find_last_of('.') == string::npos || input.substr(input.find_last_of('.') +1) != "txt"){
        usage();
        return 9;
    }
    else if (output.find_last_of('.') == string::npos || output.substr(output.find_last_of('.') +1) != "txt"){
        usage();
        return 10;
    }
    else if (binary.find_last_of('.') == string::npos || binary.substr(binary.find_last_of('.') +1) != "txt"){
        usage();
        return 11;
    }

    ifstream assembly_code(input);
    
    int line_num = 0;
    
    // Checking whether we are able to open the input file
    if (!assembly_code.is_open()){
        cout << "Error: File " << input << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return 12;
    }
    
    ofstream hexfile(output);

    // Checking whether we are able to open the output file
    if (!hexfile.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return 13;
    }
    
    string line;

    hexfile << "v2.0 raw\n";

    while(getline(assembly_code,line)){
        
        line_num++;
        
        // Convert assembly code to hex
        // and write to hexfile
        line = strip(line);
        toUpper(line);
        
        if (!line.size()){
            hexfile << "0000000\n";
            continue;
        }

        else if (count(line.begin(), line.end(), ';') == 0){
            hexfile << "Error: Missing semicolon at line " << line_num << "\n";
            ERR = true;
            continue;
        }

        line = line.substr(0, line.find_first_of(';'));

        if (!line.size()) {
            hexfile << "0000000\n";
            continue;
        }

        else if (line.size() < 3){
            hexfile << "Error: Invalid line at line " << line_num << "\n";
            ERR = true;
            continue;
        }
        

        parse(line_num, line, hexfile);
        hexfile << '\n';
    }
    hexfile << flush;

    assembly_code.close();
    hexfile.close();

    if (ERR){
        cout << "Error: Errors were found in the assembly code. Check the output file for more details." << '\n';
        cout << "Error: Failed to generate binary code." << endl;
        return 15;
    }

    // Converting the hexcode to binary code and storing it in bin.txt
    ifstream hexfile_read(output);

    // Checking whether we are able to open the input file
    if (!hexfile_read.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return 12;
    }

    ofstream binaryfile(binary);
    
    // Checking whether we are able to open the output file
    if (!binaryfile.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return 14;
    }
    
    string hex_line;

    getline(hexfile_read, hex_line); // Read the first line and skip it
    while (getline(hexfile_read, hex_line)){
        binaryfile << hex_line[0];
        for(int i = 1; i < 7; i++){
            binaryfile << hexBinConversion(hex_line[i]);
        }
        binaryfile << '\n';
    }
    binaryfile << flush;

    hexfile_read.close();
    binaryfile.close();
    
    return 0;
}

