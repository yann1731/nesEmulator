#ifndef BUS_HPP
# define BUS_HPP

# include "Cartridge.hpp"
# include "Cpu6502.hpp"
# include "Ppu2c02.hpp"
# include <memory>
# include <array>

class Bus {
public:
    Bus();
    ~Bus();

    Cpu6502 cpu_;

    Ppu2c02 ppu_;

    uint8_t cpuRead(uint16_t addr, bool readOnly = false);
    void cpuWrite(uint16_t addr, uint8_t data);

    void insertCartridge(const std::shared_ptr<Cartridge>& cart);
    void reset();
    void clock();

private:
    std::array<uint8_t, 2048> cpu_ram_;
    std::shared_ptr<Cartridge> cart_;
    uint32_t n_system_clock_counter_;
};

#endif