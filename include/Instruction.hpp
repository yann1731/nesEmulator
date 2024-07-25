#ifndef INSTRUCTION_HPP
# define INSTRUCTION_HPP
# include "header.hpp"

class Cpu6502;
struct INSTRUCTION {
    std::string name;
    uint8_t(Cpu6502::*operate)(void);
    uint8_t(Cpu6502::*addrmode)(void);
    uint8_t cycles;
};

#endif