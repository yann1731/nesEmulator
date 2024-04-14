/*
License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018-2019 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Background
	~~~~~~~~~~
	I love this microprocessor. It was at the heart of two of my favourite
	machines, the BBC Micro, and the Nintendo Entertainment System, as well
	as countless others in that era. I learnt to program on the Model B, and
	I learnt to love games on the NES, so in many ways, this processor is
	why I am the way I am today.

	In February 2019, I decided to undertake a selfish personal project and
	build a NES emulator. Ive always wanted to, and as such I've avoided
	looking at source code for such things. This made making this a real
	personal challenge. I know its been done countless times, and very likely
	in far more clever and accurate ways than mine, but I'm proud of this.

	Datasheet: http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

	Files: olc6502.h, olc6502.cpp

	Relevant Video: https://www.youtube.com/watch?v=8XmxKPJDGU0

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
				https://www.youtube.com/javidx9extra
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Patreon:	https://www.patreon.com/javidx9
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019
*/

#include "../include/Cpu6502.hpp"
#include "../include/Bus.hpp"
#include <cstdint>
#include <sys/types.h>

Cpu6502::Cpu6502(): bus(nullptr), status(0x00), pc_(0x0000), sp_(0x00),
    a_(0x00), x_(0x00), y_(0x00), fetched(0x00), addrAbs(0x0000),
     addrRel(0x0000), opCode(0x00), cycles(0)
{
    using a = Cpu6502;
    lookup = //Thanks to olc. Didn't have to write it all out
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IIX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IIX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IIX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IIX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IIX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IIY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IIX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IIX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IIX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IIY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

void Cpu6502::clock() {
    if (cycles == 0) {
        opCode = read(pc_);
        pc_++;

        cycles = lookup[opCode].cycles;

        uint8_t addCycle1 = (this->*lookup[opCode].operate)();

        uint8_t addCycle2 = (this->*lookup[opCode].addrmode)();

        cycles += (addCycle1 & addCycle2);
    }
    cycles--;
}

uint8_t Cpu6502::fetch() {
    if (!(lookup[opCode].addrmode == &Cpu6502::IMP))
        fetched = read(addrAbs);
    return fetched;
}

Cpu6502::~Cpu6502() { }

uint8_t Cpu6502::read(uint16_t addr) {
    return (this->bus->read(addr));
}

void Cpu6502::write(uint16_t addr, uint8_t data) {
    this->bus->write(addr, data);
}

void Cpu6502::connectBus(Bus *bus) {
    this->bus = bus;
}

uint8_t Cpu6502::getFlags(CPUStatusFlags f) {
    return ((status & f) > 0) ? 1 : 0;
}

void Cpu6502::setFlags(CPUStatusFlags f, bool v) {
    if (v) {
        status |= f;
    }
    else {
        status &= ~f;
    }
}

/* Adressing modes */

uint8_t Cpu6502::IMP() {
	fetched = a_;
	return 0;
}

uint8_t Cpu6502::IMM() {
    addrAbs = pc_++;
	return 0;
}

uint8_t Cpu6502::ZP0() {
    addrAbs = read(pc_++);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::ZPX() {
    addrAbs = (read(pc_++) + x_);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::ZPY() {
    addrAbs = (read(pc_++) + y_);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::REL() {
    addrRel = read(pc_++);
    if (addrRel & 0x80) {
        addrRel |= 0xFF00;
    }
    return 0;
}

uint8_t Cpu6502::ABS() {
    uint16_t lo = read(pc_++);
    uint16_t hi = read(pc_++);

    addrAbs = (hi << 8) | lo;

    return 0;
}

uint8_t Cpu6502::ABX() {
    uint16_t lo = read(pc_++);
    uint16_t hi = read(pc_++);

    addrAbs = (hi << 8) | lo;
    addrAbs += x_;

    if ((addrAbs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t Cpu6502::ABY() {
    uint16_t lo = read(pc_++);
    uint16_t hi = read(pc_++);

    addrAbs = (hi << 8) | lo;
    addrAbs += y_;

    if ((addrAbs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t Cpu6502::IND() {
    uint16_t ptrLo = read(pc_++);
    uint16_t ptrHi = read(pc_++);

    uint16_t ptr = (ptrHi << 8) | ptrLo;

    if (ptrLo == 0x00FF) {
        addrAbs = (read(0x00FF) << 8) | read(ptr + 0);
    } else {
        addrAbs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

uint8_t Cpu6502::IIX() {
    uint16_t t = read(pc_++);

    uint16_t lo = read((uint16_t) (t + (uint16_t)x_) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)x_ + 1) & 0x00FF);

    addrAbs = (hi << 8) | lo;

    return 0;
}

uint8_t Cpu6502::IIY() {
    uint16_t t = read(pc_++);

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addrAbs = (hi << 8) | lo;
    addrAbs += y_;

    if ((addrAbs & 0x00FF) != (hi << 8))
        return 1;
    else
        return 0;
}

/* Cpu instructions */

uint8_t Cpu6502::LDA() {
    fetch();
    a_ = fetched;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 1;
}

uint8_t Cpu6502::LDX() {
    fetch();
    x_ = fetched;
    setFlags(Z, x_ == 0x00);
    setFlags(N, x_ & 0x80);
    return 1;
}

uint8_t Cpu6502::LDY() {
    fetch();
    y_ = fetched;
    setFlags(Z, y_ == 0x00);
    setFlags(N, y_ & 0x80);
    return 1;
}

uint8_t Cpu6502::STA() {
    write(addrAbs, a_);
    return 0;
}

uint8_t Cpu6502::STX() {
    write(addrAbs, x_);
    return 0;
}

uint8_t Cpu6502::STY() {
    write(addrAbs, y_);
    return 0;
}

uint8_t Cpu6502::TAX() {
    x_ = a_;
    setFlags(Z, x_ == 0x00);
    setFlags(N, x_ & 0x80);
    return 0;
}

uint8_t Cpu6502::TAY() {
    y_ = a_;
    setFlags(Z, y_ == 0x00);
    setFlags(N, y_ & 0x80);
    return 0;
}

uint8_t Cpu6502::TXA() {
    a_ = x_;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 0;
}

uint8_t Cpu6502::TYA() {
    a_ = y_;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 0;
}

uint8_t Cpu6502::TSX() { //Transfer stack pointer to X
    x_ = sp_;
    setFlags(Z, x_ == 0x00);
    setFlags(N, x_ & 0x80);
    return 0;
}

uint8_t Cpu6502::TXS() { //Transfer X to stack pointer
    sp_ = x_;
    return 0;
}

uint8_t Cpu6502::PHA() { //Push accumulator to stack
    write(0x0100 + sp_, a_);
    sp_--;
    return 0;   
}

uint8_t Cpu6502::PHP() { //Push processor status to stack
    write(0x0100 + sp_, status | B | U);
    setFlags(B, 0);
    setFlags(U, 0);
    sp_--;
    return 0;
}

uint8_t Cpu6502::PLA() { //Pulls an 8 bit value from the stack and into the accumulator
    sp_++;
    a_ = read(0x0100 + sp_);
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 0;
}

uint8_t Cpu6502::PLP() { //Pulls an 8 bit value from the stack and into the processor flags
    sp_++;
    status = read(0x0100 + sp_);
    setFlags(U, 1);
    return 0;
}

uint8_t Cpu6502::AND() { //Logical AND
    fetch();
    a_ = a_ & fetched;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 1;
}

uint8_t Cpu6502::EOR() { //Exclusive OR
    fetch();
    a_ = a_ ^ fetched;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 1;
}

uint8_t Cpu6502::ORA() { //Logical inclusive OR
    fetch();
    a_ = a_ | fetched;
    setFlags(Z, a_ == 0x00);
    setFlags(N, a_ & 0x80);
    return 1;
}

uint8_t Cpu6502::BIT() { //Bit test
    fetch();
    uint16_t temp = 0x0000; 
    temp = a_ & fetched;
    setFlags(Z, (temp & 0x00FF) == 0x00);
    setFlags(N, fetched & (1 << 7));
    setFlags(V, fetched & (1 << 6));
    return 0;
}

uint8_t Cpu6502::ADC() { //Add with carry
    fetch();
    uint16_t temp = 0x0000;
    temp = (uint16_t) a_ + (uint16_t) fetched + (uint16_t) getFlags(C);

    setFlags(C, temp > 255);
    setFlags(Z, (temp & 0x00FF) == 0);
    setFlags(V, (~((uint16_t) a_ ^ (uint16_t) fetched) & ((uint16_t) a_ ^ (uint16_t)temp)) & 0x0080);
    setFlags(N, temp & 0x80);
    a_ = temp & 0x00FF;
    return 1;
}

uint8_t Cpu6502::SBC() { //Subtract with carry
    fetch();

    uint16_t temp = 0x0000;

    uint16_t value = (uint16_t) fetched ^ 0x00FF;
    temp = (uint16_t) a_ + value + (uint16_t) getFlags(C);
    setFlags(C, temp & 0xFF00);
    setFlags(Z, (temp & 0x00FF) == 0);
    setFlags(V, (temp ^ (uint16_t) a_) & ((temp & value) & 0x0080));
    setFlags(N, temp & 0x0080);
    a_ = temp & 0x00FF;
    return 1;
}

uint8_t Cpu6502::CMP() { //Compare accumulator
    fetch();

    uint16_t temp = a_ - (uint16_t) fetched;
    setFlags(C, a_ >= (uint16_t) fetched);
    setFlags(Z, a_ == (uint16_t) fetched); //may need to revisit this
    setFlags(N, temp & 0x0080);
    return 1;
}

uint8_t Cpu6502::CPX() { //Compare X register
    fetch();

    uint16_t temp = x_ - (uint16_t) fetched;
    setFlags(C, x_ >= (uint16_t) fetched);
    setFlags(Z, x_ == (uint16_t) fetched);
    setFlags(N, temp & 0x0080);
    return 1;
}

uint8_t Cpu6502::CPY() { //Compare Y register
    fetch();

    uint16_t temp = y_ - (uint16_t) fetched;
    setFlags(C, y_ >= (uint16_t) fetched);
    setFlags(Z, y_ == (uint16_t) fetched);
    setFlags(N, temp & 0x0080);
    return 1;
}

uint8_t Cpu6502::INC() { //Increment a memory location
    fetch();

    fetched++;
    setFlags(Z, fetched == 0x00);
    setFlags(N, fetched & 0x80);
    write(addrAbs, fetched);
    return 0;
}

uint8_t Cpu6502::INX() { //Increment the X register
    x_++;
    setFlags(Z, x_ == 0x00);
    setFlags(N, x_ & 0x80);
    return 0;
}

uint8_t Cpu6502::INY() { //Increment the Y register
    y_++;
    setFlags(Z, y_ == 0x00);
    setFlags(N, y_ & 0x80);
    return 0;
}

uint8_t Cpu6502::DEC() { //Decrement a memory location
    fetch();

    fetched--; //might need to revisit
    setFlags(Z, fetched == 0x00);
    setFlags(N, fetched & 0x80);
    write(addrAbs, fetched);
    return 0;
}

uint8_t Cpu6502::DEX() { //Decrement the X register
    x_--;
    setFlags(Z, x_ == 0x00);
    setFlags(N, x_ & 0x80);
    return 0;
}

uint8_t Cpu6502::DEY() { //Decrement the Y register
    y_--;
    setFlags(Z, y_ == 0x00);
    setFlags(N, y_ & 0x80);
    return 0;
}

//A,Z,C,N = M*2 or M,Z,C,N = M*2
uint8_t Cpu6502::ASL() { //Arithmetic shift left
    fetch();

    uint16_t temp = (uint16_t) fetched << 1;
    setFlags(Z, (temp & 0x00FF) == 0x00);
    setFlags(C, (temp & 0xFF00) > 0);
    setFlags(N, temp & 0x80);
    if (lookup[opCode].addrmode == &Cpu6502::IMP) {
        a_ = temp & 0x00FF;
    }
    else {
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}

// A,C,Z,N = A/2 or M,C,Z,N = M/2
// Each of the bits in A or M is shift one place to the right. The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
uint8_t Cpu6502::LSR() { //Logical shift right
    fetch();

    setFlags(C, fetched & 0x0001);
    uint16_t temp = fetched >> 1;
    setFlags(Z, (temp & 0x00FF) == 0x0000);
    setFlags(N, temp & 0x0080);
    if (lookup[opCode].addrmode == &Cpu6502::IMP) {
        a_ = temp & 0x00FF;
    }
    else {
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}

//Move each of the bits in either A or M one place to the left. Bit 0 is filled with the current value of the carry flag whilst the old bit 7 becomes the new carry flag value
uint8_t Cpu6502::ROL() { //Rotate left
    fetch();

    uint16_t temp = (uint16_t) (fetched << 1) | getFlags(C);
    setFlags(C, temp & 0xFF00);
    setFlags(Z, (temp & 0x00FF) == 0x0000);
    setFlags(N, temp & 0x0080);
    if (lookup[opCode].addrmode == &Cpu6502::IMP) {
        a_ = temp & 0x00FF;
    } else {
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}

//Move each of the bits in either A or M one place to the right. Bit 7 is filled with the current value of the carry flag whilst the old bit 0 becomes the new carry flag value.
uint8_t Cpu6502::ROR() { //Rotate right
    fetch();

    uint16_t temp = (uint16_t) (getFlags(C) << 7) | (fetched >> 1);
    setFlags(C, fetched & 0x01);
    setFlags(Z, (temp & 0x00FF) == 0x0000);
    setFlags(N, temp & 0x0080);
    if (lookup[opCode].addrmode == &Cpu6502::IMP) {
        a_ = temp & 0x00FF;
    } else {
        write(addrAbs, temp & 0x00FF);
    }
    return 0;
}

//Sets the program counter to the address specified by the operand
uint8_t Cpu6502::JMP() { //Jump to another location
    pc_ = addrAbs;
    return 0;
}

//The JSR instruction pushes the address (minus one) of the return point on to the stack and then sets the program counter to the target memory address
uint8_t Cpu6502::JSR() { //Jump to a subroutine
    pc_--;

    write(0x0100 + sp_, (pc_ >> 8) & 0x00FF);
    sp_--;
    write(0x0100 + sp_, pc_ & 0x00FF);
    sp_--;

    pc_ = addrAbs;
    return 0;
}

//The RTS instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack
uint8_t Cpu6502::RTS() { //Return from subroutine
    sp_++;
    pc_ = (uint16_t) read(0x0100 + sp_);
    sp_++;
    pc_ |= (uint16_t) read(0x0100 + sp_) << 8;

    pc_++;
    return 0;
}

uint8_t Cpu6502::BCC() {
    if (!(getFlags(C) == 1)) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BCS() {
    if (getFlags(C) == 1) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BEQ() {
    if (getFlags(Z) == 1) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BMI() {
    if (getFlags(N) == 1) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BNE() {
    if (!(getFlags(Z) == 1)) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BPL() {
    if (!(getFlags(N) == 1)) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BVC() {
    if (!(getFlags(V) == 1)) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BVS() {
    if (getFlags(V) == 1) {
        cycles++;
        addrAbs = pc_ + addrRel;
        if ((addrAbs & 0xFF00) != (pc_ & 0xFF00)) {
            cycles++;
        }
        pc_ = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::CLC() {
    setFlags(C, false);
    return 0;
}

uint8_t Cpu6502::CLD() {
    setFlags(D, false);
    return 0;
}

uint8_t Cpu6502::CLI() {
    setFlags(I, false);
    return 0;
}

uint8_t Cpu6502::CLV() {
    setFlags(V, false);
    return 0;
}

uint8_t Cpu6502::SEC() {
    setFlags(C, true);
    return 0;
}

uint8_t Cpu6502::SED() {
    setFlags(D, true);
    return 0;
}

uint8_t Cpu6502::SEI() {
    setFlags(I, true);
    return 0;
}

uint8_t Cpu6502::BRK() {
    pc_++;
    setFlags(I, 1);
    write(0x0100 + sp_, (pc_ >> 8) & 0x00FF);
    sp_--;
    write(0x0100 + sp_, pc_ & 0x00FF);
    sp_--;
    setFlags(B, 1);
    write(0x0100 + sp_, status);
    sp_--;
    setFlags(B, 0);

    pc_ = (uint16_t) read(0xFFFE) | (uint16_t) (read(0xFFFF) << 8);
    return 0;
}

uint8_t Cpu6502::NOP() {
    return 0;
}

uint8_t Cpu6502::RTI() {
    sp_++;

    status = read(0x0100 + sp_);
    status &= ~B;
    status &= ~U;

    sp_++;

    pc_ = (uint16_t) read(0x0100 + sp_);
    sp_++;
    pc_ |= (uint16_t) read(0x0100 + sp_) << 8;

    return 0;
}

uint8_t Cpu6502::XXX() {
    return 0;
}

void Cpu6502::reset() {
    addrAbs = 0xFFFC;

    uint16_t lo = read(addrAbs + 0);
    uint16_t hi = read(addrAbs + 1);

    pc_ = (hi << 8) | lo;
    a_ = 0x00;
    y_ = 0x00;
    x_ = 0x00;
    sp_ = 0xFD;
    status = 0x00 | U;
    addrRel = 0x0000;
    addrAbs = 0x0000;
    fetched = 0x00;

    cycles = 8;
}

void Cpu6502::irq() {
    if (getFlags(I) == 0) {
        write(0x0100 + sp_, (pc_ >> 8) & 0x00FF);
        sp_--;
        write(0x0100 + sp_, pc_ & 0x00FF);
        sp_--;

        setFlags(B, 0);
        setFlags(U, 1);
        setFlags(I, 1);
        write(0x0100 + sp_, status);
        sp_--;

        
    }
}

void Cpu6502::nmi() {

}