/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Defines the behaviours of the Chip8 system. Works as a high level abstraction.
    Instr. behavior were written with the help of: https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference
    and https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set
*/

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <ios>

#include "chip8.hpp"

Chip8::Chip8()
{
    
    this->reset();
};

Chip8::~Chip8() 
{
    delete[] memory;
};

bool Chip8::loadData(uint16_t addr, uint8_t data[], int size) 
{
    if(addr > MEM_ADDR_END - size) return false;

    std::copy(data, data+size, memory+addr);
    return true;
};

void Chip8::setStatusReg(bool status)
{
    reg[0xF] = status ? 0x01 : 0x00;
}

void Chip8::reset()
{
    index_reg = 0;
    pc = MEM_ADDR_START;
    sp = 0;
    delay = 0;
    sound = 0;

    delete[] memory;

    std::fill(std::begin( reg ), std::end( reg ), 0);
    std::fill(std::begin( stack ), std::end( stack ), 0);
    
    memory = new uint8_t[4096]{};

    uint8_t sprite_data[16*5]{0xF0, 0x90, 0x90, 0x90, 0xF0,
                            0x20, 0x60, 0x20, 0x20, 0x70,
                            0xF0, 0x10, 0xF0, 0x80, 0xF0,
                            0xF0, 0x10, 0xF0, 0x10, 0xF0,
                            0x90, 0x90, 0xF0, 0x10, 0x10,
                            0xF0, 0x80, 0xF0, 0x10, 0xF0,
                            0xF0, 0x80, 0xF0, 0x90, 0xF0,
                            0xF0, 0x10, 0x20, 0x40, 0x40,
                            0xF0, 0x90, 0xF0, 0x90, 0xF0,
                            0xF0, 0x90, 0xF0, 0x10, 0xF0,
                            0xF0, 0x90, 0xF0, 0x90, 0x90,
                            0xE0, 0x90, 0xE0, 0x90, 0xE0,
                            0xF0, 0x80, 0x80, 0x80, 0xF0,
                            0xE0, 0x90, 0x90, 0x90, 0xE0,
                            0xF0, 0x80, 0xF0, 0x80, 0xF0,
                            0xF0, 0x80, 0xF0, 0x80, 0x80};
    this->loadData(ADDR_SPRITE, &(sprite_data[0]), 16*5);
};

// Reading file functionality comes from https://coniferproductions.com/posts/2022/10/25/reading-binary-files-cpp/
bool Chip8::loadProgram(std::string file)
{
    std::filesystem::path input{std::filesystem::current_path()};
    input += std::filesystem::u8path(file);

    std::ifstream is{input, std::ios_base::in | std::ios_base::binary};

    if(!is.good()) return false;

    is.seekg(0, is.end);
    int size{ static_cast<int>(is.tellg()) };

    is.seekg(0, is.beg);

    if(size > MEM_ADDR_END - MEM_ADDR_START)
    {
        is.close();
        return false;
    }

    is.read(reinterpret_cast<char *>(memory+MEM_ADDR_START), size);

    pc = MEM_ADDR_START;

    is.close();
    return true;
};

void Chip8::tick()
{
    // Fetching instruction
    if(pc >= MEM_ADDR_END)
    {
        pc = MEM_ADDR_START;
    }

    uint16_t instruction{ static_cast<uint16_t>((memory[pc] << 8) + memory[pc+1]) };

    std::cout << std::hex <<  pc << ": " << instruction << std::endl;

    pc += 2;

    // Decoding instruction

    // Potential address bits
    uint16_t address_3B{ static_cast<uint16_t>(instruction & 0x0FFF) };
    uint16_t address_2B{ static_cast<uint16_t>(instruction & 0x00FF) };
    uint16_t address_1B{ static_cast<uint16_t>(instruction & 0x000F) };

    uint16_t reg_X{ static_cast<uint16_t>((instruction & 0x0F00) >> 8) };
    uint16_t reg_Y{ static_cast<uint16_t>((instruction & 0x00F0) >> 4) };

    // Executing instruction
    switch( (instruction & 0xF000) >> 12 )
    {
        case 0x0:
            if( instruction == 0x00E0 )
            {
                bus->notify(this, { .type = EventType::DISPLAY_CLEAR });
            }
            break;
        case 0x1:
            pc = address_3B;
            break;
        case 0x2:
            // Add subroutine maddness
            break;
        case 0x3:
            pc += (reg[reg_X] == address_2B) ? 2 : 0;
            break;
        case 0x4:
            pc += (reg[reg_X] != address_2B) ? 2 : 0;
            break;
        case 0x5:
            pc += (reg[reg_X] == reg[reg_Y]) ? 2 : 0;
            break;
        case 0x6:
            reg[reg_X] = address_2B;
            break;
        case 0x7:
            reg[reg_X] += address_2B;
            break;
        case 0x8:
            switch(address_1B)
            {
                case 0x0:
                    reg[reg_X] = reg[reg_Y];
                    break;
                case 0x1:
                    reg[reg_X] |= reg[reg_Y];
                    break;
                case 0x2:
                    reg[reg_X] &= reg[reg_Y];
                    break;
                case 0x3:
                    reg[reg_X] ^= reg[reg_Y];
                    break;
                case 0x4:
                    setStatusReg(0xFF - reg[reg_X] < reg[reg_Y]);
                    reg[reg_X] += reg[reg_Y];
                    break;
                case 0x5:
                    setStatusReg(reg[reg_X] < reg[reg_Y]);
                    reg[reg_X] -= reg[reg_Y];
                    break;
                case 0x6:
                    setStatusReg(reg[reg_Y] & 0x01 != 0);
                    reg[reg_X] = reg[reg_Y] >> 1;
                    break;
                case 0x7:
                    setStatusReg(reg[reg_X] > reg[reg_Y]);
                    reg[reg_X] = reg[reg_Y] - reg[reg_X];
                    break;
                case 0xE:
                    setStatusReg(reg[reg_Y] & 0x80 != 0);
                    reg[reg_X] = reg[reg_Y] << 1;
                    break;
            }
            break;
        case 0x9:
            pc += (reg[reg_X] != reg[reg_Y]) ? 2 : 0;
            break;
        case 0xA:
            index_reg = address_3B;
            break;
        case 0xB:
            pc = address_3B + reg[0];
            break;
        case 0xC:
            // Random number generator
            break;
        case 0xD:
            bus->notify(this, 
                { 
                .type = EventType::DISPLAY_DRAW,
                .draw = {
                    .xpos = reg[reg_X],
                    .ypos = reg[reg_Y],
                    .data = memory + index_reg,
                    .size = address_1B
                    }
                }
            );
            break;
        case 0xE:
            if(address_2B == 0x9E)
            {

            }
            else if(address_2B == 0xA1)
            {

            }
            break;
        case 0xF:
            switch(address_2B)
            {
                case 0x07:
                    reg[reg_X] = delay;
                    break;
                case 0x0A:
                    // Waiting for key press input
                    break;
                case 0x15:
                    delay = reg[reg_X];
                    break;
                case 0x18:
                    sound = reg[reg_X];
                    break;
                case 0x1E:
                    index_reg += reg[reg_X]; 
                    break;
                case 0x29:
                    // Getting pre-installed sprite data
                    break;
                case 0x33:
                    // BCD in [I, I+2]
                    break;
                case 0x55:
                    // Mem to reg
                    break;
                case 0x65:
                    // reg to mem
                    break;
            }
            break;
    }
};
