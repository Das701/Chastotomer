#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"


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
    Channel::A->set.typeMeasure, OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{
    Menu::SetOpenedPage(PageStatistics::self);
}

DEF_BUTTON(bStatistics, "Стат", "Открывает окно статистики", OnPress_Statistics);


static void OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    Relation::Off();

    if (Channel::A->set.modeFrequency.IsFrequency())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }
    else if (Channel::A->set.modeFrequency.IsRatioAB())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        Relation::On();
    }
    else if (Channel::A->set.modeFrequency.IsRatioAC())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        Relation::On();
    }
    else if (Channel::A->set.modeFrequency.IsT_1())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::A->set.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[3] = nullptr;
    }
    else if (Channel::A->set.modeFrequency.IsComparator())
    {
        items[2] = &bStatistics;
        items[3] = nullptr;
    }

    ModeFrequency::LoadToFPGA();
}

// Выбор режима измерения частоты, отношения частот, "тахометра"
DEF_SWITCH_6(sModeFrequency,
    "Режим", "Измерение частоты",
    "Частота",
    "f=1/T",
    "f(A)/f(B)",
    "f(A)/f(C)",
    "Тахометр",
    "Компаратор",
    Channel::A->set.modeFrequency, OnChanged_ModeFrequency
);


Switch *switchModeFrequencyA = &sModeFrequency;


static void OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::A->set.modePeriod.IsPeriod())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::A->set.modePeriod.IsF_1())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
    }

    Relation::Off();
    
    ModePeriod::LoadToFPGA();
}

// Выбор режима измерения периода
DEF_SWITCH_2(sModePeriod,
    "Режим", "Измерение периода",
    "Период", "T=1/f",
    Channel::A->set.modePeriod, OnChanged_ModePeriod
);


Switch *switchModePeriodA = &sModePeriod;


static void OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (Channel::A->set.modeDuration.value)
    {
    case ModeDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeDuration::Ndt:
    case ModeDuration::StartStop:
    case ModeDuration::FillFactor:
    case ModeDuration::Phase:
        items[2] = Channel::switchTimeLabels; 
        items[3] = nullptr;
        break;
    }

    Relation::Off();

    ModeDuration::LoadToFPGA();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "СтартА-СтопВ", "Коэфф. зап.", "Фаза",
    Channel::A->set.modeDuration, OnChanged_ModeDuration
);


Switch *switchModeDurationA = &sModeDuration;


static void OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (Channel::A->set.modeCountPulse.value)
    {
    case ModeCountPulse::AtB:
        break;

    case ModeCountPulse::ATB:
        items[2] = Channel::switchNumberPeriods;
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
    "А(tB)", "А(TB)", "Старт/Стоп",
    Channel::A->set.modeCountPulse, OnChanged_ModeCountPulse
);


Switch *switchModeCountPulseA = &sModeCountPulse;


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

    switch(Channel::A->set.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::A->set.modeFrequency.IsFrequency())
        {
            items[3] = test ? Channel::switchTimeLabels : nullptr;
        }
        else if (Channel::A->set.modeFrequency.IsTachometer())
        {
            items[2] = test ? Channel::switchTimeLabels : nullptr;
        }
        break;

    case TypeMeasure::Period:
        items[3] = test ? Channel::switchTimeLabels : nullptr;
        break;
    }

    Channel::A->pageModes->VerifySelectedItem();
}

static void OnEvent(EventType::E event)
{
    if (event == EventType::ModeTestChanged)
    {
        OnChanged_ModeTest();
    }
}


Page pageModesA(items, OnEvent);
