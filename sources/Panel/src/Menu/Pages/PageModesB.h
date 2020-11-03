#pragma once
#include "Menu/MenuItems.h"


// Режим измерения частоты
struct ModeMeasureFrequencyB : public Enumeration
{
    enum E
    {
        Frequency,  // Частота
        BA,         // f(B) / f(A)
        BC,         // f(B) / f(C)
        T_1,        // f = 1 / T
        Tachometer  // Тахометр
    };

    explicit ModeMeasureFrequencyB(E v) : Enumeration((uint8)v) {};
    bool IsFrequency()  const { return (value == Frequency); }
    bool IsBA()         const { return (value == BA); }
    bool IsBC()         const { return (value == BC);  }
    bool IsT_1()        const { return (value == T_1); }
    bool IsTachometer() const { return (value == Tachometer); }
};

// Режим счёта импульсов
struct ModeMeasureCountPulseB : public Enumeration
{
    enum E
    {
        BtA,        // B(tA)
        BTA,        // B(TA)
        StartStop
    };

    explicit ModeMeasureCountPulseB(E v) : Enumeration((uint8)v) {};
};


struct PageModesB
{
    static Page *self;

    static void Init();
    static void PressSetupB();

    // Сброс режима измерения
    static void ResetModeCurrentMeasure();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasureAB          typeMeasure;           // Тип измерения
    static ModeMeasureFrequencyB  modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasurePeriod      modeMeasurePeriod;     // Режим измерения периода
    static ModeMeasureDuration    modeMeasureDuration;   // Режим измерения длительности
    static ModeMeasureCountPulseB modeMeasureCountPulse; // Режим счёта импульсов
    static PeriodTimeLabels       periodTimeLabels;      // Период меток времени
    static TimeMeasure            timeMeasure;           // Время счета
    static NumberPeriods          numberPeriods;         // Число периодов измерения
};
