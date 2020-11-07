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
Color Color::WHITE(1);
Color Color::YELLOW(2);
Color Color::BLUE(3);
Color Color::RED(4);
Color Color::GREEN(5);


static Color current = Color::BLACK;

uint colors[Color::Count] =
{
    0x00000000U,    // BLACK
    0xFFFFFFFFU,    // WHITE
    0x00FFFF00U,    // YELLOW
    0x000000FFU,    // BLUE
    0x00FF00FFU,    // RED
    0x0000FF00U     // GREEN
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
