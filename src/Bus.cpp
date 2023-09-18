#include "../include/Bus.hpp"

Bus::Bus() {
    for (uint8_t i : ram)
        ram[i] = 0x00;

    cpu.connectBus(this);
}

Bus::~Bus() { }

uint8_t Bus::read(uint16_t addr, bool readOnly) {
    if (addr >= 0x0000 && addr <= 0xffff)
        return ram[addr];
    else
        return 0x00;
}

void Bus::write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0xffff)
        ram[addr] = data;
}