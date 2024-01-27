/*
    Author: Min Kang
    Creation Date: January 8th, 2024

    Entry point for unit testing. Utilizes Doctest that is ran by custom script in run.sh.
*/
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "chip8.hpp"
#include "bus.hpp"

class MockBus : public Bus
{
    void notify(Component *component, EventData event);
};

TEST_CASE("Chip8 functionality") {
    Chip8 chip8{};

    SUBCASE("loadProgram") {
        CHECK(chip8.loadProgram("\\test\\_data\\IBMLogo.ch8"));
        CHECK(!chip8.loadProgram("\\test\\_data\\nofile.ch8"));
    }
}