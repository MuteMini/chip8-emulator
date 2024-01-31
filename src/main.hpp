/*
    Author: Min Kang
    Creation Date: January 17th, 2024

    Declares design of bus for main program.
*/

#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include "chip8.hpp"
#include "keyboard.hpp"
#include "display.hpp"
#include "bus.hpp"

class MainBus : public Bus
{
    private:
        Chip8 cpu;
        Keyboard keyboard;
        Display display;

    public:
        MainBus(SDL_Texture *texture);

        void notify(EventData event);

        Chip8& getCPU();
        Keyboard& getKeyboard();
        Display& getDisplay();
};

#endif