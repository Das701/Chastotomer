#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Hardware/MathFPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PagesModes.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;       
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;


ModeMeasureDuration   PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, true, true, false, false, false, false, true };
ModeMeasureCountPulse PageModesB::modeMeasureCountPulse(ModeMeasureCountPulse::BtA, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);


void PageModesB::PressSetup()
{
    switch (Channel::B.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        items[1] = &sModeFrequency;
        break;

    case TypeMeasure::Period:
        items[1] = &sModePeriod;
        break;

    case TypeMeasure::Duration:
        items[1] = &sModeDuration;
        break;

    case TypeMeasure::CountPulse:
        items[1] = &sModeCountPulse;
        break;
    }
}


void PageModesB::OnChanged_TypeMeasure()
{
    switch (Channel::B.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesB::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::Period:
        PageModesB::OnChanged_ModePeriod();
        break;

    case TypeMeasure::Duration:
        PageModesB::OnChanged_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        PageModesB::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    Channel::B.typeMeasure, PageModesB::OnChanged_TypeMeasure
)


void PageModesB::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (Channel::B.modeFrequency.IsFrequency())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (Channel::B.modeFrequency.IsRatioBA())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (Channel::B.modeFrequency.IsRatioBC())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (Channel::B.modeFrequency.IsT_1())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if(Channel::B.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f=1/T", "f(B)/f(A)", "f(B)/f(C)", "Тахометр",
    Channel::B.modeFrequency, PageModesB::OnChanged_ModeFrequency
);


void PageModesB::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::B.modePeriod.IsPeriod())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::B.modePeriod.IsF_1())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();

    ModePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    Channel::B.modePeriod, PageModesB::OnChanged_ModePeriod
);


void PageModesB::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (PageModesB::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::StartStop:
    case ModeMeasureDuration::FillFactor:
    case ModeMeasureDuration::Phase:
        items[2] = Channel::switchTimeLabels;
        items[3] = nullptr;
        break;
    }

    PageModesA::RelationOff();

    ModeMeasureDuration::LoadToFPGA();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "СтартB-СтопA", "Коэфф. зап.", "Фаза",
    PageModesB::modeMeasureDuration, PageModesB::OnChanged_ModeDuration
);


void PageModesB::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesB::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::BtA:
        break;

    case ModeMeasureCountPulse::BTA:
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulse::StartStop:
        break;
    }

    PageModesA::RelationOff();

    ModeMeasureCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "B(tA)", "B(TA)", "Старт/Стоп",
    PageModesB::modeMeasureCountPulse, PageModesB::OnChanged_ModeCountPulse
);


static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    Channel::switchTimeMeasue,
    nullptr
};


static void OnChanged_ModeTest()
{
    bool test = FreqMeter::modeTest.IsEnabled();

    switch (Channel::B.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::B.modeFrequency.IsFrequency())
        {
            items[3] = test ? Channel::switchTimeLabels : nullptr;
        }
        else if (Channel::B.modeFrequency.IsTachometer())
        {
            items[2] = test ? Channel::switchTimeLabels : nullptr;
        }
        break;
    case TypeMeasure::Period:
        items[3] = test ? Channel::switchTimeLabels : nullptr;
        break;
    }

    PageModesB::self->VerifySelectedItem();
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


static Page pageModesB(items, OnEvent);

Page *PageModesB::self = &pageModesB;
