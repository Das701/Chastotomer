#include "defines.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Primitives.h"

using namespace Display::Primitives;

Color Color::BLACK(0);
Color Color::_1(1);
Color Color::_2(2);
Color Color::_3(3);
Color Color::_4(4);
Color Color::_5(5);
Color Color::_6(6);
Color Color::_7(7);
Color Color::_8(8);
Color Color::_9(9);
Color Color::_10(10);
Color Color::_11(11);
Color Color::_12(12);
Color Color::_13(13);
Color Color::_14(14);
Color Color::WHITE(15);
Color Color::Number(16);


Color Color::current = Color::BLACK;

uint cols[16] =
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
    0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff
};

#define COLOR(x) cols[x];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Color::SetAsCurrent()
{
    if (value < 16)
    {
        uint val = COLOR(value);
        uint8 blue = (uint8)val;
        uint8 green = (uint8)(val >> 8);
        uint8 red = (uint8)(val >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::GetCurrent()
{
    return current;
}
