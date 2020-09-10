#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasureB : public Enumeration
{
    enum E
    {
        Frequency,      ///< Измерение частоты
        Period,         ///< Измерение периода
        Duration,       ///< Измерение длительности
        CountPulse,     ///< Подсчёт импульсов
        Count
    };

    explicit TypeMeasureB(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения частоты
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       ///< Частота
        BA,         ///< f(B) / f(A)
        BC,         ///< f(B) / f(C)
        T_1,        ///< f = 1 / T
        Tachometer  ///< Тахометр
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения периода
struct ModeMeasurePeriodB : public Enumeration
{
    enum E
    {
        Period,     ///< Период
        F_1         ///< T = 1 / f
    };

    explicit ModeMeasurePeriodB(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения длительности
struct ModeMeasureDurationB : public Enumeration
{
    enum E
    {
        Ndt,        ///< ndt
        Ndt_1ns,    ///< ndt_1нс
        Ndt2,       ///< ndt2
        Dcycle,     ///< Скважность
        Phase       ///< Фаза
    };

    explicit ModeMeasureDurationB(E v) : Enumeration((uint8)v) {};
};

/// Режим счёта импульсов
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

/// Период меток времени
struct PeriodTimeLabelsB : public Enumeration
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

    explicit PeriodTimeLabelsB(E v) : Enumeration((uint8)v) {};
};

/// Время счёта
struct TimeMeasureB : public Enumeration
{
    enum E
    {
        _1ms,       ///< 1ms
        _10ms,      ///< 10ms
        _100ms,     ///< 100ms
        _1s,        ///< 1s
        _10s,       ///< 10s
        _100s,       ///< 100s
        _1000s
    };

    explicit TimeMeasureB(E v) : Enumeration((uint8)v) {};
};

/// Число периодов
struct NumberPeriodsB : public Enumeration
{
    enum E
    {
        _1,       ///< 1
        _10,      ///< 10
        _100,     ///< 100
        _1K,      ///< 1K
        _10K,     ///< 10K
        _100K,     ///< 100K
        _1000K
    };

    explicit NumberPeriodsB(E v) : Enumeration((uint8)v) {};
};


class PageModesB
{
public:
    static Page *self;
    static void Init();
    static void PressSetupB();
    /// Тип измерения
    static TypeMeasureB typeMeasureB;
    /// Режим измерения частоты
    static ModeMeasureFrequencyB modeMeasureFrequencyB;
    /// Режим измерения периода
    static ModeMeasurePeriodB modeMeasurePeriodB;
    /// Режим измерения длительности
    static ModeMeasureDurationB modeMeasureDurationB;
    /// Режим счёта импульсов
    static ModeMeasureCountPulseB modeMeasureCountPulseB;
    /// Период меток времени
    static PeriodTimeLabelsB periodTimeLabelsB;
    /// Время счета
    static TimeMeasureB timeMeasureB;
    /// Число периодов измерения
    static NumberPeriodsB numberPeriodsB;
};
