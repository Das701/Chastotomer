#pragma once
#include "Menu/MenuItems.h"


struct DisplayTime : public Enumeration
{
    enum E
    {
        S_001,                  ///< 0.1s.
        S_010,                  ///< 1s.
        S_100                   ///< 10s.
    };

    explicit DisplayTime(E v) : Enumeration((uint8)v) {};
};

struct RefGenerator : public Enumeration
{
    enum E
    {
        Internal,               ///< ����������
        External                ///< �������
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
};

struct LaunchSource : public Enumeration
{
    enum E
    {
        LS_Internal,            ///< ����������
        LS_External,            ///< �������
        LS_OneTime              ///< �����������
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
};

struct MemoryMode : public Enumeration
{
    enum E
    {
        M_On,                   ///< ���
        M_Off                   ///< ����
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
};

struct Calibration : public Enumeration
{
    enum E
    {
        Unpressed,              ///< �� ������ 
        Pressed                 ///< ������
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
