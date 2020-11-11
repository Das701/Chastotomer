#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"


class PageModesA
{
friend struct PageModes;

public:

    static Page *self;
    
    static void InterpolateOn();
    static void InterpolateOff();
    static bool InterpolateCheck();

    static void DCycleOn();
    static void DCycleOff();
    static bool DCycleCheck();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // ����� ������ ���������
    static void ResetModeCurrentMeasure();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasure           typeMeasure;                // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;       // ����� ��������� �������
    static ModeMeasurePeriod     modeMeasurePeriod;          // ����� ��������� �������
    static ModeMeasureDuration   modeMeasureDuration;        // ����� ��������� ������������
    static ModeMeasureCountPulse modeMeasureCountPulse;      // ����� ����� ���������
    static PeriodTimeLabels      periodTimeLabels;           // ������ ����� �������
    static TimeMeasure           timeMeasure;                // ����� �����
    static NumberPeriods         numberPeriods;              // ����� �������� ���������
};
