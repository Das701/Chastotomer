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
    static void LoadToFPGA();
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

    static void LoadToFPGA();
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
    static void LoadToFPGA();
    static ModeFilter &Current();
    static void Set(E v);
    bool IsOff() const { return (value == ModeFilter::Off); }
};


namespace FreqMeter
{
    extern ModeTest modeTest;

    // Загрузка периода меток времени
    void LoadPeriodTimeLabels();

    // Загрузка времени измерения
    void LoadTimeMeasure();

    // Загрузка числа периодов измерения
    void LoadNumerPeriodsMeasure();

    void LoadCalibration();
    
    void LoadDisplayTime();
   
    void LoadAuto();
    
    void LoadMeasure();
    
    void UnloadAuto();
    
    void LoadOneTime();
    
    void LoadStartStop();
}
