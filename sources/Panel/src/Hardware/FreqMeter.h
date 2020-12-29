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

namespace FreqMeter
{
    extern ModeTest modeTest;

    void LoadCalibration();
    
    void LoadDisplayTime();
   
    void LoadAuto();
    
    void UnloadAuto();
    
    void LoadOneTime();

    void HardwareReset();
}
