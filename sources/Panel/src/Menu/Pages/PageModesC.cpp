#include "defines.h"
#include "PageModesC.h"
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
extern Switch sModeFrequencyC;
extern Switch sModeCountPulseC;
extern Switch sPeriodTimeLabelsC;
extern Switch sTimeMeasureC;
extern Switch sNumberPeriodsC;


TypeMeasureC             PageModesC::typeMeasureC(TypeMeasureC::Frequency);
ModeMeasureFrequencyC    PageModesC::modeMeasureFrequencyC(ModeMeasureFrequencyC::Freq);
ModeMeasureCountPulseC   PageModesC::modeMeasureCountPulseC(ModeMeasureCountPulseC::CTA);
PeriodTimeLabelsC        PageModesC::periodTimeLabelsC(PeriodTimeLabelsC::T_8);
NumberPeriodsC           PageModesC::numberPeriodsC(NumberPeriodsC::_1);
TimeMeasureC             PageModesC::timeMeasureC(TimeMeasureC::_1ms);

/// �������� ������ ���������� �� �����, ������� � i-�� �����
static void ClearItems(int i);

static void OnPress_ModeFrequencyC();

static void OnPress_ModeCountPulseC();


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageModesC::Init()
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
static void OnPress_TypeMeasureC()
{
        switch (PageModesC::typeMeasureC.value)
        {
        case TypeMeasureC::Frequency:
            OnPress_ModeFrequencyC();
            break;
        case TypeMeasureC::CountPulse:
            OnPress_ModeCountPulseC();
            break;
        }        
}

DEF_SWITCH_2(sTypeMeasureC,
    "��� ���.", "����� ���������",
    "�������", "��. ���.",
    PageModesC::typeMeasureC, OnPress_TypeMeasureC
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeFrequencyC()
{
    ClearItems(2);
    items[2] = &sPeriodTimeLabelsC;   
    items[1] = &sModeFrequencyC;
    
    if (PageModesC::modeMeasureFrequencyC == ModeMeasureFrequencyC::CB)
    {
        items[3] = &sNumberPeriodsC;
    }
    else
    {
        items[3] = &sTimeMeasureC;
    }      
    FreqMeter::LoadModeMeasureFrequency();
}

/// ����� ������ ��������� �������, ��������� ������, "���������"
DEF_SWITCH_3(sModeFrequencyC,
    "����� ���.", "��������� �������",
    "�������", "f(C )/f(A)", "f(C )/f(B)",
    PageModesC::modeMeasureFrequencyC, OnPress_ModeFrequencyC);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ModeCountPulseC()
{
    ClearItems(2);

    items[1] = &sModeCountPulseC;

    if (PageModesC::modeMeasureCountPulseC == ModeMeasureCountPulseC::CTA_1)
    {
        items[2] = &sNumberPeriodsC;
    }

    FreqMeter::LoadModeMeasureCountPulse();
}

/// ����� ������ ����� ���������
DEF_SWITCH_4(sModeCountPulseC,
    "����� ���.", "���� ����� ���������",
    "C(tA)", "C(tB)", "C(TA)", "C(TB)",
    PageModesC::modeMeasureCountPulseC, OnPress_ModeCountPulseC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeLabelsC()
{
    FreqMeter::LoadPeriodTimeLabels();
}

/// ����� ������� ����� �������
DEF_SWITCH_6(sPeriodTimeLabelsC,
    "�����", "������������ ��������� �����",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    PageModesC::periodTimeLabelsC,  OnPress_TimeLabelsC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_TimeMeasureC()
{
    FreqMeter::LoadTimeMeasure();
}

/// ����� ������� ���������
DEF_SWITCH_6(sTimeMeasureC,
    "�����", "����� �����",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    PageModesC::timeMeasureC, OnPress_TimeMeasureC
);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_NumberPeriodsC()
{
    FreqMeter::LoadNumerPeriodsMeasure();
}

/// ����� ����� ����������� �������� �������� �������
DEF_SWITCH_6(sNumberPeriodsC,
    "N", "����� �������� ���������",
    "1", "10", "100", "1K", "10K", "100K",
    PageModesC::numberPeriodsC,
    OnPress_NumberPeriodsC
);


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Item *items[7] =
{
    &sTypeMeasureC,
    &sModeFrequencyC,
    &sPeriodTimeLabelsC,
    &sTimeMeasureC,
    nullptr,
    nullptr,
    nullptr
};

static Page pageModesC(items);

Page *PageModesC::self = &pageModesC;
