#include "../include/Bus.hpp"
#include <cstdint>

Bus::Bus() {
    for (uint8_t i : cpuRam)
        cpuRam[i] = 0x00;

    cpu.connectBus(this);
}

Bus::~Bus() { }

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;

    if (addr >= 0x0000 && addr <= 0x1fff)
        data = cpuRam[addr & 0x07ff];
    else if (addr >= 0x2000 && addr <= 0x3fff)
    return data;
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0x1fff)
        cpuRam[addr & 0x07ff] = data;
}