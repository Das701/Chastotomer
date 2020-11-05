#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"


uint colors[Color::Count] =
{
    0,      // BLACK
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0,      // 
    0       // WHITE
};


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
Color Color::WHITE(255);


uint Color::MakeFromFloat(float red, float green, float blue)
{
    int r = (int)(red * 255);
    int g = (int)(green * 255);
    int b = (int)(blue * 255);
    return (uint)(r | (g << 8) | (b << 16));
}


uint SColor::MakeFromUint8(uint8 red, uint8 green, uint8 blue)
{
    return (uint)(red | (green << 8) | (blue << 16));
}
