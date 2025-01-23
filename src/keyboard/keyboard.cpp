/*
    Author: Min Kang
    Creation Date: January 29th, 2022

    Defines the SDL keyboard input wrapper class
    Passes events onto Chip8 through the bus.
*/

#include <iostream>

#include "header.hpp"
#include "logger.hpp"
#include "keyboard.hpp"

// Index represents button #.
const SDL_Scancode KEYBOARD_MAP[16] {
    SDL_SCANCODE_X,
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_C,
    SDL_SCANCODE_4,
    SDL_SCANCODE_R,
    SDL_SCANCODE_F,
    SDL_SCANCODE_V,
};

Keyboard::Keyboard(Bus& bus) :
    Component("keyboard_log.txt", bus),
    key(KEY_NOTPRESSED)
{};

uint8_t Keyboard::getKey() {
    return key;
};

void Keyboard::storeKey(SDL_Scancode scancode)  
{
    logger << scancode << std::endl;

    for(uint8_t i{0}; i < 16; i++)
    {
        if(KEYBOARD_MAP[i] == scancode)
        {
            key = i;
            return;
        }
    }
    key = KEY_NOTPRESSED;
};