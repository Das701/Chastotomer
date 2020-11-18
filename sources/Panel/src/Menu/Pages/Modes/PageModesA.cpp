#include "defines.h"
#include "FreqMeter.h"
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


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, true, true, true };
TypeMeasure           PageModesA::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure, TypeMeasure::Count);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, true, true, true, false, false, false, false, true, true };
ModeMeasureFrequency  PageModesA::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency, ModeMeasureFrequency::Count);
ModeMeasurePeriod     PageModesA::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration   PageModesA::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { true, true, false, false, false, false, false, false, true};
ModeMeasureCountPulse PageModesA::modeMeasureCountPulse(ModeMeasureCountPulse::AtB, correctModeMeasureCountPulse, ModeMeasureCountPulse::Count);


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
    switch (PageModesA::typeMeasure.value)
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
    switch (PageModesA::typeMeasure.value)
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
    PageModesA::typeMeasure, PageModesA::OnChanged_TypeMeasure
);


static void OnPress_Statistics()
{
    Menu::SetOpenedPage(PageStatistics::self);
}

DEF_BUTTON(bStatistics, "Стат", "Открывает окно статистики", OnPress_Statistics);


void PageModesA::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (PageModesA::modeMeasureFrequency.IsFrequency())
    {
        items[2] = PageModes::switchTimeMeasue;
        items[3] = FreqMeter::modeTest.IsEnabled() ? PageModes::switchTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAB())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsRatioAC())
    {
        items[2] = PageModes::switchTimeMeasue;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesA::modeMeasureFrequency.IsT_1())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = PageModes::switchTimeLabels;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsTachometer())
    {
        items[2] = FreqMeter::modeTest.IsEnabled() ? PageModes::switchTimeLabels : nullptr;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesA::modeMeasureFrequency.IsComparator())
    {
        items[2] = &bStatistics;
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    ModeMeasureFrequency::LoadToFPGA();
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
    PageModesA::modeMeasureFrequency, PageModesA::OnChanged_ModeFrequency
);


void PageModesA::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesA::modeMeasurePeriod.IsPeriod())
    {
        items[2] = PageModes::switchNumberPeriods;
        items[3] = PageModes::switchTimeLabels;
        items[4] = nullptr;
    }
    else if (PageModesA::modeMeasurePeriod.IsF_1())
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
    PageModesA::modeMeasurePeriod, PageModesA::OnChanged_ModePeriod
);


void PageModesA::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    switch (PageModesA::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::StartStop:
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
    "ndt", "ndt/1нс", "СтартА-СтопВ", "Коэфф. зап.", "Фаза",
    PageModesA::modeMeasureDuration, PageModesA::OnChanged_ModeDuration
);


void PageModesA::OnChanged_ModeCountPulse()
{
    items[1] = &sModeCountPulse;
    items[2] = nullptr;

    switch (PageModesA::modeMeasureCountPulse.value)
    {
    case ModeMeasureCountPulse::AtB:
        break;

    case ModeMeasureCountPulse::ATB:
        items[2] = PageModes::switchNumberPeriods;
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
    PageModes::switchTimeMeasue,
    nullptr
};


static void OnChanged_ModeTest()
{
    bool test = FreqMeter::modeTest.IsEnabled();

    switch(PageModesA::typeMeasure.value)
    {
    case TypeMeasure::Frequency:
        if (PageModesA::modeMeasureFrequency.IsFrequency())
        {
            items[3] = test ? PageModes::switchTimeLabels : nullptr;
        }
        else if (PageModesA::modeMeasureFrequency.IsTachometer())
        {
            items[2] = test ? PageModes::switchTimeLabels : nullptr;
        }
        break;

    case TypeMeasure::Period:
        items[3] = test ? PageModes::switchTimeLabels : nullptr;
        break;
    }

    PageModesA::self->VerifySelectedItem();
}

static void OnEvent(EventType::E event)
{
    if (event == EventType::ModeTestChanged)
    {
        OnChanged_ModeTest();
    }
}


static Page pageModes(items, OnEvent);

Page *PageModesA::self = &pageModes;
