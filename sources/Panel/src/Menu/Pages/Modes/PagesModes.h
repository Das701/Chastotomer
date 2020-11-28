#pragma once
#include "Settings.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


extern Page pageModesA;
extern Page pageModesB;
extern Page pageModesC;
extern Page pageModesD;


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

    static ModeCountPulse modeMeasureCountPulse;      // Режим счёта импульсов
};


struct PageModesB
{
    static void PressSetup();

    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModePeriod();
    static void OnChanged_ModeDuration();
    static void OnChanged_ModeCountPulse();

    static ModeCountPulse modeMeasureCountPulse; // Режим счёта импульсов
};


struct PageModesC
{
    // Вызывается при измеенении вида измерения
    static void OnChanged_TypeMeasure();

    // Вызываются при изменении режимов измерения
    static void OnChanged_ModeFrequency();
    static void OnChanged_ModeCountPulse();

    static ModeCountPulse modeMeasureCountPulse; // Режим счёта импульсов
};
