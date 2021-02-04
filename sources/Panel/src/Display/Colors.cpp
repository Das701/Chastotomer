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
Color Color::GRAY_10(6);
Color Color::GRAY_50(7);
Color Color::GRAY_20(8);
Color Color::GREEN_50(9);
Color Color::GREEN_25(10);
Color Color::GREEN_10(11);
Color Color::GREEN_20(12);
Color Color::GREEN_5(13);
Color Color::GREEN_15(14);
Color Color::GREEN_12(15);
Color Color::GRAY_75(16);

Color Color::_255(255);

uint colors[Color::Count] =
{
    MAKE_COLOR(0,   0,   0),    // BLACK
    MAKE_COLOR(255, 255, 255),  // WHITE
    MAKE_COLOR(0,   255, 255),  // YELLOW
    MAKE_COLOR(0,   0,   255),  // BLUE
    MAKE_COLOR(255, 0,   0),    // RED
    MAKE_COLOR(0,   255, 0),    // GREEN
    MAKE_COLOR(25,  25,  25),   // GRAY_10
    MAKE_COLOR(127, 127, 127),  // GRAY_50
    MAKE_COLOR(51,  51,  51),   // GRAY_20
    MAKE_COLOR(0,   127, 0),    // GREEN_50
    MAKE_COLOR(0,   64,  0),    // GREEN_25
    MAKE_COLOR(0,   25,  0),    // GREEN_10
    MAKE_COLOR(0,   51,  0),    // GREEN_20
    MAKE_COLOR(0,   13,  0),    // GREEN_5
    MAKE_COLOR(0,   38,  0),    // GREEN_15
    MAKE_COLOR(0,   31,  0),    // GREEN_12
    MAKE_COLOR(191, 191, 191)   // GRAY_75
};
