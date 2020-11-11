#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"


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
    
    static TypeMeasure           typeMeasure;           // Тип измерения
    static ModeMeasureFrequency  modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasureCountPulse modeMeasureCountPulse; // Режим счёта импульсов
    static PeriodTimeLabels      periodTimeLabels;      // Период меток времени
    static TimeMeasure           timeMeasure;           // Время счета
    static NumberPeriods         numberPeriods;         // Число периодов измерения
};
