#include "../include/Cpu6502.hpp"
#include "../include/Bus.hpp"

Cpu6502::Cpu6502(): bus(nullptr), status(0x00), PC(0x0000), SP(0x00),
    A(0x00), X(0x00), Y(0x00)
{ }

Cpu6502::~Cpu6502() { }

uint8_t Cpu6502::read(uint16_t addr) {
    return (this->bus->read(addr));
}

void Cpu6502::write(uint16_t addr, uint8_t data) {
    this->bus->write(addr, data);
}

void Cpu6502::connectBus(Bus *bus) {
    this->bus = bus;
}