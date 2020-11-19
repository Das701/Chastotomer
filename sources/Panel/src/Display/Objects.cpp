#include "defines.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/MathFPGA.h"
#include "Utils/String.h"
#include <cctype>


void Indicator::Draw()
{
    int x = 10;
    int y = 150;

    if (Display::InDrawingPart(y, 50))
    {
        String data = MathFPGA::Data::GiveDigits();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
            {
                FontBig::Write(data.c_str(), x, y, Color::WHITE);
            }
            else
            {
                if (data[0] == 'П')         { x += 40;  }   // Переполнение
                else if (data[0] == 'X')    { x += 100; }   // Деление на ноль

                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(x, y + 15, Color::WHITE);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
        }
    }

    if (Display::InDrawingPart(170, 50))
    {
        FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 360, 170, Color::WHITE);
    }
}
