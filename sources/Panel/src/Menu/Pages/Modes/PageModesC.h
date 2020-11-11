#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"


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


// Режим счёта импульсов
struct ModeMeasureCountPulseC : public Enumeration
{
    enum E
    {
        CtA,      // C(tA)
        CtB,      // C(tB)
        CTA,      // C(TA)
        CTB       // C(TB)
    };

    explicit ModeMeasureCountPulseC(E v) : Enumeration((uint8)v) {};
};


struct PageModesC
{
    static Page *self;

    // Сброс режима измерения
    static void ResetModeCurrentMeasure();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasureC           typeMeasure;           // Тип измерения
    static ModeMeasureFrequency   modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasureCountPulseC modeMeasureCountPulse; // Режим счёта импульсов
    static PeriodTimeLabels       periodTimeLabels;      // Период меток времени
    static TimeMeasure            timeMeasure;           // Время счета
    static NumberPeriods          numberPeriods;         // Число периодов измерения
};
