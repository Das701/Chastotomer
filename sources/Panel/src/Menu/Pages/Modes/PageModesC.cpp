#include "defines.h"
#include "Calculate/ValuesFPGA.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModeCountPulse;


static void OnChanged_TypeMeasure()
{
    Channel::Current()->OnChanged_TypeMeasure();
}


DEF_SWITCH_2(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Сч. имп.",
    Channel::C->mod.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    switch (Channel::C->mod.modeFrequency.value)
    {
    case ModeFrequency::Frequency:
        items[2] = ModesChannel::switchTimeMeasue;
        items[3] = nullptr;
        break;

    case ModeFrequency::RatioCA:
    case ModeFrequency::RatioCB:
        items[2] = ModesChannel::switchNumberPeriods;
        items[3] = ModesChannel::switchTimeLabels;
        items[4] = nullptr;
        break;
    }

    Relation::Off();
    
    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_3(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f(C)/f(A)", "f(C)/f(B)",
    Channel::C->mod.modeFrequency, OnChanged_ModeFrequency
);


Switch *switchModeFrequencyC = &sModeFrequency;


static void OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;

    switch (Channel::C->mod.modeCountPulse.value)
    {
    case ModeCountPulse::CtA:
    case ModeCountPulse::CtB:
        items[2] = nullptr;
        break;

    case ModeCountPulse::CTA:
    case ModeCountPulse::CTB:
        items[2] = ModesChannel::switchNumberPeriods;
        break;
    }

    items[3] = nullptr;

    Relation::Off();
    
    ModeCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_4(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    Channel::C->mod.modeCountPulse, OnChanged_ModeCountPulse
);


Switch *switchModeCountPulseC = &sModeCountPulse;


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    ModesChannel::switchTimeMeasue,
    nullptr
};

PageModes pageModesC(items, nullptr);
