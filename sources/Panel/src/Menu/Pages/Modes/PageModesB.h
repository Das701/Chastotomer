#pragma once
#include "Menu/MenuItems.h"


// ����� ����� ���������
struct ModeMeasureCountPulseB : public Enumeration
{
    enum E
    {
        BtA,        // B(tA)
        BTA,        // B(TA)
        StartStop
    };

    explicit ModeMeasureCountPulseB(E v) : Enumeration((uint8)v) {};
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
    
    static TypeMeasure            typeMeasure;           // ��� ���������
    static ModeMeasureFrequency   modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasurePeriod      modeMeasurePeriod;     // ����� ��������� �������
    static ModeMeasureDuration    modeMeasureDuration;   // ����� ��������� ������������
    static ModeMeasureCountPulseB modeMeasureCountPulse; // ����� ����� ���������
    static PeriodTimeLabels       periodTimeLabels;      // ������ ����� �������
    static TimeMeasure            timeMeasure;           // ����� �����
    static NumberPeriods          numberPeriods;         // ����� �������� ���������
};
