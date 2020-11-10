#pragma once
#include "Menu/MenuItems.h"
#include "Utils/Observer.h"


// Включён или выключен режим "Тест"
struct ModeTest : public Enumeration, public Subject
{
    enum E
    {
        Disabled,
        Enabled
    };

    explicit ModeTest(E v) : Enumeration((uint8)v) {};

    bool IsEnabled() const { return value == Enabled; }

    // Переключить режим теста
    void Switch();
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


// Фронт синхронизации
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          // Фронт
        Slice           // Срез
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
    static ModeFront &Current();
    bool IsFront() const { return (value == Front); }

    static void Load();
};


// ФНЧ
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // Включен
        Off             // Выключен
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
    static void Load();
    static ModeFilter &Current();
    static void Set(E v);
    bool IsOff() const { return (value == ModeFilter::Off); }
};


namespace FreqMeter
{
    extern ModeTest modeTest;

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

    void LoadDivider();

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
