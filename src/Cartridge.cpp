#include "../include/Cartridge.hpp"
#include "../include/InesHeader.hpp"

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
		if (n_file_type == 0) {

		}

		if (n_file_type == 1) {
			n_prg_banks_ = header.prg_rom_chunks;
			v_prg_mem_.resize(n_prg_banks_ * 16384);
			file.read((char *)v_prg_mem_.data(), v_prg_mem_.size());

			n_chr_banks = header.chr_rom_chunks;
			v_chr_mem_.resize(n_chr_banks * 8192);
			file.read((char *)v_chr_mem_.data(), v_chr_mem_.size());
		}

		if (n_file_type == 2) {

		}

		file.close();
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