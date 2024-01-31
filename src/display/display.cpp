/*
    Author: Min Kang
    Creation Date: January 14th, 2024

    Defines behaviors of the game display. Abstracts calls to renderer
    and texture to write pixel data.
*/

#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>

#include <list>

#include "display.hpp"
#include "logger.hpp"

Display::Display(Logger logger, Bus& bus, SDL_Texture* texture, uint32_t off_pixel, uint32_t on_pixel) : 
    Component(logger, bus),
    texture(texture),
    off_pixel(off_pixel),
    on_pixel(on_pixel)
{};

Display::~Display()
{
    SDL_DestroyTexture(texture);
    delete[] buffer;
}

bool Display::drawPixelData(uint16_t x_pos, uint16_t y_pos, uint8_t data[], std::size_t size)
{
    x_pos %= WIDTH;
    y_pos %= HEIGHT;

    bool set_to_unset{false};
    for( std::size_t col{0}; col < HEIGHT && col < size; ++col )
    {
        uint8_t mask{ 0x80 };
        for( std::size_t row{0}; row < WIDTH && row < 8; ++row )
        {
            const std::size_t index{ (col + y_pos)*WIDTH + x_pos + row };
            if( mask & data[col] )
            {
                set_to_unset    = (buffer[index] == on_pixel) ? true      : set_to_unset;
                buffer[index]   = (buffer[index] == on_pixel) ? off_pixel : on_pixel;
            }
            mask >>= 1;
        }
    }
    return set_to_unset;
};

void Display::clearScreen()
{
    std::fill(buffer, buffer + WIDTH*HEIGHT, off_pixel);
}

void Display::updateScreen(SDL_Renderer* renderer)
{
    SDL_UpdateTexture( texture, NULL, buffer, WIDTH*sizeof(uint32_t) );

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
};