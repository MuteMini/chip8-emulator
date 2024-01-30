/*
    Author: Min Kang
    Creation Date: January 29th, 2022

    Declares wrapper for SDL keyboard input event handling.
    Passes events onto Chip8 through the bus.
*/

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL_keyboard.h>

#include <cstdint>

#include "header.hpp"
#include "bus.hpp"

class Keyboard : public Component {
    private:
        uint8_t key;

    public:
        Keyboard();

        uint8_t getKey();

        void storeKey(SDL_Scancode scancode);
};

#endif