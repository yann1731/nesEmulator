#include "../include/Bus.hpp"

Bus::Bus() { }

Bus::~Bus() { }

uint8_t Bus::read(uint16_t addr, bool readOnly = false) {
    if (addr >= 0x0000 && addr <= 0xffff)
        return ram[addr];
}

void Bus::write(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0xffff)
        ram[addr] = data;
}