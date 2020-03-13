#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasure : public Enumeration
{
    enum E
    {
        Frequency,      ///< ��������� �������
        Period,         ///< ��������� �������
        Duration,       ///< ��������� ������������
        CountPulse,     ///< ������� ���������
        Count
    };

    explicit TypeMeasure(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� ������� A
struct ModeMeasureFrequency : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    };

    explicit ModeMeasureFrequency(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� ������� C
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        CA,         ///< f(C) / f(A)
        CB,         ///< f(C) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� ������� B
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        BA,         ///< f(B) / f(A)
        BC,         ///< f(B) / f(C)
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� �������
struct ModeMeasurePeriod : public Enumeration
{
    enum E
    {
        Period,     ///< ������
        F_1         ///< T = 1 / f
    };

    explicit ModeMeasurePeriod(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� ������������
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1,      ///< <ndt>
        Ndt_1ns,    ///< ndt_1��
        Interval,   ///< ��������
        S_1,        ///< 1 / S
        Phase       ///< ����
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};
};

/// ����� ����� ���������
struct ModeMeasureCountPulse : public Enumeration
{
    enum E
    {
        Manual,     ///< ����.
        ATC,        ///< A(tC)
        ATC_1       ///< A(TC)
    };

    explicit ModeMeasureCountPulse(E v) : Enumeration((uint8)v) {};
};

/// ������ ����� �������
struct PeriodTimeLabels : public Enumeration
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

    explicit PeriodTimeLabels(E v) : Enumeration((uint8)v) {};
};

/// ����� �����
struct TimeMeasure : public Enumeration
{
    enum E
    {
        _1ms,       ///< 1ms
        _10ms,      ///< 10ms
        _100ms,     ///< 100ms
        _1s,        ///< 1s
        _10s,       ///< 10s
        _100s       ///< 100s
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������
struct NumberPeriods : public Enumeration
{
    enum E
    {
        _1,       ///< 1
        _10,      ///< 10
        _100,     ///< 100
        _1K,      ///< 1K
        _10K,     ///< 10K
        _100K     ///< 100K
    };

    explicit NumberPeriods(E v) : Enumeration((uint8)v) {};
};


class PageModes
{
public:
    static Page *self;

    static void Init();
    /// ��� ���������
    static TypeMeasure typeMeasure;
    /// ����� ��������� �������
    static ModeMeasureFrequency modeMeasureFrequency;
    /// ����� ��������� ������� C
    static ModeMeasureFrequencyC modeMeasureFrequencyC;
    /// ����� ��������� ������� B
    static ModeMeasureFrequencyB modeMeasureFrequencyB;
    /// ����� ��������� �������
    static ModeMeasurePeriod modeMeasurePeriod;
    /// ����� ��������� ������������
    static ModeMeasureDuration modeMeasureDuration;
    /// ����� ����� ���������
    static ModeMeasureCountPulse modeMeasureCountPulse;
    /// ������ ����� �������
    static PeriodTimeLabels periodTimeLabels;
    /// ����� �����
    static TimeMeasure timeMeasure;
    /// ����� �������� ���������
    static NumberPeriods numberPeriods;
};
