#include "defines.h"
#include "Display/Primitives.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PageModesA.h"


using namespace Display::Primitives;


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
    "����������", "",
    "���", "��",
    showStatistics, OnPress_ShowStatistics
)


static void OnPress_Clear()
{

}

DEF_BUTTON(bClear, "��������", OnPress_Clear);


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageModesA::self);
}

DEF_BUTTON(bExit, "�����", OnPress_Exit);


static Item *items[] =
{
    &sShow,
    &bClear,
    &bExit,
    nullptr
};


static Page pageShowStatistics(items);

Page *PageStatistics::self = &pageShowStatistics;


void PageStatistics::AppendValue(ValuePICO &)
{

}


void PageStatistics::Clear()
{

}


void PageStatistics::Draw()
{
    Rectangle(100, 100).Fill(10, 100, Color::WHITE);
}
