#include "defines.h"
#include "PageModesB.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "FreqMeter/FreqMeter.h"
#include "Menu/Pages/PageChannelA.h"
#include "Menu/Pages/PageChannelB.h"
#include "Menu/Pages/PageChannelC.h"
#include "Menu/Pages/PageChannelD.h"

using namespace Display::Primitives;
using namespace Display;

extern Item *items[7];
extern Switch sModeFrequencyB;
extern Switch sModePeriodB;
extern Switch sModeDurationB;
extern Switch sModeCountPulseB;
extern Switch sPeriodTimeLabelsB;
extern Switch sTimeMeasureB;
extern Switch sNumberPeriodsB;


TypeMeasureB             PageModesB::typeMeasureB(TypeMeasureB::Frequency);
ModeMeasureFrequencyB    PageModesB::modeMeasureFrequencyB(ModeMeasureFrequencyB::Freq);
ModeMeasurePeriodB       PageModesB::modeMeasurePeriodB(ModeMeasurePeriodB::Period);
ModeMeasureDurationB     PageModesB::modeMeasureDurationB(ModeMeasureDurationB::Ndt);
ModeMeasureCountPulseB   PageModesB::modeMeasureCountPulseB(ModeMeasureCountPulseB::Manual);

PeriodTimeLabelsB        PageModesB::periodTimeLabelsB(PeriodTimeLabelsB::T_8);
NumberPeriodsB           PageModesB::numberPeriodsB(NumberPeriodsB::_1);
TimeMeasureB             PageModesB::timeMeasureB(TimeMeasureB::_1ms);

/// �������� ������ ���������� �� �����, ������� � i-�� �����
static void ClearItems(int i);

static void OnPress_ModeFrequencyB();

static void OnPress_ModePeriodB();

static void OnPress_ModeDurationB();

static void OnPress_ModeCountPulseB();


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModesB::Init()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClearItems(int num)
{
    for (int i = num; i < 7; i++)
    {
        items[i] = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TypeMeasureB()
{
        switch (PageModesB::typeMeasureB.value)
        {
        case TypeMeasureB::Frequency:
            OnPress_ModeFrequencyB();
            break;
    
        case TypeMeasureB::Period:
            OnPress_ModePeriodB();
            break;
    
        case TypeMeasureB::Duration:
            OnPress_ModeDurationB();
            break;
    
        case TypeMeasureB::CountPulse:
            OnPress_ModeCountPulseB();
            break;
        }        
}

DEF_SWITCH_4(sTypeMeasureB,
    "��� ���.", "����� ���������",
    "�������", "������", "����.", "��. ���.",
    PageModesB::typeMeasureB, OnPress_TypeMeasureB
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequencyB()
{
    ClearItems(2);
    items[2] = &sPeriodTimeLabelsB;   
    items[1] = &sModeFrequencyB;
    
    if (PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::BC || 
        PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::T_1)
    {
        items[3] = &sNumberPeriodsB;
    }
    else if(PageModesB::modeMeasureFrequencyB == ModeMeasureFrequencyB::Tachometer)
    {
        items[3] = nullptr;
    }
    else
    {
        items[3] = &sTimeMeasureB;
    }      
    FreqMeter::LoadModeMeasureFrequency();
}

/// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_5(sModeFrequencyB,
    "����� ���.", "��������� �������",
    "�������", "f(B)/f(A)", "f(B)/f(C )", "f=1/T", "��������",
    PageModesB::modeMeasureFrequencyB, OnPress_ModeFrequencyB);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModePeriodB()
{
    ClearItems(2);

    items[1] = &sModePeriodB;

    if (PageModesB::modeMeasurePeriodB == ModeMeasurePeriodB::Period)
    {
        items[2] = &sPeriodTimeLabelsB;
        items[3] = &sNumberPeriodsB;
    }
    else
    {
        items[2] = &sTimeMeasureB;
    }

    FreqMeter::LoadModeMeasurePeriod();
}

/// ����� ������ ��������� �������
DEF_SWITCH_2(sModePeriodB,
    "����� ���.", "��������� �������",
    "������", "T=1/f",
    PageModesB::modeMeasurePeriodB, OnPress_ModePeriodB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeDurationB()
{
    ClearItems(2);

    items[1] = &sModeDurationB;

    if (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Ndt_1ns)
    {
        PageModes::InterpoleOn();
    }
    else
    {
        items[2] = &sPeriodTimeLabelsB;
    }
    if (PageModesB::modeMeasureDurationB == ModeMeasureDurationB::Dcycle)
    {
        PageModes::DCycleOn();
    }
    FreqMeter::LoadModeMeasureDuration();
}

/// ����� ������ ��������� ������������ ���������, ����������, ������������ ����������, �������� ���
DEF_SWITCH_4(sModeDurationB,
    "����� ���.", "��������� ������������",
    "ndt", "ndt/1��", "ndt2", "����������",
    PageModesB::modeMeasureDurationB, OnPress_ModeDurationB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseB()
{
    ClearItems(2);

    items[1] = &sModeCountPulseB;

    if (PageModesB::modeMeasureCountPulseB == ModeMeasureCountPulseB::BTA_1)
    {
        items[2] = &sNumberPeriodsB;
    }

    FreqMeter::LoadModeMeasureCountPulse();
}

/// ����� ������ ����� ���������
DEF_SWITCH_3(sModeCountPulseB,
    "����� ���.", "���� ����� ���������",
    "����.", "B(tA)", "B(TA)",
    PageModesB::modeMeasureCountPulseB, OnPress_ModeCountPulseB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsB()
{
    FreqMeter::LoadPeriodTimeLabels();
}

/// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabelsB,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesB::periodTimeLabelsB,  OnPress_TimeLabelsB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureB()
{
    FreqMeter::LoadTimeMeasure();
}

/// ����� ������� ���������
DEF_SWITCH_6(sTimeMeasureB,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModesB::timeMeasureB, OnPress_TimeMeasureB
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriodsB()
{
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// ����� ����� ����������� �������� �������� �������
DEF_SWITCH_6(sNumberPeriodsB,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K",
    PageModesB::numberPeriodsB,
    OnPress_NumberPeriodsB
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[7] =
{
    &sTypeMeasureB,
    &sModeFrequencyB,
    &sPeriodTimeLabelsB,
    &sTimeMeasureB,
    nullptr,
    nullptr,
    nullptr
};

static Page pageModesB(items);

Page *PageModesB::self = &pageModesB;
