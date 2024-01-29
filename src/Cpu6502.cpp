#include "../include/Cpu6502.hpp"
#include "../include/Bus.hpp"
#include <cstdint>
#include <sys/types.h>

Cpu6502::Cpu6502(): bus(nullptr), status(0x00), PC(0x0000), SP(0x00),
    A(0x00), X(0x00), Y(0x00), fetched(0x00), addrAbs(0x0000),
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
        opCode = read(PC);
        PC++;

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

uint8_t Cpu6502::GetFlags(CPUStatusFlags f) {
    return ((status & f) > 0) ? 1 : 0;
}

void Cpu6502::SetFlags(CPUStatusFlags f, bool v) {
    if (v) {
        status |= f;
    }
    else {
        status &= ~f;
    }
}

/* Adressing modes */

uint8_t Cpu6502::IMP() {
	fetched = A;
	return 0;
}

uint8_t Cpu6502::IMM() {
    addrAbs = PC++;
	return 0;
}

uint8_t Cpu6502::ZP0() {
    addrAbs = read(PC++);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::ZPX() {
    addrAbs = (read(PC++) + X);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::ZPY() {
    addrAbs = (read(PC++) + Y);
    addrAbs &= 0x00FF;
    return 0;
}

uint8_t Cpu6502::REL() {
    addrRel = read(PC++);
    if (addrRel & 0x80) {
        addrRel |= 0xFF00;
    }
    return 0;
}

uint8_t Cpu6502::ABS() {
    uint16_t lo = read(PC++);
    uint16_t hi = read(PC++);

    addrAbs = (hi << 8) | lo;

    return 0;
}

uint8_t Cpu6502::ABX() {
    uint16_t lo = read(PC++);
    uint16_t hi = read(PC++);

    addrAbs = (hi << 8) | lo;
    addrAbs += X;

    if ((addrAbs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t Cpu6502::ABY() {
    uint16_t lo = read(PC++);
    uint16_t hi = read(PC++);

    addrAbs = (hi << 8) | lo;
    addrAbs += Y;

    if ((addrAbs & 0xFF00) != (hi << 8))
        return 1;
    else
        return 0;
}

uint8_t Cpu6502::IND() {
    uint16_t ptrLo = read(PC++);
    uint16_t ptrHi = read(PC++);

    uint16_t ptr = (ptrHi << 8) | ptrLo;

    if (ptrLo == 0x00FF) {
        addrAbs = (read(0x00FF) << 8) | read(ptr + 0);
    } else {
        addrAbs = (read(ptr + 1) << 8) | read(ptr + 0);
    }

    return 0;
}

uint8_t Cpu6502::IIX() {
    uint16_t t = read(PC++);

    uint16_t lo = read((uint16_t) (t + (uint16_t)X) & 0x00FF);
    uint16_t hi = read((uint16_t)(t + (uint16_t)X + 1) & 0x00FF);

    addrAbs = (hi << 8) | lo;

    return 0;
}

uint8_t Cpu6502::IIY() {
    uint16_t t = read(PC++);

    uint16_t lo = read(t & 0x00FF);
    uint16_t hi = read((t + 1) & 0x00FF);

    addrAbs = (hi << 8) | lo;
    addrAbs += Y;

    if ((addrAbs & 0x00FF) != (hi << 8))
        return 1;
    else
        return 0;
}

/* Cpu instructions */

uint8_t Cpu6502::LDA() {
    fetch();
    A = fetched;
    SetFlags(Z, A == 0x00);
    SetFlags(N, A & 0x80);
    return 1;
}

uint8_t Cpu6502::LDX() {
    fetch();
    X = fetched;
    SetFlags(Z, X == 0x00);
    SetFlags(N, X & 0x80);
    return 1;
}

uint8_t Cpu6502::LDY() {
    fetch();
    Y = fetched;
    SetFlags(Z, Y == 0x00);
    SetFlags(N, Y & 0x80);
    return 1;
}

uint8_t Cpu6502::STA() {
    write(addrAbs, A);
    return 0;
}

uint8_t Cpu6502::STX() {
    write(addrAbs, X);
    return 0;
}

uint8_t Cpu6502::STY() {
    write(addrAbs, Y);
    return 0;
}

uint8_t Cpu6502::TAX() {
    X = A;
    SetFlags(Z, X == 0x00);
    SetFlags(N, X == 0x80);
    return 0;
}

uint8_t Cpu6502::TAY() {
    Y = A;
    SetFlags(Z, Y == 0x00);
    SetFlags(N, Y == 0x80);
    return 0;
}

uint8_t Cpu6502::TXA() {
    A = X;
    SetFlags(Z, A == 0x00);
    SetFlags(N, A == 0x80);
    return 0;
}

uint8_t Cpu6502::TYA() {
    A = Y;
    SetFlags(Z, A == 0x00);
    SetFlags(N, A == 0x80);
    return 0;
}

uint8_t Cpu6502::TSX() {
    X = SP;
    SetFlags(Z, X == 0x00);
    SetFlags(N, X == 0x80);
    return 0;
}

uint8_t Cpu6502::TXS() { //Don't think that's good
    
}

uint8_t Cpu6502::PHA() {

}

uint8_t Cpu6502::PHP() {
    
}

uint8_t Cpu6502::PLA() {
    
}

uint8_t Cpu6502::PLP() {
    
}

uint8_t Cpu6502::AND() {
    fetch();
    A = A & fetched;
    SetFlags(Z, A == 0x00);
    SetFlags(N, A & 0x80);
    return 1;
}

uint8_t Cpu6502::EOR() {
    
}

uint8_t Cpu6502::ORA() {
    
}

uint8_t Cpu6502::BIT() {
    
}

uint8_t Cpu6502::ADC() {
    
}

uint8_t Cpu6502::SBC() {
    
}

uint8_t Cpu6502::CMP() {
    
}

uint8_t Cpu6502::CPX() {
    
}

uint8_t Cpu6502::CPY() {
    
}

uint8_t Cpu6502::INC() {
    
}

uint8_t Cpu6502::INX() {
    
}

uint8_t Cpu6502::INY() {
    
}

uint8_t Cpu6502::DEC() {
    
}

uint8_t Cpu6502::DEX() {
    
}

uint8_t Cpu6502::DEY() {
    
}

uint8_t Cpu6502::ASL() {
    
}

uint8_t Cpu6502::LSR() {
    
}

uint8_t Cpu6502::ROL() {
    
}

uint8_t Cpu6502::ROR() {
    
}

uint8_t Cpu6502::JMP() {
    
}

uint8_t Cpu6502::JSR() {
    
}

uint8_t Cpu6502::RTS() {
    
}

uint8_t Cpu6502::BCC() {
    if (!(GetFlags(C) == 1)) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BCS() {
    if (GetFlags(C) == 1) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BEQ() {
    if (GetFlags(Z) == 1) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BMI() {
    if (GetFlags(N) == 1) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BNE() {
    if (!(GetFlags(Z) == 1)) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BPL() {
    if (!(GetFlags(N) == 1)) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BVC() {
    if (!(GetFlags(V) == 1)) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::BVS() {
    if (GetFlags(V) == 1) {
        cycles++;
        addrAbs = PC + addrRel;
        if ((addrAbs & 0xFF00) != (PC & 0xFF00)) {
            cycles++;
        }
        PC = addrAbs;
    }
    return 0;
}

uint8_t Cpu6502::CLC() {
    SetFlags(C, false);
    return 0;
}

uint8_t Cpu6502::CLD() {
    SetFlags(D, false);
    return 0;
}

uint8_t Cpu6502::CLI() {
    SetFlags(I, false);
    return 0;
}

uint8_t Cpu6502::CLV() {
    SetFlags(V, false);
    return 0;
}

uint8_t Cpu6502::SEC() {
    SetFlags(C, true);
    return 0;
}

uint8_t Cpu6502::SED() {
    SetFlags(D, true);
    return 0;
}

uint8_t Cpu6502::SEI() {
    SetFlags(I, true);
    return 0;
}

uint8_t Cpu6502::BRK() {
    
}

uint8_t Cpu6502::NOP() {
    
}

uint8_t Cpu6502::RTI() {
    
}

uint8_t Cpu6502::XXX() {
    
}

void Cpu6502::reset() {

}

void Cpu6502::irq() {

}

void Cpu6502::nmi() {

}