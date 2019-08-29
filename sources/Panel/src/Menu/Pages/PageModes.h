#pragma once
#include "Menu/MenuItems.h"


struct MeasureFrequency
{
    enum E
    {
        Freq,       ///< �������
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< ��������
    };
};

struct MeasurePeriod
{
    enum E
    {
        Period,     ///< ������
        F_1         ///< T = 1 / f
    };
};

struct MeasureDuration
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
};

struct TimeLabelLenght
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
};

struct CountPulse
{
    enum E
    { 
        Manual,     ///< ����.
        ATC,        ///< A(tC)
        ATC_1       ///< A(TC)
    };
};

struct TimeMeasure
{
    enum E
    {
        MSec_1,     ///< 1ms
        MSec_10,    ///< 10ms
        MSec_100,   ///< 100ms
        Sec_1,      ///< 1s
        Sec_10,     ///< 10s
        Sec_100     ///< 100s
    };
};

struct NumberPeriods
{
    enum E
    {
        NP_1,       ///< 1
        NP_10,      ///< 10
        NP_100,     ///< 100
        NP_1K,      ///< 1K
        NP_10K,     ///< 10K
        NP_100K     ///< 100K
    };
};


class PageModes
{
public:
    static Page *self;
    /// ���������� ����� ��������� �������
    static MeasureFrequency::E ModeMeasureFrequency();
    /// ���������� ����� ��������� �������
    static MeasurePeriod::E ModeMeasurePeriod();
    /// ���������� ����� ��������� ������������
    static MeasureDuration::E ModeMeasureDuration();
    /// ���������� ������������ ��������� �����
    static TimeLabelLenght::E ModeTimeLabelLenght();
    /// ���������� ���� ����� ���������
    static CountPulse::E ModeCountPulse();
    /// ���������� ����� �����
    static TimeMeasure::E ModeTimeMeasure();
    /// ���������� ����� �������� ���������
    static NumberPeriods::E ModeNumberPeriods();
};
