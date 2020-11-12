#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"


class PageModesA
{
friend struct PageModes;

public:

    static Page *self;
    
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
    static PeriodTimeLabels      timeLabels;                 // ������ ����� �������
    static TimeMeasure           timeMeasure;                // ����� �����
    static NumberPeriods         numberPeriods;              // ����� �������� ���������
};


struct PageModesB
{
    static Page *self;

    static void PressSetup();

    // ����� ������ ���������
    static void ResetModeCurrentMeasure();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasurePeriod     modeMeasurePeriod;     // ����� ��������� �������
    static ModeMeasureDuration   modeMeasureDuration;   // ����� ��������� ������������
    static ModeMeasureCountPulse modeMeasureCountPulse; // ����� ����� ���������
    static PeriodTimeLabels      timeLabels;            // ������ ����� �������
    static TimeMeasure           timeMeasure;           // ����� �����
    static NumberPeriods         numberPeriods;         // ����� �������� ���������
};


struct PageModesC
{
    static Page *self;

    // ����� ������ ���������
    static void ResetModeCurrentMeasure();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // ��� ���������
    static ModeMeasureFrequency  modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasureCountPulse modeMeasureCountPulse; // ����� ����� ���������
    static PeriodTimeLabels      timeLabels;            // ������ ����� �������
    static TimeMeasure           timeMeasure;           // ����� �����
    static NumberPeriods         numberPeriods;         // ����� �������� ���������
};



class PageModesD
{
public:
    static Page *self;

    static TypeMeasure          typeMeasure;
    static ModeMeasureFrequency modeMeasureFrequency;
    static TimeMeasure          timeMeasure;           // ����� �����
};
