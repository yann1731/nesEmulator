#pragma once
#include "header.hpp"
#include "../include/Instruction.hpp"

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

    uint8_t fetch();
    uint8_t fetched;

    uint16_t addrAbs;
    uint16_t addrRel;
    uint8_t opCode;

    int cycles;

private:
    Bus         *bus;

    uint8_t     status; //processor status
    uint16_t    PC; //program counter
    uint8_t     SP; //stack pointer
    uint8_t     A; //accumulator
    uint8_t     X; //index register x
    uint8_t     Y; //index register y

    uint8_t read(uint16_t addr);
    void write(uint16_t addr, uint8_t data);

    uint8_t GetFlags(CPUStatusFlags f);
    void SetFlags(CPUStatusFlags f, bool v);

    std::vector<INSTRUCTION> lookup;
};