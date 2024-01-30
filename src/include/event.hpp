/*
    Author: Min Kang
    Creation Date: January 16th, 2024

    Declares events that other classes will use to pass data onto the mediator (i.e. bus)
*/

#ifndef EVENT_H
#define EVENT_H

#include <cstdint>
#include <list>

enum class EventType
{
    DISPLAY_CLEAR,
    DISPLAY_DRAW,
    KEYBOARD_WAIT,
    KEYBOARD_GET,
    RANDOM
};

struct EventData
{
    EventType type;
    union
    {
        struct
        {
            uint8_t xpos;
            uint8_t ypos;
            uint8_t *data;
            std::size_t size;
        } draw;
        uint8_t *key;
        struct
        {
            uint8_t mask;
            uint8_t *dest;
        } random;
    };
};

#endif