#include "defines.h"
#include "Settings.h"
#include "Display/Symbols.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>


using namespace Primitives;


extern Switch sSync;
extern Item *items[7];


static void OnPress_Couple()
{
    InputCouple::LoadToFPGA();
}

// Выбор закрытого(открытого) входа текущего канала
DEF_SWITCH_UGO_2(sCouple,
    "Вход", "Вход",
    "Открытый вход", "Закрытый вход",
    SYMBOL_DC_IN_QUOTES, SYMBOL_AC_IN_QUOTES,
    Channel::B->set.couple, OnPress_Couple
);


static void OnPress_Impedance()
{
    InputImpedance::LoadToFPGA();
}

// Установка входного сопротивления текущего канала
DEF_SWITCH_UGO_2(sImpedance,
    "Rвх", "Входное сопротивление канала",
    "1 МОм", "50 Ом",
    "1МОм", "50Ом",
    Channel::B->set.impedance, OnPress_Impedance
);


static void OnPress_Filter()
{
    ModeFilter::LoadToFPGA();
}

// Включение(отключение) ФНЧ
DEF_SWITCH_UGO_2(sLowpassFilter,
    "ФНЧ", "Включение/отключение фильтра НЧ",
    "Откл.", "Вкл.",
    "ФНЧ", "",
    Channel::B->set.modeFilter, OnPress_Filter
);


static void OnPress_Front()
{
    ModeFront::LoadToFPGA();
}

// Выбор фронта синхронизации текущего канала
DEF_SWITCH_UGO_2(sFront,
    "\x82/\x83", "Выбор типа синхронизации",
    "Фронт", "Срез",
    SYMBOL_FRONT_IN_QUOTES, SYMBOL_RISE_IN_QUOTES,
    Channel::B->set.modeFront, OnPress_Front
);


static void OnPress_Divider()
{
    Divider::LoadToFPGA();
}

// Включение(отключение) входного делителя напряжения
DEF_SWITCH_UGO_2(sDivider,
    "Делитель", "Вкл/откл входного делителя",
    "1:1", "1:10",
    "1:1", "1:10",
    Channel::B->set.divider, OnPress_Divider
);


// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    ModesChannel::timeLabels,  PeriodTimeLabels::LoadToFPGA
);


static void OnPress_Sync()
{
    if (Channel::B->set.typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = ModesChannel::switchTimeMeasue;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;

        Channel::B->pageSettings->selectedItem = 0;
    }
    else if(Channel::B->set.typeSynch.IsManual())
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;

        Channel::B->pageSettings->selectedItem = 5;
    }

    TYPE_SYNCH_B = (TypeSynch::E)Channel::B->set.typeSynch.value;

    TypeSynch::LoadToFPGA();
}

// Выбор уровня синхронизации ТТЛ, ЭСЛ
DEF_SWITCH_UGO_2(sSync,
    "Синхр", "Выбор уровня сихронизации",
    "Ручн", "Holdoff",
    "Ручн", "Holdoff",
    Channel::B->set.typeSynch, OnPress_Sync
);

static Item *items[7] =
{
    &sCouple,
    &sImpedance,
    &sLowpassFilter,
    &sFront,
    &sDivider,
    &sSync,
    nullptr
};

Page pageChannelB(items, nullptr);
