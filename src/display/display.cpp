/*
    Author: Min Kang
    Creation Date: January 14th, 2024

    Defines behaviors of the game display. Abstracts calls to renderer
    and texture to write pixel data.
*/

#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>

#include <iostream>

#include "display.hpp"

Display::Display(SDL_Renderer* renderer, uint32_t off_pixel, uint32_t on_pixel) :
    renderer(renderer),
    texture(SDL_CreateTexture(
            renderer, 
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            WIDTH,
            HEIGHT
    )),
    OFF_PIXEL(off_pixel),
    ON_PIXEL(on_pixel)
{};

Display::~Display()
{
    SDL_DestroyTexture(texture);
    delete[] buffer;
}

bool Display::drawPixelData(uint16_t x_pos, uint16_t y_pos, uint8_t data_byte)
{   
    x_pos %= WIDTH;
    y_pos %= HEIGHT;

    uint8_t mask{ 0x80 };
    bool set_to_unset{false};

    std::cout << x_pos << "," << y_pos << std::endl;

    for( std::size_t i{x_pos}; i < WIDTH && i < x_pos + 8; ++i )
    {
        const std::size_t index{ y_pos*WIDTH + i };
        if( mask & data_byte ) 
        {
            if( buffer[index] == ON_PIXEL ) {
                buffer[index] = OFF_PIXEL;
                set_to_unset = true;
            }
            else {
                buffer[index] = ON_PIXEL;
            }
        }
        mask >>= 1;
    } 
    return true;
};

void Display::clearScreen()
{
    std::fill(buffer, buffer + WIDTH*HEIGHT, OFF_PIXEL);
}

void Display::updateScreen()
{
    SDL_UpdateTexture( texture, NULL, buffer, WIDTH*sizeof(uint32_t) );

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
};