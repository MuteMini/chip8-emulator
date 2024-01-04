#include <iostream>
#include <SDL2/SDL.h>
#include "chip8.hpp"

int main( int argc, char* argv[] ) {
    SDL_Init( SDL_INIT_EVERYTHING );

    const int WIDTH{800}, HEIGHT{600};

    SDL_Window *window = SDL_CreateWindow("Chip-8 Emulator", 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        SDL_WINDOWPOS_UNDEFINED, 
                                        WIDTH, 
                                        HEIGHT, 
                                        SDL_WINDOW_ALLOW_HIGHDPI);

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