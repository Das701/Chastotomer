#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"


uint16 colors[Color::Count] =
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


uint16 Color::MakeFromFloat(float r, float g, float b)
{
    int red = (int)(r * 0x1F);
    int green = (int)(g * 0x3F);
    int blue = (int)(b * 0x1F);

    return (uint16)((blue & 0x1F) | ((green & 0x3F) << 5) | ((red & 0x1F) << 11));
}
