#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Display.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    HLine(width).Draw(x, y);
    HLine(width).Draw(x, y + height);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Rectangle::Fill(int x0, int y0, Color color)
{
    color.SetAsCurrent();

    for (int y = y0; y < y0 + height; y++)
    {
        HLine(width).Draw(x0, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Point::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    if (x >= 0 && x < Display::GetWidth() && y >= 0 && y < Display::GetHeight())
    {
        *Display::GetPixel(x, y) = Color::GetCurrent().value;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int w = Display::GetWidth();
    int h = Display::GetHeight();

    if (y < 0 || y >= h)
    {
        return;
    }

    int delta = length;

    if (x < 0)
    {
        delta = length + x;
        x = 0;
        if (delta < 0)
        {
            return;
        }
    }

    if (x + delta >= w)
    {
        delta = w - x;
        if (delta < 0)
        {
            return;
        }
    }

    uint8 *p = Display::GetPixel(x, y);
    
    uint8 *end = p + delta;
    
    uint8 value = Color::GetCurrent().value;
    
    while (p < end)
    {
        *p++ = value;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 *p = Display::GetPixel(x, y);

    uint8 value = Color::GetCurrent().value;

    int width = Display::GetWidth();

    for (int i = 0; i < length; i++)
    {
        *p = value;
        p += width;
    }
}
