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
    modeDraw = mode;

    if (mode == Object::ModeDraw::ToBuffer)
    {
        if (Display::InDrawingPart(y0, height0))
        {
            Draw();
        }
    }
    else
    {
        if (needUpdate)
        {
            Display::Prepare(width0, height0);

            FillBackground();

            if (Draw())
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


bool DataZone::Draw()
{
    String data = MathFPGA::Data::GiveDigits();

    Color::WHITE.SetAsCurrent();

    if (data[0] != 0)
    {
        if (std::isdigit(data[0]) != 0 || data[0] == ' ' || data[0] == '-')         // Значит, есть данные
        {
            FontBig::Write(data.c_str(), X0() + 10, Y0());
        }
        else
        {
            int x = X0();

            if (data[0] == 'П') { x += 40; }   // Переполнение
            else if (data[0] == '=') { x += 150; }   // Деление на ноль

            Font::Set(TypeFont::GOSTB28B);
            Text(data.c_str()).Write(x, Y0() + 15);
            Font::Set(TypeFont::GOSTAU16BOLD);
        }
    }

    FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), X0() + 360, Y0() + 20);

    return true;
}
