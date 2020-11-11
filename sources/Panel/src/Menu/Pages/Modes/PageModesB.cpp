#include "defines.h"
#include "FreqMeter.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Menu/MenuItems.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/PageModes.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sModeFrequency;       
extern Switch sModePeriod;
extern Switch sModeDuration;
extern Switch sModeCountPulse;
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;


static const bool correctTypeMeasure[TypeMeasure::Count] = { true, true, true, true };
TypeMeasure           PageModesB::typeMeasure(TypeMeasure::Frequency, correctTypeMeasure);
static const bool correctModeMeasureFrequency[ModeMeasureFrequency::Count] = { true, true, false, false, true, true, false, false, true, false};
ModeMeasureFrequency  PageModesB::modeMeasureFrequency(ModeMeasureFrequency::Frequency, correctModeMeasureFrequency);
ModeMeasurePeriod     PageModesB::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration   PageModesB::modeMeasureDuration(ModeMeasureDuration::Ndt);
static const bool correctModeMeasureCountPulse[ModeMeasureCountPulse::Count] = { false, false, true, true, false, false, false, false, true };
ModeMeasureCountPulse PageModesB::modeMeasureCountPulse(ModeMeasureCountPulse::BtA, correctModeMeasureCountPulse);

PeriodTimeLabels      PageModesB::periodTimeLabels(PeriodTimeLabels::T_8);
NumberPeriods         PageModesB::numberPeriods(NumberPeriods::_1);
TimeMeasure           PageModesB::timeMeasure(TimeMeasure::_1ms);


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
        PageModesA::typeMeasure.value = TypeMeasure::Frequency;
        PageModesC::typeMeasure.value = TypeMeasure::Frequency;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeFrequency();
        break;

    case TypeMeasure::Period:
        PageModesA::typeMeasure.value = TypeMeasure::Period;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModePeriod();
        break;

    case TypeMeasure::Duration:
        PageModesA::typeMeasure.value = TypeMeasure::Duration;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeDuration();
        break;

    case TypeMeasure::CountPulse:
        PageModesA::typeMeasure.value = TypeMeasure::CountPulse;
        PageModesC::typeMeasure.value = TypeMeasure::CountPulse;

        PageModes::ResetModeCurrentMeasure();
        PageModesB::OnChanged_ModeCountPulse();
        break;
    }
}


DEF_SWITCH_4(sTypeMeasure,
    "���������", "����� ���������",
    "�������", "������", "����.", "��. ���.",
    PageModesB::typeMeasure, PageModesB::OnChanged_TypeMeasure
)


void PageModesB::OnChanged_ModeFrequency()
{
    items[1] = &sModeFrequency;

    if (PageModesB::modeMeasureFrequency.IsFrequency())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBA())
    {
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsRatioBC())
    {
        items[2] = &sTimeMeasure;
        items[3] = nullptr;
        PageModesA::RelationOn();
    }
    else if (PageModesB::modeMeasureFrequency.IsT_1())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
        PageModesA::RelationOff();
    }
    else if(PageModesB::modeMeasureFrequency.IsTachometer())
    {
        items[2] = &sPeriodTimeLabels;   
        items[3] = nullptr;
        PageModesA::RelationOff();
    }

    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();     
    
    ModeMeasureFrequency::LoadToFPGA();
}

// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_5(sModeFrequency,
    "�����", "��������� �������",
    "�������", "f(B)/f(A)", "f(B)/f(C)", "f=1/T", "��������",
    PageModesB::modeMeasureFrequency, PageModesB::OnChanged_ModeFrequency
);


void PageModesB::OnChanged_ModePeriod()
{
    items[1] = &sModePeriod;

    if (PageModesB::modeMeasurePeriod.IsPeriod())
    {
        items[2] = &sPeriodTimeLabels;
        items[3] = &sNumberPeriods;
        items[4] = nullptr;
    }
    else if (PageModesB::modeMeasurePeriod.IsF_1())
    {
        items[2] = &sTimeMeasure;
        items[3] = FreqMeter::modeTest.IsEnabled() ? &sPeriodTimeLabels : nullptr;
        items[4] = nullptr;
    }

    PageModesA::RelationOff();
    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();

    ModeMeasurePeriod::LoadToFPGA();
}

// ����� ������ ��������� �������
DEF_SWITCH_2(sModePeriod,
    "�����", "��������� �������",
    "������", "T=1/f",
    PageModesB::modeMeasurePeriod, PageModesB::OnChanged_ModePeriod
);


void PageModesB::OnChanged_ModeDuration()
{
    items[1] = &sModeDuration;

    PageModesA::modeMeasureDuration.value = PageModesB::modeMeasureDuration.value;
    
    switch (PageModesB::modeMeasureDuration.value)
    {
    case ModeMeasureDuration::Ndt_1ns:
        PageModesA::InterpolateOn();
        PageModesA::DCycleOff();
        items[2] = nullptr;
        break;

    case ModeMeasureDuration::Dcycle:
    case ModeMeasureDuration::Phase:
        PageModesA::DCycleOn();
        PageModesA::InterpolateOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;

    case ModeMeasureDuration::Ndt:
    case ModeMeasureDuration::Ndt2:
        PageModesA::InterpolateOff();
        PageModesA::DCycleOff();
        items[2] = &sPeriodTimeLabels;
        items[3] = nullptr;
        break;
    }

    PageModesA::RelationOff();

    CurrentModeMeasureDuration::LoadToFPGA();
}

// ����� ������ ��������� ������������ ���������, ����������, ������������ ����������, �������� ���
DEF_SWITCH_5(sModeDuration,
    "�����", "��������� ������������",
    "ndt", "ndt/1��", "S-S", "D", "����",
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
        items[2] = &sNumberPeriods;
        items[3] = nullptr;
        break;

    case ModeMeasureCountPulse::StartStop:
        PageModesA::modeMeasureCountPulse.value = ModeMeasureCountPulse::StartStop;
        break;
    }

    PageModesA::RelationOff();
    PageModesA::InterpolateOff();
    PageModesA::DCycleOff();

    CurrentModeMeasureCountPulse::LoadToFPGA();
}

// ����� ������ ����� ���������
DEF_SWITCH_3(sModeCountPulse,
    "�����", "���� ����� ���������",
    "B(tA)", "B(TA)", "�����/����",
    PageModesB::modeMeasureCountPulse, PageModesB::OnChanged_ModeCountPulse
);


static void OnPress_TimeLabelsB()
{
    uint8 period = PageModesB::periodTimeLabels.value;

    PageModesA::periodTimeLabels.value = period;
    PageModesC::periodTimeLabels.value = period;

    FreqMeter::LoadPeriodTimeLabels();
}

// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesB::periodTimeLabels,  OnPress_TimeLabelsB
);


static void OnPress_TimeMeasure()
{
    uint8 time = PageModesB::timeMeasure.value;

    PageModesA::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;
    PageModesD::timeMeasure.value = time;

    static const uint8 periods[TimeMeasure::Count] =
    {
        NumberPeriods::_1,
        NumberPeriods::_10,
        NumberPeriods::_100,
        NumberPeriods::_1K,
        NumberPeriods::_10K,
        NumberPeriods::_100K,
        NumberPeriods::_1000K
    };

    uint8 period = periods[time];

    PageModesA::numberPeriods.value = period;
    PageModesB::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    FreqMeter::LoadTimeMeasure();
}

// ����� ������� ���������
DEF_SWITCH_7(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    PageModesB::timeMeasure, OnPress_TimeMeasure
);


static void OnPress_NumberPeriodsB()
{
    uint8 period = PageModesB::numberPeriods.value;

    PageModesA::numberPeriods.value = period;
    PageModesC::numberPeriods.value = period;

    static const uint8 times[NumberPeriods::Count] =
    {
        TimeMeasure::_1ms,
        TimeMeasure::_10ms,
        TimeMeasure::_100ms,
        TimeMeasure::_1s,
        TimeMeasure::_10s,
        TimeMeasure::_100s,
        TimeMeasure::_1000s
    };

    uint8 time = times[PageModesB::numberPeriods.value];

    PageModesA::timeMeasure.value = time;
    PageModesB::timeMeasure.value = time;
    PageModesC::timeMeasure.value = time;

    FreqMeter::LoadNumerPeriodsMeasure();
}

// ����� ����� ����������� �������� �������� �������
DEF_SWITCH_7(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    PageModesB::numberPeriods,
    OnPress_NumberPeriodsB
);



static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    &sTimeMeasure,
    nullptr
};


static void OnChanged_ModeTest()
{
    switch (PageModesB::typeMeasure.value)
    {
    case TypeMeasure::Frequency: PageModesB::OnChanged_ModeFrequency(); break;
    case TypeMeasure::Period:    PageModesB::OnChanged_ModePeriod();    break;
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


void PageModesB::ResetModeCurrentMeasure()
{
    switch (typeMeasure)
    {
    case TypeMeasure::Frequency:  modeMeasureFrequency.value = 0;     break;
    case TypeMeasure::Period:     modeMeasurePeriod.value = 0;        break;
    case TypeMeasure::Duration:   modeMeasureDuration.value = 0;      break;
    case TypeMeasure::CountPulse: modeMeasureCountPulse.value = 0;    break;
    }
}
