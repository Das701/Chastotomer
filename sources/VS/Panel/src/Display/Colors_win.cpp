#include "defines.h"
#include "Display/Display.h"
#include "Display/Colors.h"
#include "Display/Primitives.h"

#pragma warning(push, 0)
#undef CRC
#include <wx/wx.h>
#pragma warning(pop)

using namespace Display::Primitives;


extern wxMemoryDC memDC;

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


static Color current = Color::BLACK;

uint colors[Color::Count] =
{
    0, 0, 0, 0,
    0, 0, 0, 0,
    0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU,
    0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU
};


void Color::SetAsCurrent()
{
    uint val = COLOR(value);
    uint8 b = (uint8)val;
    uint8 g = (uint8)(val >> 8);
    uint8 r = (uint8)(val >> 16);

    wxColour colorDraw = wxColour(r, g, b);

    memDC.SetPen(wxPen(colorDraw));
}


Color Color::GetCurrent()
{
    return current;
}
