#include <cstdint>

class Chip8 {
    private:
        uint8_t reg[16]{};
        uint8_t memory[4096]{};
        uint16_t index_reg{};
        uint16_t pc{};

        uint16_t stack[16]{};
        uint8_t sp{};

        uint8_t delay{};
        uint8_t sound{};

        uint8_t display[64*32]{};

    public:
        Chip8();

        bool loadProgram();

        bool tick();
};