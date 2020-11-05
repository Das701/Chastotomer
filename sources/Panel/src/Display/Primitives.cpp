#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Display.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{

}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    HLine(width).Draw(x, y);
    HLine(width).Draw(x, y + height);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width, y);
}


void Rectangle::Fill(int x0, int y0, Color color)
{
    color.SetAsCurrent();

    for (int y = y0; y < y0 + height; y++)
    {
        HLine(width).Draw(x0, y);
    }
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x++, y);
    }
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x, y++);
    }
}
