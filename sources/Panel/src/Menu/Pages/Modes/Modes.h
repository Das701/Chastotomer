#pragma once
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"


struct CurrentModeMeasure
{
    static Enumeration &ToEnumeration();
    static void Draw(int x, int y);
    static void DrawParameters(int x, int y);
};


// ����� ������� ����� ��� ���� ������� ������� �������
struct PageModes
{
    // ����� ������ �������� ���������
    static void ResetModeCurrentMeasure();
};


struct TypeMeasure : public Enumeration
{
    enum E
    {
        /* A B C D */  Frequency,      // ��������� �������
        /* A B     */  Period,         // ��������� �������
        /* A B     */  Duration,       // ��������� ������������
        /* A B C   */  CountPulse,     // ������� ���������
                       Count
    };

    explicit TypeMeasure(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};
    bool IsFrequency() const { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
    bool IsPeriod() const { return (value == Period); }
    bool IsDuration() const { return (value == Duration); }
    void Draw(int x, int y) const;

    static TypeMeasure &Current();
};

// ����� ��������� �������
struct ModeMeasureFrequency : public Enumeration
{
    enum E
    {
        /* A B C D */  Frequency,  // �������
        /* A B     */  T_1,        // f = 1 / T
        /* A       */  RatioAB,    // f(A) / f(B)
        /* A       */  RatioAC,    // f(A) / f(C)
        /*   B     */  RatioBA,    // f(B) / f(A)
        /*   B     */  RatioBC,    // f(B) / f(C)
        /*     C   */  RatioCA,    // f(C) / f(A)
        /*     C   */  RatioCB,    // f(C) / f(B)
        /* A B     */  Tachometer, // ��������
        /* A       */  Comparator,
                       Count
    };

    explicit ModeMeasureFrequency(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};

    bool IsFrequency()  const { return (value == Frequency); }
    bool IsRatioAB()    const { return (value == RatioAB); }
    bool IsRatioAC()    const { return (value == RatioAC); }
    bool IsRatioBA()    const { return (value == RatioBA); }
    bool IsRatioBC()    const { return (value == RatioBC); }
    bool IsRatioCA()    const { return (value == RatioCA); }
    bool IsRatioCB()    const { return (value == RatioCB); }
    bool IsRatio()      const;
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
    bool IsComparator() const { return (value == Comparator); }

    static ModeMeasureFrequency &Current();

    // ��������� ������� ����� ��������� ������� � ����������
    static void LoadToFPGA();
};


// ����� ����� ���������
struct ModeMeasureCountPulse : public Enumeration
{
    enum E
    {
        /* A     */ AtC,      // A(tC)
        /* A     */ ATB,      // A(TB)
        /*   B   */ BtA,      // B(tA)
        /*   B   */ BTA,      // B(TA)
        /*     C */ CtA,      // C(tA)
        /*     C */ CtB,      // C(tB)
        /*     C */ CTA,      // C(TA)
        /*     C */ CTB,      // C(TB)
        /* A B   */ StartStop,
        Count
    };

    explicit ModeMeasureCountPulse(E v, const bool *correct, int numStates) : Enumeration((uint8)v, correct, numStates) {};


    // ���������� true, ���� ������� ����� ��������� �� ������� (������� T � ��������)
    bool IsFromPeriod() const;

    static ModeMeasureCountPulse &Current();
    static void LoadToFPGA();
};


// ����� ��������� �������
struct ModeMeasurePeriod : public Enumeration
{
    enum E
    {
        Period,     // ������
        F_1,        // T = 1 / f
        Count
    };

    explicit ModeMeasurePeriod(E v) : Enumeration((uint8)v) {};

    bool IsPeriod() const { return (value == Period); }
    bool IsF_1()    const { return (value == F_1); }

    static ModeMeasurePeriod &Current();
    static void LoadToFPGA();
};

// ����� ��������� ������������
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        // ndt
        Ndt_1ns,    // ndt_1��
        Ndt2,       // ndt2
        FillFactor, // ���������� ����������
        Phase,      // ����
        Count
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};

    bool Is_Ndt() const        { return (value == Ndt); }
    bool Is_Ndt_1ns() const    { return (value == Ndt_1ns); }
    bool Is_Ndt2() const       { return (value == Ndt2); }
    bool Is_FillFactor() const { return (value == FillFactor); }
    bool Is_Phase() const      { return (value == Phase); }

    static ModeMeasureDuration &Current();
    static void LoadToFPGA();
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
        _100s,      // 100s
        _1000s,
        Count
    };

    explicit TimeMeasure(E v) : Enumeration((uint8)v) {};
    int ToMS() const;
    bool Is10ms() const { return (value == _10ms); }
    // ���������� TimeMeasure::E ��� �������� ������
    static TimeMeasure &Current();

    struct ProgressBar
    {
        // ���������� true, ���� ��������� ���������
        static bool IsDrawable();

        // ���������� ��������-��� ���������� �� ������� �������
        static void Draw(int x, int y);

        static void Reset() { timeStart = TIME_MS; };

    private:
        static uint timeStart;
    };
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
