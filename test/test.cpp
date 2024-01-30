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
    public:
        Chip8 cpu{};

        EventData recentData{};

        MockBus() : cpu()
        {
            cpu.linkBus(this);
        };

        void notify(Component *component, EventData event)
        {
            recentData = event;
        };

        uint8_t checkRegValue(uint8_t reg)
        {
            cpu.execute(static_cast<uint16_t>(0xD000 + (reg << 8)));
            return recentData.draw.xpos;
        };
};

TEST_CASE("Chip8 Unit Tests")
{
    MockBus bus{};

    SUBCASE("Loading program")
    {
        REQUIRE_FALSE(bus.cpu.loadProgram("\\test\\_data\\nofile.ch8"));
        REQUIRE(bus.cpu.loadProgram("\\test\\_data\\IBMLogo.ch8"));

        SUBCASE("Fetching instructions")
        {
            bus.cpu.loadProgram("\\test\\_data\\IBMLogo.ch8");

            CHECK_EQ(bus.cpu.fetch(), 0x00E0);
            bus.cpu.execute(bus.cpu.fetch());

            CHECK_EQ(bus.cpu.fetch(), 0xA22A);
            bus.cpu.execute(bus.cpu.fetch());
        }
    }

    SUBCASE("Verify bus & reg instructions")
    {
        bus.cpu.execute(0x00E0);
        CHECK_MESSAGE(bus.recentData.type == EventType::DISPLAY_CLEAR, "INSTR: 00E0 (1/34)");
 
        bus.cpu.execute(0xD010);
        REQUIRE_MESSAGE(bus.recentData.type == EventType::DISPLAY_DRAW, "INSTR: DXYN (2/34)");
        CHECK(bus.recentData.draw.size == 0);

        bus.cpu.execute(0xF20A);
        CHECK_MESSAGE(bus.recentData.type == EventType::KEYBOARD_GET, "INSTR: FX0A (3/34)");

        bus.cpu.execute(0xC123);
        CHECK_MESSAGE(bus.recentData.type == EventType::RANDOM, "INSTR: CXNN (4/34)");
        CHECK(bus.recentData.random.mask == 0x23);
    
        // Uses DXYN instruction to verify instructions that manage cpu state
        bus.cpu.execute(0x6011);
        REQUIRE_MESSAGE(bus.checkRegValue(0) == 0x11, "INSTR: 6XNN (5/34)");

        bus.cpu.execute(0x7014);
        CHECK_MESSAGE(bus.checkRegValue(0) == 0x25, "INSTR: 7XNN (6/34)");

        bus.cpu.execute(0x8100);
        CHECK_MESSAGE(bus.checkRegValue(1) == 0x25, "INSTR: 8XY0 (7/34)");

        bus.cpu.execute(0x6280);
        bus.cpu.execute(0x8211);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0xA5, "INSTR: 8XY1 (8/34)");

        bus.cpu.execute(0x620F);
        bus.cpu.execute(0x8212);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x05, "INSTR: 8XY2 (9/34)");

        bus.cpu.execute(0x620F);
        bus.cpu.execute(0x8213);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x2A, "INSTR: 8XY3 (10/34)");

        bus.cpu.execute(0x620F);
        bus.cpu.execute(0x8214);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x34, "INSTR: 8XY4 w/o carry (11/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 0, "VF 0 when carry not occured");
        bus.cpu.execute(0x62E0);
        bus.cpu.execute(0x8214);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x05, "INSTR: 8XY4 w/ carry (11/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 1, "VF 1 when carry not occured");

        bus.cpu.execute(0x6235);
        bus.cpu.execute(0x8215);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x10, "INSTR: 8XY5 (12/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 0, "VF 0 when borrow not occured");
        bus.cpu.execute(0x6215);
        bus.cpu.execute(0x8215);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0xF0, "INSTR: 8XY5 (12/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 1, "VF 1 when borrow occured");

        bus.cpu.execute(0x6324);
        bus.cpu.execute(0x8236);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x12, "INSTR: 8XY6 (13/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 0, "VF 0 when LSB 0");
        bus.cpu.execute(0x8226);
        bus.cpu.execute(0x8226);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x04, "INSTR: 8XY6 (13/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 1, "VF 1 when LSB 1");

        bus.cpu.execute(0x6215);
        bus.cpu.execute(0x8217);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x10, "INSTR: 8XY7 (14/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 0, "VF 0 when borrow not occured");
        bus.cpu.execute(0x6235);
        bus.cpu.execute(0x8217);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0xF0, "INSTR: 8XY7 (14/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 1, "VF 1 when borrow occured");

        bus.cpu.execute(0x6324);
        bus.cpu.execute(0x823E);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x48, "INSTR: 8XYE (15/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 0, "VF 0 when MSB 0");
        bus.cpu.execute(0x822E);
        bus.cpu.execute(0x822E);
        CHECK_MESSAGE(bus.checkRegValue(2) == 0x20, "INSTR: 8XYE (15/34)");
        CHECK_MESSAGE(bus.checkRegValue(15) == 1, "VF 1 when MSB 1");

        bus.cpu.execute(0xF507);
        CHECK_MESSAGE(bus.checkRegValue(5) == 0x00, "INSTR: FX07 (28/34)");

        bus.cpu.execute(0xF115);
        bus.cpu.execute(0xF507);
        CHECK_MESSAGE(bus.checkRegValue(5) == 0x25, "INSTR: FX15 (29/34)");
    }

    SUBCASE("Loading data")
    {
        uint8_t test_data[8]{0, 1, 0, 2, 0, 3, 0, 4};
        REQUIRE(bus.cpu.loadData(0x200, test_data, 8));

        for(uint8_t i{1}; i <= 4; ++i)
        {
            CHECK_EQ(bus.cpu.fetch(), i);
            bus.cpu.execute(0);
        }
        CHECK_EQ(bus.cpu.fetch(), 0x0000);

        SUBCASE("Verify address instructions")
        {
            bus.cpu.execute(0x6020);
            bus.cpu.execute(0x6104);
            bus.cpu.execute(0x6411);
            bus.cpu.execute(0x6522);
            bus.cpu.execute(0x6611);

            bus.cpu.execute(0x1200);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 1200 (16/34)");

            bus.cpu.execute(0x2204);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0003, "INSTR: 2202 (17/34)");

            bus.cpu.execute(0x00EE);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 00EE (18/34)");

            bus.cpu.execute(0x1200 - 2);
            bus.cpu.execute(0x3400);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 3XNN not skipped (19/34)");
            bus.cpu.execute(0x3411);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0003, "INSTR: 3XNN skipped (19/34)");

            bus.cpu.execute(0x1200 - 2);
            bus.cpu.execute(0x4411);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 4XNN not skipped (20/34)");
            bus.cpu.execute(0x4400);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0003, "INSTR: 4XNN skipped (20/34)");

            bus.cpu.execute(0x1200 - 2);
            bus.cpu.execute(0x5450);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 5XY0 not skipped (21/34)");
            bus.cpu.execute(0x5460);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0003, "INSTR: 5XY0 skipped (21/34)");
            
            bus.cpu.execute(0x1200 - 2);
            bus.cpu.execute(0x9460);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0001, "INSTR: 9XY0 not skipped (22/34)");
            bus.cpu.execute(0x9450);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0003, "INSTR: 9XY0 skipped (22/34)");

            bus.cpu.execute(0xB1E2);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x0002, "INSTR: BNNN (23/34)");

            // No real manner to test ANNN
            bus.cpu.execute(0xA300);
            bus.cpu.execute(0xF655);
            bus.cpu.execute(0x1304);
            CHECK_MESSAGE(bus.cpu.fetch() == 0x1122, "INSTR: ANNN, FX55 (25/34)");

            // No real manner to test FXIE
            bus.cpu.execute(0xF11E);
            bus.cpu.execute(0xF465);
            CHECK_MESSAGE(bus.checkRegValue(0) == 0x11, "INSTR: FX1E, FX65 (27/34)");
            CHECK_MESSAGE(bus.checkRegValue(1) == 0x22, "INSTR: FX1E, FX65 (27/34)");
        }
    }

    // Currently missing EX9E, EXA1, FX18, FX29, FX33
}