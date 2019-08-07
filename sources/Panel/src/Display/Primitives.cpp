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

    uint8 *address = Display::GetBuff() + 320 * y + x;
    *address = color.value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HLine::HLine(int _width) : width(_width)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    uint8 *address = Display::GetBuff() + 320 * y + x;
    
    int number = width;
    
    while (number > 0)
    {
        *address = Color::GetCurrent().value;
        address++;
        number--;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
VLine::VLine(int _height) : height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 *address = Display::GetBuff() + 320 * y + x;

    int number = height;

    while (number > 0)
    {
        *address = Color::GetCurrent().value;
        address += 320;
        number--;
    }
}
