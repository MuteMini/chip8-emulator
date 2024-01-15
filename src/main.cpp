/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Entry point of the SDL executable. Sets up the event loop for the Chip8 interpreter.
*/

#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.hpp"
#include "display.hpp"

int main( int argc, char* argv[] )
{
    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator", 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        WIDTH*SCALE, 
                                        HEIGHT*SCALE, 
                                        SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if( window == nullptr )
    {
        std::cout << "Could not create the window: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize all components of the Chip-8 System
    Chip8 cpu{};
    Display display{renderer, 0x00000000, 0xFFFFFFFF};

    cpu.loadProgram("\\test\\_data\\IBMLogo.ch8");

    // Game Loop, idea from https://stackoverflow.com/questions/26664139/sdl-keydown-and-key-recognition-not-working-properly
    SDL_Event event;
    bool run_loop{true};

    while(run_loop)
    {        
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_KEYDOWN ) 
            {
                cpu.tick(display);
                display.updateScreen();
            } 
            else if( event.type == SDL_QUIT ) 
            {
                run_loop = false;
            }
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}