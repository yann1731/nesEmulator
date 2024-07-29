#ifndef SHEADER_HPP
#define SHEADER_HPP

#ifdef __APPLE__
# include <_types/_uint8_t.h>
#else
# include <cstdint>
#endif

struct InesHeader {
	char name[4];
	uint8_t prg_rom_chunks;
	uint8_t chr_rom_chunks;
	uint8_t mapper1;
	uint8_t mapper2;
	uint8_t prg_ram_size;
	uint8_t tv_system_1;
	uint8_t tv_system_2;
	char unused[5];
};

#endif