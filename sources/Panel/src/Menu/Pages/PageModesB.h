#pragma once
#include "Menu/MenuItems.h"


// ����� ��������� �������
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       // �������
        BA,         // f(B) / f(A)
        BC,         // f(B) / f(C)
        T_1,        // f = 1 / T
        Tachometer  // ��������
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency()  const { return (value == Freq); }
    bool IsBA()         const { return (value == BA); }
    bool IsBC()         const { return (value == BC);  }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
};

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


class PageModesB
{
public:
    static Page *self;
    static void Init();
    static void PressSetupB();
    // ��� ���������
    static TypeMeasureAB typeMeasure;
    // ����� ��������� �������
    static ModeMeasureFrequencyB modeMeasureFrequency;
    // ����� ��������� �������
    static ModeMeasurePeriod modeMeasurePeriod;
    // ����� ��������� ������������
    static ModeMeasureDuration modeMeasureDuration;
    // ����� ����� ���������
    static ModeMeasureCountPulseB modeMeasureCountPulse;
    // ������ ����� �������
    static PeriodTimeLabels periodTimeLabels;
    // ����� �����
    static TimeMeasure timeMeasure;
    // ����� �������� ���������
    static NumberPeriods numberPeriods;
};
