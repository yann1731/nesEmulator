#include "../include/Mapper000.hpp"

Mapper000::Mapper000(uint8_t prg_banks, uint8_t chr_banks):Mapper(prg_banks, chr_banks) {}

Mapper000::~Mapper000() {}

bool Mapper000::cpuMapRead(uint16_t addr, uint32_t &mapped_addr) {
	if (addr >= 0x8000 && addr <= 0xffff) {
		mapped_addr = addr & (n_prg_banks_ > 1 ? 0x7fff : 0x3fff);
		return true;
	}
	return false;
}

bool Mapper000::cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
	if (addr >= 0x8000 && addr <= 0xffff) {
		mapped_addr = addr & (n_prg_banks_ > 1 ? 0x7fff : 0x3fff);
		return true;
	}
	return false;
}

bool Mapper000::ppuMapRead(uint16_t addr, uint32_t &mapped_addr) {
	if (addr >= 0x0000 && addr <= 0x1fff) {
		mapped_addr = addr;
		return true;
	}
	return false;
}

bool Mapper000::ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) {
	if (addr >= 0x0000 && addr <= 0x1fff) {
		if (n_chr_banks_ == 0) {
			mapped_addr = addr;
			return true;
		}
	}
	return false;
}
