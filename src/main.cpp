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

// Including the header file for getopt
extern "C" {                            // This ensure this particular code is compiled with C linkage

    // This file and getopt.c file in src folder are licenced under GNU LGPL v2.1
    #include "getopt.h"
}

// Including the rest of the headers
#include "assembler.h"    // Header file for the Assembler
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>
#include <string>

#define INVALID_INPUT_FILE 1
#define INVALID_OUTPUT_FILE 2
#define INVALID_BINARY_FILE 3
#define UNABLE_TO_OPEN_INPUT_FILE 4
#define UNABLE_TO_OPEN_OUTPUT_FILE 5
#define UNABLE_TO_OPEN_BINARY_FILE 6
#define ASSEMBLY_CODE_ERROR  7
#define COMMAND_LINE_ERROR 8

using namespace std;

void usage(void);  // Function to tell what to pass is expected in command line arguement

int main(int argc, char **argv){
    string input = "asmcode.txt";     // Default input file
    string output = "hexcode.txt";    // Default output file
    string binary = "bin.txt";         // Default binary file
    int line_num = 0;           // Line number of the assembly code
    bool make_bin = true; // Flag to check whether to make binary code or not
    char c;         // Variable to store the command line argument

    // Using getopt to parse the command line arguments
    while((c = getopt(argc, argv, ":i:o:nb:")) != -1) {
        switch (c) {
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'b':
                binary = optarg;
                break;
                case 'n':
                make_bin = false;
                break;
            case ':':
                cout << "Unrecognized option: " << (char)optopt << endl;
                return ASSEMBLY_CODE_ERROR;
            case '?':
                cout << "Expected argument for option: " << (char)optopt << '\n';
                usage();
                cout << flush;
                return COMMAND_LINE_ERROR;
            default:
                usage();
                cout << flush;
                return COMMAND_LINE_ERROR;
        }
    }
    
    
    // Checking whether we are able to open the input file
    ifstream assembly_code(input);
    if (!assembly_code.is_open()){
        cout << "Error: File " << input << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return UNABLE_TO_OPEN_INPUT_FILE;
    }
    
    
    // Checking whether we are able to open the output file
    ofstream hexfile(output);
    if (!hexfile.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return UNABLE_TO_OPEN_OUTPUT_FILE;
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
        return ASSEMBLY_CODE_ERROR;
    }

    // Converting the hexcode to binary code and storing it in bin.txt
    
    // Checking whether we are able to open the input file
    ifstream hexfile_read(output);
    if (!hexfile_read.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return UNABLE_TO_OPEN_INPUT_FILE;
    }

    
    // Checking whether we are able to open the output file
    ofstream binaryfile(binary);
    if (!binaryfile.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return UNABLE_TO_OPEN_BINARY_FILE;
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

// function to print use of command line arguement.
void usage(void) {
    cout << "Usage: ./Assembler <input_file>.txt <output_file>.txt <binary_file>.txt\n";
    cout << "Default input file: asmcode.txt\n";
    cout << "Default output file: hexcode.txt\n";
    cout << "Default binary file: bin.txt\n";
    cout << "Input, Output, and Binary files should be .txt files\n";
}