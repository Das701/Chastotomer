#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModeCountPulse;


DEF_SWITCH_2(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Сч. имп.",
    Channel::C.typeMeasure, Channel::OnChanged_TypeMeasure
)


void Channel::OnChanged_ModeFrequencyC()
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

    Channel::RelationOff();
    
    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(C)/f(A)", "f(C)/f(B)",
    Channel::C.modeFrequency, Channel::OnChanged_ModeFrequencyC
);


void Channel::OnChanged_ModeCountPulseC()
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

    Channel::RelationOff();
    
    ModeCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    Channel::C.modeMeasureCountPulse, Channel::OnChanged_ModeCountPulseC
);


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr
};

Page pageModesC(items, nullptr);
