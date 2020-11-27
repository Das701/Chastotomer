#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"


using namespace Primitives;


extern Item *items[7];


static void OnChanged_TypeMeasure()
{

}


DEF_SWITCH_2(sTypeMeasure,
    "Измерения", "Выбор измерения",
    "Частота", "Частота",
    Channel::D.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeMeasureFrequency()
{

}


DEF_SWITCH_2(sModeFrequency,
    "Режим", "Измерение частоы",
    "Частота", "Частота",
    Channel::D.modeFrequency, OnChanged_ModeMeasureFrequency
)


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr,
    nullptr
};

static Page pageModesD(items, nullptr);

Page *PageModesD::self = &pageModesD;
