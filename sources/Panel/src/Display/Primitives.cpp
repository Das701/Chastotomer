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


void Rectangle::DrawRounded(int x, int y, int round, Color color)
{
    color.SetAsCurrent();

    HLine(width - round * 2 + 1).Draw(x + round, y);
    HLine(width - round * 2 + 1).Draw(x + round, y + height);
    VLine(height - round * 2 + 1).Draw(x, y + round);
    VLine(height - round * 2 + 1).Draw(x + width, y + round);

    if (round == 2)
    {
        Point().Draw(x + 1, y + 1);
        Point().Draw(x + width - 1, y + 1);
        Point().Draw(x + 1, y + height - 1);
        Point().Draw(x + width - 1, y + height - 1);
    }
}


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
