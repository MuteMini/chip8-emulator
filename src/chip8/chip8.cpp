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

#include "chip8.hpp"

Chip8::Chip8() {};

Chip8::~Chip8() {
    delete[] memory;
    delete[] display;
};

void Chip8::reset() {
    index_reg = 0;
    pc = MEM_ADDR_START;
    sp = 0;
    delay = 0;
    sound = 0;

    delete[] memory;
    delete[] display;

    std::fill(std::begin( reg ), std::end( reg ), 0);
    std::fill(std::begin( stack ), std::end( stack ), 0);
    
    memory = new uint8_t[4096]{};
    display = new uint32_t[WIDTH*HEIGHT]{};
}

// Reading file functionality comes from https://coniferproductions.com/posts/2022/10/25/reading-binary-files-cpp/
bool Chip8::loadProgram(std::string file) {
    std::filesystem::path input{std::filesystem::current_path()};
    input += std::filesystem::u8path(file);

    std::ifstream is{input, std::ios_base::in | std::ios_base::binary};

    if(!is.good()) return false;

    is.seekg(0, is.end);
    int size{ static_cast<int>(is.tellg()) };

    is.seekg(0, is.beg);

    if(size > MEM_ADDR_END - MEM_ADDR_START) {
        is.close();
        return false;
    }

    is.read(reinterpret_cast<char *>(memory+MEM_ADDR_START), size);

    pc = MEM_ADDR_START;

    is.close();
    return true;
};

void Chip8::tick() {
    // Fetching instruction
    uint16_t instruction{ ((memory[pc]) << 8) + memory[pc+1] };
    pc += 2;

    // Decoding instruction

    // Potential address bits
    uint16_t address_3B{ instruction & 0x0FFF };
    uint16_t address_2B{ instruction & 0x00FF };
    uint16_t address_1B{ instruction & 0x000F };

    uint16_t reg_X{ (instruction & 0x0F00) >> 8 };
    uint16_t reg_Y{ (instruction & 0x00F0) >> 4 };

    // Executing instruction
    switch( instruction & 0xF000 ) {
        case 0x0:
            if( instruction == 0x00E0 ) {
                std::fill(display, display + WIDTH*HEIGHT, 0);
            }
            break;
        case 0x1:
            pc = address_3B;
            break;
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
            reg[reg_X] = address_2B;
        case 0x7:
            reg[reg_X] += address_2B;
        case 0x8:
        case 0x9:
        case 0xA:
            index_reg = address_3B;
        case 0xB:
        case 0xC:
        case 0xD:
            this->drawBytes( static_cast<uint16_t>(reg[reg_X]) + static_cast<uint16_t>(reg[reg_Y])*WIDTH, address_1B );
        case 0xE:
        case 0xF:
    }
};
