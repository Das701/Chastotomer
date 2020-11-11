#pragma once
#include "Menu/MenuItems.h"


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
    static PeriodTimeLabels      periodTimeLabels;      // ������ ����� �������
    static TimeMeasure           timeMeasure;           // ����� �����
    static NumberPeriods         numberPeriods;         // ����� �������� ���������
};
