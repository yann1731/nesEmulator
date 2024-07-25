#ifndef BUS_HPP
# define BUS_HPP

# include "header.hpp"
# include "Cpu6502.hpp"
# include "Ppu2c02.hpp"

class Bus {
public:
    Bus();
    ~Bus();

    Cpu6502 cpu;

    Ppu2c02 ppu;

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);
    
private:
    std::array<uint8_t, 2048> cpuRam;
};

#endif