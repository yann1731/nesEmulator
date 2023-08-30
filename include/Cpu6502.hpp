#pragma once
#include "header.hpp"

class Bus;
class Cpu6502 {
public:
    Cpu6502();
    ~Cpu6502();

    void connectBus(Bus *bus);

    /*processor status flags*/
    enum PS {
        C = (1 << 0), //Carry flag
        Z = (1 << 1), //Zero flag
        I = (1 << 2), //interrupt disable
        D = (1 << 3), //Decimal mode
        B = (1 << 4), //Break command
        U = (1 << 5), //Unused
        O = (1 << 6), //Overflow flag
        N = (1 << 7), //Negative
    };

    /*processor addressing modes*/
    uint8_t IMP(); //implied
    uint8_t ACC(); //accumulator
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

private:
    Bus         *bus;

    uint8_t     status; //processor status
    uint16_t    PC; //program counter
    uint8_t     SP; //stack pointer
    uint8_t     A; //accumulator
    uint8_t     X; //index register x
    uint8_t     Y; //index register y

    void read(uint16_t addr);
    uint8_t write(uint16_t addr, uint8_t data);
};