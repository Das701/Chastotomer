#include "defines.h"
#include "Settings.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Display/Indicator.h"
#include "Display/Objects.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cctype>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


uint ProgressBarTimeMeasureZone::timeStart = 0;
uint SynchroZone::timeStart = 0;


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
        if(Menu::OpenedPage() == PageStatistics::self || PageIndication::calibrationMode.IsEnabled())
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


static bool ConditionSplit(char *text)
{
    if (ValueFPGA::IsEmpty())
    {
        return false;
    }

    return !(SU::ExistSymbol(text, '.') || SU::ExistSymbol(text, ','));
}


#define RIGHT_DIGITS 360
#define LEFT_UNTIS   380


bool DataZone::Draw()
{
    String data = ValueFPGA::GiveDigits();

    Color::WHITE.SetAsCurrent();

    if (data[0] != 0)
    {
        if (ValueFPGA::IsData())
        {
            if (gset.styleGUI.IsModern())
            {
                FontBig::WriteAboutRight(data.c_str(), x0 + RIGHT_DIGITS, y0, ConditionSplit(data.c_str()));
            }
            else
            {
                Indicator::DrawDataAboutRight(data.c_str(), x0 + RIGHT_DIGITS, y0 + 1, Color::WHITE, Color::BLACK);
            }
        }
        else
        {
            int x = x0;

            if (ValueFPGA::IsOverlapped())   { x += 40; }   // Переполнение
            else if (ValueFPGA::IsDivNULL()) { x += 150; }  // Деление на ноль

            if (gset.styleGUI.IsModern())
            {
                Font::Set(TypeFont::GOSTB28B);
                Text(data.c_str()).Write(x, y0 + 15);
                Font::Set(TypeFont::GOSTAU16BOLD);
            }
            else
            {
                Indicator::DrawDataAboutRight(data.c_str(), RIGHT_DIGITS, y0 + 1, Color::WHITE, Color::BLACK);
            }
        }
    }

    FontMid::Write(ValueFPGA::GiveUnits().c_str(), x0 + LEFT_UNTIS, y0 + 20);

    return true;
}


bool ProgressBarTimeMeasureZone::Draw()
{
    if (Channel::Current()->mod.ConsistTimeMeasure() && ModesChannel::timeMeasure.value > TimeMeasure::_10ms)
    {
        int timeCycle = ModesChannel::timeMeasure.ToMS();

        float part = ((float)(TIME_MS - timeStart) / (float)timeCycle);

        int w = (int)(46.0F * part) + 1;

        LIMITATION(w, 0, Width() - 1); //-V2516

        Primitives::Rectangle(w, 5).Fill(x0, y0, Color::WHITE);
    }

    return true;
}


void ProgressBarTimeMeasureZone::Reset()
{
    timeStart = TIME_MS;
}


bool SynchroZone::Draw()
{
    int size = CalculateSize();

    if (size > 0)
    {
        Primitives::Rectangle(size, size).Fill(x0, y0, Color::WHITE);
    }

    return true;
}


int SynchroZone::CalculateSize()
{
    float timeLife = 2000.0F;

    float time = (float)(TIME_MS - timeStart);

    if (time > timeLife)
    {
        time = timeLife;
    }

    const int MAX_SIZE = 15;

    int size = (int)(MAX_SIZE * (time / timeLife));

    return MAX_SIZE - size;
}


void SynchroZone::Fire()
{
    timeStart = TIME_MS;
}
