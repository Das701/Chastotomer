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


// ����� ���� ������������� - ��� ��� ���
struct TypeSynch : public Enumeration
{
    enum E
    {
        Manual,         // ����
        Holdoff,        // Holdoff
        Count
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    // ��������� � ����������
    static void LoadToFPGA();

    // ��� ������������� �� ������� ������
    static const TypeSynch &Current();
};


// ����� �������������
struct ModeFront : public Enumeration
{
    enum E
    {
        Front,          // �����
        Slice           // ����
    };

    explicit ModeFront(E v) : Enumeration((uint8)v) {};
    bool IsFront() const { return (value == Front); }

    static ModeFront &Current();
    static void LoadToFPGA();
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
