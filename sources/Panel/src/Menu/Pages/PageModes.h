#pragma once
#include "Menu/MenuItems.h"

//struct Enum
//{
//    int value;
//    char *names[];
//    virtual char *ToText();
//};

struct TypeMeasure 
{
    enum E
    {
        Frequency,      ///< ��������� �������
        Period,         ///< ��������� �������
        Duration,       ///< ��������� ������������
        CountPulse      ///< ������� ���������
    } value;

    explicit TypeMeasure(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

struct Hint
{
    enum E
    {
        Frequency,      ///< ��������� �������
        Period,         ///< ��������� �������
        Duration,       ///< ��������� ������������
        CountPulse,     ///< ������� ���������
        TimeLabels,
        TimeMeasure,
        NumberPeriods   
    } value;

    explicit Hint(E v) : value(v) {};
    operator int() { return (int)value; };
};

/// ����� ��������� �������
struct ModeMeasureFrequency
{
    enum E
    {
        Freq,       ///< �������
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    } value;

    explicit ModeMeasureFrequency(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ����� ��������� �������
struct ModeMeasurePeriod
{
    enum E
    {
        Period,     ///< ������
        F_1         ///< T = 1 / f
    } value;

    explicit ModeMeasurePeriod(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ����� ��������� ������������
struct ModeMeasureDuration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1,      ///< <ndt>
        Ndt_1ns,    ///< ndt_1��
        Interval,   ///< ��������
        S_1,        ///< 1 / S
        Phase       ///< ����
    } value;

    explicit ModeMeasureDuration(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ����� ����� ���������
struct ModeCountPulse
{
    enum E
    {
        Manual,     ///< ����.
        ATC,        ///< A(tC)
        ATC_1       ///< A(TC)
    } value;

    explicit ModeCountPulse(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ������ ����� �������
struct PeriodTimeLabels
{
    enum E
    {
        T_3,        ///< 10-3
        T_4,        ///< 10-4
        T_5,        ///< 10-5
        T_6,        ///< 10-6
        T_7,        ///< 10-7
        T_8         ///< 10-8
    } value;

    explicit PeriodTimeLabels(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ����� �����
struct TimeMeasure
{
    enum E
    {
        _1ms,       ///< 1ms
        _10ms,      ///< 10ms
        _100ms,     ///< 100ms
        _1s,        ///< 1s
        _10s,       ///< 10s
        _100s       ///< 100s
    } value;

    explicit TimeMeasure(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText ();
};

/// ����� ��������
struct NumberPeriods
{
    enum E
    {
        _1,       ///< 1
        _10,      ///< 10
        _100,     ///< 100
        _1K,      ///< 1K
        _10K,     ///< 10K
        _100K     ///< 100K
    } value;

    explicit NumberPeriods(E v) : value(v) {};
    operator int() { return (int)value; };
    pString ToText (); 
};


class PageModes
{
public:
    static Page *self;
    /// ��� ���������
    static TypeMeasure typeMeasure;
    /// ���������
    static Hint hint;
    /// ����� ��������� �������
    static ModeMeasureFrequency modeMeasureFrequency;
    /// ����� ��������� �������
    static ModeMeasurePeriod modeMeasurePeriod;
    /// ����� ��������� ������������
    static ModeMeasureDuration modeMeasureDuration;
    /// ����� ����� ���������
    static ModeCountPulse modeCountPulse;
    /// ������ ����� �������
    static PeriodTimeLabels periodTimeLabels;
    /// ����� �����
    static TimeMeasure timeMeasure;
    /// ����� �������� ���������
    static NumberPeriods numberPeriods;
};
