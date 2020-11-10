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


Stack<double> PageStatistics::values(400);


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
    values.Push(value / 1e3);
}


void PageStatistics::Clear()
{
}


void PageStatistics::Draw()
{
    int x0 = 10;
    int y0 = 10;

    Rectangle(Display::WIDTH - 20, 110).Fill(x0, y0, Color::GRAY_50);

    int startElement = values.Size() - 7;

    if (startElement < 0)
    {
        startElement = 0;
    }

    for (int i = startElement; i < values.Size(); i++)
    {
        Text(String("%d - %f", i, values[i]).c_str()).Write(x0 + 1, y0 + 15 * (i - startElement), Color::BLACK);
    }
}
