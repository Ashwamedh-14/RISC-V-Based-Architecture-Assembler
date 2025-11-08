#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <array>
#include <cstdint>
#include <fstream>

struct Opcode{
    std::string opcode;
    std::string hex;
    int max_instruction;
};

struct Instruction {
    std::string opcode;                                            // Opcode of the instruction
    std::array<std::string, 3> registers = {"", "", ""};           // Registers used in the instruction
    int reg_num;                                                   // Number of registers used in the instruction
    std::string dataline;                                          // Data line of the instruction
};

// Helper Functions
std::string hexBinConversion(char c);

// Main Functions
uint8_t instr_chk(Instruction &instr); // Function to check whether the values in Instruction are correct
void parse(size_t line_num, const std::string &line, std::ofstream &out_file); // Function to parse the instruction and check for errors

extern bool ERR;

#endif // ASSEMBLER_H
