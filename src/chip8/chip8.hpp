/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Declares the behaviours of the Chip8 system.
    Structure/variable types were made with the help of this blog: https://austinmorlan.com/posts/chip8_emulator/ 
*/
#ifndef CHIP8_H
#define CHIP8_H

#define MEM_ADDR_START 0x200
#define MEM_ADDR_END 0xE8F

#define ADDR_SPRITE 0x000

#include <cstdint>
#include <fstream>
#include <string>

#include "../header.hpp"


class InstructionFailed;

class Chip8 {
    private:
        uint8_t reg[16]{};
        uint16_t index_reg{};

        uint8_t *memory{ new uint8_t[4096]{} };
        uint16_t pc{};

        uint16_t stack[16]{};
        uint8_t sp{};

        uint8_t delay{};
        uint8_t sound{};

        bool loadData(uint16_t addr, uint8_t data[], int size);

        bool drawBytes(uint16_t addr, uint8_t bits);

    public:
        Chip8();
        ~Chip8();

        void reset();

        bool loadProgram(std::string file);

        void tick(Display &display);
};

#endif