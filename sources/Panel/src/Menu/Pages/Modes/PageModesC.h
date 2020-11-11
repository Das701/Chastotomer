#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"


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
    static PeriodTimeLabels      periodTimeLabels;      // ������ ����� �������
    static TimeMeasure           timeMeasure;           // ����� �����
    static NumberPeriods         numberPeriods;         // ����� �������� ���������
};
