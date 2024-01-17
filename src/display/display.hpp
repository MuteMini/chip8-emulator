/*
    Author: Min Kang
    Creation Date: January 11th, 2024

    Declares behaviors of the game display. Acts as a abstraction to the 
    SDL renderer functions.
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>

#include "header.hpp"

class Display 
{
    private:
        const uint32_t OFF_PIXEL;
        const uint32_t ON_PIXEL;

        uint32_t *buffer{ new uint32_t[WIDTH*HEIGHT]{} };

        SDL_Renderer* renderer{};
        SDL_Texture* texture{};

    public:
        Display(SDL_Renderer* renderer, uint32_t off_pixel, uint32_t on_pixel);
        ~Display();

        bool drawPixelData(uint16_t x_pos, uint16_t y_pos, uint8_t data_byte);

        void clearScreen();
        void updateScreen();
};

#endif