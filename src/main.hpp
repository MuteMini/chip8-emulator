#ifndef MAIN_H
#define MAIN_H

#include <memory>

#include "chip8.hpp"
#include "display.hpp"
#include "bus.hpp"

class MainBus : public Bus
{
    private:
        Chip8 *cpu{};
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