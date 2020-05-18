#include "defines.h"
#include "PageModesD.h"
#include "PageModes.h"
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

TimeMeasureD             PageModesD::timeMeasureD(TimeMeasureD::_1ms);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureD()
{
    switch (PageModesD::timeMeasureD.value)
    {
        case TimeMeasureD::_1ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1ms;
            PageModes::timeMeasure.value = TimeMeasure::_1ms;
            break;
        case TimeMeasureD::_10ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10ms;
            PageModes::timeMeasure.value = TimeMeasure::_10ms;
            break;
        case TimeMeasureD::_100ms:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100ms;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100ms;
            PageModes::timeMeasure.value = TimeMeasure::_100ms;
            break;
        case TimeMeasureD::_1s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_1s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_1s;
            PageModes::timeMeasure.value = TimeMeasure::_1s;
            break;
        case TimeMeasureD::_10s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_10s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_10s;
            PageModes::timeMeasure.value = TimeMeasure::_10s;
            break;
        case TimeMeasureD::_100s:
            PageModesB::timeMeasureB.value = TimeMeasureB::_100s;
            PageModesC::timeMeasureC.value = TimeMeasureC::_100s;
            PageModes::timeMeasure.value = TimeMeasure::_100s;
            break;
    }
    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_6(sTimeMeasureD,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModesD::timeMeasureD, OnPress_TimeMeasureD
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
