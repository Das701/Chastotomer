#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/PageModesA.h"
#include "Menu/Pages/Modes/PageModesB.h"
#include "Menu/Pages/Modes/PageModesC.h"
#include "Menu/Pages/Modes/PageModesD.h"


struct CurrentTypeMeasure
{
    static int Value();
    static bool IsFrequency();
    static bool IsPeriod();
    static bool IsDuration();
    static bool IsCountPulse();
    static Enumeration &ToEnumeration();
    static void Draw(int x, int y);

};


struct CurrentModeMeasure
{
    static Enumeration &ToEnumeration();
    static void Draw(int x, int y);
    static void DrawParameters(int x, int y);
};


struct CurrentModeMeasurePeriod
{
    static void LoadToFPGA();
};


struct CurrentModeMeasureDuration
{
    static void LoadToFPGA();
};


struct CurrentModeMeasureCountPulse
{
    static bool IsBig_T();
    static void LoadToFPGA();
};


// Здесь функции общие для всех страниц режимов каналов
struct PageModes
{
    // Сброс режима текущего измерения
    static void ResetModeCurrentMeasure();
};
