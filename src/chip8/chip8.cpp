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

#include "header.hpp"
#include "chip8.hpp"

Chip8::Chip8()
{
    this->reset();
};

Chip8::~Chip8() 
{
    delete[] memory;
};

void Chip8::setStatusReg(bool status)
{
    reg[0xF] = status ? 0x01 : 0x00;
}

bool Chip8::loadData(uint16_t addr, uint8_t data[], int size) 
{
    if(addr > MEM_ADDR_END - size) return false;

    std::copy(data, data+size, memory+addr);
    return true;
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

uint16_t Chip8::fetch()
{
    if(pc >= MEM_ADDR_END)
    {
        pc = MEM_ADDR_START;
    }

    return static_cast<uint16_t>((memory[pc] << 8) + memory[pc+1]);
};

void Chip8::execute(uint16_t opcode)
{
    uint16_t address_3B{ static_cast<uint16_t>(opcode & 0x0FFF) };
    uint16_t address_2B{ static_cast<uint16_t>(opcode & 0x00FF) };
    uint16_t address_1B{ static_cast<uint16_t>(opcode & 0x000F) };

    uint16_t reg_X{ static_cast<uint16_t>((opcode & 0x0F00) >> 8) };
    uint16_t reg_Y{ static_cast<uint16_t>((opcode & 0x00F0) >> 4) };

    pc += 2;
    switch( (opcode & 0xF000) >> 12 )
    {
        case 0x0:
            if( opcode == 0x00E0 )
            {
                bus->notify(this, { .type = EventType::DISPLAY_CLEAR });
            }
            else if( opcode == 0x00EE )
            {
                sp -= (sp > 0);
                pc = stack[sp];
            }
            break;
        case 0x1:
            pc = address_3B;
            break;
        case 0x2:
            stack[sp] = pc - 2;
            sp += (sp < 15);
            pc = address_3B;
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
                    setStatusReg((reg[reg_Y] & 0x01) != 0);
                    reg[reg_X] = reg[reg_Y] >> 1;
                    break;
                case 0x7:
                    setStatusReg(reg[reg_X] > reg[reg_Y]);
                    reg[reg_X] = reg[reg_Y] - reg[reg_X];
                    break;
                case 0xE:
                    setStatusReg((reg[reg_Y] & 0x80) != 0);
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
            bus->notify(this, {
                .type = EventType::RANDOM,
                .random = {
                    .mask = static_cast<uint8_t>(address_2B),
                    .dest = &reg[reg_X]
                }
            });
            break;
        case 0xD:
            bus->notify(this, { 
                .type = EventType::DISPLAY_DRAW,
                .draw = {
                    .xpos = reg[reg_X],
                    .ypos = reg[reg_Y],
                    .data = memory + index_reg,
                    .size = address_1B
                }
            });
            break;
        case 0xE:
        {
            uint8_t key{0x10};
            bus->notify(this, {
                .type = EventType::KEYBOARD_GET,
                .key = &key,
            });

            pc += ((address_2B == 0x9E && (reg[reg_X] & 0x0F) == key) 
                || (address_2B == 0xA1 && (reg[reg_X] & 0x0F) != key)) ? 2 : 0;
            break;
        }
        case 0xF:
            switch(address_2B)
            {
                case 0x07:
                    reg[reg_X] = delay;
                    break;
                case 0x0A:
                    bus->notify(this, { 
                        .type = EventType::KEYBOARD_GET,
                        .key = &reg[reg_X]
                    });
                    pc -= (reg[reg_X] == KEY_NOTPRESSED) ? 2 : 0;
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
                    index_reg = ADDR_SPRITE + reg[reg_X]*5;
                    break;
                case 0x33:
                {   
                    uint16_t bcd{reg[reg_X]};
                    for(std::size_t i{2}; i >= 0; ++i ) 
                    {
                        memory[index_reg + i] = bcd % 10;
                        bcd /= 10;
                    }
                    break;
                }
                case 0x55:
                    for(std::size_t i{0}; i <= reg_X; ++i)
                    {
                        memory[index_reg + i] = reg[i];
                    }
                    break;
                case 0x65:
                    for(std::size_t i{0}; i <= reg_X; ++i)
                    {
                        reg[i] = memory[index_reg + i];
                    }
                    break;
            }
            break;
    }
};