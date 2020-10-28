#include "defines.h"
#include "PageModesD.h"
#include "PageModesA.h"
#include "PageModesB.h"
#include "PageModesC.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sTimeMeasureD;

TimeMeasure  PageModesD::timeMeasure(TimeMeasure::_1ms);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureD()
{
    switch (PageModesD::timeMeasure.value)
    {
        case TimeMeasure::_1ms:
            PageModesB::timeMeasure.value = TimeMeasure::_1ms;
            PageModesC::timeMeasure.value = TimeMeasure::_1ms;
            PageModesA::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case TimeMeasure::_10ms:
            PageModesB::timeMeasure.value = TimeMeasure::_10ms;
            PageModesC::timeMeasure.value = TimeMeasure::_10ms;
            PageModesA::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case TimeMeasure::_100ms:
            PageModesB::timeMeasure.value = TimeMeasure::_100ms;
            PageModesC::timeMeasure.value = TimeMeasure::_100ms;
            PageModesA::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case TimeMeasure::_1s:
            PageModesB::timeMeasure.value = TimeMeasure::_1s;
            PageModesC::timeMeasure.value = TimeMeasure::_1s;
            PageModesA::timeMeasure.value = TimeMeasure::_1s;
            break;
        case TimeMeasure::_10s:
            PageModesB::timeMeasure.value = TimeMeasure::_10s;
            PageModesC::timeMeasure.value = TimeMeasure::_10s;
            PageModesA::timeMeasure.value = TimeMeasure::_10s;
            break;
        case TimeMeasure::_100s:
            PageModesB::timeMeasure.value = TimeMeasure::_100s;
            PageModesC::timeMeasure.value = TimeMeasure::_100s;
            PageModesA::timeMeasure.value = TimeMeasure::_100s;
            break;
        case TimeMeasure::_1000s:
            PageModesB::timeMeasure.value = TimeMeasure::_1000s;
            PageModesC::timeMeasure.value = TimeMeasure::_1000s;
            PageModesA::timeMeasure.value = TimeMeasure::_1000s;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureD,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesD::timeMeasure, OnPress_TimeMeasureD
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[7] =
{
    &sTimeMeasureD,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static Page pageModesD(items);

Page *PageModesD::self = &pageModesD;
