#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"


SColor colors[Color::Count] =
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


SColor Color::MakeFromFloat(float r, float g, float b)
{
    SColor result;
    result.r = (uint8)(r * 255);
    result.g = (uint8)(g * 255);
    result.b = (uint8)(b * 255);
    return result;
}


SColor SColor::MakeFromUint8(uint8 red, uint8 green, uint8 blue)
{
    SColor result;
    result.r = red;
    result.g = green;
    result.b = blue;
    return result;
}
