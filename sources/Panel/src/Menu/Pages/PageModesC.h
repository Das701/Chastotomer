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

/// ����� ��������� �������
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       // �������
        CA,         // f(C) / f(A)
        CB          // f(C) / f(B)
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const { return (value == Freq); }
    bool IsCA()        const { return (value == CA); }
    bool IsCB()        const { return (value == CB); }
};

/// ����� ����� ���������
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CtA,      // C(tA)
        CtB,      // C(tB)
        CTA,      // C(TA)
        CTB       // C(TB)
//        StartStop
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
};


class PageModesC
{
public:
    static Page *self;

    static void Init();
    // ��� ���������
    static TypeMeasureC typeMeasure;
    // ����� ��������� �������
    static ModeMeasureFrequencyC modeMeasureFrequency;
    // ����� ����� ���������
    static ModeMeasureCountPulseC modeMeasureCountPulse;
    // ������ ����� �������
    static PeriodTimeLabels periodTimeLabels;
    // ����� �����
    static TimeMeasure timeMeasure;
    // ����� �������� ���������
    static NumberPeriods numberPeriods;
};
