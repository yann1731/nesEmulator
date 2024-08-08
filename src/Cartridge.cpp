#include "../include/Cartridge.hpp"
#include "../include/InesHeader.hpp"
#include <_types/_uint32_t.h>
#include <memory>

Cartridge::Cartridge(const std::string& cartName): v_prg_mem_(0),
v_chr_mem_(0),
n_mapper_id_(0),
n_prg_banks_(0),
n_chr_banks(0) {
	memset(&header_, 0, sizeof(header_));

	std::ifstream file;
	file.open(cartName, std::ifstream::binary);
	if (file.is_open()) {
		file.read((char*) &header_, sizeof(header_));

		if (header_.mapper1 & 0x04)
			file.seekg(512, std::ios_base::cur);
		n_mapper_id_ = ((header_.mapper2 >> 4) << 4) | (header_.mapper1 >> 4);

		uint8_t n_file_type = 1;
		if (n_file_type == 0) {

		}

		if (n_file_type == 1) {
			n_prg_banks_ = header_.prg_rom_chunks;
			v_prg_mem_.resize(n_prg_banks_ * 16384);
			file.read((char *)v_prg_mem_.data(), v_prg_mem_.size());

			n_chr_banks = header_.chr_rom_chunks;
			v_chr_mem_.resize(n_chr_banks * 8192);
			file.read((char *)v_chr_mem_.data(), v_chr_mem_.size());
		}

		if (n_file_type == 2) {

		}

		switch (n_mapper_id_) {
			case 0:
				mapper_ = std::make_shared<Mapper000>(n_prg_banks_, n_chr_banks);
				break;
		}

		file.close();
	}
	else {
		std::cerr << "Error opening file" << std::endl;
	}
}	

Cartridge::~Cartridge() {

}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
	uint32_t mapped_addr = 0;

	if (mapper_->cpuMapRead(addr, mapped_addr)) {
		data = v_prg_mem_[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;

	if (mapper_->cpuMapWrite(addr, mapped_addr)) {
		v_prg_mem_[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) {
	uint32_t mapped_addr = 0;

	if (mapper_->ppuMapRead(addr, mapped_addr)) {
		data = v_chr_mem_[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
	uint32_t mapped_addr = 0;

	if (mapper_->ppuMapWrite(addr, mapped_addr)) {
		v_chr_mem_[mapped_addr] = data;
		return true;
	}
	else
		return false;
}