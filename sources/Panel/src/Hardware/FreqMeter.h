#pragma once
#include "Menu/MenuItems.h"
#include "Utils/Observer.h"


// ������� ��� �������� ����� "����"
struct ModeTest : public Enumeration, public Subject
{
    enum E
    {
        Disabled,
        Enabled
    };

    explicit ModeTest(E v) : Enumeration((uint8)v) {};

    bool IsEnabled() const { return value == Enabled; }

    // ����������� ����� �����
    void Switch();
};


// ���
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // �������
        Off,            // ��������
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
