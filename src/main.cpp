/*
Author: Ashwamedh-14
Date Started: 17 January 2025
Software: Assembly to Hex Converter, i.e, Assembler
Description: 
This program converts assembly code to hex code.
The program reads the assembly code from a file named "asmcode.txt" and writes the hex code to a file named "hexcode.txt".
It then also converts the hex code to binary code and writes to a file named "bin.txt".
People can specify the files to read or written to by the use of command line arguements:

./Assembler <input_file>.txt <output_file>.txt <binary_file>.txt

The assembly code should be written in the following format:
1. Each line should contain only one instruction or a label.
2. The instruction should be written in the following format:
    <OPCODE>, <REGISTER1>, <REGISTER2>, <REGISTER3>, <DATALINE>;
    where:
    a. OPCODE: The operation code of the instruction. It should be written in uppercase.
    b. REGISTER1, REGISTER2, REGISTER3: The registers used in the instruction. The registers should be written in the format "R<register number>". The register number should be written in decimal.
    c. DATALINE: The data to be used in the instruction. It should be written in hexadecimal, unless a label name. Can be PORT Address, MEM Address or a existing Label name too.
3. The instruction should be followed by a semicolon.
4. A label should be written in the following format:
    <Label_Name>:
    where <Label_Name> is a valid name of a label.
5. Rules regarding labels are:
    a. A line with a label should always end with a colon
    b. A label must always start with an alphabet or an underscore
    c. Only alphanumeric characters and underscores are allowed in valid label names.
*/

// Including the header file for getopt
extern "C" {                            // This ensure this particular code is compiled with C linkage

    // This file and getopt.c file in src folder are licenced under GNU LGPL v2.1
    #include "getopt.h"
}

// Including the rest of the headers
#include "assembler.h"    // Header file for the Assembler
#include <iostream>
#include <cstddef> // For size_t
#include <fstream>
#include <map>
#include <string>
#include <filesystem>
#include <algorithm>

#ifndef ASSEMBLER_VERSION
#define ASSEMBLER_VERSION "dev"
#endif

#define INVALID_INPUT_FILE 1
#define INVALID_OUTPUT_FILE 2
#define INVALID_BINARY_FILE 3
#define UNABLE_TO_OPEN_INPUT_FILE 4
#define UNABLE_TO_OPEN_OUTPUT_FILE 5
#define UNABLE_TO_OPEN_BINARY_FILE 6
#define COMMAND_LINE_ERROR 7
#define ASSEMBLY_CODE_ERROR  8

using namespace std;

void usage(void);  // Function to tell what to pass is expected in command line arguement

int main(int argc, char **argv){
    string input = "asmcode.txt";       // Default input file
    string output = "hexcode.txt";      // Default output file
    string binary = "bin.txt";          // Default binary file
    string formatted = "format.txt";    // Default format file for assembly
    string label = "";                  // Keeping label blank in case there is no label at line 0
    string line;
    size_t line_num = 0;                // Line number of the assembly code 
    char c;         // Variable to store the command line argument
    map<string, size_t> labels;

    // Setting ERR to false;
    ERR = false;
    /*
     * Flag to make checks
     * 0th bit set. Make a binary file (default);
     * 1st bit set. Only compile till the formatted file, will generate the format file
     * 2nd bit set. Generate the format file. Set with flag -f
    */
    unsigned char flag = 0x01;
    // Using getopt to parse the command line arguments
    while((c = getopt(argc, argv, ":i:o:b:f:cnhv")) != -1) {
        switch (c) {
            case 'i':
                input = optarg;
                if (input.find_last_of('.') == string::npos || input.substr(input.find_last_of('.') + 1) != "txt"){
                    cout << "Error: Invalid input file. The input file should be a text file.\n";
                    ERR = true;
                }
                break;

            case 'o':
                output = optarg;
                if (output.find_last_of('.') == string::npos || output.substr(output.find_last_of('.') + 1) != "txt"){
                    cout << "Error: Invalid output file. The output file should be a text file.\n";
                    ERR = true;
                }
                break;

            case 'b':
                binary = optarg;
                if (binary.find_last_of('.') == string::npos || binary.substr(binary.find_last_of('.') + 1) != "txt"){
                    cout << "Error: Invalid binary file. The binary file should be a text file.\n";
                    ERR = true;
                }
                break;

            case 'f':
                flag |= 0x04;
                formatted = optarg;
                if (formatted.find_last_of('.') == string::npos || formatted.substr(formatted.find_last_of('.') + 1) != "txt"){
                    cout << "Error: Invalid format file. The format file " << formatted << " should be a text file.\n";
                    ERR = true;
                }
                break;

            case 'c':
                flag |= 0x02;
                break;

            case 'n':
                flag &= 0xfe;         // Resetting bit 0;
                break;

            case 'h':
                usage();
                return 0;

            case 'v':
                cout << "Assembler Version: " << ASSEMBLER_VERSION << endl;
                return 0;

            case ':':
                cout << "Unrecognized option: " << (char)optopt << endl;
                ERR = true;
                break;

            case '?':
                cout << "Expected argument for option: " << (char)optopt << '\n';
                usage();
                return COMMAND_LINE_ERROR;
            default:
                usage();
                return COMMAND_LINE_ERROR;
        }
    }
    if (ERR) return COMMAND_LINE_ERROR; // If there was an error in the command line arguments, return error code
    
    // Checking whether we are able to open the input file
    ifstream assembly_code(input);
    if (!assembly_code.is_open()){
        cout << "Error: File " << input << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return UNABLE_TO_OPEN_INPUT_FILE;
    }
    
    
    // Checking whether we are able to open the format file
    ofstream format_file(formatted);
    if (!format_file.is_open()){
        cout << "Error: File " << formatted << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return UNABLE_TO_OPEN_OUTPUT_FILE;
    }

    

    // First pass
    // Formatting Assembly code
    // This parse does not check whether the line is valid instruction or not.
    // It checks just two things:
    // 1. If the line is blank, skip it, don't include it in the formatted assembly code
    // 2. If a label is encountered, check it, record it along with its line number, and replace it with an NOP statement
    //
    // **NOTE**
    // Since the labels will be replaced with the hexadecimal value of their locations in JMP statements, their line numbering starts from 0.
    while(getline(assembly_code, line)){      
        line = sanitizeLine(line);
        
        // The line should be now completely uppercase, stripped of leading and trailing whitespaces and tabs, and comments removed.
        if (!line.size()) continue;
        else if (line.find(':') != string::npos){
            c = isValidLabel(line);         // Reusing 'c' here since the return type is uint8_t which is typically an unsigned char
            
            if (c) format_file << "Error: Invalid Label at line " << ++line_num << ".\n";

            switch (c){
                case 0:
                    if (isLabelRecorded(line, labels)){
                        format_file << "Error: Already duplicate label: " << strip(line.substr(0, line.size() - 1)) << ", at line number " << ++line_num << ".\n";
                        format_file << "Label already defined at: " << labels[strip(line.substr(0, line.size() - 1))] + 1 << ".\n";
                        ERR = true;
                        continue;
                    }
                    line = sanitizeLine(line.substr(0, line.size() - 1));
                    labels[line] = line_num;
                    format_file << line << ":\n";
                    break;
                case 1:
                    format_file << "Empty labels are invalid\n";
                    ERR = true;
                    continue;

                case 2:
                    format_file << "Label ended with a semi-colon\n";
                    ERR = true;
                    continue;

                case 3:
                    format_file << "Label does not end with a colon\n";
                    ERR = true;
                    continue;

                case 4:
                    format_file << "Label: " << strip(line.substr(0, line.size() - 1)) << " is a valid OPCode, which is a reserved name\n";
                    ERR = true;
                    continue;

                case 5:
                    format_file << "Label: " << strip(line.substr(0, line.size() - 1)) << " is not a valid label name\n";
                    ERR = true;
                    continue;

                default:
                    format_file << "Unknown Label Error\n";
                    ERR = true;
            }
        }
        else {
            line = strip(line.substr(0, line.find_first_of(';')));
            if (!line.size()) continue;                           // Skip the line with only a semi-colon present;
            format_file << line << ";\n";
        }
        line_num++;
    }
    
    // flushing the formatted file
    format_file << flush;

    format_file.close();
    assembly_code.close();
    

    // At this point, all the assembly code should be formatted neatly in our intermediate text file.
    // There will be no spaces, all labels would be recorded and stored in a map to their expected line number.
    // Now we check if we hit any error, if yes, we don't begin the second parsing

    if (ERR) {
        cout << "Found Errors in labelling.\n";
        if (!(flag & 0x02)) cout << "Not converting to hex_code. See file: " << formatted << " for errors.\n";
        cout << "Exiting..." << endl;
        return ASSEMBLY_CODE_ERROR;
    }

    if (flag & 0x02) return 0;

    // If no errors were found, we open formatted file in read mode, and hex file in write mode.
    
    ifstream format_read(formatted);
    if (!format_read.is_open()){
        cout << "Error: File " << formatted << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to read it" << endl;
        return UNABLE_TO_OPEN_INPUT_FILE;
    }
    
    
    // Checking whether we are able to open the format file
    ofstream hexfile(output);
    if (!hexfile.is_open()){
        cout << "Error: File " << output << " was not found, or we were unable to open it.\n";
        cout << "Check whether you have the file in the same directory, as well as the permission to write to it" << endl;
        return UNABLE_TO_OPEN_OUTPUT_FILE;
    }
    
    // Resetting the line number
    line_num = 0;
    
    // Putting the header for logisim
    hexfile << "v2.0 raw\n";

    while(getline(format_read,line)){
        
        line_num++;
        
        // Convert assembly code to hex
        // and write to hexfile
        
        if (!isValidLabel(line)){                        // If the line is a valid label, the function returns a 0
            label = line.substr(0, line.size() - 1);
            hexfile << "0000000\n";
            continue;
        }

        else if (count(line.begin(), line.end(), ';') == 0){
            if (!label.empty()) hexfile << "In block: " << label << "\n";
            hexfile << "Error: Missing semicolon at line " << line_num << "\n";
            ERR = true;
            continue;
        }

        line = line.substr(0, line.find_first_of(';'));

        if (line.size() < 3){
            if (!label.empty()) hexfile << "In block: " << label << "\n";
            hexfile << "Error: Invalid line at line " << line_num << "\n";
            ERR = true;
            continue;
        }
        parse(line_num, line, label, labels, hexfile);
    }
    hexfile << flush;

    format_read.close();
    hexfile.close();

    if (!(flag & 0x04)) filesystem::remove(formatted);

    if (ERR){
        cout << "Error: Errors were found in the assembly code. Check the output file for more details." << endl;
        if (!(flag & 0x04)) cout << "If your source code had blank lines and labels, generate the formatted file and check against that file." << endl;
        if (flag & 0x01) cout << "Error: Failed to generate binary code." << endl;
        return ASSEMBLY_CODE_ERROR;
    }
    
    cout << "Hex code generated successfully. Check the output file: " << output << endl;

    // Binary code generation

    // Condition to check whether the user specified not to generate binary code
    if (!(flag & 0x01)){
        cout << "Specifically told not to generate binary code. Exiting the program." << endl;
        return 0;
    }
    
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
        for(uint8_t i = 1; i < 7; i++){
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
    cout << "Usage: ./Assembler <options> ...\n";
    cout << "Options being:\n";
    cout << "  -i <input_file> : Input file containing assembly code (default: asmcode.txt)\n";
    cout << "  -o <output_file> : Output file to write hex code (default: hexcode.txt)\n";
    cout << "  -b <binary_file> : Output file to write binary code (default: bin.txt)\n";
    cout << "  -f <format_file> : Format file to write formatted code (default: format.txt)\n";
    cout << "  -n : Tells to not generate binary code\n";
    cout << "  -h : Show this help message. It will override the execution of the program and only show this message\n";
    cout << "\n\nKindly Note:-\n";
    cout << "  1. All the files should be in the same directory as this executable.\n";
    cout << "  2. If not in the same directory, the path specified should be valid.\n";
    cout << "  3. All the I/O files should be a text files.\n";
    cout << "  4. The input file should contain assembly code in the specified format.\n";
    cout << "  5. The output file will be overwritten if it already exists.\n";
    cout << "  6. The binary file will be overwritten if it already exists.\n";
    cout << "  7. Flags followed by <value> means that providing a value upon invoking those flags is necessary.\n";
    cout << "  8. -n ovverides the -b flag, if provided." << endl;
}

