#pragma once
#include "Menu/MenuItems.h"


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

    static void PressSetup();

    // Сброс режима измерения
    static void ResetModeCurrentMeasure();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasure            typeMeasure;           // Тип измерения
    static ModeMeasureFrequency   modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasurePeriod      modeMeasurePeriod;     // Режим измерения периода
    static ModeMeasureDuration    modeMeasureDuration;   // Режим измерения длительности
    static ModeMeasureCountPulseB modeMeasureCountPulse; // Режим счёта импульсов
    static PeriodTimeLabels       periodTimeLabels;      // Период меток времени
    static TimeMeasure            timeMeasure;           // Время счета
    static NumberPeriods          numberPeriods;         // Число периодов измерения
};
