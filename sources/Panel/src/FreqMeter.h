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
        Holdoff         // Holdoff
    };

    explicit TypeSynch(E v) : Enumeration((uint8)v) {};
    bool IsHoldoff() const { return (value == Holdoff); }
    bool IsManual() const { return (value == Manual); }

    // ��������� � ����������
    static void Load();
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
    static ModeFront &Current();
    bool IsFront() const { return (value == Front); }

    static void Load();
};


// ���
struct ModeFilter : public Enumeration
{
    enum E
    {
        On,             // �������
        Off             // ��������
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

    // �������� ������ ��������� �������
    void LoadModeMeasureFrequency();

    // �������� ������ ��������� �������
    void LoadModeMeasurePeriod();

    // �������� ������ ��������� ������������
    void LoadModeMeasureDuration();

    // �������� ������ ��������� ����� ���������
    void LoadModeMeasureCountPulse();

    // �������� ������� ����� �������
    void LoadPeriodTimeLabels();

    // �������� ������� ���������
    void LoadTimeMeasure();

    // �������� ����� �������� ���������
    void LoadNumerPeriodsMeasure();

    // �������� �������� ������������� �� �������� ������
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
