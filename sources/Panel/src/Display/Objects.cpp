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


void Object::Update(Object::ModeDraw::E mode)
{
    if (mode == Object::ModeDraw::ToBuffer)
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
//    Rectangle(width0, height0).Fill(0, 0, Color::GREEN_20);

    Display::BeginScene(x0, y0);
}


bool DataZone::DrawToHardware()
{
    String data = MathFPGA::Data::GiveDigits();

    Color::WHITE.SetAsCurrent();

    if (data[0] != 0)
    {
        if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
        {
            FontBig::Write(data.c_str(), 10, 0);
        }
        else
        {
            int x = 0;

            if (data[0] == 'П')      { x += 40; }   // Переполнение
            else if (data[0] == '=') { x += 150; }   // Деление на ноль

            Font::Set(TypeFont::GOSTB28B);
            Text(data.c_str()).Write(x, 15);
            Font::Set(TypeFont::GOSTAU16BOLD);
        }
    }

    FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 360, 20);

    return true;
}


void DataZone::DrawToBuffer()
{
    if (Display::InDrawingPart(y0, height0))
    {
        String data = MathFPGA::Data::GiveDigits();

        Color::WHITE.SetAsCurrent();

        if (data[0] != 0)
        {
            if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
            {
                FontBig::Write(data.c_str(), x0 + 10, y0);
            }
            else
            {
                int x = x0;

                if (data[0] == 'П')         { x += 40;  }   // Переполнение
                else if (data[0] == '=')    { x += 150; }   // Деление на ноль

                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(x0, y0 + 15);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
        }

        FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), 370, 170);
    }
}
