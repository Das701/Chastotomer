#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"



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
