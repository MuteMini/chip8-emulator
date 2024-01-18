/*
    Author: Min Kang
    Creation Date: January 16th, 2024

    Declares the mediator pattern - each module (cpu, display, keyboard, etc)
    raises an event to the bus, the mediator of all components.
    Idea took from https://refactoring.guru/design-patterns/mediator
*/

#ifndef BUS_H
#define BUS_H

#include <cstdint>

#include "event.hpp"

class Component;

class Bus
{
    public:
        virtual void notify(Component *component, EventData event) = 0;
};

class Component 
{
    protected:
        Bus *bus{};
    
    public:
        void linkBus(Bus *bus) { this->bus = bus; };
};

#endif