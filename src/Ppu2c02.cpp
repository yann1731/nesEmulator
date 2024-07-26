#include "../include/Ppu2c02.hpp"
#include <_types/_uint8_t.h>
#include <cstdint>
#include <sys/types.h>

uint8_t Ppu2c02::cpuRead(uint16_t addr, bool readOnly) {
	uint8_t data = 0x00;

	switch(addr) {
		case 0x0000:
			break;
		case 0x0001:
			break;
		case 0x0002:
			break;
		case 0x0003:
			break;
		case 0x0004:
			break;
		case 0x0005:
			break;
		case 0x0006:
			break;
		case 0x0007:
			break;
	}

	return data;
}

void Ppu2c02::cpuWrite(uint16_t addr, uint8_t data) {
	switch(addr) {
		case 0x0000:
			break;
		case 0x0001:
			break;
		case 0x0002:
			break;
		case 0x0003:
			break;
		case 0x0004:
			break;
		case 0x0005:
			break;
		case 0x0006:
			break;
		case 0x0007:
			break;
	}
}

uint8_t Ppu2c02::ppuRead(uint16_t addr, bool readOnly) {
	uint8_t data = 0x00;

	addr &= 0x3fff;

	return data;
}

void Ppu2c02::ppuWrite(uint16_t addr, uint8_t data) {
	addr &= 0x3fff;
}

void Ppu2c02::connectCartridge(const std::shared_ptr<Cartridge> &cart) {
	this->cart_ = cart;
}