#pragma once
#include "Menu/MenuItems.h"


// Включён или выключен режим "Тест"
struct ModeTest : public Enumeration
{
    enum E
    {
        Disabled,
        Enabled
    };

    explicit ModeTest(E v) : Enumeration((uint8)v) {};

    static bool IsEnabled();

    // Переключить режим теста
    static void Switch();
};


// Выбор типа синхронизации - ТТЛ или ЭСЛ
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         // Ручн
        Holdoff         // Holdoff
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    // Загрузить в аппаратуру
    static void Load();
};


namespace FreqMeter
{
    // Загрузка режима измерения частоты
    void LoadModeMeasureFrequency();

    // Загрузка режима измерения периода
    void LoadModeMeasurePeriod();

    // Загрузка режима измерения длительности
    void LoadModeMeasureDuration();

    // Загрузка режима измерения счёта импульсов
    void LoadModeMeasureCountPulse();

    // Загрузка периода меток времени
    void LoadPeriodTimeLabels();

    // Загрузка времени измерения
    void LoadTimeMeasure();

    // Загрузка числа периодов измерения
    void LoadNumerPeriodsMeasure();

    // Загрузка входного сопротивления по текущему каналу
    void LoadImpedance();

    void LoadModeFront();

    void LoadDivider();

    void LoadChannel();
    
    void LoadMemoryMode();
    
    void LoadCalibration();
    
    void LoadLaunchSource();
    
    void LoadRefGenerator();
    
    void LoadDisplayTime();
   
    void LoadAuto();
    
    void LoadMeasure();
    
    void UnloadAuto();
    
    void LoadOneTime();
    
    void LoadStartStop();
}
