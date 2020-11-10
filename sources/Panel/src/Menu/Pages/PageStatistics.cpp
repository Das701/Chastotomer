#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PageModesA.h"
#include "Utils/String.h"


using namespace Display::Primitives;
using namespace Display;


Stack<double> PageStatistics::values(440);


struct ShowStatistics : public Enumeration
{
    enum E
    {
        No,
        Yes
    };

    explicit ShowStatistics(E v) : Enumeration((uint8)v) {};
};


static ShowStatistics showStatistics(ShowStatistics::No);


static void OnPress_ShowStatistics()
{

}

DEF_SWITCH_2(sShow,
    "Показывать", "",
    "Нет", "Да",
    showStatistics, OnPress_ShowStatistics
)


static void OnPress_Clear()
{

}

DEF_BUTTON(bClear, "Очистить", OnPress_Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageModesA::self);
}

DEF_BUTTON(bExit, "Выход", OnPress_Exit);


static Item *items[] =
{
    &sShow,
    &bClear,
    &bExit,
    nullptr
};


static Page pageShowStatistics(items);

Page *PageStatistics::self = &pageShowStatistics;


void PageStatistics::AppendValue(double value)
{
    if (value < 1.0)
    {
        values.Push(value);
    }
}


void PageStatistics::Clear()
{
}


void PageStatistics::Draw()
{
    if (values.Size() == 0)
    {
        return;
    }

    int x0 = 10;
    int y0 = 10;
    int width = Display::WIDTH - 20;
    int height = 110;

    Rectangle(width, height).Fill(x0, y0, Color::GRAY_50);

    int startElement = values.Size() - 7;

    if (startElement < 0)
    {
        startElement = 0;
    }

    for (int i = startElement; i < values.Size(); i++)
    {
        Text(String("%d - %f", i, values[i]).c_str()).Write(x0 + 1, y0 + 15 * (i - startElement), Color::WHITE);
    }

    double min = 1e100;
    double max = 0;

    for (int i = 0; i < values.Size(); i++)
    {
        if (values[i] < min) { min = values[i]; }

        if (values[i] > max) { max = values[i]; }
    }

    float stepX = (float)width / values.Size();
    float stepY = height / (float)(max - min);

    y0 = y0 + height;

    int x = x0;
    int y = y0 - (int)(stepY * (values[0] - min));
    
    Color::BLACK.SetAsCurrent();

    Point().Draw(x, y);

    for (int i = 1; i < values.Size(); i++)
    {
        int x1 = x0 + (int)((i - 1) * stepX);
        int x2 = x0 + (int)(i * stepX);

        int y1 = y0 - (int)(stepY * (values[i - 1] - min));
        int y2 = y0 - (int)(stepY * (values[i] - min));

        Line().Draw(x1, y1, x2, y2);
    }

    Text(String("%f", max).c_str()).Write(0, 0);
    Text(String("%f", min).c_str()).Write(0, 10 + height);
}
