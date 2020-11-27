#pragma once
#include "Menu/MenuItems.h"


struct DisplayTime : public Enumeration
{
    enum E
    {
        _100ms,             // 0.1s.
        _1s,                // 1s.
        _10s                // 10s.
    };

    explicit DisplayTime(E v) : Enumeration((uint8)v) {};
    bool Is_1s() const {  return (value == _1s); }
    bool Is_10s() const { return (value == _10s); }
};

struct RefGenerator : public Enumeration
{
    enum E
    {
        Internal,           // ����������
        External            // �������
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }

    static void LoadToFPGA();
};


struct LaunchSource : public Enumeration
{
    enum E
    {
        Internal,           // ����������
        External,           // �������
        OneTime             // �����������
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }
    bool IsOneTime() const  { return (value == OneTime); }

    static void LoadToFPGA();
};


struct MemoryMode : public Enumeration
{
    enum E
    { 
        Off,                // ����
        On                  // ���
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
    bool IsOn() const { return (value == On); }

    static void Load();
};


struct Calibration : public Enumeration
{
    enum E
    {
        Unpressed,          // ��������� �� � ������ ���������� - ����� ��������� ������� �������������
        Pressed             // ��������� � ������ ���������� - ����� ��������� �����������
    };

    explicit Calibration(E v) : Enumeration((uint8)v) {};
    bool IsPressed() const { return (value == Pressed); }
};


class PageIndication
{
public:
    static MenuPage *self;
    // ����� ����������� ���������� ���������
    static DisplayTime displayTime;
    // ����� �������� ����������
    static RefGenerator refGenerator;
    // ����� ��������� �������
    static LaunchSource launchSource;
    // ����� ��������� �������
    static Calibration calibration;
    // ���������(����������) ������ ������
    static MemoryMode memoryMode;
    static bool OnceLaunch();
    static void OnceLaunchSwitchTrue();
    static void OnceLaunchSwitchFalse();
};
