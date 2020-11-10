#include "defines.h"
#include "FreqMeter.h"
#include "PageModesD.h"
#include "PageModesA.h"
#include "PageModesB.h"
#include "PageModesC.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sTimeMeasureD;


TypeMeasureD          PageModesD::typeMeasure(TypeMeasureD::Frequency);
ModeMeasureFrequencyD PageModesD::modeMeasureFrequency(ModeMeasureFrequencyD::Frequency);
TimeMeasure           PageModesD::timeMeasure(TimeMeasure::_1ms);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesD::timeMeasure.value;

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadTimeMeasure();
}

// Выбор времени измерения
DEF_SWITCH_7(sTimeMeasureD,
    "Время", "Время счёта",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesD::timeMeasure, OnPress_TimeMeasure
);


static void OnChanged_TypeMeasure()
{

}


DEF_SWITCH_2(sTypeMeasure,
    "Измерения", "Выбор измерения",
    "Частота", "Частота",
    PageModesD::typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeMeasureFrequency()
{

}


DEF_SWITCH_2(sModeMeasureFrequency,
    "Режим", "Измерение частоы",
    "Частота", "Частота",
    PageModesD::modeMeasureFrequency, OnChanged_ModeMeasureFrequency
)


static Item *items[7] =
{
    &sTimeMeasureD,
    nullptr,
    &sTypeMeasure,
    &sModeMeasureFrequency,
    nullptr
};

static Page pageModesD(items, nullptr);

Page *PageModesD::self = &pageModesD;
