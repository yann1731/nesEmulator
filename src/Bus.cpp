#include "../include/Bus.hpp"
#include <cstdint>

Bus::Bus(): n_system_clock_counter_(0) {
    for (uint8_t i : cpu_ram_)
        cpu_ram_[i] = 0x00;

    cpu_.connectBus(this);
}

Bus::~Bus() { }

uint8_t Bus::cpuRead(uint16_t addr, bool readOnly) {
    uint8_t data = 0x00;

    if (addr >= 0x0000 && addr <= 0x1fff)
        data = cpu_ram_[addr & 0x07ff];
    else if (addr >= 0x2000 && addr <= 0x3fff)
        data = ppu_.cpuRead(addr & 0x0007, readOnly);
    return data;
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {
    if (addr >= 0x0000 && addr <= 0x1fff)
        cpu_ram_[addr & 0x07ff] = data;
    else if (addr >= 0x2000 && addr <= 0x3fff)
        ppu_.cpuWrite(addr & 0x0007, data);
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cart) {
    cart_ = cart;
    ppu_.connectCartridge(cart);
}

void Bus::reset() {
    cpu_.reset();
}

void Bus::clock() {

}