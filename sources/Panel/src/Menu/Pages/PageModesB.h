#pragma once
#include "Menu/MenuItems.h"


/// Режим измерения частоты
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Freq,       // Частота
        BA,         // f(B) / f(A)
        BC,         // f(B) / f(C)
        T_1,        // f = 1 / T
        Tachometer  // Тахометр
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency()  const { return (value == Freq); }
    bool IsBA()         const { return (value == BA); }
    bool IsBC()         const { return (value == BC);  }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
};

/// Режим счёта импульсов
struct ModeMeasureCountPulseB : public Enumeration
{
    enum E
    {
        BtA,        // B(tA)
        BTA_1,      // B(TA)
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
    // Тип измерения
    static TypeMeasureAB typeMeasure;
    // Режим измерения частоты
    static ModeMeasureFrequencyB modeMeasureFrequency;
    // Режим измерения периода
    static ModeMeasurePeriod modeMeasurePeriod;
    // Режим измерения длительности
    static ModeMeasureDuration modeMeasureDuration;
    // Режим счёта импульсов
    static ModeMeasureCountPulseB modeMeasureCountPulse;
    // Период меток времени
    static PeriodTimeLabels periodTimeLabels;
    // Время счета
    static TimeMeasure timeMeasure;
    // Число периодов измерения
    static NumberPeriods numberPeriods;
};
