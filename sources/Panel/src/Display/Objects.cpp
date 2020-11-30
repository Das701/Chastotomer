#include "defines.h"
#include "Display/Display.h"
#include "Display/Objects.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include <cctype>
#include <cstdlib>


using namespace Primitives;


uint ProgressBarTimeMeasureZone::timeStart = 0;


void Object::Update(Object::ModeDraw::E mode)
{
    modeDraw = mode;

    x0 = (mode == ModeDraw::ToHardware) ? 0 : left;
    y0 = (mode == ModeDraw::ToHardware) ? 0 : top;

    if (mode == Object::ModeDraw::ToBuffer)
    {
        if (Display::InDrawingPart(top, height))
        {
            Draw();
        }
    }
    else
    {
        if (needUpdate)
        {
            Display::Prepare(width, height);

            FillBackground();

            if (Draw())
            {
                needUpdate = false;
            }

            Display::SendToFSMC(left, top);

            Display::Restore();
        }
    }
}


void Object::Refresh()
{
    if (this == Display::zoneProgressBarTimeMeasure)
    {
        if(Menu::OpenedPage() == PageStatistics::self || PageIndication::calibration.Is(Calibration::Pressed))
        {
            return;
        }
    }


    needUpdate = true;
}


void Object::FillBackground()
{
    //    Rectangle(width, height).Fill(0, 0, Color::BLACK);

    Display::BeginScene(left, top);
}


bool DataZone::Draw()
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

            if (data[0] == 'П') { x += 40; }   // Переполнение
            else if (data[0] == '=') { x += 150; }   // Деление на ноль

            Font::Set(TypeFont::GOSTB28B);
            Text(data.c_str()).Write(x, y0 + 15);
            Font::Set(TypeFont::GOSTAU16BOLD);
        }
    }

    FontMid::Write(MathFPGA::Data::GiveUnits().c_str(), x0 + 360, y0 + 20);

    return true;
}


bool ProgressBarTimeMeasureZone::Draw()
{
    if (Channel::Current()->ConsistTimeMeasure() && Channel::timeMeasure.value > TimeMeasure::_10ms)
    {
        int timeCycle = Channel::timeMeasure.ToMS();

        float part = ((float)(TIME_MS - timeStart) / (float)timeCycle);

        int w = (int)(46.0F * part) + 1;

        LIMITATION(w, 0, Width() - 1);

        Primitives::Rectangle(w, 5).Fill(x0, y0, Color::WHITE);
    }

    return true;
}
