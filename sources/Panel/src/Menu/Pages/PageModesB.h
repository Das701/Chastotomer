#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasureB : public Enumeration
{
    enum E
    {
        Frequency,      ///< ��������� �������
        Period,         ///< ��������� �������
        Duration,       ///< ��������� ������������
        CountPulse,     ///< ������� ���������
        Count
    };

    explicit TypeMeasureB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const  { return (value == Frequency); }
    bool IsPeriod() const     { return (value == Period); }
    bool IsDuration() const   { return (value == Duration); }
    bool IsCountPulse() const { return (value == CountPulse); }
};

/// ����� ��������� �������
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        BA,         ///< f(B) / f(A)
        BC,         ///< f(B) / f(C)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� �������
struct ModeMeasurePeriodB : public Enumeration
{
    enum E
    {
        Period,     ///< ������
        F_1         ///< T = 1 / f
    };

    explicit ModeMeasurePeriodB(E v) : Enumeration((uint8)v) {};
};

/// ����� ��������� ������������
struct ModeMeasureDurationB : public Enumeration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1ns,    ///< ndt_1��
        Ndt2,       ///< ndt2
        Dcycle,     ///< ����������
        Phase       ///< ����
    };

    explicit ModeMeasureDurationB(E v) : Enumeration((uint8)v) {};
};

/// ����� ����� ���������
struct ModeMeasureCountPulseB : public Enumeration
{
    enum E
    {
        BTA,        ///< B(tA)
        BTA_1,      ///< B(TA)
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
    /// ��� ���������
    static TypeMeasureB typeMeasure;
    /// ����� ��������� �������
    static ModeMeasureFrequencyB modeMeasureFrequency;
    /// ����� ��������� �������
    static ModeMeasurePeriodB modeMeasurePeriod;
    /// ����� ��������� ������������
    static ModeMeasureDurationB modeMeasureDuration;
    /// ����� ����� ���������
    static ModeMeasureCountPulseB modeMeasureCountPulse;
    /// ������ ����� �������
    static PeriodTimeLabels periodTimeLabels;
    /// ����� �����
    static TimeMeasure timeMeasure;
    /// ����� �������� ���������
    static NumberPeriods numberPeriods;
};
