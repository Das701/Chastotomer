#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"


using namespace Primitives;


extern Item *items[7];


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, false, false, false };
TypeMeasure          PageModesD::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, false, false, false, false, false, false, false, false, false};
ModeMeasureFrequency PageModesD::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);


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


DEF_SWITCH_2(sModeFrequency,
    "Режим", "Измерение частоы",
    "Частота", "Частота",
    PageModesD::modeMeasureFrequency, OnChanged_ModeMeasureFrequency
)


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    PageModes::switchTimeMeasue,
    nullptr,
    nullptr
};

static Page pageModesD(items, nullptr);

Page *PageModesD::self = &pageModesD;
