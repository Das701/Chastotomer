#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageModesA.h"


struct TypeMeasureC : public Enumeration
{
    enum E
    {
        Frequency,      // ��������� �������
        CountPulse,     // ������� ���������
        Count
    };

    explicit TypeMeasureC(E v) : Enumeration((uint8)v) {};

    bool IsFrequency() const  { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
};

// ����� ��������� �������
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Frequency,  // �������
        CA,         // f(C) / f(A)
        CB          // f(C) / f(B)
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const { return (value == Frequency); }
    bool IsCA()        const { return (value == CA); }
    bool IsCB()        const { return (value == CB); }
};

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

    static void Init();

    // ����� ������ ���������
    static void ResetModeCurrentMeasure();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasureC           typeMeasure;           // ��� ���������
    static ModeMeasureFrequencyC  modeMeasureFrequency;  // ����� ��������� �������
    static ModeMeasureCountPulseC modeMeasureCountPulse; // ����� ����� ���������
    static PeriodTimeLabels       periodTimeLabels;      // ������ ����� �������
    static TimeMeasure            timeMeasure;           // ����� �����
    static NumberPeriods          numberPeriods;         // ����� �������� ���������
};
