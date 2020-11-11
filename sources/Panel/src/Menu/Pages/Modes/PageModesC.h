#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"


// ����� ����� ���������
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CtA,      // C(tA)
        CtB,      // C(tB)
        CTA,      // C(TA)
        CTB       // C(TB)
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
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
    
    static TypeMeasure            typeMeasure;           // ��� ���������
    static ModeMeasureFrequency   modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasureCountPulseC modeMeasureCountPulse; // ����� ����� ���������
    static PeriodTimeLabels       periodTimeLabels;      // ������ ����� �������
    static TimeMeasure            timeMeasure;           // ����� �����
    static NumberPeriods          numberPeriods;         // ����� �������� ���������
};
