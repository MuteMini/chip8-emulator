/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Defines the behaviours of the Chip8 system. Works as a high level abstraction.
    Instr. behavior were written with the help of: https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference#graphics
*/

#include "chip8.hpp"

Chip8::Chip8() {};

Chip8::~Chip8() {
    delete[] memory;
    delete[] display;
};
