#include "defines.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];


static void OnChanged_TypeMeasure()
{

}


DEF_SWITCH_2(sTypeMeasure,
    "Измерения", "Выбор измерения",
    "Частота", "Частота",
    Channel::D->mod.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeMeasureFrequency()
{

}


DEF_SWITCH_2(sModeFrequency,
    "Режим", "Измерение частоы",
    "Частота", "Частота",
    Channel::D->mod.modeFrequency, OnChanged_ModeMeasureFrequency
)

Switch *switchModeFrequencyD = &sModeFrequency;

static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    ModesChannel::switchTimeMeasue,
    nullptr,
    nullptr
};

PageModes pageModesD(items, nullptr);
