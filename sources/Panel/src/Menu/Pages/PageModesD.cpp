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


static void OnPress_TimeMeasureD()
{
    uint8 time = PageModesD::timeMeasure.value;

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadTimeMeasure();
}

/// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureD,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesD::timeMeasure, OnPress_TimeMeasureD
);


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
