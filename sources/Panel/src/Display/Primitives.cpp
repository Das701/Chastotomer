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
        uint8 *point = Display::GetPixel(x, y);     // Адрес байта, в котором наша точка

        if ((x % 2) == 0)
        {
            *point &= 0xF0;                         // Очищаем младшую тетраду
            *point |= color.value;                  // Записываем в младшую тетраду цвет
        }
        else
        {
            *point &= 0x0F;                         // Очищаем старшую тетраду
            *point |= (color.value << 4);           // Записываем в старшую тетраду цвет
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x++, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    
    for(int i = 0; i < length; i++)
    {
        Point().Draw(x, y++);
    }
}
