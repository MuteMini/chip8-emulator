/*
    Author: Min Kang
    Creation Date: January 11th, 2024

    Declares behaviors of the game display. Acts as a abstraction to the 
    SDL renderer functions.
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL_render.h>

#include "../header.hpp"

class Display 
{
    private:
        SDL_Renderer* renderer{nullptr};
        SDL_Texture* texture{nullptr};

    public:
        Display(SDL_Renderer* renderer);

        void drawPixelData(uint32_t pixels[WIDTH*HEIGHT]);
};

#endif