#pragma once
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


struct CurrentPageModes
{
    // Возвращает true, если текущая страница режимов содержит время измерения
    static bool ConsistTimeMeasure();
};


// Здесь функции общие для всех страниц режимов каналов
class PageModes
{
public:

    static void OnChanged_TypeMeasure();

    static void DrawParameters(int x, int y);

    static Page *Current();
};


class PageModesA
{
public:

    static Page *self;
    
    static void RelationOn();
    static void RelationOff();
    static bool RelationCheck();

    static void PressSetup();
    static bool StartStop();
    static void ToggleStartStop();

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
};


struct PageModesB
{
    static Page *self;

    static void PressSetup();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // Тип измерения
    static ModeMeasureFrequency  modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasurePeriod     modeMeasurePeriod;     // Режим измерения периода
    static ModeMeasureDuration   modeMeasureDuration;   // Режим измерения длительности
    static ModeMeasureCountPulse modeMeasureCountPulse; // Режим счёта импульсов
};


struct PageModesC
{
    static Page *self;

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();

    static TypeMeasure           typeMeasure;           // Тип измерения
    static ModeMeasureFrequency  modeMeasureFrequency;  // Режим измерения частоты
    static ModeMeasureCountPulse modeMeasureCountPulse; // Режим счёта импульсов
};



class PageModesD
{
public:
    static Page *self;

    static TypeMeasure          typeMeasure;
    static ModeMeasureFrequency modeMeasureFrequency;
};
