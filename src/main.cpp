/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Entry point of the SDL executable. Sets up the event loop for the Chip8 interpreter.
*/

#include <iostream>
#include <SDL.h>
#include "chip8.hpp"

const int WIDTH{800}, HEIGHT{600};

int main( int argc, char* argv[] ) {

    SDL_Init( SDL_INIT_EVERYTHING );

    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator", 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        WIDTH, 
                                        HEIGHT, 
                                        SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(renderer, 200, 300);

    SDL_RenderPresent(renderer);

    if( window == nullptr )
    {
        std::cout << "Could not create the window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Event windowEvent;

    while(true)
    {
        if( SDL_PollEvent( &windowEvent ) ) 
        {
            if( windowEvent.type == SDL_QUIT ) break;
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}