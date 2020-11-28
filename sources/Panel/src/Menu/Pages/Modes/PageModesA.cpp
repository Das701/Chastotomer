#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/MathFPGA.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/PageStatistics.h"
#include "Menu/Pages/Modes/PagesModes.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"


using namespace Primitives;


extern Item *items[7];
extern Switch sModeFrequency;
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;

static bool relationOn = false;
static bool startStop = false;


static const bool correctModeMeasureCountPulse[ModeCountPulse::Count] = { true, true, false, false, false, false, false, false, true};
ModeCountPulse PageModesA::modeMeasureCountPulse(ModeCountPulse::AtB, correctModeMeasureCountPulse, ModeCountPulse::Count);


void PageModesA::RelationOn()
{
    relationOn = true;
}

void PageModesA::RelationOff()
{
    relationOn = false;
}

bool PageModesA::RelationCheck()
{
    return relationOn == true;
}


void PageModesA::PressSetup()
{
    switch (Channel::A.typeMeasure.value)
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


void PageModesA::OnChanged_TypeMeasure()
{
    switch (Channel::A.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        PageModesA::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::Period:
        PageModesA::OnChanged_ModePeriod();
        break;

    case TypeMeasure::Duration:
        PageModesA::OnChanged_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        PageModesA::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "Измерение", "Выбор измерения",
    "Частота", "Период", "Длит.", "Сч. имп.",
    Channel::A.typeMeasure, PageModesA::OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{
    Menu::SetOpenedPage(PageStatistics::self);
}

DEF_BUTTON(bStatistics, "Стат", "Открывает окно статистики", OnPress_Statistics);


void PageModesA::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (Channel::A.modeFrequency.IsFrequency())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsRatioAB())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (Channel::A.modeFrequency.IsRatioAC())
    {
        items[2] = Channel::switchTimeMeasue;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (Channel::A.modeFrequency.IsT_1())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? Channel::switchTimeLabels : nullptr;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if (Channel::A.modeFrequency.IsComparator())
    {
        items[2] = &bStatistics;
        items[3] = nullptr;
        PageModesA::RelationOff();
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
    Channel::A.modeFrequency, PageModesA::OnChanged_ModeFrequency
);


void PageModesA::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (Channel::A.modePeriod.IsPeriod())
    {
        items[2] = Channel::switchNumberPeriods;
        items[3] = Channel::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (Channel::A.modePeriod.IsF_1())
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
    Channel::A.modePeriod, PageModesA::OnChanged_ModePeriod
);


void PageModesA::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (Channel::A.modeDuration.value)
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

    PageModesA::RelationOff();

    ModeDuration::LoadToFPGA();
}

// Выбор режима измерения длительности импульсов, интервалов, коэффициента заполнения, разности фаз
DEF_SWITCH_5(sModeDuration,
    "Режим", "Измерение длительности",
    "ndt", "ndt/1нс", "СтартА-СтопВ", "Коэфф. зап.", "Фаза",
    Channel::A.modeDuration, PageModesA::OnChanged_ModeDuration
);


void PageModesA::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesA::modeMeasureCountPulse.value)
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

    PageModesA::RelationOff();

    ModeCountPulse::LoadToFPGA();
}

// Выбор режима счёта импульсов
DEF_SWITCH_3(sModeCountPulse,
    "Режим", "Счёт числа импульсов",
    "А(tB)", "А(TB)", "Старт/Стоп",
    PageModesA::modeMeasureCountPulse, PageModesA::OnChanged_ModeCountPulse
);


bool PageModesA::StartStop()
{
    return startStop;
}

void PageModesA::ToggleStartStop()
{
    if (startStop == false)
    {
        startStop = true;
    }
    else
    {
        startStop = false;
    }
}


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

    switch(Channel::A.typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (Channel::A.modeFrequency.IsFrequency())
        {
            items[3] = test ? Channel::switchTimeLabels : nullptr;
        }
        else if (Channel::A.modeFrequency.IsTachometer())
        {
            items[2] = test ? Channel::switchTimeLabels : nullptr;
        }
        break;

    case TypeMeasure::Period:
        items[3] = test ? Channel::switchTimeLabels : nullptr;
        break;
    }

    Channel::A.pageModes->VerifySelectedItem();
}

static void OnEvent(EventType::E event)
{
    if (event == EventType::ModeTestChanged)
    {
        OnChanged_ModeTest();
    }
}


Page pageModesA(items, OnEvent);
