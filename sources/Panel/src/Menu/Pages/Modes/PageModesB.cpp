#include "defines.h"
#include "FreqMeter.h"
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


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, true, true, true };
TypeMeasure           PageModesB::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, true, false, false, true, true, false, false, true, false};
ModeMeasureFrequency  PageModesB::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
ModeMeasurePeriod     PageModesB::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration   PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, true, true, false, false, false, false, true };
ModeMeasureCountPulse PageModesB::modeMeasureCountPulse(ModeMeasureCountPulse::BtA, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);


void PageModesB::PressSetup()
{
    switch (PageModesB::typeMeasure.value)
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
    switch (PageModesB::typeMeasure.value)
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
    PageModesB::typeMeasure, PageModesB::OnChanged_TypeMeasure
)


void PageModesB::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (PageModesB::modeMeasureFrequency.IsFrequency())
    {
        items[2] = PageModes::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? PageModes::switchTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBA())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBC())
    {
        items[2] = PageModes::switchTimeMeasue;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsT_1())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = PageModes::switchTimeLabels;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? PageModes::switchTimeLabels : nullptr;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    ModeMeasureFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_5(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота", "f=1/T", "f(B)/f(A)", "f(B)/f(C)", "Тахометр",
    PageModesB::modeMeasureFrequency, PageModesB::OnChanged_ModeFrequency
);


void PageModesB::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesB::modeMeasurePeriod.IsPeriod())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = PageModes::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (PageModesB::modeMeasurePeriod.IsF_1())
    {
        items[2] = PageModes::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? PageModes::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();

    ModeMeasurePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    PageModesB::modeMeasurePeriod, PageModesB::OnChanged_ModePeriod
);


void PageModesB::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesA::modeMeasureDuration.value = PageModesB::modeMeasureDuration.value;
    
    switch (PageModesB::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::Ndt2:
    case ModeMeasureDuration::FillFactor:
    case ModeMeasureDuration::Phase:
        items[2] = PageModes::switchTimeLabels;
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
        items[2] = PageModes::switchNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulse::StartStop:
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
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
    PageModes::switchTimeMeasue,
    nullptr
};


static void OnChanged_ModeTest()
{
    bool test = FreqMeter::modeTest.IsEnabled();

    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (PageModesB::modeMeasureFrequency.IsFrequency())
        {
            items[3] = test ? PageModes::switchTimeLabels : nullptr;
        }
        else if (PageModesB::modeMeasureFrequency.IsTachometer())
        {
            items[2] = test ? PageModes::switchTimeLabels : nullptr;
        }
        break;
    case TypeMeasure::Period:
        items[3] = test ? PageModes::switchTimeLabels : nullptr;
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
