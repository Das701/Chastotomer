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


struct CurrentModeMeasureFrequency
{
    static bool Is_RatioAB_or_RatioBA();
    static bool Is_RatioCA_or_RatioCB();
    static bool Is_Ratio();

    // Загрузить текущий режим измерения частоты в аппаратуру
    static void LoadToFPGA();
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
