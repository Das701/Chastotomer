#pragma once
#include "Menu/MenuItems.h"


struct DisplayTime : public Enumeration
{
    enum E
    {
        _100ms,             ///< 0.1s.
        _1s,                ///< 1s.
        _10s                ///< 10s.
    };

    explicit DisplayTime(E v) : Enumeration((uint8)v) {};
};

struct RefGenerator : public Enumeration
{
    enum E
    {
        Internal,           ///< ����������
        External            ///< �������
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
};

struct LaunchSource : public Enumeration
{
    enum E
    {
        Internal,           ///< ����������
        External,           ///< �������
        OneTime             ///< �����������
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
};

struct MemoryMode : public Enumeration
{
    enum E
    { 
        Off,                ///< ����
        On                  ///< ���
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
};

struct Calibration : public Enumeration
{
    enum E
    {
        Unpressed,          ///< �� ������ 
        Pressed             ///< ������
    };

    explicit Calibration(E v) : Enumeration((uint8)v) {};
};

class PageIndication
{
public:
    static Page *self;
    /// ����� ����������� ���������� ���������
    static DisplayTime displayTime;
    /// ����� �������� ����������
    static RefGenerator refGenerator;
    /// ����� ��������� �������
    static LaunchSource launchSource;
    /// ����� ��������� �������
    static Calibration calibration;
    /// ���������(����������) ������ ������
    static MemoryMode memoryMode;
};
