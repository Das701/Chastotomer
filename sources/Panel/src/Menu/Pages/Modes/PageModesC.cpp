#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModeCountPulse;


void PageModesC::OnChanged_TypeMeasure()
{
    switch (Channel::C.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesC::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::CountPulse:
        PageModesC::OnChanged_ModeCountPulse();
        break;
    }
}

DEF_SWITCH_2(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Сч. имп.",
    Channel::C.typeMeasure, PageModesC::OnChanged_TypeMeasure
)


void PageModesC::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    switch (Channel::C.modeFrequency.value)
    {
    case ModeFrequency::Frequency:
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        break;

    case ModeFrequency::RatioCA:
    case ModeFrequency::RatioCB:
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
        break;
    }

    PageModesA::RelationOff();
    
    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(C)/f(A)", "f(C)/f(B)",
    Channel::C.modeFrequency, PageModesC::OnChanged_ModeFrequency
);


void PageModesC::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;

    switch (Channel::C.modeMeasureCountPulse.value)
    {
    case ModeCountPulse::CtA:
    case ModeCountPulse::CtB:
        items[2] = nullptr;
        break;

    case ModeCountPulse::CTA:
    case ModeCountPulse::CTB:
        items[2] = Channel::switchNumberPeriods;
        break;
    }

    items[3] = nullptr;

    PageModesA::RelationOff();
    
    ModeCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    Channel::C.modeMeasureCountPulse, PageModesC::OnChanged_ModeCountPulse
);


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr
};

Page pageModesC(items, nullptr);
