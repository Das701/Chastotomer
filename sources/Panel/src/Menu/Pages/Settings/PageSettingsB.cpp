#include "defines.h"
#include "FreqMeter.h"
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Settings/PagesSettings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include <cstring>


using namespace Display::Primitives;
using namespace Display;

extern Switch sSync;
extern Item *items[7];


InputCouple      PageSettingsB::couple(InputCouple::AC);
InputImpedance   PageSettingsB::impedance(InputImpedance::_1MOmh);
ModeFilter       PageSettingsB::modeFilter(ModeFilter::Off);
ModeFront        PageSettingsB::modeFront(ModeFront::Front);
Divider          PageSettingsB::divider(Divider::_1);
TypeSynch        PageSettingsB::typeSynch(TypeSynch::Manual);


static void OnPress_Couple()
{
    InputCouple::Load();
}

// Выбор закрытого(открытого) входа текущего канала
DEF_SWITCH_UGO_2(sCouple,
    "Вход", "Вход",
    "Открытый вход", "Закрытый вход",
    "\x81", "\x80",
    PageSettingsB::couple, OnPress_Couple
);


static void OnPress_Impedance()
{
    FreqMeter::LoadImpedance();
}

// Установка входного сопротивления текущего канала
DEF_SWITCH_UGO_2(sImpedance,
    "Rвх", "Входное сопротивление канала",
    "1 МОм", "50 Ом",
    "1МОм", "50Ом",
    PageSettingsB::impedance, OnPress_Impedance
);


static void OnPress_Filter()
{
    ModeFilter::Load();
}

// Включение(отключение) ФНЧ
DEF_SWITCH_UGO_2(sLowpassFilter,
    "ФНЧ", "Включение/отключение фильтра НЧ",
    "Откл.", "Вкл.",
    "ФНЧ", "",
    PageSettingsB::modeFilter, OnPress_Filter
);


static void OnPress_Front()
{
    FreqMeter::LoadModeFront();
}

// Выбор фронта синхронизации текущего канала
DEF_SWITCH_UGO_2(sFront,
    "\x82/\x83", "Выбор типа синхронизации",
    "Фронт", "Срез",
    "\x82", "\x83",
    PageSettingsB::modeFront, OnPress_Front
);


static void OnPress_Divider()
{
    FreqMeter::LoadDivider();
}

// Включение(отключение) входного делителя напряжения
DEF_SWITCH_UGO_2(sDivider,
    "Делитель", "Вкл/откл входного делителя",
    "1:1", "1:10",
    "1:1", "1:10",
    PageSettingsB::divider, OnPress_Divider
);


static void OnPress_TimeLabels()
{
    uint8 period = PageModesA::periodTimeLabels.value;

    PageModesB::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesA::periodTimeLabels,  OnPress_TimeLabels
);



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

    uint8 period = periods[time];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesA::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_Sync()
{
    if (PageSettingsB::typeSynch.IsHoldoff())
    {
        items[0] = &sSync;
        items[1] = &sTimeMeasure;
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
    }
    else if(PageSettingsB::typeSynch.IsManual())
    {
        items[0] = &sCouple;
        items[1] = &sImpedance;
        items[2] = &sLowpassFilter;
        items[3] = &sFront;
        items[4] = &sDivider;
        items[5] = &sSync;
        items[6] = nullptr;
    }
    TYPE_SYNCH_B = (TypeSynch::E)PageSettingsB::typeSynch.value;

    TypeSynch::Load(); 
}

// Выбор уровня синхронизации ТТЛ, ЭСЛ
DEF_SWITCH_UGO_2(sSync,
    "Синхр", "Выбор уровня сихронизации",
    "Ручн", "Holdoff",
    "Ручн", "Holdoff",
    PageSettingsB::typeSynch, OnPress_Sync
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

static Page pageChannelB(items);

Page *PageSettingsB::self = &pageChannelB;

Switch *PageSettingsB::switchTypeSynch = &sSync;

void PageSettingsB::FixPress()
{
    OnPress_Sync();
}
