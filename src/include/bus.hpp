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
#include <string>

#include "event.hpp"
#include "logger.hpp"

class Component;

class Bus
{
    public:
        virtual void notify(EventData event) = 0;
};

class Component 
{
    protected:
        Logger logger;
        Bus& bus;
    
    public:
        Component(std::string logName, Bus& bus) : logger(logName), bus(bus) {};
};

#endif