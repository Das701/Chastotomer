#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"


class PageModesA
{
friend struct PageModes;

public:

    static Page *self;
    
    static void InterpolateOn();
    static void InterpolateOff();
    static bool InterpolateCheck();

    static void DCycleOn();
    static void DCycleOff();
    static bool DCycleCheck();

    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

    // Сброс режима измерения
    static void ResetModeCurrentMeasure();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();
    
    static TypeMeasure           typeMeasure;                // Тип измерения
    static ModeMeasureFrequency  modeMeasureFrequency;       // Режим измерения частоты
    static ModeMeasurePeriod     modeMeasurePeriod;          // Режим измерения периода
    static ModeMeasureDuration   modeMeasureDuration;        // Режим измерения длительности
    static ModeMeasureCountPulse modeMeasureCountPulse;      // Режим счёта импульсов
    static PeriodTimeLabels      periodTimeLabels;           // Период меток времени
    static TimeMeasure           timeMeasure;                // Время счета
    static NumberPeriods         numberPeriods;              // Число периодов измерения
};
