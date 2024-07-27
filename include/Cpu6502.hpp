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

#ifndef CPU6502_HPP
# define CPU6502_HPP

# include "Instruction.hpp"
# include <cstdint>
# include <map>
# include <vector>

class Bus;
class Cpu6502 {
public:
    Cpu6502();
    ~Cpu6502();

    void connectBus(Bus *bus);

    /*processor status flags*/
    enum CPUStatusFlags {
        C = (1 << 0), //Carry flag
        Z = (1 << 1), //Zero flag
        I = (1 << 2), //interrupt disable
        D = (1 << 3), //Decimal mode
        B = (1 << 4), //Break command
        U = (1 << 5), //Unused
        V = (1 << 6), //Overflow flag
        N = (1 << 7), //Negative
    };

    /*processor addressing modes*/
    uint8_t IMP(); //implied
    uint8_t IMM(); //immediate
    uint8_t ZP0(); //zero page 
    uint8_t ZPX(); //zero page x
    uint8_t ZPY(); //zero page y
    uint8_t REL(); //relative
    uint8_t ABS(); //absolute
    uint8_t ABX(); //absolute x
    uint8_t ABY(); //absolute y
    uint8_t IND(); //indirect
    uint8_t IIX(); //indexed indirect x
    uint8_t IIY(); //indexed indirect y

    /*processor instructions*/

    /*load/store operations*/
    uint8_t LDA(); //load accumulator
    uint8_t LDX(); //load x register
    uint8_t LDY(); //load y register
    uint8_t STA(); //store accumulator
    uint8_t STX(); //store x register
    uint8_t STY(); //store y register
    
    /*register transfers*/
    uint8_t TAX(); //transfer accumulator to x
    uint8_t TAY(); //transfer accumulator to y
    uint8_t TXA(); //transfer x to accumulator
    uint8_t TYA(); //transfer y to accumulator

    /*stack operations*/
    uint8_t TSX(); //transfer stack pointer to x
    uint8_t TXS(); //transfer x to stack pointer
    uint8_t PHA(); //push accumulator on stack
    uint8_t PHP(); //push processor status on stack
    uint8_t PLA(); //pull accumulator from stack
    uint8_t PLP(); //pull processor status from stack

    /*logical*/
    uint8_t AND(); //logical and
    uint8_t EOR(); //exclusive or
    uint8_t ORA(); //logical inclusive or
    uint8_t BIT(); //bit test

    /*arithmetic*/
    uint8_t ADC(); //add with carry
    uint8_t SBC(); //subtract with carry
    uint8_t CMP(); //compare accumulator
    uint8_t CPX(); //compare x register
    uint8_t CPY(); //compare y register

    /*increments & decrements*/
    uint8_t INC(); //increment a memory location
    uint8_t INX(); //increment the x register
    uint8_t INY(); //increment the y register
    uint8_t DEC(); //decrement a memory location
    uint8_t DEX(); //decrement the x register
    uint8_t DEY(); //decrement the y register

    /*shifts*/
    uint8_t ASL(); //arithmetic shift left
    uint8_t LSR(); //logical shift right
    uint8_t ROL(); //rotate left
    uint8_t ROR(); //rotate right

    /*jumps & calls*/
    uint8_t JMP(); //jump
    uint8_t JSR(); //jump to subroutine
    uint8_t RTS(); //return from subroutine

    /*branches*/
    uint8_t BCC(); //branch if carry clear
    uint8_t BCS(); //branch if carry set
    uint8_t BEQ(); //branch if equal
    uint8_t BMI(); //branch if minus
    uint8_t BNE(); //branch if zero flag clear
    uint8_t BPL(); //branch if negative flag clear
    uint8_t BVC(); //branch if overflow flag clear
    uint8_t BVS(); //branch if overflow flag set

    /*status flag changes*/
    uint8_t CLC(); //clear carry flag
    uint8_t CLD(); //clear decimal mode flag
    uint8_t CLI(); //clear interrupt disable flag
    uint8_t CLV(); //clear overflow flag
    uint8_t SEC(); //set carry flag
    uint8_t SED(); //set decimal mode flag
    uint8_t SEI(); //set interrupt disable flag

    /*system functions*/
    uint8_t BRK(); //force an interrupt
    uint8_t NOP(); //no operation
    uint8_t RTI(); //return from interrupt

    /*catchall for illegal opcodes*/
    uint8_t XXX();

    void clock();
    void reset();
    void irq();
    void nmi();
    std::map<uint16_t, std::string> disassemble(uint16_t start, uint16_t stop);

    uint8_t fetch();
    uint8_t fetched_;

    uint16_t addrAbs_;
    uint16_t addrRel_;
    uint8_t opCode_;

    int cycles_;

private:
    Bus         *bus_;

    uint8_t     status_; //processor status
    uint16_t    pc_; //program counter
    uint8_t     sp_; //stack pointer
    uint8_t     a_; //accumulator
    uint8_t     x_; //index register x
    uint8_t     y_; //index register y

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    uint8_t getFlags(CPUStatusFlags f);
    void setFlags(CPUStatusFlags f, bool v);

    std::vector<INSTRUCTION> lookup_;
};

#endif