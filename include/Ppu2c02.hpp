#ifndef PPU2C02_HPP
# define PPU2C02_HPP

# include <cstdint>
# include <memory>
#ifdef __APPLE__
# include <_types/_uint8_t.h>
#else
# include <sys/types.h>
#endif

# include "Cartridge.hpp"

class Ppu2c02 {
public:
	Ppu2c02();
	~Ppu2c02();

	uint8_t cpuRead(uint16_t addr, bool readOnly = false);
	void cpuWrite(uint16_t addr, uint8_t data);

	uint8_t ppuRead(uint16_t addr, bool readOnly = false);
	void ppuWrite(uint16_t addr, uint8_t data);

	void connectCartridge(const std::shared_ptr<Cartridge>& cart);
	void clock();
private:
	std::shared_ptr<Cartridge> cart_;
	uint8_t tbl_name_[2][1024];
	uint8_t tbl_palette_[32];
};

#endif