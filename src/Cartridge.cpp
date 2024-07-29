#include "../include/Cartridge.hpp"
#include "../include/InesHeader.hpp"
#include <cstdint>
#include <ios>

Cartridge::Cartridge(const std::string& cartName): v_prg_mem_(0),
v_chr_mem_(0),
n_mapper_id_(0),
n_prg_banks_(0),
n_chr_banks(0) {
	memset(&header, 0, sizeof(header));

	std::ifstream file;
	file.open(cartName, std::ifstream::binary);
	if (file.is_open()) {
		file.read((char*) &header, sizeof(header));

		if (header.mapper1 & 0x04)
			file.seekg(512, std::ios_base::cur);
		n_mapper_id_ = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);

		uint8_t n_file_type = 1;
		
	}
	else {
		std::cerr << "Error opening file" << std::endl;
	}
}	

Cartridge::~Cartridge() {

}

bool Cartridge::cpuRead(uint16_t addr, uint8_t data) {

}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t &data) {

}

bool Cartridge::ppuRead(uint16_t addr, uint8_t data) {

}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t &data) {

}