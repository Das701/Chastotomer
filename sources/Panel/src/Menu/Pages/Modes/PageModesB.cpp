#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/FreqMeter.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;       
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;


static void OnChanged_TypeMeasure()
{
    Channel::Current()->OnChanged_TypeMeasure();
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    Channel::B->mod.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    Relation::Off();

    if (Channel::B->mod.modeFrequency.IsFrequency())
    {
        items[2] = ModesChannel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? ModesChannel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }
    else if (Channel::B->mod.modeFrequency.IsRatioBA())
    {
        items[2] = ModesChannel::switchNumberPeriods;
        items[3] = nullptr;
        Relation::On();
    }
    else if (Channel::B->mod.modeFrequency.IsRatioBC())
    {
        items[2] = ModesChannel::switchTimeMeasue;
        items[3] = nullptr;
        Relation::On();
    }
    else if (Channel::B->mod.modeFrequency.IsT_1())
    {
        items[2] = ModesChannel::switchNumberPeriods;
        items[3] = ModesChannel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if(Channel::B->mod.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? ModesChannel::switchTimeLabels : nullptr;
        items[3] = nullptr;
    }

    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f=1/T", "f(B)/f(A)", "f(B)/f(C)", "Тахометр",
    Channel::B->mod.modeFrequency, OnChanged_ModeFrequency
);


Switch *switchModeFrequencyB = &sModeFrequency;


static void OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::B->mod.modePeriod.IsPeriod())
    {
        items[2] = ModesChannel::switchNumberPeriods;
        items[3] = ModesChannel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::B->mod.modePeriod.IsF_1())
    {
        items[2] = ModesChannel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? ModesChannel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }

    Relation::Off();

    ModePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    Channel::B->mod.modePeriod, OnChanged_ModePeriod
);


Switch *switchModePeriodB = &sModePeriod;


static void OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (Channel::B->mod.modeDuration.value)
    {
    case ModeDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeDuration::Ndt:
    case ModeDuration::StartStop:
    case ModeDuration::FillFactor:
    case ModeDuration::Phase:
        items[2] = ModesChannel::switchTimeLabels;
        items[3] = nullptr;
        break;
    }

    Relation::Off();

    ModeDuration::LoadToFPGA();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "СтартB-СтопA", "Коэфф. зап.", "Фаза",
    Channel::B->mod.modeDuration, OnChanged_ModeDuration
);


Switch *switchModeDurationB = &sModeDuration;


static void OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (Channel::B->mod.modeCountPulse.value)
    {
    case ModeCountPulse::BtA:
        break;

    case ModeCountPulse::BTA:
        items[2] = ModesChannel::switchNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeCountPulse::StartStop:
        break;
    }

    Relation::Off();

    ModeCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "B(tA)", "B(TA)", "Старт/Стоп",
    Channel::B->mod.modeCountPulse, OnChanged_ModeCountPulse
);


Switch *switchModeCountPulseB = &sModeCountPulse;


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    ModesChannel::switchTimeMeasue,
    nullptr
};


static void OnChanged_ModeTest()
{
    bool test = FreqMeter::modeTest.IsEnabled();

    switch (Channel::B->mod.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::B->mod.modeFrequency.IsFrequency())
        {
            items[3] = test ? ModesChannel::switchTimeLabels : nullptr;
        }
        else if (Channel::B->mod.modeFrequency.IsTachometer())
        {
            items[2] = test ? ModesChannel::switchTimeLabels : nullptr;
        }
        break;
    case TypeMeasure::Period:
        items[3] = test ? ModesChannel::switchTimeLabels : nullptr;
        break;
    }

    Channel::B->pageModes->VerifySelectedItem();
}


static void OnEvent(EventType::E event)
{
    switch(event)
    {
    case EventType::ModeTestChanged:
        OnChanged_ModeTest();
        break;

    case EventType::Count:
        break;
    }
}


PageModes pageModesB(items, OnEvent);
