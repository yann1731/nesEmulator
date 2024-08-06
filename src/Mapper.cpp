#include "../include/Mapper.hpp"
#include <cstdint>

Mapper::Mapper(uint8_t prg_banks, uint8_t chr_banks): n_prg_banks_(prg_banks), n_chr_banks_(chr_banks) {}

Mapper::~Mapper() {}