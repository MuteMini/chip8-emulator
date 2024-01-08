/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Declares the behaviours of the Chip8 system.
    Structure/variable types were made with the help of this blog: https://austinmorlan.com/posts/chip8_emulator/ 
*/

#include <cstdint>

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

        uint8_t *display{ new uint8_t[64*32]{} };

    public:
        Chip8();
        ~Chip8();

        bool loadProgram(uint8_t data);

        bool tick();

        
};