#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Utils/String.h"


using namespace Display::Primitives;
using namespace Display;


struct ModeShow : public Enumeration
{
    enum E
    {
        Full,       // Показывать полный вид - от 0 до максимального значения
        Parth       // Показывать частичный вид - от минимального до максимального значения
    };

    explicit ModeShow(E v) : Enumeration((uint8)v) {};
};


static ModeShow modeShow(ModeShow::Full);

Stack<double> PageStatistics::values(440);


static void OnPress_ModeShow()
{
    Display::Refresh();
}

DEF_SWITCH_2(sModeShow,
    "Вид", "Вид",
    "Полный", "Частичный",
    modeShow, OnPress_ModeShow
);


DEF_BUTTON(bClear, "Очистить", PageStatistics::Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageModesA::self);
}

DEF_BUTTON(bExit, "Выход", OnPress_Exit);


static Item *items[7] =
{
    &sModeShow,
    &bClear,
    &bExit,
    nullptr
};


static Page pageShowStatistics(items, nullptr);

Page *PageStatistics::self = &pageShowStatistics;


void PageStatistics::AppendValue(double value)
{
    values.Push(value);
}


void PageStatistics::Clear()
{
    PageStatistics::values.Clear();
}


void PageStatistics::Draw()
{
    int x0 = 10;
    int y0 = 10;
    int width = Display::WIDTH - 20;
    int height = 110;

    Rectangle(width, height).Fill(x0, y0, Color::GRAY_50);

    if (values.Size() < 2)
    {
        return;
    }

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

    if (modeShow.Is(ModeShow::Full))
    {
        min = 0.0;
    }

    float stepX = (float)width / (float)(values.Size() - 1);
    float stepY = (float)height / (float)(max - min);

    y0 = y0 + height;

    int x = x0;
    int y = y0 - (int)(stepY * (values[0] - min));
    
    Color::BLACK.SetAsCurrent();

    Point().Draw(x, y);

    for (int i = 1; i < values.Size(); i++)
    {
        int x1 = x0 + (int)((float)(i - 1) * stepX);
        int x2 = x0 + (int)((float)i * stepX);

        int y1 = y0 - (int)(stepY * (values[i - 1] - min));
        int y2 = y0 - (int)(stepY * (values[i] - min));

        Line().Draw(x1, y1, x2, y2);
    }

    Text(String("%f", max).c_str()).Write(0, 0);
    Text(String("%f", min).c_str()).Write(0, 10 + height);
}
