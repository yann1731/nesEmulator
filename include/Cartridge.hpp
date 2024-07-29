#ifndef CARTRIDGE_HPP
# define CARTRIDGE_HPP

# include <cstdint>
# include <vector>
# include <string>
# include "InesHeader.hpp"
# include <cstring>
# include <fstream>
# include <iostream>

class Cartridge {
public:
	Cartridge(const std::string& cartName);
	~Cartridge();

	bool cpuRead(uint16_t addr, uint8_t data);
	bool cpuWrite(uint16_t addr, uint8_t &data);

	bool ppuRead(uint16_t addr, uint8_t data);
	bool ppuWrite(uint16_t addr, uint8_t &data);
private:
	std::vector<uint8_t> v_prg_mem_;
	std::vector<uint8_t> v_chr_mem_;
	
	InesHeader header;

	uint8_t n_mapper_id_;
	uint8_t n_prg_banks_;
	uint8_t n_chr_banks;
};

#endif