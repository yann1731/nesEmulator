#include "../include/Instruction.hpp"
#include "../include/Cpu6502.hpp"

INSTRUCTION::INSTRUCTION(): operate(nullptr), addrmode(nullptr), cycles(0) { }