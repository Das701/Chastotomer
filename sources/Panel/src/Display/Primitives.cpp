#include "defines.h"
#include "Display/Primitives.h"


using namespace Display::Primitives;


void Rectangle::FillRounded(int x, int y, int round, Color colorFill, Color colorBound)
{
    DrawRounded(x, y, round, colorBound);
    Rectangle(width - 3, height - 3).Fill(x + 2, y + 2, colorFill);
    HLine(width - 3).Draw(x + 2, y + 1);
    HLine(width - 3).Draw(x + 2, y + height - 1);
    VLine(height - 3).Draw(x + 1, y + 2);
    VLine(height - 3).Draw(x + width - 1, y + 2);
}


void Rectangle::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    Fill(x, y);
}
