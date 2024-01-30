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
        Chip8 *cpu{};
        Keyboard *keyboard{};
        Display *display{};

    public:
        MainBus(Chip8 *cpu, Display* display);
        ~MainBus();

        void notify(Component *component, EventData event);

        void setDisplay(Display *display);

        Chip8& getCPU();
        Display& getDisplay();
};

#endif