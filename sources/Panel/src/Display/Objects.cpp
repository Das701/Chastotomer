#include "defines.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/MathFPGA.h"
#include "Utils/String.h"
#include <cctype>
#include <cstdlib>


using namespace Primitives;


void Object::Update()
{
    if (Display::DrawingToBuffer())
    {
        DrawToBuffer();
    }
    else
    {
        if (needUpdate)
        {
            Display::Prepare(width0, height0);

            FillBackground();

            if (DrawToHardware())
            {
                needUpdate = false;
            }

            Display::SendToFSMC(x0, y0);

            Display::Restore();
        }
    }
}


void Object::FillBackground()
{
    Rectangle(width0, height0).Fill(0, 0, Color::WHITE);
}


bool DataZone::DrawToHardware()
{
    Rectangle(10, 10).Fill(10, 10, Color::BLACK);

//    // Отрисовка заключается в следующем.
//    // Каждый объект отрисовывается в начале дисплейного буфера
//
//
//    uint8 buffer[100][100];
//
//    for (int i = 0; i < 100; i++)
//    {
//        for (int j = 0; j < 100; j++)
//        {
//            buffer[i][j] = (uint8)(std::rand() % 16);
//        }
//    }
//
//    HAL_FSMC::SendBuffer(&buffer[0][0], 10, 50, 100, 100);

    return true;
}


void DataZone::DrawToBuffer()
{
    if (Display::InDrawingPart(y0, height0))
    {
        String data = MathFPGA::Data::GiveDigits();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
            {
                FontBig::Write(data.c_str(), x0 + 10, y0, Color::WHITE);
            }
            else
            {
                int x = x0;

                if (data[0] == 'П')         { x += 40;  }   // Переполнение
                else if (data[0] == '=')    { x += 150; }   // Деление на ноль

                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(x, y0 + 15, Color::WHITE);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
        }

        FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 360, 170, Color::WHITE);
    }
}
