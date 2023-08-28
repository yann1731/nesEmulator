#pragma once
#include "header.hpp"

class Bus;
class Cpu6502 {
public:
    Cpu6502();
    ~Cpu6502();

    enum PS {
        C = (1 << 0), //Carry flag
        Z = (1 << 1), //Zero flag
        I = (1 << 2), //interrupt disable
        D = (1 << 3), //Decimal mode
        B = (1 << 4), //Break command
        U = (1 << 5), //Unused
        O = (1 << 6), //Overflow flag
        N = (1 << 7), //Negative
    };

private:
    Bus *bus;

    uint16_t PC;
    uint8_t SP;
    uint8_t A;
    uint8_t X;
    uint8_t Y;

    void read(uint16_t addr);
    uint8_t write(uint16_t addr, uint8_t data);
};