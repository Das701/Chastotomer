#pragma once
#include "Menu/MenuItems.h"


struct TypeMeasureC : public Enumeration
{
    enum E
    {
        Frequency,      ///< Измерение частоты
        CountPulse,     ///< Подсчёт импульсов
        Count
    };

    explicit TypeMeasureC(E v) : Enumeration((uint8)v) {};
};

/// Режим измерения частоты
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       ///< Частота
        CA,         ///< f(C) / f(A)
        CB          ///< f(C) / f(B)
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
};

/// Режим счёта импульсов
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CTA,        ///< C(tA)
        CTB,        ///< C(tB)
        CTA_1,      ///< C(TA)
        CTB_1       ///< C(TB)
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
};

/// Период меток времени
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

/// Время счёта
struct TimeMeasureC : public Enumeration
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

    explicit TimeMeasureC(E v) : Enumeration((uint8)v) {};
};

/// Число периодов
struct NumberPeriodsC : public Enumeration
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

    explicit NumberPeriodsC(E v) : Enumeration((uint8)v) {};
};


class PageModesC
{
public:
    static Page *self;

    static void Init();
    /// Тип измерения
    static TypeMeasureC typeMeasureC;
    /// Режим измерения частоты
    static ModeMeasureFrequencyC modeMeasureFrequencyC;
    /// Режим счёта импульсов
    static ModeMeasureCountPulseC modeMeasureCountPulseC;
    /// Период меток времени
    static PeriodTimeLabelsC periodTimeLabelsC;
    /// Время счета
    static TimeMeasureC timeMeasureC;
    /// Число периодов измерения
    static NumberPeriodsC numberPeriodsC;
};
