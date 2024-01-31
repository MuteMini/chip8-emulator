/*
    Author: Min Kang
    Creation Date: January 7th, 2024

    Entry point of the SDL executable. Sets up the event loop for the Chip8 interpreter.
*/

#include <SDL2/SDL.h>

#include <iostream>
#include <random>

#include "header.hpp"
#include "logger.hpp"
#include "main.hpp"

// Code from https://stackoverflow.com/questions/288739/generate-random-numbers-uniformly-over-an-entire-range
std::random_device rand_dev{};
std::mt19937 generator{rand_dev()};
std::uniform_int_distribution<uint8_t> distr(0x00, 0xFF);

Logger logger{};

uint8_t generateRandom() { return distr(generator); }

MainBus::MainBus(SDL_Texture *texture) :
    cpu(Logger{}, *this),
    keyboard(Logger{}, *this),
    display(Logger{}, *this, texture, 0x00000000, 0xFFFFFFFF)
{};

Chip8&      MainBus::getCPU()       { return cpu;      };
Keyboard&   MainBus::getKeyboard()  { return keyboard; };
Display&    MainBus::getDisplay()   { return display;  };

void MainBus::notify(EventData event)
{
    switch(event.type)
    {
        case EventType::DISPLAY_CLEAR:
            display.clearScreen();
            break;
        case EventType::DISPLAY_DRAW:
            cpu.setStatusReg(
                display.drawPixelData(
                    event.draw.xpos, 
                    event.draw.ypos, 
                    event.draw.data, 
                    event.draw.size
                )
            );
            break;
        case EventType::KEYBOARD_GET:
            (*event.key) = keyboard.getKey();
            break;
        case EventType::RANDOM: 
            *event.random.dest = event.random.mask & generateRandom();
            break;
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
        logger << "Could not create the window: " << SDL_GetError() << std::endl;
        return 1;
    }

    MainBus main_bus{texture};

    main_bus.getCPU().loadProgram("\\test\\_data\\chipquarium.ch8");
    // main_bus.getCPU().loadProgram("\\test\\_data\\fez.ch8");

    // Game Loop, idea from https://stackoverflow.com/questions/26664139/sdl-keydown-and-key-recognition-not-working-properly
    
    SDL_Event event;
    while(true)
    {        
        const uint64_t prev{SDL_GetTicks64()};

        while(SDL_PollEvent( &event ))
        {
            switch(event.type) {
                case SDL_QUIT:
                    goto end_program;
                case SDL_KEYDOWN:
                    main_bus.getKeyboard().storeKey( event.key.keysym.scancode );
                    std::cout << event.key.keysym.scancode << std::endl;
                    break;
                case SDL_KEYUP:
                    main_bus.getKeyboard().storeKey( SDL_SCANCODE_UNKNOWN );    
                    break;
                default:
                    break;
            }
        }
        
        for(int i{0}; i < 10; ++i) {
            main_bus.getCPU().execute( main_bus.getCPU().fetch() );
        }
        
        main_bus.getDisplay().updateScreen( renderer );

        uint32_t delay{ FRAMES_IN_MS - (SDL_GetTicks64() - prev) };

        main_bus.getCPU().tickTimer();

        SDL_Delay( (delay > FRAMES_IN_MS) ? 0 : delay );
    }

    end_program:

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}