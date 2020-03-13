#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasure : public Enumeration
{
    enum E
    {
        Frequency,      ///< Измерение частоты
        Period,         ///< Измерение периода
        Duration,       ///< Измерение длительности
        CountPulse,     ///< Подсчёт импульсов
        Count
    };

    explicit TypeMeasure(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения частоты A
struct ModeMeasureFrequency : public Enumeration
{
    enum E
    {
        Freq,       ///< Частота
        AC,         ///< f(A) / f(C)
        AB,         ///< f(A) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< Тахометр
    };

    explicit ModeMeasureFrequency(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения частоты C
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       ///< Частота
        CA,         ///< f(C) / f(A)
        CB,         ///< f(C) / f(B)
        T_1,        ///< f = 1 / T
        Tachometer  ///< Тахометр
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения частоты B
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       ///< Частота
        BA,         ///< f(B) / f(A)
        BC,         ///< f(B) / f(C)
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения периода
struct ModeMeasurePeriod : public Enumeration
{
    enum E
    {
        Period,     ///< Период
        F_1         ///< T = 1 / f
    };

    explicit ModeMeasurePeriod(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения длительности
struct ModeMeasureDuration : public Enumeration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1,      ///< <ndt>
        Ndt_1ns,    ///< ndt_1нс
        Interval,   ///< Интервал
        S_1,        ///< 1 / S
        Phase       ///< Фаза
    };

    explicit ModeMeasureDuration(E v) : Enumeration((uint8)v) {};
};

/// Режим счёта импульсов
struct ModeMeasureCountPulse : public Enumeration
{
    enum E
    {
        Manual,     ///< Ручн.
        ATC,        ///< A(tC)
        ATC_1       ///< A(TC)
    };

    explicit ModeMeasureCountPulse(E v) : Enumeration((uint8)v) {};
};

/// Период меток времени
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

/// Время счёта
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

/// Число периодов
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
    /// Тип измерения
    static TypeMeasure typeMeasure;
    /// Режим измерения частоты
    static ModeMeasureFrequency modeMeasureFrequency;
    /// Режим измерения частоты C
    static ModeMeasureFrequencyC modeMeasureFrequencyC;
    /// Режим измерения частоты B
    static ModeMeasureFrequencyB modeMeasureFrequencyB;
    /// Режим измерения периода
    static ModeMeasurePeriod modeMeasurePeriod;
    /// Режим измерения длительности
    static ModeMeasureDuration modeMeasureDuration;
    /// Режим счёта импульсов
    static ModeMeasureCountPulse modeMeasureCountPulse;
    /// Период меток времени
    static PeriodTimeLabels periodTimeLabels;
    /// Время счета
    static TimeMeasure timeMeasure;
    /// Число периодов измерения
    static NumberPeriods numberPeriods;
};
