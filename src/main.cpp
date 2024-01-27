/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Entry point of the SDL executable. Sets up the event loop for the Chip8 interpreter.
*/

#include <SDL2/SDL.h>

#include <iostream>
#include <random>

#include "header.hpp"
#include "main.hpp"

// Code from https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
std::random_device rand_dev{};
std::mt19937 generator{rand_dev()};
std::uniform_int_distribution<uint8_t> distr(0x00, 0xFF);

uint8_t generateRandom() { return distr(generator); }

MainBus::MainBus(Chip8 *cpu, Display* display) :
    cpu(cpu),
    display(display)
{
    this->cpu->linkBus(this);
    this->display->linkBus(this);
};

MainBus::~MainBus()
{
    delete cpu;
    delete display;
};

Chip8& MainBus::getCPU()        { return *cpu; };
Display& MainBus::getDisplay()  { return *display; };

void MainBus::notify(Component *component, EventData event)
{
    if( component == cpu )
    {
        if( event.type == EventType::DISPLAY_CLEAR )
        {
            display->clearScreen();
        }
        else if( event.type == EventType::DISPLAY_DRAW )
        {
            cpu->setStatusReg(
                display->drawPixelData(
                    event.draw.xpos, 
                    event.draw.ypos, 
                    event.draw.data, 
                    event.draw.size
                )
            );
        }
        else if( event.type == EventType::RANDOM )
        {
            *event.random.dest = event.random.mask & generateRandom();
        }
    }
    else if( component == display )
    {

    }
}

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
    SDL_Texture *texture = SDL_CreateTexture(renderer, 
                                            SDL_PIXELFORMAT_ARGB8888, 
                                            SDL_TEXTUREACCESS_STREAMING, 
                                            WIDTH,
                                            HEIGHT);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if( window == nullptr )
    {
        std::cout << "Could not create the window: " << SDL_GetError() << std::endl;
        return 1;
    }

    MainBus main_bus{new Chip8{}, new Display{texture, 0x00000000, 0xFFFFFFFF}};

    main_bus.getCPU().loadProgram("\\test\\_data\\IBMLogo.ch8");

    // Game Loop, idea from https://stackoverflow.com/questions/26664139/sdl-keydown-and-key-recognition-not-working-properly
    SDL_Event event;
    bool run_loop{true};
    while(run_loop)
    {        
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_KEYDOWN ) 
            {
                main_bus.getCPU().execute( main_bus.getCPU().fetch() );
                main_bus.getDisplay().updateScreen(renderer);
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