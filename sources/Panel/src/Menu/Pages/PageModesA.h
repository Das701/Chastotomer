#pragma once
#include "Menu/MenuItems.h"


struct CurrentTypeMeasure
{
    static bool IsFrequency();
    static bool IsPeriod();
    static bool IsDuration();
    static bool IsCountPulse();
};

struct CurrentModeMeasureFrequency
{
    static bool IsTachometer();
    static bool IsComparator();
    static bool IsT_1();
    static bool Is_RatioAC_or_RatioBC();
    static bool Is_RatioAB_or_RatioBA();
    static bool Is_RatioCA_or_RatioCB();
};


struct CurrentModeMeasureCountPulse
{
    static bool IsBig_T();
};


struct TypeMeasureAB : public Enumeration
{
    enum E
    {
        Frequency,      // ��������� �������
        Period,         // ��������� �������
        Duration,       // ��������� ������������
        CountPulse,     // ������� ���������
        Count
    };

    explicit TypeMeasureAB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const  { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
    bool IsPeriod() const     { return (value == Period); }
    bool IsDuration() const   { return (value == Duration); }
};

// ����� ��������� �������
struct ModeMeasureFrequencyA : public Enumeration
{
    enum E
    {
        Frequency,  // �������
        RatioAB,    // f(A) / f(B)
        RatioAC,    // f(A) / f(C)
        T_1,        // f = 1 / T
        Tachometer, // ��������
        Comparator
    };

    explicit ModeMeasureFrequencyA(E v) : Enumeration((uint8)v) {};

    bool IsFrequency()  const { return (value == Frequency); }
    bool IsRatioAB()    const { return (value == RatioAB); }
    bool IsRatioAC()    const { return (value == RatioAC); }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
    bool IsComparator() const { return (value == Comparator); }
};

// ����� ��������� �������
struct ModeMeasurePeriod : public Enumeration
{
    enum E
    {
        Period,     // ������
        F_1         // T = 1 / f
    };

    explicit ModeMeasurePeriod(E v) : Enumeration((uint8)v) {};

    bool IsPeriod() const { return (value == Period); }
    bool IsF_1()    const { return (value == F_1); }
    static ModeMeasurePeriod &Current();
};

// ����� ��������� ������������
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        // ndt
        Ndt_1ns,    // ndt_1��
        Ndt2,       // ndt2
        Dcycle,     // ����������
        Phase       // ����
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};

    bool Is_Ndt() const     { return (value == Ndt); }
    bool Is_Ndt_1ns() const { return (value == Ndt_1ns); }
    bool Is_Ndt2() const    { return (value == Ndt2); }
    bool Is_Dcycle() const  { return (value == Dcycle); }
    bool Is_Phase() const   { return (value == Phase); }

    static ModeMeasureDuration &Current();
};

// ����� ����� ���������
struct ModeMeasureCountPulseA : public Enumeration
{
    enum E
    {
        AtC,        // A(tC)
        ATB,        // A(TB)
        StartStop
    };

    explicit ModeMeasureCountPulseA(E v) : Enumeration((uint8)v) {};
};

// ������ ����� �������
struct PeriodTimeLabels : public Enumeration
{
    enum E
    {
        T_3,        // 10-3
        T_4,        // 10-4
        T_5,        // 10-5
        T_6,        // 10-6
        T_7,        // 10-7
        T_8,        // 10-8
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

// ����� �����
struct TimeMeasure : public Enumeration
{
    enum E
    {
        _1ms,       // 1ms
        _10ms,      // 10ms
        _100ms,     // 100ms
        _1s,        // 1s
        _10s,       // 10s
        _100s,       // 100s
        _1000s,
        Count
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};
    int ToMS() const;
    bool Is10ms() const { return (value == _10ms); }
    // ���������� TimeMeasure::E ��� �������� ������
    static TimeMeasure &Current();
};

// ����� ��������
struct NumberPeriods : public Enumeration
{
    enum E
    {
        _1,         // 1
        _10,        // 10
        _100,       // 100
        _1K,        // 1K
        _10K,       // 10K
        _100K,      // 100K
        _1000K,
        Count
    };

    explicit NumberPeriods(E v) : Enumeration((uint8)v) {};
    int ToAbs() const;
    // ���������� NumberPeriods ��� �������� ������
    static NumberPeriods &Current();
};


// ����� ������� ����� ��� ���� ������� ������� �������
struct PageModes
{
    // ����� ������ �������� ���������
    static void ResetModeCurrentMeasure();
};


struct PageModesA
{
    static Page *self;
    
    static void Init();

    static void InterpolateOn();
    static void InterpolateOff();
    static bool InterpolateCheck();

    static void DCycleOn();
    static void DCycleOff();
    static bool DCycleCheck();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // ����� ������ ���������
    static void ResetModeCurrentMeasure();

    // ���������� ��� ���������� ���� ���������
    static void OnChanged_TypeMeasure();

    // ���������� ��� ��������� ������� ���������
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasureAB            typeMeasure;                // ��� ���������
    static ModeMeasureFrequencyA    modeMeasureFrequency;       // ����� ��������� �������
    static ModeMeasurePeriod        modeMeasurePeriod;          // ����� ��������� �������
    static ModeMeasureDuration      modeMeasureDuration;        // ����� ��������� ������������
    static ModeMeasureCountPulseA   modeMeasureCountPulse;      // ����� ����� ���������
    static PeriodTimeLabels         periodTimeLabels;           // ������ ����� �������
    static TimeMeasure              timeMeasure;                // ����� �����
    static NumberPeriods            numberPeriods;              // ����� �������� ���������
};
