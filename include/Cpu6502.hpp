#pragma once
#include "header.hpp"

class Bus;
class Cpu6502 {
public:
    Cpu6502();
    ~Cpu6502();

private:
    Bus *bus;

    void read(uint16_t addr);
    uint8_t write(uint16_t addr, uint8_t data);
};