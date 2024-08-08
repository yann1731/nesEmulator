#ifndef MAPPER000_HPP
#define MAPPER000_HPP

# include "Mapper.hpp"

class Mapper000: public Mapper {
public:
	Mapper000(uint8_t prg_banks, uint8_t chr_banks);
	~Mapper000();

	bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) override;
};

#endif