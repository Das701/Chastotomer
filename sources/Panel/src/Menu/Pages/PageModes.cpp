#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[];
extern Switch sPeriodTimeLabels;
extern Switch sTimeMeasure;
extern Switch sNumberPeriods;


TypeMeasure             PageModes::typeMeasure(TypeMeasure::Frequency);
ModeMeasureFrequency    PageModes::modeMeasureFrequency(ModeMeasureFrequency::Freq);
ModeMeasurePeriod       PageModes::modeMeasurePeriod(ModeMeasurePeriod::Period);
ModeMeasureDuration     PageModes::modeMeasureDuration(ModeMeasureDuration::Ndt);
ModeMeasureCountPulse   PageModes::modeMeasureCountPulse(ModeMeasureCountPulse::Manual);

PeriodTimeLabels        PageModes::periodTimeLabels(PeriodTimeLabels::T_3);
NumberPeriods           PageModes::numberPeriods(NumberPeriods::_1);
TimeMeasure             PageModes::timeMeasure(TimeMeasure::_1ms);



//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModes::Init()
{
    static char *names[] = { "f", "T", "t", "����", nullptr };

    typeMeasure.names = names;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Frequency()
{
    if (PageModes::modeMeasureFrequency == ModeMeasureFrequency::AC || 
        PageModes::modeMeasureFrequency == ModeMeasureFrequency::T_1)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sTimeMeasure;
    }

    PageModes::typeMeasure.value = TypeMeasure::Frequency;
}

/// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_5(sFrequency,
    "f", "��������� �������",
    "�������", "f(A)/f(C)", "f(A)/f(B)", "f=1/T", "��������",
    PageModes::modeMeasureFrequency, OnPress_Frequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Period()
{
    if (PageModes::modeMeasurePeriod == ModeMeasurePeriod::Period)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else
    {
        items[4] = &sTimeMeasure;
        items[5] = nullptr;
    }

    PageModes::typeMeasure.value = TypeMeasure::Period;
}

/// ����� ������ ��������� �������
DEF_SWITCH_2(sPeriod,
    "T", "��������� �������",
    "������", "T=1/f",
    PageModes::modeMeasurePeriod, OnPress_Period
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Duration()
{
    if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1)
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = &sNumberPeriods;
    }
    else if (PageModes::modeMeasureDuration == ModeMeasureDuration::Ndt_1ns)
    {
        items[4] = nullptr;
        items[5] = nullptr;
    }
    else
    {
        items[4] = &sPeriodTimeLabels;
        items[5] = nullptr;
    }

    PageModes::typeMeasure.value = TypeMeasure::Duration;
}

/// ����� ������ ��������� ������������ ���������, ����������, ������������ ����������, �������� ���
DEF_SWITCH_6(sDuration,
    "t", "��������� ������������",
    "ndt", "<ndt>", "ndt/1��", "��������", "1/S", "����",
    PageModes::modeMeasureDuration,
    OnPress_Duration
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_CountPulse()
{
    if (PageModes::modeMeasureCountPulse == ModeMeasureCountPulse::ATC_1)
    {

        items[4] = &sNumberPeriods;
        items[5] = nullptr;
    }
    else 
    {
        items[4] = nullptr;
        items[5] = nullptr;
    }

    PageModes::typeMeasure.value = TypeMeasure::CountPulse;
}

/// ����� ������ ����� ���������
DEF_SWITCH_3(sCountPulse,
    "����", "���� ����� ���������",
    "����.", "�(tC)", "�(TC)",
    PageModes::modeMeasureCountPulse,
    OnPress_CountPulse
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabels()
{
}

/// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabels,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModes::periodTimeLabels,
    OnPress_TimeLabels
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasure()
{
}

/// ����� ������� ���������
DEF_SWITCH_6(sTimeMeasure,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModes::timeMeasure,
    OnPress_TimeMeasure
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriods()
{
}

/// ����� ����� ����������� �������� �������� �������
DEF_SWITCH_6(sNumberPeriods,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K",
    PageModes::numberPeriods,
    OnPress_NumberPeriods
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[] =
{
    &sFrequency,
    &sPeriod,
    &sDuration,
    &sCountPulse,
    &sPeriodTimeLabels,
    &sTimeMeasure,
    nullptr
};

static Page pageModes(items);

Page *PageModes::self = &pageModes;
