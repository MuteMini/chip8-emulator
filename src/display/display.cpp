/*
    Author: Min Kang
    Creation Date: January 14th, 2024

    Declares behaviors of the game display. Acts as a abstraction to the 
    SDL renderer functions.
*/

#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>

#include "display.hpp"

Display::Display(SDL_Renderer* renderer) : 
    renderer(renderer) 
{
    
};

void Display::drawPixelData(uint32_t pixels[WIDTH*HEIGHT]) {

};