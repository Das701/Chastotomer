#include "defines.h"
#include "PageModes.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"


using namespace Display::Primitives;
using namespace Display;

extern Item *items[];

extern Button bPeriodTimeLabels;
extern Button bTimeMeasure;
extern Button bNumberPeriods;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MeasureFrequency::E modeFreq = MeasureFrequency::Freq;

MeasureFrequency::E PageModes::ModeMeasureFrequency()
{
    return modeFreq;
}

static void OnPress_Frequency()
{
    if (modeFreq == MeasureFrequency::AC || modeFreq == MeasureFrequency::T_1)
    {
        items[5] = &bNumberPeriods;
    }
    else
    {
        items[5] = &bTimeMeasure;
    }
}

/// ����� ������ ��������� �������, ��������� ������, "���������"
static Switch sFrequency("f", (uint8 *)&modeFreq, 5, &OnPress_Frequency);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static MeasurePeriod::E modePeriod = MeasurePeriod::Period;

MeasurePeriod::E PageModes::ModeMeasurePeriod()
{
    return modePeriod;
}

static void OnPress_Period()
{
    if (modePeriod == MeasurePeriod::Period)
    {
        items[4] = &bPeriodTimeLabels;
        items[5] = &bNumberPeriods;
    }
    else
    {
        items[4] = &bTimeMeasure;
        items[5] = nullptr;
    }
}

/// ����� ������ ��������� �������
static Switch sPeriod("T", (uint8 *)&modePeriod, 2, &OnPress_Period);

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ����� ������ ��������� ������������ ���������, ����������, ������������ ����������, �������� ���
static Button bDuration("t");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ����� ������ ����� ���������
static Button bCountPulse("����");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ����� ������� ����� �������
static Button bPeriodTimeLabels("�����");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ����� ������� ���������
static Button bTimeMeasure("�����");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// ����� ����� ����������� �������� �������� �������
static Button bNumberPeriods("N");

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


static Item *items[] =
{
    &sFrequency,
    &sPeriod,
    &bDuration,
    &bCountPulse,
    &bPeriodTimeLabels,
    &bTimeMeasure,
    nullptr
};

static Page pageModes(items);

Page *PageModes::self = &pageModes;
