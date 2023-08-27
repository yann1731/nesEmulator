#pragma once
#include "header.hpp"
#include "Cpu6502.hpp"

class Bus {
public:
    Bus();
    ~Bus();

    Cpu6502 cpu;

    uint8_t read(uint16_t addr, bool readOnly = false);
    void write(uint16_t addr, uint8_t data);
private:
    std::array<uint8_t, 64 * 1024> ram;
};