#include "../include/Cpu6502.hpp"
#include "../include/Bus.hpp"

Cpu6502::Cpu6502() {

}

Cpu6502::~Cpu6502() {

}

void Cpu6502::read(uint16_t addr) {
    this->bus->read(addr);
}

uint8_t Cpu6502::write(uint16_t addr, uint8_t data) {
    this->bus->write(addr, data);
}