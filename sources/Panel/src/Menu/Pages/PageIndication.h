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
        Internal,               ///< Внутренний
        External                ///< Внешний
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
};

struct LaunchSource : public Enumeration
{
    enum E
    {
        LS_Internal,            ///< Внутренний
        LS_External,            ///< Внешний
        LS_OneTime              ///< Однократный
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
};

struct MemoryMode : public Enumeration
{
    enum E
    {
        M_On,                   ///< Вкл
        M_Off                   ///< Выкл
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
};

struct Calibration : public Enumeration
{
    enum E
    {
        Unpressed,              ///< Не нажата 
        Pressed                 ///< Нажата
    };

    explicit Calibration(E v) : Enumeration((uint8)v) {};
};

class PageIndication
{
public:
    static Page *self;
    /// Время отображения результата измерения
    static DisplayTime displayTime;
    /// Выбор опорного генератора
    static RefGenerator refGenerator;
    /// Выбор источника запуска
    static LaunchSource launchSource;

    /// Выбор источника запуска
    static Calibration calibration;
    /// Включение(отключение) режима памяти
    static MemoryMode memoryMode;
};
