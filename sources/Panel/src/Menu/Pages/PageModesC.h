#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageModesA.h"


struct TypeMeasureC : public Enumeration
{
    enum E
    {
        Frequency,      ///< ��������� �������
        CountPulse,     ///< ������� ���������
        Count
    };

    explicit TypeMeasureC(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� �������
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        CA,         ///< f(C) / f(A)
        CB          ///< f(C) / f(B)
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
};

/// ����� ����� ���������
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CTA,        ///< C(tA)
        CTB,        ///< C(tB)
        CTA_1,      ///< C(TA)
        CTB_1       ///< C(TB)
//        StartStop
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
};

/// ������ ����� �������
struct PeriodTimeLabelsC : public Enumeration
{
    enum E
    {
        T_3,        ///< 10-3
        T_4,        ///< 10-4
        T_5,        ///< 10-5
        T_6,        ///< 10-6
        T_7,        ///< 10-7
        T_8         ///< 10-8
    };

    explicit PeriodTimeLabelsC(E v) : Enumeration((uint8)v) {};
};


class PageModesC
{
public:
    static Page *self;

    static void Init();
    /// ��� ���������
    static TypeMeasureC typeMeasureC;
    /// ����� ��������� �������
    static ModeMeasureFrequencyC modeMeasureFrequencyC;
    /// ����� ����� ���������
    static ModeMeasureCountPulseC modeMeasureCountPulseC;
    /// ������ ����� �������
    static PeriodTimeLabelsC periodTimeLabelsC;
    /// ����� �����
    static TimeMeasure timeMeasure;
    /// ����� �������� ���������
    static NumberPeriods numberPeriods;
};