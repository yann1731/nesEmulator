#ifndef CARTRIDGE_HPP
# define CARTRIDGE_HPP

#include <_types/_uint8_t.h>
# include <cstdint>
# include <vector>
class Cartridge {
public:
	bool cpuRead(uint16_t addr, uint8_t data);
	bool cpuWrite(uint16_t addr, uint8_t &data);

	bool ppuRead(uint16_t addr, uint8_t data);
	bool ppuWrite(uint16_t addr, uint8_t &data);
private:
	std::vector<uint8_t> v_prg_mem_;
	std::vector<uint8_t> v_chr_mem_;
};

#endif