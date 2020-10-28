#include "defines.h"
#include "PageChannelB.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings.h"
#include <cstring>


using namespace Display::Primitives;
using namespace Display;

extern Switch sSync;
extern Item *items[7];


InputCouple      PageChannelB::couple(InputCouple::AC);
InputImpedance   PageChannelB::impedance(InputImpedance::_1MOmh);
ModeFilter       PageChannelB::modeFilter(ModeFilter::Off);
ModeFront        PageChannelB::modeFront(ModeFront::Front);
Divider          PageChannelB::divider(Divider::_1);
TypeSynch        PageChannelB::typeSynch(TypeSynch::Manual);

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
    PageChannelB::couple, OnPress_Couple
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
    PageChannelB::impedance, OnPress_Impedance
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
    PageChannelB::modeFilter, OnPress_Filter
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
    PageChannelB::modeFront, OnPress_Front
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
    PageChannelB::divider, OnPress_Divider
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
    switch (PageModes::periodTimeLabels.value)
    {
        case PeriodTimeLabels::T_3:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_3;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_3;
            break;
        case PeriodTimeLabels::T_4:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_4;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_4;
            break;
        case PeriodTimeLabels::T_5:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_5;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_5;
            break;
        case PeriodTimeLabels::T_6:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_6;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_6;
            break;
        case PeriodTimeLabels::T_7:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_7;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_7;
            break;
        case PeriodTimeLabels::T_8:
            PageModesB::periodTimeLabelsB.value = PeriodTimeLabelsB::T_8;
            PageModesC::periodTimeLabelsC.value = PeriodTimeLabelsC::T_8;
            break;
    }
    FreqMeter::LoadPeriodTimeLabels();
}

/// Выбор периода меток времени
DEF_SWITCH_6(sPeriodTimeLabels,
    "Метки", "Длительность временных меток",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::periodTimeLabels,  OnPress_TimeLabels
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
    switch (PageModes::timeMeasure.value)
    {
        case TimeMeasure::_1ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1ms;
            PageModesB::numberPeriods.value = NumberPeriods::_1;
            PageModesC::numberPeriods.value = NumberPeriods::_1;
            PageModes::numberPeriods.value = NumberPeriods::_1;
            break;
        case TimeMeasure::_10ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10;
            PageModes::numberPeriods.value = NumberPeriods::_10;
            break;
        case TimeMeasure::_100ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100ms;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100;
            PageModes::numberPeriods.value = NumberPeriods::_100;
            break;
        case TimeMeasure::_1s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1K;
            PageModes::numberPeriods.value = NumberPeriods::_1K;
            break;
        case TimeMeasure::_10s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_10s;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_10K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_10K;
            PageModes::numberPeriods.value = NumberPeriods::_10K;
            break;
        case TimeMeasure::_100s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_100s;
            PageModes::numberPeriods.value = NumberPeriods::_100K;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_100K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_100K;
            break;
        case TimeMeasure::_1000s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1000s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1000s;
            PageModesD::timeMeasureD.value = TimeMeasureD::_1000s;
            PageModes::numberPeriods.value = NumberPeriods::_1000K;
            PageModesB::numberPeriodsB.value = NumberPeriodsB::_1000K;
            PageModesC::numberPeriodsC.value = NumberPeriodsC::_1000K;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModes::timeMeasure, OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Sync()
{
    if (PageChannelB::typeSynch == TypeSynch::Holdoff)
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
    TYPE_SYNCH_B = (TypeSynch::E)PageChannelB::typeSynch.value;
    FreqMeter::LoadTypeSynch();
    
}

/// Выбор уровня синхронизации ТТЛ, ЭСЛ
DEF_SWITCH_UGO_2(sSync,
    "Синхр", "Выбор уровня сихронизации",
    "Ручн", "Holdoff",
    "Ручн", "Holdoff",
    PageChannelB::typeSynch, OnPress_Sync
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

Page *PageChannelB::self = &pageChannelB;

Switch *PageChannelB::switchTypeSynch = &sSync;

void PageChannelB::FixPress()
{
    OnPress_Sync();
}
