#include "defines.h"
#include "FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sTimeMeasureD;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, false, false, false };
TypeMeasure          PageModesD::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, false, false, false, false, false, false, false, false, false};
ModeMeasureFrequency PageModesD::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency);
TimeMeasure          PageModesD::timeMeasure(TimeMeasure::_1ms);


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
