#ifndef MAPPER_HPP
#define MAPPER_HPP

# include <cstdint>

class Mapper {
public:
    Mapper(uint8_t prg_banks, uint8_t chr_banks);
    ~Mapper();

    virtual bool cpuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool cpuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapRead(uint16_t addr, uint32_t &mapped_addr) = 0;
    virtual bool ppuMapWrite(uint16_t addr, uint32_t &mapped_addr) = 0;

protected:
    uint8_t n_prg_banks_;
    uint8_t n_chr_banks_;
};

#endif