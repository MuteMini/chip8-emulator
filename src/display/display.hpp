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
#include "logger.hpp"
#include "bus.hpp"

class Display : public Component
{
    private:
        uint32_t off_pixel{};
        uint32_t on_pixel{};

        uint32_t *buffer{ new uint32_t[WIDTH*HEIGHT]{} };

        SDL_Texture* texture{};

    public:
        Display(Bus& bus, SDL_Texture* texture, uint32_t off_pixel, uint32_t on_pixel);
        ~Display();

        bool drawPixelData(uint16_t x_pos, uint16_t y_pos, uint8_t data[], std::size_t size);

        void clearScreen();
        void updateScreen(SDL_Renderer* renderer);
};

#endif