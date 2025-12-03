#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <string>
#include <array>
#include <cstdint>
#include <fstream>
#include <map>

struct Opcode{
    /*
    * The 2 LSB bit indicate total number of parameters needed for given opcode
    * The next 2 indicate number of registers needed in parameters
    * The next bit indicates whether a Dataline value is required
    */
    unsigned char instr_num;
    std::string opcode;
    std::string hex;
};

struct Instruction {
    const Opcode *opcode;                                            // Opcode of the instruction
    std::array<std::string, 3> registers = {"", "", ""};           // Registers used in the instruction
    int reg_num;                                                   // Number of registers used in the instruction
    std::string dataline;                                          // Data line of the instruction
};

// Helper Functions
void toUpper(std::string &s);
std::string strip(const std::string &s);
std::string sanitizeLine(const std::string &s);

// Check functions
std::string hexBinConversion(char c);
uint8_t isValidLabel(const std::string &s);
bool isLabelRecorded(const std::string &s, const std::map<std::string, size_t> &labels);

// Main Functions
uint8_t instr_chk(Instruction &instr, const std::map<std::string, size_t> &labels); // Function to check whether the values in Instruction are correct
uint8_t parse(size_t line_num, const std::string &line, const std::string block_label, const std::map<std::string, size_t> &labels, std::ofstream &out_file); // Function to parse the instruction and check for errors

extern bool ERR;

#endif // ASSEMBLER_H
