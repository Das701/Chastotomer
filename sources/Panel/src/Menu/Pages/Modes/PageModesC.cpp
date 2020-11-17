#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModeCountPulse;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, false, false, true };
TypeMeasure           PageModesC::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, false, false, false, false, false, true, true, false, false };
ModeMeasureFrequency  PageModesC::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, false, false, true, true, true, true, false };
ModeMeasureCountPulse PageModesC::modeMeasureCountPulse(ModeMeasureCountPulse::CtA, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);


void PageModesC::OnChanged_TypeMeasure()
{
    switch (PageModesC::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesA::typeMeasure.value = TypeMeasure::Frequency;
        PageModesB::typeMeasure.value = TypeMeasure::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeFrequency();
        break;
    case TypeMeasure::CountPulse:
        PageModesB::typeMeasure.value = TypeMeasure::CountPulse;
        PageModesA::typeMeasure.value = TypeMeasure::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesC::OnChanged_ModeCountPulse();
        break;
    }
}

DEF_SWITCH_2(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Сч. имп.",
    PageModesC::typeMeasure, PageModesC::OnChanged_TypeMeasure
)


void PageModesC::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    switch (PageModesC::modeMeasureFrequency.value)
    {
    case ModeMeasureFrequency::Frequency:
        items[2] = PageModes::switchTimeMeasue;
        items[3] = nullptr;
        break;

    case ModeMeasureFrequency::RatioCA:
    case ModeMeasureFrequency::RatioCB:
        items[2] = PageModes::switchNumberPeriods;
        items[3] = PageModes::switchTimeLabels;
        items[4] = nullptr;
        break;
    }

    PageModesA::RelationOff();
    
    ModeMeasureFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(C)/f(A)", "f(C)/f(B)",
    PageModesC::modeMeasureFrequency, PageModesC::OnChanged_ModeFrequency
);


void PageModesC::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;

    switch (PageModesC::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::CtA:
    case ModeMeasureCountPulse::CtB:
        items[2] = nullptr;
        break;

    case ModeMeasureCountPulse::CTA:
    case ModeMeasureCountPulse::CTB:
        items[2] = PageModes::switchNumberPeriods;
        items[3] = nullptr;
        break;
    }

    items[3] = nullptr;

    PageModesA::RelationOff();
    
    ModeMeasureCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    PageModesC::modeMeasureCountPulse, PageModesC::OnChanged_ModeCountPulse
);


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    PageModes::switchTimeMeasue,
    nullptr
};

static Page pageModesC(items, nullptr);

Page *PageModesC::self = &pageModesC;


void PageModesC::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasure::Frequency:  modeMeasureFrequency.value = 0;                           break;
    case TypeMeasure::CountPulse: modeMeasureCountPulse.value = ModeMeasureCountPulse::CtA; break;
    }
}
