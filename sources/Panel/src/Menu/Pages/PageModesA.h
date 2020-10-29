#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasureA : public Enumeration
{
    enum E
    {
        Frequency,      ///< ��������� �������
        Period,         ///< ��������� �������
        Duration,       ///< ��������� ������������
        CountPulse,     ///< ������� ���������
        Count
    };

    explicit TypeMeasureA(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const  { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
    bool IsPeriod() const     { return (value == Period); }
    bool IsDuration() const   { return (value == Duration); }
};

/// ����� ��������� �������
struct ModeMeasureFrequency : public Enumeration
{
    enum E
    {
        Freq,       ///< �������
        AB,         ///< f(A) / f(B)
        AC,         ///< f(A) / f(C)
        T_1,        ///< f = 1 / T
        Tachometer, ///< ��������
        Comparator
    };

    explicit ModeMeasureFrequency(E v) : Enumeration((uint8)v) {};

    bool IsFrequency()  const { return (value == Freq); }
    bool IsAB()         const { return (value == AB); }
    bool IsAC()         const { return (value == AC); }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
    bool IsComparator() const { return (value == Comparator); }
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

    bool IsPeriod() const { return (value == Period); }
    bool IsF_1()    const { return (value == F_1); }
    static ModeMeasurePeriod &Current();
};

/// ����� ��������� ������������
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1ns,    ///< ndt_1��
        Ndt2,       ///< ndt2
        Dcycle,     ///< ����������
        Phase       ///< ����
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};

    bool Is_Ndt() const     { return (value == Ndt); }
    bool Is_Ndt_1ns() const { return (value == Ndt_1ns); }
    bool Is_Ndt2() const    { return (value == Ndt2); }
    bool Is_Dcycle() const  { return (value == Dcycle); }
    bool Is_Phase() const   { return (value == Phase); }
};

/// ����� ����� ���������
struct ModeMeasureCountPulse : public Enumeration
{
    enum E
    {
        ATC,        ///< A(tC)
        ATB,        ///< A(TB)
        StartStop
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
        T_8,        ///< 10-8
        Count
    };

    explicit PeriodTimeLabels(E v) : Enumeration((uint8)v) {};
    // ���������� PeriodTimeLabels ��� �������� ������
    static PeriodTimeLabels &Current();
    int ToZeros() const;
    bool IsT_3() const { return (value == T_3); }
    bool IsT_4() const { return (value == T_4); }
    bool IsT_5() const { return (value == T_5); }
    bool IsT_6() const { return (value == T_6); }
    bool IsT_7() const { return (value == T_7); }
    bool IsT_8() const { return (value == T_8); }
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
        _100s,       ///< 100s
        _1000s,
        Count
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};
    int ToMS() const;
    bool Is10ms() const { return (value == _10ms); }
    // ���������� TimeMeasure::E ��� �������� ������
    static TimeMeasure &Current();
};

/// ����� ��������
struct NumberPeriods : public Enumeration
{
    enum E
    {
        _1,         ///< 1
        _10,        ///< 10
        _100,       ///< 100
        _1K,        ///< 1K
        _10K,       ///< 10K
        _100K,      ///< 100K
        _1000K,
        Count
    };

    explicit NumberPeriods(E v) : Enumeration((uint8)v) {};
    int ToAbs() const;
    // ���������� NumberPeriods ��� �������� ������
    static NumberPeriods &Current();
};


class PageModesA
{
public:
    static Page *self;
    
    static void Init();
    static void InterpoleOn();
    static void InterpoleOff();
    static bool InterpoleCheck();
    static void DCycleOn();
    static void DCycleOff();
    static bool DCycleCheck();
    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();
    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();
    /// ��� ���������
    static TypeMeasureA typeMeasure;
    /// ����� ��������� �������
    static ModeMeasureFrequency modeMeasureFrequency;
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
