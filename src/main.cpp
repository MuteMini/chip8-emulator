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

MainBus::MainBus(Chip8 *cpu, Keyboard *keyboard, Display* display) :
    cpu(cpu),
    keyboard(keyboard),
    display(display)
{
    this->cpu->linkBus(this);
    this->keyboard->linkBus(this);
    this->display->linkBus(this);
};

MainBus::~MainBus()
{
    delete cpu;
    delete keyboard;
    delete display;
};

Chip8& MainBus::getCPU()        { return *cpu; };
Display& MainBus::getDisplay()  { return *display; };

void MainBus::notify(Component *component, EventData event)
{
    if( component == cpu )
    {
        switch(event.type)
        {
            case EventType::DISPLAY_CLEAR:
                display->clearScreen();
                break;
            case EventType::DISPLAY_DRAW:
                cpu->setStatusReg(
                    display->drawPixelData(
                        event.draw.xpos, 
                        event.draw.ypos, 
                        event.draw.data, 
                        event.draw.size
                    )
                );
                break;
            case EventType::KEYBOARD_GET:
                *event.key = keyboard->getKey();
                break;
            case EventType::RANDOM: 
                *event.random.dest = event.random.mask & generateRandom();
                break;
        }
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

    MainBus main_bus{new Chip8{}, new Keyboard{}, new Display{texture, 0x00000000, 0xFFFFFFFF}};

    main_bus.getCPU().loadProgram("\\test\\_data\\IBMLogo.ch8");

    // Game Loop, idea from https://stackoverflow.com/questions/26664139/sdl-keydown-and-key-recognition-not-working-properly
    SDL_Event event;
    while(true)
    {        
        main_bus.getCPU().execute( main_bus.getCPU().fetch() );
        main_bus.getDisplay().updateScreen( renderer );

        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT ) 
            {
                goto end_program;
            }
            else if( event.type == SDL_KEYDOWN )
            {
                main_bus.getKeyboard().storeKey( event.key.keysym.scancode );
            }
        }

        SDL_Delay(10);
    }

    end_program:

    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}