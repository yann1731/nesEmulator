#ifndef _PPU2C02_
#define _PPU2C02_
#endif

#include <cstdint>

class Ppu2c02 {
public:
	Ppu2c02();
	~Ppu2c02();

	uint8_t cpuRead(uint16_t addr, bool rdonly = false);
	void cpuWrite(uint16_t addr, uint8_t data);

	uint8_t ppuRead(uint16_t addr, bool rdonly = false);
	void ppuWrite(uint16_t addr, uint8_t data);
};