/*
    Author: Min Kang
    Creation Date: January 11th, 2024

    Declares behaviors of the game display. Acts as a abstraction to the 
    SDL renderer functions.
*/

#include<SDL2/SDL_render.h>

class Display {
    private:
        SDL_Renderer* renderer{};

    public:
        Display();

        
};