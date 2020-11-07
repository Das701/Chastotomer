#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Display.h"


using namespace Display::Primitives;



void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Rectangle::Draw(int x, int y)
{
    HLine(width).Draw(x, y);
    HLine(width).Draw(x, y + height - 1);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width - 1, y);
}


void Rectangle::Fill(int x0, int y0)
{
    for (int y = y0; y < y0 + height; y++)
    {
        HLine(width).Draw(x0, y);
    }
}
