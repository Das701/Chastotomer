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
        Holdoff,        // Holdoff
        Count
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    // Загрузить в аппаратуру
    static void LoadToFPGA();

    // Тип синхронизации на текущем канале
    static const TypeSynch &Current();
};


// ФНЧ
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // Включен
        Off,            // Выключен
        Count
    };

    explicit ModeFilter(E v) : Enumeration((uint8)v) {};
    static void Set(E v);
    bool IsOff() const { return (value == ModeFilter::Off); }

    static const ModeFilter &Current();
    static void LoadToFPGA();
};


namespace FreqMeter
{
    extern ModeTest modeTest;

    void LoadCalibration();
    
    void LoadDisplayTime();
   
    void LoadAuto();
    
    void UnloadAuto();
    
    void LoadOneTime();
    
    void LoadStartStop();
}
