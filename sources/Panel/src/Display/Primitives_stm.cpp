#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Display.h"


using namespace Display::Primitives;



Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{

}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
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
