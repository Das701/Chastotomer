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

    if (x >= 0 && x < Display::WIDTH && y >= 0 && y < Display::HEIGHT)
    {    
        static int tempX = x / 2;
        if(x%2 != 0)
        {
            *Display::GetPixel(tempX, y) = *Display::GetPixel(tempX, y) | (Color::GetCurrent().value & 0xF0);
        }
        else
        {
            *Display::GetPixel(tempX, y) = *Display::GetPixel(tempX, y) | (Color::GetCurrent().value & 0x0F);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x, y);
        x++;
    }
//    uint8 *p = Display::GetPixel(x, y);
//    
//    uint8 *end = p + length;
//    
//    uint8 value = Color::GetCurrent().value;
//    
//    while (p <= end)
//    {
//        *p++ = value;
//    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x, y);
        x++;
    }
//    uint8 *p = Display::GetPixel(x, y);
//    
//    uint8 value = Color::GetCurrent().value;
//    
//    for (int i = 0; i < length; i++)
//    {
//        *p = value;
//        p += 320;
//    }
}
