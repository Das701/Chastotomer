#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"


Color Color::BLACK(0);
Color Color::WHITE(1);
Color Color::YELLOW(2);
Color Color::BLUE(3);
Color Color::RED(4);
Color Color::GREEN(5);
Color Color::GREY_10(6);
Color Color::GREY_50(7);
Color Color::GREY_20(8);
Color Color::GREEN_50(9);
Color Color::GREEN_25(10);
Color Color::GREEN_10(11);
Color Color::GREEN_20(12);
Color Color::GREEN_5(13);
Color Color::GREEN_15(14);
Color Color::GREEN_12(15);

uint colors[Color::Count] =
{
    MAKE_COLOR(0, 0, 0),            // BLACK
    MAKE_COLOR(0xFF, 0xFF, 0xFF),   // WHITE
    MAKE_COLOR(0x00, 0xFF, 0xFF),   // YELLOW
    MAKE_COLOR(0x00, 0x00, 0xFF),   // BLUE
    MAKE_COLOR(0xFF, 0x00, 0x00),   // RED
    MAKE_COLOR(0x00, 0xFF, 0x00),   // GREEN
    MAKE_COLOR(25, 25, 25),         // GREY_10
    MAKE_COLOR(127, 127, 127),      // GREY_50
    MAKE_COLOR(51, 51, 51),         // GREY_20
    MAKE_COLOR(0, 127, 0),          // GREEN_50
    MAKE_COLOR(0, 255 / 4, 0),      // GREEN_25
    MAKE_COLOR(0, 255 / 10, 0),     // GREEN_10
    MAKE_COLOR(0, 255 / 5, 0),      // GREEN_20
    MAKE_COLOR(0, 255 / 20, 0),     // GREEN_5
    MAKE_COLOR(0, 38, 0),           // GREEN_15
    MAKE_COLOR(0, 31, 0)            // GREEN_12
};
