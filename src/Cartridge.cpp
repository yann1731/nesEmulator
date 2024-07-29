#include "../include/Cartridge.hpp"
#include "../include/InesHeader.hpp"

Cartridge::Cartridge(const std::string& cartName): v_prg_mem_(0), v_chr_mem_(0) {
	memset(&header, 0, sizeof(header));

	std::ifstream file;
	file.open(cartName, std::ifstream::binary);
	if (file.is_open()) {
		file.read((char*) &header, sizeof(header));
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