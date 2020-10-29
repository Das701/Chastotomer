#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageModesA.h"


struct TypeMeasureC : public Enumeration
{
    enum E
    {
        Frequency,      // Измерение частоты
        CountPulse,     // Подсчёт импульсов
        Count
    };

    explicit TypeMeasureC(E v) : Enumeration((uint8)v) {};

    bool IsFrequency() const  { return (value == Frequency); }
    bool IsCountPulse() const { return (value == CountPulse); }
};

/// Режим измерения частоты
struct ModeMeasureFrequencyC : public Enumeration
{
    enum E
    {
        Freq,       // Частота
        CA,         // f(C) / f(A)
        CB          // f(C) / f(B)
    };

    explicit ModeMeasureFrequencyC(E v) : Enumeration((uint8)v) {};
    bool IsFrequency() const { return (value == Freq); }
    bool IsCA()        const { return (value == CA); }
    bool IsCB()        const { return (value == CB); }
};

/// Режим счёта импульсов
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CtA,      // C(tA)
        CtB,      // C(tB)
        CTA,      // C(TA)
        CTB       // C(TB)
//        StartStop
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
};


class PageModesC
{
public:
    static Page *self;

    static void Init();
    // Тип измерения
    static TypeMeasureC typeMeasure;
    // Режим измерения частоты
    static ModeMeasureFrequencyC modeMeasureFrequency;
    // Режим счёта импульсов
    static ModeMeasureCountPulseC modeMeasureCountPulse;
    // Период меток времени
    static PeriodTimeLabels periodTimeLabels;
    // Время счета
    static TimeMeasure timeMeasure;
    // Число периодов измерения
    static NumberPeriods numberPeriods;
};
