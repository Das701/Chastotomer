#include "defines.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageModesA.h"
#include "Settings.h"
#include "FreqMeter/FreqMeter.h"
#include <cstring>

using namespace Display::Primitives;
using namespace Display;

extern Switch sSync;
extern Item *items[7];

InputCouple     PageChannelA::couple(InputCouple::AC);
InputImpedance  PageChannelA::impedance(InputImpedance::_1MOmh);
ModeFilter      PageChannelA::modeFilter(ModeFilter::Off);
ModeFront       PageChannelA::modeFront(ModeFront::Front);
Divider         PageChannelA::divider(Divider::_1);
TypeSynch       PageChannelA::typeSynch(TypeSynch::Manual);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Couple()
{
    InputCouple::Load();
}

/// Выбор закрытого(открытого) входа текущего канала
DEF_SWITCH_UGO_2(sCouple,
    "\x7C \x7D", "Вход",
    "Открытый вход", "Закрытый вход",
    "\x7C", "\x7D",
    PageChannelA::couple, OnPress_Couple
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Impedance()
{
    FreqMeter::LoadImpedance();
}

/// Установка входного сопротивления текущего канала
DEF_SWITCH_UGO_2(sImpedance,
    "Rвх", "Входное сопротивление канала",
    "1 МОм", "50 Ом",
    "1МОм", "50Ом",
    PageChannelA::impedance, OnPress_Impedance
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Filter()
{
    ModeFilter::Load();
}

/// Включение(отключение) ФНЧ
DEF_SWITCH_UGO_2(sLowpassFilter,
    "ФНЧ", "Включение/отключение фильтра НЧ",
    "Откл.", "Вкл.",
    "ФНЧ", "",
    PageChannelA::modeFilter, OnPress_Filter
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Front()
{
    FreqMeter::LoadModeFront();
}

/// Выбор фронта синхронизации текущего канала
DEF_SWITCH_UGO_2(sFront,
    "\x8D \x8E", "Выбор типа синхронизации",
    "Фронт", "Срез",
    "\x8D", "\x8E",
    PageChannelA::modeFront, OnPress_Front
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Divider()
{
    FreqMeter::LoadDivider();
}

/// Включение(отключение) входного делителя напряжения
DEF_SWITCH_UGO_2(sDivider,
    "\x7E\x7F\x7E \x7E\x7F\x8F", "Вкл/откл входного делителя",
    "1:1", "1:10",
    "", "1:10",
    PageChannelA::divider, OnPress_Divider
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels,  OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    uint8 time = PageModesA::timeMeasure.value;

    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;
    PageModesD::timeMeasure.value = time;

    static const uint8 periods[TimeMeasure::Count] =
    {
        NumberPeriods::_1,
        NumberPeriods::_10,
        NumberPeriods::_100,
        NumberPeriods::_1K,
        NumberPeriods::_10K,
        NumberPeriods::_100K,
        NumberPeriods::_1000K
    };

    uint8 period = periods[PageModesA::timeMeasure.value];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesA::timeMeasure, OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    if (PageChannelA::typeSynch == TypeSynch::Holdoff)
    {
        items[0] = &sSync;
        items[1] = &sTimeMeasure;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        items[4] = nullptr;
        items[5] = nullptr;
    }
    else
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        
    }
    TYPE_SYNCH_A = (TypeSynch::E)PageChannelA::typeSynch.value;
    FreqMeter::LoadTypeSynch();
}

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
DEF_SWITCH_UGO_2(sSync,
    "Синхр", "Выбор уровня сихронизации",
    "Ручн", "Holdoff",
    "Ручн", "Holdoff",
    PageChannelA::typeSynch, OnPress_Sync
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

static Page pageChannelA(items);

Page *PageChannelA::self = &pageChannelA;

Switch *PageChannelA::switchTypeSynch = &sSync;

void PageChannelA::FixPress()
{
    OnPress_Sync();
}


InputImpedance &InputImpedance::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageChannelA::impedance : PageChannelB::impedance;
}


ModeFront &ModeFront::Current()
{
    return CURRENT_CHANNEL_IS_A ? PageChannelA::modeFront : PageChannelB::modeFront;
}
