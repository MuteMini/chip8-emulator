/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Declares the behaviours of the Chip8 system.
    Structure/variable types were made with the help of this blog: https://austinmorlan.com/posts/chip8_emulator/ 
*/

#include <cstdint>
#include <fstream>
#include <string>

#define MEM_ADDR_START 0x200
#define MEM_ADDR_END 0xE8F

#define WIDTH 64
#define HEIGHT 32

class Chip8 {
    private:
        uint8_t reg[16]{};
        uint8_t *memory{ new uint8_t[4096]{} };
        uint16_t index_reg{};
        uint16_t pc{};

        uint16_t stack[16]{};
        uint8_t sp{};

        uint8_t delay{};
        uint8_t sound{};

        uint8_t *display{ new uint8_t[WIDTH*HEIGHT]{} };

    public:
        Chip8();
        ~Chip8();

        void reset();

        bool loadProgram(std::string file);

        bool tick();
};