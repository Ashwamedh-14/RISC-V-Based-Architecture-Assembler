#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <vector>
#include <fstream>

struct Instruction {
    std::string opcode;                                // Opcode of the instruction
    std::vector<std::string> registers = {"", "", ""}; // Registers used in the instruction
    int reg_num;                                       // Number of registers used in the instruction
    std::string dataline;                              // Data line of the instruction
};

// Helper Functions
void usage(void);
void toUpper(std::string &s);
std::string strip(const std::string &s);
std::string hexBinConversion(char c);

// Main Functions
int instr_chk(Instruction &instr); // Function to check whether the values in Instruction are correct
void parse(int line_num, const std::string &line, std::ofstream &out_file); // Function to parse the instruction and check for errors

extern bool ERR;

#endif // ASSEMBLER_H